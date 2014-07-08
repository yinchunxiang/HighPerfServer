/*
 * akg.c
 *	
 */

#include "akg.h"
#include "ptcl.h"
#include "collect.h"
#include "hiredis.h"
#include "json/json.h"
#include "base64.h"
#include "md5.h"
#include "conf.h"

const size_t kMaxRespSize = 512;
const size_t kHeadSize = sizeof(HEAD);

void compute_hash(char* data, size_t size, char* out_buf) {
    char* invert_data = new char[size];
    if (NULL == invert_data) {
        return;
    }

    for(size_t i = 0; i < size; ++i) {
        invert_data[i] = data[size - 1 - i];
    }

    std::string strbase64 = Base64Tool::easy_convert_to_base64(invert_data, size);

    MD5_CTX md5_context;
    MD5Init(&md5_context);
    const unsigned char* p = reinterpret_cast<const unsigned char*>(strbase64.c_str());
    MD5Update(&md5_context, p, strbase64.size());
    
    MD5Final(&md5_context, (unsigned char*)out_buf);
    return;
}

inline void send_data(CONN* c, const std::string& data) {
    memcpy(c->out_buf, c->in_buf, kHeadSize);
    memcpy(c->out_buf + kHeadSize, data.c_str(), data.size());
    HEAD* resp_head = (HEAD*) c->out_buf;
    resp_head->pkglen = kHeadSize + data.size();
    bufferevent_write(c->bufev, c->out_buf, resp_head->pkglen);
}

int report_upgrade(CONN* c) {
    slog_info_t_w(c->owner->qlog, "report_upgrade");
    HEAD* req_head = (HEAD*) c->in_buf;

    return 0;
}

int check_upgrade(CONN* c)  {
    HEAD* req_head = (HEAD*) c->in_buf;
    
    Json::Value root;
    Json::Features features = Json::Features::strictMode();
    Json::FastWriter writer;
    if (req_head->version < ver->intver[ver->vernum - 1]) {
        root["status"] = 1;
        root["statusDesc"] = "Need upgrade";
        root["newVersion"] = ver->strver[ver->vernum - 1];
        root["updateDesc"] = "README";
    }
    else {
        root["status"] = 0;
    }

    std::string strjson = writer.write(root);
    send_data(c, strjson);
    return 0;
}

int admin_user_code(CONN* c) {
    HEAD* req_head = (HEAD*)c->in_buf;
    HEAD* resp_head = (HEAD*)c->out_buf;

    Json::Value root;
    Json::Reader reader;
    Json::Features features = Json::Features::strictMode();
    char* pdata = c->in_buf + kHeadSize;
    std::string req_data = std::string(pdata, req_head->pkglen - kHeadSize);
    bool ret = reader.parse(req_data, root, false);
    if (!ret) {
        slog_err_t_w(c->owner->qlog, "json parse failed");
        return -1;
    }

    if (root["Hash"].isNull()) {
        slog_err_t_w(c->owner->qlog, "root[Hash] does not exist");
        return -1;
    }

    std::string inhash = root["Hash"].asString();
    char out_buf[16];
    compute_hash(c->in_buf, kHeadSize, out_buf);
    std::string outhash = std::string(out_buf, 16);

    Json::Value ret_data;
    Json::FastWriter writer;
    if (inhash != outhash) {
        ret_data["status"] = 2;
        ret_data["statusDesc"] = "hash inconsistent";
        std::string strjson = writer.write(ret_data);
        send_data(c, strjson);
        return 0;
    }

    struct timeval timeout = {2, 0};    //2s的超时时间
    redisContext *pRedisContext = (redisContext*)redisConnectWithTimeout("127.0.0.1", 6379, timeout);
    if ( (NULL == pRedisContext) || (pRedisContext->err) )
    {   
        if (pRedisContext)
        {   
            slog_err_t_w(c->owner->qlog, "connect error:%s", pRedisContext->errstr);
        }   
        else
        {   
            slog_info_t_w(c->owner->qlog, "connect error: can't allocate redis context.");
        }   
        ret_data["status"] = 2;
        ret_data["statusDesc"] = "other error";
        std::string strjson = writer.write(ret_data);
        send_data(c, strjson);
        return 0; 
    }   

    redisReply *pRedisReply = (redisReply*)redisCommand(pRedisContext, "hget userinfo %b", req_head->user_code, 64);  
    if (NULL == pRedisReply) {
        slog_err_t_w(c->owner->qlog, "%s", pRedisContext->errstr);
        redisFree(pRedisContext);
        ret_data["status"] = 2;
        ret_data["statusDesc"] = "other error";
        std::string strjson = writer.write(ret_data);
        send_data(c, strjson);
        return 0; 
    }   

    switch (pRedisReply->type) {
        case REDIS_REPLY_NIL: 
            {   
                freeReplyObject(pRedisReply);   
                pRedisReply = (redisReply*)redisCommand(pRedisContext, "hset userinfo %b unused", req_head->user_code, 64);
                if (REDIS_REPLY_INTEGER == pRedisReply->type) {
                    switch(pRedisReply->integer) {
                        case 0: //already exist
                            {
                                ret_data["status"] = 2;
                                ret_data["statusDesc"] = "already stored";
                            }
                            break;
                        case 1: // set success
                            {
                                ret_data["status"] = 0;
                                ret_data["statusDesc"] = "set success";
                            }
                            break;
                        default:
                            slog_err_t_w(c->owner->qlog, "hset should not be here");
                            break;

                    }
                }   
                else {
                    ret_data["status"] = 2;
                    ret_data["statusDesc"] = "other error";
                    std::string strjson = writer.write(ret_data);
                    send_data(c, strjson);
                }   

            }   
            break;

        case REDIS_REPLY_ERROR:
            {   
                ret_data["status"] = 2;
                ret_data["statusDesc"] = "other error";
            }   
            break;

        case REDIS_REPLY_STRING:
            {   
                if (strncmp("unused", pRedisReply->str, 6)) {
                    ret_data["status"] = 2;
                    ret_data["statusDesc"] = "already stored";
                }   
                else {
                    ret_data["status"] = 1;
                    ret_data["statusDesc"] = "already used";
                }   
            }   
            break;
        default:
            slog_err_t_w(c->owner->qlog, "hget should not be here");
            break;
    }   
    std::string strjson = writer.write(ret_data);
    send_data(c, strjson);
    freeReplyObject(pRedisReply);   
    redisFree(pRedisContext);

    return 0;
}

int check_user_code(CONN* c) {
    HEAD* head = (HEAD*)c->in_buf;
    HEAD* resp = (HEAD*) c->out_buf;
    char* data_buf = c->out_buf + sizeof(HEAD);
    size_t data_size = 0;

    slog_info_t_w(c->owner->qlog, "check_user_code");
    struct timeval timeout = {2, 0};    //2s的超时时间
    //redisContext是Redis操作对象
    redisContext *pRedisContext = (redisContext*)redisConnectWithTimeout("127.0.0.1", 6379, timeout);
    if ( (NULL == pRedisContext) || (pRedisContext->err) )
    {
        if (pRedisContext)
        {
            slog_err_t_w(c->owner->qlog, "connect error:%s", pRedisContext->errstr);
        }
        else
        {
            slog_info_t_w(c->owner->qlog, "connect error: can't allocate redis context.");
        }
        return -1;
    }
    //redisReply是Redis命令回复对象 redis返回的信息保存在redisReply对象中
    redisReply *pRedisReply = (redisReply*)redisCommand(pRedisContext, "hget userinfo %b", head->user_code, 64);  
    if (NULL == pRedisReply) {
        slog_err_t_w(c->owner->qlog, "%s", pRedisContext->errstr);
        redisFree(pRedisContext);
        return -1;
    }  

    switch (pRedisReply->type) {
        case REDIS_REPLY_NIL: 
            {
                freeReplyObject(pRedisReply);   
                pRedisReply = (redisReply*)redisCommand(pRedisContext, "hset userinfo %b %b", 
                        head->user_code, 64, head->machine_code, 64);
                if (REDIS_REPLY_ERROR != pRedisReply->type) {
                    snprintf(data_buf, kMaxRespSize, "{\"status\":1, \"statusDesc\":\"success\"}");
                }
                else {
                    snprintf(data_buf, kMaxRespSize, "{\"status\":0, \"statusDesc\":\"failed\"}");
                }

            }
            break;

        case REDIS_REPLY_ERROR:
            {
                snprintf(data_buf, kMaxRespSize, "{\"status\":0, \"statusDesc\":\"failed\"}");
            }
            break;
        case REDIS_REPLY_STRING:
            {
                if (strncmp(head->machine_code, pRedisReply->str, 64)) {
                    snprintf(data_buf, kMaxRespSize, "{\"status\":1, \"statusDesc\":\"success\"}");
                }
                else {
                    snprintf(data_buf, kMaxRespSize, "{\"status\":0, \"statusDesc\":\"failed\"}");
                }
            }
            break;
    }
    freeReplyObject(pRedisReply);   
    redisFree(pRedisContext);

    resp->pkglen = sizeof(HEAD) + strlen(data_buf);
    slog_info_t_w(c->owner->qlog, "resp_data:%s", data_buf);
    slog_info_t_w(c->owner->qlog, "resp_data_size:%d", strlen(data_buf));
    bufferevent_write(c->bufev, c->out_buf, resp->pkglen);
    return 0;
}

sint32 logout(CONN* c) {
    if (c->login) {
        c->login = 0;
        --report[c->owner->ind].online[c->verind_4online][c->chid];
        ++report[c->owner->ind].logout[c->verind_4online][c->chid][ind_collect_1];
        slog_info_t(c->owner->qlog, "logout: %u err %u", c->uid, c->err_type);
    }
    return 0;
}

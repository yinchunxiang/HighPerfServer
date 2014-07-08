#include <hiredis/hiredis.h>
#include <iostream>
#include <string>


int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3) {
        printf("Usage: %s COMMAND\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* cmd = argv[1];

    struct timeval timeout = {2, 0};    //2s的超时时间
    //redisContext是Redis操作对象
    redisContext *pRedisContext = (redisContext*)redisConnectWithTimeout("127.0.0.1", 6379, timeout);
    if ( (NULL == pRedisContext) || (pRedisContext->err) )
    {
        if (pRedisContext)
        {
            std::cout << "connect error:" << pRedisContext->errstr << std::endl;
        }
        else
        {
            std::cout << "connect error: can't allocate redis context." << std::endl;
        }
        return -1;
    }
    //redisReply是Redis命令回复对象 redis返回的信息保存在redisReply对象中
    std::cout << "redis command: " << cmd << std::endl;
    redisReply *pRedisReply = (redisReply*)redisCommand(pRedisContext, cmd);  //执行INFO命令
    if (NULL == pRedisReply) {
        std::cerr << pRedisContext->errstr << std::endl;
        redisFree(pRedisContext);
        return EXIT_FAILURE;
    }
    switch(pRedisReply->type) {
        case REDIS_REPLY_NIL:
            std::cout << "REDIS_REPLY_NIL" << std::endl;
            break;

        case REDIS_REPLY_ARRAY:
            std::cout << "REDIS_REPLY_ARRAY" << std::endl;
            for(size_t i = 0; i < pRedisReply->elements; ++i) {
                if (pRedisReply->element[i]->type == REDIS_REPLY_NIL) {
                    std::cout << "reply is null" << std::endl;
                }
                else {
                    std::cout << pRedisReply->str << std::endl;
                }
            }
            break;
        case REDIS_REPLY_INTEGER:
            std::cout << "REDIS_REPLY_INTEGER" << std::endl;
            std::cout << pRedisReply->integer << std::endl;
            break;

        case REDIS_REPLY_STATUS:
            std::cout << "REDIS_REPLY_STATUS" << std::endl;
            std::cout << pRedisReply->str << std::endl;
            break;

        case REDIS_REPLY_STRING:
            std::cout << "REDIS_REPLY_STRING" << std::endl;
            std::cout << pRedisReply->str << std::endl;
            break;

        case REDIS_REPLY_ERROR:
            std::cout << "REDIS_REPLY_ERROR" << std::endl;
            std::cerr << pRedisReply->str << std::endl;
            break;
        default:
            std::cerr << "It should not be here" << std::endl;
            std::cerr << "redis_type:" << pRedisReply->type << std::endl;
            std::cerr << "redis_str:" << pRedisReply->str << std::endl;
            break;
    }
    //当多条Redis命令使用同一个redisReply对象时 
    //每一次执行完Redis命令后需要清空redisReply 以免对下一次的Redis操作造成影响
    //freeReplyObject会释放array里的子reply对象，所以千万不要重复释放子reply对象
    //使用异步api的时候，没有必要调用freeReplyObject，在callback返回的时候hiredis会自动清理
    freeReplyObject(pRedisReply);   

    redisFree(pRedisContext);

    return 0;
}

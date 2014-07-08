/*
 * conf.h
 *	
 *  功能：配置管理
 *  
 *  Created on: 2011-11-3
 *      Author: fengjianhua
 *        Mail: jianhuafeng@sohu-inc.com 56683216@qq.com
 *
 *  修改记录：
 */

#ifndef STATS_CONF_H_
#define STATS_CONF_H_

#include "../jeep/jeep_type.h"
#include "../jeep/jeep_util.h"

#define NAT_TYPE_NUM					(8)
#define VER_MAX								(32)
#define CHANNEL_MAX						(2048)
#define CHNLID_INNER_STARTID	(1000)
#define CHNLID_OUTER_STARTID	(2000)

#pragma pack(1)

typedef struct _VER {
	int vernum;
	char strver[VER_MAX][64];
	uint32 intver[VER_MAX];
} VER;

typedef struct _CONF {
	uint16 port;
    uint32 timeout;
	uint32 workernum;
	uint32 connnum;
	uint16 maxinchid;
	uint16 maxoutchid;
} CONF;

#pragma pack()

sint32 init_conf(char *file);
sint32 init_ver(char *file);
int get_ver_ind(uint32 intver, int def);
void enable_reaload_confver();

extern CONF *conf;
extern VER *ver;
extern uint32 chid[CHANNEL_MAX];

#endif /* STATS_CONF_H_ */

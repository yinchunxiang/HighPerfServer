/*
 * collect.h
 *	
 *  功能：定期收集、汇总数据，向数据库模块添加操作任务
 *  
 *  Created on: 2011-11-7
 *      Author: fengjianhua
 *        Mail: jianhuafeng@sohu-inc.com 56683216@qq.com
 *
 *  修改记录：
 */

#ifndef STATS_COLLECT_H_
#define STATS_COLLECT_H_

#include "conf.h"


#define NUM_START_TYPE (5)

typedef struct _REPORT {
	uint32 starttype[NUM_START_TYPE][2]; //不分版本
	uint64 avgspeed[VER_MAX][2];
	sint32 avgspeedcount[VER_MAX][2];
	uint64 cdnspeed[VER_MAX][2];
	uint64 p2pspeed[VER_MAX][2];
	uint32 speed_count[VER_MAX][2];
	sint32 speedlt240[VER_MAX][2];
	sint32 speed240_640[VER_MAX][2];
	sint32 speed640_800[VER_MAX][2];
	sint32 speedge800[VER_MAX][2];
	uint64 online[VER_MAX][CHANNEL_MAX];
	uint64 p2pbyte[VER_MAX][2];
	uint64 cdnbyte[VER_MAX][2];
	uint32 natsuccess[VER_MAX][NAT_TYPE_NUM][2];
	uint32 natfailed[VER_MAX][NAT_TYPE_NUM][2];
	uint32 logout[VER_MAX][CHANNEL_MAX][2];
} REPORT;

extern REPORT *report;

extern int ind_collect_1;
extern int ind_collect_30;
extern int ind_collect_60;

/**
 * 初始化全局report
 */
sint32 init_report();

sint32 start_collect_thread();

#endif /* STATS_COLLECT_H_ */

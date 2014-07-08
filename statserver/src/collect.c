/*
 * collcet.c
 *	
 *  Created on: 2011-11-7
 *      Author: fengjianhua
 *        Mail: jianhuafeng@sohu-inc.com 56683216@qq.com
 *
 *  ÐÞ¸Ä¼ÇÂ¼£º
 */

#include "../jeep/jeep.h"
#include "collect.h"

#define COLLECT_MINUTE		    (60)
#define COLLECT_30MINUTE			(1800)
#define COLLECT_60MINUTE			(3600)

REPORT *report = NULL;

int ind_collect_1 = 0;
int ind_collect_30 = 0;
int ind_collect_60 = 0;

static LOG_QUEUE *log = NULL;

sint32 init_report() {
	log = create_log_queue();
	if (log == NULL) {
		fprintf(stderr, "init_report log error! errno: %d %m\n", errno);
		return -1;
	}
	report = (REPORT *) malloc(sizeof(REPORT) * conf->workernum);
	if (report == NULL) {
		fprintf(stderr, "init_report error!\n");
		return -1;
	}
	memset(report, 0, sizeof(REPORT) * conf->workernum);
	return 0;
}

static void online_collect(const char *datetime, int ind) {
}

void speed_collect(const char *datetime, int ind) {
}

void bandwith_collect(const char *datetime, int ind) {
}

void natconn_collect(const char *datetime, int ind) {
}

void speed_range_collect(const char *datetime) {
}

void starttype_collect(const char *datetime) {
}

static void *collect_worker_fun(void *arg) {
	static int64_t timecount = 0;
	time_t timesec;
	static struct tm *timestr;
	static char datetime[50];
	while (1) {
		susleep(1, 0);
		if ((++timecount) % COLLECT_MINUTE == 0) {
			timesec = time(NULL);
			timestr = localtime(&timesec);
			memset(datetime, 0, sizeof(datetime));
			strftime(datetime, 50, "%Y-%m-%d %H:%M:%S", timestr);


			int ind = ind_collect_1;
			ind_collect_1 = (ind_collect_1 + 1) % 2;
			online_collect(datetime,ind);
			speed_collect(datetime, ind);
			bandwith_collect(datetime, ind);
			natconn_collect(datetime, ind);

			if (timecount % COLLECT_30MINUTE == 0)
				speed_range_collect(datetime);
			if (timecount % COLLECT_60MINUTE == 0)
				starttype_collect(datetime);
		}
	}
	return NULL;
}

sint32 start_collect_thread() {
	pthread_t thread_id;
	return pthread_create(&thread_id, NULL, collect_worker_fun, NULL);
}

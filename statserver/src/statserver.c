/*
 * statserver.c
 *	
 *  名称：统计服务器
 *  功能：统计服务器代码主文件
 *  
 *  Created on: 2011-10-13
 *      Author: fengjianhua
 *        Mail: jianhuafeng@sohu-inc.com 56683216@qq.com
 *
 *  修改记录：
 */
//#define _GNU_SOURCE
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "jeep.h"
#include "ptcl.h"
#include "akg.h"
#include "conf.h"
#include "collect.h"

int main(int argc, char *argv[]) {

	daemonize();

	if (init_conf("./conf") < 0)
		return -1;
	if (init_ver("./ver") < 0)
		return -1;
	enable_reaload_confver();

	SERVER *server = init_server(conf->port, conf->workernum, conf->connnum, conf->timeout, conf->timeout);
	if (server == NULL)
		return -1;

    regist_akg_func(CMD_ADMIN_USER_CODE, admin_user_code);
    printf("CMD_CHECK_USER_CODE:%d\n", CMD_CHECK_USER_CODE);
    printf("check_user_code:%p\n", check_user_code);
    regist_akg_func(CMD_CHECK_USER_CODE, check_user_code);
    regist_akg_func(CMD_CHECK_UPGRADE, check_upgrade);
    regist_akg_func(CMD_REPORT_UPGRADE, report_upgrade);

	if (init_report() < 0)
		return -1;
	if (start_log_thread() != 0) {
		fprintf(stderr, "start_log_thread error, errno: %d %m\n", errno);
		return -1;
	}
	if (start_collect_thread() != 0) {
		fprintf(stderr, "start_collect_thread error, errno: %d %m\n", errno);
		return -1;
	}

	start_server(server);

	return 0;
}

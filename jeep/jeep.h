/*
 * JEEP: Johan Efficient Event Platform
 *
 * jeep.h
 *	
 *  ���ƣ�jeep�ӿ�ģ��
 *  ���ܣ�jeep��Ч���紦�����ܵĽӿ�
 *  
 *  Created on: 2011-10-4
 *      Author: feng jianhua (johan fong)
 *        Mail: jianhuafeng@sohu-inc.com 56683216@qq.com
 *
 *  �޸ļ�¼��
 */

#ifndef JEEP_H_
#define JEEP_H_
#include "jeep_global.h"
#include "jeep_type.h"
#include "jeep_log.h"
#include "jeep_conn.h"
#include "jeep_network.h"
#include "jeep_akg.h"
#include "jeep_util.h"

extern SERVER* init_server(int port, uint16 workernum, uint32 connnum, int read_timeout, int write_timeout);

extern int start_server(SERVER* server);

extern inline void close_err_conn(CONN *c);

/**
 *	ע��ҵ��������
 */
void regist_akg_func(uint32_t id, FUNC_PTR func);

/**
 *	ע����ܿͻ�������ʱ��Ӧ����
 */
void regist_connected_func(FUNC_PTR func);

/**
 *	ע��ͻ��˶Ͽ���������
 */
void regist_disconnected_func(FUNC_PTR func);

/**
 *	ע�����糬ʱ��������
 */
void regist_timeout_func(FUNC_PTR func);

/**
 *	ע��ͻ������ӹرմ�������
 */
void regist_closed_func(FUNC_PTR func);

/**
 *	ע���������������
 */
void regist_error_func(FUNC_PTR func);

#endif /* JEEP_H_ */
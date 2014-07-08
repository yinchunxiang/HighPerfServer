/*
 * JEEP: Johan Efficient Event Platform
 *
 * jeep_conn.c
 *	
 *  Created on: 2011-10-3
 *      Author: feng jianhua (johan fong)
 *        Mail: jianhuafeng@sohu-inc.com 56683216@qq.com
 *
 *  ÐÞ¸Ä¼ÇÂ¼£º
 */
#include "jeep_conn.h"

CONN_LIST* init_conn_list(uint32 size) {
	CONN_LIST *lst = NULL;
	uint32 len = sizeof(CONN_LIST) + sizeof(CONN) * (size + 1);
	lst = (CONN_LIST*) malloc(len);
	if (lst == NULL)
		return NULL;
	memset(lst, 0, len);
	lst->head = &lst->list[0];
	lst->tail = &lst->list[size];
	int i = 0;
	for (; i < size; i++) {
		lst->list[i].ind = i;
		lst->list[i].next = &lst->list[i + 1];
	}
	lst->list[size].ind = size;
	lst->list[size].next = NULL;
	return lst;
}

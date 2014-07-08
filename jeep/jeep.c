/*
 * JEEP: Johan Efficient Event Platform
 *
 * jeep.h
 *
 *  Created on: 2011-10-4
 *      Author: feng jianhua (johan fong)
 *        Mail: jianhuafeng@sohu-inc.com 56683216@qq.com
 *
 *  ÐÞ¸Ä¼ÇÂ¼£º
 */
#include "jeep.h"

void regist_akg_func(uint32 id, FUNC_PTR func) {
	AKG_FUNC[id] = func;
}

void regist_connected_func(FUNC_PTR func) {
	AKG_FUNC[g_akg_connected_id] = func;
}

void regist_disconnected_func(FUNC_PTR func) {
	AKG_FUNC[g_akg_timeout_id] = func;
	AKG_FUNC[g_akg_closed_id] = func;
	AKG_FUNC[g_akg_error_id] = func;
}

void regist_timeout_func(FUNC_PTR func) {
	AKG_FUNC[g_akg_timeout_id] = func;
}

void regist_closed_func(FUNC_PTR func) {
	AKG_FUNC[g_akg_closed_id] = func;
}

void regist_error_func(FUNC_PTR func) {
	AKG_FUNC[g_akg_error_id] = func;
}

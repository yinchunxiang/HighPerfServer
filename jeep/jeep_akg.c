/*
 * JEEP: Johan Efficient Event Platform
 *
 * jeep_akg.c
 *	
 *  Created on: 2011-10-3
 *      Author: feng jianhua (johan fong)
 *        Mail: jianhuafeng@sohu-inc.com 56683216@qq.com
 *
 *  ÐÞ¸Ä¼ÇÂ¼£º
 */

#include "jeep_akg.h"

const uint32_t g_akg_connected_id = 0xfffffc;
const uint32_t g_akg_timeout_id = 0xfffffd;
const uint32_t g_akg_closed_id = 0xfffffe;
const uint32_t g_akg_error_id = 0xffffff;

FUNC_PTR AKG_FUNC[MAX_PKG_TYPE];

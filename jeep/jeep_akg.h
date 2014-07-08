/*
 * JEEP: Johan Efficient Event Platform
 *
 * jeep_akg.h
 *	
 *  名称：业务逻辑触发模块
 *  功能：管理、触发业务逻辑，接收处理报告
 *  
 *  Created on: 2011-9-30
 *      Author: feng jianhua (johan fong)
 *        Mail: jianhuafeng@sohu-inc.com 56683216@qq.com
 *
 *  修改记录：
 */

#ifndef JEEP_AKG_H_
#define JEEP_AKG_H_
#include "jeep_type.h"
#include "jeep_conn.h"

#define MAX_PKG_TYPE (0xffffff)
extern const uint32_t g_akg_connected_id;
extern const uint32_t g_akg_timeout_id;
extern const uint32_t g_akg_closed_id;
extern const uint32_t g_akg_error_id;

typedef sint32 (*FUNC_PTR)(CONN* c);

extern FUNC_PTR AKG_FUNC[MAX_PKG_TYPE];

#pragma pack(1)
typedef struct _HEAD {
	uint16 version;
	uint32 pkglen; // head + body
	uint32 akg_id;
    char user_code[64];
    char machine_code[64];
} HEAD;
#pragma pack()
#define SOHU_P2P_STX	(0x7fe8576f)
#define SIZE_OF_HEAD	((uint32) 138)

#endif /* JEEP_AKG_H_ */

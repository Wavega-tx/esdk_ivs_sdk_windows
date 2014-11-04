/********************************************************************
	filename	: 	OCXLogMgr.h
	author		:	wWX155351
	created		:	2012/12/10	
	description	:	����澯OCX
	copyright	:	Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
	history		:	2012/12/10 ��ʼ�汾
*********************************************************************/

#ifndef __OCX_LOG_MGR_H__
#define __OCX_LOG_MGR_H__

#include "hwsdk.h"

class COCXLogMgr
{
public:
	COCXLogMgr();
	~COCXLogMgr();

	// ��ѯ������־
	static IVS_INT32 GetOperationLog(IVS_INT32 iSessionId, const IVS_CHAR* pReqXml, CString &strResult);

};

#endif // __OCX_LOG_MGR_H__

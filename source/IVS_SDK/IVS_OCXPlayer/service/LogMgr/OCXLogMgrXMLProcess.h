/********************************************************************
	filename	: 	OCXLogMgrXMLProcess.h
	author		:	wulixia
	created		:	2012/12/03	
	description	:	����澯XML
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/12/03 ��ʼ�汾
*********************************************************************/

#ifndef __OCX_LOG_MGR_XML_PROCESS_H__
#define __OCX_LOG_MGR_XML_PROCESS_H__

#include "OCXXmlProcess.h"

class COCXLogMgrXMLProcess
{
private:
	COCXLogMgrXMLProcess();
public:
	~COCXLogMgrXMLProcess();

public:	

	/**************************************************************************
	* name       : GetResOperationLogXML()
	* description: ƴװ��Ӧ��Ϣ��xml
	* input      : pQueryUnifiedFormat        ������ڴ�ṹ��	
	*              iFieNum    ��ѯ�ֶεĸ���
	*              pUserData		 �û�����;
	* output     : xmlReq
	* return     : �ɹ�����0 �����󷵻ش�����
	* remark     : NA
	**************************************************************************/
	static IVS_INT32 GetResOperationLogXML(IVS_OPERATION_LOG_LIST* pOperationLogList, CXml& xmlRes);
};

#endif //__OCX_LOG_MGR_XML_PROCESS_H__

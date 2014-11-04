/********************************************************************
	filename	: 	LogMgrXMLProcess.h
	author		:	wulixia
	created		:	2012/12/03	
	description	:	���������־XML
	copyright	:	Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
	history		:	2012/12/03 ��ʼ�汾
*********************************************************************/

#ifndef __LOG_MGR_XML_PROCESS_H__
#define __LOG_MGR_XML_PROCESS_H__

#include "XmlProcess.h"

class CLogMgrXMLProcess
{
private:
	CLogMgrXMLProcess();
public:
	~CLogMgrXMLProcess();

public:
	/*****************************************************************
     function   : PraseResOperationLogXML
     description: ��ѯ������־������Ӧ
	 intput     : pResOperationLog����Ӧ��Ϣ�ṹ��
	              iBufSize�������С
     output     : xml�������Ŀ��xml
     return     : �ɹ�����0��ʧ�ܷ��ش�����;
     *****************************************************************/

	static IVS_INT32 PraseResOperationLogXML(CXml& xml,IVS_OPERATION_LOG_LIST* pResOperationLog,const IVS_UINT32& uiBufSize);

	/*****************************************************************
     function   : GetResOperationLogExXML
     description: ƴװ��ѯ������־����Xml
	 intput     : pUnifiedQuery ͨ�÷�ҳ��ѯ�ṹ��
	 intput     : pOptLogQueryEx ��չ��ѯ������־����
     output    : xml�������Ŀ��xml
     return     : �ɹ�����0��ʧ�ܷ��ش�����;
     *****************************************************************/
	static IVS_INT32 GetResOperationLogExXML(const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, 
		const IVS_QUERY_OPTLOG_EX *pOptLogQueryEx,
		CXml &xml);

	/*****************************************************************
     function   : ParseRspOperationLogExXML
     description: ������ѯ������־��ӦXml
	 intput     : pResOperationLog����Ӧ��Ϣ�ṹ��
	 intput     : iBufSize�������С
     output     : xml�������Ŀ��xml
     return     : �ɹ�����0��ʧ�ܷ��ش�����;
     *****************************************************************/
	static IVS_INT32 ParseRspOperationLogExXML(CXml& xml,
		IVS_OPERATION_LOG_LIST* pResOperationLog, 
		IVS_UINT32 uiBufSize);

	
};

#endif //__LOG_MGR_XML_PROCESS_H__


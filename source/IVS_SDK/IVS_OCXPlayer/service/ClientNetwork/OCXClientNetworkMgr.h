/********************************************************************
filename : OCXClientNetworkMgr.h
author : pkf57481
created : 2013/01/22
description : �ͻ�������
copyright : Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history : 2013/01/22 ��ʼ�汾
*********************************************************************/


#ifndef __OCX_CLIENTNETWORK_MGR_H_
#define __OCX_CLIENTNETWORK_MGR_H_

#include "hwsdk.h"
#include "IVSCommon.h"

class COCXClientNetworkMgr
{
public:
    COCXClientNetworkMgr(void);
    ~COCXClientNetworkMgr(void);

public: 
	/***********************************************************************************
    * name       : GetCPUMemory
    * description: CPU/�ڴ����ܲ�ѯocx
    * input      : iSessionId:��¼Id; pNodeCode:CU������;
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetCPUMemory(IVS_INT32 iSessionID, const IVS_CHAR* pNodeCode, CString& strResult);

	/***********************************************************************************
    * name       : GetIACPUMemory
    * description: ���ܷ���CPU/�ڴ����ܲ�ѯocx
    * input      : iSessionId:��¼Id; pNodeCode:CU������;
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetIACPUMemory(IVS_INT32 iSessionID, const IVS_CHAR* pNodeCode, CString& strResult);

	/***********************************************************************************
    * name       : QueryPerformance
    * description: ��Ԫ���ܲ�ѯocx
    * input      : iSessionId:��¼Id; pNodeCode:CU������;
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 QueryPerformance(IVS_INT32 iSessionID, const IVS_CHAR* pNodeCode, CString& strResult);

	/*****************************************************************
	     function   : AddAlarmLinkageRule
	     description: �����澯֪ͨ����ӿ�
	     input      : IVS_INT32 iSessionID      ��¼�ɹ���ĻỰID
	     input      : const IVS_CHAR * pReqXml  ��������XML
	     output     : CString & strResult       ���XML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	static IVS_INT32 AddAlarmLinkageRule(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, CString& strResult);

	/*****************************************************************
	     function   : ModAlarmLinkageRule
	     description: �޸ĸ澯֪ͨ����ӿ�
	     output     : IVS_INT32 iSessionID       ��¼�ɹ���ĻỰID
	     output     : const IVS_CHAR * pReqXml   ��������XML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	static IVS_INT32 ModAlarmLinkageRule(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/*****************************************************************
	     function   : DelAlarmLinkageRule
	     description: ɾ���澯֪ͨ����ӿ�
	     output     : IVS_INT32 iSessionID       ��¼�ɹ���ĻỰID
	     output     : const IVS_CHAR * pReqXml   ��������XML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	static IVS_INT32 DelAlarmLinkageRule(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/*****************************************************************
	     function   : GetAlarmLinkageRuleList
	     description: ��ȡ�澯֪ͨ�����б�ӿ�
	     input      : IVS_INT32 iSessionID      ��¼�ɹ���ĻỰID
	     input      : const IVS_CHAR * pReqXml  ��������XML
	     output     : CString & strResult       ���XML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	static IVS_INT32 GetAlarmLinkageRuleList(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, CString& strResult);

	/*****************************************************************
	     function   : GetAlarmLinkageRuleDetail
	     description: ��ȡ�澯֪ͨ��������ӿ�
	     input      : IVS_INT32 iSessionID      ��¼�ɹ���ĻỰID
	     input      : const IVS_CHAR * pReqXml  ��������XML
	     output     : CString & strResult       ���XML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	static IVS_INT32 GetAlarmLinkageRuleDetail(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, CString& strResult);

	/*****************************************************************
	     function   : GetDevTypeByNodeCode
	     description: ������Ԫ��ȡ�豸���ͽӿ�
	     input      : IVS_INT32 iSessionID      ��¼�ɹ���ĻỰID
	     input      : const IVS_CHAR * pReqXml  ��������XML
	     output     : CString & strResult       ���XML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	static IVS_INT32 GetDevTypeByNodeCode(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, CString& strResult);

	/*****************************************************************
	     function   : GetAlarmLinkageRuleDetail
	     description: �ͻ����������
	     input      : IVS_INT32 iSessionID      ��¼�ɹ���ĻỰID
	     input      : const IVS_CHAR * pReqXml  ��������XML
	     output     : CString & strResult       ���XML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	static IVS_INT32 CheckNewVersion(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, CString& strResult);
};

#endif

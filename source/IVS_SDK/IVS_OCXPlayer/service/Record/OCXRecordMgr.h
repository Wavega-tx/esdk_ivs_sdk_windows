/********************************************************************
	filename	: 	OCXRecordMgr.h
	author		:	s00193168
	created		:	2012/12/05	
	description	:	����¼�������;
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/12/05 ��ʼ�汾;
*********************************************************************/
#ifndef __OCX_RECORD_MGR_H__ 
#define __OCX_RECORD_MGR_H__

#include "hwsdk.h"
#include "stdafx.h"
class OCXRecordMgr
{
public:
	OCXRecordMgr(void);
	~OCXRecordMgr(void);

	/**********************************************************************
	* name			: AddRecordPlan
	* description	: ����¼��ƻ�
	* input			: strResult ¼��ƻ�xml
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
	static CString AddRecordPlan(const CString& pRecordPlanReq,const IVS_INT32 iSessionID);

	/**********************************************************************
	* name			: ModifyRecordPlan
	* description	: �޸�¼��ƻ�
	* input			: strResult ¼��ƻ�xml
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
	static CString ModifyRecordPlan(const CString& pRecordPlanReq,const IVS_INT32 iSessionID);

	/**********************************************************************
	* name			: DeleteRecordPlan
	* description	: ɾ��¼��ƻ�
	* input			: strResult ¼��ƻ�xml
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
	static CString DeleteRecordPlan(const CString& pRecordPlanReq,const IVS_INT32 iSessionID);

	/**********************************************************************
	* name			: GetRecordPlan
	* description	: ��ѯ¼��ƻ�
	* input			: strResult ¼��ƻ�xml
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
	static CString GetRecordPlan(const CString& pRecordPlanReq,const IVS_INT32 iSessionID);

	/**********************************************************************
	* name			: GetRecordStatus
	* description	: ��ѯ¼��״̬
	* input			: strResult ¼��״̬xml
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
	static CString GetRecordStatus(const CString& pCameraCode,const ULONG ulRecordMethod,const IVS_INT32 m_iSDKSessionID);

	/*****************************************************************
	     function   : SetCameraBackupConfig
	     description: ������������ݲ���        
	     output     : IVS_INT32 iSessionID          ��¼Id
	     output     : const IVS_CHAR * pBackupXml   ����xml
	     return     : �ɹ�-0 ʧ��-������ 
	*****************************************************************/
	static IVS_INT32 SetCameraBackupConfig(IVS_INT32 iSessionID, const IVS_CHAR* pBackupXml);

	/*****************************************************************
	     function   : GetCameraBackupCfg
	     description: ��ȡ��������ݲ���
	     input      : IVS_INT32 iSessionId			 ��¼Id
				   	  const IVS_CHAR * pCameraCode   ���������
	     output     : CString & strResult            SMU����Ӧ
	     return     : �ɹ�-0 ʧ��-������ 
	*****************************************************************/
	static IVS_INT32 GetCameraBackupCfg(IVS_INT32 iSessionId, const IVS_CHAR* pCameraCode, CString &strResult);

	/*****************************************************************
	     function   : AddRecordBackupPlan
	     description: ���¼�񱸷ݼƻ�
	     input      : IVS_INT32 iSessionID   ��¼�ɹ���ĻỰID
	                  const IVS_CHAR * pBackupPlan   ���¼�񱸷ݼƻ�XML
	     return     : �ɹ�-0 ʧ��-������ 
	*****************************************************************/
	static IVS_INT32 AddRecordBackupPlan(IVS_INT32 iSessionID, const IVS_CHAR* pBackupPlan);

	/*****************************************************************
	     function   : AddRecordBackupPlan
	     description: �޸�¼�񱸷ݼƻ�
	     input      : IVS_INT32 iSessionID   ��¼�ɹ���ĻỰID
	                  const IVS_CHAR * pBackupPlan   �޸�¼�񱸷ݼƻ�XML
	     return     : �ɹ�-0 ʧ��-������ 
	*****************************************************************/
	static IVS_INT32 ModifyRecordBackupPlan(IVS_INT32 iSessionID, const IVS_CHAR* pBackupPlan);

	/*****************************************************************
	     function   : AddRecordBackupPlan
	     description: ɾ��¼�񱸷ݼƻ�
	     input      : IVS_INT32 iSessionID   ��¼�ɹ���ĻỰID
	                  const IVS_CHAR * pBackupPlan   ɾ��¼�񱸷ݼƻ�XML
	     return     : �ɹ�-0 ʧ��-������ 
	*****************************************************************/
	static IVS_INT32 DeleteRecordBackupPlan(IVS_INT32 iSessionID, const IVS_CHAR* pBackupPlan);

	
	/*****************************************************************
	     function   : GetRecordBackupPlan
	     description: ��ѯ¼�񱸷ݼƻ�
	     output     : IVS_INT32 iSessionID          ��¼�ɹ���ĻỰID
	     output     : const IVS_CHAR * pQueryInfo   ��ѯ¼�񱸷ݼƻ�XML
	     output     : CString & strResult           ¼�񱸷ݼƻ�XML
	     return     : �ɹ�-0 ʧ��-������ 
	*****************************************************************/
	static IVS_INT32 GetRecordBackupPlan(IVS_INT32 iSessionID, const IVS_CHAR* pQueryInfo, CString &strResult);

	/*****************************************************************
	     function   : GetRecordTask
	     description: ��ѯ�ֶ�¼������
	     input     : IVS_INT32 iSessionID      ��¼�ɹ���ĻỰID
	     output     : CString & strResult      ��ѯ�ֶ�¼������XML
	     return     : �ɹ�-0 ʧ��-������ 
	*****************************************************************/
	static IVS_INT32 GetRecordTask(IVS_INT32 iSessionId, CString &strResult);

	/*****************************************************************
	     function   : GetRecordBackupTaskList
	     description: ��ѯ¼�񱸷������б�
	     output     : IVS_INT32 iSessionID          ��¼�ɹ���ĻỰID
	     output     : const IVS_CHAR * pQueryInfo   ��ѯ¼�񱸷�����XML
	     output     : CString & strResult           ¼�񱸷�����XML
	     return     : �ɹ�-0 ʧ��-������ 
	*****************************************************************/
	static IVS_INT32 GetRecordBackupTaskList(IVS_INT32 iSessionID, const IVS_CHAR* pQueryInfo, CString &strResult);

	/*****************************************************************
	     function   : AddRecordBackupTask
	     description: ����¼�񱸷������б�
	     output     : IVS_INT32 iSessionID   ��¼�ɹ���ĻỰID
	     output     : const IVS_CHAR * pBackupTask  ����¼�񱸷�����XML
	     return     : 
	*****************************************************************/
	static IVS_INT32 AddRecordBackupTask(IVS_INT32 iSessionID, const IVS_CHAR* pBackupTask);

    /******************************************************************
    function : GetCameraPlanInfo
    description: ��ѯ������ƻ�������Ϣ
    input : IVS_INT32 iSessionID            ��¼�ɹ���ĻỰID   
    input : const IVS_CHAR * pCameraList    ����������б�
    output: CString & strResult             ���XML
    return : IVS_INT32
    *******************************************************************/
    static IVS_INT32 GetCameraPlanInfo(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, CString &strResult);

		/**********************************************************************
	* name			: AddExDomainRecordPlan
	* description	: �����������������¼��ƻ�
	* input			: pRecordPlanReq ¼��ƻ�����xml
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
	static CString AddExDomainRecordPlan(const CString& pRecordPlanReq,const IVS_INT32 iSessionID);

	/**********************************************************************
	* name			: ModifyExDomainRecordPlan
	* description	: �޸��������������¼��ƻ�
	* input			: pRecordPlanReq ¼��ƻ�����xml
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
	static CString ModifyExDomainRecordPlan(const CString& pRecordPlanReq,const IVS_INT32 iSessionID);

	/**********************************************************************
	* name			: DeleteExDomainRecordPlan
	* description	: ɾ���������������¼��ƻ�
	* input			: pRecordPlanReq ¼��ƻ�����xml
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
	static CString DeleteExDomainRecordPlan(const CString& pRecordPlanReq,const IVS_INT32 iSessionID);

	/**********************************************************************
	* name			: GetExDomainRecordPlan
	* description	: ��ѯ�������������¼��ƻ�
	* input			: pRecordPlanReq ¼��ƻ�����xml
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
	static CString GetExDomainRecordPlan(const CString& pRecordPlanReq,const IVS_INT32 iSessionID);
};

#endif


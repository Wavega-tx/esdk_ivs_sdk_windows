/********************************************************************
	filename	: 	OCXLogMgrXMLProcess.h
	author		:	songjianfeng(00193168)
	created		:	2012/12/03	
	description	:	����ý����Ƶ�طŵ�XML
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/12/03 ��ʼ�汾
*********************************************************************/
#ifndef __OCX_RECORD_XML_PROCESS_H__ 
#define __OCX_RECORD_XML_PROCESS_H__

#include "OCXDatatype.h"
#include "SDKDef.h"

class COCXRecordXMLProcess
{
private:
	COCXRecordXMLProcess();
public:
	~COCXRecordXMLProcess();

public:

	/******************************************************************
	function : GetRecordListGetXML
	description: ��ѯ¼���б�����xml;
	input : pCameraCode,�������ţ�pRecordInfoList��¼����Ϣ�б�iRetCord������룻xml��ƴװ������xml
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 GetRecordListGetXML(const IVS_CHAR* pCameraCode,IVS_RECORD_INFO_LIST *pRecordInfoList,IVS_INT32 iRetCord,CXml &xml);

	/******************************************************************
	function : GetQueryUnifiedCameraCode
	description: ��ȡ¼���ѯ�����е�CameraCode;
	input : pCameraCode,�������ţ�xml��ƴװ������xml
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 GetQueryUnifiedCameraCode(CXml &xml,IVS_CHAR* pCameraCode);
	
	/******************************************************************
	function : AddOrModRecordPlanGetXML
	description: ���/�޸�¼��ƻ�:������XMLת��Ϊ�ṹ��;
	input : uiDeviceNum���������Ŀ��pDeviceList,�������ţ�pRecordPlan��¼��ƻ����������xml��ƴװ������xml
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 AddOrModRecordPlanGetXML(IVS_UINT32& uiDeviceNum,const IVS_DEVICE_CODE* pDeviceList, IVS_RECORD_PLAN *pRecordPlan, CXml &xml);

	/******************************************************************
	function : DelRecordPlanGetXML
	description: ɾ��¼��ƻ�:������XMLת��Ϊ�ṹ��;
	input :  uiDeviceNum���������Ŀ��pDeviceList,�������ţ�uiRecordMethod��¼��ʽ��xml��ƴװ������xml
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 DelRecordPlanGetXML(IVS_UINT32& uiDeviceNum,const IVS_DEVICE_CODE* pDeviceList, IVS_UINT32 &uiRecordMethod, CXml &xml);

	/******************************************************************
	function : GetRecordPlanGetXML
	description: ��ѯ¼��ƻ�:������XMLת��Ϊ�ṹ��;
	input : pCameraCode,�������ţ�uiRecordMethod��¼��ƻ���ʽ��xml��ƴװ������xml
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 GetRecordPlanGetXML(const char* pCameraCode, IVS_UINT32 &uiRecordMethod, CXml &xml);

	/******************************************************************
	function : RecordPlanGetRspXML
	description: ���/�޸�/ɾ��¼��ƻ�:������ṹ��ת��Ϊ��ӦXML;
	input : iRet������룻resultList��¼��ƻ����������xml��ƴװ������xml
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    static IVS_INT32 RecordPlanGetRspXML(IVS_INT32 iRet,const IVS_RESULT_LIST* resultList, CXml &xml);

	/******************************************************************
	function : RecordPlanGetRspXMLBypDeviceList
	description: ���/�޸�/ɾ��¼��ƻ�:������ṹ��ת��Ϊ��ӦXML(�����쳣�������);
	input : iRet������룻uiDeviceNum���������Ŀ��pDeviceList,�������ţ�xml��ƴװ������xml
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static void RecordPlanGetRspXMLBypDeviceList(IVS_INT32 iRet,IVS_UINT32 uiDeviceNum,const IVS_DEVICE_CODE* pDeviceList, CXml &xml);

	/******************************************************************
	function : GetRecordPlanGetRspXML
	description: ��ѯ¼��ƻ�:������ṹ��ת��Ϊ��ӦXML;
	input : iRet������룻pCameraCode,�������ţ�pRecordPlan��¼��ƻ���ѯ�����xml��ƴװ������xml
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 GetRecordPlanGetRspXML(IVS_INT32 iRet,const char* pCameraCode,const IVS_RECORD_PLAN* pRecordPlan, CXml &xml);

	/******************************************************************
	function : GetRecordStatusRspXML
	description: ¼��״̬��ѯ;
	input : iRet������룻pCameraCode,�������ţ�uiRecordMethod��¼��ƻ���ʽ��ulRecordStatus��¼��״̬��xml��ƴװ������xml
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 GetRecordStatusRspXML(IVS_INT32 iRet,const char* pCameraCode,IVS_UINT32 ulRecordMethod,IVS_UINT32 ulRecordStatus, CXml &xml);

	/******************************************************************
	function : RecordPolicyPraseXML
	description: ����¼�������ӦXML;
	input : pCameraCode,�������ţ�pRecordSpace��¼����ԣ�ʱ�䣩��xml����Ӧxml
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 RecordPolicyPraseXML(IVS_RECORD_POLICY_TIME* pRecordSpace, IVS_CHAR *cCameraCode,CXml &xml);

	/******************************************************************
	function : GetRecordPolicyGetXML
	description: ¼����Խ�����Ӧ��ϢXML;
	input : pCameraCode,�������ţ�pRecordPolicy��¼�������Ϣ��xml����Ӧxml
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 GetRecordPolicyGetXML(const IVS_RECORD_POLICY_TIME* pRecordPolicy ,LPCTSTR cCameraCode ,CXml &xml);

	/******************************************************************
	function : LocalRecordParseXML
	description: ��������¼�����ò���XML;
	input : LocalParam,����¼�������pXmlStr��XML�����ַ���
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    static IVS_INT32 LocalRecordParseXML(IVS_LOCAL_RECORD_PARAM &LocalParam, LPCTSTR pXmlStr);

	/******************************************************************
	function : LocalRecordParseClientConfig
	description: ��ȡCU����XML ת��ʹ��;
	input : LocalParam,����¼�������pXmlStr��XML�����ַ���
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 LocalRecordParseClientConfig(IVS_LOCAL_RECORD_PARAM &LocalParam, LPCTSTR pXmlStr);

	/******************************************************************
	function : LocalRecordBuildXML
	description: ����¼�񴴽�xml;
	input : pInfo,����¼���ļ��ϱ���Ϣ��xml������������xml
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 LocalRecordBuildXML(IVS_LOCAL_RECORD_SUCCESS_INFO *pInfo, 
		           unsigned long ulHandle, CXml &xml);

	/******************************************************************
	function : LocalRecordBuidlXMLGeneric
	description: ����¼������xml;
	input : code,��ţ�wndID������ID��xml����Ӧxml
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 LocalRecordBuidlXMLGeneric(int code, int wndID, CXml &xml);

	/******************************************************************
	function : RecordDownLoadParseXml
	description: ƽ̨/ǰ��¼�����ؽ���xml;
	input :  pXmlStr xml�ַ���
	output : pParam  �����ṹ��
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 RecordDownLoadParseXml(IVS_DOWNLOAD_PARAM &DLParam, LPCTSTR pXmlStr);

	/******************************************************************
	function : RecordDownLoadBuildXml
	description: ƽ̨/ǰ��¼�����ط���xml;
	input : ulHandle ����ͨ�������uiCode ������
	output : NA
	return : �ɹ�-0 ʧ��-1 
	*******************************************************************/
	static IVS_INT32 RecordDownLoadBuildXml(unsigned long ulHandle, unsigned int uiCode, CXml &xml);

	/******************************************************************
	function : RecordDownLoadInfoBuildXml
	description: ƽ̨/ǰ��¼��������Ϣ;
	input  : ulHandle ����ͨ�������IVS_DOWNLOAD_INFO ������Ϣ
	output : NA
	return : �ɹ�-0 ʧ��-1 
	*******************************************************************/
	static IVS_INT32 RecordDownLoadInfoBuildXml(int ResultCode, IVS_DOWNLOAD_INFO& DLInfo, CXml &xml);

	/******************************************************************
	function : GetRecordBookmarkListGetXML
	description: ¼����ǩ�б�;
	input : pCameraCode,�������ţ�pRecordBookmarkInfoList��¼����ǩ��Ϣ�б�iRetCode������룻xml����Ӧxml
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 GetRecordBookmarkListGetXML(const IVS_CHAR* pCameraCode,IVS_RECORD_BOOKMARK_INFO_LIST *pRecordBookmarkInfoList,IVS_INT32 iRetCode,CXml &xml);

	/***********************************************************************************
    * name       : ManualRecordStateGetXML
    * description: �ֶ�¼��״̬�ϱ�ƴ��XML
    * input      : pManualRecordStatusNotify:�ֶ�¼��״̬�ϱ��ṹ��; xml:����XML; 
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 ManualRecordStateGetXML(const IVS_MANUAL_RECORD_STATUS_NOTIFY* pManualRecordStatusNotify, CXml& xml);

	/***********************************************************************************
    * name       : LockRecordGetXML
    * description: ¼����������xml
    * input      : pRecordLockInfo:¼��������Ϣ�ṹ��; 
	               strLockInfo:����XML;
				   strCameraCode����������
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 LockRecordGetXML(const std::string& strLockInfo,std::string& strDomainCode, std::string& strNVRCode, std::string& strCameraCode, IVS_RECORD_LOCK_INFO* pRecordLockInfo);

	/***********************************************************************************
    * name       : ModLockRecordGetXML
    * description: �޸�¼��������Ϣ����xml
    * input      : pRecordLockInfo:¼��������Ϣ�ṹ��; 
	               strLockInfo:����XML;
				   strCameraCode����������
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 ModLockRecordGetXML(const std::string& strLockInfo,std::string& strDomainCode, std::string& strNVRCode, std::string& strCameraCode, IVS_RECORD_LOCK_INFO* pRecordLockInfo);


	/***********************************************************************************
    * name       : UnLockRecordGetXML
    * description: ¼���������xml
    * input      : pRecordLockInfo:¼��������Ϣ�ṹ��; 
	               strUnLockInfo:����XML;
				   strCameraCode����������
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 UnLockRecordGetXML(const std::string& strUnLockInfo,std::string& strDomainCode,std::string& strNVRCode, std::string& strCameraCode, IVS_RECORD_LOCK_INFO* pRecordLockInfo);

	/******************************************************************
	function : AddBookmarkParseXML
	description: �����ǩ���ɽ��XML��
	input : iRetCode������룻uiBookmarkID�����ɵ���ǩID��xml��ƴװ������xml
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    static IVS_INT32 AddBookmarkParseXML(IVS_UINT32 uiBookmarkID,IVS_INT32 iRetCode,CXml &xml);

    static IVS_INT32 GetCameraPlanInfoParseXML(const IVS_CHAR* pReqXml, IVS_UINT32& uiPlanType);
};
#endif


/********************************************************************
	filename	: 	OCXAlarmMgrXMLProcess.h
	author		:	z00193167
	created		:	2012/12/03	
	description	:	����澯XML
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/12/03 ��ʼ�汾
*********************************************************************/

#ifndef __OCX_ALARM_MGR_XML_PROCESS_H__
#define __OCX_ALARM_MGR_XML_PROCESS_H__

#include "OCXXmlProcess.h"
#include <atlstr.h>
#include "SDKDef.h"

class COCXAlarmMgrXMLProcess
{
private:
	COCXAlarmMgrXMLProcess();
public:
	~COCXAlarmMgrXMLProcess();

public:
	/******************************************************************
	function : GetWatermarkAlarmListGetXML
	description: ��ѯˮӡ�澯�б�����xml;
	input : pWatermarkAlarmList��ˮӡ�澯�б�iRetCord������룻xml��ƴװ������xml
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 GetWatermarkAlarmListGetXML(IVS_WATERMARK_ALARM_INFO_LIST *pWatermarkAlarmList,IVS_INT32 iRetCord,CXml &xml);

	/******************************************************************
	function : GetWatermarkAlarmCancelInfo
	description: ��ȡˮӡ�澯�����Ľṹ;
	input : pCancelInfo��ˮӡ�澯����XML��
	output : pstCancelInfo :�ṹ��Ϣ
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 GetWatermarkAlarmCancelInfo(const IVS_CHAR * pCancelInfo,IVS_WATERMARK_ALARM_CANCEL* pstCancelInfo);

	/******************************************************************
	function : GetWatermarkAlarmConfirmInfo
	description: ��ȡˮӡ�澯ȷ�ϵĽṹ;
	input : pCancelInfo��ˮӡ�澯ȷ��XML��
	output : pstCancelInfo :�ṹ��Ϣ
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 GetWatermarkAlarmConfirmInfo(const IVS_CHAR * pConfirmInfo,IVS_WATERMARK_ALARM_CONFIRM* pstConfirmInfo);

	/******************************************************************
	function : GetWaterTIPReportInfoXML
	description: ��ȡˮӡ�澯��ʾXML
	input : pWaterAlarmInfo��ˮӡ�澯��Ϣ
	output : xml 
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 GetWaterTIPReportInfoXML(const IVS_ALARM_WATERMARK_INFO * pWaterAlarmInfo, CXml &xml);

	/******************************************************************
	function : GetWaterAlarmReportInfoXML
	description: ��ȡˮӡ�۸ĸ澯
	input : pWaterAlarmInfo��ˮӡ�澯��Ϣ
	output : xml 
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	static IVS_INT32 GetWaterAlarmReportInfoXML(const IVS_ALARM_WATERMARK_INFO * pWaterAlarmInfo, CXml &xml);

	/***********************************************************************************
    * name       : AlarmLevelPraseXML
    * description: �澯����ͨ�ý���XML
    * input      : pAlarmLevel:�澯����ṹ��; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 AlarmLevelPraseXML(IVS_ALARM_LEVEL *pAlarmLevel, CXml &xml);
	
	/***********************************************************************************
    * name       : HelpInfoPraseXML
    * description: �澯����ͨ�ý���XML
    * input      : helpInfo:������Ϣ�ṹ��; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 HelpInfoPraseXML(IVS_HELP_INFO *helpInfo, CXml &xml, IVS_CHAR* pDomainCode);

	/***********************************************************************************
    * name       : QueryHelpInfoGetXML
    * description: ��ѯ�澯����ƴ��XML
    * input      : cstrRet:���ظ�CU���ַ���; helpInfo:������Ϣ�ṹ��; iRet:������; uiAlarmHelpSourceType:������Ϣ����; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 QueryHelpInfoGetXML(CString &cstrRet, const IVS_HELP_INFO *helpInfo, IVS_INT32 iRet, CXml &xml);

    /******************************************************************
     function   : QueryHelpInfoTextGetXML
     description: ��ѯ�澯������Ϣ����
     input      : CString & cstrRet
     input      : const char * pHelpInfoText ������Ϣ����
     input      : IVS_INT32 iRet 
     output      : CXml & xml
     return     : IVS_INT32
    *******************************************************************/
    static IVS_INT32 QueryHelpInfoTextGetXML(CString &cstrRet, const char *pHelpInfoText, IVS_INT32 iRet, CXml &xml);

	/***********************************************************************************
    * name       : GetAlarmLevelListGetXML
    * description: ��ѯ�澯�����б�ƴ��XML
    * input      : cstrRet:���ظ�CU���ַ���; pNum:��õĸ澯�������; pAlarmLevel:�澯����ṹ��; iRet:������; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAlarmLevelListGetXML(CString &cstrRet,const IVS_UINT32* pNum ,const IVS_ALARM_LEVEL *pAlarmLevel,IVS_INT32 iRet, CXml &xml);

	/***********************************************************************************
    * name       : GetAlarmLevel
    * description: ��ѯ�澯����+������ƴ��XML
    * input      : cstrRet:���ظ�CU���ַ���; pAlarmLevel:�澯����ṹ��; iRet:������; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAlarmLevel(CString &cstrRet,const IVS_ALARM_LEVEL *pAlarmLevel,IVS_INT32 iRet,CXml &xml);

	/***********************************************************************************
    * name       : GetAlarmLevel
    * description: ��ѯ�澯�����б�ƴ��XML
    * input      : pAlarmTypeList:�澯�����б�ṹ��; xmlRsp:����XML; bufSize:������;
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAlarmTypeListGetXml(IVS_ALARM_TYPE_LIST* pAlarmTypeList,CXml& xmlRsp, unsigned int bufSize);

	/***********************************************************************************
    * name       : GetAddAlarmErrString
    * description: ���Ӹ澯����ƴ��XML
    * input      : strResult:���ظ�CU���ַ���; iNum:�澯ID; iRet:������; 
    * output     : NA
    * return     : �޷���ֵ
    * remark     : NA
    ***********************************************************************************/
	static IVS_VOID GetAddAlarmErrString(CString &strResult,IVS_UINT32 iNum, IVS_INT32 iRet);

	/***********************************************************************************
    * name       : SetAlarmTypeLevelGetXml
    * description: ���ø澯���ͼ���ƴ��XML
    * input      : ReqXml:����XML; pAlarmType:�澯���ͽṹ��; 
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 SetAlarmTypeLevelGetXml(CXml &ReqXml, IVS_ALARM_TYPE_LIST* pAlarmType);

	/***********************************************************************************
    * name       : AlarmReportGetXML
    * description: �澯�ϱ�ƴ��XML
    * input      : pAlarmNotify:�澯�ϱ��ṹ��; xml:����XML; 
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 AlarmReportGetXML(const IVS_ALARM_NOTIFY* pAlarmNotify, CXml& xml);

	/***********************************************************************************
    * name       : GeneralGetString
    * description: ͨ�û�ȡ������
    * input      : strResult:������; pRspXml:SMU���ص��ַ���; iRet:������;
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GeneralGetString(CString &strResult, const IVS_CHAR* pRspXml, IVS_INT32 iRet);

	/***********************************************************************************
    * name       : ParseAlarmCommissionXML
    * description: �����澯��Ȩ����Xml
    * input      : pReqXml:����xml; pAlarmCommission:�澯��Ȩ�ṹ��
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 ParseAlarmCommissionXML(const IVS_CHAR* pReqXml, IVS_CHAR* pAlarmInCode, IVS_ALARM_COMMISSION*& pAlarmCommission) ;


	/***********************************************************************************
    * name       : GetAlarmListGetXML
    * description: ƴ�Ӳ�ѯ�澯��ʷ��Ϣ�б�Xml
    * input      : strResult:����ֵ; pResAlarmEventList:�澯�ϱ���Ϣ��ҳ��ѯ�б�; iRet:������; xml;ƴ��XML
    * output     : NA
    * return     : �޷���ֵ
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAlarmListGetXML(CString &strResult, IVS_ALARM_EVENT_LIST* pResAlarmEventList, IVS_INT32 &iRet, CXml &xml);

	/***********************************************************************************
    * name       : GetAlarmListGetXML
    * description: ƴ�Ӳ�ѯ�澯��ʷ��Ϣ�б�Xml
    * input      : strResult:����ֵ; pResAlarmEventList:�澯�ϱ���Ϣ��ҳ��ѯ�б�; iRet:������; xml;ƴ��XML
    * output     : NA
    * return     : �޷���ֵ
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAlarmInfoGetXML(CString &strResult, const IVS_ALARM_EVENT* pAlarmEvent);

	/***********************************************************************************
    * name       : GetAlarmStatusGetXML
    * description: �澯״̬�ϱ�ƴ��XML
    * input      : pAlarmEventList:�澯�ϱ���Ϣ��ҳ��ѯ�б�; uiBufferSize:�ƻ�������ڴ��С; xml:ƴ�ӵ�xml;
    * output     : NA
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAlarmStatusGetXML(const IVS_ALARM_STATUS_NOTIFY* pAlarmStatusNotify, CXml& xml);

	/***********************************************************************************
    * name       : GetAlarmStatusGetXML
    * description: �澯��Ϣ�ϱ�����XML
    * input      : pAlarmReport:�澯�ϱ��ṹ��; pReqXml��cu��������xml
    * output     : NA
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 ParseAlarmReportXML(const IVS_CHAR* pReqXml, IVS_ALARM_REPORT* pAlarmReport);

	/***********************************************************************************
    * name       : AlarmConfirmParseXML
    * description: �澯ȷ�Ͻ���XML
    * input      : ullAlarmEventID:�澯�¼�Id; pAlarmOperateInfo:�澯������Ϣ�ṹ��; xml:ƴ�ӵ�xml;
    * output     : NA
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 AlarmConfirmParseXML(IVS_UINT64* ullAlarmEventID, IVS_ALARM_OPERATE_INFO* pAlarmOperateInfo, IVS_CHAR* pAlarmInCode, CXml &xml);

	/***********************************************************************************
    * name       : GetDeviceUserList
    * description: ��ȡӵ��ָ���豸Ȩ�޵��û��б�ƴ��XML
    * input      : pResAlarmEventList:�豸�û��б�; xml:ƴ�ӵ�xml;
    * output     : NA
    * return     : NA
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetDeviceUserList(std::list<IVS_DEVICE_USER_INFO_LIST*>& listDevUserInfoList, CString &strResult, IVS_UINT32 uiFromIndex, IVS_UINT32 uiToIndex, CXml &xml);

	/***********************************************************************************
    * name       : AddResultCodeUp
    * description: ��ResultCodeƴ�ڱ��ĵ�ǰ��
    * input      : pRsqXml:SMU���ص�����; xml:ƴ�ӵ�xml;
    * output     : NA
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 AddResultCodeUp(const char* pRsqXml, CString &strResult);
	
	/***********************************************************************************
    * name       : GetUserNum
    * description: �õ��澯��Ȩ����xml��UserID�ĸ���
    * input      : pReqXml:CU��������; 
    * output     : iNum��UserID����
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetUserNum(const char* pReqXml, IVS_INT32& iNum);

	/***********************************************************************************
    * name       : GetDevAlarmListGetXML
    * description: ��ѯOMU�澯�¼��б�ƴװ��Ӧxml
    * input      : xml��Ŀ��xml;
	               pDevAlarmList���澯�¼��б�ṹ��
    * output     : NA
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetDevAlarmListGetXML(CXml& xml, IVS_DEVICE_ALARM_EVENT_LIST* pDevAlarmList, IVS_INT32& iRet);

	/***********************************************************************************
    * name       : DevAlarmNotifyGetXML
    * description: OMU�豸�澯�ϱ�ƴװxml
    * input      : xml��Ŀ��xml;
	               pDevAlarmNotify���豸�澯�ϱ��ṹ��
    * output     : NA
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 DevAlarmNotifyGetXML(CXml& xml, const IVS_DEVICE_ALARM_NOTIFY* pDevAlarmNotify);

    /******************************************************************
     function   : ReLoginGetXML
     description: ƴ���ص�¼�¼�����
     input      : CXml & xml
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    static IVS_INT32 ReLoginGetXML(CXml& xml);

	/******************************************************************
     function   : ParseHelpInfoXml
     description: ɾ������ѯ������Ϣ����������xml
     input      : pHelpInfo�� ����xml
     output     : ulSourceID�� ������ϢĿ��ID
				  ulSourceType�� Ŀ������
				  pDomainCode�� ��ϢĿ�������
     return     : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    static IVS_INT32 ParseHelpInfoXml(const IVS_CHAR* pHelpInfo, IVS_UINT32&  ulSourceID, IVS_UINT32& ulSourceType, IVS_CHAR* pDomainCode);



	/******************************************************************
     function   : GetRspXml
     description: ��ȡ�澯ץ��ͼƬ�б���û����Ӧ��Ϣ��ƴװ��Ӧxml
     input      : iRet ����Ӧ��
	              strResult����Ӧxml
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    static IVS_INT32 GetRspXml(CString& strResult, IVS_INT32 iRet);
};



#endif // __OCX_ALARM_MGR_XML_PROCESS_H__

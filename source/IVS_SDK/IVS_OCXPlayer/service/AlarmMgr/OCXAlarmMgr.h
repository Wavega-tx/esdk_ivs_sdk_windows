/********************************************************************
	filename	: 	OCXAlarmMgr.h
	author		:	z00193167
	created		:	2012/12/06	
	description	:	����澯OCX
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/12/06 ��ʼ�汾
*********************************************************************/

#ifndef __OCX_ALARM_MGR_H__
#define __OCX_ALARM_MGR_H__

#include "hwsdk.h"

class COCXAlarmMgr
{
public:
	COCXAlarmMgr(void);
	~COCXAlarmMgr(void);

	/***********************************************************************************
    * name       : AddAlarmLevel
    * description: �����澯����ocx
    * input      : iSessionId:��¼Id; pReqXml:CU������; strResult:����ֵ
    * output     : strResult:���ظ�CU��xml
    * return     : NA
    * remark     : NA
    ***********************************************************************************/
	static IVS_VOID AddAlarmLevel(IVS_INT32 iSessionId, const IVS_CHAR* pAlarmLevel, CString &strResult);

	/***********************************************************************************
    * name       : ModifyAlarmLevel
    * description: �޸ĸ澯����ocx
    * input      : iSessionId:��¼Id; pReqXml:CU������;
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 ModifyAlarmLevel(IVS_INT32 iSessionId, const IVS_CHAR* pAlarmLevel);

	/***********************************************************************************
    * name       : ModifyAlarmLevel
    * description: �޸ĸ澯����ocx
    * input      : iSessionId:��¼Id; uiAlarmLevelId:CU������(�澯����Id);
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 DeleteAlarmLevel(IVS_INT32 iSessionId, IVS_UINT32 uiAlarmLevelId);

	/***********************************************************************************
    * name       : GetAlarmLevel
    * description: ��ѯ������ϸ��Ϣocx
    * input      : iSessionId:��¼Id; uiAlarmLevelId:CU������(�澯����Id); strResult:����ֵ;
    * output     : strResult:���ظ�CU��xml
    * return     : NA
    * remark     : NA
    ***********************************************************************************/
	static IVS_VOID GetAlarmLevel(IVS_INT32 iSessionId, IVS_INT32 iAlarmLevelID, CString &strResult);

	/***********************************************************************************
    * name       : GetAlarmLevelList
    * description: ��ѯ�����б�ocx
    * input      : iSessionId:��¼Id; strResult:����ֵ;
    * output     : strResult:���ظ�CU��xml
    * return     : NA
    * remark     : NA
    ***********************************************************************************/
	static IVS_VOID GetAlarmLevelList(IVS_INT32 iSessionId, CString &strResult);

	/***********************************************************************************
    * name       : AddHelpInfo
    * description: ���Ӹ澯����ocx
    * input      : iSessionId:��¼Id; pReqXml:CU������;
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 AddHelpInfo(IVS_INT32 iSessionId, const IVS_CHAR* pReqXml);

	/***********************************************************************************
    * name       : ModifyHelpInfo
    * description: �޸ĸ澯����ocx
    * input      : iSessionId:��¼Id; pReqXml:CU������;
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 ModifyHelpInfo(IVS_INT32 iSessionId, const IVS_CHAR* pReqXml);

	/***********************************************************************************
    * name       : DeleteHelpInfo
    * description: ɾ���澯����ocx
    * input      : iSessionId:��¼Id; uiAlarmHelpSourceType:�澯����Դ����; uiAlarmHelpSourceId:�澯����ԴId; 
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 DeleteHelpInfo(IVS_INT32 iSessionId, const IVS_CHAR* pHelpInfo);

	/***********************************************************************************
    * name       : QueryHelpInfo
    * description: ��ѯ�澯������Ϣocx
    * input      : iSessionId:��¼Id; uiAlarmHelpSourceType:�澯����Դ����; uiAlarmHelpSourceId:�澯����ԴId; strResult:����ֵ;
    * output     : strResult:���ظ�CU��xml
    * return     : NA
    * remark     : NA
    ***********************************************************************************/
	static IVS_VOID QueryHelpInfo(IVS_INT32 iSessionId, const IVS_CHAR* pHelpInfo, CString &strResult);

	/***********************************************************************************
    * name       : SetAlarmTypeLevel
    * description: ���ø澯���ͼ���ocx
    * input      : iSessionId:��¼Id; pReqXml:CU������;
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 SetAlarmTypeLevel(IVS_INT32 iSessionId, const IVS_CHAR* pReqXml);

	/***********************************************************************************
    * name       : GetAlarmTypeList
    * description: ��ѯ�澯�����б�ocx
    * input      : iSessionId:��¼Id; pReqXml:CU������; strResult:����ֵ;
    * output     : strResult:���ظ�CU��xml
    * return     : NA
    * remark     : NA
    ***********************************************************************************/
	static IVS_VOID GetAlarmTypeList(IVS_INT32 iSessionId, const IVS_CHAR* pReqXml, CString &strResult);

	/***********************************************************************************
    * name       : SubscribeAlarm
    * description: �����澯����ocx
    * input      : iSessionId:��¼Id; pReqXml:CU������;
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 SubscribeAlarm(IVS_INT32 iSessionId, const IVS_CHAR* pReqXml);

	/***********************************************************************************
    * name       : SubscribeAlarm
    * description: �����澯����ocx
    * input      : iSessionId:��¼Id; pReqXml:CU������; strResult:����ֵ;
    * output     : strResult:���ظ�CU��xml
    * return     : NA
    * remark     : NA
    ***********************************************************************************/
	static IVS_VOID SubscribeAlarmQuery(IVS_INT32 iSessionId, const IVS_CHAR* pReqXml, CString &strResult);

	/***********************************************************************************
    * name       : ModifyAlarmArea
    * description: �޸ĸ澯����ocx
    * input      : iSessionId:��¼Id; pReqXml:CU������;
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 ModifyAlarmArea(IVS_INT32 iSessionId, const IVS_CHAR* pReqXml);

	/***********************************************************************************
    * name       : DeleteAlarmArea
    * description: ɾ���澯����ocx
    * input      : iSessionId:��¼Id; uiAlarmAreaId:CU������;
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 DeleteAlarmArea(IVS_INT32 iSessionId, const IVS_CHAR* pDomainCode, IVS_UINT32 uiAlarmAreaId);

	/***********************************************************************************
    * name       : GetAlarmAreaList
    * description: ��ѯ�澯�����б�ocx
    * input      : iSessionId:��¼Id; pReqXml:CU������; strResult:����ֵ;
    * output     : strResult:���ظ�CU��xml
    * return     : NA
    * remark     : NA
    ***********************************************************************************/
	static IVS_VOID GetAlarmAreaList(IVS_INT32 iSessionId, const IVS_CHAR* pReqXml, CString &strResult);

	/***********************************************************************************
    * name       : UserAuthentication
    * description: �û���֤ocx
    * input      : iSessionId:��¼Id; pPasswordInfo:�û�����;
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 UserAuthentication(IVS_INT32 iSessionId, const IVS_CHAR* pPWD);

	/***********************************************************************************
    * name       : AllocAreaGuardPlan
    * description: ָ�����������ƻ�
    * input      : iSessionId:��¼Id; pReqXml:����xml;
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 AllocAreaGuardPlan(IVS_INT32 iSessionId, const IVS_CHAR* pReqXml);

	/***********************************************************************************
    * name       : ModifyAreaGuardPlan
    * description: �޸ķ��������ƻ�
    * input      : iSessionId:��¼Id; pReqXml:����xml;
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 ModifyAreaGuardPlan(IVS_INT32 iSessionId, const IVS_CHAR* pReqXml);

	/***********************************************************************************
    * name       : DeleteAreaGuardPlan
	* description: ɾ�����������ƻ�
	* input      : iSessionId:��¼Id; ulAlarmAreaId:����ID;
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	***********************************************************************************/
	static IVS_INT32 DeleteAreaGuardPlan(IVS_INT32 iSessionId, const IVS_CHAR* pDomainCode, ULONG ulAlarmAreaId);

	/***********************************************************************************
    * name       : GetAreaGuardPlan
	* description: ��ȡ���������ƻ�
	* input      : iSessionId:��¼Id; ulAlarmAreaId:����ID;
	* output     : RspXml ��Ӧ��Ϣxml
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	***********************************************************************************/
	static IVS_INT32 GetAreaGuardPlan(IVS_INT32 iSessionId, const IVS_CHAR* pDomainCode, ULONG ulAlarmAreaId, CString& strResult);

	/***********************************************************************************
    * name       : AddAlarmArea
	* description: ���ӷ���
	* input      : iSessionId:��¼Id; 
	* output     : xmlRsp ��Ӧ��Ϣxml
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	***********************************************************************************/
	static IVS_INT32 AddAlarmArea(IVS_INT32 iSessionId, const IVS_CHAR* pReqXml,  CString& strResult);

	/***********************************************************************************
    * name       : GetAlarmAreaInfo
	* description: ��ȡ������ϸ��Ϣ
	* input      : iSessionId:��¼Id; pReqXml:������Ϣ;
	* output     : pRspXml ��Ӧ��Ϣ
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	***********************************************************************************/
	static IVS_INT32 GetAlarmAreaInfo(IVS_INT32 iSessionId, const IVS_CHAR* pDomainCode, ULONG ulAlarmAreaId, CString& strResult);

	/***********************************************************************************
    * name       : StartAlarmAreaGuard
	* description: �ֶ�����
	* input      : iSessionId:��¼Id; ulAlarmAreaId:����ID;
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	***********************************************************************************/
	static IVS_INT32 StartAlarmAreaGuard(IVS_INT32 iSessionId, const IVS_CHAR* pDomainCode, ULONG ulAlarmAreaId);
	
	/***********************************************************************************
    * name       : StopAlarmAreaGuard
	* description: �ֶ�����
	* input      : iSessionId:��¼Id; ulAlarmAreaId:����ID;
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	***********************************************************************************/
	static IVS_INT32 StopAlarmAreaGuard(IVS_INT32 iSessionId, const IVS_CHAR* pDomainCode, ULONG ulAlarmAreaId);

	/***********************************************************************************
    * name       : CopySubscribeAlarm
	* description: �澯���ĸ���
	* input      : iSessionId:��¼Id; uiSrcUserID:Դ�û�ID;uiDstUserID��Ŀ���û�ID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	***********************************************************************************/
	static IVS_INT32 AlarmCommission(IVS_INT32 iSessionId, const IVS_CHAR* pReqXml);

	/***********************************************************************************
    * name       : GetAlarmList
	* description: ��ѯ�澯��ʷ��Ϣ�б�
	* input      : iSessionId:��¼Id; pReqXml:CU��XML;strResult:����ֵ
	* output     : NA
	* return     : �޷���ֵ
	* remark     : NA
	***********************************************************************************/
	static IVS_VOID GetAlarmList(IVS_INT32 iSessionId, const IVS_CHAR* pReqXml, CString &strResult);

	/***********************************************************************************
    * name       : GetAlarmInfo
	* description: ��ѯ�澯��Ϣ�¼�
	* input      : iSessionId:��¼Id; uiAlarmEventId:�澯�¼�ID;pAlarmInCode:�澯Դ����; strResult:����ֵ
	* output     : NA
	* return     : �޷���ֵ
	* remark     : NA
	***********************************************************************************/
	static IVS_VOID GetAlarmInfo(IVS_INT32 iSessionId, IVS_UINT64 uiAlarmEventId, const IVS_CHAR* pAlarmInCode, CString &strResult);

	/***********************************************************************************
    * name       : AlarmCancel
	* description: �澯����
	* input      : iSessionId:��¼Id; pReqXml:CU��XML;
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	***********************************************************************************/
	static IVS_INT32 AlarmCancelORConfirm(IVS_INT32 iSessionId, const IVS_CHAR* pReqXml, const IVS_INT32& iType);

	/***********************************************************************************
    * name       : AlarmReport
	* description: �澯��Ϣ�ϱ�
	* input      : iSessionId:��¼Id; uiAlarmEventId:�澯�¼�ID;strResult:����ֵ
	* output     : NA
	* return     : �޷���ֵ
	* remark     : NA
	***********************************************************************************/
	static IVS_INT32 AlarmReport(IVS_INT32 iSessionId, const IVS_CHAR* pReqXML);

	/***********************************************************************************
    * name       : GetDeviceUserList
	* description: ��ȡӵ��ָ���豸Ȩ�޵��û��б�
	* input      : iSessionId:��¼Id; pDevCode:�豸����; uiFromIndex:��ʼ����; uiToIndex:��������; strResult:����ֵ
	* output     : NA
	* return     : �޷���ֵ
	* remark     : NA
	***********************************************************************************/
	static IVS_VOID GetDeviceUserList(IVS_INT32 iSessionId, const IVS_CHAR* pDomainCode, const IVS_CHAR* pDevCode, IVS_UINT32 ulIsUserOnline, IVS_UINT32 uiFromIndex, IVS_UINT32 uiToIndex, CString &strResult);

	/***********************************************************************************
    * name       : GetConfigInfo
	* description: ��ȡƽ̨������Ϣ
	* input      : iSessionId:��¼Id; pReqXml:CU������; strResult:����ֵ
	* output     : NA
	* return     : �޷���ֵ
	* remark     : NA
	***********************************************************************************/
	static IVS_VOID GetConfigInfo(IVS_INT32 iSessionId, const IVS_CHAR* pReqXml, CString &strResult);

		/***********************************************************************************
    * name       : GetAbilityInfo
	* description:��ѯƽ̨��������Ϣ
	* input      : iSessionId:��¼Id
	* output     : NA
	* return     : �޷���ֵ
	* remark     : NA
	***********************************************************************************/
	static IVS_ULONG GetAbilityInfo(IVS_INT32 iSessionId);

	/***********************************************************************************
    * name       : StartAlarmOut
	* description: ��������������澯
	* input      : iSessionId:��¼Id; pAlarmOutCode������������豸����
	* output     : NA
	* return     : �޷���ֵ
	* remark     : NA
	***********************************************************************************/
	static IVS_INT32 StartAlarmOut(IVS_INT32 iSessionId, const IVS_CHAR* pAlarmOutCode);

	/***********************************************************************************
    * name       : StopAlarmOut
	* description: ֹͣ����������澯
	* input      : iSessionId:��¼Id; pAlarmOutCode������������豸����
	* output     : NA
	* return     : �޷���ֵ
	* remark     : NA
	***********************************************************************************/
	static IVS_INT32 StopAlarmOut(IVS_INT32 iSessionId, const IVS_CHAR* pAlarmOutCode);

	/***********************************************************************************
    * name       : GetDeviceAlarmList
	* description: ��ѯOMU�澯�¼��б�
	* input      : iSessionId:��¼Id; pReqXml����ѯ����xml
	* output     : NA
	* return     : �޷���ֵ
	* remark     : NA
	***********************************************************************************/
	static IVS_INT32 GetDeviceAlarmList(IVS_INT32 iSessionId, const IVS_CHAR* pReqXml, CString& strResult);

	/***********************************************************************************
    * name       : SetDeviceGuard
	* description: �豸������
	* input      : iSessionId:��¼Id; pReqXml����ѯ����xml
	* output     : NA
	* return     : �޷���ֵ
	* remark     : NA
	***********************************************************************************/
	static IVS_INT32 SetDeviceGuard(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/***********************************************************************************
    * name       : ResetAlarm
	* description: ������λ
	* input      : iSessionId:��¼Id; pReqXml����ѯ����xml
	* output     : NA
	* return     : �޷���ֵ
	* remark     : NA
	***********************************************************************************/
	static IVS_INT32 ResetAlarm(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/***********************************************************************************
    * name       : ExEventSubscribe
	* description: �ⲿ��澯�¼�����
	* input      : iSessionId:��¼Id; pReqXml����ѯ����xml
	* output     : NA
	* return     : �޷���ֵ
	* remark     : NA
	***********************************************************************************/
	static IVS_INT32 ExEventSubscribe(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/***********************************************************************************
    * name       : GetExEventSubscribe
	* description: ��ѯ�ⲿ��澯�¼�����
	* input      : iSessionId:��¼Id; pReqXml����ѯ����xml
	* output     : NA
	* return     : �޷���ֵ
	* remark     : NA
	***********************************************************************************/
	static IVS_INT32 GetExEventSubscribe(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, CString& strResult);

	/***********************************************************************************
    * name       : ExCatalogSubscribe
	* description: �ⲿ���豸Ŀ¼����
	* input      : iSessionId:��¼Id; pReqXml����ѯ����xml
	* output     : NA
	* return     : �޷���ֵ
	* remark     : NA
	***********************************************************************************/
	static IVS_INT32 ExCatalogSubscribe(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/***********************************************************************************
    * name       : GetExCatalogSubscribeList
	* description: ��ѯ�ⲿ���豸Ŀ¼����
	* input      : iSessionId:��¼Id; pReqXml����ѯ����xml
	* output     : NA
	* return     : �޷���ֵ
	* remark     : NA
	***********************************************************************************/
	static IVS_INT32 GetExCatalogSubscribeList(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, CString& strResult);

   /***********************************************************************************
    * name       : GetAlarmSnapshotList
	* description: ��ȡ�澯ץ��ͼƬ�б�
	* input      : iSessionId:��¼Id; pReqXml����ѯ����xml
	* output     : NA
	* return     : �޷���ֵ
	* remark     : NA
	***********************************************************************************/
	static IVS_INT32 GetAlarmSnapshotList(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, CString& strResult);

	/***********************************************************************************
    * name       : ReleaseListBuffer
	* description: �ͷ�list�е��ڴ�
	* input      : listDevUserInfoList�� list����
	* output     : NA
	* return     : �޷���ֵ
	* remark     : NA
	***********************************************************************************/
	static IVS_INT32 ReleaseListBuffer(std::list<IVS_DEVICE_USER_INFO_LIST*> listDevUserInfoList);
};

#endif

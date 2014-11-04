/*****************************************************************
 filename    :    OCXDevicMgrXMLProcess.h
 author      :    guandiqun
 created     :    2012/11/19
 description :    ʵ���豸;
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/11/19 ��ʼ�汾
*****************************************************************/
#ifndef __OCX_DEVICE_MGR_XML_PROCESS_H__
#define __OCX_DEVICE_MGR_XML_PROCESS_H__
//#include <afx.h>
#include "SDKDef.h"
//#include <wtypes.h>
class COCXDeviceXMLProcess
{
public:
    ~COCXDeviceXMLProcess();

    
    // ������豸�Ľ���xml
    static IVS_INT32 AddDeviceParseXml(CXml &reqXml, IVS_VOID* PVoid, 
        const IVS_UINT32 &uiNum, 
        IVS_CHAR* pSequence);
    
    // �����豸ƴ��XML
    static IVS_INT32 AddDeviceGetXml(CXml &rspXml,
        IVS_VOID *pRspData, 
        const IVS_UINT32 &uiLen, const IVS_CHAR *pDomainCode);

    // ��ȡ���豸�б�ƴ��XML
    static IVS_INT32 GetMainDeviceListGetXml( 
        CXml &rspXml, 
        IVS_VOID* pRspData ,
        IVS_VOID* pDomain);
    
	// ��ȡ������豸�б�ƴ��xml
	static IVS_INT32 GetCamerasGetXml( IVS_CAMERA_BRIEF_INFO_LIST* pRspDeviceList,
		CXml &rspXml);

	// �����ȡ������豸�б�ƴ��xml
	static IVS_INT32 GetAlarmsGetXml( IVS_ALARM_BRIEF_INFO_LIST* pRspDeviceList,
		CXml &rspXml);

	// �����ȡ��Ƶ�豸�豸�б�ƴ��XML
	static IVS_INT32 GetAudiosGetXml( IVS_AUDIO_BRIEF_INFO_LIST* pRspDeviceList,
		CXml &rspXml);
    
    // ��ѯ�豸�б�ƴ��XML
    static IVS_INT32 QueryDeviceListGetXml(IVS_VOID* pRspData ,
        IVS_VOID* pDomain ,
        IVS_UINT32 iDevType, 
        CXml &xmlRsp);

	// ��ȡ�豸�б�ƴ��XML
	static IVS_INT32 GetDeviceListGetXml(IVS_VOID* pRspData ,
		IVS_UINT32 iDevType, 
		CXml &xmlRsp);
      
	// ��ѯNVR�б����XML
    static IVS_INT32 ParseGetNVRListReq(CXml& xml,IVS_CHAR* cDomainCode,
        IVS_INDEX_RANGE &sIndex,
        IVS_UINT32 &uiServerType);

    // ��ѯNVR�б�ƴ��XML
    static IVS_INT32 GetNVRListRsp(IVS_DEVICE_BRIEF_INFO_LIST *pNVRList,
        IVS_INT32 iRetCode,
        CXml& xml);

    // ɾ���豸����XML
    static IVS_INT32 DeleteDevicePraseXML(IVS_DEVICE_CODE* pQueryList, 
	   IVS_CHAR *pDomainCode,
       const IVS_UINT32& uiDeviceNum,
       CXml& xmlReq);
    
    // ɾ���豸ƴ��XML
    static IVS_INT32 DeleteDeviceGetXml(IVS_RESULT_LIST* pResList,
        CXml& xmlRsp, const IVS_CHAR *pDomainCode);
    
	// ������ѯ���豸��ϸ��Ϣ����xml
    static IVS_INT32 GetDeviceInfoParseXML(const IVS_CHAR* pDomainCode,
        const IVS_CHAR* pDevCode,
        CXml& xml);
     
    // ��ѯ���豸��ϸ��Ϣ��Ӧxml
    static IVS_INT32 GetDeviceInfoXML(const IVS_DEVICE_INFO* pDeviceInfo,
        CXml& xml);     
    
	// �豸�ϱ�ƴ��XML
    static IVS_INT32 DeviceStateGetXML(const IVS_DEVICE_STATE* pstDeviceState, CXml& xml);

	// ����DevCode����
    static IVS_INT32 GetDevCodeNum(CXml &xml);
       
	// ��ȡ��·��ƴ��XML
    static IVS_INT32 GetDomainRouteGetXML(IVS_DOMAIN_ROUTE_LIST *pList, 
        CXml &xmlRsp);

	// �������豸��ѯ���豸�б�ƴ��XML
    static IVS_INT32 GetChannelListGetXML(IVS_DEV_CHANNEL_BRIEF_INFO_LIST *pList, CXml &xmlRsp, const IVS_CHAR *pDomainCode);

	// ����ǰ���豸�������XML
	static IVS_INT32 SetDevPWDParseXML(IVS_CHAR* pDevCode, IVS_CHAR* pDevUserName, IVS_CHAR* pDevPWD, IVS_CHAR* pDevRegPWD, CXml &xml);

protected:
private:
    COCXDeviceXMLProcess();
    
};

#endif

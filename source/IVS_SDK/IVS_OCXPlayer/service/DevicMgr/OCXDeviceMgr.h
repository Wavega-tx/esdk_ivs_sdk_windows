/*****************************************************************
 filename    :    OCXDeviceMgr.h
 author      :    guandiqun
 created     :    2012/11/19
 description :    ʵ���豸;
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/11/19 ��ʼ�汾
*****************************************************************/
#ifndef __OCX_DEVICE_MGR_H__
#define __OCX_DEVICE_MGR_H__

#include <afx.h>
#include "SDKDef.h"


class COCXDeviceMgr
{
public:
    ~COCXDeviceMgr();

	// �����豸
    static IVS_INT32 AddDevice(IVS_INT32 iSessionID, CXml& xmlReq, CXml& xmlRsp);

	// ��ѯ�豸�б��������ò�ѯ������
    static IVS_INT32 QueryDeviceList( IVS_INT32 iSessionID, 
                                    IVS_ULONG ulDeviceType, 
                                    CXml& xmlReq, 
                                    CXml& xmlRsp);

	// ��ȡ�豸�б�(�޲�ѯ��������)
	static IVS_INT32 GetDeviceList( IVS_INT32 iSessionID, 
									IVS_ULONG ulDeviceType, 
									IVS_ULONG ulFromIndex, 
									IVS_ULONG ulToIndex,
									CXml& xmlRsp);

	// ��ȡ��·��
    static IVS_VOID GetDomainRoute(IVS_INT32 iSessionID,                                    
                                    CString &strResult);

	// ��ѯ���豸��ϸ��Ϣ
    static IVS_VOID GetDeviceInfo(IVS_INT32 iSessionID,  
                                  CXml& xmlReq,
                                  CString &strResult);
	// ��ѯNVR�б�
    static IVS_VOID GetNVRList(IVS_INT32 iSessionID,  
                               CXml& xmlReq,
                               CString &strResult);
    // �������豸��ѯ���豸�б�
    static IVS_VOID GetChannelList(IVS_INT32 iSessionID, 
                                   const IVS_CHAR *pDevCode,
                                   CString &strResult);

	// �޸����豸����
	static IVS_INT32 SetMainDeviceName(IVS_INT32 iSessionID,
									const IVS_CHAR *pDevCode,
									const IVS_CHAR *pNewDevName);

	// �޸����������
	static IVS_INT32 SetCameraName(IVS_INT32 iSessionID,
									const IVS_CHAR *pCameraCode,
									const IVS_CHAR *pNewCameraName);
    // ɾ���豸
	static IVS_VOID DeleteDevice(IVS_INT32 iSessionID, CString &strResult, LPCTSTR pReqXml);

	// ����ǰ���豸
	static IVS_INT32 RestartDevice(IVS_INT32 iSessionID, const IVS_CHAR* pDevCode, IVS_UINT32 uiRebootType);

	// ��������ǰ���豸
	static IVS_INT32 StartSearchDevice(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_CHAR* pSearchInfo);

	// ֹͣ����ǰ���豸
	static IVS_INT32 StopSearchDevice(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_CHAR* pSearchInfo);

	// ��֤ǰ���豸
	static IVS_VOID VerifyDevice(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_CHAR* pVerifyInfo, CString &strResult);

	// ��ѯǰ����־
	static IVS_VOID GetDevLog(IVS_INT32 iSessionID, const IVS_CHAR* pQueryXml, CString &strResult);

	// ��ѯ�������������
	static IVS_VOID GetDeviceCapability(IVS_INT32 iSessionID, IVS_UINT32 uiCapabilityType, const IVS_CHAR* pDevCode, CString &strResult);

	// ����ǰ���豸����
	static IVS_INT32 SetDevPWD(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

    // ˢ��������б�
    static IVS_INT32 RefreshCameraList(IVS_INT32 iSessionID);

    // ��ѯOnvif�豸��ǰ�˻�ȡ����������Ϊ
    static IVS_INT32 GetCameraEncodeCapabilities(IVS_STREAM_INFO_LIST *pStreamInfoList,IVS_INT32 iRetCord,CXml &xml);

	// ��ѯ��̨VCN����Ƶ������ϼƻ�
	static IVS_INT32 GetVideoDiagnose(IVS_INT32 iSessionID,
		const IVS_CHAR* pDomainCode,
		const IVS_CHAR* pCameraCode,
		IVS_CHAR** pRspXml);

	// ���õ�̨VCN����Ƶ������ϼƻ�
	static IVS_INT32 SetVideoDiagnose(IVS_INT32 iSessionID, 
		const IVS_CHAR* pReqXml);

protected:
private:
    COCXDeviceMgr();
	// �����ڴ�
    static IVS_VOID* GetListObject( IVS_INT32 lDeviceType , IVS_UINT32 uiNum, IVS_UINT32 &uiSizeBuff);

};



#endif

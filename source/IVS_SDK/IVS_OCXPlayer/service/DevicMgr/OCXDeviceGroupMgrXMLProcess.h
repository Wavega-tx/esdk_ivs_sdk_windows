/*****************************************************************
 filename    :    OCXDeviceGroupMgrXMLProcess.h
 author      :    guandiqun
 created     :    2012/11/19
 description :    ʵ���豸��XML��ƴװ�ͽ���;
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/11/19 ��ʼ�汾
*****************************************************************/
#ifndef __OCX_DEVICE_GROUP_PROCESS_H__
#define __OCX_DEVICE_GROUP_PROCESS_H__

#pragma once
#include <afx.h>
#include "SDKDef.h"
#include "OCXXmlProcess.h"

class COCXDeviceGroupMgrXMLProcess
{
public:
    ~COCXDeviceGroupMgrXMLProcess();

    // ��ѯ�豸��info������
	static IVS_UINT32 GetGroupInfoNum(CXml &xmlTemp);

	// ����豸��ƴ��XML
	static IVS_INT32 AddDeviceGroupGetXML(const IVS_UINT32& uiDevGroupID,
		CXml &xml);

	// �����豸�����XML
	static IVS_INT32 AddDeviceGroupParseXML(IVS_DEV_GROUP* pGroup,
		CXml &xml);

	// ɾ���豸�����XML
	static IVS_INT32 DeleteDeviceGroupParseXML(IVS_DEV_GROUP &stDevGroup,
		CXml &xml);

	// �ƶ��豸�����XML
	static IVS_INT32 MoveDeviceGroupParseXML(IVS_DEV_GROUP &stDevGroup,
		CXml& xml);

	// ��ѯ�豸��ƴ��XML
    static IVS_INT32 GetDeviceGroupGetXML(IVS_DEVICE_GROUP_LIST* pDeviceGroupList, IVS_UINT32 uiBufferSize, CXml& xml);

	// ��ѯ�豸�����XML
    static IVS_INT32 GetDeviceGroupParseXML(IVS_CHAR* pDomainCode, IVS_CHAR* pDevGroupCode, CXml& xmlReq);

    // �޸��豸�����ƽ���XML
	static IVS_INT32 ModifyDeviceGroupNamePraseXML(IVS_DEV_GROUP* pDevGroupInfo,
		CXml &xml);

	// �����豸�����������XML
	static IVS_INT32 AddDeviceToGroupPraseXML(IVS_INT32 iSeesionID, 
		IVS_CHAR* pTargetDomainCode,
		IVS_UINT32 uiDeviceNum,
		IVS_DEV_GROUP_CAMERA* pGroupCameraList,
		IVS_UINT32& uiTargetDevGroupID,
		CXml& xmlReq);
	// ɾ���豸�����������XML
	static IVS_INT32 DeleteDeviceFromGroupPraseXML(IVS_INT32 iSeesionID, 
													IVS_CHAR* pTargetDomainCode,
													IVS_UINT32 uiDeviceNum,
													IVS_DEV_GROUP_CAMERA* pGroupCameraList,
													CXml& xmlReq);

	// �����豸�鱸ע��Ϣ����XML
	static IVS_INT32 SetDevGroupRemarkParseXML(IVS_CHAR* pDomainCode, IVS_UINT32 &uiDevGroupID, IVS_CHAR* pDevGroupRemark, CXml &xml);

	// ��ѯ�豸�鱸ע��Ϣ����XML
	static IVS_INT32 GetDevGroupRemarkParseXML(IVS_CHAR* pDomainCode, IVS_UINT32 &uiDevGroupID, CXml &xml);

	// ��ѯ�豸�鱸ע��Ϣƴ��XML
	static IVS_INT32 GetDevGroupRemarkGetXML(const IVS_CHAR* pDevGroupRemark, CString& strResult);

private:
    COCXDeviceGroupMgrXMLProcess();
    
};



#endif

/*****************************************************************
 filename    :    OCXDeviceGroupMgr.cpp
 author      :    guandiqun
 created     :    2012/11/19
 description :    ʵ���豸��;
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/11/19 ��ʼ�汾
*****************************************************************/

#include "OCXDeviceGroupMgr.h"
#include "OCXXmlProcess.h"
#include "OCXDeviceGroupMgrXMLProcess.h"
#include "IVS_Trace.h"

// �����豸��
IVS_VOID COCXDeviceGroupMgr::AddDeviceGroup( IVS_INT32 iSessionID, CXml& xmlReq, CString& strResult )
{
	IVS_DEBUG_TRACE("");

	IVS_DEV_GROUP pGroupInfo;
	memset(&pGroupInfo,0,sizeof(IVS_DEV_GROUP));
	IVS_INT32 iRet = COCXDeviceGroupMgrXMLProcess::AddDeviceGroupParseXML(&pGroupInfo,xmlReq);
	if (IVS_SUCCEED == iRet)
	{
		IVS_UINT32 uiDevGroupID = 0;
		iRet = IVS_SDK_AddDeviceGroup(iSessionID, &pGroupInfo,&uiDevGroupID);
		if (IVS_SUCCEED == iRet)
		{
			CXml xmlRsp;
			(IVS_VOID)COCXDeviceGroupMgrXMLProcess::AddDeviceGroupGetXML(uiDevGroupID,xmlRsp);
			IVS_UINT32 uiLen = 0;
			strResult = xmlRsp.GetXMLStream(uiLen);
		}
		else
		{
			BP_RUN_LOG_ERR(iRet, "Add DeviceGroup ParseXML fail", "IVS_SUCCEED != iRet");
			COCXXmlProcess::GetErrString(strResult, iRet);
		}
	}

	else
	{
		BP_RUN_LOG_ERR(iRet, "Add DeviceGroup ParseXML fail", "IVS_SUCCEED != iRet");
		COCXXmlProcess::GetErrString(strResult, iRet);
	}

	return;
}

// ɾ���豸��
IVS_INT32 COCXDeviceGroupMgr::DeleteDeviceGroup( IVS_INT32 iSessionID, CXml& xmlReq )
{
	IVS_DEBUG_TRACE("");

	IVS_DEV_GROUP stDevGroup;
	memset(&stDevGroup, 0, sizeof(IVS_DEV_GROUP));
	IVS_INT32 iRet = COCXDeviceGroupMgrXMLProcess::DeleteDeviceGroupParseXML(stDevGroup, xmlReq);
	if (IVS_SUCCEED == iRet)
	{
		iRet = IVS_SDK_DeleteDeviceGroup(iSessionID, stDevGroup.cDomainCode, stDevGroup.uiID);  
	}   

	return iRet;
}

// �ƶ��豸��
IVS_INT32 COCXDeviceGroupMgr::MoveDeviceGroup( IVS_INT32 iSessionID, CXml& xmlReq )
{
	IVS_DEBUG_TRACE("");

	IVS_DEV_GROUP stDevGroup;
	memset(&stDevGroup, 0, sizeof(IVS_DEV_GROUP));
	IVS_INT32 iRet = COCXDeviceGroupMgrXMLProcess::MoveDeviceGroupParseXML(stDevGroup, xmlReq);
	if (IVS_SUCCEED == iRet)
	{
		iRet = IVS_SDK_MoveDeviceGroup(iSessionID, stDevGroup.cDomainCode, stDevGroup.uiParentID, stDevGroup.uiID);  
	}   

	return iRet;
}

// ��ѯ�豸���б���Ϣ
IVS_VOID COCXDeviceGroupMgr::GetDeviceGroup( IVS_INT32 iSessionID, CXml& reqXml, CString& strResult )
{
    IVS_DEBUG_TRACE("");

    //������
    IVS_CHAR szDomainCode[IVS_DOMAIN_CODE_LEN + 1];
    memset(szDomainCode, 0, IVS_DOMAIN_CODE_LEN + 1);
    IVS_CHAR cDevGroupCode[IVS_DEVICE_GROUP_LEN + 1] = {0};

    // ����xml,��ʧ��
    IVS_INT32  iRet = COCXDeviceGroupMgrXMLProcess::GetDeviceGroupParseXML(szDomainCode, cDevGroupCode, reqXml);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Get DeviceGroup ParseXML fail", "IVS_SUCCEED != iRet");
        COCXXmlProcess::GetErrString(strResult, iRet);
        return; 
    }
	// �����ڴ�
    IVS_UINT32 uiBufferSize= sizeof(IVS_DEVICE_GROUP_LIST) + sizeof(IVS_DEVICE_GROUP) * (IVS_MAX_DEV_GROUP_NUM - 1);    
    IVS_VOID *pBufferRsp = IVS_NEW((IVS_CHAR* &)pBufferRsp, uiBufferSize);
    if (NULL == pBufferRsp)
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "'new pBufferRsp' fail", "NULL == pBufferRsp");
        COCXXmlProcess::GetErrString(strResult, IVS_ALLOC_MEMORY_ERROR);
        return; 
    }  

    IVS_DEVICE_GROUP_LIST* pDeviceGroupList = static_cast<IVS_DEVICE_GROUP_LIST*>(pBufferRsp);

    // ����SDK�ӿ�
	iRet = IVS_SDK_GetDeviceGroup(iSessionID, szDomainCode, cDevGroupCode, pDeviceGroupList, uiBufferSize);
    if (IVS_SUCCEED == iRet)
    {       
        CXml xmlRsp;
        //��pRspData����װxml
        iRet = COCXDeviceGroupMgrXMLProcess::GetDeviceGroupGetXML(pDeviceGroupList, uiBufferSize, xmlRsp);
        if (IVS_SUCCEED == iRet)
        {
            IVS_UINT32 iLen = 0;
            strResult = xmlRsp.GetXMLStream(iLen);
        }      
    } 
	IVS_DELETE(pBufferRsp, MUILI);
    if (IVS_SUCCEED != iRet)
    {
        COCXXmlProcess::GetErrString(strResult, iRet);
    }    

	return;
}

// ɾ���豸�������
IVS_INT32 COCXDeviceGroupMgr::DeleteDeviceFromGroup( IVS_INT32 iSessionID, CXml& xmlReq)
{
	IVS_DEBUG_TRACE("");

    IVS_UINT32 uiNum = COCXXmlProcess::GetXmlDevinfoNum(xmlReq);

    //�鿴�ж��ٸ�info �������Ϊ0����ô�ͷ���IVS_SMU_XML_NODE_VALUE_NOT_EXIST
    if (0 == uiNum)
    {
        BP_RUN_LOG_ERR(IVS_XML_INVALID, "Delete DeviceFromGroup fail", "iNum == 0");
        return IVS_XML_INVALID;
    }  

    IVS_UINT32 uiReqBufferSize = sizeof(IVS_DEV_GROUP_CAMERA)*uiNum;
    IVS_VOID* pBuffer = IVS_NEW((IVS_CHAR* &)pBuffer, uiReqBufferSize); 
    if (NULL == pBuffer)
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "'new pCameraList' fail", "NULL == pCameraList");
        return IVS_ALLOC_MEMORY_ERROR;       
    }
    memset(pBuffer, 0, uiReqBufferSize);

    IVS_CHAR chDomainCode[IVS_DOMAIN_CODE_LEN + 1] ;  
    memset(chDomainCode, 0, IVS_DOMAIN_CODE_LEN + 1);
	IVS_DEV_GROUP_CAMERA* pCameraList = static_cast<IVS_DEV_GROUP_CAMERA*>(pBuffer);
    //����xml���浽pReqData��
    IVS_INT32 iRet = COCXDeviceGroupMgrXMLProcess::DeleteDeviceFromGroupPraseXML(iSessionID, chDomainCode, uiNum, pCameraList, xmlReq);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Delete DeviceFromGroup ParseXml FALSE", "NA");
        IVS_DELETE(pBuffer, MUILI);     
        return iRet;
    } 
    
   iRet = IVS_SDK_DeleteDeviceFromGroup(iSessionID, chDomainCode, uiNum,  pCameraList);
   IVS_DELETE(pBuffer, MUILI); 

    return iRet;
}

// �޸��豸������
IVS_INT32 COCXDeviceGroupMgr::ModifyDeviceGroupName( IVS_INT32 iSessionID, CXml& xmlReq )
{
	IVS_DEBUG_TRACE("");

	IVS_DEV_GROUP stDevGroupInfo;
	memset(&stDevGroupInfo,0,sizeof(IVS_DEV_GROUP));
	//����xml
	IVS_INT32 iRet = COCXDeviceGroupMgrXMLProcess::ModifyDeviceGroupNamePraseXML(&stDevGroupInfo, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "ModifyDeviceGroupName PraseXML fail", "IVS_SUCCEED != iRet");
		return iRet;
	}

	//����sdk�Ľӿ�    
	iRet = IVS_SDK_ModifyDeviceGroupName(iSessionID, stDevGroupInfo.cDomainCode, stDevGroupInfo.uiID, stDevGroupInfo.cName);
    return iRet;
}

// �����豸�������
IVS_INT32 COCXDeviceGroupMgr::AddDeviceToGroup( IVS_INT32 iSessionID, CXml& xmlReq )
{
	IVS_DEBUG_TRACE("");

	IVS_UINT32 uiNum = COCXXmlProcess::GetXmlDevinfoNum(xmlReq);

	// �鿴�ж��ٸ�info �������Ϊ0����ô�ͷ���IVS_SMU_XML_NODE_VALUE_NOT_EXIST
	if (0 == uiNum)
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "Add DeviceToGroup fail", "iNum == 0");
		return IVS_XML_INVALID;
	}  

	IVS_UINT32 uiReqBufferSize = sizeof(IVS_DEV_GROUP_CAMERA)*uiNum;
	IVS_VOID* pBuffer = IVS_NEW((IVS_CHAR* &)pBuffer, uiReqBufferSize); 
	if (NULL == pBuffer)
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "'new pCameraList' fail", "NULL == pCameraList");
		return IVS_ALLOC_MEMORY_ERROR;       
	}
	memset(pBuffer, 0, uiReqBufferSize);

	IVS_CHAR chDomainCode[IVS_DOMAIN_CODE_LEN + 1] ;  
	memset(chDomainCode, 0, IVS_DOMAIN_CODE_LEN + 1);

	IVS_DEV_GROUP_CAMERA* pCameraList = static_cast<IVS_DEV_GROUP_CAMERA*>(pBuffer);
	IVS_UINT32 uiTargetGroupID = 0;
	//����xml���浽pReqData��
	IVS_INT32 iRet = COCXDeviceGroupMgrXMLProcess::AddDeviceToGroupPraseXML(iSessionID, chDomainCode, uiNum, pCameraList, uiTargetGroupID, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Add DeviceToGroup PraseXML FALSE", "NA");
		IVS_DELETE(pBuffer, MUILI);     
		return iRet;
	} 

	iRet = IVS_SDK_AddDeviceToGroup(iSessionID, chDomainCode, uiTargetGroupID, uiNum,  pCameraList);
	IVS_DELETE(pBuffer, MUILI); 

	return iRet;
}

#define IVS_GROUP_MAX_REMARK_LEN 324
// �����豸�鱸ע��Ϣ
IVS_INT32 COCXDeviceGroupMgr::SetDevGroupRemark(IVS_INT32 iSessionID, CXml& xmlReq)
{
	IVS_DEBUG_TRACE("");

	IVS_CHAR cDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
	IVS_UINT32 uiDevGroupID = 0;
	IVS_CHAR cDevGroupRemark[IVS_GROUP_MAX_REMARK_LEN + 1] = {0};
	IVS_INT32 iRet = COCXDeviceGroupMgrXMLProcess::SetDevGroupRemarkParseXML(cDomainCode, uiDevGroupID, cDevGroupRemark, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set DevGroupRemark ParseXML fail", "NA");
		return iRet;
	}
	iRet = IVS_SDK_SetDevGroupRemark(iSessionID, cDomainCode, uiDevGroupID, cDevGroupRemark);

	return iRet;
}

// ��ѯ�豸�鱸ע��Ϣ
IVS_VOID COCXDeviceGroupMgr::GetDevGroupRemark(IVS_INT32 iSessionID,  CXml& xmlReq, CString& strResult)
{
	IVS_DEBUG_TRACE("");

	IVS_CHAR cDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
	IVS_UINT32 uiDevGroupID = 0;
	IVS_CHAR cDevGroupRemark[IVS_GROUP_MAX_REMARK_LEN + 1] = {0};
	IVS_UINT32 uiRemarkLen = IVS_GROUP_MAX_REMARK_LEN;
	IVS_INT32 iRet = COCXDeviceGroupMgrXMLProcess::GetDevGroupRemarkParseXML(cDomainCode, uiDevGroupID, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get DevGroupRemark ParseXML fail", "NA");
		COCXXmlProcess::GetErrString(strResult, iRet);
		return;
	}

	iRet = IVS_SDK_GetDevGroupRemark(iSessionID, cDomainCode, uiDevGroupID, cDevGroupRemark, uiRemarkLen);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "IVS_SDK_GetDevGroupRemark fail", "NA");
		COCXXmlProcess::GetErrString(strResult, iRet);
		return;
	}
	iRet = COCXDeviceGroupMgrXMLProcess::GetDevGroupRemarkGetXML(cDevGroupRemark, strResult);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get DevGroupRemark GetXML fail", "NA");
		COCXXmlProcess::GetErrString(strResult, iRet);
		return;
	}

	return;
}

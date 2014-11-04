#include "stdafx.h"
#include "IVS_OCXPlayerCtrl.h"
#include "VideoPaneMgr.h"
#include "hwsdk.h"
#include "ToolsHelp.h"
#include "ivs_error.h"
#include "OCXUserGroupMgr.h"
#include "OCXUserInfoMgrXMLProcess.h"
#include "IVS_Trace.h"

COCXUserGroupMgr::COCXUserGroupMgr()
{
}

COCXUserGroupMgr::~COCXUserGroupMgr()
{
}

// �����û���
CString COCXUserGroupMgr::AddUserGroup(IVS_INT32 iSessionID, LPCTSTR pUserGroup)
{
    IVS_DEBUG_TRACE("");

    CString strResult = NULL;
    IVS_INT32 iRet = IVS_FAIL;
    IVS_USER_GROUP stUserGroup;
    memset(&stUserGroup, 0, sizeof(IVS_USER_GROUP));

    iRet = COCXUserInfoMgrXMLProcess::UserGroupParseXML(pUserGroup, stUserGroup);
    if(IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "UserGroup ParseXML", "Error");
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }

    // ���ӳ���
    IVS_UINT32 puiUserGroupID = 0;  
    iRet = IVS_SDK_AddUserGroup(iSessionID, &stUserGroup, &puiUserGroupID);

    if(IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Add UserGroup", "ErrorCode");
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }

    CXml xmlRsp;
    (void)COCXUserInfoMgrXMLProcess::UserGroupSetXML(xmlRsp, puiUserGroupID, iRet);
    unsigned int iXMLSize = 0;
    strResult = xmlRsp.GetXMLStream(iXMLSize);

    return	strResult;
}

// ɾ���û���
LONG COCXUserGroupMgr::DeleteUserGroup(IVS_INT32 iSessionID, LPCTSTR pDomainCode, ULONG ulUserGroupID)
{
    IVS_DEBUG_TRACE("");
    IVS_INT32 iRet = IVS_SDK_DeleteUserGroup(iSessionID, pDomainCode, (IVS_UINT32)ulUserGroupID);
    return iRet;
}

// �޸��û���
LONG COCXUserGroupMgr::ModifyUserGroup(IVS_INT32 iSessionID, LPCTSTR pUserGroup)
{    
    CHECK_POINTER(pUserGroup, IVS_FAIL);
    IVS_DEBUG_TRACE("");

    IVS_INT32 iRet = IVS_FAIL;
    IVS_USER_GROUP stUserGroup;
    memset(&stUserGroup, 0, sizeof(IVS_USER_GROUP));

    iRet = COCXUserInfoMgrXMLProcess::UserGroupParseXML(pUserGroup, stUserGroup);
    if(IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Modify UserGroup", "Error");
        return iRet;
    }
    iRet = IVS_SDK_ModifyUserGroup(iSessionID, &stUserGroup);
    return iRet;
}

// �ƶ��û���
LONG COCXUserGroupMgr::MoveUserGroup(IVS_INT32 iSessionID, LPCTSTR pUserGroup)
{
    CHECK_POINTER(pUserGroup,IVS_FAIL);
    IVS_DEBUG_TRACE("");

    //1.STR תΪ XML
    CXml xml;
    (void)xml.Parse(pUserGroup);

    //2.XML תΪ struct
    IVS_CHAR szDomainCode[IVS_DOMAIN_CODE_LEN+1] = {0};
    IVS_UINT32 uiTargetDevGroupID = 0;
    IVS_USER_GROUP_LIST* pUserGroupList = NULL;

    (void)COCXUserInfoMgrXMLProcess::MoveUserGroupPraseXML(szDomainCode, uiTargetDevGroupID, pUserGroupList, xml);

    //3.����strct ���� sdk
    //��װSDK
    IVS_INT32 iRet = IVS_SDK_MoveUserGroup(iSessionID, szDomainCode, uiTargetDevGroupID, pUserGroupList);
	if (NULL != pUserGroupList)
	{
		IVS_DELETE(pUserGroupList, MUILI);
	}
    return iRet;
}

// ��ѯ�û����б�
CString COCXUserGroupMgr::GetUserGroupList(IVS_INT32 iSessionID, LPCTSTR pReqXml)
{
    IVS_DEBUG_TRACE("");

    CString strResult;
    IVS_INT32 iRet = IVS_FAIL;

    if(NULL == pReqXml)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Get UserGroup List", "pReqXml is null");
        COCXXmlProcess::GetErrString(strResult, IVS_PARA_INVALID);
        return strResult;
    }

    // �ж��Ƿ������ȷ
    CXml reqXml;    
    if (!reqXml.Parse(pReqXml))
    {
        BP_RUN_LOG_ERR(IVS_XML_INVALID, "Get UserGroup List", "xml parsing error");
        COCXXmlProcess::GetErrString(strResult, IVS_XML_INVALID);
        return strResult;
    }

    // ����ͨ�õķ���:����������ͨ�ò�ѯ����ṹ��ռ�
    IVS_VOID *pReqBuffer = NULL;
    IVS_CHAR cDomainCode[IVS_DOMAIN_CODE_LEN+1] = {0};
    IVS_INT32 iFieldNum = COCXXmlProcess::GetFieldNum(reqXml);
    iRet = COCXXmlProcess::GetUnifiedFormatQueryParseXMLEx(pReqBuffer, reqXml, iFieldNum, cDomainCode);
    if (IVS_SUCCEED != iRet)
    {
        IVS_DELETE(pReqBuffer, MUILI);
        BP_RUN_LOG_ERR(iRet, "Get UserGroup List", "xml parsing error");
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    IVS_QUERY_UNIFIED_FORMAT *pQueryFormat = reinterpret_cast<IVS_QUERY_UNIFIED_FORMAT*>(pReqBuffer);

	if(NULL == pQueryFormat)
	{
		BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "Get UserGroup List", "pQueryFormat is null");
		COCXXmlProcess::GetErrString(strResult, IVS_OPERATE_MEMORY_ERROR);
		return strResult;
	}

	// ���䷵���б����ݽṹ�ռ�
    IVS_UINT32 iItemNum = (pQueryFormat->stIndex.uiToIndex - pQueryFormat->stIndex.uiFromIndex) + 1;
    IVS_UINT32 iBufferSize = sizeof(IVS_USER_GROUP_LIST) + sizeof(IVS_USER_GROUP) * iItemNum;

    IVS_CHAR *pBuffer = IVS_NEW(pBuffer,iBufferSize);
	if (NULL == pBuffer)
	{
		IVS_DELETE(pReqBuffer, MUILI);
		return NULL;
	}
    memset(pBuffer, 0, iBufferSize);
    IVS_USER_GROUP_LIST *pUserGroupList = reinterpret_cast<IVS_USER_GROUP_LIST*>(pBuffer);//lint !e826

    //��ȡlist    
    iRet = IVS_SDK_GetUserGroupList(iSessionID, cDomainCode, &pQueryFormat->stIndex, pUserGroupList, iBufferSize);
    if(IVS_SUCCEED == iRet)
    {
        CXml rspXml;
        COCXUserInfoMgrXMLProcess::GetUserGroupListGetXML(pUserGroupList, iRet, rspXml);
        IVS_UINT32 iLen = 0;
        strResult = rspXml.GetXMLStream(iLen);
    }
    else
    {
        COCXXmlProcess::GetErrString(strResult, iRet);
    }     

    IVS_DELETE(pReqBuffer, MUILI);
    IVS_DELETE(pBuffer, MUILI);
    return strResult;
}

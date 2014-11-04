#include "stdafx.h"
#include "SDKDef.h"
#include "VideoPaneMgr.h"
#include "hwsdk.h"
#include "ToolsHelp.h"
#include "ivs_error.h"
#include "OCXUserInfoMgr.h"
#include "OCXUserInfoMgrXMLProcess.h"
#include "IVS_Trace.h"

COCXUserInfoMgr::COCXUserInfoMgr(void)
{
}

COCXUserInfoMgr::~COCXUserInfoMgr(void)
{
}

// ����û���Ϣ
CString COCXUserInfoMgr::AddUser(IVS_INT32 iSessionID, LPCTSTR pReqXml)
{
    IVS_DEBUG_TRACE("");
    CString strResult = NULL;
    IVS_INT32 iRet = IVS_FAIL;
    IVS_USER_INFO stUserInfo;
    memset(&stUserInfo, 0, sizeof(IVS_USER_INFO));

    iRet = COCXUserInfoMgrXMLProcess::GetUserInfoParseXml(pReqXml, stUserInfo);
    if(IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Get UserInfo ParseXml", "Error");
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }

    IVS_UINT32 iUserId = 0;    
    iRet = IVS_SDK_AddUser(iSessionID, &stUserInfo, &iUserId);        
    if(IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Add User", "ErrorCode");
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }

    CXml rspXml;
    COCXUserInfoMgrXMLProcess::AddUserGetXml(iUserId, iRet, rspXml);
    IVS_UINT32 iLen = 0;
    strResult = rspXml.GetXMLStream(iLen); 
    return strResult;
}

// ɾ���û�
LONG COCXUserInfoMgr::DeleteUser(IVS_INT32 iSessionID, LPCTSTR pDomainCode, ULONG ulUserID)
{
    IVS_DEBUG_TRACE("");
    IVS_INT32 iRet = IVS_FAIL;
    iRet = IVS_SDK_DeleteUser(iSessionID, pDomainCode, ulUserID);
    return iRet;
}

// �����û���Ϣ
LONG COCXUserInfoMgr::ModifyUser(IVS_INT32 iSessionID, LPCTSTR pReqXml)
{
    IVS_DEBUG_TRACE("");
    IVS_INT32 iRet = IVS_FAIL;
    IVS_USER_INFO stUserInfo;
    memset(&stUserInfo, 0, sizeof(IVS_USER_INFO));

    iRet = COCXUserInfoMgrXMLProcess::GetUserInfoParseXml(pReqXml, stUserInfo);
    if(IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Get UserInfo ParseXml", "Error");
        return iRet;
    }

    iRet = IVS_SDK_ModifyUser(iSessionID, &stUserInfo);
    return iRet;
}

// ��ѯ�û���Ϣ
CString COCXUserInfoMgr::GetUserInfo(IVS_INT32 iSessionID, LPCTSTR pDomainCode, ULONG ulUserID)
{
    IVS_DEBUG_TRACE("");

    CString strResult = NULL;
    IVS_INT32 iRet = IVS_SUCCEED;
    IVS_USER_INFO stUserInfo;
    memset(&stUserInfo, 0, sizeof(IVS_USER_INFO));

    iRet = IVS_SDK_GetUserInfo(iSessionID, pDomainCode, ulUserID, &stUserInfo);

    if(IVS_SUCCEED == iRet)
    {
        CXml rspXml;
        COCXUserInfoMgrXMLProcess::GetUserInfoGetXml(&stUserInfo, iRet, rspXml);     
        IVS_UINT32 iLen = 0;
        strResult = rspXml.GetXMLStream(iLen);
    }
    else
    {
        BP_RUN_LOG_ERR(iRet, "Get User Info", "Error");
        COCXXmlProcess::GetErrString(strResult, iRet);
    }
    return strResult;
}

// ����/�����û�
LONG COCXUserInfoMgr::LockUser(IVS_INT32 iSessionID, LPCTSTR pDomainCode, ULONG ulUserID, ULONG ulLock)
{
    IVS_DEBUG_TRACE("");
    IVS_INT32 iRet = IVS_FAIL;
    iRet = IVS_SDK_LockUser(iSessionID, pDomainCode, ulUserID, (IVS_INT32)ulLock);
    return iRet;
}

// ��ѯ�û��б�
CString COCXUserInfoMgr::GetUserList(IVS_INT32 iSessionID, LPCTSTR pUnifiedQuery)
{
    IVS_DEBUG_TRACE("");

    CString strResult;
    IVS_INT32 iRet = IVS_FAIL;

    if(NULL == pUnifiedQuery)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Get User List", "pUnifiedQuery is null");
        COCXXmlProcess::GetErrString(strResult, IVS_PARA_INVALID);
        return strResult;
    }

    // �ж��Ƿ������ȷ
    CXml reqXml;
    if (!reqXml.Parse(pUnifiedQuery))
    {
        BP_RUN_LOG_ERR(IVS_XML_INVALID, "Xml Parse", "error");
        COCXXmlProcess::GetErrString(strResult, IVS_XML_INVALID);
        return strResult;
    }

    // ����ͨ�õķ���:����������ͨ�ò�ѯ����ṹ��ռ�
    IVS_VOID *pReqBuffer = NULL;
    IVS_CHAR cDomainCode[IVS_DOMAIN_CODE_LEN+1] = {0};
    IVS_INT32 iNum = COCXXmlProcess::GetFieldNum(reqXml);
    iRet = COCXXmlProcess::GetUnifiedFormatQueryParseXMLEx(pReqBuffer, reqXml, iNum, cDomainCode);
    if (IVS_SUCCEED != iRet)
    {
        IVS_DELETE(pReqBuffer, MUILI);
        BP_RUN_LOG_ERR(iRet, "Get Unified Format Query ParseXML", "Error");
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    IVS_QUERY_UNIFIED_FORMAT *pQuery = reinterpret_cast<IVS_QUERY_UNIFIED_FORMAT*>(pReqBuffer);

	if(NULL == pQuery)
	{
		BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "Get User List", "pQuery is null");
		COCXXmlProcess::GetErrString(strResult, IVS_OPERATE_MEMORY_ERROR);
		return strResult;
	}

	// ���䷵���б����ݽṹ�ռ�
    IVS_UINT32 iItemNum = (pQuery->stIndex.uiToIndex - pQuery->stIndex.uiFromIndex) + 1;
    IVS_UINT32 iBufferSize = sizeof(IVS_USER_INFO_LIST) + sizeof(IVS_USER_INFO) * iItemNum;

    IVS_CHAR *pBuffer = IVS_NEW(pBuffer, iBufferSize);
	if (NULL == pBuffer)
	{
		IVS_DELETE(pReqBuffer, MUILI);
		return NULL;
	}
   
    memset(pBuffer, 0, iBufferSize);
    IVS_USER_INFO_LIST *pUserInfoList = reinterpret_cast<IVS_USER_INFO_LIST*>(pBuffer);//lint !e826

    // ��ȡlist    
    iRet = IVS_SDK_GetUserList(iSessionID, cDomainCode, pQuery, pUserInfoList, iBufferSize);
    if(iRet == IVS_SUCCEED)
    {
        CXml rspXml;
        COCXUserInfoMgrXMLProcess::GetUserListGetXml(pUserInfoList, iRet, rspXml);
        IVS_UINT32 iLen = 0;
        strResult = rspXml.GetXMLStream(iLen);
    }
    else
    {
        BP_RUN_LOG_ERR(iRet, "Get User List", "Error");
        COCXXmlProcess::GetErrString(strResult, iRet);
    }     

    IVS_DELETE(pReqBuffer, MUILI);
    IVS_DELETE(pBuffer, MUILI);
    return strResult;
}

// ��ѯ�����û��б�
CString COCXUserInfoMgr::GetOnlineUser(IVS_INT32 iSessionID, LPCTSTR pUnifiedQuery)
{
    IVS_DEBUG_TRACE("");
    
    IVS_INT32 iRet = IVS_SUCCEED;
    CString strResult;
    if(NULL == pUnifiedQuery)
    {
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "Get Online User", "pUnifiedQuery is null");
        COCXXmlProcess::GetErrString(strResult, IVS_OPERATE_MEMORY_ERROR);
        return strResult;
    }

    // �ж��Ƿ������ȷ
    CXml reqXml;
    if (!reqXml.Parse(pUnifiedQuery))
    {
        //��¼��־
        BP_RUN_LOG_ERR(IVS_XML_INVALID, "CXml Parse", "xml parsing error");
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
        //��¼��־
        IVS_DELETE(pReqBuffer, MUILI);
        BP_RUN_LOG_ERR(iRet, "Get Unified Format Query ParseXML", "Error");
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    IVS_QUERY_UNIFIED_FORMAT *pQueryFormat = reinterpret_cast<IVS_QUERY_UNIFIED_FORMAT*>(pReqBuffer);

    // ���䷵���б����ݽṹ�ռ�
    IVS_UINT32 iItemNum = (pQueryFormat->stIndex.uiToIndex - pQueryFormat->stIndex.uiFromIndex) + 1;
    IVS_UINT32 iBufferSize = sizeof(IVS_ONLINE_USER_LIST) + sizeof(IVS_ONLINE_USER) * iItemNum;

	IVS_CHAR *pBuffer = IVS_NEW(pBuffer, iBufferSize);
	if (NULL == pBuffer)
	{
		IVS_DELETE(pReqBuffer, MUILI);
		return NULL;
	}
    memset(pBuffer, 0, iBufferSize);
    IVS_ONLINE_USER_LIST *pOnlineUserList = reinterpret_cast<IVS_ONLINE_USER_LIST*>(pBuffer);//lint !e826

    // ��ȡlist    
    iRet = IVS_SDK_GetOnlineUser(iSessionID, cDomainCode, pQueryFormat, pOnlineUserList, iBufferSize);
    if(IVS_SUCCEED == iRet)
    {
        CXml rspXml;
        COCXUserInfoMgrXMLProcess::GetOnlineUserListGetXml(pOnlineUserList, iRet, rspXml);        
        IVS_UINT32 iLen = 0;
        strResult = rspXml.GetXMLStream(iLen);
    }
    else
    {
        BP_RUN_LOG_ERR(iRet, "Get Online User", "ErrorCode");
        COCXXmlProcess::GetErrString(strResult, iRet);
    }     

    IVS_DELETE(pReqBuffer, MUILI);
    IVS_DELETE(pBuffer, MUILI);
    return strResult;
}

// �޸�����
LONG COCXUserInfoMgr::ChangePWD(IVS_INT32 iSessionID, LPCTSTR pOldPWD, LPCTSTR pNewPWD)
{
    IVS_DEBUG_TRACE("");
    IVS_INT32 iRet = IVS_FAIL;
    iRet = IVS_SDK_ChangePWD(iSessionID, pOldPWD, pNewPWD);
    return iRet;
}

// ��������
LONG COCXUserInfoMgr::ResetPWD(IVS_INT32 iSessionID, LPCTSTR pDomainCode, ULONG ulUserID, LPCTSTR pNewPWD)
{
    IVS_DEBUG_TRACE("");
    IVS_INT32 iRet = IVS_FAIL;
    iRet = IVS_SDK_ResetPWD(iSessionID, pDomainCode, ulUserID, pNewPWD);
    return iRet;
}

// У������������Ƿ�����˺Ź���
LONG COCXUserInfoMgr::CheckPWDByRole(IVS_INT32 iSessionID, LPCTSTR pDomainCode, ULONG ulRoleID, LPCTSTR pLoginName, LPCTSTR pPWD)
{
    IVS_DEBUG_TRACE("");
    IVS_INT32 iRet = IVS_FAIL;
    iRet = IVS_SDK_CheckPWDByRole(iSessionID, pDomainCode, ulRoleID, pLoginName, pPWD);
    return iRet;
}

// ���ݽ�ɫ��ȡ�����˺Ź��������
CString COCXUserInfoMgr::GetAutoCreatePWD(IVS_INT32 iSessionID, LPCTSTR pDomainCode, ULONG ulRoleID, LPCTSTR pLoginName)
{
    IVS_DEBUG_TRACE("");

    CString strResult = NULL;
    IVS_INT32 iRet = IVS_SUCCEED;
    IVS_CHAR szPWD[IVS_PWD_LEN+1]={0};

    iRet = IVS_SDK_GetAutoCreatePWD(iSessionID, pDomainCode, ulRoleID, pLoginName, szPWD, IVS_PWD_LEN+1);

    if(IVS_SUCCEED == iRet)
    {
        CXml rspXml;
        COCXUserInfoMgrXMLProcess::GetAutoCreatePWDGetXml(szPWD, iRet, rspXml);
        IVS_UINT32 iLen = 0;
        strResult = rspXml.GetXMLStream(iLen);
    }
    else
    {
        BP_RUN_LOG_ERR(iRet, "Get Auto Create PWD", "ErrorCode");
        COCXXmlProcess::GetErrString(strResult, iRet);
    }

    return strResult;
}

// ��ȡ����ǿ��
CString COCXUserInfoMgr::GetPWDStrength(LPCTSTR pPWD)
{
    IVS_DEBUG_TRACE("");

    CString strResult = NULL;
    IVS_INT32 iRet = IVS_FAIL;

    IVS_UINT32 uiPWDStrength = 0;
    iRet = IVS_SDK_GetPWDStrength(pPWD, &uiPWDStrength);

    if(IVS_SUCCEED == iRet)
    {
        CXml rspXml;
        COCXUserInfoMgrXMLProcess::GetPWDStrengthGetXml(uiPWDStrength, iRet, rspXml);     
        IVS_UINT32 iLen = 0;
        strResult = rspXml.GetXMLStream(iLen);
    }
    else
    {
        BP_RUN_LOG_ERR(iRet, "Get PWD Strength", "ErrorCode");
        COCXXmlProcess::GetErrString(strResult, iRet);
    }

    return strResult;
}

// ��ȡ���������Ϣ
CString COCXUserInfoMgr::GetPWDExpireInfo(IVS_INT32 iSessionID)
{
    IVS_DEBUG_TRACE("");

    CString strResult = NULL;
    IVS_INT32 iRet = IVS_FAIL;

    IVS_PWD_EXPIRE_INFO stPWDExpireInfo;
    memset(&stPWDExpireInfo, 0, sizeof(IVS_PWD_EXPIRE_INFO));
    iRet = IVS_SDK_GetPWDExpireInfo(iSessionID, &stPWDExpireInfo);

    if(IVS_SUCCEED == iRet)
    {
        CXml rspXml;
        COCXUserInfoMgrXMLProcess::GetPWDExpireInfoGetXml(&stPWDExpireInfo, iRet, rspXml);     
        IVS_UINT32 iLen = 0;
        strResult = rspXml.GetXMLStream(iLen);
    }
    else
    {
        BP_RUN_LOG_ERR(iRet, "Get PWD Strength", "ErrorCode");
        COCXXmlProcess::GetErrString(strResult, iRet);
    }

    return strResult;
}

// �����û��豸�б�
LONG COCXUserInfoMgr::SetUserDeviceList(IVS_INT32 iSessionID, LPCTSTR pReqXml)
{
    IVS_DEBUG_TRACE("");

    //��ȡ�м�����¼
    IVS_UINT32 iNum = 0;
    CXml xml;
    if (!xml.Parse(pReqXml))
    {
        BP_RUN_LOG_ERR(IVS_XML_INVALID, "CXml Parse", "xml parsing error");
        return IVS_XML_INVALID;
    }
    iNum = COCXUserInfoMgrXMLProcess::GetXmlListItemNum(xml, "Content/DevList/DevInfo");

    //�����ڴ�
    IVS_UINT32 iBufferSize = sizeof(IVS_USER_DEVICE_INFO_LIST) + sizeof(IVS_USER_DEVICE_INFO) * iNum;

	IVS_CHAR *pBuffer = IVS_NEW(pBuffer, iBufferSize);
	CHECK_POINTER(pBuffer, NULL);
    memset(pBuffer, 0, iBufferSize);
    IVS_USER_DEVICE_INFO_LIST *pUserDevList = reinterpret_cast<IVS_USER_DEVICE_INFO_LIST*>(pBuffer);//lint !e826

    pUserDevList->uiTotal = iNum;

    IVS_CHAR cUserDomainCode[IVS_DOMAIN_CODE_LEN+1] = {0};

    IVS_UINT32 uiUserId = 0;
    IVS_INT32 iRet = IVS_FAIL;
    iRet = COCXUserInfoMgrXMLProcess::SetUserDevListParseXml(pReqXml, uiUserId, pUserDevList, cUserDomainCode);
    if(IVS_SUCCEED != iRet)
    {
        IVS_DELETE(pBuffer, MUILI);
        BP_RUN_LOG_ERR(iRet, "Set User Dev List ParseXml", "Error");
        return iRet;
    }

    iRet = IVS_SDK_SetUserDeviceList(iSessionID, cUserDomainCode, uiUserId, pUserDevList, iBufferSize);

    IVS_DELETE(pBuffer, MUILI);
    return iRet;
}

// ��ȡ�û��豸�б�
CString COCXUserInfoMgr::GetUserDeviceList(IVS_INT32 iSessionID, LPCTSTR pUnifiedQuery)
{
    IVS_DEBUG_TRACE("");

    CString strResult;
    IVS_INT32 iRet = IVS_FAIL;

    if(NULL == pUnifiedQuery)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Get User Device List", "pUnifiedQuery is null");
        COCXXmlProcess::GetErrString(strResult, IVS_PARA_INVALID);
        return strResult;
    }

    // �ж��Ƿ������ȷ
    CXml reqXml;
    if (!reqXml.Parse(pUnifiedQuery))
    {
        BP_RUN_LOG_ERR(IVS_XML_INVALID, "CXml Parse method", "xml parsing error");
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
        BP_RUN_LOG_ERR(iRet, "Get Unified Format Query ParseXML", "Error");
        COCXXmlProcess::GetErrString(strResult, iRet);
        return strResult;
    }
    IVS_QUERY_UNIFIED_FORMAT *pQueryFormat = reinterpret_cast<IVS_QUERY_UNIFIED_FORMAT*>(pReqBuffer);
	if (NULL == pQueryFormat)
	{
		BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "Get Unified Format Query ParseXML", "pQueryFormat is null");
		COCXXmlProcess::GetErrString(strResult, IVS_OPERATE_MEMORY_ERROR);
		return strResult;
	}

    IVS_UINT32 uiUserID = 0;
    for (int i = 0;i < iFieldNum; i++)
    {
        // ��ȡuserID
        if(QUERY_USER_ID == pQueryFormat->stQueryField[i].eFieID)
        {
            uiUserID = (IVS_UINT32)atoi(pQueryFormat->stQueryField[i].cValue);
        }
    }
    
    // ���䷵���б����ݽṹ�ռ�
    IVS_UINT32 iItemNum = (pQueryFormat->stIndex.uiToIndex - pQueryFormat->stIndex.uiFromIndex) + 1;
    IVS_UINT32 iBufferSize = sizeof(IVS_USER_DEVICE_INFO_LIST) + sizeof(IVS_USER_DEVICE_INFO) * iItemNum;

	IVS_CHAR *pBuffer = IVS_NEW(pBuffer, iBufferSize);
	if (NULL == pBuffer)
	{
		IVS_DELETE(pReqBuffer, MUILI);
		return NULL;
	}
    memset(pBuffer, 0, iBufferSize);
    IVS_USER_DEVICE_INFO_LIST *pUserDevList = reinterpret_cast<IVS_USER_DEVICE_INFO_LIST*>(pBuffer);//lint !e826

    // ��ȡlist
    iRet = IVS_SDK_GetUserDeviceList(iSessionID, cDomainCode, uiUserID, &pQueryFormat->stIndex, pUserDevList, iBufferSize);
    if(iRet == IVS_SUCCEED)
    {
        CXml rspXml;
        (void)COCXUserInfoMgrXMLProcess::GetUserDevListGetXml(pUserDevList, iRet, rspXml);
        IVS_UINT32 iLen = 0;
        strResult = rspXml.GetXMLStream(iLen);
    }
    else
    {
        BP_RUN_LOG_ERR(iRet, "Get User Device List", "Error");
        COCXXmlProcess::GetErrString(strResult, iRet);
    }     

    IVS_DELETE(pReqBuffer, MUILI);
    IVS_DELETE(pBuffer, MUILI);
    return strResult;
}

// ǩ�������û��Ự
LONG COCXUserInfoMgr::SignOutOnlineUser(IVS_INT32 iSessionID, LPCTSTR pDomainCode, ULONG ulUserOnlineID)
{
    IVS_DEBUG_TRACE("");
    IVS_INT32 iRet = IVS_FAIL;
    iRet = IVS_SDK_SignOutOnlineUser(iSessionID, pDomainCode, (IVS_UINT32)ulUserOnlineID);
    return iRet;
}

// �����û�IDǩ�������û��Ự
LONG COCXUserInfoMgr::SignOutUser(IVS_INT32 iSessionID, LPCTSTR pDomainCode, ULONG ulUserID)
{
    IVS_DEBUG_TRACE("");
    IVS_INT32 iRet = IVS_FAIL;
    iRet = IVS_SDK_SignOutUser(iSessionID, pDomainCode, (IVS_UINT32)ulUserID);
    return iRet;
}

// ���ݽ�ɫIDǩ�������û��Ự
LONG COCXUserInfoMgr::SignOutUserByRole(IVS_INT32 iSessionID, LPCTSTR pDomainCode, ULONG ulRoleID)
{
    IVS_DEBUG_TRACE("");
    IVS_INT32 iRet = IVS_FAIL;
    iRet = IVS_SDK_SignOutUserByRole(iSessionID, pDomainCode, (IVS_UINT32)ulRoleID);
    return iRet;
}


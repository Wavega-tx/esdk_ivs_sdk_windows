#include "stdafx.h"
#include "Favorite.h"
#include "ivs_error.h"
#include "LocalConfig.h"
#include "OCXXmlProcess.h"
#include "ToolsHelp.h"
#include "IVS_Trace.h"
typedef enum
{
    OCX_FAVORITE_FOLDER   = 0,
    OCX_FAVORITE_DEVICE   = 1
}OCX_FAVORITE_TYPE;

/*�豸�ղ����ղؼ������ļ�xml��ʽ
 <Content>
   <FavoriteFolderList>
     <DeviceFavoriteInfo><!- ��� ->
       <FolderID>0D0EA60D-A0F4-4c97-A636-719A5D591675</FolderID>
       <ParentFolderID />
       <FolderName>HW10001</FolderName>
     </DeviceFavoriteInfo>
   </FavoriteFolderList>
   <DeviceFavoriteList>
     <DeviceFavoriteInfo><!- ��� ->
       <DeviceFavoriteID>E8E84275-35F7-4c44-848C-7FA192E1653E</DeviceFavoriteID>
       <ParentFolderID>0D0EA60D-A0F4-4c97-A636-719A5D591675</ParentFolderID>
       <DeviceCode>910100012</DeviceCode>
     </DeviceFavoriteInfo>
   </DeviceFavoriteList>
 </Content>
*/

CFavorite::CFavorite(void)
{
}

CFavorite::~CFavorite(void)
{
}

const IVS_CHAR* CFavorite::NewGUID()
{
    static IVS_CHAR buf[IVS_FAVORITE_ID_LEN] = {0};
    GUID guid;
    if (S_OK == ::CoCreateGuid(&guid))
    {
        _snprintf(buf, sizeof(buf)
            , "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X"
            , guid.Data1
            , guid.Data2
            , guid.Data3
            , guid.Data4[0], guid.Data4[1]
        , guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
        , guid.Data4[6], guid.Data4[7]
        );
    }
    return (const IVS_CHAR*)buf;
}

IVS_INT32 ForwardElemByID(const IVS_CHAR* pszID, const OCX_FAVORITE_TYPE enType, CXml& xml)
{
    CHECK_POINTER(pszID, IVS_FAIL);
    xml.GetRootPos();

    if (!xml.FindElem("Content"))
    {
        return IVS_FAIL;
    }
    (void)xml.IntoElem();

    const IVS_CHAR* szElem = "DeviceFavoriteList";
    if (OCX_FAVORITE_FOLDER == enType)
    {
        szElem = "FavoriteFolderList";
    }
    if (!xml.FindElem(szElem))
    {
        return IVS_FAIL;
    }
    (void)xml.IntoElem();

    const IVS_CHAR* szElemValue = NULL;
    szElem = "DeviceFavoriteID";
    if (OCX_FAVORITE_FOLDER == enType)
    {
        szElem = "FolderID";
    }
    while (xml.FindElemValue("DeviceFavoriteInfo"))
    {
        (void)xml.IntoElem();

        if (xml.FindElem(szElem))
        {
            szElemValue = xml.GetElemValue();
            if ((NULL != szElemValue) && (0 == strcmp(szElemValue, pszID)))
            {//ƥ���豸�ղػ��ղؼ�ID,��ǰԪ��ָ���ID�ڵ�
                return IVS_SUCCEED;
            }
        }
        
        xml.OutOfElem();
        if (!xml.NextElem())
        {
            return IVS_FAIL;
        }
    }

    return IVS_FAIL;
}

// ����豸�ղ�
IVS_INT32 CFavorite::AddFavoriteDevice(const IVS_CHAR* pDeviceFavoriteInfo, std::string& strDeviceGUID)
{
    CHECK_POINTER(pDeviceFavoriteInfo, IVS_FAIL);
    IVS_CHAR szParentFolderID[IVS_FAVORITE_ID_LEN+1] = {0};
    IVS_CHAR szDeviceCode[IVS_DEV_CODE_LEN+1] = {0};
    IVS_INT32 iRet = AddFavoriteDeviceParseXML(pDeviceFavoriteInfo, szParentFolderID, szDeviceCode);
    if (IVS_SUCCEED != iRet)
    {
        return iRet;
    }
    //�豸�ղ������ļ�
    CLocalConfig localConfig;
    string strPath = localConfig.MakeSureLocalConfigFileExist(FAVORITE_CONFIG_FILE_NAME);
    if (strPath.empty())
    {
        return IVS_FAIL;
    }
    CXml xmlConfig;
    if (!xmlConfig.Load(strPath.c_str(), TIXML_ENCODING_LEGACY))
    {
        return IVS_FAIL;
    }
    if ('\0' != szParentFolderID[0])//�ж��ϼ��Ƿ�Ϊ��
    {
        //��xml,�ж�ID�Ƿ����
        iRet = ForwardElemByID(szParentFolderID, OCX_FAVORITE_FOLDER, xmlConfig);
        if (IVS_SUCCEED != iRet)
        {
            return iRet;
        }
    }
    const char* pszDeviceFavoriteID = NewGUID();
    //���豸�ղ���ӵ�����xml
    xmlConfig.GetRootPos();
    if (!xmlConfig.FindElem("Content"))
    {
        return IVS_FAIL;
    }
    (void)xmlConfig.IntoElem();
    if (!xmlConfig.FindElem("DeviceFavoriteList"))
    {
        return IVS_FAIL;
    }
    //CXml���ͬ���ӽڵ�
    if (xmlConfig.IntoElem())
    {
        xmlConfig.AddElem("DeviceFavoriteInfo");
    }
    else
    {//��һ������
        xmlConfig.AddChildElem("DeviceFavoriteInfo");
        (void)xmlConfig.IntoElem();
    }
    xmlConfig.AddChildElem("DeviceFavoriteID");
    (void)xmlConfig.IntoElem();
    xmlConfig.SetElemValue(pszDeviceFavoriteID);
    xmlConfig.AddElem("ParentFolderID");
    xmlConfig.SetElemValue(szParentFolderID);
    xmlConfig.AddElem("DeviceCode");
    xmlConfig.SetElemValue(szDeviceCode);
    xmlConfig.AddElem("FolderID");
    xmlConfig.SetElemValue("");
    xmlConfig.AddElem("FolderName");
    xmlConfig.SetElemValue("");
    xmlConfig.OutOfElem();
    xmlConfig.OutOfElem();
    xmlConfig.OutOfElem();

    xmlConfig.Save();
    strDeviceGUID = pszDeviceFavoriteID; // �����ɹ������ɵ�GUID��ʶ���ظ�CU��ɾ�����޸ģ��ƶ��豸ʹ��
    return IVS_SUCCEED;
}
// ƴ������豸�ղ���Ӧ
IVS_INT32 CFavorite::AddFavoriteDeviceParseXML(const IVS_CHAR* pDeviceFavoriteInfo, IVS_CHAR* pszParentFolderID, IVS_CHAR* pszDeviceCode)
{
    CHECK_POINTER(pDeviceFavoriteInfo, IVS_FAIL);

    CXml xml;
    if (!xml.Parse(pDeviceFavoriteInfo))
    {
        return IVS_FAIL;
    }
    if (!xml.FindElem("Content"))
    {
        return IVS_FAIL;
    }
    (void)xml.IntoElem();

    if (!xml.FindElem("DeviceFavoriteInfo"))
    {
        return IVS_FAIL;
    }
    (void)xml.IntoElem();
    const char* szElemValue = NULL;
    GET_ELEM_VALUE_CHAR("ParentFolderID", szElemValue, pszParentFolderID, IVS_FAVORITE_ID_LEN, xml);//lint !e534
    szElemValue = NULL;
    GET_ELEM_VALUE_CHAR("DeviceCode", szElemValue, pszDeviceCode, IVS_DEV_CODE_LEN, xml);//lint !e534
    if (NULL == szElemValue)
    {
        return IVS_FAIL;
    }
    xml.OutOfElem();
    xml.OutOfElem();
    return IVS_SUCCEED;
}

// �ƶ��豸�ղ�
IVS_INT32 CFavorite::MoveFavoriteDevice(const IVS_CHAR* pDeviceFavoriteInfo)
{
    CHECK_POINTER(pDeviceFavoriteInfo, IVS_FAIL);
    IVS_CHAR szParentFolderID[IVS_FAVORITE_ID_LEN+1] = {0};
    IVS_CHAR szDeviceFavoriteID[IVS_FAVORITE_ID_LEN+1] = {0};
    IVS_INT32 iRet = MoveFavoriteDeviceParseXML(pDeviceFavoriteInfo, szParentFolderID, szDeviceFavoriteID);
    if (IVS_SUCCEED != iRet)
    {
        return iRet;
    }
    //�豸�ղ������ļ�
    CLocalConfig localConfig;
    string strPath = localConfig.MakeSureLocalConfigFileExist(FAVORITE_CONFIG_FILE_NAME);
    if (strPath.empty())
    {
        return IVS_FAIL;
    }
    CXml xmlConfig;
    if (!xmlConfig.Load(strPath.c_str(), TIXML_ENCODING_LEGACY))
    {
        return IVS_FAIL;
    }
    if ('\0' != szParentFolderID[0])//�ж��ϼ��Ƿ�Ϊ��
    {
        //��xml,�ж�ID�Ƿ����
        iRet = ForwardElemByID(szParentFolderID, OCX_FAVORITE_FOLDER, xmlConfig);
        if (IVS_SUCCEED != iRet)
        {
            return iRet;
        }
    }
    iRet = ForwardElemByID(szDeviceFavoriteID, OCX_FAVORITE_DEVICE, xmlConfig);
    if (IVS_SUCCEED != iRet)
    {
        return iRet;
    }
    //�޸�����xml
    if (!xmlConfig.FindElem("ParentFolderID"))
    {
        return IVS_FAIL;
    }
    //xmlConfig.ModifyElemValue(szParentFolderID);�������ļ���ParentFolderIDΪ��ʱ���˽ӿڻ����
    //�������淽ʽ�޸�ParentFolderID�ڵ�
    xmlConfig.RemoveElem();
    xmlConfig.AddElem("ParentFolderID");
    xmlConfig.SetElemValue(szParentFolderID);

    xmlConfig.Save();
    return IVS_SUCCEED;
}

// ƴ���ƶ��豸�ղ���Ӧ
IVS_INT32 CFavorite::MoveFavoriteDeviceParseXML(const IVS_CHAR* pDeviceFavoriteInfo, IVS_CHAR* pszParentFolderID, IVS_CHAR* pszDeviceFavoriteID)
{
    CHECK_POINTER(pDeviceFavoriteInfo, IVS_FAIL);
    CXml xml;
    if (!xml.Parse(pDeviceFavoriteInfo))
    {
        return IVS_FAIL;
    }
    if (!xml.FindElem("Content"))
    {
        return IVS_FAIL;
    }
    (void)xml.IntoElem();

    if (!xml.FindElem("DeviceFavoriteInfo"))
    {
        return IVS_FAIL;
    }
    (void)xml.IntoElem();
    const char* szElemValue = NULL;
    GET_ELEM_VALUE_CHAR("ParentFolderID", szElemValue, pszParentFolderID, IVS_FAVORITE_ID_LEN, xml);//lint !e534
    szElemValue = NULL;
    GET_ELEM_VALUE_CHAR("DeviceFavoriteID", szElemValue, pszDeviceFavoriteID, IVS_FAVORITE_ID_LEN, xml);//lint !e534
    if (NULL == szElemValue)
    {
        return IVS_FAIL;
    }
    xml.OutOfElem();
    xml.OutOfElem();
    return IVS_SUCCEED;
}

// ɾ���豸�ղ�
IVS_INT32 CFavorite::DelFavoriteDevice(const IVS_CHAR* pDeviceFavoriteInfo)
{
    CHECK_POINTER(pDeviceFavoriteInfo, IVS_FAIL);
    IVS_CHAR szDeviceFavoriteID[IVS_FAVORITE_ID_LEN+1] = {0};
    IVS_INT32 iRet = DelFavoriteDeviceParseXML(pDeviceFavoriteInfo, szDeviceFavoriteID);
    if (IVS_SUCCEED != iRet)
    {
        return iRet;
    }
    //�豸�ղ������ļ�
    CLocalConfig localConfig;
    string strPath = localConfig.MakeSureLocalConfigFileExist(FAVORITE_CONFIG_FILE_NAME);
    if (strPath.empty())
    {
        return IVS_FAIL;
    }
    CXml xmlConfig;
    if (!xmlConfig.Load(strPath.c_str(), TIXML_ENCODING_LEGACY))
    {
        return IVS_FAIL;
    }
    //���ң�ɾ��xml
    iRet = ForwardElemByID(szDeviceFavoriteID, OCX_FAVORITE_DEVICE, xmlConfig);
    if (IVS_SUCCEED != iRet)
    {//�����Ҳ���,��Ϊɾ���ɹ�
        return IVS_SUCCEED;
    }
    xmlConfig.OutOfElem();
    xmlConfig.RemoveElem();
    xmlConfig.Save();
    return IVS_SUCCEED;
}

// ƴ��ɾ���豸�ղ�
IVS_INT32 CFavorite::DelFavoriteDeviceParseXML(const IVS_CHAR* pDeviceFavoriteInfo, IVS_CHAR* pszDeviceFavoriteID)
{
    CHECK_POINTER(pDeviceFavoriteInfo, IVS_FAIL);
    CXml xml;
    if (!xml.Parse(pDeviceFavoriteInfo))
    {
        return IVS_FAIL;
    }
    if (!xml.FindElem("Content"))
    {
        return IVS_FAIL;
    }
    (void)xml.IntoElem();
    if (!xml.FindElem("DeviceFavoriteInfo"))
    {
        return IVS_FAIL;
    }
    (void)xml.IntoElem();
    const char* szElemValue = NULL;
    GET_ELEM_VALUE_CHAR("DeviceFavoriteID", szElemValue, pszDeviceFavoriteID, IVS_FAVORITE_ID_LEN, xml);//lint !e534
    if (NULL == szElemValue)
    {
        return IVS_FAIL;
    }
    xml.OutOfElem();
    xml.OutOfElem();
    return IVS_SUCCEED;
}

// ��ѯ�豸�ղ�
IVS_INT32 CFavorite::GetFavoriteDevice(CXml& xmlRsp)
{
    //�豸�ղ������ļ�
    CLocalConfig localConfig;
    string strPath = localConfig.MakeSureLocalConfigFileExist(FAVORITE_CONFIG_FILE_NAME);
    if (strPath.empty())
    {
        return IVS_FAIL;
    }
    CXml xmlConfig;
    if (!xmlConfig.Load(strPath.c_str(), TIXML_ENCODING_LEGACY))
    {
        return IVS_FAIL;
    }
    IVS_UINT32 uiLen = 0;
    const IVS_CHAR* pszXml = xmlConfig.GetXMLStream(uiLen);
    CHECK_POINTER(pszXml, IVS_FAIL);

    const IVS_CHAR* pFolderBeg = NULL;
    const IVS_CHAR* pFolderEnd = NULL;
    const IVS_CHAR* pDeviceBeg = NULL;
    const IVS_CHAR* pDeviceEnd = NULL;
    int iFolderContentLen = 0;   // �ղص��ļ������ݳ���
    int iDeviceContentLen = 0;   // �ղص��豸���ݳ���
    // �ڵ�Ϊ��ʱ��xmlLoad�����ݿ���Ϊ<FavoriteFolderList/>��ʽ
    if (NULL == strstr(pszXml, "<FavoriteFolderList />") && NULL == strstr(pszXml, "<FavoriteFolderList/>"))
    {
        //ֱ�ӽ�ȡDeviceFavoriteInfo��Ϣ��Ϊ��Ӧ,Ĭ��ģ�屣֤��ֵ������
        pFolderBeg = strstr(pszXml, "<FavoriteFolderList>");
        CHECK_POINTER(pFolderBeg, IVS_FAIL);
        pFolderBeg += sizeof("<FavoriteFolderList>") - 1;
        pFolderEnd = strstr(pszXml, "</FavoriteFolderList>");
        CHECK_POINTER(pFolderEnd, IVS_FAIL);
        iFolderContentLen = pFolderEnd - pFolderBeg;
    }
    if (NULL == strstr(pszXml, "<DeviceFavoriteList/>") && NULL == strstr(pszXml, "<DeviceFavoriteList />"))
    {
        //ֱ�ӽ�ȡDeviceFavoriteInfo��Ϣ��Ϊ��Ӧ,Ĭ��ģ�屣֤��ֵ������
        pDeviceBeg = strstr(pszXml, "<DeviceFavoriteList>");
        CHECK_POINTER(pDeviceBeg, IVS_FAIL);
        pDeviceBeg += sizeof("<DeviceFavoriteList>") - 1;
        pDeviceEnd = strstr(pszXml, "</DeviceFavoriteList>");
        CHECK_POINTER(pDeviceEnd, IVS_FAIL);
        iDeviceContentLen = pDeviceEnd - pDeviceBeg;
    }
    
    if ((0 == iFolderContentLen)
        && (0 == iDeviceContentLen))
    {
        //���豸�ղ�
        (void)xmlRsp.Parse("<?xml version=\"1.0\" encoding=\"UTF-8\" ?><Content><ResultCode>0</ResultCode></Content>");
        return IVS_SUCCEED;
    }

    //�����ѯ��Ӧ����
    IVS_INT32 uiPos = 0;
    uiPos = sizeof("<?xml version=\"1.0\" encoding=\"UTF-8\" ?><Content><ResultCode>0</ResultCode><DeviceFavoriteList>") - 1;
    IVS_UINT32 iBuffLen = uiLen + uiPos + 1;  //lint !e737 û�о�������
	IVS_CHAR* pszXmlRsp = IVS_NEW(pszXmlRsp,iBuffLen);// ��ֹFavoriteFolderList��DeviceFavoriteListΪ��ʱ������ԭxml���ݳ���������ڴ�᲻����
    memset(pszXmlRsp, 0, uiLen + 1);
    (void)CToolsHelp::Strncpy(pszXmlRsp, (IVS_UINT32)(uiPos + 1), "<?xml version=\"1.0\" encoding=\"UTF-8\" ?><Content><ResultCode>0</ResultCode><DeviceFavoriteList>", (IVS_UINT32)uiPos);
    //strncpy(pszXmlRsp, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?><Content><ResultCode>0</ResultCode><DeviceFavoriteList>", uiPos);//lint !e732
    if (0 != iFolderContentLen)
    {
        (void)CToolsHelp::Strncpy(pszXmlRsp + uiPos, (IVS_UINT32)(iFolderContentLen + 1), pFolderBeg, (IVS_UINT32)iFolderContentLen);
        //strncpy(pszXmlRsp + uiPos, pFolderBeg, iFolderContentLen);//lint !e732
        uiPos += iFolderContentLen;
    }
    if (0 != iDeviceContentLen)
    {
        (void)CToolsHelp::Strncpy(pszXmlRsp + uiPos, (IVS_UINT32)(iDeviceContentLen + 1), pDeviceBeg, (IVS_UINT32)iDeviceContentLen);
        //strncpy(pszXmlRsp + uiPos, pDeviceBeg, iDeviceContentLen);//lint !e732
        uiPos += iDeviceContentLen;
    }
    (void)CToolsHelp::Strncpy(pszXmlRsp + uiPos, sizeof("</DeviceFavoriteList></Content>") + 1, "</DeviceFavoriteList></Content>", sizeof("</DeviceFavoriteList></Content>"));
    //strncpy(pszXmlRsp + uiPos, "</DeviceFavoriteList></Content>", sizeof("</DeviceFavoriteList></Content>"));
    // UTF-8ת��Ϊansi���ظ�CU
//     char* pAnsiResult = CToolsHelp::UTF8ToANSI(pszXmlRsp);
//     if (NULL == pAnsiResult)
//     {
//         HW_DELETE_A(pszXmlRsp);
//         return IVS_FAIL;
//     }
    if (!xmlRsp.Parse(pszXmlRsp))
    {
        IVS_DELETE(pszXmlRsp, MUILI);
    //    free(pAnsiResult);
        return IVS_FAIL;
    }
    IVS_DELETE(pszXmlRsp, MUILI);
   // free(pAnsiResult);
    return IVS_SUCCEED;
}

// ɾ���ղ��豸
IVS_INT32 DelFavoriteDeviceInFolder(const IVS_CHAR* pszFolderID, CXml& xml)
{
    CHECK_POINTER(pszFolderID, IVS_FAIL);
    xml.GetRootPos();
    if (!xml.FindElem("Content"))
    {
        return IVS_FAIL;
    }
    (void)xml.IntoElem();
    if (!xml.FindElem("DeviceFavoriteList"))
    {
        return IVS_FAIL;
    }
    if (!xml.IntoElem())
    {//���豸�ղ�
        return IVS_SUCCEED;
    }
    const IVS_CHAR* szElemValue = NULL;
    while (xml.FindElemValue("DeviceFavoriteInfo"))
    {
        (void)xml.IntoElem();
        if (xml.FindElem("ParentFolderID"))
        {
            szElemValue = xml.GetElemValue();
            if ((NULL != szElemValue) && (0 == strcmp(szElemValue, pszFolderID)))
            {//ɾ��ƥ����豸�ղ���Ϣ
                xml.OutOfElem();
                xml.RemoveElem();
                continue;
            }
        }
        xml.OutOfElem();
        if (!xml.NextElem())
        {
            BP_RUN_LOG_INF("CFavorite::DelFavoriteDeviceInFolder","Leave");
            return IVS_SUCCEED;
        }
    }
    return IVS_SUCCEED;
}

// ɾ���ղؼ�
IVS_INT32 CFavorite::DelFavoriteFolder(const IVS_CHAR* pDeviceFavoriteInfo)
{
    CHECK_POINTER(pDeviceFavoriteInfo, IVS_FAIL);
    IVS_CHAR szFolderID[IVS_FAVORITE_ID_LEN+1] = {0};
    IVS_INT32 iRet = DelFavoriteFolderParseXML(pDeviceFavoriteInfo, szFolderID);
    if (IVS_SUCCEED != iRet)
    {
        return iRet;
    }

    //�豸�ղ������ļ�
    CLocalConfig localConfig;
    string strPath = localConfig.MakeSureLocalConfigFileExist(FAVORITE_CONFIG_FILE_NAME);
    if (strPath.empty())
    {
        return IVS_FAIL;
    }
    CXml xmlConfig;
    if (!xmlConfig.Load(strPath.c_str(), TIXML_ENCODING_LEGACY))
    {
        return IVS_FAIL;
    }

    //���ң�ɾ��xml
    iRet = ForwardElemByID(szFolderID, OCX_FAVORITE_FOLDER, xmlConfig);
    if (IVS_SUCCEED != iRet)
    {//�����Ҳ���,��Ϊɾ���ɹ�
        return IVS_SUCCEED;
    }
    //ɾ���ղؼм����豸�ղ�,�ղز���������ղؼ�(CU����)
    xmlConfig.OutOfElem();
    xmlConfig.RemoveElem();
    (void)DelFavoriteDeviceInFolder(szFolderID, xmlConfig);
    xmlConfig.Save();
    return IVS_SUCCEED;
}

// ����ɾ���ղ��豸����
IVS_INT32 CFavorite::DelFavoriteFolderParseXML(const IVS_CHAR* pDeviceFavoriteInfo, IVS_CHAR* pszFolderID)
{
    CHECK_POINTER(pDeviceFavoriteInfo, IVS_FAIL);
    CXml xml;
    if (!xml.Parse(pDeviceFavoriteInfo))
    {
        return IVS_FAIL;
    }
    if (!xml.FindElem("Content"))
    {
        return IVS_FAIL;
    }
    (void)xml.IntoElem();
    if (!xml.FindElem("FolderInfo"))
    {
        return IVS_FAIL;
    }
    (void)xml.IntoElem();
    const char* szElemValue = NULL;
    GET_ELEM_VALUE_CHAR("FolderID", szElemValue, pszFolderID, IVS_FAVORITE_ID_LEN, xml);//lint !e534
    if (NULL == szElemValue)
    {
        return IVS_FAIL;
    }
    xml.OutOfElem();
    xml.OutOfElem();
    return IVS_SUCCEED;
}

// �����ղؼ�
IVS_INT32 CFavorite::AddFavoriteFolder(const IVS_CHAR* pDeviceFavoriteInfo, std::string& strFolderGUID)
{
    CHECK_POINTER(pDeviceFavoriteInfo, IVS_FAIL);
    IVS_CHAR szParentFolderID[IVS_FAVORITE_ID_LEN+1] = {0};
    IVS_CHAR szFolderName[IVS_FOLDER_NAME_LEN+1] = {0};
    IVS_INT32 iRet = AddFavoriteFolderParseXML(pDeviceFavoriteInfo, szParentFolderID, szFolderName);
    if (IVS_SUCCEED != iRet)
    {
        return iRet;
    }
    //�豸�ղ������ļ�
    CLocalConfig localConfig;
    string strPath = localConfig.MakeSureLocalConfigFileExist(FAVORITE_CONFIG_FILE_NAME);
    if (strPath.empty())
    {
        return IVS_FAIL;
    }
    CXml xmlConfig;
    if (!xmlConfig.Load(strPath.c_str(), TIXML_ENCODING_LEGACY))
    {
        return IVS_FAIL;
    }
    if ('\0' != szParentFolderID[0])//�ж��ϼ��Ƿ�Ϊ��
    {
        //��xml,�ж�ID�Ƿ����
        iRet = ForwardElemByID(szParentFolderID, OCX_FAVORITE_FOLDER, xmlConfig);
        if (IVS_SUCCEED != iRet)
        {
            return iRet;
        }
    }
    const char* pszFolderID = NewGUID();
    CHECK_POINTER(pszFolderID, IVS_FAIL);

    //�����ղؼ���Ϣ
    xmlConfig.GetRootPos();
    if (!xmlConfig.FindElem("Content"))
    {
        return IVS_FAIL;
    }
    (void)xmlConfig.IntoElem();

    if (!xmlConfig.FindElem("FavoriteFolderList"))
    {
        return IVS_FAIL;
    }
    
    //CXml���ͬ���ӽڵ�
    if (xmlConfig.IntoElem())
    {
        xmlConfig.AddElem("DeviceFavoriteInfo");
    }
    else
    {//��һ������
        xmlConfig.AddChildElem("DeviceFavoriteInfo");
        (void)xmlConfig.IntoElem();
    }
//     IVS_CHAR* pFolderName = CToolsHelp::ANSIToUTF8(szFolderName);
//     if (NULL == pFolderName)
//     {
//         return IVS_FAIL;
//     }
    xmlConfig.AddChildElem("FolderID");
    (void)xmlConfig.IntoElem();
    xmlConfig.SetElemValue(pszFolderID);
    xmlConfig.AddElem("ParentFolderID");
    xmlConfig.SetElemValue(szParentFolderID);
    xmlConfig.AddElem("FolderName");
    xmlConfig.SetElemValue(szFolderName);
    xmlConfig.AddElem("DeviceCode");
    xmlConfig.SetElemValue("");
    xmlConfig.AddElem("DeviceFavoriteID");
    xmlConfig.SetElemValue("");

    xmlConfig.OutOfElem();
    xmlConfig.OutOfElem();
    xmlConfig.OutOfElem();

    xmlConfig.Save();
  //  free(pFolderName);
    strFolderGUID = pszFolderID;  // �����ɹ������ɵ�GUID��ʶ���ظ�CU��ɾ�����޸ģ��ƶ��ղؼ�ʹ��
    return IVS_SUCCEED;
}

// ���������豸�ղ�����
IVS_INT32 CFavorite::AddFavoriteFolderParseXML(const IVS_CHAR* pDeviceFavoriteInfo, IVS_CHAR* pszParentFolderID, IVS_CHAR* pszFolderName)
{
    CHECK_POINTER(pDeviceFavoriteInfo, IVS_FAIL);
    CXml xml;
    if (!xml.Parse(pDeviceFavoriteInfo))
    {
        return IVS_FAIL;
    }
    if (!xml.FindElem("Content"))
    {
        return IVS_FAIL;
    }
    (void)xml.IntoElem();
    if (!xml.FindElem("FolderInfo"))
    {
        return IVS_FAIL;
    }
    (void)xml.IntoElem();
    const char* szElemValue = NULL;
    GET_ELEM_VALUE_CHAR("ParentFolderID", szElemValue, pszParentFolderID, IVS_FAVORITE_ID_LEN, xml);//lint !e534
    szElemValue = NULL;
    GET_ELEM_VALUE_CHAR("FolderName", szElemValue, pszFolderName, IVS_FOLDER_NAME_LEN, xml);//lint !e534
    if (NULL == szElemValue)
    {
        return IVS_FAIL;
    }
    xml.OutOfElem();
    xml.OutOfElem();
    return IVS_SUCCEED;
}

// �޸��ղؼ�
IVS_INT32 CFavorite::ModFavoriteFolder(const IVS_CHAR* pDeviceFavoriteInfo)
{
    CHECK_POINTER(pDeviceFavoriteInfo, IVS_FAIL);
    IVS_CHAR szFolderID[IVS_FAVORITE_ID_LEN+1] = {0};
    IVS_CHAR szFolderName[IVS_FOLDER_NAME_LEN+1] = {0};
    IVS_INT32 iRet = ModFavoriteFolderParseXML(pDeviceFavoriteInfo, szFolderID, szFolderName);
    if (IVS_SUCCEED != iRet)
    {
        return iRet;
    }

    //�豸�ղ������ļ�
    CLocalConfig localConfig;
    string strPath = localConfig.MakeSureLocalConfigFileExist(FAVORITE_CONFIG_FILE_NAME);
    if (strPath.empty())
    {
        return IVS_FAIL;
    }
    CXml xmlConfig;
    if (!xmlConfig.Load(strPath.c_str(), TIXML_ENCODING_LEGACY))
    {
        return IVS_FAIL;
    }
    if ('\0' != szFolderID[0])//�ж��ϼ��Ƿ�Ϊ��
    {
        //��xml,�ж�ID�Ƿ����
        iRet = ForwardElemByID(szFolderID, OCX_FAVORITE_FOLDER, xmlConfig);
        if (IVS_SUCCEED != iRet)
        {
            return iRet;
        }
    }
    //�޸�����xml
    if (!xmlConfig.FindElem("FolderName"))
    {
        return IVS_FAIL;
    }
//     IVS_CHAR* pFolderName = CToolsHelp::ANSIToUTF8(szFolderName);
//     if (NULL == pFolderName)
//     {
//         return IVS_FAIL;
//     }
    //FolderName�ڵ㲻��Ϊ��
    xmlConfig.ModifyElemValue(szFolderName);
    xmlConfig.Save();
   // free(pFolderName);
    return IVS_SUCCEED;
}

// �޸��ղ��ļ���
IVS_INT32 CFavorite::ModFavoriteFolderParseXML(const IVS_CHAR* pFavoriteFolder, IVS_CHAR* pszFolderID, IVS_CHAR* pszFolderName)
{
    CHECK_POINTER(pFavoriteFolder, IVS_FAIL);
    CXml xml;
    if (!xml.Parse(pFavoriteFolder))
    {
        return IVS_FAIL;
    }
    if (!xml.FindElem("Content"))
    {
        return IVS_FAIL;
    }
    (void)xml.IntoElem();
    if (!xml.FindElem("FolderInfo"))
    {
        return IVS_FAIL;
    }
    (void)xml.IntoElem();
    const char* szElemValue = NULL;
    GET_ELEM_VALUE_CHAR("FolderID", szElemValue, pszFolderID, IVS_FAVORITE_ID_LEN, xml);//lint !e534
    if (NULL == szElemValue)
    {
        return IVS_FAIL;
    }
    szElemValue = NULL;
    GET_ELEM_VALUE_CHAR("FolderName", szElemValue, pszFolderName, IVS_FOLDER_NAME_LEN, xml);//lint !e534
    if (NULL == szElemValue)
    {
        return IVS_FAIL;
    }
    xml.OutOfElem();
    xml.OutOfElem();
    return IVS_SUCCEED;
}

// �ƶ��ղؼ�
IVS_INT32 CFavorite::MoveFavoriteFolder(const IVS_CHAR* pDeviceFavoriteInfo)
{
    CHECK_POINTER(pDeviceFavoriteInfo, IVS_FAIL);
    IVS_CHAR szParentFolderID[IVS_FAVORITE_ID_LEN+1] = {0};
    IVS_CHAR szFolderID[IVS_FAVORITE_ID_LEN+1] = {0};
    IVS_INT32 iRet = MoveFavoriteFolderParseXML(pDeviceFavoriteInfo, szParentFolderID, szFolderID);
    if (IVS_SUCCEED != iRet)
    {
        return iRet;
    }

    //�豸�ղ������ļ�
    CLocalConfig localConfig;
    string strPath = localConfig.MakeSureLocalConfigFileExist(FAVORITE_CONFIG_FILE_NAME);
    if (strPath.empty())
    {
        return IVS_FAIL;
    }
    CXml xmlConfig;
    if (!xmlConfig.Load(strPath.c_str(), TIXML_ENCODING_LEGACY))
    {
        return IVS_FAIL;
    }
    if ('\0' != szParentFolderID[0])//�ж��ϼ��Ƿ�Ϊ��
    {
        //��xml,�ж�ID�Ƿ����
        iRet = ForwardElemByID(szParentFolderID, OCX_FAVORITE_FOLDER, xmlConfig);
        if (IVS_SUCCEED != iRet)
        {
            return iRet;
        }
    }
    iRet = ForwardElemByID(szFolderID, OCX_FAVORITE_FOLDER, xmlConfig);
    if (IVS_SUCCEED != iRet)
    {
        return iRet;
    }
    //�޸�����xml
    if (!xmlConfig.FindElem("ParentFolderID"))
    {
        return IVS_FAIL;
    }
    //xmlConfig.ModifyElemValue(szParentFolderID);�������ļ���ParentFolderIDΪ��ʱ���˽ӿڻ����
    //�������淽ʽ�޸�ParentFolderID�ڵ�
    xmlConfig.RemoveElem();
    xmlConfig.AddElem("ParentFolderID");
    xmlConfig.SetElemValue(szParentFolderID);
    xmlConfig.Save();
    return IVS_SUCCEED;
}

// �����ƶ��ļ�������
IVS_INT32 CFavorite::MoveFavoriteFolderParseXML(const IVS_CHAR* pFavoriteFolder, IVS_CHAR* pszParentFolderID, IVS_CHAR* pszFolderID)
{
    BP_RUN_LOG_INF("CFavorite::MoveFavoriteFolderParseXML","Enter");
    CHECK_POINTER(pFavoriteFolder, IVS_FAIL);
    CXml xml;
    if (!xml.Parse(pFavoriteFolder))
    {
        return IVS_FAIL;
    }
    if (!xml.FindElem("Content"))
    {
        return IVS_FAIL;
    }
    (void)xml.IntoElem();

    if (!xml.FindElem("FolderInfo"))
    {
        return IVS_FAIL;
    }
    (void)xml.IntoElem();
    const char* szElemValue = NULL;
    GET_ELEM_VALUE_CHAR("ParentFolderID", szElemValue, pszParentFolderID, IVS_FAVORITE_ID_LEN, xml);//lint !e534
    szElemValue = NULL;
    GET_ELEM_VALUE_CHAR("FolderID", szElemValue, pszFolderID, IVS_FAVORITE_ID_LEN, xml);//lint !e534
    if (NULL == szElemValue)
    {
        return IVS_FAIL;
    }
    xml.OutOfElem();
    xml.OutOfElem();
    return IVS_SUCCEED;
}

// ƴ�������豸�ղ���Ӧ
void CFavorite::AddFavoriteDeviceRspXML(const std::string& strDeviceGUID, CXml& xml)
{
    (void)xml.AddDeclaration("1.0","UTF-8","");
    (void)xml.AddElem("Content");
    (void)xml.AddChildElem("ResultCode");
    (void)xml.IntoElem();
    (void)xml.SetElemValue("0");

    (void)xml.AddElem("DeviceFavoriteID");
    (void)xml.SetElemValue(strDeviceGUID.c_str());
    xml.OutOfElem();
}

// ƴ�������ļ����ղ���Ӧ
void CFavorite::AddFavoriteFolderRspXML(const std::string& strFolderGUID, CXml& xml)
{
    (void)xml.AddDeclaration("1.0","UTF-8","");
    (void)xml.AddElem("Content");
    (void)xml.AddChildElem("ResultCode");
    (void)xml.IntoElem();
    (void)xml.SetElemValue("0");

    (void)xml.AddElem("FolderID");
    (void)xml.SetElemValue(strFolderGUID.c_str());
    xml.OutOfElem();
}

// ��������
IVS_INT32 CFavorite::AddScenarioFavorite(const IVS_CHAR* pScenarioXml, const std::string& strID)
{
	CHECK_POINTER(pScenarioXml, IVS_PARA_INVALID);
    IVS_DEBUG_TRACE("pScenarioXml:%s", pScenarioXml);
	// �ж��ļ��Ƿ���ڣ������ھʹ���һ��ģ��
	CLocalConfig localConfig;
	string strPath = localConfig.MakeSureLocalConfigFileExist(SCENARIO_FAVORITE_FILE_NAME);
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_CREATE_FILE_ERROR, "file create failed","NA");
		return IVS_CREATE_FILE_ERROR;
	}

	// ����XML�ļ�,�ҵ�list�ڵ�
	TiXmlDocument pScenarioXmlDoc;
	if (!pScenarioXmlDoc.LoadFile(strPath.c_str(), TIXML_ENCODING_LEGACY))
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "load file failed","NA");
		return IVS_PARA_INVALID;
	}
	
	TiXmlElement* pScenarloListElem = CToolsHelp::FindElementByPath(&pScenarioXmlDoc,SCENARIOLIST_NODE);
	CHECK_POINTER(pScenarloListElem, IVS_FAIL);

// 	IVS_CHAR* pScenarioXmlUTF8 = CToolsHelp::ANSIToUTF8(pScenarioXml);
// 	CHECK_POINTER(pScenarioXmlUTF8, IVS_FAIL);

	// ��һ��ID�ڵ㵽�����XML
	TiXmlDocument pXmlDoc;
	(void)(pXmlDoc.Parse(pScenarioXml, NULL, TIXML_ENCODING_LEGACY));
	if (0 != pXmlDoc.Error())
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "parse xml failed","NA");
      //  free(pScenarioXmlUTF8);
		return IVS_PARA_INVALID;
	}


	// ����ID�Ľڵ�
	// ����LoginInfo�Ľڵ�
	TiXmlElement* pScenarioIDElem = NULL;
	try
	{
		pScenarioIDElem = new TiXmlElement(SCENARIO_ID_NODE);
	}
	catch(...)
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "new TiXmlElement(SCENARIO_ID_NODE) is error", "NA");
// 		pScenarioIDElem = NULL;
// 		free(pScenarioXmlUTF8);
		return IVS_ALLOC_MEMORY_ERROR;
	}

	//CHECK_POINTER(pScenarioIDElem, IVS_PARA_INVALID); //lint !e774
	pScenarioIDElem->LinkEndChild(new TiXmlText(strID.c_str()));
	// ��ԭ�������ϰ�ID�Ž�ȥ
	TiXmlElement* pScenarloInfoElem = CToolsHelp::FindElementByPath(&pXmlDoc,"Content/ScenarioInfo");
	if (NULL == pScenarloInfoElem)
	{
		delete pScenarioIDElem;
// 		pScenarioIDElem = NULL;
//         free(pScenarioXmlUTF8);
		return IVS_PARA_INVALID;
	}
	pScenarloInfoElem->InsertBeforeChild(pScenarloInfoElem->FirstChildElement(), *pScenarioIDElem);
	delete pScenarioIDElem;
	pScenarioIDElem = NULL; //TODO ����
	// ���뵽�ļ�
	TiXmlNode* pCloneScenarioInfoNode = pScenarloInfoElem->Clone();
    if (NULL == pCloneScenarioInfoNode)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "AddScenarioFavorite is error", "pCloneScenarioInfoNode is null.");
        return IVS_PARA_INVALID;
    }
	pScenarloListElem->LinkEndChild(pCloneScenarioInfoNode->ToElement());
	pScenarioXmlDoc.SaveFile(strPath.c_str());
 //   free(pScenarioXmlUTF8);
	return IVS_SUCCEED;
}

// �޸ĳ���
IVS_INT32 CFavorite::ModScenarioFavorite(const IVS_CHAR* pScenarioXml)
{
	CHECK_POINTER(pScenarioXml, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("pScenarioXml:%s", pScenarioXml);
	// ����ļ��Ƿ����
	CLocalConfig localConfig;
	std::string strPath = localConfig.CheckLocalConfigFileExist(SCENARIO_FAVORITE_FILE_NAME);
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_OPEN_FILE_ERROR,"file not exist","NA");
		return IVS_OPEN_FILE_ERROR;
	}
//     IVS_CHAR* pScenarioXmlUTF8 = CToolsHelp::ANSIToUTF8(pScenarioXml);
//     CHECK_POINTER(pScenarioXmlUTF8, IVS_FAIL);

	// ��ȡIDֵ
	TiXmlDocument pXmlDoc;
	(void)(pXmlDoc.Parse(pScenarioXml, NULL, TIXML_ENCODING_LEGACY));
	if (0 != pXmlDoc.Error())
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "parse xml failed","NA");
//         free(pScenarioXmlUTF8);
// 		pScenarioXmlUTF8 = NULL;
		return IVS_PARA_INVALID;
	}
	TiXmlElement* pScenarloIDElem = CToolsHelp::FindElementByPath(&pXmlDoc,"Content/ScenarioInfo/ID");
    if (NULL == pScenarloIDElem)
    {
      //  free(pScenarioXmlUTF8);
	//	pScenarioXmlUTF8 = NULL;
        return IVS_PARA_INVALID;
    }
	//CHECK_POINTER(pScenarloIDElem, IVS_PARA_INVALID);
	const char* pIDValue = pScenarloIDElem->GetText();

	// ����XML�ļ�������ID �ҵ��ڵ㲢ɾ��
	TiXmlDocument pScenarioXmlDoc;
	if (!pScenarioXmlDoc.LoadFile(strPath.c_str(), TIXML_ENCODING_LEGACY))
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "load file failed","NA");
        //free(pScenarioXmlUTF8);
	//	pScenarioXmlUTF8 = NULL;
		return IVS_PARA_INVALID;
	}
	TiXmlElement* pScenarloListElem = CToolsHelp::FindElementByPath(&pScenarioXmlDoc,SCENARIOLIST_NODE);
    if (NULL == pScenarloListElem)
    {
       // free(pScenarioXmlUTF8);
	//	pScenarioXmlUTF8 = NULL;
        return IVS_FAIL;
    }
	//CHECK_POINTER(pScenarloListElem, IVS_FAIL);

	// �����ҵ�һ����ID,�ҵ��� ֱ��ɾ�����info
	bool bFind = false;
	TiXmlElement* pNode = pScenarloListElem->FirstChildElement(SCENARIO_INFO_NODE);
	while(NULL != pNode)
	{
		TiXmlElement* pIDElem = CToolsHelp::FindElementByPath(pNode,"ID");
        if (NULL == pIDElem)
        {
           // free(pScenarioXmlUTF8);
		//	pScenarioXmlUTF8 = NULL;
            return IVS_PARA_INVALID;
        }
		//CHECK_POINTER(pIDElem, IVS_PARA_INVALID);
		// ����ҵ�һ����ID,ɾ�����info�ڵ�
		if (strcmp(pIDValue, pIDElem->GetText()) == 0)
		{
			bFind = true;
			pScenarloListElem->RemoveChild(pIDElem->Parent());
			break;
		}
		pNode = pNode->NextSiblingElement(SCENARIO_INFO_NODE);
	}
	// û���ҵ�����ڵ㣬ֱ�ӷ���ʧ��
	if (!bFind)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "ID not find","NA");
       // free(pScenarioXmlUTF8);
	//	pScenarioXmlUTF8 = NULL;
		return IVS_PARA_INVALID;
	}

	// ��ȡinfo�ڵ㣬�ҵ������
	TiXmlElement* pScenarloInfoElem = CToolsHelp::FindElementByPath(&pXmlDoc,"Content/ScenarioInfo");
	if (NULL == pScenarloInfoElem)
	{
	//	free(pScenarioXmlUTF8);
	//	pScenarioXmlUTF8 = NULL;
		return IVS_PARA_INVALID;
	}
	TiXmlNode* pCloneScenarioInfoNode = pScenarloInfoElem->Clone();
	if (NULL == pCloneScenarioInfoNode)
	{
	//	free(pScenarioXmlUTF8);
	//	pScenarioXmlUTF8 = NULL;
		return IVS_PARA_INVALID;
	}
	pScenarloListElem->LinkEndChild(pCloneScenarioInfoNode->ToElement());
	pScenarioXmlDoc.SaveFile(strPath.c_str());
  //  free(pScenarioXmlUTF8);
	return IVS_SUCCEED;
}
// �޸ĵ���ǽ����
IVS_INT32 CFavorite::SetTVwallScenarioFavorite(const IVS_CHAR* pScenarioXml)
{
	CHECK_POINTER(pScenarioXml, IVS_PARA_INVALID);
	// ����ļ��Ƿ����
	CLocalConfig localConfig;
	std::string strPath = localConfig.CheckLocalConfigFileExist(TVWALL_SCENARIO_FAVORITE_FILE_NAME);
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_OPEN_FILE_ERROR,"file not exist","NA");
		return IVS_OPEN_FILE_ERROR;
	}

	// ��ȡIDֵ
	TiXmlDocument pXmlDoc;
	(void)(pXmlDoc.Parse(pScenarioXml, NULL, TIXML_ENCODING_LEGACY));
	if (0 != pXmlDoc.Error())
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "parse xml failed","NA");
		return IVS_PARA_INVALID;
	}
	TiXmlElement* pScenarloIDElem = CToolsHelp::FindElementByPath(&pXmlDoc,"Content/ScenarioInfo/ID");
	CHECK_POINTER(pScenarloIDElem, IVS_PARA_INVALID);
	const char* pIDValue = pScenarloIDElem->GetText();

	// ����XML�ļ�������ID �ҵ��ڵ㲢ɾ��
	TiXmlDocument pScenarioXmlDoc;
	if (!pScenarioXmlDoc.LoadFile(strPath.c_str(), TIXML_ENCODING_LEGACY))
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "load file failed","NA");
		return IVS_PARA_INVALID;
	}
	TiXmlElement* pScenarloListElem = CToolsHelp::FindElementByPath(&pScenarioXmlDoc,SCENARIOLIST_NODE);
	CHECK_POINTER(pScenarloListElem, IVS_FAIL);

	// �����ҵ�һ����ID,�ҵ��� ֱ��ɾ�����info
	//bool bFind = false;
	TiXmlElement* pNode = pScenarloListElem->FirstChildElement(SCENARIO_INFO_NODE);
	while(NULL != pNode)
	{
		TiXmlElement* pIDElem = CToolsHelp::FindElementByPath(pNode,"ID");
		CHECK_POINTER(pIDElem, IVS_PARA_INVALID);
		// ����ҵ�һ����ID,ɾ�����info�ڵ�
		if (strcmp(pIDValue, pIDElem->GetText()) == 0)
		{
			//bFind = true;
			pScenarloListElem->RemoveChild(pIDElem->Parent());
			break;
		}
		pNode = pNode->NextSiblingElement(SCENARIO_INFO_NODE);
	}
	/*
	// û���ҵ�����ڵ㣬ֱ�ӷ���ʧ��
	if (!bFind)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "ID not find","NA");
		return IVS_PARA_INVALID;
	}
	*/

	// ��ȡinfo�ڵ㣬�ҵ������
	TiXmlElement* pScenarloInfoElem = CToolsHelp::FindElementByPath(&pXmlDoc,"Content/ScenarioInfo");
	CHECK_POINTER(pScenarloInfoElem,IVS_FAIL);
	TiXmlNode* pCloneScenarioInfoNode = pScenarloInfoElem->Clone();
	CHECK_POINTER(pCloneScenarioInfoNode,IVS_FAIL);
	pScenarloListElem->LinkEndChild(pCloneScenarioInfoNode->ToElement());
	pScenarioXmlDoc.SaveFile(strPath.c_str());
	return IVS_SUCCEED;
}
// ɾ������
IVS_INT32 CFavorite::DelScenarioFavorite(const IVS_CHAR* pScenarioID)
{
	CHECK_POINTER(pScenarioID, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("pScenarioID:%s", pScenarioID);
	// ����ļ��Ƿ����
	CLocalConfig localConfig;
	std::string strPath = localConfig.CheckLocalConfigFileExist(SCENARIO_FAVORITE_FILE_NAME);
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_OPEN_FILE_ERROR,"file not exist","NA");
		return IVS_OPEN_FILE_ERROR;
	}

	// ����XML�ļ�������ID �ҵ��ڵ㲢ɾ��
	TiXmlDocument pScenarioXmlDoc;
	if (!pScenarioXmlDoc.LoadFile(strPath.c_str(), TIXML_ENCODING_LEGACY))
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "load file failed","NA");
		return IVS_PARA_INVALID;
	}
	TiXmlElement* pScenarloListElem = CToolsHelp::FindElementByPath(&pScenarioXmlDoc,SCENARIOLIST_NODE);
	CHECK_POINTER(pScenarloListElem, IVS_FAIL);

	// �����ҵ�һ����ID,�ҵ��� ֱ��ɾ�����info
	//bool bFind = false;
	TiXmlElement* pNode = pScenarloListElem->FirstChildElement(SCENARIO_INFO_NODE);
	while(NULL != pNode)
	{
		TiXmlElement* pIDElem = CToolsHelp::FindElementByPath(pNode,"ID");
		CHECK_POINTER(pIDElem, IVS_PARA_INVALID);
		// ����ҵ�һ����ID,ɾ�����info�ڵ�
		if (strcmp(pScenarioID, pIDElem->GetText()) == 0)
		{
			//bFind = true;
			pScenarloListElem->RemoveChild(pIDElem->Parent());
			pScenarioXmlDoc.SaveFile(strPath.c_str());
			return IVS_SUCCEED;
		}
		pNode = pNode->NextSiblingElement(SCENARIO_INFO_NODE);
	}
	// û���ҵ�����ڵ㣬ֱ�ӷ���ʧ��
	//if (!bFind)
	//{
	//	BP_RUN_LOG_ERR(IVS_PARA_INVALID, "ID not find","NA");
	//	return IVS_PARA_INVALID;
	//}
    BP_RUN_LOG_ERR(IVS_PARA_INVALID, "ID not find","NA");
	return IVS_SUCCEED;
}

// ��ȡ�����б�
IVS_INT32 CFavorite::GetScenarioFavoriteList(std::string& strScenarioInfoList)
{
	// ����ļ��Ƿ����
	CLocalConfig localConfig;
	std::string strPath = localConfig.MakeSureLocalConfigFileExist(SCENARIO_FAVORITE_FILE_NAME);
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_OPEN_FILE_ERROR,"file not exist","NA");
		return IVS_OPEN_FILE_ERROR;
	}

	// ����XML�ļ�������ID �ҵ��ڵ㲢����
	TiXmlDocument pScenarioXmlDoc;
	if (!pScenarioXmlDoc.LoadFile(strPath.c_str(), TIXML_ENCODING_LEGACY))
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "load file failed","NA");
		return IVS_PARA_INVALID;
	}
	TiXmlElement* pScenarloListElem = CToolsHelp::FindElementByPath(&pScenarioXmlDoc,SCENARIOLIST_NODE);
	CHECK_POINTER(pScenarloListElem, IVS_FAIL);

	// ����ɾ����layout,��ɾ������ʱ������Ϣ
	TiXmlElement* pNode = pScenarloListElem->FirstChildElement(SCENARIO_INFO_NODE);
	while(NULL != pNode)
	{
		TiXmlElement* pIDElem = CToolsHelp::FindElementByPath(pNode,"ID");
		CHECK_POINTER(pIDElem, IVS_PARA_INVALID);
		// ����ҵ�һ����ID,ɾ�����info�ڵ�
		if (strcmp(TEMP_SCENARIO_INFO_ID, pIDElem->GetText()) == 0)
		{
			pNode = pNode->NextSiblingElement(SCENARIO_INFO_NODE);
			pScenarloListElem->RemoveChild(pIDElem->Parent());
			continue;
		}
		// ɾ��layout�ڵ�
		TiXmlElement* pLayoutElem = CToolsHelp::FindElementByPath(pNode,"Layout");
		(void)pNode->RemoveChild(pLayoutElem);
		pNode = pNode->NextSiblingElement(SCENARIO_INFO_NODE);
	}

	TiXmlPrinter xmlPrinter;
	xmlPrinter.SetStreamPrinting();		

	if (!pScenarioXmlDoc.Accept(&xmlPrinter))
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "print error", "NA");
		return IVS_FAIL;
	}
//     IVS_CHAR* pScenarioXmlANSI = CToolsHelp::UTF8ToANSI(xmlPrinter.CStr());
//     CHECK_POINTER(pScenarioXmlANSI, IVS_FAIL);
	strScenarioInfoList  = xmlPrinter.CStr();
	BP_DBG_LOG("Get Scenario FavoriteList xml = %s", strScenarioInfoList.c_str());
 //   free(pScenarioXmlANSI);
	return IVS_SUCCEED;
}

// ��ȡ��������
IVS_INT32 CFavorite::GetScenarioFavorite(const IVS_CHAR* pScenarioID, std::string& strScenarioInfo)
{
	CHECK_POINTER(pScenarioID, IVS_PARA_INVALID);
    IVS_DEBUG_TRACE("pScenarioID:%s", pScenarioID);
	// ����ļ��Ƿ����
	CLocalConfig localConfig;
	std::string strPath = localConfig.MakeSureLocalConfigFileExist(SCENARIO_FAVORITE_FILE_NAME);
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_OPEN_FILE_ERROR,"file not exist","NA");
		return IVS_OPEN_FILE_ERROR;
	}

	// ����XML�ļ�������ID �ҵ��ڵ㲢����
	TiXmlDocument pScenarioXmlDoc;
	if (!pScenarioXmlDoc.LoadFile(strPath.c_str(), TIXML_ENCODING_LEGACY))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "load file failed","FilePath[%s]", strPath.c_str());
		return IVS_XML_INVALID;
	}
	TiXmlElement* pScenarloListElem = CToolsHelp::FindElementByPath(&pScenarioXmlDoc,SCENARIOLIST_NODE);
	CHECK_POINTER(pScenarloListElem, IVS_FAIL);

	// �����ҵ�һ����ID,������Ϣ
	//bool bFind = false;
	TiXmlElement* pNode = pScenarloListElem->FirstChildElement(SCENARIO_INFO_NODE);
	while(NULL != pNode)
	{
		TiXmlElement* pIDElem = CToolsHelp::FindElementByPath(pNode,"ID");
		CHECK_POINTER(pIDElem, IVS_PARA_INVALID);
		// ����ҵ�һ����ID,������Ϣ
		if (strcmp(pScenarioID, pIDElem->GetText()) == 0)
		{
			//bFind = true;
			TiXmlPrinter xmlPrinter;
			xmlPrinter.SetStreamPrinting();		
			if (!pIDElem->Parent()->Accept(&xmlPrinter))
			{
				BP_RUN_LOG_ERR(IVS_FAIL, "print error", "NA");
				return IVS_FAIL;//lint !e429
			}
//             IVS_CHAR* pScenarioXmlANSI = CToolsHelp::UTF8ToANSI(xmlPrinter.CStr());
//             CHECK_POINTER(pScenarioXmlANSI, IVS_FAIL);
            strScenarioInfo = xmlPrinter.CStr();
            BP_DBG_LOG("strScenarioInfo = %s", strScenarioInfo.c_str());
          //  free(pScenarioXmlANSI);
			return IVS_SUCCEED;
		}
		pNode = pNode->NextSiblingElement(SCENARIO_INFO_NODE);
	}
	// û���ҵ�����ڵ㣬ֱ�ӷ���ʧ��
	//if (!bFind)
	//{
	//	BP_RUN_LOG_ERR(IVS_FAIL, "ID not find","NA");
	//	return IVS_FAIL;
	//}
    BP_RUN_LOG_ERR(IVS_FAIL, "ID not find","NA");
	return IVS_FAIL;
}
IVS_INT32 CFavorite::GetTVWallScenarioFavorite(const IVS_CHAR* pScenarioID, std::string& strScenarioInfo)
{
		CHECK_POINTER(pScenarioID, IVS_PARA_INVALID);
    IVS_DEBUG_TRACE("pScenarioID:%s", pScenarioID);
	// ����ļ��Ƿ����
	CLocalConfig localConfig;
	std::string strPath = localConfig.MakeSureLocalConfigFileExist(TVWALL_SCENARIO_FAVORITE_FILE_NAME);
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_OPEN_FILE_ERROR,"file not exist","NA");
		return IVS_OPEN_FILE_ERROR;
	}

	// ����XML�ļ�������ID �ҵ��ڵ㲢����
	TiXmlDocument pScenarioXmlDoc;
	if (!pScenarioXmlDoc.LoadFile(strPath.c_str(), TIXML_ENCODING_LEGACY))
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "load file failed","NA");
		return IVS_PARA_INVALID;
	}
	TiXmlElement* pScenarloListElem = CToolsHelp::FindElementByPath(&pScenarioXmlDoc,SCENARIOLIST_NODE);
	CHECK_POINTER(pScenarloListElem, IVS_FAIL);

	// �����ҵ�һ����ID,������Ϣ
	//bool bFind = false;
	TiXmlElement* pNode = pScenarloListElem->FirstChildElement(SCENARIO_INFO_NODE);
	while(NULL != pNode)
	{
		TiXmlElement* pIDElem = CToolsHelp::FindElementByPath(pNode,"ID");
		CHECK_POINTER(pIDElem, IVS_PARA_INVALID);
		// ����ҵ�һ����ID,������Ϣ
		if (strcmp(pScenarioID, pIDElem->GetText()) == 0)
		{
			//bFind = true;
			TiXmlPrinter xmlPrinter;
			xmlPrinter.SetStreamPrinting();		
			if (!pIDElem->Parent()->Accept(&xmlPrinter))
			{
				BP_RUN_LOG_ERR(IVS_FAIL, "print error", "NA");
				return IVS_FAIL;//lint !e429
			}
			strScenarioInfo = xmlPrinter.CStr();
            BP_DBG_LOG("strScenarioInfo = %s", strScenarioInfo.c_str());
			return IVS_SUCCEED;
		}
		pNode = pNode->NextSiblingElement(SCENARIO_INFO_NODE);
	}
	// û���ҵ�����ڵ㣬ֱ�ӷ���ʧ��
	//if (!bFind)
	//{
	//	BP_RUN_LOG_ERR(IVS_FAIL, "ID not find","NA");
	//	return IVS_FAIL;
	//}
    BP_RUN_LOG_ERR(IVS_FAIL, "ID not find","NA");
	return IVS_FAIL;
}
// ��ȡ��ʱ����
std::string CFavorite::GetTempScenario(const IVS_CHAR* pScenarioID)
{
	std::string strScenarioInfo;
	IVS_INT32 iResultCode = GetScenarioFavorite(pScenarioID, strScenarioInfo);
	// ȥ��ID�ֶ� ���س�ȥ
	std::string strResult;
	BulidGetScenarioInfoResult(iResultCode, strScenarioInfo, strResult);
	CXml xml;
	if (!xml.Parse(strResult.c_str()))
	{
		BP_RUN_LOG_ERR(IVS_FAIL,"parse xml failed", "NA");
		return "";
	}
	if (!xml.FindElemEx("Content/ScenarioInfo/ID"))
	{
		BP_RUN_LOG_ERR(IVS_FAIL,"not find id elem", "NA");
		return "";
	}
	(void)xml.RemoveElem();	
	unsigned int len = 0;

	return xml.GetXMLStream(len); 
}

//��ȡ����ǽ��ʱ����
std::string CFavorite::GetTVWallTempScenario(const std::string& strScenarioID)
{
	std::string strScenarioInfo;
	IVS_INT32 iResultCode = GetTVWallScenarioFavorite(strScenarioID.c_str(), strScenarioInfo);
	// ȥ��ID�ֶ� ���س�ȥ
	std::string strResult;
	BulidGetScenarioInfoResult(iResultCode, strScenarioInfo, strResult);
	CXml xml;
	if (!xml.Parse(strResult.c_str()))
	{
		BP_RUN_LOG_ERR(IVS_FAIL,"parse xml failed", "NA");
		return "";
	}
	//if (!xml.FindElemEx("Content/ScenarioInfo/ID"))
	//{
		//BP_RUN_LOG_ERR(IVS_FAIL,"not find id elem", "NA");
		//return "";
	//}
	//(void)xml.RemoveElem();	
	unsigned int len = 0;

	return xml.GetXMLStream(len); 
}

// ƴ������������Ӧ
void CFavorite::BulidAddScenarioInfoResult(IVS_INT32 iResultCode, const std::string& strScenarioID, std::string& strResult)
{
	CXml xml;
	(void)xml.AddDeclaration("1.0","UTF-8","");
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResultCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CToolsHelp::Int2Str(iResultCode).c_str());
	(void)xml.AddElem("ID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(strScenarioID.c_str());
	unsigned int len = 0;
	const char* pXml = xml.GetXMLStream(len);
	CHECK_POINTER_VOID(pXml);
	strResult = pXml;
}

// ƴ�ӻ�ȡ������Ӧ
void CFavorite::BulidGetScenarioInfoResult(IVS_INT32 iResultCode, const std::string& strScenarioInfo, std::string& strResult)
{
	CXml xml;
	(void)xml.AddDeclaration("1.0","UTF-8","");
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResultCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CToolsHelp::Int2Str(iResultCode).c_str());
	unsigned int len = 0;
	const char* pXml = xml.GetXMLStream(len);
	CHECK_POINTER_VOID(pXml);
	TiXmlDocument pXmlDoc;
	(void)(pXmlDoc.Parse(pXml, NULL, TIXML_ENCODING_LEGACY));
	if (0 != pXmlDoc.Error())
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "parse xml failed","NA");
		return;
	}
	// ��ȡContent�ڵ�
	TiXmlElement* pContentElem = CToolsHelp::FindElementByPath(&pXmlDoc,"Content");
	CHECK_POINTER_VOID(pContentElem);
	// �����ѯ��info,����Ǹ��յģ���Ҫƴװ����XML��CU
	TiXmlDocument pScenarioInfoDoc;
	(void)(pScenarioInfoDoc.Parse(strScenarioInfo.c_str(), NULL, TIXML_ENCODING_LEGACY));
	if (0 != pScenarioInfoDoc.Error())
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "parse xml failed","NA");
		TiXmlPrinter xmlPrinter1;
		xmlPrinter1.SetStreamPrinting();		

		if (!pXmlDoc.Accept(&xmlPrinter1))
		{
			BP_RUN_LOG_ERR(IVS_FAIL, "print error", "NA");
			return;
		}
		strResult = xmlPrinter1.CStr();
		return;
	}

	TiXmlNode* pCloneScenarioInfo = pScenarioInfoDoc.RootElement()->Clone();
	CHECK_POINTER_VOID(pCloneScenarioInfo);
	pContentElem->LinkEndChild(pCloneScenarioInfo->ToElement());

	TiXmlPrinter xmlPrinter;
	xmlPrinter.SetStreamPrinting();		

	if (!pXmlDoc.Accept(&xmlPrinter))
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "print error", "NA");
		return;
	}
	strResult = xmlPrinter.CStr();
}

// ƴ�ӻ�ȡ�����б���Ӧ
void CFavorite::BulidGetScenarioInfoListResult(IVS_INT32 iResultCode, const std::string& strScenarioInfoList, std::string& strResult)
{
	TiXmlDocument pXmlDoc;
	(void)(pXmlDoc.Parse(strScenarioInfoList.c_str(), NULL, TIXML_ENCODING_LEGACY));
	if (0 != pXmlDoc.Error())
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "parse xml failed","NA");
		return;
	}
	// ��ȡContent�ڵ�
	TiXmlElement* pContentElem = CToolsHelp::FindElementByPath(&pXmlDoc,"Content");
	CHECK_POINTER_VOID(pContentElem);
	// ����resultcode�ڵ�
	TiXmlElement* pResultCodeElem = new TiXmlElement("ResultCode");
	CHECK_POINTER_VOID(pResultCodeElem); //lint !e774
	pResultCodeElem->LinkEndChild(new TiXmlText(CToolsHelp::Int2Str(iResultCode).c_str()));
	// ���뵽content����
	pContentElem->InsertBeforeChild(pContentElem->FirstChildElement(), *pResultCodeElem);
	delete pResultCodeElem;
	pResultCodeElem = NULL; // TODO ����
	// ��ӡ���ڵ�
	TiXmlPrinter xmlPrinter;
	xmlPrinter.SetStreamPrinting();		

	if (!pXmlDoc.Accept(&xmlPrinter))
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "print error", "NA");
		return;
	}
	strResult = xmlPrinter.CStr();
}

// ������Ѳ��Դ
IVS_INT32 CFavorite::AddCameraSwitch(const IVS_CHAR* pSwitchInfo, const std::string& strSwitchID)
{
	CHECK_POINTER(pSwitchInfo, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("pSwitchInfoXml:%s", pSwitchInfo);
	// �ж��ļ��Ƿ���ڣ������ھʹ���һ��ģ��
	CLocalConfig localConfig;
	string strPath = localConfig.MakeSureLocalConfigFileExist(CAMERA_SWITCH_FILE_NAME);
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_CREATE_FILE_ERROR, "file create failed","NA");
		return IVS_CREATE_FILE_ERROR;
	}

	// ����XML�ļ�,�ҵ�list�ڵ�
	TiXmlDocument pCameraSwitchXmlDoc;
	if (!pCameraSwitchXmlDoc.LoadFile(strPath.c_str(), TIXML_ENCODING_LEGACY))
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "load file failed","NA");
		return IVS_PARA_INVALID;
	}

	TiXmlElement* pCameraSwitchListElem = CToolsHelp::FindElementByPath(&pCameraSwitchXmlDoc,CAMERASWITCHLIST_NODE);
	CHECK_POINTER(pCameraSwitchListElem, IVS_FAIL);

	// ��һ��ID�ڵ㵽�����XML
	TiXmlDocument pXmlDoc;
	(void)(pXmlDoc.Parse(pSwitchInfo, NULL, TIXML_ENCODING_LEGACY));
	if (0 != pXmlDoc.Error())
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "parse xml failed","NA");
		return IVS_PARA_INVALID;
	}

	// ����ID�Ľڵ�
	TiXmlElement* pCameraSwitchIDElem = new TiXmlElement(CAMERASWITCH_ID_NODE);
	CHECK_POINTER(pCameraSwitchIDElem, IVS_PARA_INVALID); //lint !e774
	pCameraSwitchIDElem->LinkEndChild(new TiXmlText(strSwitchID.c_str()));
	// ��ԭ�������ϰ�ID�Ž�ȥ
	TiXmlElement* pCameraSwitchInfoElem = CToolsHelp::FindElementByPath(&pXmlDoc,"Content/CameraSwitchInfo");
	if (NULL == pCameraSwitchInfoElem)
	{
		delete pCameraSwitchIDElem;
		pCameraSwitchIDElem = NULL;
		return IVS_PARA_INVALID;
	}
	pCameraSwitchInfoElem->InsertBeforeChild(pCameraSwitchInfoElem->FirstChildElement(), *pCameraSwitchIDElem);
	delete pCameraSwitchIDElem;
	pCameraSwitchIDElem = NULL; //TODO ����
	// ���뵽�ļ�
	TiXmlNode* pCloneCameraSwitchInfoNode = pCameraSwitchInfoElem->Clone();
    if(NULL == pCloneCameraSwitchInfoNode)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "parse xml failed","pCloneCameraSwitchInfoNode is null.");
        return IVS_PARA_INVALID;
    }

	pCameraSwitchListElem->LinkEndChild(pCloneCameraSwitchInfoNode->ToElement());
	pCameraSwitchXmlDoc.SaveFile(strPath.c_str());
	return IVS_SUCCEED;
}

// ɾ����Ѳ��Դ
IVS_INT32 CFavorite::DelCameraSwitch(const IVS_CHAR* pCameraSwitchID)
{
	CHECK_POINTER(pCameraSwitchID, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("pCameraSwitchID:%s", pCameraSwitchID);
	// ����ļ��Ƿ����
	CLocalConfig localConfig;
	std::string strPath = localConfig.CheckLocalConfigFileExist(CAMERA_SWITCH_FILE_NAME);
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_OPEN_FILE_ERROR,"file not exist","NA");
		return IVS_OPEN_FILE_ERROR;
	}

	// ����XML�ļ�������ID �ҵ��ڵ㲢ɾ��
	TiXmlDocument pCameraSwitchXmlDoc;
	if (!pCameraSwitchXmlDoc.LoadFile(strPath.c_str(), TIXML_ENCODING_LEGACY))
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "load file failed","NA");
		return IVS_PARA_INVALID;
	}
	TiXmlElement* pCameraSwitchListElem = CToolsHelp::FindElementByPath(&pCameraSwitchXmlDoc,CAMERASWITCHLIST_NODE);
	CHECK_POINTER(pCameraSwitchListElem, IVS_FAIL);

	// �����ҵ�һ����ID,�ҵ��� ֱ��ɾ�����info
	//bool bFind = false;
	TiXmlElement* pNode = pCameraSwitchListElem->FirstChildElement(CAMERASWITCH_INFO_NODE);
	while(NULL != pNode)
	{
		TiXmlElement* pIDElem = CToolsHelp::FindElementByPath(pNode,"CameraSwitchID");
		CHECK_POINTER(pIDElem, IVS_PARA_INVALID);
		// ����ҵ�һ����ID,ɾ�����info�ڵ�
		if (strcmp(pCameraSwitchID, pIDElem->GetText()) == 0)
		{
			//bFind = true;
			pCameraSwitchListElem->RemoveChild(pIDElem->Parent());
			pCameraSwitchXmlDoc.SaveFile(strPath.c_str());
			return IVS_SUCCEED;
		}
		pNode = pNode->NextSiblingElement(CAMERASWITCH_INFO_NODE);
	}
	// û���ҵ�����ڵ㣬ֱ�ӷ���ʧ��
	//if (!bFind)
	//{
	//	BP_RUN_LOG_ERR(IVS_PARA_INVALID, "ID not find","NA");
	//	return IVS_PARA_INVALID;
	//}
    BP_RUN_LOG_ERR(IVS_PARA_INVALID, "ID not find","NA");
	return IVS_SUCCEED;
}
// �޸���ѵ��Դ
IVS_INT32 CFavorite::ModCameraSwitch(const IVS_CHAR* pSwitchInfo)
{
    IVS_DEBUG_TRACE("pSwitchInfo:%s", pSwitchInfo);
    CXml xmlReq;
    if (!xmlReq.Parse(pSwitchInfo))
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "IVS OCX Modify CameraSwitch Info Failed.", "NA");
        return IVS_PARA_INVALID;
    }
    // ��Ѳ��ԴID
    if (!xmlReq.FindElemEx("Content/CameraSwitchInfo/CameraSwitchID"))
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "IVS OCX Modify CameraSwitch Info Failed.", "NA");
        return IVS_PARA_INVALID;
    }
	const char* pCameraSwitchID = xmlReq.GetElemValue();
	CHECK_POINTER(pCameraSwitchID, IVS_XML_INVALID);
    std::string strCameraSwitchID = pCameraSwitchID;
    IVS_INT32 iDelRet = DelCameraSwitch(strCameraSwitchID.c_str());
    BP_RUN_LOG_INF("IVS OCX Delete CameraSwitch Info", "iDelRet:%d", iDelRet);

    // �ж��ļ��Ƿ���ڣ������ھʹ���һ��ģ��
    CLocalConfig localConfig;
    string strPath = localConfig.MakeSureLocalConfigFileExist(CAMERA_SWITCH_FILE_NAME);
    if (strPath.empty())
    {
        BP_RUN_LOG_ERR(IVS_CREATE_FILE_ERROR, "file create failed","NA");
        return IVS_CREATE_FILE_ERROR;
    }

    // ����XML�ļ�,�ҵ�list�ڵ�
    TiXmlDocument pCameraSwitchXmlDoc;
    if (!pCameraSwitchXmlDoc.LoadFile(strPath.c_str(), TIXML_ENCODING_LEGACY))
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "load file failed","NA");
        return IVS_PARA_INVALID;
    }
    TiXmlElement* pCameraSwitchListElem = CToolsHelp::FindElementByPath(&pCameraSwitchXmlDoc,CAMERASWITCHLIST_NODE);
    CHECK_POINTER(pCameraSwitchListElem, IVS_FAIL);

    // �����XML���뵽�ļ�
    TiXmlDocument pXmlDoc;
    (void)(pXmlDoc.Parse(pSwitchInfo, NULL, TIXML_ENCODING_LEGACY));
    if (0 != pXmlDoc.Error())
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "parse xml failed","NA");
        return IVS_PARA_INVALID;
    }
    
    TiXmlElement* pCameraSwitchInfoElem = CToolsHelp::FindElementByPath(&pXmlDoc,"Content/CameraSwitchInfo");
    CHECK_POINTER(pCameraSwitchInfoElem, IVS_PARA_INVALID);

    TiXmlNode* pCloneCameraSwitchInfoNode = pCameraSwitchInfoElem->Clone();
    if (NULL == pCloneCameraSwitchInfoNode)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "ModCameraSwitch parse xml failed","pCloneCameraSwitchInfoNode is null.");
        return IVS_PARA_INVALID;
    }
    pCameraSwitchListElem->LinkEndChild(pCloneCameraSwitchInfoNode->ToElement());
    pCameraSwitchXmlDoc.SaveFile(strPath.c_str());
    return IVS_SUCCEED;

}
// ��ȡ��Ѳ��Դ�б�
IVS_INT32 CFavorite::GetCameraSwitchInfoList(std::string& strCameraSwitchInfoList)
{
	// ����ļ��Ƿ����
	CLocalConfig localConfig;
	std::string strPath = localConfig.MakeSureLocalConfigFileExist(CAMERA_SWITCH_FILE_NAME);
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_OPEN_FILE_ERROR,"file not exist","NA");
		return IVS_OPEN_FILE_ERROR;
	}

	// ����XML�ļ�������ID �ҵ��ڵ㲢����
	TiXmlDocument pCameraSwitchXmlDoc;
	if (!pCameraSwitchXmlDoc.LoadFile(strPath.c_str(),TIXML_ENCODING_LEGACY))
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "load file failed","NA");
		return IVS_PARA_INVALID;
	}
	TiXmlElement* pCameraSwitchListElem = CToolsHelp::FindElementByPath(&pCameraSwitchXmlDoc,CAMERASWITCHLIST_NODE);
	CHECK_POINTER(pCameraSwitchListElem, IVS_FAIL);

	// ����ɾ����CameraGroupList
	TiXmlElement* pNode = pCameraSwitchListElem->FirstChildElement(CAMERASWITCH_INFO_NODE);
	while(NULL != pNode)
	{
		// ɾ��CameraGroupList�ڵ�
		TiXmlElement* pCameraGroupListElem = CToolsHelp::FindElementByPath(pNode,"CameraGroupList");
		(void)pNode->RemoveChild(pCameraGroupListElem);
		pNode = pNode->NextSiblingElement(CAMERASWITCH_INFO_NODE);
	}

	TiXmlPrinter xmlPrinter;
	xmlPrinter.SetStreamPrinting();		

	if (!pCameraSwitchXmlDoc.Accept(&xmlPrinter))
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "print error", "NA");
		return IVS_FAIL;
	}

	strCameraSwitchInfoList  = xmlPrinter.CStr();
	BP_DBG_LOG("Get CameraSwitch List xml = %s", strCameraSwitchInfoList.c_str());
	return IVS_SUCCEED;
}

// ��ѯ��Ѳ��Դ����
IVS_INT32 CFavorite::GetCameraSwitchInfo(const IVS_CHAR* pCameraSwitchID, std::string& strCameraSwitchInfo)
{
	CHECK_POINTER(pCameraSwitchID, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("pCameraSwitchID:%s", pCameraSwitchID);
	// ����ļ��Ƿ����
	CLocalConfig localConfig;
	std::string strPath = localConfig.MakeSureLocalConfigFileExist(CAMERA_SWITCH_FILE_NAME);
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_OPEN_FILE_ERROR,"file not exist","NA");
		return IVS_OPEN_FILE_ERROR;
	}

	// ����XML�ļ�������ID �ҵ��ڵ㲢����
	TiXmlDocument pScenarioXmlDoc;
	if (!pScenarioXmlDoc.LoadFile(strPath.c_str(), TIXML_ENCODING_LEGACY))
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "load file failed","NA");
		return IVS_PARA_INVALID;
	}
	TiXmlElement* pCameraSwitchListElem = CToolsHelp::FindElementByPath(&pScenarioXmlDoc,CAMERASWITCHLIST_NODE);
	CHECK_POINTER(pCameraSwitchListElem, IVS_FAIL);

	// �����ҵ�һ����ID�����������Ϣ
	//bool bFind = false;
	TiXmlElement* pNode = pCameraSwitchListElem->FirstChildElement(CAMERASWITCH_INFO_NODE);
	while(NULL != pNode)
	{
		TiXmlElement* pIDElem = CToolsHelp::FindElementByPath(pNode,"CameraSwitchID");
		CHECK_POINTER(pIDElem, IVS_PARA_INVALID);
		// ����ҵ�һ����ID,���������Ϣ
		if (strcmp(pCameraSwitchID, pIDElem->GetText()) == 0)
		{
			//bFind = true;
			TiXmlPrinter xmlPrinter;
			xmlPrinter.SetStreamPrinting();		
			if (!pIDElem->Parent()->Accept(&xmlPrinter))
			{
				BP_RUN_LOG_ERR(IVS_FAIL, "print error", "NA");
				return IVS_FAIL;//lint !e429
			}
			strCameraSwitchInfo = xmlPrinter.CStr();
			BP_DBG_LOG("strCameraSwitchInfo = %s", strCameraSwitchInfo.c_str());
			return IVS_SUCCEED;
		}
		pNode = pNode->NextSiblingElement(CAMERASWITCH_INFO_NODE);
	}
	// û���ҵ�����ڵ㣬ֱ�ӷ���ʧ��
	//if (!bFind)
	//{
	//	BP_RUN_LOG_ERR(IVS_FAIL, "ID not find","NA");
	//	return IVS_FAIL;
	//}
    BP_RUN_LOG_ERR(IVS_FAIL, "ID not find","NA");
	return IVS_FAIL;
}

// ƴ��������Ѳ��Դ��Ӧ
void CFavorite::BulidAddCameraSwitchResult(IVS_INT32 iResultCode, const std::string& strCameraSwitchID, std::string& strResult)
{
	CXml xml;
	(void)xml.AddDeclaration("1.0","UTF-8","");
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResultCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CToolsHelp::Int2Str(iResultCode).c_str());
	(void)xml.AddElem("CameraSwitchID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(strCameraSwitchID.c_str());
	unsigned int len = 0;
	const char* pXml = xml.GetXMLStream(len);
	CHECK_POINTER_VOID(pXml);
	strResult = pXml;
}

// ƴ�ӻ�ȡ��Ѳ��Դ�б���Ӧ
void CFavorite::BulidGetCameraSwitchListResult(IVS_INT32 iResultCode, const std::string& strCameraSwitchInfoList, std::string& strResult)
{
	TiXmlDocument pXmlDoc;
	(void)(pXmlDoc.Parse(strCameraSwitchInfoList.c_str(), NULL, TIXML_ENCODING_LEGACY));
	if (0 != pXmlDoc.Error())
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "parse xml failed","NA");
		return;
	}
	// ��ȡContent�ڵ�
	TiXmlElement* pContentElem = CToolsHelp::FindElementByPath(&pXmlDoc,"Content");
	CHECK_POINTER_VOID(pContentElem);
	// ����resultcode�ڵ�
	TiXmlElement* pResultCodeElem = new TiXmlElement("ResultCode");
	CHECK_POINTER_VOID(pResultCodeElem); //lint !e774
	pResultCodeElem->LinkEndChild(new TiXmlText(CToolsHelp::Int2Str(iResultCode).c_str()));
	// ���뵽content����
	pContentElem->InsertBeforeChild(pContentElem->FirstChildElement(), *pResultCodeElem);
	delete pResultCodeElem;
	pResultCodeElem = NULL; // TODO ����
	// ��ӡ���ڵ�
	TiXmlPrinter xmlPrinter;
	xmlPrinter.SetStreamPrinting();		

	if (!pXmlDoc.Accept(&xmlPrinter))
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "print error", "NA");
		return;
	}
	strResult = xmlPrinter.CStr();
}

// ƴ�Ӳ�ѯ��Ѳ��Դ������Ӧ
void CFavorite::BulidGetCameraSwitchInfoResult(IVS_INT32 iResultCode, const std::string& strCameraSwitchInfo, std::string& strResult)
{
	CXml xml;
	(void)xml.AddDeclaration("1.0","UTF-8","");
	(void)xml.AddElem("Content");
	(void)xml.AddChildElem("ResultCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CToolsHelp::Int2Str(iResultCode).c_str());
	unsigned int len = 0;
	const char* pXml = xml.GetXMLStream(len);
	CHECK_POINTER_VOID(pXml);
	TiXmlDocument pXmlDoc;
	(void)(pXmlDoc.Parse(pXml, NULL, TIXML_ENCODING_LEGACY));
	if (0 != pXmlDoc.Error())
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "parse xml failed","NA");
		return;
	}
	// ��ȡContent�ڵ�
	TiXmlElement* pContentElem = CToolsHelp::FindElementByPath(&pXmlDoc,"Content");
	CHECK_POINTER_VOID(pContentElem);
	// �����ѯ��info
	TiXmlDocument pCameraSwitchInfoDoc;
	(void)(pCameraSwitchInfoDoc.Parse(strCameraSwitchInfo.c_str(), NULL, TIXML_ENCODING_LEGACY));
	if (0 != pCameraSwitchInfoDoc.Error())
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "parse xml failed","NA");
		return;
	}

	TiXmlNode* pCloneCameraSwitchInfo = pCameraSwitchInfoDoc.RootElement()->Clone();
	CHECK_POINTER_VOID(pCloneCameraSwitchInfo);
	pContentElem->LinkEndChild(pCloneCameraSwitchInfo->ToElement());

	TiXmlPrinter xmlPrinter;
	xmlPrinter.SetStreamPrinting();		

	if (!pXmlDoc.Accept(&xmlPrinter))
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "print error", "NA");
		return;
	}
	strResult = xmlPrinter.CStr();
}

// �����Ѳ�ƻ�
IVS_INT32 CFavorite::AddCameraSwitchPlan(const IVS_CHAR* pSwitchPlanInfo)
{
	CHECK_POINTER(pSwitchPlanInfo, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("pSwitchPlanInfo:%s", pSwitchPlanInfo);
	// �ж��ļ��Ƿ���ڣ������ھʹ���һ��ģ��
	CLocalConfig localConfig;
	string strPath = localConfig.MakeSureLocalConfigFileExist(CAMERRA_SWITCH_PLAN_FINE_NAME);
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_CREATE_FILE_ERROR, "file create failed","NA");
		return IVS_CREATE_FILE_ERROR;
	}


	// ����XML�ļ�,�ҵ�list�ڵ�
	TiXmlDocument pSwitchPlanXmlDoc;
	if (!pSwitchPlanXmlDoc.LoadFile(strPath.c_str(), TIXML_ENCODING_LEGACY))
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "load file failed","NA");
		return IVS_PARA_INVALID;
	}
	TiXmlElement* pCameraSwitchListElem = CToolsHelp::FindElementByPath(&pSwitchPlanXmlDoc,CAMERASWITCHLPLANIST_NODE);
	CHECK_POINTER(pCameraSwitchListElem, IVS_FAIL);

	// ��һ��ID�ڵ㵽�����XML
	TiXmlDocument pXmlDoc;
	(void)(pXmlDoc.Parse(pSwitchPlanInfo, NULL, TIXML_ENCODING_LEGACY));
	if (0 != pXmlDoc.Error())
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "parse xml failed","NA");
		return IVS_PARA_INVALID;
	}

	TiXmlElement* pCameraSwitchInfoElem = CToolsHelp::FindElementByPath(&pXmlDoc,"Content/CameraSwitchPlan");
	if (NULL == pCameraSwitchInfoElem)
	{
		return IVS_PARA_INVALID;
	}

	// ���뵽�ļ�
	TiXmlNode* pCloneCameraSwitchInfoNode = pCameraSwitchInfoElem->Clone();
    if (NULL == pCloneCameraSwitchInfoNode)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "AddCameraSwitchPlan parse xml failed","pCloneCameraSwitchInfoNode is null.");
        return IVS_PARA_INVALID;
    }
	pCameraSwitchListElem->LinkEndChild(pCloneCameraSwitchInfoNode->ToElement());
	pSwitchPlanXmlDoc.SaveFile(strPath.c_str());
	return IVS_SUCCEED;
}

//���ӵ���ǽ��ѵ�ƻ�
IVS_INT32 CFavorite::AddCameraSwitchPlanTVWall(const IVS_CHAR* pSwitchPlanInfo)
{
	CHECK_POINTER(pSwitchPlanInfo, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("SwitchPlanInfo:%s", pSwitchPlanInfo);

	//����ļ��Ƿ����,�������򴴽�һ��
	CLocalConfig localConfig;
	string strPath = localConfig.MakeSureLocalConfigFileExist(CAMERRA_SWITCH_PLAN_TVWALL_FILE_NAME);
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_CREATE_FILE_ERROR, "file create failed","NA");
		return IVS_CREATE_FILE_ERROR;
	}
	
	// ��ȡ����ǽIDֵ����ѵID
	TiXmlDocument pXmlDoc;
	(void)(pXmlDoc.Parse(pSwitchPlanInfo, NULL, TIXML_ENCODING_LEGACY));
	if (0 != pXmlDoc.Error())
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "parse xml failed","NA");
		return IVS_PARA_INVALID;
	}

	TiXmlElement* pTVWallIDElem = CToolsHelp::FindElementByPath(&pXmlDoc,"Content/CameraSwitchPlan/TVWallID");
	CHECK_POINTER(pTVWallIDElem, IVS_PARA_INVALID);
	const char* pTVWallIDValue = pTVWallIDElem->GetText();
	TiXmlElement* pCameraSwitchIDElem = CToolsHelp::FindElementByPath(&pXmlDoc,"Content/CameraSwitchPlan/CameraSwitchID");
	CHECK_POINTER(pCameraSwitchIDElem, IVS_PARA_INVALID);
	const char* pSwitchPlanIDValue = pCameraSwitchIDElem->GetText();

	//����XML�ļ������ݵ���ǽID �ҵ��ڵ㲢ɾ��
	TiXmlDocument pSwitchPlanXmlDoc;
	if (!pSwitchPlanXmlDoc.LoadFile(strPath.c_str(), TIXML_ENCODING_LEGACY))
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "load file failed","NA");
		return IVS_PARA_INVALID;
	}
	TiXmlElement* pSwitchPlanListElem = CToolsHelp::FindElementByPath(&pSwitchPlanXmlDoc,CAMERASWITCHLPLANIST_NODE);
	CHECK_POINTER(pSwitchPlanListElem, IVS_FAIL);

	// �����ҵ�һ����ID,�ҵ��� ֱ��ɾ�����info
	//bool bFind = false;
	TiXmlElement* pNode = pSwitchPlanListElem->FirstChildElement(CAMERASWITCHPLAN_INFO_NODE);
	while(NULL != pNode)
	{
		TiXmlElement* pTVWallID = CToolsHelp::FindElementByPath(pNode,"TVWallID");
		TiXmlElement* pCameraSwitchID = CToolsHelp::FindElementByPath(pNode,"CameraSwitchID");
		if(NULL == pTVWallID || NULL == pCameraSwitchID)
		{
			pNode = pNode->NextSiblingElement(CAMERASWITCHPLAN_INFO_NODE);
			continue;
		}

		// ����ҵ�һ����ID,ɾ�����info�ڵ�
		if (0==strcmp(pTVWallIDValue, pTVWallID->GetText()) && 0==strcmp(pSwitchPlanIDValue,pCameraSwitchID->GetText()))
		{
			//bFind = true;
			pSwitchPlanListElem->RemoveChild(pTVWallID->Parent());
			break;
		}
		pNode = pNode->NextSiblingElement(CAMERASWITCHPLAN_INFO_NODE);
	}

	// ��ȡinfo�ڵ㣬�ҵ������
	TiXmlElement* pSwitchPlanElem = CToolsHelp::FindElementByPath(&pXmlDoc,"Content/CameraSwitchPlan");
	CHECK_POINTER(pSwitchPlanElem,IVS_FAIL);
	TiXmlNode* pCloneScenarioInfoNode = pSwitchPlanElem->Clone();
	CHECK_POINTER(pCloneScenarioInfoNode,IVS_FAIL);
	pSwitchPlanListElem->LinkEndChild(pCloneScenarioInfoNode->ToElement());
	pSwitchPlanXmlDoc.SaveFile(strPath.c_str());
	return IVS_SUCCEED;
}

// ɾ����Ѳ�ƻ�
IVS_INT32 CFavorite::DelCameraSwitchPlan(const IVS_CHAR* pCameraSwitchID)
{
	CHECK_POINTER(pCameraSwitchID, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("pCameraSwitchID:%s", pCameraSwitchID);
	// ����ļ��Ƿ����
	CLocalConfig localConfig;
	std::string strPath = localConfig.CheckLocalConfigFileExist(CAMERRA_SWITCH_PLAN_FINE_NAME);
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_OPEN_FILE_ERROR,"file not exist","NA");
		return IVS_OPEN_FILE_ERROR;
	}

	// ����XML�ļ�������ID �ҵ��ڵ㲢ɾ��
	TiXmlDocument pCameraSwitchXmlDoc;
	if (!pCameraSwitchXmlDoc.LoadFile(strPath.c_str(), TIXML_ENCODING_LEGACY))
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "load file failed","NA");
		return IVS_PARA_INVALID;
	}
	TiXmlElement* pCameraSwitchListElem = CToolsHelp::FindElementByPath(&pCameraSwitchXmlDoc,CAMERASWITCHLPLANIST_NODE);
	CHECK_POINTER(pCameraSwitchListElem, IVS_FAIL);

	// �����ҵ�һ����ID,�ҵ��� ֱ��ɾ�����info
	// bool bFind = false;
	TiXmlElement* pNode = pCameraSwitchListElem->FirstChildElement(CAMERASWITCHPLAN_INFO_NODE);
	while(NULL != pNode)
	{
		TiXmlElement* pIDElem = CToolsHelp::FindElementByPath(pNode,"CameraSwitchID");
		CHECK_POINTER(pIDElem, IVS_PARA_INVALID);
		// ����ҵ�һ����ID,ɾ�����info�ڵ�
		if (strcmp(pCameraSwitchID, pIDElem->GetText()) == 0)
		{
			// bFind = true;
			pCameraSwitchListElem->RemoveChild(pIDElem->Parent());
			pCameraSwitchXmlDoc.SaveFile(strPath.c_str());
			return IVS_SUCCEED;
		}
		pNode = pNode->NextSiblingElement(CAMERASWITCHPLAN_INFO_NODE);
	}
	// û���ҵ�����ڵ㣬ֱ�ӷ���ʧ��
	//if (!bFind)
	//{
	//	BP_RUN_LOG_ERR(IVS_PARA_INVALID, "ID not find","NA");
	//	return IVS_SUCCEED;
	//}
    BP_RUN_LOG_ERR(IVS_PARA_INVALID, "ID not find","NA");
	return IVS_SUCCEED;
}

// ɾ������ǽ��ѵ�ƻ�
IVS_INT32 CFavorite::DelCameraSwitchPlanTVWall(ULONG ulTVWallID,const IVS_CHAR* pCameraSwitchID)
{
	CHECK_POINTER(pCameraSwitchID, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("TVWallID:%lu,CameraSwitchID:%s", ulTVWallID,pCameraSwitchID);
	// ����ļ��Ƿ����
	CLocalConfig localConfig;
	std::string strPath = localConfig.CheckLocalConfigFileExist(CAMERRA_SWITCH_PLAN_TVWALL_FILE_NAME);
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_OPEN_FILE_ERROR,"Del TVWall CameraSwitchPlan Failed.","Reason:File[TVWallCameraSwitchPlan.xml] not exist.");
		return IVS_OPEN_FILE_ERROR;
	}

	// ����TVWallCameraSwitchPlan.xml�ļ�
	TiXmlDocument pCameraSwitchTVWallXmlDoc;
	if (!pCameraSwitchTVWallXmlDoc.LoadFile(strPath.c_str(), TIXML_ENCODING_LEGACY))
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Del TVWall CameraSwitchPlan Failed.","Reason:Load File[TVWallCameraSwitchPlan.xml] Failed.");
		return IVS_PARA_INVALID;
	}
	TiXmlElement* pCameraSwitchListTVWallElem = CToolsHelp::FindElementByPath(&pCameraSwitchTVWallXmlDoc,CAMERASWITCHLPLANIST_NODE);
	CHECK_POINTER(pCameraSwitchListTVWallElem, IVS_FAIL);

	// �����ҵ�һ����TVWallID��CameraSwitchID,�ҵ���ֱ��ɾ������ڵ�
	// bool bFind = false;
	TiXmlElement* pNode = pCameraSwitchListTVWallElem->FirstChildElement(CAMERASWITCHPLAN_INFO_NODE);
	while(NULL != pNode)
	{
		TiXmlElement* pTVWallIDElem = CToolsHelp::FindElementByPath(pNode,"TVWallID");
		TiXmlElement* pCameraSwitchIDElem = CToolsHelp::FindElementByPath(pNode,"CameraSwitchID");
		if(NULL == pTVWallIDElem || NULL == pCameraSwitchIDElem)
		{
			pNode = pNode->NextSiblingElement(CAMERASWITCHPLAN_INFO_NODE);
			continue;
		}
		// �Ƚ�
		std::string strTVWallID = std::string(pTVWallIDElem->GetText());
		std::string strTVWallIDForDel = CToolsHelp::Int2Str((IVS_INT32)ulTVWallID);

		std::string strCameraSwitchID = std::string(pCameraSwitchIDElem->GetText());
		std::string strCameraSwitchIDForDel = std::string(pCameraSwitchID);

		if (0 == strTVWallID.compare(strTVWallIDForDel) && 0 == strCameraSwitchID.compare(strCameraSwitchIDForDel))
		{
			// bFind = true;
			pCameraSwitchListTVWallElem->RemoveChild(pTVWallIDElem->Parent());
			pCameraSwitchTVWallXmlDoc.SaveFile(strPath.c_str());
			return IVS_SUCCEED;
		}
		pNode = pNode->NextSiblingElement(CAMERASWITCHPLAN_INFO_NODE);
	}
	// û���ҵ���ֱ�ӷ���ʧ��
	//if (!bFind)
	//{
	//	BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Del TVWall CameraSwitchPlan Failed.","Reason:TVWallID&CameraSwitchID Not Find.");
	//	return IVS_PARA_INVALID;
	//}
    BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Del TVWall CameraSwitchPlan Failed.","Reason:TVWallID&CameraSwitchID Not Find.");
	return IVS_SUCCEED;
}

// ��ѯ��Ѳ�ƻ�
IVS_INT32 CFavorite::GetCameraSwitchPlan(const IVS_CHAR* pCameraSwitchID, std::string& strCameraSwitchPlan)
{
	CHECK_POINTER(pCameraSwitchID, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("pCameraSwitchID:%s", pCameraSwitchID);
	// ����ļ��Ƿ����
	CLocalConfig localConfig;
	std::string strPath = localConfig.MakeSureLocalConfigFileExist(CAMERRA_SWITCH_PLAN_FINE_NAME);
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_OPEN_FILE_ERROR,"file not exist","NA");
		return IVS_OPEN_FILE_ERROR;
	}

	// ����XML�ļ�������ID �ҵ��ڵ㲢����
	TiXmlDocument pScenarioXmlDoc;
	if (!pScenarioXmlDoc.LoadFile(strPath.c_str(), TIXML_ENCODING_LEGACY))
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "load file failed","NA");
		return IVS_PARA_INVALID;
	}
	TiXmlElement* pCameraSwitchListElem = CToolsHelp::FindElementByPath(&pScenarioXmlDoc,CAMERASWITCHLPLANIST_NODE);
	CHECK_POINTER(pCameraSwitchListElem, IVS_FAIL);

	// �����ҵ�һ����ID�����������Ϣ
	//bool bFind = false;
	TiXmlElement* pNode = pCameraSwitchListElem->FirstChildElement(CAMERASWITCHPLAN_INFO_NODE);
	while(NULL != pNode)
	{
		TiXmlElement* pIDElem = CToolsHelp::FindElementByPath(pNode,"CameraSwitchID");
		CHECK_POINTER(pIDElem, IVS_PARA_INVALID);
		// ����ҵ�һ����ID,���������Ϣ
		if (strcmp(pCameraSwitchID, pIDElem->GetText()) == 0)
		{
			//bFind = true;
			TiXmlPrinter xmlPrinter;
			xmlPrinter.SetStreamPrinting();		
			if (!pIDElem->Parent()->Accept(&xmlPrinter))
			{
				BP_RUN_LOG_ERR(IVS_FAIL, "print error", "NA");
				return IVS_FAIL;//lint !e429
			}
			strCameraSwitchPlan = xmlPrinter.CStr();
			BP_DBG_LOG("strCameraSwitchPlan = %s", strCameraSwitchPlan.c_str());
			return IVS_SUCCEED;
		}
		pNode = pNode->NextSiblingElement(CAMERASWITCHPLAN_INFO_NODE);
	}
	// û���ҵ�����ڵ㣬ֱ�ӷ���ʧ��
	//if (!bFind)
	//{
	//	BP_RUN_LOG_ERR(IVS_FAIL, "ID not find","NA");
	//	return IVS_FAIL;
	//}
    BP_RUN_LOG_ERR(IVS_FAIL, "ID not find","NA");
	return IVS_FAIL;
}

// ��ȡ����ǽ��Ѳ�ƻ�
IVS_INT32 CFavorite::GetCameraSwitchPlanTVWall(ULONG ulTVWallID, const IVS_CHAR* pCameraSwitchID, std::string& strTVwallCameraSwitchPlan)
{
	CHECK_POINTER(pCameraSwitchID, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("TVWallID: %lu,CameraSwitchID: %s", ulTVWallID,pCameraSwitchID);
	// ����ļ�TVWallCameraSwitchPlan.xml�Ƿ����
	CLocalConfig localConfig;
	std::string strPath = localConfig.MakeSureLocalConfigFileExist(CAMERRA_SWITCH_PLAN_TVWALL_FILE_NAME);
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_OPEN_FILE_ERROR,"Get TVWall CameraSwitchPlan Failed.","Reason:File[TVWallCameraSwitchPlan.xml] not exist.");
		return IVS_OPEN_FILE_ERROR;
	}

	// ����TVWallCameraSwitchPlan.xml�ļ�������TVWallID �ҵ��ڵ㲢����
	TiXmlDocument pTVWallCameraSwitchPlanXmlDoc;
	if (!pTVWallCameraSwitchPlanXmlDoc.LoadFile(strPath.c_str(), TIXML_ENCODING_LEGACY))
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Get TVWall CameraSwitchPlan Failed.","Reason:load file[TVWallCameraSwitchPlan.xml] failed.");
		return IVS_PARA_INVALID;
	}
	TiXmlElement* pTVWallCameraSwitchPlanElem = CToolsHelp::FindElementByPath(&pTVWallCameraSwitchPlanXmlDoc,CAMERASWITCHLPLANIST_NODE);
	CHECK_POINTER(pTVWallCameraSwitchPlanElem, IVS_FAIL);

	// �����ҵ�һ����TVwallID�����������Ϣ
	// bool bFind = false;
	TiXmlElement* pNode = pTVWallCameraSwitchPlanElem->FirstChildElement(CAMERASWITCHPLAN_INFO_NODE);
	while(NULL != pNode)
	{
		TiXmlElement* pTVWallIDElem = CToolsHelp::FindElementByPath(pNode,"TVWallID");
		TiXmlElement* pCameraSwitchIDElem = CToolsHelp::FindElementByPath(pNode,"CameraSwitchID");
		if(NULL == pTVWallIDElem || NULL == pCameraSwitchIDElem)
		{
			pNode = pNode->NextSiblingElement(CAMERASWITCHPLAN_INFO_NODE);
			continue;
		}
		// ����ҵ�һ����TVWallID��CameraSwitchID,���������Ϣ
		std::string strTVWallID = std::string(pTVWallIDElem->GetText());
		std::string strTVWallIDForGet = CToolsHelp::Int2Str((IVS_INT32)ulTVWallID);
		std::string strCameraSwitchID = std::string(pCameraSwitchIDElem->GetText());
		std::string strCameraSwitchIDForGet = std::string(pCameraSwitchID);
		if (0 == strTVWallID.compare(strTVWallIDForGet) && 0 == strCameraSwitchID.compare(strCameraSwitchIDForGet))
		{
			// bFind = true;
			TiXmlPrinter xmlPrinter;
			xmlPrinter.SetStreamPrinting();		
			if (!pTVWallIDElem->Parent()->Accept(&xmlPrinter))
			{
				BP_RUN_LOG_ERR(IVS_FAIL, "Get TVWall CameraSwitchPlan Failed.", "Reason:print error.");
				return IVS_FAIL;//lint !e429
			}
			strTVwallCameraSwitchPlan = xmlPrinter.CStr();
			BP_DBG_LOG("TVWallCameraSwitchPlan = %s", strTVwallCameraSwitchPlan.c_str());
			return IVS_SUCCEED;
		}
		pNode = pNode->NextSiblingElement(CAMERASWITCHPLAN_INFO_NODE);
	}
	// û���ҵ�����ڵ㣬ֱ�ӷ���ʧ��
	//if (!bFind)
	//{
	//	BP_RUN_LOG_ERR(IVS_FAIL, "Get TVWall CameraSwitchPlan Failed.","Reason:TVwallID&CameraSwitchID not find.");
	//	return IVS_FAIL;
	//}
    BP_RUN_LOG_ERR(IVS_FAIL, "Get TVWall CameraSwitchPlan Failed.","Reason:TVwallID&CameraSwitchID not find.");
	return IVS_FAIL;
}

// ��ȡ��Ѳ�ƻ��б�
IVS_INT32 CFavorite::GetCameraSwitchPlanList(std::string& strCameraSwitchPlanList)
{
	// ����ļ��Ƿ����
	CLocalConfig localConfig;
	std::string strPath = localConfig.MakeSureLocalConfigFileExist(CAMERRA_SWITCH_PLAN_FINE_NAME);
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_OPEN_FILE_ERROR,"file not exist","NA");
		return IVS_OPEN_FILE_ERROR;
	}

	// ����XML�ļ�������ID �ҵ��ڵ㲢����
	TiXmlDocument pCameraSwitchXmlDoc;
	if (!pCameraSwitchXmlDoc.LoadFile(strPath.c_str(), TIXML_ENCODING_LEGACY))
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "load file failed","NA");
		return IVS_PARA_INVALID;
	}
	TiXmlElement* pCameraSwitchListElem = CToolsHelp::FindElementByPath(&pCameraSwitchXmlDoc,CAMERASWITCHLPLANIST_NODE);
	CHECK_POINTER(pCameraSwitchListElem, IVS_FAIL);

	// ����ɾ����CameraGroupList
	TiXmlElement* pNode = pCameraSwitchListElem->FirstChildElement(CAMERASWITCHPLAN_INFO_NODE);
	while(NULL != pNode)
	{
		// ɾ��CameraGroupList�ڵ�
		TiXmlElement* pCameraGroupListElem = CToolsHelp::FindElementByPath(pNode,"CameraGroupList");
		(void)pNode->RemoveChild(pCameraGroupListElem);
		pNode = pNode->NextSiblingElement(CAMERASWITCHPLAN_INFO_NODE);
	}

	TiXmlPrinter xmlPrinter;
	xmlPrinter.SetStreamPrinting();		

	if (!pCameraSwitchXmlDoc.Accept(&xmlPrinter))
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "print error", "NA");
		return IVS_FAIL;
	}

	strCameraSwitchPlanList  = xmlPrinter.CStr();
	BP_DBG_LOG("Get CameraSwitch List xml = %s", strCameraSwitchPlanList.c_str());
	return IVS_SUCCEED;
}

// ��ȡ����ǽ��Ѳ�ƻ��б�
IVS_INT32 CFavorite::GetCameraSwitchPlanListTVWall(std::string& strTVWallCameraSwitchPlanList)
{
	// ����ļ�TVWallCameraSwitchPlan.xml�Ƿ����
	CLocalConfig localConfig;
	std::string strPath = localConfig.MakeSureLocalConfigFileExist(CAMERRA_SWITCH_PLAN_TVWALL_FILE_NAME);
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_OPEN_FILE_ERROR,"Get TVWall CameraSwitchPlan List Failed.","Reason:File[TVWallCameraSwitchPlan.xml] not exist");
		return IVS_OPEN_FILE_ERROR;
	}

	// ����TVWallCameraSwitchPlan.xml�ļ�������ID �ҵ��ڵ㲢����
	TiXmlDocument pTVWallCameraSwitchXmlDoc;
	if (!pTVWallCameraSwitchXmlDoc.LoadFile(strPath.c_str(), TIXML_ENCODING_LEGACY))
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Get TVWall CameraSwitchPlan List Failed.","Reason:Load file[TVWallCameraSwitchPlan.xml] failed");
		return IVS_PARA_INVALID;
	}
	TiXmlElement* pTVWallCameraSwitchListElem = CToolsHelp::FindElementByPath(&pTVWallCameraSwitchXmlDoc,CAMERASWITCHLPLANIST_NODE);
	CHECK_POINTER(pTVWallCameraSwitchListElem, IVS_FAIL);

	/*
	// ����ɾ����CameraGroupList
	TiXmlElement* pNode = pTVWallCameraSwitchListElem->FirstChildElement(CAMERASWITCHPLAN_INFO_NODE);
	while(NULL != pNode)
	{
		// ɾ��CameraGroupList�ڵ�
		TiXmlElement* pCameraGroupListElem = CToolsHelp::FindElementByPath(pNode,"CameraGroupList");
		(void)pNode->RemoveChild(pCameraGroupListElem);
		pNode = pNode->NextSiblingElement(CAMERASWITCHPLAN_INFO_NODE);
	}
	*/

	TiXmlPrinter xmlPrinter;
	xmlPrinter.SetStreamPrinting();		

	if (!pTVWallCameraSwitchXmlDoc.Accept(&xmlPrinter))
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "Get TVWall CameraSwitchPlan List Failed.", "Reason:print error.");
		return IVS_FAIL;
	}

	strTVWallCameraSwitchPlanList  = xmlPrinter.CStr();
	BP_DBG_LOG("Get CameraSwitch List xml = %s", strTVWallCameraSwitchPlanList.c_str());
	return IVS_SUCCEED;
}

// ��ȡxml�е���Ѳ��ԴID
IVS_INT32 CFavorite::GetCameraSwitchID(const IVS_CHAR* pSwitchPlanInfo, std::string &pCameraSwitchID)
{
	CHECK_POINTER(pSwitchPlanInfo, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("");
	CXml xml;
	if (!xml.Parse(pSwitchPlanInfo))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID,  "Set Camera Backup Cfg", "xmlReq.Parse(pReqXml) fail");
		return IVS_XML_INVALID;
	}

	if (!xml.FindElemEx("Content/CameraSwitchPlan"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID,"Parse Result XML", "xml.FindElem(Content/PageInfo) is fail");
		return IVS_XML_INVALID;
	}   
	IVS_CHAR cCameraSwitchID[IVS_DEV_CODE_LEN + 1];
	const char* szElemValue = NULL;
	memset(cCameraSwitchID, 0, IVS_DEV_CODE_LEN+1);
	GET_ELEM_VALUE_CHAR("CameraSwitchID",szElemValue, cCameraSwitchID,IVS_DEV_CODE_LEN,xml);
	pCameraSwitchID = cCameraSwitchID;
	return IVS_SUCCEED;
}


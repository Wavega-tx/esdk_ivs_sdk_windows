#include "stdafx.h"
#include "LocalConfig.h"
#include "SDKConfig.h"
#include "ToolsHelp.h"
#include "zip.h"
#include "unzip.h"
#include "IVS_SDK.h"

std::string CLocalConfig::m_strLocalLoginName;
std::string CLocalConfig::m_strLocalstrIP;

CLocalConfig::CLocalConfig()
	: m_iSessionID(0)
	, m_bBackUserData(false)
{
	m_pMutexLocalConfigFileLock = VOS_CreateMutex();
}
CLocalConfig::~CLocalConfig()
{
	m_iSessionID = 0;
	m_bBackUserData = false;
	VOS_DestroyMutex(m_pMutexLocalConfigFileLock);
	m_pMutexLocalConfigFileLock = NULL;
}

// ��鱾���ļ��Ƿ���ڣ���������ھ͸���ģ������һ��,�����ļ������app data/ivs/ip/username/localconfig.xml
std::string CLocalConfig::MakeSureLocalConfigFileExist(const char* pFileName) const
{
	(void)VOS_MutexLock(m_pMutexLocalConfigFileLock);

	// ���Ŀ¼�Ƿ����
	std::string strPath = MakeSureLocalConfigDirExist();
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_FAIL,"create dir failed","NA");
		(void)VOS_MutexUnlock(m_pMutexLocalConfigFileLock);
		return "";
	}

	// �ж��ļ��Ƿ���ڣ������ھʹ���
	strPath = strPath + pFileName;
	if(_access(strPath.c_str(),0) != 0)
	{
		bool bRet = false;
		
		if (!strcmp(pFileName, LOCAL_CONFING_FILE_NAME))
		{
			bRet = CreateLocalConfigXML(strPath.c_str());
		}
		else if (!strcmp(pFileName, DEVICE_KEYWORD_FILE_NAME))
		{
			bRet = CreateDeviceKeyWordXML(strPath.c_str());
		}
		else if (!strcmp(pFileName, CUSTOM_LAYOUT_CONFIG_FILE_NAME))
		{
			bRet = CreateCustomLayoutXML(strPath.c_str());
		}
        else if (!strcmp(pFileName, FAVORITE_CONFIG_FILE_NAME))
        {
			bRet = CreateFavoriteConfigXML(strPath.c_str());
        }
		else if (!strcmp(pFileName, SCENARIO_FAVORITE_FILE_NAME))
		{
			bRet = CreateScenarioFavoriteXML(strPath.c_str());
		}
		else if (!strcmp(pFileName,TVWALL_SCENARIO_FAVORITE_FILE_NAME))
		{
			bRet = CreateTVWallScenarioFavoriteXML(strPath.c_str());
		}
		else if (!strcmp(pFileName, CAMERA_SWITCH_FILE_NAME))
		{
			bRet = CreateCameraSwitchXML(strPath.c_str());
		}
		else if (!strcmp(pFileName, CAMERA_ALIAS_FILE_NAME))
		{
			bRet = CreateCameraAliasXML(strPath.c_str());
		}
		else if (!strcmp(pFileName, CAMERRA_SWITCH_PLAN_FINE_NAME))
		{
			bRet = CreateCameraSwitchPlanXML(strPath.c_str());
		}
		else if (!strcmp(pFileName, CAMERRA_SWITCH_PLAN_TVWALL_FILE_NAME))
		{
			bRet = CreateCameraSwitchPlanXMLTVWall(strPath.c_str());
		}
        else if (!strcmp(pFileName, NET_KEYBOARD_INFO_FILE_NAME))
        {
            bRet = CreateNetKeyBoardInfoXML(strPath.c_str());
        }
        else if (!strcmp(pFileName, BROADCAST_FILELIST_FILE_NAME))
        {
            bRet = CreateBroadcastFileListXML(strPath.c_str());
        }
		else if (!strcmp(pFileName, BROADCAST_CAMERALIST_CAMERA_CODE))
		{
			bRet = CreateBroadcastCameraListXML(strPath.c_str());
		}
		else
		{
			// ����ģ��
		}

		if (!bRet)
		{
			BP_RUN_LOG_ERR(IVS_FAIL,"create file failed","file name =%s", pFileName);
			(void)VOS_MutexUnlock(m_pMutexLocalConfigFileLock);
			return "";
		}
	}
	(void)VOS_MutexUnlock(m_pMutexLocalConfigFileLock);

	return strPath; //lint !e1036
}

// ��鱾�����õ��ļ��У�·���� local data/IVS/IP/loginname����������ھʹ������
std::string CLocalConfig::MakeSureLocalConfigDirExist() const
{
	// ��ȡappdatĿ¼,��������ھʹ���
	char szPath[MAX_PATH]; 
	memset(szPath,0x0,MAX_PATH);
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, TRUE); 
	// �ж�ָ����·���Ƿ���ڣ������ھʹ����ļ���
	std::string strPath = std::string(szPath) + DIR_SEPARATOR + SOFTWARE_NAME + DIR_SEPARATOR + CLocalConfig::m_strLocalstrIP + DIR_SEPARATOR + CLocalConfig::m_strLocalLoginName + DIR_SEPARATOR; //lint !e1036
	if (!MakeSureDirectoryPathExists(strPath.c_str()))
	{
		strPath = "";
	}
	
	return strPath; //lint !e1036
}

// ��鱾���ļ��Ƿ���ڣ������ڷ��ؿ�·�������ڷ����ļ�·��
std::string CLocalConfig::CheckLocalConfigFileExist(const char* pFileName) const
{
	// ��ȡappdatĿ¼
	char szPath[MAX_PATH]; 
	memset(szPath,0x0,MAX_PATH);
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, FALSE);
	std::string strPath = std::string(szPath) + DIR_SEPARATOR + SOFTWARE_NAME + DIR_SEPARATOR + CLocalConfig::m_strLocalstrIP + DIR_SEPARATOR + CLocalConfig::m_strLocalLoginName + DIR_SEPARATOR; //lint !e1036
	strPath += pFileName;
	// ����ļ��Ƿ���ڣ������ھͷ��ؿ�
	if(_access(strPath.c_str(),0) != 0)
	{
		strPath = "";
	}
	return strPath; //lint !e1036
}

// ����localconfig.xml��ģ���ļ�
bool CLocalConfig::CreateLocalConfigXML(const char* pFilePath) const
{
	std::string strXml = LOCAL_CONFIG_XML_TEMPLATE;
	return CreateXML(pFilePath,strXml.c_str());
}

// ����devicekeyword.xml��ģ���ļ�
bool CLocalConfig::CreateDeviceKeyWordXML(const char* pFilePath) const
{
	std::string strXml = DEVICE_KEYWORD_XML_TEMPLATE;
	return CreateXML(pFilePath,strXml.c_str());
}

// ����customlayout.xml��ģ���ļ�
bool CLocalConfig::CreateCustomLayoutXML(const char* pFilePath) const
{
	std::string strXml = CUSTOM_LAYOUT_CONFIG_XML_TEMPLATE;
	return CreateXML(pFilePath,strXml.c_str());
}

// ����FavoriteConfig.xml��ģ��
bool CLocalConfig::CreateFavoriteConfigXML(const char* pFilePath) const
{
    std::string strXml = FAVORITE_CONFIG_XML_TEMPLATE;
    return CreateXML(pFilePath,strXml.c_str());
}

// ����FavoriteConfig.xml��ģ��
bool CLocalConfig::CreateScenarioFavoriteXML(const char* pFilePath) const
{
	std::string strXml = SCENARIO_FAVORITE_XML_TEMPLATE;
	return CreateXML(pFilePath,strXml.c_str());
}

// ����TVWallFavoriteConfig.xml��ģ��
bool CLocalConfig::CreateTVWallScenarioFavoriteXML(const char* pFilePath) const
{
	std::string strXml = SCENARIO_FAVORITE_XML_TEMPLATE;
	return CreateXML(pFilePath,strXml.c_str());
}

// ����CameraSwitch.xml��ģ��
bool CLocalConfig::CreateCameraSwitchXML(const char* pFilePath) const
{
	std::string strXml = CAMERA_SWITCH_XML_TEMPLATE;
	return CreateXML(pFilePath,strXml.c_str());
}

// ����CameraSwitch.xml��ģ��
bool CLocalConfig::CreateCameraAliasXML(const char* pFilePath) const
{
	std::string strXml = CAMERA_ALIAS_XML_TEMPLATE;
	return CreateXML(pFilePath,strXml.c_str());
}

bool CLocalConfig::CreateCameraSwitchPlanXML(const char* pFilePath) const
{
	std::string strXml = CAMERA_SWITCH_PLAN_XML_TEMPLATE;
	return CreateXML(pFilePath,strXml.c_str());
}

bool CLocalConfig::CreateCameraSwitchPlanXMLTVWall(const char* pFilePath) const
{
	std::string strXml = CAMERA_SWITCH_PLAN_TVWALL_XML_TEMPLATE;
	return CreateXML(pFilePath,strXml.c_str());
}

// �������������Ϣ��ģ��
bool CLocalConfig::CreateNetKeyBoardInfoXML(const char* pFilePath) const
{
    std::string strXml = NET_KEYBOARD_INFO_XML_TEMPLATE;
    return CreateXML(pFilePath,strXml.c_str());
}

// �����㲥�ļ��б��ģ��
bool CLocalConfig::CreateBroadcastFileListXML(const char* pFilePath) const
{
    std::string strXml = BROADCAST_FILELIST_XML_TEMPLATE;
    return CreateXML(pFilePath,strXml.c_str());
}

// ���������㲥������б��ģ��
bool CLocalConfig::CreateBroadcastCameraListXML(const char* pFilePath) const
{
	std::string strXml = BROADCAST_CAMERALIST_XML_TEMPLATE;
	return CreateXML(pFilePath,strXml.c_str());
}

// ��ָ��·������һ��XML(����)
bool CLocalConfig::CreateXML(const char* pFilePath, const char* pStream) const
{
	bool bRet = false;
	do 
	{
		TiXmlDocument pDoc;
		pDoc.Parse(pStream);
		//pDoc.Parse(pStream,NULL, TIXML_ENCODING_LEGACY)
		if (!pDoc.SaveFile(pFilePath))
		{
			BP_RUN_LOG_ERR(IVS_FAIL,"SaveFile failed, file path is:","%s", pFilePath);
			bRet = false;
			break;
		}
		bRet = true;

	} while (0); //lint !e717

	return bRet;
}

void CLocalConfig::SetLoginName(const std::string& strLoginName)
{
	CLocalConfig::m_strLocalLoginName = strLoginName;
}

void CLocalConfig::SetIP(const std::string& strIP)
{
	CLocalConfig::m_strLocalstrIP = strIP;
}

// ��ȡ��־�����ļ���Ŀ¼
IVS_INT32 CLocalConfig::GetConfigDir(std::string& dir)
{
	int nRet = 0;
	char buf[1024] = {0};
	nRet = (int)GetModuleFileName(NULL, buf, sizeof(buf) - 1);
	if(0 == nRet)
	{
		return -1;
	}

	const char separator = '\\';

	char *pch = NULL;
	//ȥ��������
	for(int i = 0; i < 2; ++i) //ѭ��2��: ��1���ҵ���������Ŀ¼����2���ҵ���������Ŀ¼�ĸ�Ŀ¼
	{
		pch = strrchr(buf, separator);
		if((NULL == pch) || (pch == buf))
		{
			return -1;
		}
		*pch = '\0';
		pch = NULL;
	}

	dir = buf;
	dir += "\\config";
	return 0;
}

bool CLocalConfig::SetCDATA2XML(const char* pFileName, const char* pElement, const char* pValue) const
{
	// ��鱾�����õ��ļ��Ƿ���ڣ������ھʹ����ļ���������·��
	std::string strPath = MakeSureLocalConfigFileExist(pFileName);
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_FAIL,"create file failed","NA");
		return false;
	}

	// ��ȡXML���������
	CXml xml;
	if (!xml.Load(strPath.c_str(), TIXML_ENCODING_LEGACY))
	{
		// �������ʧ�ܣ�������ģ���������XML
		if (!CreateLocalConfigXML(strPath.c_str()))
		{
			BP_RUN_LOG_ERR(IVS_FAIL,"create xml failed","NA");
			return false;
		}
		// �����ʧ�ܡ���
		if (!xml.Load(strPath.c_str(), TIXML_ENCODING_LEGACY))
		{
			BP_RUN_LOG_ERR(IVS_FAIL,"load xml failed","NA");
			return false;
		}
	}
    if (NULL == pElement)
    {
        (void)xml.Parse(pValue);
    }
	else if (!xml.SetElemValueCDATAEx(pElement,pValue))
	{
		BP_RUN_LOG_ERR(IVS_FAIL,"set elem value failed","NA");
		return false;
	}

	if (!xml.Save())
	{
		BP_RUN_LOG_ERR(IVS_FAIL,"save xml failed","NA");
		return false;
	}
	
	return true;
}

std::string CLocalConfig::GetCDATA2XML(const char* pFileName, const char* pElement) const
{
	std::string strResult;
	do 
	{
		// ��ȡappdatĿ¼
		std::string strPath = CheckLocalConfigFileExist(pFileName);
		if (strPath.empty())
		{
			BP_RUN_LOG_ERR(IVS_FAIL,"file not exist","NA");
			strResult = "";
			break;
		}

		CXml xml;
		if (!xml.Load(strPath.c_str(), TIXML_ENCODING_LEGACY))
		{
			BP_RUN_LOG_ERR(IVS_FAIL,"load file failed","NA");
			strResult = "";
			break;
		}
        if (NULL == pElement)
        {
            IVS_UINT32 uiLen = 0;
            strResult = xml.GetXMLStream(uiLen);
        }
        else
        {
			const char* pV = xml.GetElemValueEx(pElement);
			if (NULL != pV)
			{
				strResult = pV;
			}
        }
	} while (0); //lint !e717
	
	return strResult; //lint !e1036
}

// ���������ݵ�XMLת���ɿɻ�ͼ�ĵ�,xml���ʽ <Coord>0,0,1,1</Coord>
IVS_INT32 CLocalConfig::ParseLayoutXML2List(const char* layoutXML, std::list<CRect> &liRectList, IVS_UINT32& xNum, IVS_UINT32& yNum) const
{
	int iRet = IVS_SUCCEED;
	do 
	{
		CHECK_POINTER(layoutXML, IVS_FAIL);
		// ����XML
		CXml xml;
		if (!xml.Parse(layoutXML))
		{
			BP_RUN_LOG_ERR(IVS_FAIL,"parse xml failed","NA");
			iRet = IVS_FAIL;
			break;
		}

		if (!xml.FindElemEx("CONTENT/WndList"))
		{
			BP_RUN_LOG_ERR(IVS_FAIL,"find element failed","NA");
			iRet = IVS_FAIL;
			break;
		}
		// �ж���ľ������ڵ㣬û�о��ǲ���ûֵ
		if (!xml.FindElem("Coord"))
		{
			BP_RUN_LOG_ERR(IVS_FAIL,"no coord","NA");
			return IVS_FAIL;
		}
		xNum = 0;
        yNum = 0;
		const char* strCoord = NULL;
		do 
		{
			CRect rect;
			strCoord = xml.GetElemValue();
			CHECK_POINTER(strCoord, IVS_FAIL);
			// �����ĴΣ���ȡ���꣬������������ֱ�ӷ���ʧ��
			char* pNext = NULL;
			char* token = strtok_s(const_cast<char*>(strCoord), ",", &pNext);
			CHECK_POINTER(token, IVS_FAIL);
			rect.left = atoi(token);

			token = strtok_s(NULL, ",", &pNext);
			CHECK_POINTER(token, IVS_FAIL);
			rect.top = atoi(token);

			token = strtok_s(NULL, ",", &pNext);
			CHECK_POINTER(token, IVS_FAIL);
			rect.right = atoi(token);

			token = strtok_s(NULL, ",", &pNext);
			CHECK_POINTER(token, IVS_FAIL);
			rect.bottom = atoi(token);

			liRectList.push_back(rect);

            if (rect.right > static_cast<IVS_LONG>(xNum))
            {
                xNum = static_cast<IVS_UINT32>(rect.right);
            }

            if (rect.bottom > static_cast<IVS_LONG>(yNum))
            {
                yNum = static_cast<IVS_UINT32>(rect.bottom);
            }
		} while (xml.NextElem());
	} while (0); //lint !e717

	return iRet;
}

// ��ȡ�����ļ�����ȡָ���Զ��岼�ֵ�XML
IVS_UINT32 CLocalConfig::GetLayoutXML( unsigned long ulLayoutID, std::string &strlayoutXML ) const
{
	// �ж��ļ��Ƿ���ڣ����ھͷ���·��
	std::string strPath = CheckLocalConfigFileExist(CUSTOM_LAYOUT_CONFIG_FILE_NAME);
	if (strPath.empty())
	{
		BP_RUN_LOG_ERR(IVS_OPEN_FILE_ERROR,"file not exist","NA");
		return IVS_OPEN_FILE_ERROR;
	}

	// ����XML�ļ�
	TiXmlDocument* pXmlDoc = IVS_NEW(pXmlDoc);
	CHECK_POINTER(pXmlDoc, IVS_ALLOC_MEMORY_ERROR);
	if (!pXmlDoc->LoadFile(strPath.c_str(), TIXML_ENCODING_LEGACY))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "IVS OCX Get LayoutXML Failed.","Reason:Create LayoutXML xml Failed.");
		return IVS_XML_INVALID;
	}
	
	TiXmlElement* pConnListElem = CToolsHelp::FindElementByPath(pXmlDoc,CUSTOM_LAYOUT_LIST_ELEMENT);
	CHECK_POINTER(pConnListElem, IVS_XML_INVALID);
	// �����Ǵ�������ڵ�����
	TiXmlElement* pNode = pConnListElem->FirstChildElement(CUSTOM_LAYOUT_NODE);
	
	const char* pCode = NULL;
	while(NULL != pNode)
	{
		pCode = pNode->Attribute(LAYOUT_ATTRIBUTE);
		CHECK_POINTER(pCode, IVS_XML_INVALID);
		// ����ҵ�һ����code
		if (ulLayoutID == static_cast<unsigned int>(atoi(pCode)))
		{
            strlayoutXML.clear();
            strlayoutXML = pNode->GetText();
			IVS_DELETE(pXmlDoc);
			return IVS_SUCCEED;
		}
		pNode = pNode->NextSiblingElement(CUSTOM_LAYOUT_NODE);
	}

	// �ͷŵ�doc,��������Ҳ���ͷ���
	IVS_DELETE(pXmlDoc);
	return IVS_FAIL;
}

// ѹ���ļ����е��������ݵ�ָ��Ŀ¼��
IVS_INT32 CLocalConfig::CompressionDir(const std::string& zipFileName, const std::string& strInPath, const std::string& strOutPath)
{
	std::string strSerachPatten=strInPath + "\\" +"*.*";

	// ����zip��
	std::string strZipPath = strOutPath;
	strZipPath += "\\";
	strZipPath += zipFileName;

	HZIP hz = CreateZip(strZipPath.c_str(),0);
	// �����ļ�������������ļ������ļ����뵽zip��
	struct _finddata_t fileinfo;
	long handle = _findfirst(strSerachPatten.c_str(),&fileinfo);
	if(handle == -1L)
	{
		BP_RUN_LOG_ERR(IVS_FAIL,"path error","NA");
		return IVS_FAIL;
	}
	// ������� .����..
	std::string filePath = strInPath + "\\" + fileinfo.name;
	if (!(!strcmp(fileinfo.name,".") || !strcmp(fileinfo.name,"..")))
	{
		ZipAdd(hz, fileinfo.name, filePath.c_str());
	}

	while(!_findnext(handle,&fileinfo))
	{
		// ������� .����..
		//modify by s00193168:2013-6-28����������ǽ���ܣ��ӳ�������Ѳ���˹�����Ҫ��TVWallScenarioFavorite.xml���ٴ���ϴ����������� 
		if (!(!strcmp(fileinfo.name,".") || !strcmp(fileinfo.name,"..")) && strcmp(fileinfo.name,"TVWallScenarioFavorite.xml"))
		{
			filePath = strInPath + "\\" + fileinfo.name;
			ZipAdd(hz, fileinfo.name, filePath.c_str());
		}
	}

	_findclose(handle);

	CloseZip(hz);
	return IVS_SUCCEED;
}

// ��ѹһ��ѹ���ļ���һ��ָ��Ŀ¼
IVS_INT32 CLocalConfig::DecompressionDir(const std::string& zipFileName,const std::string& zipFilePath, const std::string& strOutPath)
{
	std::string zipFile = zipFilePath;
	//zipFile += "\\";
	zipFile += zipFileName;

	HZIP hz = OpenZip(zipFile.c_str(),0);
	ZIPENTRY ze; 
	GetZipItem(hz, -1, &ze); 
	int numitems = ze.index;
	for (int i=0; i < numitems; i++)
	{ 
		GetZipItem(hz,i,&ze);
		UnzipItem(hz,i,ze.name);
		// �ƶ��ļ���ָ��Ŀ¼,������ھ͸��ǵ��������ѹ�������ļ���Ĭ���ڳ���·��

		std::string moveSrcFile = std::string(".\\") + ze.name;
		std::string moveDstFile = strOutPath + "\\" + ze.name;
		MoveFileEx(moveSrcFile.c_str(),moveDstFile.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED); //add by xiongfeng �ƶ�����ͬ�ľ������ļ���ɾ��ԭ�����ļ�
	}
	CloseZip(hz);
	return IVS_SUCCEED;
}

// �ļ��Զ��ָ��Ϳ������ݶ�ʱ��
IVS_INT32 CLocalConfig::StartUserData(IVS_INT32 iSessionID, std::string &strLocalLoginName, std::string &strLocalstrIP)
{
	//������ʱ��
	StartLocalConfigTimer();

	m_strLocalLoginName = strLocalLoginName;
	m_strLocalstrIP = strLocalstrIP;
	m_iSessionID = iSessionID;

 	//��ȡ�ļ�·��
	m_strFileNamePath = MakeSureLocalConfigDirExist();

 	//����sdk�ָ��ӿ�
 	std::string strName; 
	strName.append(strLocalstrIP).append("_").append(strLocalLoginName).append(".zip");
 	std::string strPathAndName = m_strFileNamePath + strName; //lint !e1036

    BP_RUN_LOG_INF("Start UserData", "m_strLocalLoginName: %s,m_LocalstrIP: %s, m_iSessionID: %d, m_strFileNamePath: %s, strPathAndName: %s", m_strLocalLoginName.c_str(),m_strLocalstrIP.c_str(),m_iSessionID,m_strFileNamePath.c_str(),strPathAndName.c_str());
	IVS_INT32 iRet = IVS_SDK_GetUserData(m_iSessionID, strPathAndName.c_str());
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "get user data fail", "NA");
		return iRet;
	}

	//��ѹ���ļ�
	iRet = DecompressionDir(strName, m_strFileNamePath, m_strFileNamePath);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "get user data fail", "NA");
	}

	//ɾ��ѹ���ļ�
	if (!DeleteFile(strPathAndName.c_str()))
	{
		BP_RUN_LOG_ERR(IVS_SDK_ERR_DELETEZIPFILE_FAILED, "onTrigger", "delete user zip file fail");
	}

	return iRet;
} //lint !e451 !e1764

// �����Զ����ݶ�ʱ��
void CLocalConfig::StartLocalConfigTimer(void)
{
	m_bBackUserData = true; // ��ʱ��������־
	m_LocalConfigTimerInst.init(100);
	m_LocalConfigTimerInst.run();
    // �����ļ��е�λΪ���ӣ�����60 * 10 * value ��λΪ��
    IVS_UINT32 uiBackUpInterval = 600 * CSDKConfig::instance().GetBackUpInterval();
	try
	{
 		m_LocalConfigTimerInst.registerTimer(this , NULL, uiBackUpInterval, enRepeated);
	}
	catch (...)
	{
		return;
	}
}

// ֹͣ�Զ����ݶ�ʱ��
void CLocalConfig::StopLocalConfigTimer(void)
{
	if (m_bBackUserData)
	{
		m_bBackUserData = false;
		m_LocalConfigTimerInst.cancelTimer(this);
		m_LocalConfigTimerInst.clearTimer();
		m_LocalConfigTimerInst.exit();
	}
}

// ��ʱ�������������
void CLocalConfig::onTrigger(void* pArg, ULONGLONG ullScales, TriggerStyle style)
{
	(void)BackUserData();
} //lint !e715

// �����ض�Ŀ¼�µ��ļ�
IVS_INT32 CLocalConfig::BackUserData(void)
{
    // û�������ݹ��ܣ�����Ҫ�����û�����
    if (!m_bBackUserData)
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "Back UserData", "m_bBackUserData is false, needn't backUserData");
        return IVS_FAIL;
    }
	std::string strName;
	strName.append(CLocalConfig::m_strLocalstrIP).append("_").append(CLocalConfig::m_strLocalLoginName).append(".zip");
	std::string strPathAndName;
	strPathAndName.append(m_strFileNamePath).append(strName);

    BP_RUN_LOG_INF("Back UserData", "strName: %s,m_strFileNamePath: %s, strPathAndName: %s", strName.c_str(),m_strFileNamePath.c_str(),strPathAndName.c_str());
	IVS_INT32 iRet = CompressionDir(strName, m_strFileNamePath, m_strFileNamePath);

	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(IVS_SDK_ERR_CREATEZIPFILE_FAILED, "CLocalConfig::onTrigger", "CompressionDir failed");
		return iRet;
	}

	iRet = IVS_SDK_SetUserData(m_iSessionID ,strPathAndName.c_str());

	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet,"onTrigger", "IVS_SDK_SetUserData fail. iRet = %d", iRet);
	}

	//��ѹ�ļ���
	//(void)DecompressionDir(strName, m_strFileNamePath, m_strFileNamePath); 

	//ɾ���ļ�
	if (!DeleteFile(strPathAndName.c_str()))
	{
		BP_RUN_LOG_ERR(IVS_SDK_ERR_DELETEZIPFILE_FAILED, "onTrigger", "delete user zip file fail");
	}	
	return iRet;
}

#ifndef __LOCAL_CONFIG_H__
#define __LOCAL_CONFIG_H__

#include "SDKTimer.h"
#include "io.h"
#include <fstream>
#include <ImageHlp.h>
#pragma comment (lib, "ImageHlp.lib")
#include "SDKDef.h"

#define SOFTWARE_NAME "IVS"
#define LOCAL_CONFING_FILE_NAME "LocalConfig.xml"
#define DEVICE_KEYWORD_FILE_NAME "DeviceKeyWord.xml"
#define CUSTOM_LAYOUT_CONFIG_FILE_NAME "CustomLayoutConfig.xml"
#define FAVORITE_CONFIG_FILE_NAME "FavoriteConfig.xml"
#define SCENARIO_FAVORITE_FILE_NAME "ScenarioFavorite.xml"
#define TVWALL_SCENARIO_FAVORITE_FILE_NAME "TVWallScenarioFavorite.xml"
#define CAMERA_SWITCH_FILE_NAME "CameraSwitch.xml"
#define CAMERA_ALIAS_FILE_NAME "CameraAlias.xml"
#define DEFAULT_LOCALCONFIG_PATH "\\defaultConfig\\LocalConfig.xml"
#define CAMERRA_SWITCH_PLAN_FINE_NAME "CameraSwitchPlan.xml"
#define CAMERRA_SWITCH_PLAN_TVWALL_FILE_NAME "TVWallCameraSwitchPlan.xml"
#define I18N_DIR "\\i18n\\"
#define NET_KEYBOARD_INFO_FILE_NAME "NetKeyBoardInfo.xml"
#define BROADCAST_FILELIST_FILE_NAME "BroadcastFileList.xml"
#define BROADCAST_CAMERALIST_CAMERA_CODE "BroadcastCameraList.xml"

#define DIR_SEPARATOR "\\"
#define DEVCODE_ATTRIBUTE "devcode"
#define LAYOUT_ATTRIBUTE "layoutID"

#define CONNECT_MOD_NODE "ConnectMod"

#define DEVICE_KEYWORDD_NODE "DeviceKeyWord"
#define DEVICE_ALIAS_NODE "CameraAlias"

#define CUSTOM_LAYOUT_NODE "CustomLayout"

#define SCENARIOLIST_NODE "Content/ScenarioList"

#define CAMERASWITCHLIST_NODE "Content/CameraSwitchList"

#define  CAMERASWITCHLPLANIST_NODE "Content/CameraSwitchPlanList"
//#define SCENARIO_INFO_NODE "ScenarioInfo"

// �������õĽڵ㣨ֻ���ڴ�XML��
#define ALARM_CONFIG_ELEMENT    "LocalConfig/AlarmConfigInfo"
#define RECORD_CONFIG_ELEMENT   "LocalConfig/RecordConfigInfo"
#define CAPTURE_CONFIG_ELEMENT  "LocalConfig/CaptureConfigInfo"
#define NETWORK_CONFIG_ELEMENT  "LocalConfig/NetWorkConfig"
#define PTZ_CONFIG_ELEMENT  "LocalConfig/PTZConfig"
#define RECORD_SEARCH_INFO_ELEMENT  "LocalConfig/RecordSearchInfo"
#define CONNECT_MOD_LIST_ELEMENT     "LocalConfig/ConnectModList"
#define DEVICE_KEYWORD_ELEMENT     "LocalConfig/DeviceKeyWordList"
#define DEVICE_CAMERAALIAS_ELEMENT     "LocalConfig/CameraAliasList"
#define CUSTOM_LAYOUT_LIST_ELEMENT     "CustomLayoutConfig/CustomLayoutList"
#define NET_KEYBORAD_LIST_ELEMENT     "Content/NetKeyBoardInfoList"
#define BROADCAST_FILELIST_ELEMENT    "Content/BroadcastFileList"
#define BROADCAST_CAMERALIST_ELEMENT    "Content/BroadcastList"

// DeviceKeyWord.xml��ģ��
#define DEVICE_KEYWORD_XML_TEMPLATE "<?xml version='1.0' encoding='utf-8' ?>                  \
<LocalConfig>                                             \
<DeviceKeyWordList></DeviceKeyWordList>                   \
</LocalConfig>"

// LocalConfig.xml��ģ��
#define LOCAL_CONFIG_XML_TEMPLATE "<?xml version='1.0' encoding='utf-8' ?>                  \
<LocalConfig>                                             \
<AlarmConfigInfo></AlarmConfigInfo>                       \
<RecordConfigInfo></RecordConfigInfo>                     \
<CaptureConfigInfo></CaptureConfigInfo>                   \
<NetWorkConfig></NetWorkConfig>                           \
<PTZConfig></PTZConfig>                                   \
<ConnectModList></ConnectModList>                                 \
<ScenarioRecovery>0</ScenarioRecovery>\
</LocalConfig>"

// CustomLayoutConfig.xml��ģ��
#define CUSTOM_LAYOUT_CONFIG_XML_TEMPLATE "<?xml version='1.0' encoding='utf-8' ?>                  \
<CustomLayoutConfig>                                             \
<CustomLayoutList></CustomLayoutList>                            \
</CustomLayoutConfig>"

// FavoriteConfig.xml��ģ��
#define FAVORITE_CONFIG_XML_TEMPLATE "<?xml version='1.0' encoding='utf-8' ?>                  \
<Content>                                             \
<FavoriteFolderList></FavoriteFolderList>                       \
<DeviceFavoriteList></DeviceFavoriteList>                     \
</Content>"

// ScenarioFavorite.xml��ģ��
#define SCENARIO_FAVORITE_XML_TEMPLATE "<?xml version='1.0' encoding='utf-8' ?>                  \
<Content>                                             \
<ScenarioList></ScenarioList>                       \
</Content>"

// CameraSwitch.xml��ģ��
#define CAMERA_SWITCH_XML_TEMPLATE "<?xml version='1.0' encoding='utf-8' ?>                  \
<Content>                                             \
<CameraSwitchList></CameraSwitchList>                       \
</Content>"

// CameraAlias.xml��ģ��
#define CAMERA_ALIAS_XML_TEMPLATE "<?xml version='1.0' encoding='utf-8' ?>                  \
<LocalConfig>                                             \
<CameraAliasList></CameraAliasList>                       \
</LocalConfig>"

// CameraSwitchPlan.xml
#define CAMERA_SWITCH_PLAN_XML_TEMPLATE "<?xml version='1.0' encoding='utf-8' ?>                  \
<Content>                                             \
<CameraSwitchPlanList></CameraSwitchPlanList>                       \
</Content>"

// CameraSwitchPlanTVWall.xml
#define CAMERA_SWITCH_PLAN_TVWALL_XML_TEMPLATE "<?xml version='1.0' encoding='utf-8' ?>                  \
<Content>                                             \
<CameraSwitchPlanList></CameraSwitchPlanList>                       \
</Content>"

#define NET_KEYBOARD_INFO_XML_TEMPLATE "<?xml version='1.0' encoding='utf-8' ?>                  \
<Content>                                             \
<NetboardCameraList></NetboardCameraList>                       \
</Content>"

// BroadcastFileList.xml��ģ��
#define BROADCAST_FILELIST_XML_TEMPLATE "<?xml version='1.0' encoding='utf-8' ?>                  \
<Content>                                             \
<BroadcastFileList></BroadcastFileList>                       \
</Content>"

// BroadcastCameraList.xml��ģ��
#define BROADCAST_CAMERALIST_XML_TEMPLATE "<?xml version='1.0' encoding='utf-8' ?>                  \
<Content>                                             \
<BroadcastList></BroadcastList>                       \
</Content>"



class CLocalConfig : public ITrigger
{
public:
	CLocalConfig();
	~CLocalConfig();
	/**********************************************************************
	* name			: SetCDATA2XML
	* description	: ��һ��xml�Ľڵ��в���һ��CDATA����,
					  �ļ�Ĭ��·����app data/ivs/ip/username/
	* input			: pFileName �ļ�����Ĭ��·��app data/ivs/ip/username/
					  pElement  �ڵ�����
					  pValue    �ڵ�ֵ
	* output     	: NA
	* return     	: true - ���óɹ� false - ����ʧ��
	* remark     	: NA   
	*********************************************************************/
	bool SetCDATA2XML(const char* pFileName, const char* pElement, const char* pValue) const;

	/**********************************************************************
	* name			: SetCDATA2XML
	* description	: ��ȡһ��CDATA����,�ļ�Ĭ��·����app data/ivs/ip/username/
	* input			: pFileName �ļ�����Ĭ��·��app data/ivs/ip/username/
					  pElement  �ڵ�����
	* output     	: NA
	* return     	: std::string - �ڵ��е�����
	* remark     	: NA   
	*********************************************************************/
	std::string GetCDATA2XML(const char* pFileName, const char* pElement) const;

	/**********************************************************************
	* name			: MakeSureLocalConfigDirExist
	* description	: ��鱾�����õ��ļ��У�Ĭ��·����app data/ivs/ip/username/
	* input			: NA
	* output     	: NA
	* return     	: std::string - �ļ��е�Ŀ¼
	* remark     	: NA   
	*********************************************************************/
	std::string MakeSureLocalConfigDirExist() const;

	/**********************************************************************
	* name			: MakeSureLocalConfigFileExist
	* description	: ��鱾���ļ��Ƿ���ڣ���������ھ͸���ģ������һ��,
					  �����ļ������app data/ivs/ip/username/localconfig.xml
	* input			: pFileName �ļ�����Ĭ��·��app data/ivs/ip/username/
	* output     	: NA
	* return     	: std::string - �ļ��е�·��
	* remark     	: NA   
	*********************************************************************/
	std::string MakeSureLocalConfigFileExist(const char* pFileName) const;

	/**********************************************************************
	* name			: CheckLocalConfigFileExist
	* description	: ��鱾���ļ��Ƿ���ڣ������ڲ����������ؿ�·��
	* input			: pFileName �ļ�����Ĭ��·��app data/ivs/ip/username/
	* output     	: NA
	* return     	: std::string - �ļ��е�·��
	* remark     	: NA   
	*********************************************************************/
	std::string CheckLocalConfigFileExist(const char* pFileName) const;

	/**********************************************************************
	* name			: CreateLocalConfigXML
	* description	: ����localConfig.xml��ģ��
	* input			: pFilePath �ļ�·��
	* output     	: NA
	* return     	: bool - �Ƿ񴴽��ɹ�
	* remark     	: NA   
	*********************************************************************/
	bool CreateLocalConfigXML(const char* pFilePath) const;

	/**********************************************************************
	* name			: CreateDeviceKeyWordXML
	* description	: ����DeviceKeyWord.xml��ģ��
	* input			: pFilePath �ļ�·��
	* output     	: NA
	* return     	: bool - �Ƿ񴴽��ɹ�
	* remark     	: NA   
	*********************************************************************/
	bool CreateDeviceKeyWordXML(const char* pFilePath) const;

	/**********************************************************************
	* name			: CreateCustomLayoutXML
	* description	: ����CustomLayoutConfig.xml��ģ��
	* input			: pFilePath �ļ�·��
	* output     	: NA
	* return     	: bool - �Ƿ񴴽��ɹ�
	* remark     	: NA   
	*********************************************************************/
	bool CreateCustomLayoutXML(const char* pFilePath) const;

	/**********************************************************************
	* name			: CreateFavoriteConfigXML
	* description	: ����FavoriteConfig.xml��ģ��
	* input			: pFilePath �ļ�·��
	* output     	: NA
	* return     	: bool - �Ƿ񴴽��ɹ�
	* remark     	: NA   
	*********************************************************************/
	bool CreateFavoriteConfigXML(const char* pFilePath) const;

	/**********************************************************************
	* name			: CreateScenarioFavoriteXML
	* description	: ����ScenarioFavorite.xml��ģ��
	* input			: pFilePath �ļ�·��
	* output     	: NA
	* return     	: bool - �Ƿ񴴽��ɹ�
	* remark     	: NA   
	*********************************************************************/
	bool CreateScenarioFavoriteXML(const char* pFilePath) const;

	/**********************************************************************
	* name			: CreateScenarioFavoriteXML
	* description	: ����TVWallScenarioFavorite.xml��ģ��
	* input			: pFilePath �ļ�·��
	* output     	: NA
	* return     	: bool - �Ƿ񴴽��ɹ�
	* remark     	: NA   
	*********************************************************************/
	bool CreateTVWallScenarioFavoriteXML(const char* pFilePath) const;

	/**********************************************************************
	* name			: CreateCameraSwitchXML
	* description	: ����CameraSwitch.xml��ģ��
	* input			: pFilePath �ļ�·��
	* output     	: NA
	* return     	: bool - �Ƿ񴴽��ɹ�
	* remark     	: NA   
	*********************************************************************/
	bool CreateCameraSwitchXML(const char* pFilePath) const;


	/**********************************************************************
	* name			: CreateCameraAliasXML
	* description	: ����CameraAlias.xml��ģ��
	* input			: pFilePath �ļ�·��
	* output     	: NA
	* return     	: bool - �Ƿ񴴽��ɹ�
	* remark     	: NA   
	*********************************************************************/
	bool CreateCameraAliasXML(const char* pFilePath) const;

	/*****************************************************************
	     function   : CreateCameraSwitchPlanXML
	     description: ����CameraSwitchPlan.xml��ģ��
	     input      : pFilePath �ļ�·��
	     return     : bool - �Ƿ񴴽��ɹ�
	*****************************************************************/
	bool CreateCameraSwitchPlanXML(const char* pFilePath) const;
		
	/*****************************************************************
	     function   : CreateCameraSwitchPlanXMLTVWall
	     description: ����TVWallCameraSwitchPlan.xml��ģ��
	     input      : pFilePath �ļ�·��
	     return     : bool - �Ƿ񴴽��ɹ�
	*****************************************************************/
	bool CreateCameraSwitchPlanXMLTVWall(const char* pFilePath) const;

	/*****************************************************************
	     function   : CreateCameraSwitchPlanXML
	     description: ����CameraSwitchPlan.xml��ģ��
	     input      : pFilePath �ļ�·��
	     return     : bool - �Ƿ񴴽��ɹ�
	*****************************************************************/
	bool CreateNetKeyBoardInfoXML(const char* pFilePath) const;

	/*****************************************************************
	     function   : CreateBroadcastFileListXML
	     description: ����CameraSwitchPlan.xml��ģ��
	     input      : pFilePath �ļ�·��
	     return     : bool - �Ƿ񴴽��ɹ�
	*****************************************************************/
	bool CreateBroadcastFileListXML(const char* pFilePath) const;

	/*****************************************************************
	     function   : CreateBroadcastCameraListXML
	     description: ����BroadcastCameraList.xml��ģ��
	     input      : pFilePath �ļ�·��
	     return     : bool - �Ƿ񴴽��ɹ�
	*****************************************************************/
	bool CreateBroadcastCameraListXML(const char* pFilePath) const;


	/**********************************************************************
	* name			: ParseLayoutXML2List
	* description	: ���������ݵ�XMLת���ɿɻ�ͼ�ĵ�
	* input			: layoutXML ���ֵ�xml��Ϣ
					  liRectList xml��Ϣת����������
	* output     	: NA
	* return     	: int - ͨ����Ӧ��
	* remark     	: NA   
	*********************************************************************/
	IVS_INT32 ParseLayoutXML2List(const char* layoutXML, std::list<CRect> &liRectList, IVS_UINT32& xNum, IVS_UINT32& yNum) const;
	
	/**********************************************************************
	* name			: GetLayoutXML
	* description	: ��ȡ�����ļ�����ȡָ���Զ��岼�ֵ�XML
	* input			: ulLayoutID ����ID
	* output     	: NA
	* return     	: std::string - ����xml
	* remark     	: NA   
	*********************************************************************/
    IVS_UINT32 GetLayoutXML(unsigned long ulLayoutID, std::string & strlayoutXML) const;

	/**********************************************************************
	* name			: BackUserData
	* description	: ���ݱ����ļ�
	* input			: ulLayoutID ����ID
	* output     	: NA
	* return     	: ������
	* remark     	: NA   
	*********************************************************************/
	IVS_INT32 BackUserData(void);

	/**************************************************************************
	* name       : GetConfigDir
	* description: ��ȡconfigĿ¼
	* input      : dir		���Ŀ¼�Ĵ��
	* return     : �ɹ����ط�����������
	* remark     : ����ģ���벻Ҫ����
    **************************************************************************/
	static IVS_INT32 GetConfigDir(std::string& dir);

	// �����½��
	static void SetLoginName(const std::string& strLoginName);
	// ����IP
	static void SetIP(const std::string& strIP);

	/***********************************************************************************
    * name       : CompressionDir
    * description: ѹ��һ���ļ����µ������ļ���zip,��������Ŀ¼
    * input      : zipName:ѹ����������
				   strInPath: �ļ���·��
				   strOutPath:���zip�ļ�·��
    * output     : 
    * return     : ���سɹ�����ʧ��
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 CompressionDir(const std::string& zipFileName, const std::string& strInPath, const std::string& strOutPath);

	/***********************************************************************************
    * name       : CompressionDir
    * description: ѹ��һ���ļ����µ������ļ���zip,��������Ŀ¼
    * input      : zipFileName:ѹ����������
				   zipFile:ѹ������·��
				   strOutPath:�ļ���ѹ·��
    * output     : 
    * return     : ���سɹ�����ʧ��
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 DecompressionDir(const std::string& zipFileName, const std::string& zipFilePath, const std::string& strOutPath);

	/***********************************************************************************
    * name       : StartUserData
    * description: ѹ��һ���ļ����µ������ļ���zip,��������Ŀ¼
    * input      : iSessionID sessionid
				   strLocalLoginName �û���
	               strLocalstrIP   ip
    * output     : 
    * return     : ���سɹ�����ʧ��
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 StartUserData(IVS_INT32 iSessionID, std::string &strLocalLoginName, std::string &strLocalstrIP);

	/***********************************************************************************
    * name       : StopLocalConfigTimer
    * description: ѹ��һ���ļ����µ������ļ���zip,��������Ŀ¼
    * input      : NA
    * output     : 
    * return     : NA
    * remark     : NA
    ***********************************************************************************/
	void StopLocalConfigTimer(void);
	
	/***********************************************************************************
    * name       : onTrigger
    * description: ��ʱ���Ļص�����
    * input      : pArg ���Ӳ���
				   ullScales ��ʱ��ʱ��
	               style   ��ʱ������
    * output     : 
    * return     : NA
    * remark     : NA
    ***********************************************************************************/
	virtual void onTrigger(void* pArg, ULONGLONG ullScales, TriggerStyle style);

private:
	/**********************************************************************
	* name			: CreateXML
	* description	: ��ָ��·��������ģ��XML
	* input			: pFilePath �ļ�·��
					  pStream xmlģ����Ϣ
	* output     	: NA
	* return     	: bool - �Ƿ񴴽�ģ��ɹ�
	* remark     	: NA   
	*********************************************************************/
	bool CreateXML(const char* pFilePath, const char* pStream) const;

	/**********************************************************************
	* name			: StartLocalConfigTimer
	* description	: ������ʱ��
	* input			: NA
	* output     	: NA
	* return     	: NA
	* remark     	: NA   
	*********************************************************************/
	void StartLocalConfigTimer();
public:
	static std::string m_strLocalLoginName;  
	static std::string m_strLocalstrIP;
    std::string m_strFileNamePath;

	CNVSTimer m_LocalConfigTimerInst; 	     // �û����ݱ��ݶ�ʱ��
	IVS_INT32 m_iSessionID;
	bool m_bBackUserData;
	VOS_Mutex* m_pMutexLocalConfigFileLock;  // ��鱾���ļ��Ƿ�������������	

};

#endif

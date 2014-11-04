/********************************************************************
filename    :    Favorite.h
author      :    
created     :    2012/11/26
description :    �豸�ղغͳ���ʵ��
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/13 ��ʼ�汾
*********************************************************************/
#ifndef _IVS_FAVORITE_H_
#define _IVS_FAVORITE_H_

#include "hwsdk.h"
#include "SDKDef.h"

USING_NAMESPACE_BP


class CFavorite
{
public:
	CFavorite(void);
	virtual ~CFavorite(void);
	static const IVS_CHAR* NewGUID();
    
    /******************************************************************
     function   : AddFavoriteDevice
     description: ����豸�ղ�
     input      : const IVS_CHAR * pDeviceFavoriteInfo
     input      : std::string & strDeviceGUID
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    static IVS_INT32 AddFavoriteDevice(const IVS_CHAR* pDeviceFavoriteInfo, std::string& strDeviceGUID);
    
    /******************************************************************
     function   : MoveFavoriteDevice
     description: �ƶ��豸�ղ�
     input      : const IVS_CHAR * pDeviceFavoriteInfo
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    static IVS_INT32 MoveFavoriteDevice(const IVS_CHAR* pDeviceFavoriteInfo);
    
    /******************************************************************
     function   : DelFavoriteDevice
     description: ɾ���豸�ղ�
     input      : const IVS_CHAR * pDeviceFavoriteInfo
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    static IVS_INT32 DelFavoriteDevice(const IVS_CHAR* pDeviceFavoriteInfo);
    
    /******************************************************************
     function   : DelFavoriteFolder
     description: ɾ���ղؼ�
     input      : const IVS_CHAR * pDeviceFavoriteInfo
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    static IVS_INT32 DelFavoriteFolder(const IVS_CHAR* pDeviceFavoriteInfo);
    
    /******************************************************************
     function   : AddFavoriteFolder
     description: �����ղؼ�
     input      : const IVS_CHAR * pDeviceFavoriteInfo
     input      : std::string & strFolderGUID
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    static IVS_INT32 AddFavoriteFolder(const IVS_CHAR* pDeviceFavoriteInfo, std::string& strFolderGUID);
    
    /******************************************************************
     function   : ModFavoriteFolder
     description: �޸��ղؼ�
     input      : const IVS_CHAR * pDeviceFavoriteInfo
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    static IVS_INT32 ModFavoriteFolder(const IVS_CHAR* pDeviceFavoriteInfo);
    
    /******************************************************************
     function   : MoveFavoriteFolder
     description: �ƶ��ղؼ�
     input      : const IVS_CHAR * pDeviceFavoriteInfo
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    static IVS_INT32 MoveFavoriteFolder(const IVS_CHAR* pDeviceFavoriteInfo);
    
    /******************************************************************
     function   : GetFavoriteDevice
     description: ��ѯ�豸�ղ�
     input      : CXml & xmlRsp
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    static IVS_INT32 GetFavoriteDevice(CXml& xmlRsp);
    
    /******************************************************************
     function   : AddFavoriteDeviceRspXML
     description: ƴװ�ղ��豸�ɹ���Ӧxml
     input      : const std::string & strDeviceGUID
     input      : CXml & xml
     output     : NA
     return     : void
    *******************************************************************/
    static void AddFavoriteDeviceRspXML(const std::string& strDeviceGUID, CXml& xml);
    
    /******************************************************************
     function   : AddFavoriteFolderRspXML
     description: ƴװ����ղؼгɹ���Ӧxml
     input      : const std::string & strFolderGUID
     input      : CXml & xml
     output     : NA
     return     : void
    *******************************************************************/
    static void AddFavoriteFolderRspXML(const std::string& strFolderGUID, CXml& xml);

	/******************************************************************
	 function   : AddScenarioFavorite
	 description: ��������
	 input      : const IVS_CHAR * pScenarioXml
	 input      : const std::string & strID
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	static IVS_INT32 AddScenarioFavorite(const IVS_CHAR* pScenarioXml, const std::string& strID);

	/******************************************************************
	 function   : ModScenarioFavorite
	 description: �޸ĳ���
	 input      : const IVS_CHAR * pScenarioXml
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	static IVS_INT32 ModScenarioFavorite(const IVS_CHAR* pScenarioXml);

	//�޸ĵ���ǽ����
	static IVS_INT32 SetTVwallScenarioFavorite(const IVS_CHAR* pScenarioXml);
	
	/******************************************************************
	 function   : DelScenarioFavorite
	 description: ɾ������
	 input      : const IVS_CHAR * pScenarioID
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	static IVS_INT32 DelScenarioFavorite(const IVS_CHAR* pScenarioID);
	
	/******************************************************************
	 function   : GetScenarioFavoriteList
	 description: ��ȡ�����б�
	 input      : std::string & strScenarioInfoList
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	static IVS_INT32 GetScenarioFavoriteList(std::string& strScenarioInfoList);
	
	/******************************************************************
	 function   : GetScenarioFavorite
	 description: ��ȡ��������
	 input      : const IVS_CHAR * pScenarioID
	 input      : std::string & strScenarioInfo
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	static IVS_INT32 GetScenarioFavorite(const IVS_CHAR* pScenarioID, std::string& strScenarioInfo);

	/******************************************************************
	 function   : GetScenarioFavorite
	 description: ��ȡ����ǽ��������
	 input      : const IVS_CHAR * pScenarioID
	 input      : std::string & strScenarioInfo
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	static IVS_INT32 GetTVWallScenarioFavorite(const IVS_CHAR* pScenarioID, std::string& strScenarioInfo);
	
	/******************************************************************
	 function   : GetTempScenario
	 description: ��ȡ��ʱ����
	 input      : void
	 output     : NA
	 return     : std::string
	*******************************************************************/
	static std::string GetTempScenario(const IVS_CHAR* pScenarioID);

	//��ȡ����ǽ��ʱ����
	static std::string GetTVWallTempScenario(const std::string& strScenarioID);

	// ƴ������������Ӧ
	static void BulidAddScenarioInfoResult(IVS_INT32 iResultCode, const std::string& strScenarioID, std::string& strResult);
	// ƴ�ӻ�ȡ������Ӧ
	static void BulidGetScenarioInfoResult(IVS_INT32 iResultCode, const std::string& strScenarioInfo, std::string& strResult);
	// ƴ�ӻ�ȡ�����б���Ӧ
	static void BulidGetScenarioInfoListResult(IVS_INT32 iResultCode, const std::string& strScenarioInfoList, std::string& strResult);

	/******************************************************************
	 function   : AddScenarioFavorite
	 description: ������Ѳ��Դ
	 input      : const IVS_CHAR * pSwitchInfo
	 input      : const std::string & strSwitchID
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	static IVS_INT32 AddCameraSwitch(const IVS_CHAR* pSwitchInfo, const std::string& strSwitchID);
	
	/******************************************************************
	 function   : DelCameraSwitch
	 description: ɾ����Ѳ��Դ
	 input      : const IVS_CHAR * pScenarioID
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	static IVS_INT32 DelCameraSwitch(const IVS_CHAR* pCameraSwitchID);

    /******************************************************************
	 function   : ModCameraSwitch
	 description: ɾ����Ѳ��Դ
	 input      : const IVS_CHAR * pScenarioID
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	static IVS_INT32 ModCameraSwitch(const IVS_CHAR* pSwitchInfo);
	
	/******************************************************************
	 function   : GetCameraSwitchInfoList
	 description: ��ȡ��Ѳ��Դ�б�
	 input      : std::string & strCameraSwitchInfoList
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	static IVS_INT32 GetCameraSwitchInfoList(std::string& strCameraSwitchInfoList);
	
	/******************************************************************
	 function   : GetCameraSwitchInfo
	 description: ��ȡ��Ѳ��Դ����
	 input      : const IVS_CHAR * pSwitchInfo
	 input      : std::string & strCameraSwitchInfo
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	static IVS_INT32 GetCameraSwitchInfo(const IVS_CHAR* pCameraSwitchID, std::string& strCameraSwitchInfo);

	// ƴ��������Ѳ��Դ��Ӧ
	static void BulidAddCameraSwitchResult(IVS_INT32 iResultCode, const std::string& strCameraSwitchID, std::string& strResult);
	// ƴ�Ӳ�ѯ��Ѳ��Դ������Ӧ
	static void BulidGetCameraSwitchInfoResult(IVS_INT32 iResultCode, const std::string& strCameraSwitchInfo, std::string& strResult);
	// ƴ�ӻ�ȡ��Ѳ��Դ�б���Ӧ
	static void BulidGetCameraSwitchListResult(IVS_INT32 iResultCode, const std::string& strCameraSwitchInfoList, std::string& strResult);

	// �����Ѳ�ƻ�
	static IVS_INT32 AddCameraSwitchPlan(const IVS_CHAR* pSwitchPlanInfo);

	// ɾ����Ѳ�ƻ�
	static IVS_INT32 DelCameraSwitchPlan(const IVS_CHAR* pCameraSwitchID);

	// ��ȡ��Ѳ�ƻ�
	static IVS_INT32 GetCameraSwitchPlan(const IVS_CHAR* pCameraSwitchID, std::string& strCameraSwitchPlan);

	// ��ȡ��Ѳ�ƻ��б�
	static IVS_INT32 GetCameraSwitchPlanList(std::string& strCameraSwitchPlanList);

	// ��ӵ���ǽ��ѵ�ƻ�
	static IVS_INT32 AddCameraSwitchPlanTVWall(const IVS_CHAR* pSwitchPlanInfo);

	// ɾ������ǽ��Ѳ�ƻ�
	static IVS_INT32 DelCameraSwitchPlanTVWall(ULONG ulTVWallID,const IVS_CHAR* pCameraSwitchID);
	
	// ��ȡ����ǽ��Ѳ�ƻ�
	static IVS_INT32 GetCameraSwitchPlanTVWall(ULONG ulTVWallID, const IVS_CHAR* pCameraSwitchID, std::string& strTVWallCameraSwitchPlan);

	// ��ȡ����ǽ��Ѳ�ƻ��б�
	static IVS_INT32 GetCameraSwitchPlanListTVWall(std::string& strTVWallCameraSwitchPlanList);

	// ��ȡxml�е���Ѳ��ԴID
	static IVS_INT32 GetCameraSwitchID(const IVS_CHAR* pSwitchPlanInfo, std::string &pCameraSwitchID);
private:
    static IVS_INT32 AddFavoriteDeviceParseXML(const IVS_CHAR* pDeviceFavoriteInfo, IVS_CHAR* pszParentFolderID, IVS_CHAR* pszDeviceCode);
    static IVS_INT32 MoveFavoriteDeviceParseXML(const IVS_CHAR* pDeviceFavoriteInfo, IVS_CHAR* pszParentFolderID, IVS_CHAR* pszDeviceFavoriteID);
    static IVS_INT32 DelFavoriteDeviceParseXML(const IVS_CHAR* pDeviceFavoriteInfo, IVS_CHAR* pszDeviceFavoriteID);
    static IVS_INT32 DelFavoriteFolderParseXML(const IVS_CHAR* pDeviceFavoriteInfo, IVS_CHAR* pszFolderID);
    static IVS_INT32 AddFavoriteFolderParseXML(const IVS_CHAR* pDeviceFavoriteInfo, IVS_CHAR* pszParentFolderID, IVS_CHAR* pszFolderName);
    static IVS_INT32 ModFavoriteFolderParseXML(const IVS_CHAR* pFavoriteFolder, IVS_CHAR* pszFolderID, IVS_CHAR* pszFolderName);
    static IVS_INT32 MoveFavoriteFolderParseXML(const IVS_CHAR* pFavoriteFolder, IVS_CHAR* pszParentFolderID, IVS_CHAR* pszFolderID);
};

#endif //_IVS_FAVORITE_H_

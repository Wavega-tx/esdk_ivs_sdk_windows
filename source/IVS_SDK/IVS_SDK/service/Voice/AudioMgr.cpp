#include "StdAfx.h"
#include "AudioMgr.h"
#include "IVSCommon.h"
#include "LockGuard.h"


CAudioMgr::CAudioMgr(void)
{
    m_pUserMgr = NULL;
    m_pHandleTalkBackMutex = VOS_CreateMutex();
}

CAudioMgr::~CAudioMgr(void)
{
    m_pUserMgr = NULL;
    try
    {
        HandleTalkBackMapIter iter = m_HanleTalkBackMap.begin();
        while(m_HanleTalkBackMap.end() != iter)
        {
            CAudioObj* pTalkback = dynamic_cast<CAudioObj*>(iter->second);//lint !e611
            if (NULL != pTalkback)
            {
                IVS_DELETE(pTalkback);
            }
            ++iter;
        }
        m_HanleTalkBackMap.clear();

        if (NULL != m_pHandleTalkBackMutex)
        {
            (void)VOS_DestroyMutex(m_pHandleTalkBackMutex);
            m_pHandleTalkBackMutex = NULL;
        }
    }
    catch (...)
    {
    }

    if (NULL != m_pHandleTalkBackMutex)
    {
        VOS_DestroyMutex(m_pHandleTalkBackMutex);
        m_pHandleTalkBackMutex = NULL;
    }
}

//�������
void CAudioMgr::ClearUp()
{
	m_audioObj.ClearUp();
}

// �����û�����
// void CAudioMgr::SetUserObj(void* pUserObj)
// {
// 	if(pUserObj)
// 	{
// 		m_pUserObj = pUserObj;
// 	}
// }

void CAudioMgr::SetUserMgr(CUserMgr* pUserMgr)
{
    m_pUserMgr = pUserMgr;

    // modify by wanglei 00165153:2013.5.3 m_audioObj�ǳ�Ա������m_pUserMgr������Ҫ�õ�
    m_audioObj.SetUserMgr(m_pUserMgr);
}

//���ݾ����ȡ���ж���
CAudioObj* CAudioMgr::GetFreeTalkBack(IVS_ULONG& ulHandle)
{
    CLockGuard lock(m_pHandleTalkBackMutex);
    CAudioObj *pTalkback = NULL;
    HandleTalkBackMapIter talkbackIter = m_HanleTalkBackMap.begin();
    HandleTalkBackMapIter talkbackIterEnd = m_HanleTalkBackMap.end();

    //���ҿ��ж���;
    for (; talkbackIter != talkbackIterEnd; talkbackIter++)
    {
        pTalkback = dynamic_cast<CAudioObj*>(talkbackIter->second);//lint !e611
        if (NULL != pTalkback)
        {
            if (MEDIA_STATUS_FREE == pTalkback->GetStatus())
            {
                break;
            }
        }
        pTalkback = NULL;
    }

    if (NULL != pTalkback)
    {
        m_HanleTalkBackMap.erase(talkbackIter);
    }
    else
    {
        pTalkback = IVS_NEW((CAudioObj*&)pTalkback);
        if (NULL == pTalkback)
        {
            BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "Get Free Talkback", "create talkback error");
            return NULL;
        }
    }

    //��ʼ��talkback����;
    pTalkback->SetUserMgr(m_pUserMgr);
    pTalkback->SetStatus(MEDIA_STATUS_BUSY);

    //�����б�;
    ulHandle = reinterpret_cast<unsigned long>(pTalkback);
    (void)m_HanleTalkBackMap.insert(std::make_pair(ulHandle, pTalkback));

    pTalkback->SetHandle(ulHandle);
    return pTalkback;
}

//��ʼ�����Խ�
IVS_INT32 CAudioMgr::StartVoiceTalkback(const IVS_VOICE_TALKBACK_PARAM* pTalkbackParam,
                                        const IVS_CHAR* pCameraCode,
                                        IVS_ULONG* pHandle)
{
//    AddBroadcastDevice(pCameraCode);
//    return StartFileBroadcast("D:\\01.WorkSpace\\code\\eIVS V100R001C30\\IVS_MediaPlayer\\Debug\\alarm.Mp3", TRUE);

	if (NULL == pTalkbackParam)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Start Voice Talkback", "NULL == pTalkbackParam");
		return IVS_PARA_INVALID;
	}

	if (NULL == pCameraCode)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Start Voice Talkback", "NULL == pCameraCode");
		return IVS_PARA_INVALID;
	}

    if (NULL == pHandle)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Start Voice Talkback", "NULL == pHandle");
        return IVS_PARA_INVALID;
    }

	int voiceCodeLen = strlen(pCameraCode);//lint !e713
	if( voiceCodeLen <= 0 || voiceCodeLen > IVS_DEV_CODE_LEN)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Audio Mgr",
                       "StartVoiceTalkback invalid pCameraCode:%s",pCameraCode);
		return IVS_PARA_INVALID;
	}

    IVS_ULONG ulHandle = SDK_INVALID_HANDLE;
    int iRet = IVS_FAIL;

    //��ʼ��talkback����;
    CAudioObj *pAudioObj = GetFreeTalkBack(ulHandle);

    if (NULL != pAudioObj)
    {
        iRet = pAudioObj->StartVoiceTalkback(pTalkbackParam->uiProtocolType,
                                             pTalkbackParam->bDirectFirst,
                                             pCameraCode);
        if (IVS_SUCCEED != iRet)
        {
            pAudioObj->RetSet();
            BP_RUN_LOG_ERR(IVS_FAIL, "Start Voice Talkback",
                        "pAudioObj->StartVoiceTalkback failed iRet:%d",iRet);
            // modify by wanglei 00165153:2013.6.8 DTS2013052308679 
            // ����Ѿ��������豸�����Խ���ͳһ���� IVS_SDK_TALKBACK_ERR_ALREADY_OPENED ������
            if (iRet ==  IVS_PLAYER_RET_VOICE_TALKBACK_STATUS_ERROR)
            {
                iRet = IVS_SDK_TALKBACK_ERR_ALREADY_OPENED;
            }
        }
        else
        {
            *pHandle = ulHandle;
        }
    }
    else
    {
        iRet = IVS_SDK_ERR_FINDAUDIOHANDLE_FAILED;
    }

    return iRet;
	
	//��handle���Ӧ�������������豸��Ϣ��Ӧ��ϵ��������,AddStream�ڲ�������ڴ汣�����ݣ����Դ˴����Դ�����ʱ����ָ�롣
// 	long lRet = m_StreamData.AddStream(lHandle, pAudioObj, NULL, (char*)pCameraCode,&voiceRtspInfo);//lint !e1773
// 	if(lRet < IVS_SUCCEED)
// 	{
// 		BP_RUN_LOG_ERR(IVS_FAIL, "Start Voice Talkback", "m_StreamData.AddStream failed lRet:%d",lRet);
// 		return lRet;
// 	}
// 
//     return IVS_SUCCEED;
}

//ֹͣ�����Խ�
IVS_INT32 CAudioMgr::StopVoiceTalkback(INT32 iTalkbackHandle)
{
//    return StopFileBroadcast();

	//��ȡ����Ϣ�ﱣ�����������ָ��
	CAudioObj* pAudioObj = m_HanleTalkBackMap[iTalkbackHandle];//lint !e732
	if(NULL == pAudioObj)
	{
		BP_RUN_LOG_ERR(IVS_NULL_POINTER, "AudioMgr StopVoiceTalkback",
                       "CAudioMgr::StopVoiceTalkback NULL == pAudioObj");
		return IVS_NULL_POINTER;
	}

	//int iRet = pAudioObj->StopVoiceTalkback(&pInfo->voiceRtspInfo);
	int iRet = pAudioObj->StopVoiceTalkback();
	if(IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "AudioMgr StopVoiceTalkback",
            "CAudioMgr::StopVoiceTalkback pAudioObj->StopVoiceTalkback failed iRet:%d",iRet);
		return iRet;
	}

	//�ͷ���Դ
	if(pAudioObj) //lint !e774
	{ 
		delete pAudioObj;
        
        // modify by wanglei 00165153:2013.5.8 DTS2013050708570
        // m_HanleTalkBackMap.second �д�ŵ���ָ�룬ɾ��ָ���Ӧ��ֵ�ռ�󣬲��ÿջᵼ��Ұָ�룬��������ʱ����ٴ�ɾ���ᵼ�·���Խ��
        // m_HanleTalkBackMap[iTalkbackHandle] = NULL;
        m_HanleTalkBackMap.erase(iTalkbackHandle);//lint !e732

		//pAudioObj = NULL; 
	}
// 	if(pInfo->lEventHandle) 
// 	{ 
// 		CloseHandle(pInfo->lEventHandle);     
// 		pInfo->lEventHandle = NULL;
// 	}
// 	if(pInfo) //lint !e774
// 	{ 
// 		delete pInfo;     
// 		//pInfo = NULL; 
// 	}

	return IVS_SUCCEED;
}

//��������豸���㲥��
IVS_INT32 CAudioMgr::AddBroadcastDevice(const IVS_CHAR* pCameraCode)
{
	if (NULL == pCameraCode)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "AudioMgr AddBroadcastDevice",
            "CAudioMgr::AddBroadcastDevice NULL == pCameraCode");
		return IVS_PARA_INVALID;
	}

	int voiceCodeLen = strlen(pCameraCode);//lint !e713
	if( voiceCodeLen <= 0 || voiceCodeLen > IVS_DEV_CODE_LEN)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "AudioMgr AddBroadcastDevice",
            "CAudioMgr::AddBroadcastDevice invalid pCameraCode:%s",pCameraCode);
		return IVS_PARA_INVALID;
	}

    INT32 iRet = m_audioObj.AddBroadcastDevice(pCameraCode);
    if(IVS_SUCCEED != iRet)
    {
		BP_RUN_LOG_ERR(iRet, "AudioMgr AddBroadcastDevice",
            "CAudioMgr::AddBroadcastDevice pAudioObj->StartVoiceTalkback failed iRet:%d",iRet);
        return iRet;
    }
	return IVS_SUCCEED;
}

//ɾ�������豸
IVS_INT32 CAudioMgr::DeleteBroadcastDevice(const IVS_CHAR* pCameraCode)
{
	if (NULL == pCameraCode)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "AudioMgr::DeleteBroadcastDevice",
            "CAudioMgr::DeleteBroadcastDevice NULL == pCameraCode");
		return IVS_PARA_INVALID;
	}

	int voiceCodeLen = strlen(pCameraCode);//lint !e713
	if( voiceCodeLen <= 0 || voiceCodeLen > IVS_DEV_CODE_LEN)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "AudioMgr::DeleteBroadcastDevice",
            "CAudioMgr::DeleteBroadcastDevice invalid pCameraCode:%s",pCameraCode);
		return IVS_PARA_INVALID;
	}

    INT32 iRet = m_audioObj.DeleteBroadcastDevice(pCameraCode);
    if(IVS_SUCCEED != iRet)
    {
		BP_RUN_LOG_ERR(iRet, "AudioMgr::DeleteBroadcastDevice",
            "CAudioMgr::DeleteBroadcastDevice pAudioObj->DeleteBroadcastDevice failed iRet:%d",iRet);
        return iRet;
    }
	return IVS_SUCCEED;
}

//��ʼʵʱ�����㲥
IVS_INT32 CAudioMgr::StartRealBroadcast()
{
// 	if (NULL == pMediaPara)
// 	{
// 		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "AudioMgr StartRealBroadcast",
//             "CAudioMgr::StartRealBroadcast NULL == pMediaPara");
// 		return IVS_PARA_INVALID;
// 	}
// 
// 	//Ŀǰֻ֧�����²�����д������������û����룬��������ֻΪ�Ժ���չ
// 	pMediaPara->DecoderCompany = VENDOR_HUAWEI;
// 	pMediaPara->StreamType = STREAM_TYPE_MAIN;


	//Ϊ��ʵʱ��ȡ��¼��Ϣ��ÿ������������Ҫ����ָ���ϲ��û���ʵ����ָ�롣
	//m_audioObj.SetUserObj((CUserObj*)m_pUserObj);

        // delete by wanglei:00165153 2013.5.3, �ò��ַ���void CAudioMgr::SetUserMgr(CUserMgr* pUserMgr)����
    // wanglei:�õط�����ɾ����ԭ����ܣ�	(void)GetLocalIpPort(); ��SetUserMgr����
    m_audioObj.SetUserMgr(m_pUserMgr);
    

    INT32 iRet = m_audioObj.StartRealBroadcast();
    if(IVS_SUCCEED != iRet)
    {
		BP_RUN_LOG_ERR(iRet, "AudioMgr StartRealBroadcast",
            "CAudioMgr::StartRealBroadcast pAudioObj->StartVoiceTalkback failed iRet:%d",iRet);
    }		
	return iRet;//lint !e539
}

//ֹͣʵʱ�����㲥
IVS_INT32 CAudioMgr::StopRealBroadcast()
{
	//int iRet = IVS_SUCCEED;

	int iRet = m_audioObj.StopRealBroadcast();
    if(IVS_SUCCEED != iRet)
    {
		BP_RUN_LOG_ERR(iRet, "AudioMgr::StopRealBroadcast",
            "CAudioMgr::StopRealBroadcast pAudioObj->StopRealBroadcast failed iRet:%d",iRet);
    }	
	return iRet;
}

//��ʼ�ļ������㲥
IVS_INT32 CAudioMgr::StartFileBroadcast(const IVS_CHAR* pFileName,IVS_INT32 iCycle)
{
// 	if (NULL == pMediaPara)
// 	{
// 		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "AudioMgr StartFileBroadcast",
//             "CAudioMgr::StartFileBroadcast NULL == pMediaPara");
// 		return IVS_PARA_INVALID;
// 	}

	if(NULL == pFileName)//pFileName���ȼ�������m_audioObj��
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "AudioMgr StartFileBroadcast",
            "CAudioMgr::StartFileBroadcast NULL == pFileName");
		return IVS_PARA_INVALID;
	}

	if(iCycle<0 || iCycle>1)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "AudioMgr StartFileBroadcast",
            "CAudioMgr::StartFileBroadcast invalid iCycle");
		return IVS_PARA_INVALID;
	}

	//Ŀǰֻ֧�����²�����д������������û����룬��������ֻΪ�Ժ���չ
// 	pMediaPara->DecoderCompany = VENDOR_HUAWEI;
// 	pMediaPara->StreamType = STREAM_TYPE_MAIN;

	//Ϊ��ʵʱ��ȡ��¼��Ϣ��ÿ������������Ҫ����ָ���ϲ��û���ʵ����ָ�롣
	//m_audioObj.SetUserObj((CUserObj*)m_pUserObj);

     // modify by wanglei 00165153:2013.5.3 �Ѿ��ڳ�ʼ��������
        // wanglei:�õط�����ɾ����ԭ����ܣ�	(void)GetLocalIpPort(); ��SetUserMgr����
    m_audioObj.SetUserMgr(m_pUserMgr);

    INT32 iRet = m_audioObj.StartFileBroadcast(pFileName, iCycle);
    if(IVS_SUCCEED != iRet)
    {
		BP_RUN_LOG_ERR(iRet, "AudioMgr StartFileBroadcast",
            "CAudioMgr::StartFileBroadcast pAudioObj->StartVoiceTalkback failed iRet:%d",iRet);
    }	
	return iRet;
}

//ֹͣ�ļ������㲥
IVS_INT32 CAudioMgr::StopFileBroadcast()
{
	int iRet = m_audioObj.StopFileBroadcast();
    if(IVS_SUCCEED != iRet)
    {
		BP_RUN_LOG_ERR(iRet, "AudioMgr StopFileBroadcast",
            "CAudioMgr::StopFileBroadcast pAudioObj->StopRealBroadcast failed iRet:%d",iRet);
    }	

	return iRet;
}

//��������
IVS_INT32 CAudioMgr::SetVolume(unsigned int uiPlayHandle, unsigned int uiVolumeValue)
{
    IVS_INT32 iRet = IVS_FAIL;

    CAudioObj *pAudioObj = m_HanleTalkBackMap[uiPlayHandle];
    if (NULL != pAudioObj)
    {
        iRet = pAudioObj->SetVolume(uiVolumeValue);
    }
    else
    {
        iRet = IVS_SDK_ERR_FINDVIDEOHANDLE_FAILED;
    }

    return iRet;
}

//��ȡ����
IVS_INT32 CAudioMgr::GetVolume(unsigned int uiPlayHandle, unsigned int* puiVolumeValue)
{	
    IVS_INT32 iRet = IVS_FAIL;

    CAudioObj *pAudioObj = m_HanleTalkBackMap[uiPlayHandle];
    if (NULL != pAudioObj)
    {
        iRet = pAudioObj->GetVolume(puiVolumeValue);
    }
    else
    {
        iRet = IVS_SDK_ERR_FINDVIDEOHANDLE_FAILED;
    }

    return iRet;
}

CAudioObj* CAudioMgr::GetAudioObj(IVS_ULONG ulHandle)
{
	//CLockGuard lock(m_pHandleDownLoadMutex);
	CAudioObj *pAudioObj = NULL;
	HandleTalkBackMapIter iter = m_HanleTalkBackMap.begin();
	HandleTalkBackMapIter end  = m_HanleTalkBackMap.end();

	//���ҿ��ж���;
	for (; iter != end; iter++)
	{
		if (iter->first == ulHandle)
		{
			pAudioObj = dynamic_cast<CAudioObj*>(iter->second);//lint !e611
			break;
		}
	}
	return pAudioObj;
}

// ͨ��RTSP��ȡPalyHandle(CAudioObj�Ļ����ַ)�������¼��ϱ�
IVS_ULONG CAudioMgr::GetTalkBackPlayHandleByRtspHandle(IVS_ULONG ulRtspHandle)
{
	CAudioObj *pAudioObj = NULL;
	HandleTalkBackMapIter iter = m_HanleTalkBackMap.begin();
	HandleTalkBackMapIter end  = m_HanleTalkBackMap.end();

	IVS_ULONG ulPlayHandle = 0;
	//���ҿ��ж���;
	for (; iter != end; iter++)
	{
		pAudioObj = dynamic_cast<CAudioObj*>(iter->second);//lint !e611
		if (NULL == pAudioObj)
		{
			continue;
		}

		if (pAudioObj->IsExistTalkBackRtspHandle(ulRtspHandle))
		{
			ulPlayHandle = iter->first;
			break;
		}
	}

	return ulPlayHandle;
}

// ͨ��RTSP��ȡPalyHandle(CAudioObj�Ļ����ַ)�������¼��ϱ�
IVS_ULONG CAudioMgr::GetBroadCastPlayHandleByRtspHandle(IVS_ULONG ulRtspHandle)
{
	bool bRet = m_audioObj.IsExistBroadCastRtspHandle(ulRtspHandle);
	if (bRet)
	{
		return m_audioObj.GetPlayHandle();
	}
	
	return 0;	
}

IVS_ULONG CAudioMgr::GetTalkBackPlayHandleByNetHandle(IVS_ULONG ulNetHandle)
{
    CAudioObj *pAudioObj = NULL;
    HandleTalkBackMapIter iter = m_HanleTalkBackMap.begin();
    HandleTalkBackMapIter end  = m_HanleTalkBackMap.end();

    IVS_ULONG ulPlayHandle = 0;
    //���ҿ��ж���;
    for (; iter != end; iter++)
    {
        pAudioObj = dynamic_cast<CAudioObj*>(iter->second);//lint !e611
        if (NULL == pAudioObj)
        {
            continue;
        }

        if (pAudioObj->IsExistTalkBackNetHandle(ulNetHandle))
        {
            ulPlayHandle = iter->first;
            break;
        }
    }

    return ulPlayHandle;
}

IVS_ULONG CAudioMgr::GetBroadCastPlayHandleByNetHandle(IVS_ULONG ulNetHandle)
{
    bool bRet = m_audioObj.IsExistBroadCastNetHandle(ulNetHandle);
    if (bRet)
    {
        return m_audioObj.GetPlayHandle();
    }

    return 0;	
}

bool CAudioMgr::IsBroadCast(IVS_ULONG ulPlayHandle) const
{
	unsigned long ulPlayHandleTmp = m_audioObj.GetPlayHandle();
	return (ulPlayHandle == ulPlayHandleTmp) ? true : false;
}

//IVS_INT32 CAudioMgr::GetBroadCastVoiceCodeByRtspHandle(IVS_ULONG ulRtspHandle, std::string& strVoiceCode)
//{
//	int iRet = m_audioObj.GetBroadCastVoiceCodeByRtspHandle(ulRtspHandle, strVoiceCode);
//	return iRet;
//}

IVS_INT32 CAudioMgr::GetBroadCastDeviceCodeByConnHandle(IVS_ULONG ulRtspOrNetHandle, std::string& strDeviceCode)
{
    return m_audioObj.GetBroadCastDeviceCodeByConnHandle(ulRtspOrNetHandle, strDeviceCode);
}
IVS_INT32 CAudioMgr::GetTalkBackDeviceCodeByConnHandle(IVS_ULONG ulRtspOrNetHandle, std::string& strDeviceCode)
{
    CAudioObj *pAudioObj = NULL;
    HandleTalkBackMapIter iter = m_HanleTalkBackMap.begin();
    HandleTalkBackMapIter end  = m_HanleTalkBackMap.end();

    for (; iter != end; iter++)
    {
        pAudioObj = dynamic_cast<CAudioObj*>(iter->second);
        if (NULL == pAudioObj)
        {
            continue;
        }
        if (pAudioObj->GetRtspHandle() == (int)ulRtspOrNetHandle || pAudioObj->GetNetSourceHandle() == ulRtspOrNetHandle)
        {
            pAudioObj->GetCameraID(strDeviceCode);
            return IVS_SUCCEED;
        }
    }

    return IVS_FAIL;
}

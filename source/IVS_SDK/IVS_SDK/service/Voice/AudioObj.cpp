//#include "StdAfx.h"
#include "AudioObj.h"
#include "LockGuard.h"
#include "IVS_Player.h"
#include "UserMgr.h"
#include <io.h>

#define MAX_BROADCAST_COUNT				256
//�ļ��㲥����
//#define VOICE_FILE_BROADCAST 0			//�ļ������㲥
//#define VOICE_REAL_BROADCAST 1			//ʵʱ�����㲥

unsigned int CAudioObj::m_stcAudioType = 0;

// Rtspý�����ݻص�
static void  __SDK_CALL g_fRtspDataCallBack(int /*handle*/, 
    char *pBuffer,
    unsigned int ulBufSize,
    void *pUser)//lint !e528 
{
    //�����Ч�Լ��
    if ((NULL == pBuffer) 
        || (0 == ulBufSize))
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Audio Obj g_fRtsp Data Call Back", "Input param invalid.");
        return;
    }

    if (*pBuffer != '$')
    {
        BP_RUN_LOG_ERR(IVS_PLAYER_RET_RTSP_DATA_ERROR, "Audio Obj g_fRtsp Data Call Back", "rtsp media data format error, do not have $.");
        return;
    }
    if (1 == *(pBuffer+1) || 3 == *(pBuffer+1))
    {
        // RTCP����������ַ�����ȥ
        //(void)IVS_InputData(ulPort, NULL, 0);
        return;
    }

    //��ȡ�ص������������;
    CMediaBase* pMediaBase = static_cast<CMediaBase*>(pUser);
    if (NULL == pMediaBase)
    {
        BP_RUN_LOG_ERR(IVS_FAIL,"Audio Obj g_fRtsp Data Call Back", "transform pUser to MediaBase pointer failed.");
        return;
    }

    unsigned int uiPlayerChannel = pMediaBase->GetPlayerChannel();
    if (0 == uiPlayerChannel)
    {
        BP_RUN_LOG_ERR(IVS_FAIL,"Audio Obj g_fRtsp Data Call Back", "pMediaBase's uiPlayerChannel = 0.");
        return;
    }

    pBuffer   += 4;
    ulBufSize -= 4;
    (void)IVS_PLAY_InputRtpData(uiPlayerChannel, pBuffer, ulBufSize);
    return;
} //lint !e818 !e528

static void __SDK_CALL g_fNetDataCallBack( char *pBuf, unsigned int uiSize, void *pUser)//lint !e818 !e830
{
    //��ȡ�ص������������;
    CMediaBase* pMediaBase = reinterpret_cast<CMediaBase*>(pUser);
    if (NULL == pMediaBase)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID,"AudioObj Net Data Call Back", "transform pUser to MediaBase pointer failed.");
        return;
    }

    unsigned int uiPlayerChannel = pMediaBase->GetPlayerChannel();
    if (0 == uiPlayerChannel)
    {
        BP_RUN_LOG_ERR(IVS_FAIL,"AudioObj Net Data Call Back", "pMediaBase's uiPlayerChannel = 0.");
        return;
    }

    (void)IVS_PLAY_InputRtpData(uiPlayerChannel, pBuf, uiSize);
    return;
}//lint !e818 !e528

CAudioObj::CAudioObj(void)
{
	m_bIsBroadcastOpenedFlag = false;
	m_CodeHandle.clear();
	memset(&m_mediaPara,0,sizeof(m_mediaPara));
	memset(m_fileName,0,FILE_NAME_LEN+1);
	m_iFileBroadcastCycle = -1;
	m_iBroadcastType = -1;
	InitializeCriticalSection(&m_csCodeHandleLock);
}

CAudioObj::~CAudioObj(void)
{
	//m_CodeHandle.clear();//lint !e1551
    try
	{
		ClearUpCodeHandle();
		DeleteCriticalSection(&m_csCodeHandleLock);
	}
	catch(...)
	{

	}
}

void CAudioObj::SetHandle(unsigned long ulHandle)
{
    m_ulHandle = ulHandle;
}//lint !e1762

//�������
void CAudioObj::ClearUp()
{
	StopFileBroadcast();//lint !e534
	StopRealBroadcast();//lint !e534
	m_bIsBroadcastOpenedFlag = false;
    ClearUpCodeHandle();
	//m_CodeHandle.clear();
	memset(&m_mediaPara,0,sizeof(m_mediaPara));
	memset(m_fileName,0,FILE_NAME_LEN+1);
	m_iFileBroadcastCycle = -1;
	m_iBroadcastType = -1;
}

//��ȡЭ������
ULONG CAudioObj::GetProtocolType(char *pProtocol, ULONG ulProtocolType)
{
    ulProtocolType -= 1;
    switch (ulProtocolType)
    {
    case PROTOCOL_RTP_OVER_UDP:
        (void)strncpy(pProtocol, "UDP", strlen("UDP"));
        return PROTOCOL_RTP_OVER_UDP;
    case PROTOCOL_RTP_OVER_TCP:
        (void)strncpy(pProtocol, "TCP", strlen("TCP"));
        return PROTOCOL_RTP_OVER_TCP;
    default:
        (void)strncpy(pProtocol, "UDP", strlen("UDP"));
        return PROTOCOL_RTP_OVER_UDP;
    }
}//lint !e1762


//��ʼ�����Խ�
int CAudioObj::StartVoiceTalkback(unsigned int uiProtocolType,
                                  IVS_BOOL bDirectFirst,
                                  const char* pCameraCode)
{
    CLockGuard lock(m_pMediaMutex);

    // add by wanglei 00165153:2013.6.13 DTS2013060904823
    if (m_bIsBroadcastOpenedFlag)
    {
        BP_RUN_LOG_ERR(IVS_SDK_BROADCASE_ERR_ALREADY_OPENED,"Start Voice Talkback", "broadcast opened");
        return IVS_SDK_BROADCASE_ERR_ALREADY_OPENED;
    }
    
    IVS_MEDIA_PARA stMediaPara;
    memset(&stMediaPara, 0, sizeof(IVS_MEDIA_PARA));
    stMediaPara.ProtocolType = static_cast<IVS_PROTOCOL_TYPE>(uiProtocolType);//�˴�ת���϶���ȷ��ǰ���������;

    // ����Ĭ��ֵ;
    stMediaPara.DecoderCompany = VENDOR_HUAWEI;
    stMediaPara.StreamType = STREAM_TYPE_UNSPECIFIED;
    stMediaPara.TransMode = static_cast<IVS_MEDIA_TRANS_MODE>(bDirectFirst);//�˴�ת���϶���ȷ��ǰ���������;
    stMediaPara.BroadCastType = BROADCAST_UNICAST;
    stMediaPara.VideoDecType = VIDEO_DEC_H264;
    stMediaPara.AudioDecType = AUDIO_DEC_G711U;

    START_PLAY_PARAM stParam;
    stParam.enServiceType = SERVICE_TYPE_AUDIO_CALL;
    stParam.cbRtspDataCallBack = g_fRtspDataCallBack;
    stParam.cbNetDataCallBack = g_fNetDataCallBack;
	// ���öԽ�����
	SetServiceType(SERVICE_TYPE_AUDIO_CALL);
    //��ʼ�����Խ�;
    int iRet = StartPlay(&stMediaPara, pCameraCode, stParam);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"Start Voice Talkback", "Start play error.");
        return iRet;
    }

    iRet = GetSupportAudioType(CAudioObj::m_stcAudioType);

    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"Start Voice Talkback",
                       "Get Support AudioType error.");
        (void)CMediaBase::StopPlay();
        return iRet;
    }

    IVS_ULONG ulVoiceType = 0;

    if (AUDIO_DEC_G711U == CAudioObj::m_stcAudioType)
    {
        ulVoiceType = PAY_LOAD_TYPE_PCMU;
    }
    else if (AUDIO_DEC_G711A == CAudioObj::m_stcAudioType)
    {
        ulVoiceType = PAY_LOAD_TYPE_PCMA;
    }

    iRet = IVS_PLAY_SetVoiceDecoderType(ulVoiceType);

    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"Start Voice Talkback", "Set VoiceDecoder Type.");
        (void)CMediaBase::StopPlay();

        return iRet;
    }

    iRet = IVS_PLAY_StartVoiceTalkback((IVS_CHAR*)m_strLocalIp.c_str(),
                                       (IVS_ULONG)m_uLocalPort,
                                       (IVS_CHAR*)m_szRemoteIp,
                                       (IVS_ULONG)m_uRemotePort);

    if (IVS_SUCCEED != iRet)
    {
        (void)CMediaBase::StopPlay();
        BP_RUN_LOG_ERR(iRet, "Start Voice Talkback",
                       "IVS PLAY Start Voice Talkback failed.");
        return iRet;
    }

    return IVS_SUCCEED;
}//lint !e818

//ֹͣ�����Խ�
int CAudioObj::StopVoiceTalkback()
{
	/*if(NULL == pOutVoiceRtspInfo)
	{
		BP_RUN_LOG_ERR(iRet, "","CAudioObj::StopVoiceTalkback NULL == pOutVoiceRtspInfo");
		return IVS_PARA_INVALID;
	}*/

	int iRet = IVS_PLAY_StopVoiceTalkback();//lint !e732
	if(IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(IVS_SDK_VOICE_ERR_CALL_MLIB_FAIL, "AudioObj StopVoiceTalkback",
            "CAudioObj::StopVoiceTalkback IVS_PLAY_StopVoiceTalkback failed iRet:%d", iRet);
        return IVS_SDK_VOICE_ERR_CALL_MLIB_FAIL;
	}

    iRet += CMediaBase::StopPlay();

	return IVS_SUCCEED;
}//lint !e1762

void CAudioObj::AddCodeHandle(const IVS_CHAR* pCameraCode,
                              BROADCAST_RTSP_INFO* pRtspInfo)
{
	CHECK_POINTER_VOID(pCameraCode);
	CHECK_POINTER_VOID(pRtspInfo);

	EnterCriticalSection(&m_csCodeHandleLock);
    
    std::string strCameraCode = pCameraCode;
    CODE_HANDLE_ITER iter = m_CodeHandle.find(strCameraCode);
    if (iter != m_CodeHandle.end())
    {
        BROADCAST_RTSP_INFO* pstInfo = iter->second;
        memcpy(pstInfo, pRtspInfo, sizeof(BROADCAST_RTSP_INFO));
    }
    else
    {
        (void)m_CodeHandle.insert(make_pair(strCameraCode, pRtspInfo));
    }

    //m_CodeHandle.insert(strVoiceCode, pRtspInfo);
	//m_CodeHandle[strCameraCode] = pRtspInfo;
	LeaveCriticalSection(&m_csCodeHandleLock);
}

BROADCAST_RTSP_INFO* CAudioObj::GetHandleByCode(const IVS_CHAR* pCameraCode)
{
	BROADCAST_RTSP_INFO* pRtspInfo= NULL;
	EnterCriticalSection(&m_csCodeHandleLock);
	std::string strTmp = pCameraCode;
	CODE_HANDLE_ITER iter = m_CodeHandle.find( strTmp );
	if ( iter != m_CodeHandle.end() )
	{
		pRtspInfo = iter->second;//lint !e64
	}
	LeaveCriticalSection(&m_csCodeHandleLock);
	return pRtspInfo;
}

void CAudioObj::DeleteCodeHandle( const IVS_CHAR* pCameraCode )
{
	EnterCriticalSection(&m_csCodeHandleLock);
	std::string strTmp = pCameraCode;
	CODE_HANDLE_ITER iter = m_CodeHandle.find( strTmp );
	if ( iter != m_CodeHandle.end() )
	{
        BROADCAST_RTSP_INFO* pRtspInfo = dynamic_cast<BROADCAST_RTSP_INFO*>(iter->second);
		m_CodeHandle.erase( iter );
        IVS_DELETE(pRtspInfo);
	}
    if (m_CodeHandle.empty())
    {
        // modify by wanglei 00165153:2013.4.26 ���Ŀǰ�����ڹ㲥�豸��Ĭ��Ŀǰ����δ����״̬�������´μ��豸�������Ƶ��ʽ��ͻ
        //m_bIsBroadcastOpenedFlag = false;
        CAudioObj::m_stcAudioType = 0;
    }
	LeaveCriticalSection(&m_csCodeHandleLock);
}

void CAudioObj::ClearUpCodeHandle()
{
    CODE_HANDLE_ITER iter = m_CodeHandle.begin();
    for( ; iter != m_CodeHandle.end(); iter++)
    {
        BROADCAST_RTSP_INFO* pRtspInfo = dynamic_cast<BROADCAST_RTSP_INFO*>(iter->second);
        iter = m_CodeHandle.erase(iter);
        IVS_DELETE(pRtspInfo);
    }
}

//��ӹ㲥��ͷ
int CAudioObj::AddBroadcastDevice(const IVS_CHAR* pCameraCode)
{
	if(NULL == pCameraCode)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "AudioObj AddBroadcastDevice",
            "CAudioObj::AddBroadcastDevice NULL == pCameraCode");
		return IVS_PARA_INVALID;
	}

	int strVoiceCodeLen = (int)strlen(pCameraCode);
	if(strVoiceCodeLen<= 0 || strVoiceCodeLen>IVS_DEV_CODE_LEN)
	{
		return IVS_PARA_INVALID;
	}


	//�ж��Ƿ��Ѿ����б��м�����豸�����ǣ���ôֱ�ӷ��أ���֤ÿ���豸ֻ���һ��
	EnterCriticalSection(&m_csCodeHandleLock);

	//CODE_HANDLE_ITER codeIter = m_CodeHandle.begin();
    // add by wanglei 00165153:2013.4.27
    unsigned int unCodeHandleSize = m_CodeHandle.size();
    CODE_HANDLE_ITER codeIter = m_CodeHandle.find(pCameraCode);
    if (m_CodeHandle.end() != codeIter)
    {
        LeaveCriticalSection(&m_csCodeHandleLock);
        return IVS_SDK_VOICE_ERR_DEV_ALREADY_IN;//�豸�Ѽ����豸�б�
    }
    LeaveCriticalSection(&m_csCodeHandleLock);

	//����豸���е��豸���Ѿ�Ϊ���,�����������
	if ( MAX_BROADCAST_COUNT <= unCodeHandleSize)
	{
		return IVS_SDK_VOICE_ERR_TOO_MUCH_DEVICES;
	}

    //���û�п����㲥��ֻ������豸��handleֵ��Ϊ-1
    BROADCAST_RTSP_INFO *pRtspInfo = NULL;
    pRtspInfo = IVS_NEW(pRtspInfo);

    if (NULL == pRtspInfo)
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"Add Broadcast Device",
            "NEW BROADCAST RTSP INFO error.");
        return IVS_ALLOC_MEMORY_ERROR;
    }

    // start add by wanglei 00165153:2013.5.3 �����������ͳ�ͻ���
    // ԭ�ȸü�����start�н��У�DTS2013040903851 ��Ҫ���������ʵ�ʾ���Ҫ��⣬�����˽�����Ƶ�����ʽһ���Լ��
    unsigned int unAudioType = 0;
    int iRet = GetSupportAudioType(pCameraCode, unAudioType);
    if (iRet != IVS_SUCCEED)
    {
        BP_RUN_LOG_ERR(iRet, "add broadcast device", "fail to get support audio type, camera code:%s", pCameraCode);
        return iRet;
    }
    BP_RUN_LOG_INF("add broadcast device", "success to get support audio type, camera code:%s, support audio type:%d", 
        pCameraCode, unAudioType);
    
    unsigned int unPayLoadType = 0;
    if (AUDIO_DEC_G711U == unAudioType)
    {
        unPayLoadType = PAY_LOAD_TYPE_PCMU;
    }
    else/* if (AUDIO_DEC_G711A == unAudioType)*/
    {
        unPayLoadType = PAY_LOAD_TYPE_PCMA;
    }

    // �豸���������ݣ�������������������֮ǰ�Ĳ�һ������ܾ����
    if (0 < unCodeHandleSize && CAudioObj::m_stcAudioType != unAudioType)
    {
        BP_RUN_LOG_ERR(iRet,"Start Voice Broadcast", "audio pay load type conflict, current-type:%d, requet-type:%d", 
            CAudioObj::m_stcAudioType, unAudioType);
        return IVS_SDK_VOICE_ERR_AUDIO_TYPE_CONFLICT;
    }

    CAudioObj::m_stcAudioType = unAudioType;
  
    iRet = IVS_PLAY_SetVoiceDecoderType(unPayLoadType);

    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"Start Voice Broadcast", "Set VoiceDecoder Type.");
        return iRet;
    }
    // end add by wanglei 00165153:2013.5.3

    memset(pRtspInfo, 0, sizeof(BROADCAST_RTSP_INFO));
    AddCodeHandle(pCameraCode, pRtspInfo);

	iRet = IVS_FAIL;
	//�ж��Ƿ��Ѿ������㲥���������ô��Ӵ˾�ͷ��Ҫ�����㲥������ֻ����뾵ͷ�б�
	if(m_bIsBroadcastOpenedFlag)
	{
		//���������㲥�ӿ� ��������ɹ������ں����ڲ�������Ӧ�豸��handleֵ
		iRet = StartSingleBroadcast(m_iBroadcastType, pCameraCode,
                                    m_fileName,m_iFileBroadcastCycle);
		if(IVS_SUCCEED != iRet)//�������ʧ��ֱ�ӷ���
		{
            // modify by wanglei 00165153:2013.4.26 δ�ɹ������㲥����Ҫɾ��֮ǰԤ�ӵ��豸
            DeleteCodeHandle(pCameraCode); 
			return iRet;
		}	
	}

	return IVS_SUCCEED;
}

//ɾ���㲥��ͷ
int CAudioObj::DeleteBroadcastDevice(const IVS_CHAR* pCameraCode)
{
	if(NULL == pCameraCode)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "AudioObj DeleteBroadcastDevice",
            "CAudioObj::DeleteBroadcastDevice NULL == pCameraCode");
		return IVS_PARA_INVALID;
	}

	int strVoiceCodeLen = (int)strlen(pCameraCode);
	if(strVoiceCodeLen<= 0 || strVoiceCodeLen>IVS_DEV_CODE_LEN)
	{
		return IVS_PARA_INVALID;
	}
    
	BROADCAST_RTSP_INFO* pRtspInfo = GetHandleByCode(pCameraCode);
	if ( NULL == pRtspInfo )
	{
		return IVS_SDK_VOICE_ERR_DEV_NOT_IN;//ɾ�����豸���ڹ㲥�б���
	}

	//�ж��Ƿ��Ѿ������㲥���������ôɾ���˾�ͷ��Ҫֹͣ��㲥������ֻɾ��
	if(m_bIsBroadcastOpenedFlag)
	{
		//����ֹͣ��·�㲥�ӿ�
		int iRet = IVS_PLAY_StopVoiceBroadcast((IVS_CHAR*)pRtspInfo->ServiceIP,
                                               pRtspInfo->ServicePort);
		if ( IVS_SUCCEED != iRet )
		{
			return IVS_SDK_VOICE_ERR_STOP_BROADCAST_FAIL;//ʧ��ֱ�ӷ�����󣬲�ɾ���豸�б��еĶ�Ӧ��
		}

        // modify by wanglei 00165153:2013.4.26 ������ֹͣ���ٴν��й㲥MU�ͻ᷵��ʧ��
        iRet += StopPlay(pRtspInfo); // ����RTSPֹͣ�㲥
	}

	//���б���ɾ��
	DeleteCodeHandle(pCameraCode);
	return IVS_SUCCEED;
}
	
//��ʼ�����㲥
int CAudioObj::StartSingleBroadcast(int ulBroadcastType,
                                    const IVS_CHAR* pCameraCode,
                                    const IVS_CHAR* fileName,
                                    int iCycle)
{
    if (NULL == pCameraCode)
    {
        return IVS_PARA_INVALID;
    }

    EnterCriticalSection(&m_csCodeHandleLock);
    if ( MAX_BROADCAST_COUNT-1 < m_CodeHandle.size() )
    {
        LeaveCriticalSection(&m_csCodeHandleLock);
        return IVS_SDK_VOICE_ERR_TOO_MUCH_DEVICES;
    }
    LeaveCriticalSection(&m_csCodeHandleLock);

    CLockGuard lock(m_pMediaMutex);

    IVS_MEDIA_PARA stMediaPara;
    memset(&stMediaPara, 0, sizeof(IVS_MEDIA_PARA));
    stMediaPara.ProtocolType = PROTOCOL_RTP_OVER_UDP;

    // ����Ĭ��ֵ;
    stMediaPara.DecoderCompany = VENDOR_HUAWEI;
    stMediaPara.StreamType = STREAM_TYPE_UNSPECIFIED;
    stMediaPara.TransMode = MEDIA_TRANS;
    stMediaPara.BroadCastType = BROADCAST_UNICAST;
    stMediaPara.VideoDecType = VIDEO_DEC_H264;
    stMediaPara.AudioDecType = AUDIO_DEC_G711U;

    START_PLAY_PARAM stParam;
    stParam.enServiceType = SERVICE_TYPE_AUDIO_BROADCAST;
    stParam.cbRtspDataCallBack = g_fRtspDataCallBack;
    stParam.cbNetDataCallBack = g_fNetDataCallBack;
    //��ʼ�����Խ�;
    int iRet = StartPlay(&stMediaPara, pCameraCode, stParam);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"StartSingleBroadcast", "Start play error.");
        return iRet;
    }

    // delete by wanglei 00165153:2013.5.3 ��������ʱ���Ѿ���������Ƶ����һ���Լ��DTS2013040903851 ��
    // ����������м�⣬���н�������

    // add for ���������ͷ����Ƶ��ʽ�Ƿ�͵�һ·��ͬ
    //iRet = CheckAudioType();
    //if (IVS_SUCCEED != iRet)
    //{
    //    (void)CMediaBase::StopPlay();
    //    return iRet;
    //}
    // end add

    iRet = IVS_PLAY_StartVoiceBroadcast(m_strLocalIp.c_str(),
                                        (IVS_ULONG)m_uLocalPort,
                                        (IVS_CHAR*)m_szRemoteIp,
                                        (IVS_ULONG)m_uRemotePort,
                                        (IVS_ULONG)ulBroadcastType,
                                        fileName, //lint !e1773
                                        (bool)iCycle); //lint !e1786

    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"Start Voice Broadcast",
                       "Start Voice Broadcast fail with %d.", iRet);
        (void)CMediaBase::StopPlay();

        // modofy by wanglei 00165153:2013.6.13 DTS2013060904823
        // ������Ҳ�����Ƶ�豸�Ĵ���ԭ�򣬾��к�ǿ�������ԣ���Ҫ���ظô�����
        return IVS_PLAYER_RET_AUDIO_DEVICE_NOT_FIND == iRet ? IVS_PLAYER_RET_AUDIO_DEVICE_NOT_FIND : IVS_SDK_VOICE_ERR_CALL_MLIB_FAIL;
    }

    BROADCAST_RTSP_INFO *pRtspInfo = NULL;
    pRtspInfo = GetHandleByCode(pCameraCode);

    if (NULL == pRtspInfo)
    {
        BP_RUN_LOG_ERR(iRet,"Start Voice Broadcast",
            "NEW BROADCAST RTSP INFO error.");
        (void)IVS_PLAY_StopVoiceBroadcast((IVS_CHAR*)m_szRemoteIp,
            (IVS_ULONG)m_uRemotePort);
        (void)CMediaBase::StopPlay();
        return IVS_FAIL;
    }

    (void)CToolsHelp::Strncpy(pRtspInfo->szUrl,
                              IVS_URL_LEN,
                              m_stMediaRsp.szURL,
                              strlen(m_stMediaRsp.szURL));

    (void)CToolsHelp::Strncpy(pRtspInfo->ServiceIP,
                              IVS_IP_LEN,
                              m_szRemoteIp,
                              strlen(m_szRemoteIp));

    //int iPort = CToolsHelp::StrToInt(m_stMediaRsp.szRTSPPORT);
    pRtspInfo->ServicePort = m_uRemotePort;

    pRtspInfo->RtspHandle = (ULONG)m_iRtspHandle;
    pRtspInfo->PlayHandle = m_ulPlayerChannel;
    pRtspInfo->NetSourceHandle = m_ulNetSourceChannel;

    //��ʱ����AddCodeHandle�����޸�pVoiceCode��Ӧ��handleֵ
    //AddCodeHandle(pCameraCode, pRtspInfo);
    return IVS_SUCCEED;
}//lint !e818

//��ʼ�����㲥
int CAudioObj::StartRealBroadcast()
{
	//�ж��Ƿ��ѿ����㲥�����򷵻�
	if(m_bIsBroadcastOpenedFlag)
	{
		return IVS_SDK_BROADCASE_ERR_ALREADY_OPENED;
	}

	//����ý�����
	//memcpy(&m_mediaPara,pMediaPara,sizeof(IVS_MEDIA_PARA));

	int iRet = IVS_FAIL;
	m_iBroadcastType = VOICE_REAL_BROADCAST;

	std::string pCameraCode;

	//ѭ�������豸�б������㲥
	EnterCriticalSection(&m_csCodeHandleLock);

	int codesSize = (int)m_CodeHandle.size();
	if(codesSize <= 0)
	{
		LeaveCriticalSection(&m_csCodeHandleLock);
		return IVS_SDK_BROADCASE_ERR_NO_DEVICE;
	}

	//ȫ������ʧ����Ҫ���ش���
	bool bIsAnySucceed = false;
    m_ulHandle = (unsigned long)this;
	//���濪ʼ�����豸�鲢һһ�����㲥
	CODE_HANDLE_ITER iter = m_CodeHandle.begin();
	for( ;iter != m_CodeHandle.end();iter++)
	{
		pCameraCode = iter->first;
		iRet = StartSingleBroadcast(m_iBroadcastType, pCameraCode.c_str());
		if(IVS_SUCCEED != iRet)//���ĳ·����ʧ�ܣ��������������������豸
		{
			//д��־
			BP_RUN_LOG_ERR(IVS_SDK_VOICE_ERR_ALL_BROADCAST_FAIL, "AudioObj StartRealBroadcast",
                "CAudioObj::StartRealBroadcast StartSingleBroadcast failed iRet:%d pCameraCode:%s",
                iRet, pCameraCode.c_str());
			
			//�ϱ��澯
			EventCallBack fnCallBack = m_pUserMgr->GetEventCallBack();
			if (NULL == fnCallBack)
			{
				BP_RUN_LOG_ERR(IVS_SDK_VOICE_ERR_CANNOT_REPORT_FAIL, "AudioObj StartRealBroadcast",
                    "CAudioObj::StartRealBroadcast NULL == fnCallBack");
				LeaveCriticalSection(&m_csCodeHandleLock);
				return IVS_SDK_VOICE_ERR_CANNOT_REPORT_FAIL;
			}

            STREAM_EXCEPTION_INFO exceptionInfo;
            memset(&exceptionInfo, 0, sizeof(exceptionInfo));
            exceptionInfo.iSessionID = m_pUserMgr->GetSessionID();
            exceptionInfo.ulHandle = m_ulHandle;
           
            strncpy(exceptionInfo.szDeviceCode, pCameraCode.c_str(), IVS_DEV_CODE_LEN - 1);
            exceptionInfo.iExceptionCode = IVS_EVENT_BROADCAST_FAILED;

			fnCallBack(IVS_EVENT_BROADCAST_FAILED, &exceptionInfo,
                sizeof(exceptionInfo), m_pUserMgr->GetEventUserData());
		}
		else
		{
			bIsAnySucceed = true;//ֻҪ��һ·�����ɹ��ͷ��سɹ�
		}
		
	}
	LeaveCriticalSection(&m_csCodeHandleLock);
	
	if(bIsAnySucceed)
	{
		m_bIsBroadcastOpenedFlag = true;
		return IVS_SUCCEED;
	}
	else
	{
        // modofy by wanglei 00165153:2013.6.13 DTS2013060904823
        // ������Ҳ�����Ƶ�豸�Ĵ���ԭ�򣬾��к�ǿ�������ԣ���Ҫ���ظô�����
        return IVS_PLAYER_RET_AUDIO_DEVICE_NOT_FIND == iRet ? IVS_PLAYER_RET_AUDIO_DEVICE_NOT_FIND : IVS_SDK_VOICE_ERR_ALL_BROADCAST_FAIL;
		// return IVS_SDK_VOICE_ERR_ALL_BROADCAST_FAIL;
	}
    
}

//ֹͣ��·�����㲥
int CAudioObj::StopSingleVoiceBroadcast(const BROADCAST_RTSP_INFO* pRtspInfo)
{
    if (NULL == pRtspInfo)
    {
        return IVS_PARA_INVALID;
    }

	int iRet =  IVS_PLAY_StopVoiceBroadcast((IVS_CHAR*)pRtspInfo->ServiceIP,
                                            pRtspInfo->ServicePort);

	if ( IVS_SUCCEED != iRet )
	{
		return IVS_SDK_VOICE_ERR_CALL_MLIB_FAIL;
	}

    iRet += StopPlay(pRtspInfo);
	
	//ֹͣ�㲥ʱ��Ҫɾ���豸�������û����е���ɾ���ӿ�ɾ��
	//DeleteCodeHandle(pCameraCode);
	
	return iRet;
}//lint !e1762

int CAudioObj::StopPlay(const BROADCAST_RTSP_INFO* pRtspInfo) const
{
    if (NULL == pRtspInfo)
    {
        return IVS_PARA_INVALID;
    }

    int iRet = RTSP_CLIENT_SendTeardownMsg((int)pRtspInfo->RtspHandle,
                                           pRtspInfo->szUrl,
                                           RTSP_CLIENT_TIMEOUT);
    if (RET_OK != iRet)
    {
        BP_RUN_LOG_ERR(IVS_PLAYER_RET_RTSP_SEND_TEARDOWN_FAIL,"Stop Play",
                    "call RTSP_CLIENT_SendTeardownMsg failed, ret = %d", iRet);
    }

    iRet += IVS_NETSOURCE_CloseNetStream(pRtspInfo->NetSourceHandle);

    iRet += IVS_PLAY_CloseStream(pRtspInfo->PlayHandle);

    iRet += IVS_PLAY_StopPlay(pRtspInfo->PlayHandle);

    iRet += IVS_NETSOURCE_FreeChannel(pRtspInfo->NetSourceHandle);


    iRet += IVS_PLAY_FreeChannel(pRtspInfo->PlayHandle);
    if (IVS_SUCCEED != iRet) //lint !e774 !e831
    {
        BP_RUN_LOG_ERR(iRet,"Stop Play", "call ivs_player stop play failed, ret = %d", iRet);
    }

    iRet = RTSP_CLIENT_FreeHandle((int)pRtspInfo->RtspHandle);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"Stop Play", "call rtspclient free handle failed, ret = %d", iRet);
    }

    return IVS_SUCCEED;
}

//ֹͣ�����㲥 ����൱��ֹͣȫ���㲥 
int CAudioObj::StopRealBroadcast()
{
	//std::vector<std::string> strVector;
	//�ж��Ƿ��ѿ����㲥�������򷵻�
	if(!m_bIsBroadcastOpenedFlag)
	{
		return IVS_SDK_BROADCASE_ERR_NOT_OPENED;
	}

	EnterCriticalSection(&m_csCodeHandleLock);

	int codesSize = (int)m_CodeHandle.size();
	if(codesSize <= 0)
	{
		LeaveCriticalSection(&m_csCodeHandleLock);
		m_bIsBroadcastOpenedFlag = false;
		return IVS_SUCCEED;//�㲥����û���豸ʱ���سɹ�!
	}

    //ȫ��ֹͣʧ����Ҫ���ش���
    bool bIsAnySucceed = false;
    int iRet = IVS_FAIL;
    std::string strCameraCode;

	CODE_HANDLE_ITER iter = m_CodeHandle.begin();
	for (; iter != m_CodeHandle.end(); ++iter)
	{
        strCameraCode = (iter->first);

        iRet = StopSingleVoiceBroadcast(dynamic_cast<BROADCAST_RTSP_INFO*>(iter->second));
        if(IVS_SUCCEED != iRet)//һ·ֹͣʧ�ܲ���������ֹͣ����
        {
            //д��־
            BP_RUN_LOG_ERR(IVS_SDK_VOICE_ERR_ALL_BROADCAST_FAIL, "AudioObj StopRealBroadcast",
                "CAudioObj::StopRealBroadcast StopSingleVoiceBroadcast failed. pCameraCode:%s",
                strCameraCode.c_str());

            //�ϱ��澯
            EventCallBack fnCallBack = m_pUserMgr->GetEventCallBack();
            if (NULL == fnCallBack)
            {
				LeaveCriticalSection(&m_csCodeHandleLock);
                BP_RUN_LOG_ERR(IVS_SDK_VOICE_ERR_CANNOT_REPORT_FAIL,
                    "AudioObj StopRealBroadcast",
                    "CAudioObj::StopRealBroadcast NULL == fnCallBack");
                return IVS_SDK_VOICE_ERR_CANNOT_REPORT_FAIL;
            }

            //STREAM_EXCEPTION_INFO streamExceptionInfo;
            //memset(&streamExceptionInfo, 0, sizeof(STREAM_EXCEPTION_INFO));
            //streamExceptionInfo.iSessionID = m_pUserMgr->GetSessionID();
            //streamExceptionInfo.ulHandle = m_ulHandle;
            //streamExceptionInfo.iExceptionCode = IVS_EVENT_BROADCAST_FAILED;

            //fnCallBack(IVS_EVENT_BROADCAST_FAILED, &streamExceptionInfo,
            //    sizeof(STREAM_EXCEPTION_INFO), m_pUserMgr->GetEventUserData());

            // edit by wanglei 00165153:2013.8.24 �㲥�쳣���ϱ�
            //STREAM_EXCEPTION_INFO exceptionInfo;
            //memset(&exceptionInfo, 0, sizeof(exceptionInfo));
            //exceptionInfo.iSessionID = m_pUserMgr->GetSessionID();
            //exceptionInfo.ulHandle = m_ulHandle;

            //strncpy(exceptionInfo.szDeviceCode, strCameraCode.c_str(), IVS_DEV_CODE_LEN - 1);
            //exceptionInfo.iExceptionCode = IVS_EVENT_BROADCAST_FAILED;

            //fnCallBack(IVS_EVENT_BROADCAST_FAILED, &exceptionInfo,
            //    sizeof(exceptionInfo), m_pUserMgr->GetEventUserData());

        }
        else
        {
            bIsAnySucceed = true;//ֻҪ��һ·ֹͣ�ɹ��ͷ��سɹ�
        }
	}

	LeaveCriticalSection(&m_csCodeHandleLock);

	m_iBroadcastType = -1;
	if(bIsAnySucceed)
	{
		m_bIsBroadcastOpenedFlag = false;
		return IVS_SUCCEED;
	}
	else
	{
		return IVS_SDK_VOICE_ERR_ALL_BROADCAST_FAIL;
	}
}

int CAudioObj::CheckFileExt(const char*pFileName)
{
	//ֻ֧��wav��ʽ����׺���ȹ̶�Ϊ3
	const int fileExtLen = 3;

	int fileNameLen = (int)strlen(pFileName);

	//���ݳ���
	int tempFileNameLen = fileNameLen;

	while(tempFileNameLen>0 && pFileName[tempFileNameLen-1]!='.')
	{
		tempFileNameLen--;
	}
	if(tempFileNameLen<=0)//�ļ����쳣
	{
		return IVS_PARA_INVALID;
	}

	if(fileNameLen-tempFileNameLen != fileExtLen)
	{
		return IVS_SDK_VOICE_ERR_FILE_NOT_RIGHT_TYPE;
	}

	char fileNameExt[fileExtLen+1] = {0};
	memcpy(fileNameExt,&pFileName[tempFileNameLen],fileExtLen);
	
    if(0 != _stricmp(fileNameExt, "wav")
       && 0 != _stricmp(fileNameExt, "mp3"))
    {
        return IVS_SDK_VOICE_ERR_FILE_NOT_RIGHT_TYPE;
    }

	return IVS_SUCCEED;
}//lint !e1762

//��ʼ�ļ������㲥
int CAudioObj::StartFileBroadcast(const IVS_CHAR* pFileName,IVS_INT32 iCycle)
{
	//�ж��Ƿ��ѿ����㲥�����򷵻�
	if(m_bIsBroadcastOpenedFlag)
	{
		return IVS_SDK_BROADCASE_ERR_ALREADY_OPENED;
	}

	if(NULL == pFileName)
	{
		return IVS_PARA_INVALID;
	}

	int fileNameLen = (int)strlen(pFileName);
	if(fileNameLen<=0 || fileNameLen>FILE_NAME_LEN)
	{
		return IVS_PARA_INVALID;
	}

	//�㲥�ļ��������ж�
	int iRet = _access(pFileName,0);
	if (iRet)
	{
		return IVS_SDK_VOICE_ERR_FILE_NOT_EXIST;
	}

	//�㲥�ļ���׺�жϣ�Ŀǰֻ֧��.wav��ʽ
	iRet = CheckFileExt(pFileName);
	if (iRet)
	{
		return iRet;
	}
	
	//����ý�����
//	memcpy(&m_mediaPara,pMediaPara,sizeof(IVS_MEDIA_PARA));
	//�����ļ���
	memcpy(m_fileName,pFileName,(unsigned long)fileNameLen);
	//�����ļ��㲥��ʽ
	m_iFileBroadcastCycle = iCycle;

	iRet = IVS_FAIL;
	std::string pCameraCode;

	//ȫ������ʧ����Ҫ���ش���
	bool bIsAnySucceed = false;

	//ѭ�������豸�б������㲥
	EnterCriticalSection(&m_csCodeHandleLock);
	
	int codesSize = (int)m_CodeHandle.size();
	if(codesSize <= 0)
	{
		LeaveCriticalSection(&m_csCodeHandleLock);
		return IVS_SDK_BROADCASE_ERR_NO_DEVICE;
	}

	m_iBroadcastType = VOICE_FILE_BROADCAST;
    m_ulHandle = (unsigned long)this;
	CODE_HANDLE_ITER iter = m_CodeHandle.begin();
	for( ;iter != m_CodeHandle.end();iter++)
	{
		pCameraCode = iter->first;
		iRet = StartSingleBroadcast(m_iBroadcastType, pCameraCode.c_str(),
                                    pFileName, iCycle);
		if(IVS_SUCCEED != iRet)
		{
			//д��־
			BP_RUN_LOG_ERR(iRet, "AudioObj StartFileBroadcast",
                "CAudioObj::StartFileBroadcast StartSingleBroadcast failed iRet:%d pCameraCode:%s",
                iRet,pCameraCode.c_str());
			//�ϱ��澯
			EventCallBack fnCallBack = m_pUserMgr->GetEventCallBack();
			if (NULL == fnCallBack)
			{
				BP_RUN_LOG_ERR(iRet, "Start File Broadcast","CAudioObj::StartFileBroadcast NULL == fnCallBack");
				return IVS_SDK_VOICE_ERR_CANNOT_REPORT_FAIL;
			}
			
			//BROADCAST_FAILED_INFO broadcastFailedInfo;
			//memset(&broadcastFailedInfo,0,sizeof(BROADCAST_FAILED_INFO));

			//broadcastFailedInfo.iSessionID = m_pUserMgr->GetSessionID();
			//int voiceCodeLen = (int)strlen(pCameraCode.c_str());
			//voiceCodeLen = voiceCodeLen > IVS_DEV_CODE_LEN ? IVS_DEV_CODE_LEN : voiceCodeLen;
			//memcpy(broadcastFailedInfo.cVoiceCode,pCameraCode.c_str(),(unsigned long)voiceCodeLen);

			//fnCallBack(IVS_EVENT_BROADCAST_FAILED,&broadcastFailedInfo,sizeof(BROADCAST_FAILED_INFO),m_pUserMgr->GetEventUserData());

            // edit by wanglei 00165153:2013.8.24 �㲥�쳣��ͳһ��STREAM_EXCEPTION_INFO�ṹ��

            STREAM_EXCEPTION_INFO exceptionInfo;
            memset(&exceptionInfo, 0, sizeof(exceptionInfo));
            exceptionInfo.iSessionID = m_pUserMgr->GetSessionID();
            exceptionInfo.ulHandle = m_ulHandle;

            strncpy(exceptionInfo.szDeviceCode, pCameraCode.c_str(), IVS_DEV_CODE_LEN - 1);
            exceptionInfo.iExceptionCode = IVS_EVENT_BROADCAST_FAILED;

            fnCallBack(IVS_EVENT_BROADCAST_FAILED, &exceptionInfo,
                sizeof(exceptionInfo), m_pUserMgr->GetEventUserData());


		}
		else
		{
			bIsAnySucceed = true;//ֻҪ��һ·�����ɹ��ͷ��سɹ�
		}
	}
	LeaveCriticalSection(&m_csCodeHandleLock);

	
	
    if(bIsAnySucceed)
	{
		m_bIsBroadcastOpenedFlag = true;
		return IVS_SUCCEED;
	}
	else
	{
		return IVS_SDK_VOICE_ERR_ALL_BROADCAST_FAIL;
	}
}

//ֹͣ�ļ������㲥
int CAudioObj::StopFileBroadcast()
{
	std::vector<std::string> strVector;

	//�ж��Ƿ��ѿ����������򷵻�
	if(!m_bIsBroadcastOpenedFlag)
	{
		return IVS_SDK_BROADCASE_ERR_NOT_OPENED;
	}

	EnterCriticalSection(&m_csCodeHandleLock);

	int codesSize = (int)m_CodeHandle.size();
	if(codesSize <= 0)
	{
		LeaveCriticalSection(&m_csCodeHandleLock);
		m_bIsBroadcastOpenedFlag = false;
		return IVS_SUCCEED;//�㲥����û���豸ʱ���سɹ�!
	}

    bool bIsAnySucceed = false;
    std::string strCameraCode;

	CODE_HANDLE_ITER iter = m_CodeHandle.begin();
	for (; iter != m_CodeHandle.end(); ++iter)
	{
		strCameraCode = (iter->first);
        int iRet = StopSingleVoiceBroadcast(dynamic_cast<BROADCAST_RTSP_INFO*>(iter->second));
        if(IVS_SUCCEED != iRet)
        {
            //д��־
            BP_RUN_LOG_ERR(iRet, "","CAudioObj::StopFileBroadcast StopSingleVoiceBroadcast failed. VoiceCode:%s",
                           strCameraCode.c_str());
            //�ϱ��澯
            EventCallBack fnCallBack = m_pUserMgr->GetEventCallBack();
            if (NULL == fnCallBack)
            {
                BP_RUN_LOG_ERR(iRet, "","CAudioObj::StopFileBroadcast NULL == fnCallBack");
				LeaveCriticalSection(&m_csCodeHandleLock);
                return IVS_SDK_VOICE_ERR_CANNOT_REPORT_FAIL;
            }
            
            // edit by wanglei 00165153:2013.8.24 �㲥�ر�ʧ�ܺ��ϱ�
            //BROADCAST_FAILED_INFO broadcastFailedInfo;
            //memset(&broadcastFailedInfo,0,sizeof(BROADCAST_FAILED_INFO));

            //broadcastFailedInfo.iSessionID = m_pUserMgr->GetSessionID();
            //int voiceCodeLen = (int)strlen(strCameraCode.c_str());
            //voiceCodeLen = voiceCodeLen > IVS_DEV_CODE_LEN ? IVS_DEV_CODE_LEN : voiceCodeLen;
            //memcpy(broadcastFailedInfo.cVoiceCode,strCameraCode.c_str(),(unsigned long)voiceCodeLen);

            //fnCallBack(IVS_EVENT_BROADCAST_FAILED,&broadcastFailedInfo,sizeof(BROADCAST_FAILED_INFO),m_pUserMgr->GetEventUserData());

            //STREAM_EXCEPTION_INFO exceptionInfo;
            //memset(&exceptionInfo, 0, sizeof(exceptionInfo));
            //exceptionInfo.iSessionID = m_pUserMgr->GetSessionID();
            //exceptionInfo.ulHandle = m_ulHandle;

            //strncpy(exceptionInfo.szDeviceCode, strCameraCode.c_str(), IVS_DEV_CODE_LEN - 1);
            //exceptionInfo.iExceptionCode = IVS_EVENT_BROADCAST_FAILED;

            //fnCallBack(IVS_EVENT_BROADCAST_FAILED, &exceptionInfo,
            //    sizeof(exceptionInfo), m_pUserMgr->GetEventUserData());
        }
        else
        {
            bIsAnySucceed = true;//ֻҪ��һ·ֹͣ�ɹ��ͷ��سɹ�
        }
	}
	LeaveCriticalSection(&m_csCodeHandleLock);
	
	m_iBroadcastType = -1;
	if(bIsAnySucceed)
	{
		m_bIsBroadcastOpenedFlag = false;
		return IVS_SUCCEED;
	}
	else
	{
		return IVS_SDK_VOICE_ERR_ALL_BROADCAST_FAIL;
	}
}

int CAudioObj::StartPlay(const IVS_MEDIA_PARA* pstMediaPara, 
                         const char* pCameraCode, 
                         const START_PLAY_PARAM& stParam)
{
    //У�����
    if (NULL == pCameraCode || strlen(pCameraCode) > (CODE_LEN-1) || 0 == strlen(pCameraCode))
    {
        BP_RUN_LOG_ERR(IVS_SMU_CAMERA_CODE_NOT_EXIST,"Start Talk Back", "Voice code invalid");
        return IVS_PARA_INVALID;
    }

    //���ò���
    bool bRet = CToolsHelp::Strncpy(m_szCameraID, sizeof(m_szCameraID),
                                    pCameraCode, strlen(pCameraCode));

    if(false == bRet)
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"Start Talk Back", "Strncpy error.");
        return IVS_ALLOC_MEMORY_ERROR;
    }

    int iRet = SetMediaParam(pstMediaPara);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"Start Talk Back", "set media param error");
        return iRet;
    }

    //��SCU��ȡý��URL���������ݱ����ڻ���.
    iRet = DoGetUrl(stParam.enServiceType);
    if (IVS_SUCCEED != iRet) 
    {
        BP_RUN_LOG_ERR(iRet,"Start Talk Back", "get url play back error, stream default.");
        return iRet;
    }

    // ���û�����������;
    iRet = CMediaBase::StartPlay(stParam);
    if (iRet != IVS_SUCCEED)
    {
        //m_fSpeed = 0.0;
        BP_RUN_LOG_ERR(iRet,"Start Play Back", "call base class failed.");
        return iRet;
    }

    return iRet;
}

int CAudioObj::DoGetUrl(IVS_SERVICE_TYPE enServiceType)
{
    //������Ϣ;
    memset(&m_stUrlMediaInfo, 0, sizeof(ST_URL_MEDIA_INFO));

    //TODO : szDstPort��ʵ�����ã���һ���̶�ֵ�����Э������szDstPP��νṹ��δ����
    m_stUrlMediaInfo.ServiceType = enServiceType;

    //std::string strCameraDomainCode = m_szCameraID;
    std::string strCameraCode;
    std::string strDomainCode;
    int iRet = CToolsHelp::SplitCameraDomainCode(m_szCameraID, strCameraCode, strDomainCode);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"Do Get Url Talkback", "CToolsHelp::SplitCameraCode failed, RetCode=%d.", iRet);
        return iRet;
    }

    bool bRet = CToolsHelp::Strncpy(m_stUrlMediaInfo.szSrcCode, sizeof(m_stUrlMediaInfo.szSrcCode), strCameraCode.c_str(), strCameraCode.size());//lint !e534
    if(false == bRet)
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"Do Get Url Talkback", "Strncpy error.");
        return IVS_ALLOC_MEMORY_ERROR;
    }
    bRet = CToolsHelp::Strncpy(m_stUrlMediaInfo.cDomainCode,IVS_DOMAIN_CODE_LEN+1,  strDomainCode.c_str(), strDomainCode.size());//lint !e534
    if(false == bRet)
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"Do Get Url Talkback", "Strncpy error.");
        return IVS_ALLOC_MEMORY_ERROR;
    }
    bRet = CToolsHelp::Strncpy(m_stUrlMediaInfo.szDstCode,sizeof(m_stUrlMediaInfo.szDstCode), "01",strlen("01"));//lint !e534
    if(false == bRet)
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"Do Get Url Talkback", "Strncpy error.");
        return IVS_ALLOC_MEMORY_ERROR;
    }
    m_stUrlMediaInfo.MediaPara.StreamType = m_stuMediaParam.StreamType;
    m_stUrlMediaInfo.MediaPara.ProtocolType = m_stuMediaParam.ProtocolType;
    m_stUrlMediaInfo.PackProtocolType = PACK_PROTOCOL_ES;
    m_stUrlMediaInfo.MediaPara.BroadCastType = m_stuMediaParam.BroadCastType;
    bRet = CToolsHelp::Strncpy(m_stUrlMediaInfo.szDstIP,sizeof(m_stUrlMediaInfo.szDstIP), "",strlen(""));//lint !e534
    if(false == bRet)
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"Do Get Url Talkback", "Strncpy error.");
        return IVS_ALLOC_MEMORY_ERROR;
    }
    bRet = CToolsHelp::Strncpy(m_stUrlMediaInfo.szDstPort,sizeof(m_stUrlMediaInfo.szDstPort), "35016",strlen("35016"));//lint !e534
    if(false == bRet)
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"Do Get Url Talkback", "Strncpy error.");
        return IVS_ALLOC_MEMORY_ERROR;
    }

    m_stUrlMediaInfo.MediaPara.VideoDecType = m_stuMediaParam.VideoDecType;
    m_stUrlMediaInfo.MediaPara.AudioDecType = m_stuMediaParam.AudioDecType;
    m_stUrlMediaInfo.MediaPara.TransMode = m_stuMediaParam.TransMode;

    //��ȡý��URL
    iRet = GetURL(CToolsHelp::GetSCCHead().c_str(),m_pUserMgr, m_stUrlMediaInfo, m_stMediaRsp, m_bAssociatedAudio);//lint !e838
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(IVS_SDK_VOICE_ERR_GET_URL_FAIL, "Do Get Url Talkback",
                     "Start Voice Talkback SCC get URL failed with %d!", iRet);
        return iRet;
    }

    return iRet;
}

 // add by wanglei 00165153:2013.5.3 ���ڻ�ȡָ�������֧�ֵ���Ƶ�����ʽ
int CAudioObj::GetSupportAudioType(const char* pszCameraCode, unsigned int& unAudioType)
{
    // ͨ������������ȡVoiceOutCode
    IVS_CAMERA_CFG stCameraCfg;
    memset(&stCameraCfg, 0, sizeof(stCameraCfg));

    int iRet = m_pUserMgr->GetDeviceParaConfig().GetDeviceConfig(pszCameraCode,
        CONFIG_CAMERA_CFG,
        &stCameraCfg,
        sizeof(stCameraCfg));
    if (IVS_SUCCEED  != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Get Support Audio Type",
            "Get Device Config CONFIG_CAMERA_CFG failed with %d!", iRet);
        return iRet;
    }

    // ͨ��cVoiceOutCode��ѯ֧�ֵ���Ƶ��ʽ
    std::string strAudioOutID = stCameraCfg.cVoiceOutCode;

    IVS_AUDIO_CFG stAudioCfg;
    memset(&stAudioCfg, 0, sizeof(stAudioCfg));

    iRet = m_pUserMgr->GetDeviceParaConfig().GetAudioDeviceConfig(
        pszCameraCode,
        strAudioOutID.c_str(),
        CONFIG_CAMERA_AUDIO_CFG,
        &stAudioCfg,
        sizeof(stAudioCfg));
    if (IVS_SUCCEED  != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Get Support Audio Type",
            "Get Audio Device Config CONFIG_CAMERA_AUDIO_CFG failed with %d!", iRet);
        return iRet;
    }

    if (1 != stAudioCfg.uiChannelType)
    {
        return IVS_FAIL;
    }

    unAudioType = stAudioCfg.stAudioParam.uiDataFormat;
    return IVS_SUCCEED;
}

// ������SEȷ�ϣ������޷�ֱ��ͨ������������ȡ��Ƶ���֧�ֵĸ�ʽ��Ҫ��������
// 1. SDK�ӿ�IVS_SDK_GetDeviceConfig(��ѯ�������������,���������)�����õ� VoiceOutCode
// 2. ����VoiceOutCode SDK�ӿ�IVS_SDK_GetDeviceConfig(��ѯ�������豸��������,VoiceOutCode)�����õ��������豸�ı�������������G711A��G711U
int CAudioObj::GetSupportAudioType(unsigned int &uAudioType)
{
    // ͨ������������ȡVoiceOutCode
    IVS_CAMERA_CFG stCameraCfg;
    memset(&stCameraCfg, 0, sizeof(stCameraCfg));

    int iRet = m_pUserMgr->GetDeviceParaConfig().GetDeviceConfig(m_szCameraID,
                                                        CONFIG_CAMERA_CFG,
                                                        &stCameraCfg,
                                                        sizeof(stCameraCfg));
    if (IVS_SUCCEED  != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Get Support Audio Type",
            "Get Device Config CONFIG_CAMERA_CFG failed with %d!", iRet);
        return iRet;
    }

    // ͨ��cVoiceOutCode��ѯ֧�ֵ���Ƶ��ʽ
	std::string strAudioOutID = stCameraCfg.cVoiceOutCode;

    IVS_AUDIO_CFG stAudioCfg;
    memset(&stAudioCfg, 0, sizeof(stAudioCfg));

    iRet = m_pUserMgr->GetDeviceParaConfig().GetAudioDeviceConfig(
		                                              m_szCameraID,
                                                      strAudioOutID.c_str(),
                                                      CONFIG_CAMERA_AUDIO_CFG,
                                                      &stAudioCfg,
                                                      sizeof(stAudioCfg));
    if (IVS_SUCCEED  != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Get Support Audio Type",
                    "Get Audio Device Config CONFIG_CAMERA_AUDIO_CFG failed with %d!", iRet);
        return iRet;
    }

    if (1 != stAudioCfg.uiChannelType)
    {
        return IVS_FAIL;
    }

    uAudioType = stAudioCfg.stAudioParam.uiDataFormat;
    return IVS_SUCCEED;
}


int CAudioObj::CheckAudioType()
{
    unsigned int uAudioType = 0;
    int iRet = GetSupportAudioType(uAudioType);

    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"Start Voice Broadcast",
            "Get Support AudioType error.");
        return iRet;
    }

    // modify by wanglei 00165153:2013.4.26 �����ǰû�豸, ������ˢ�±�������
    if (m_bIsBroadcastOpenedFlag && CAudioObj::m_stcAudioType != 0)
    {
        
        if (CAudioObj::m_stcAudioType != uAudioType)
        {
            BP_RUN_LOG_ERR(IVS_SDK_VOICE_ERR_AUDIO_TYPE_CONFLICT,
                "Start Voice Broadcast",
                "AudioType conflict %u : %u error.",
                CAudioObj::m_stcAudioType, uAudioType);
            return IVS_SDK_VOICE_ERR_AUDIO_TYPE_CONFLICT;
        }
    }
    else
    {
        IVS_ULONG ulVoiceType = 0;

        if (AUDIO_DEC_G711U == uAudioType)
        {
            ulVoiceType = PAY_LOAD_TYPE_PCMU;
        }
        else if (AUDIO_DEC_G711A == uAudioType)
        {
            ulVoiceType = PAY_LOAD_TYPE_PCMA;
        }

        iRet = IVS_PLAY_SetVoiceDecoderType(ulVoiceType);

        if (IVS_SUCCEED != iRet)
        {
            BP_RUN_LOG_ERR(iRet,"Start Voice Broadcast", "Set VoiceDecoder Type.");
            return iRet;
        }

        CAudioObj::m_stcAudioType = uAudioType;
    }

    return iRet;
}

bool CAudioObj::IsExistTalkBackRtspHandle(IVS_ULONG ulRtspHandle) const
{
	return ulRtspHandle == (IVS_ULONG)m_iRtspHandle ? true : false;
}

bool CAudioObj::IsExistBroadCastRtspHandle(IVS_ULONG ulRtspHandle)
{
     BP_RUN_LOG_ERR(11,"IsExistBroadCastRtspHandle",  "Param[%lu]", ulRtspHandle);
    // ��ӡmapֵ
    CODE_HANDLE_ITER iterBegin = m_CodeHandle.begin();
    CODE_HANDLE_ITER iterEnd = m_CodeHandle.end();
    int iIndex = 0;
    for (; iterBegin != iterEnd; iterBegin++)
    {
        BROADCAST_RTSP_INFO *pRtspInfoTmp = iterBegin->second;
        BP_RUN_LOG_ERR(11,"IsExistBroadCastRtspHandle", "Map[%d]. RtspHandle[%lu] NetSourceHandle[%lu]", iIndex, pRtspInfoTmp->RtspHandle, pRtspInfoTmp->NetSourceHandle);
        iIndex++;
    }


    CODE_HANDLE_ITER iter = m_CodeHandle.begin();
    for (; iter != m_CodeHandle.end(); ++iter)
    {
        BROADCAST_RTSP_INFO* pstBroadCastRtspInfo = iter->second;
        if (NULL == pstBroadCastRtspInfo)
        {
            continue;
        }

        if (ulRtspHandle == (IVS_ULONG)pstBroadCastRtspInfo->RtspHandle)
        {
            BP_RUN_LOG_ERR(11,"IsExistBroadCastRtspHandle", "RtspHandle Equal", ulRtspHandle);
            return true;
        }
    }
    BP_RUN_LOG_ERR(11,"IsExistBroadCastRtspHandle", "RtspHandle Different", ulRtspHandle);
    return false;
}

bool CAudioObj::IsExistTalkBackNetHandle(IVS_ULONG ulNetHandle) const
{
    return ulNetHandle == m_ulNetSourceChannel ? true : false;
}

bool CAudioObj::IsExistBroadCastNetHandle(IVS_ULONG ulNetHandle)
{
    CODE_HANDLE_ITER iter = m_CodeHandle.begin();
    for (; iter != m_CodeHandle.end(); ++iter)
    {
        BROADCAST_RTSP_INFO* pstBroadCastRtspInfo = iter->second;
        if (NULL == pstBroadCastRtspInfo)
        {
            continue;
        }

        if (ulNetHandle == pstBroadCastRtspInfo->NetSourceHandle)
        {
            return true;
        }
    }

    return false;
}

//int CAudioObj::GetBroadCastVoiceCodeByRtspHandle(IVS_ULONG ulRtspHandle, std::string& strVoiceCode)
//{
//	CODE_HANDLE_ITER iter = m_CodeHandle.begin();
//	for (; iter != m_CodeHandle.end(); ++iter)
//	{
//		BROADCAST_RTSP_INFO* pstBroadCastRtspInfo = iter->second;
//		if (NULL == pstBroadCastRtspInfo)
//		{
//			continue;
//		}
//
//		if (ulRtspHandle == pstBroadCastRtspInfo->RtspHandle)
//		{
//			strVoiceCode = iter->first;
//			return IVS_SUCCEED;
//		}
//	}
//
//	return IVS_FAIL;
//}

int CAudioObj::GetBroadCastDeviceCodeByConnHandle(IVS_ULONG ulNetHandle, std::string& strDeviceCode)
{
    CODE_HANDLE_ITER iter = m_CodeHandle.begin();
    for (; iter != m_CodeHandle.end(); ++iter)
    {
        BROADCAST_RTSP_INFO* pstBroadCastRtspInfo = iter->second;
        if (NULL == pstBroadCastRtspInfo)
        {
            continue;
        }

        if (ulNetHandle == pstBroadCastRtspInfo->RtspHandle || ulNetHandle == pstBroadCastRtspInfo->NetSourceHandle)
        {
            strDeviceCode = iter->first;
            return IVS_SUCCEED;
        }
    }

    return IVS_FAIL;
}



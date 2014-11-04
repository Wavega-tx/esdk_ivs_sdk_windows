#include "DecoderChannel.h"
#include "ToolsHelp.h"
#include "bp_log.h"
#include "vos.h"
#include "TVWallMgr.h"
#include "MediaBase.h"



CDecoderChannel::CDecoderChannel(void) : m_ulAsyncSeq(0)
										, m_uiChannel(0)
										, m_bReqStop(false) //lint !e1729
										, m_pTVWallMgr(NULL)
										, m_ulHandle(SDK_INVALID_HANDLE) //lint !e1729
                                        , m_wndStatus(WND_STATUS_IDLE)
{
	m_bStoping = false;
	HandlePlayException();
}

CDecoderChannel::CDecoderChannel(IVS_ULONG ulAsyncSeq, 
								const IVS_CHAR* pDecoderID, 
								IVS_UINT32 uiChannel, 
								CTVWallMgr* pTVWallMgr) 
								: m_ulAsyncSeq(ulAsyncSeq)
								, m_uiChannel(uiChannel)
								, m_bReqStop(false) //lint !e1729
								, m_pTVWallMgr(pTVWallMgr)
								, m_ulHandle(SDK_INVALID_HANDLE) //lint !e1729
                                , m_wndStatus(WND_STATUS_IDLE)
{
	m_bStoping = false;

    if(NULL!=pDecoderID)
    {
        m_strDecoderID = pDecoderID;
    }
	HandlePlayException();
}


CDecoderChannel::~CDecoderChannel(void)
{
}

IVS_INT32 CDecoderChannel::StartRealPlay(const IVS_CHAR* pCameraCode, 
                                            const IVS_REALPLAY_PARAM& stRealplayParam, 
                                            const IVS_TVWALL_PARAM& stTVWallParam)
{
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
    TVWALL_PLAY_PARAM stPlayParam = {0};
	stPlayParam.iPlayType = PLAY_TYPE_REAL;
	if(!CToolsHelp::Strncpy(stPlayParam.szCameraCode, IVS_DEV_CODE_LEN, pCameraCode, strlen(pCameraCode)))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Start Real Play","Copy CameraCode failed");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	stPlayParam.uiStreamType = stRealplayParam.uiStreamType;
	stPlayParam.uiProtocolType = stRealplayParam.uiProtocolType;
	stPlayParam.bDirectFirst = stRealplayParam.bDirectFirst;
	stPlayParam.bMultiCast = stRealplayParam.bMultiCast;

	for (int i = 0;i < 10; i++)
	{
		if (!m_bStoping)
		{
			break;
		}
		
		BP_DBG_LOG("CDecoderChannel::StartRealPlay", "AsyncSeq=%lu wait.", m_ulAsyncSeq);
		Sleep(10);
	}

    IVS_INT32 iRet = StartPlay(stPlayParam, stTVWallParam);
    return iRet;
}

IVS_INT32 CDecoderChannel::StartPlayBack(const IVS_CHAR* pCameraCode, 
                                                const IVS_PLAYBACK_PARAM& stPlaybackParam, 
                                                const IVS_TVWALL_PARAM& stTVWallParam, 
												REPLAY_TYPE iReplayType,
												const IVS_CHAR* pDomainCode,
												const IVS_CHAR* pNVRCode,
                                                const IVS_CHAR* pFileName)
{
    CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	TVWALL_PLAY_PARAM stPlayParam = {0};
	stPlayParam.iPlayType = PLAY_TYPE_REPLAY;
	if(!CToolsHelp::Strncpy(stPlayParam.szCameraCode, IVS_DEV_CODE_LEN, pCameraCode, strlen(pCameraCode)))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Start Play Back","Copy CameraCode failed");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	if(NULL!=pDomainCode && !CToolsHelp::Strncpy(stPlayParam.szDomainCode, IVS_DEV_CODE_LEN, pDomainCode, strlen(pDomainCode)))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Start Play Back","Copy DomainCode failed");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	if(NULL!=pNVRCode && !CToolsHelp::Strncpy(stPlayParam.szNVRCode, IVS_DEV_CODE_LEN, pNVRCode, strlen(pNVRCode)))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Start Play Back","Copy NVRCode failed");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	stPlayParam.uiProtocolType = stPlaybackParam.uiProtocolType;
	if(!CToolsHelp::Memcpy(&stPlayParam.stTimeSpan, sizeof(IVS_TIME_SPAN), &stPlaybackParam.stTimeSpan, sizeof(IVS_TIME_SPAN)))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Start Play Back","Copy Time failed");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	stPlayParam.fSpeed = stPlaybackParam.fSpeed;
	stPlayParam.iReplayType = iReplayType;
   IVS_INT32 iRet = StartPlay(stPlayParam, stTVWallParam, pFileName);
    return iRet;
}

IVS_ULONG CDecoderChannel::GetAsyncSeq() const
{
	return m_ulAsyncSeq;
}

//��ȡ������ID
const IVS_CHAR* CDecoderChannel::GetDecoderID() const
{
    return m_strDecoderID.c_str();
}

IVS_UINT32 CDecoderChannel::GetChannel() const
{
	return m_uiChannel;
}

//��ȡ������ǽ��Camera
const IVS_CHAR* CDecoderChannel::GetCameraCode() const
{
    return m_wndConnectInfo.szCameraCode;
}

//�Ƿ���������
bool CDecoderChannel::IsStarting() const
{
    return WND_STATUS_STARTING==m_wndStatus ? true:false;
}

bool CDecoderChannel::IsPlaying() const
{
    return WND_STATUS_PLAYING==m_wndStatus ? true:false;
}

//��ȡ�Զ��ָ�����
void CDecoderChannel::GetAutoResumeParam(AUTO_RESUME_PARAM* pParam) const
{
    BP_DBG_LOG("Enter CDecoderChannel::GetAutoResumeParam");

	pParam->bAutoResuming = FALSE;
    pParam->iPlayType = m_wndConnectInfo.iPlayType;
    if(!CToolsHelp::Strncpy(pParam->szCameraCode, sizeof(pParam->szCameraCode), m_wndConnectInfo.szCameraCode, strlen(m_wndConnectInfo.szCameraCode)))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Get AutoResume Param","Copy CameraCode failed");
	}
    if(!CToolsHelp::Strncpy(pParam->stTVWallParam.cDecoderCode, IVS_TVWALL_DECODER_CODE_LEN, m_strDecoderID.c_str(), m_strDecoderID.length()))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Get AutoResume Param", "Copy DecoderCode failed");
	}
    pParam->stTVWallParam.uiChannel = m_uiChannel;
    if(PLAY_TYPE_REAL==pParam->iPlayType)
    {
        pParam->stRealplayParam.uiStreamType = m_wndConnectInfo.uiStreamType;
        pParam->stRealplayParam.uiProtocolType = m_wndConnectInfo.uiProtocolType;
        pParam->stRealplayParam.bDirectFirst = m_wndConnectInfo.bDirectFirst;
        pParam->stRealplayParam.bMultiCast = FALSE;
    }

    (void)CToolsHelp::Strncpy(pParam->szScuUrl, URL_LEN+1, m_wndConnectInfo.szScuUrl, strlen(m_wndConnectInfo.szScuUrl));
    pParam->iHandle = m_wndConnectInfo.iHandle;

    BP_DBG_LOG("Out CDecoderChannel::GetAutoResumeParam: szScuUrl = %s, iHandle = %d.", pParam->szScuUrl, pParam->iHandle);
}

//�����쳣
void  CDecoderChannel::SetException(bool bException)
{
	m_wndConnectInfo.bException = bException;
}

void CDecoderChannel::HandlePlayException()
{
	m_wndStatus = WND_STATUS_IDLE;
	memset(&m_wndConnectInfo, 0, sizeof(m_wndConnectInfo));
	CHECK_POINTER_VOID(m_pTVWallMgr);
    m_pTVWallMgr->DeleteDecoderInitInfo(m_ulAsyncSeq);
}

bool CDecoderChannel::DealRuningPlayHard(const IVS_CHAR *pCameraCode, IVS_UINT32 uiStreamType, IVS_INT32 iPlayType, IVS_BOOL bDirect)
{
    BP_DBG_LOG("CDecoderChannel::DealRuningPlayHard: pCameraCode = %s.", pCameraCode);

	// ָ���������в���
	if (PLAY_TYPE_NONE!=m_wndConnectInfo.iPlayType)
	{
		ST_WND_CONNECT_INFO stConnectInfo = {0};
		if(!CToolsHelp::Memcpy(&stConnectInfo, sizeof(ST_WND_CONNECT_INFO), &m_wndConnectInfo, sizeof(ST_WND_CONNECT_INFO)))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Deal Runing Play Hard", "Copy ConnectInfo failed");
		}
		//ԭ�����������µ��������Ͳ�ͬ���ر�ԭ�в���
		if (iPlayType != stConnectInfo.iPlayType
			|| m_wndConnectInfo.bException)
		{
			(void)StopPlay();
			VOS_Sleep(STOP_WEIT_TIME);
			return true;
		}
		//ԭ�����������µ��������Ͷ���ʵ��
		if (PLAY_TYPE_REAL == iPlayType)
		{
			// ������ͬ,ֹͣԭ��ʵ��
			if (uiStreamType != stConnectInfo.uiStreamType ||
				bDirect != stConnectInfo.bDirectFirst)
			{
				(void)StopPlay();
				VOS_Sleep(STOP_WEIT_TIME);
				return true;
			}
			// ������ͬ
			// ��ͷ��ͬ������ֹͣԭ��ʵ��
			if (0 == strncmp(pCameraCode,stConnectInfo.szCameraCode,IVS_DEV_CODE_LEN))
			{
				if (stConnectInfo.bException) //�������쳣�ģ�ֹͣԭ����
				{
					(void)StopPlay();
					VOS_Sleep(STOP_WEIT_TIME);
					return true;
				}
				return false;
			}
			// ��ͷ��ͬ���ر�ԭ��ʵ��
			else
			{
				(void)StopPlay();
				VOS_Sleep(STOP_WEIT_TIME);
				return true;
			}
		}
		//ԭ�����������µ��������Ͷ��ǻط�
		if (PLAY_TYPE_REPLAY == iPlayType)
		{
			// ��ͷ��ͬ������ֹͣԭ�лط�
			if (0 == strncmp(pCameraCode,stConnectInfo.szCameraCode,IVS_DEV_CODE_LEN))
			{
				return false;
			}
			// ��ͷ��ͬ���ر�ԭ�лط�
			else
			{
				(void)StopPlay();
				VOS_Sleep(STOP_WEIT_TIME);
				return true;
			}
		}
	}
	return true;
}

//��ʼ����
IVS_INT32 CDecoderChannel::StartPlay(const TVWALL_PLAY_PARAM& stPlayParam, const IVS_TVWALL_PARAM& stTVWallParam, const IVS_CHAR* pFileName)
{
    BP_DBG_LOG("CDecoderChannel::StartPlay");

	CHECK_POINTER(m_pTVWallMgr, IVS_OPERATE_MEMORY_ERROR);
    //��鵱ǰ�����Ƿ�����ʵ��
    if( !DealRuningPlayHard(stPlayParam.szCameraCode, stPlayParam.uiStreamType, stPlayParam.iPlayType, stPlayParam.bDirectFirst))
    {
		BP_RUN_LOG_INF("Start Play Req", "Camera[%s] is playing, DecoderID=%s, Channel=%u", stPlayParam.szCameraCode, stTVWallParam.cDecoderCode, stTVWallParam.uiChannel);
        return IVS_SUCCEED;
    }

	m_bReqStop = false;
    IVS_INT32 iRet = IVS_SUCCEED;
    //����Ƿ��Ѿ�����������������������С�����У���ֱ��ͣԭ�в��š�
    if (WND_STATUS_STARTING==m_wndStatus)
    {
        BP_DBG_LOG("CDecoderChannel::StartPlay: Is Playing,Stop It.");
        iRet = StopPlay();
        if (IVS_SDK_NOT_PLAY_TVWALL == iRet)
        {
            iRet = IVS_SUCCEED;
        }
        CHECK_SUCCESS_RET(IVS_SUCCEED != iRet, iRet);
        VOS_Sleep(STOP_WEIT_TIME);
    }

    //����������������
    m_wndStatus = WND_STATUS_STARTING;
	//��ʼ��ͨ��
	MEDIA_ADDR stDecoderAddr = {0};	//������ý���ַ
	iRet = InitChannel(stPlayParam.uiProtocolType, stDecoderAddr);
    if (iRet != IVS_SUCCEED)
    {
        BP_RUN_LOG_ERR(iRet, "Start Play Req", "Init Channel failed!");
        HandlePlayException();
        return iRet;
    }

    //����Ƿ���ֹͣ����
    CHECK_STOP_FLAG_FOR_START();

    ////ѭ������������setup��Ϣʧ��ʱ���ط����󣬼��200���룬���25�Σ���5��
    int iTryTimes = 0;
    do 
    {
		//��ȡý��URL
		//������Ϣ�뷵����Ϣ
		ST_MEDIA_RSP mediaRsp;
		memset(&mediaRsp, 0, sizeof(ST_MEDIA_RSP));
        iRet = GetPlayURL(stPlayParam, mediaRsp, pFileName);
        if (IVS_SUCCEED != iRet || 0 == strlen(mediaRsp.szURL))
        {
            HandlePlayException();
            (void)StopChannel();
            BP_RUN_LOG_ERR(iRet,"Start Play Req", "get URL failed!");
            return iRet;
        }
        //����Ƿ���ֹͣ����
        CHECK_STOP_FLAG_FOR_START();
        BP_DBG_LOG("CDecoderChannel::StartPlay, get URL success: url = %s,iRet=%d, AsyncSeq=%lu",mediaRsp.szURL, iRet, m_ulAsyncSeq);
        //֪ͨmu��������������Ƶ����֪ͨ����������
        iRet = NotifyPlay(stPlayParam, stTVWallParam, mediaRsp, stDecoderAddr);
		//�����setupʧ�ܣ����ظ�����3��
		iTryTimes++;
		BP_DBG_LOG("CDecoderChannel::StartPlay, current try time is %d, AsyncSeq = %lu", iTryTimes, m_ulAsyncSeq);
		VOS_Sleep(200);
    } while (IVS_PLAYER_RET_RTSP_SEND_SETUP_FAIL==iRet && iTryTimes<3);
    if (IVS_SUCCEED != iRet)
    {  
        HandlePlayException();
        (void)StopChannel();
        BP_RUN_LOG_ERR(iRet,"Start Play Req", "Failed!");
    }
    return iRet;
}

//ֹͣ����
IVS_INT32 CDecoderChannel::StopPlay()
{
    BP_DBG_LOG("CDecoderChannel::StopPlay");

	m_bStoping = true;

	{//����ʲô�������ֹͣ������
		//ֹͣ����
		int iRet = StopChannel();
		if (0 != iRet)
		{
			BP_RUN_LOG_ERR(iRet, "Start Play Req", "Stop decoder failed, AsyncSeq=%lu.", m_ulAsyncSeq);
			//return iRet;
		}
        else
        {
		    BP_DBG_LOG("CDecoderChannel::StopPlay,The AsyncSeq=%lu stop success.", m_ulAsyncSeq);
        }
	}
	// ���ҵ�ǰ�����Ƿ��Ѿ�ʵ��
    if (PLAY_TYPE_NONE!=m_wndConnectInfo.iPlayType)
    {
        BP_DBG_LOG("CDecoderChannel::StopPlay,The AsyncSeq=%lu is playing, and stop it.", m_ulAsyncSeq);

        //�ر�rtsp
        (void)RTSP_CLIENT_SendTeardownMsg(m_wndConnectInfo.iHandle, m_wndConnectInfo.szScuUrl, 1);
        (void)RTSP_CLIENT_FreeHandle(m_wndConnectInfo.iHandle); 
        //������Ų���
        HandlePlayException();
		//CHECK_POINTER(m_pTVWallMgr, IVS_OPERATE_MEMORY_ERROR);
		//m_pTVWallMgr->DeleteDecoderInitInfo(m_ulAsyncSeq);
		m_bStoping = false;
        BP_DBG_LOG("CDecoderChannel::StopPlay: end.");
        return IVS_SUCCEED;
    }
    else
    {
        // ��ǰ����δ����ʵ��
        if (WND_STATUS_IDLE==m_wndStatus)
        {       
            BP_DBG_LOG("CDecoderChannel::StopPlay: this wnd is not playing.");
			m_bStoping = false;
            return IVS_SDK_NOT_PLAY_TVWALL;//IVS_SUCCEED;
        }
        // ��ǰ������������ʵ��������
        else
        {
            //����ֹͣ
            m_bReqStop = true;
            // ǰ���Ѿ�������ֹͣ�ı�־�����ȴ�ֹͣ�����ı�־��ѭ��100�����飬1�볬ʱ
            BP_DBG_LOG("CDecoderChannel::StopPlay,The AsyncSeq=%lu starting play but not finish, wait for stop it.", m_ulAsyncSeq);
            for (int i=0;i<10;i++)
            {
                if (WND_STATUS_IDLE==m_wndStatus)
                {
                    BP_DBG_LOG("CDecoderChannel::StopPlay", "AsyncSeq=%lu stoped.", m_ulAsyncSeq);
                    m_bReqStop = false;
					m_bStoping = false;
                    return IVS_SUCCEED;
                }
                Sleep(100);
            }
            m_bReqStop = false;
			m_bStoping = false;
            BP_RUN_LOG_ERR(IVS_FAIL, "Stop Play req", "Stop AsyncSeq=%lu timeout.", m_ulAsyncSeq);
            return IVS_FAIL;
        }
    }
}

IVS_INT32 CDecoderChannel::PausePlayback()
{
	if(WND_STATUS_PLAYING!=m_wndStatus || PLAY_TYPE_REPLAY!=m_wndConnectInfo.iPlayType)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Frame Step", "Playback not start, AsyncSeq=%lu.", m_ulAsyncSeq);
		return IVS_PARA_INVALID;
	}
	IVS_INT32 iRet = RTSP_CLIENT_SendPauseMsg(m_wndConnectInfo.iHandle, m_wndConnectInfo.szScuUrl, RTSP_CLIENT_TIMEOUT);

	if (IVS_SUCCEED != iRet)
	{  
		BP_RUN_LOG_ERR(iRet, "Pause Playback", "Send Pause Message failed, AsyncSeq=%lu.", m_ulAsyncSeq);
		return IVS_PLAYER_RET_RTSP_SEND_PLAY_FAIL;
	}
	return IVS_SUCCEED;
}

IVS_INT32 CDecoderChannel::ResumePlayback()
{
	if(WND_STATUS_PLAYING!=m_wndStatus || PLAY_TYPE_REPLAY!=m_wndConnectInfo.iPlayType)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Frame Step", "Playback not start, AsyncSeq=%lu.", m_ulAsyncSeq);
		return IVS_PARA_INVALID;
	}
	MEDIA_RANGE_S stMdieaRange;
	double dScale = m_wndConnectInfo.fSpeed;
	memset(&stMdieaRange, 0x0, sizeof(MEDIA_RANGE_S));
	stMdieaRange.enRangeType = RANGE_TYPE_UTC;
	stMdieaRange.MediaBeginOffset = OFFSET_CUR;
	IVS_INT32 iRet = RTSP_CLIENT_SendPlayMsg(m_wndConnectInfo.iHandle, m_wndConnectInfo.szScuUrl, dScale, &stMdieaRange, RTSP_CLIENT_TIMEOUT);

	if (IVS_SUCCEED != iRet)
	{  
		BP_RUN_LOG_ERR(iRet, "Resume Playback", "Send Play Message failed, AsyncSeq=%lu.", m_ulAsyncSeq);
		return IVS_PLAYER_RET_RTSP_SEND_PLAY_FAIL;
	}
	return IVS_SUCCEED;
}

IVS_INT32 CDecoderChannel::PlaybackFrameStepForward()
{
	if(WND_STATUS_PLAYING!=m_wndStatus || PLAY_TYPE_REPLAY!=m_wndConnectInfo.iPlayType)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Frame Step", "Playback not start, AsyncSeq=%lu.", m_ulAsyncSeq);
		return IVS_PARA_INVALID;
	}
	MEDIA_RANGE_S stMdieaRange;
	double dScale = 256.0;
	memset(&stMdieaRange, 0x0, sizeof(MEDIA_RANGE_S));
	stMdieaRange.enRangeType = RANGE_TYPE_UTC;
	stMdieaRange.MediaBeginOffset = OFFSET_CUR;
	IVS_INT32 iRet = RTSP_CLIENT_SendPlayMsg(m_wndConnectInfo.iHandle, m_wndConnectInfo.szScuUrl, dScale, &stMdieaRange, RTSP_CLIENT_TIMEOUT);

	if (IVS_SUCCEED != iRet)
	{  
		BP_RUN_LOG_ERR(iRet, "Frame Step", "Send Play Message failed, AsyncSeq=%lu.", m_ulAsyncSeq);
		return IVS_PLAYER_RET_RTSP_SEND_PLAY_FAIL;
	}
	return IVS_SUCCEED;
}
// ��������
IVS_INT32 CDecoderChannel::StartSound()
{
	return StartChannelSound();
}
// ֹͣ����
IVS_INT32 CDecoderChannel::StopSound()
{
	return StopChannelSound();
}
// ��������
IVS_INT32 CDecoderChannel::SetVolume(IVS_UINT32 uiVolumeValue)
{
	return SetChannelVolume(uiVolumeValue);
}

// Ӳ��Rtspý��ص�����
void __stdcall  HardDataCallBack(int /*handle*/, char * /*pBuffer*/, unsigned int /*ulBufSize*/, void * /*pUser*/)
{
    return;
}//lint !e818

//֪ͨ����(rtsp֪ͨmu��֪ͨ���������룩
IVS_INT32 CDecoderChannel::NotifyPlay(const TVWALL_PLAY_PARAM& stPlayParam, const IVS_TVWALL_PARAM& stTVWallParam, 
										const ST_MEDIA_RSP& mediaRsp, const MEDIA_ADDR& stDecoderAddr)
{
	IVS_INT32 iRet = IVS_SUCCEED;
	IVS_UINT32 uiStreamType = stPlayParam.uiStreamType;
	//���ʵ����Ƶ�����Ĭ������,��ô����URL,�õ�����������
	if (PLAY_TYPE_REAL==stPlayParam.iPlayType && STREAM_TYPE_UNSPECIFIED == uiStreamType)
	{
		std::string strStreamType;
		iRet = ParseURL(mediaRsp.szURL, URL_STREAM_TYPE, strStreamType);
		if (IVS_SUCCEED!=iRet)
		{
			BP_RUN_LOG_ERR(iRet, "Notify Play", "Parse stream type failed from url, AsyncSeq=%lu", m_ulAsyncSeq);
			return iRet;
		}
		uiStreamType = static_cast<IVS_UINT32>(CToolsHelp::StrToInt(strStreamType));
	}

	// ����RTSP����
	int iHandle = 0;
	iRet = RTSP_CLIENT_GetHandle(true, &iHandle);
	if (IVS_SUCCEED != iRet || 0==iHandle)
	{
		BP_RUN_LOG_ERR(IVS_PLAYER_RET_RTSP_GET_HANDLE_FAIL,"Notify Play", "Rtsp client get handle fail.");
		HandlePlayException();
		return IVS_PLAYER_RET_RTSP_GET_HANDLE_FAIL;
	}
	BP_DBG_LOG("CDecoderChannel::NotifyPlay, RTSP_CLIENT_GetHandle: iHandle=%d, AsyncSeq=%lu", iHandle, m_ulAsyncSeq);

	// setup
	MEDIA_ADDR stSourceMediaAddr;
	INET_ADDR stServiceAddr;
    bool bTcpFlag = false;
	memset(&stServiceAddr, 0x0, sizeof(INET_ADDR));
	stServiceAddr.unAddrIp = htonl((unsigned int)inet_addr(mediaRsp.szRTSPIP));
	stServiceAddr.unAddrPort = (unsigned short)atoi(mediaRsp.szRTSPPORT);
	bTcpFlag = ((1==stPlayParam.uiProtocolType) ? false:true);
    iRet = RTSP_CLIENT_SetDataCallBack(iHandle, HardDataCallBack, this);
	if (IVS_SUCCEED != iRet)
	{         
        BP_RUN_LOG_ERR(CMediaBase::GetRtspErrorCode(iRet),"Notify Play", "RTSP_CLIENT_SetDataCallBack failed, rtsp errcode=%d, AsyncSeq=%lu.", iRet, m_ulAsyncSeq);
        (void)RTSP_CLIENT_SendTeardownMsg(iHandle, mediaRsp.szURL, 1);
        (void)RTSP_CLIENT_FreeHandle(iHandle);
		return CMediaBase::GetRtspErrorCode(iRet);
	}

	iRet += RTSP_CLIENT_SendSetupMsg(iHandle, &stServiceAddr, mediaRsp.szURL, RTSP_CLIENT_TIMEOUT, bTcpFlag, &stDecoderAddr, &stSourceMediaAddr);

	if (IVS_SUCCEED != iRet)
	{         
        BP_RUN_LOG_ERR(CMediaBase::GetRtspErrorCode(iRet),"Notify Play", "Send Setup Message failed, rtsp errcode=%d, AsyncSeq=%lu.", iRet, m_ulAsyncSeq);
        (void)RTSP_CLIENT_SendTeardownMsg(iHandle, mediaRsp.szURL, 1);
        (void)RTSP_CLIENT_FreeHandle(iHandle);
		return CMediaBase::GetRtspErrorCode(iRet);
	}
	if(0==stSourceMediaAddr.unMediaVideoPort && 0==stSourceMediaAddr.unMediaAudioPort)
	{
        BP_RUN_LOG_ERR(IVS_SDK_PLAYBACK_ERR_GET_REMOTE_DATAINFO,"Notify Play", "Setup Message return port=0.");
		(void)RTSP_CLIENT_SendTeardownMsg(iHandle, mediaRsp.szURL, 1);
		(void)RTSP_CLIENT_FreeHandle(iHandle);
		return IVS_SDK_PLAYBACK_ERR_GET_REMOTE_DATAINFO;
	}

    BP_DBG_LOG("CDecoderChannel::NotifyPlay: Setup End.");

	//����Ƿ���ֹͣ����
	CHECK_STOP_FLAG_FOR_PLAY();
	// play
	MEDIA_RANGE_S stMdieaRange;
	unsigned int ulStartTime = 0;
	double dScale = 1.0;
	memset(&stMdieaRange, 0x0, sizeof(MEDIA_RANGE_S));
	if(PLAY_TYPE_REPLAY==stPlayParam.iPlayType)
	{
		dScale = stPlayParam.fSpeed;
		if(dScale<0)
		{
			ulStartTime = static_cast<unsigned int>(CMediaBase::UTCStr2Time(stPlayParam.stTimeSpan.cEnd, IVS_DATE_TIME_FORMAT)
													- CMediaBase::UTCStr2Time(stPlayParam.stTimeSpan.cStart, IVS_DATE_TIME_FORMAT));
		}
	}
	stMdieaRange.enRangeType = RANGE_TYPE_NPT;
	stMdieaRange.MediaBeginOffset = ulStartTime;

    BP_DBG_LOG("CDecoderChannel::NotifyPlay: Start Play.");

	iRet = RTSP_CLIENT_SendPlayMsg(iHandle, mediaRsp.szURL, dScale, &stMdieaRange, RTSP_CLIENT_TIMEOUT);
	if (IVS_SUCCEED != iRet)
	{  
        BP_RUN_LOG_ERR(iRet, "Notify Play", "Send Play Message failed, AsyncSeq=%lu.", m_ulAsyncSeq);
		(void)RTSP_CLIENT_SendTeardownMsg(iHandle, mediaRsp.szURL, 1);
        (void)RTSP_CLIENT_FreeHandle(iHandle);
		return IVS_PLAYER_RET_RTSP_SEND_PLAY_FAIL;
	}

    BP_DBG_LOG("CDecoderChannel::NotifyPlay: End Play.");

	//����Ƿ���ֹͣ����
	CHECK_STOP_FLAG_FOR_PLAY();
	iRet = StartChannel(mediaRsp.szURL, stSourceMediaAddr);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Notify Play", "Start Decoder Channel failed, AsyncSeq=%lu.", m_ulAsyncSeq); 
        (void)RTSP_CLIENT_SendTeardownMsg(iHandle, mediaRsp.szURL, 1);
        (void)RTSP_CLIENT_FreeHandle(iHandle);
		return iRet;
	}

    BP_DBG_LOG("CDecoderChannel::NotifyPlay: Start Channel End.");

	//����״̬Ϊ���ڲ���
	m_wndStatus = WND_STATUS_PLAYING;
	// ����RTSP������Ϣ
	m_wndConnectInfo.iPlayType = stPlayParam.iPlayType;
	m_wndConnectInfo.ulAsyncSeq = m_ulAsyncSeq;
	m_wndConnectInfo.iHandle = iHandle;
	m_wndConnectInfo.uiChannelNo = stTVWallParam.uiChannel;
	m_wndConnectInfo.uiStreamType = uiStreamType;
    m_wndConnectInfo.uiProtocolType = stPlayParam.uiProtocolType;
	m_wndConnectInfo.bDirectFirst = stPlayParam.bDirectFirst;
	m_wndConnectInfo.fSpeed =stPlayParam.fSpeed;
	m_wndConnectInfo.bException = false;
	memset(m_wndConnectInfo.szDecoderID, 0, CUMW_DECODER_ID_LEN+1);
	bool bSucc = CToolsHelp::Strncpy(m_wndConnectInfo.szDecoderID, sizeof(m_wndConnectInfo.szDecoderID), stTVWallParam.cDecoderCode,strlen(stTVWallParam.cDecoderCode));
	CHECK_SUCCESS_RET(bSucc, IVS_ALLOC_MEMORY_ERROR);

    BP_DBG_LOG("CDecoderChannel::NotifyPlay: Strncpy DecoderID End.");

	memset(m_wndConnectInfo.szScuUrl, 0, URL_LEN+1);
	bSucc = CToolsHelp::Strncpy(m_wndConnectInfo.szScuUrl,sizeof(m_wndConnectInfo.szScuUrl),mediaRsp.szURL,strlen(mediaRsp.szURL));
	CHECK_SUCCESS_RET(bSucc, IVS_ALLOC_MEMORY_ERROR);

    BP_DBG_LOG("CDecoderChannel::NotifyPlay: Strncpy ScuUrl End.");

	memset(m_wndConnectInfo.szCameraCode, 0, IVS_DEV_CODE_LEN +1);
	bSucc = CToolsHelp::Strncpy(m_wndConnectInfo.szCameraCode, sizeof(m_wndConnectInfo.szCameraCode), stPlayParam.szCameraCode, strlen(stPlayParam.szCameraCode));
	CHECK_SUCCESS_RET(bSucc, IVS_ALLOC_MEMORY_ERROR);

	m_ulHandle = iHandle;//lint !e732

    BP_DBG_LOG("CDecoderChannel::NotifyPlay: Leave.");

	return IVS_SUCCEED;
}

//��ȡURL������iPlayType�ж���ʵ�����ǻط�
IVS_INT32 CDecoderChannel::GetPlayURL(const TVWALL_PLAY_PARAM& stPlayParam, 
                                        ST_MEDIA_RSP& stMediaRsp, const IVS_CHAR* pFileName)
{
	CHECK_POINTER(m_pTVWallMgr, IVS_OPERATE_MEMORY_ERROR);
    ST_URL_MEDIA_INFO stUrlMediaInfo;
    memset(&stUrlMediaInfo, 0, sizeof(ST_URL_MEDIA_INFO));

	//��������ͷCode
	std::string strCameraCode;
	std::string strDomainCode;
	IVS_INT32 iRet = CToolsHelp::SplitCameraDomainCode(stPlayParam.szCameraCode, strCameraCode, strDomainCode);
	if(IVS_SUCCEED!=iRet)
	{
		BP_RUN_LOG_ERR(iRet, "TVWall Get URL", "SplitCameraCode failed, CameraCode=%s", stPlayParam.szCameraCode); 
		return iRet;
	}
	bool bSucc = false;
	if(strlen(stPlayParam.szNVRCode)>0)
	{
		bSucc = CToolsHelp::Strncpy(stUrlMediaInfo.szNVRCode, sizeof(stUrlMediaInfo.szNVRCode), stPlayParam.szNVRCode, strlen(stPlayParam.szNVRCode));
		CHECK_SUCCESS_RET(bSucc, IVS_ALLOC_MEMORY_ERROR);
	}
	if(strlen(stPlayParam.szDomainCode)>0)
	{
		bSucc = CToolsHelp::Strncpy(stUrlMediaInfo.cDomainCode, sizeof(stUrlMediaInfo.cDomainCode), stPlayParam.szDomainCode, strlen(stPlayParam.szDomainCode));
		CHECK_SUCCESS_RET(bSucc, IVS_ALLOC_MEMORY_ERROR);
	}
	else
	{
		bSucc = CToolsHelp::Strncpy(stUrlMediaInfo.cDomainCode, sizeof(stUrlMediaInfo.cDomainCode), strDomainCode.c_str(), strDomainCode.length());
		CHECK_SUCCESS_RET(bSucc, IVS_ALLOC_MEMORY_ERROR);
	}

	//TODO : szDstPort��ʵ�����ã���һ���̶�ֵ�����Э������szDstPP��νṹ��δ����
	bSucc = CToolsHelp::Strncpy(stUrlMediaInfo.szSrcCode, sizeof(stUrlMediaInfo.szSrcCode), strCameraCode.c_str(), strCameraCode.length());
	CHECK_SUCCESS_RET(bSucc, IVS_ALLOC_MEMORY_ERROR);

    bSucc = CToolsHelp::Strncpy(stUrlMediaInfo.szDstCode,sizeof(stUrlMediaInfo.szDstCode), "01",strlen("01"));
	CHECK_SUCCESS_RET(bSucc, IVS_ALLOC_MEMORY_ERROR);

    if(PLAY_TYPE_REAL==stPlayParam.iPlayType)
    {
        stUrlMediaInfo.ServiceType = SERVICE_TYPE_REALVIDEO;
        stUrlMediaInfo.MediaPara.StreamType = (IVS_STREAM_TYPE)stPlayParam.uiStreamType;
        stUrlMediaInfo.MediaPara.ProtocolType = (IVS_PROTOCOL_TYPE)stPlayParam.uiProtocolType;
        stUrlMediaInfo.MediaPara.TransMode = stPlayParam.bDirectFirst ? MEDIA_DIRECT:MEDIA_TRANS;
    }
    else
    {
		switch(stPlayParam.iReplayType)
		{
		case REPLAY_TYPE_PLATFORM:
			stUrlMediaInfo.ServiceType = SERVICE_TYPE_PLAYBACK;
			break;
		case REPLAY_TYPE_PU:
			stUrlMediaInfo.ServiceType = SERVICE_TYPE_PU_PLAYBACK;
			break;
		case REPLAY_TYPE_BACKUP:
			stUrlMediaInfo.ServiceType = SERVICE_TYPE_BACKUPRECORD_PLAYBACK;
			break;
		default:
			return IVS_PARA_INVALID;
		}
        stUrlMediaInfo.MediaPara.StreamType = STREAM_TYPE_MAIN;
        stUrlMediaInfo.MediaPara.ProtocolType = (IVS_PROTOCOL_TYPE)stPlayParam.uiProtocolType;
        stUrlMediaInfo.MediaPara.TransMode = MEDIA_TRANS;
        //�ط�ʱ��
		std::string strTime;
		time_t iTime = CMediaBase::UTCStr2Time(stPlayParam.stTimeSpan.cStart, IVS_DATE_TIME_FORMAT);
		CMediaBase::UTCTime2Str(iTime, strTime, IVS_DATE_TIME_NET_FORMAT);
		strTime.append("-");
		std::string strEndTime;
		iTime = CMediaBase::UTCStr2Time(stPlayParam.stTimeSpan.cEnd, IVS_DATE_TIME_FORMAT);
		CMediaBase::UTCTime2Str(iTime, strEndTime, IVS_DATE_TIME_NET_FORMAT);   
		strTime.append(strEndTime);
        if (TIME_SPAN_LEN <= strTime.size())
        {
            BP_RUN_LOG_ERR(IVS_PARA_INVALID,"Get URL", "Parse time span from rsp url failed. time string over max length. strTime=%s.", strTime.c_str());
            return IVS_PARA_INVALID;
        }
        bSucc = CToolsHelp::Strncpy(stUrlMediaInfo.szTimeSpan, sizeof(stUrlMediaInfo.szTimeSpan), strTime.c_str(), strTime.length());
		CHECK_SUCCESS_RET(bSucc, IVS_ALLOC_MEMORY_ERROR);
    }
	stUrlMediaInfo.iClientType = CLIENT_TYPE_DECODER;
	bSucc = CToolsHelp::Strncpy(stUrlMediaInfo.szEncryAlgorithm,sizeof(stUrlMediaInfo.szEncryAlgorithm), "0000000000",strlen("0000000000"));
	CHECK_SUCCESS_RET(bSucc, IVS_ALLOC_MEMORY_ERROR);
	
    stUrlMediaInfo.MediaPara.BroadCastType = BROADCAST_UNICAST;
    stUrlMediaInfo.MediaPara.VideoDecType = VIDEO_DEC_H264;
    stUrlMediaInfo.MediaPara.AudioDecType = AUDIO_DEC_G711A;
    stUrlMediaInfo.PackProtocolType = PACK_PROTOCOL_ES;
    (void)CToolsHelp::Strncpy(stUrlMediaInfo.szDstIP,sizeof(stUrlMediaInfo.szDstIP), "",strlen(""));//lint !e534

    //bSucc = CToolsHelp::Strncpy(stUrlMediaInfo.szDstPort,sizeof(stUrlMediaInfo.szDstPort), "9900",strlen("9900"));
	//CHECK_SUCCESS_RET(bSucc, IVS_ALLOC_MEMORY_ERROR);

    if (NULL != pFileName)
    {
        (void)CToolsHelp::Memcpy(stUrlMediaInfo.szFileName, FILE_NAME_LEN, pFileName, strlen(pFileName));
    }
    
    iRet = m_pTVWallMgr->GetURL(CToolsHelp::GetSCCHead().c_str(), stUrlMediaInfo, stMediaRsp);
    return iRet;
}//lint !e1762

//����URL,pKey����Ҫ�õ���ֵ
IVS_INT32 CDecoderChannel::ParseURL(const char* pUrl, const char* pKey, std::string& strValue)
{
	if ((URL_LEN < strlen(pUrl)) || (NULL == pKey))
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Parse URL", "Failed");
		return IVS_PARA_INVALID;
	}

	std::string strSrc = "";

	strSrc.append(pUrl);
	unsigned int pos1 = strSrc.find(pKey);
    if(pos1==std::string::npos)
    {
        return IVS_FAIL;
    }
	unsigned int pos2 = strSrc.find("&", pos1);
	if (pos2 == std::string::npos)
	{
		return IVS_FAIL;
	}
	strValue = strSrc.substr(pos1 + strlen(pKey), pos2 - (pos1 + strlen(pKey)));
	return IVS_SUCCEED;
}//lint !e1762

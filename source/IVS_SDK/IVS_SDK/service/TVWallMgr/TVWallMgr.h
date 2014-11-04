/********************************************************************
 filename    :    TVWallMgr.h
 author      :    g00209332
 created     :    2012/12/19
 description :    ����ǽ������;
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/12/19 ��ʼ�汾
*********************************************************************/
#pragma once
#include "hwsdk.h"
#include "SDKDef.h"
#include "ivs_xml.h"
#include "DecoderChannel.h"
#include "vos.h"
#include "SDKTimer.h"
#include "SDKDecoderMgr.h"
#include "AutoResumeTrigger.h"

//����ͷID
#define TVWALL_CAMERA_CODE_LEN  128

//�ص���Ϣ
typedef struct 
{
	PCUMW_CALLBACK_NOTIFY pCallBackFunc;		//�ص�����
	void* pUserData;							//�ص�����
}TVWALL_CALLBACK_INFO;

//�Զ�������Ϣ
typedef struct 
{
    CTVWallMgr* pTVWallMgr;
    CDecoderChannel* pCh;
    CUMW_BREAKDOWN_REASON emReason;
}TVWALL_ADD_AUTO_RESUME_INFO;

//�Զ������������Ϣ
typedef struct 
{
    CTVWallMgr* pTVWallMgr;
    AUTO_RESUME_PARAM stAutoResumePara;
}AUTO_RESUME_TREAD_PARAM;

class CUserMgr;
class CTVWallMgr
{
	typedef std::map<std::string, CUMW_NOTIFY_REMOTE_DECODER_STATUS*> REMOTE_DECODER_MAP;	   //��������Ϣ����
	typedef std::map<IVS_ULONG, CUMW_NOTIFY_INIT_DECODER_INFO*> REMOTE_DECODER_INIT_INFO_MAP;//��������ʼ����ϢMAP
	typedef std::map<std::string, CDecoderChannel*> TVWALL_DEC_CH_MAP;	//������MAP
    typedef std::map<std::string, AUTO_RESUME_PARAM*> AUTO_RESUME_MAP;//�Զ��ָ�����MAP
	typedef std::map<IVS_UINT64, TVWALL_CALLBACK_INFO> TVWALL_CALLBACK_MAP;
    typedef std::map<time_t, CDecoderChannel*> TVWALL_IDLE_CH_MAP;
public:
    // ��ʼ��������ǽģ��
    IVS_INT32 InitTVWall(PCUMW_CALLBACK_NOTIFY pFunNotifyCallBack, void* pUserData);

    // �ͷŵ�����ǽģ��
    IVS_INT32 CleanupTVWall();
	// ��ȡ�������б�
	IVS_INT32 GetDecoderList(const IVS_INDEX_RANGE* pIndexRange,
								CUMW_NOTIFY_REMOTE_DECODER_STATUS_LIST* pDecoderList,
								IVS_UINT32 uiBufferSize);
	// ��������ǽʵʱ���
	IVS_INT32 StartRealPlayTVWall(const IVS_CHAR* pCameraCode,
									const IVS_REALPLAY_PARAM* pRealplayParam,
									const IVS_TVWALL_PARAM* pTVWallParam,
									IVS_BOOL bAutoResume=FALSE);	
	// ��������ǽʵʱ���
	IVS_INT32 StopRealPlayTVWall(const IVS_TVWALL_PARAM* pTVWallParam);

	// ��������ǽ¼��ط�
    IVS_INT32 StartPlayBackTVWall(const IVS_CHAR* pCameraCode,
                                            const IVS_PLAYBACK_PARAM* pPlaybackParam,
                                            const IVS_TVWALL_PARAM* pTVWallParam,
											REPLAY_TYPE iReplayType,
											const IVS_CHAR* pDomainCode=NULL,
											const IVS_CHAR* pNVRCode=NULL);

	// ֹͣ����ǽ¼��ط�
	IVS_INT32 StopPlayBackTVWall(const IVS_TVWALL_PARAM* pTVWallParam);
	// �ݶ�����ǽ¼��ط�
	IVS_INT32 PlayBackPauseTVWall(const IVS_TVWALL_PARAM& stTVWallParam);
	// �ָ�����ǽ¼��ط�
	IVS_INT32 PlayBackResumeTVWall(const IVS_TVWALL_PARAM& stTVWallParam);

	// ��֡���
	IVS_INT32 PlaybackFrameStepForwardTVWall(const IVS_TVWALL_PARAM& stTVWallParam);

	// ��������
	IVS_INT32 PlaySoundTVWall(const IVS_TVWALL_PARAM& stTVWallParam);
	// ֹͣ����
	IVS_INT32 StopSoundTVWall(const IVS_TVWALL_PARAM& stTVWallParam);
	// ��������
	IVS_INT32 SetVolumeTVWall(const IVS_TVWALL_PARAM& stTVWallParam, IVS_UINT32 uiVolumeValue);
	// ����һ���ص���Ϣ
	IVS_INT32 AddCallBack(PCUMW_CALLBACK_NOTIFY pFunNotifyCallBack, void* pUserData);
	// �Ƴ�һ���ص���Ϣ
	IVS_INT32 RemoveCallBack(void* pUserData);

    //ɾ������ͨ������
    void DelIdleCh();

public:
	CTVWallMgr(void);
	~CTVWallMgr(void);

	void SetUserMgr(CUserMgr* pUserMgr);
	//��SCU��ȡý��URL
	IVS_INT32 GetURL(const char *pszAuthHeader, ST_URL_MEDIA_INFO& stUrlMediaInfo, ST_MEDIA_RSP& stMediaRsp);
	//��ȡ������ͨ����ʼ����Ϣ
	IVS_INT32 GetDecoderInitInfo(IVS_ULONG ulWndID, CUMW_NOTIFY_INIT_DECODER_INFO* pInitDecoderInfo);
	//���ӽ�����ͨ����ʼ����Ϣ
	void AddDecoderInitInfo(IVS_ULONG ulWndID, const CUMW_NOTIFY_INIT_DECODER_INFO& initDecoderInfo);
	//ɾ��ָ������Ľ�����ͨ����ʼ����Ϣ
	void DeleteDecoderInitInfo(IVS_ULONG ulWndID);
    //�Զ��ָ�����,�ɴ�������������
    void HandleAutoResume();
    //�첽��������֪ͨ
    void NotifyAsyncReturn(const CUMW_NOTIFY_ASYNC_RET_EX& stAsyncRet);
	//�¼�֪ͨ�ϲ�
	void NotifyEvent(CUMW_NOTIFY_INFO* pstNotifyInfo);
private:
	//��ȡָ��������ͨ������û�оʹ���һ��
    CDecoderChannel* GetDecoderChannel(const char* pDecoderID, IVS_UINT32 uiChannel);
    //��ȡָ��������ͨ������ֻ�ǻ�ȡ
    CDecoderChannel* GetDecoderChannel(const std::string& key);
    //��ȡָ��������ͨ������ֻ�ǻ�ȡ
	CDecoderChannel* GetDecoderChannel(IVS_ULONG ulAsyncSeq);
	//����MAP��Key
	std::string MakeKey(const char* pDecoderID, IVS_UINT32 uiChannel) const;
	//����һ����������Ϣ
	void AddRemoteDecoder(const CUMW_NOTIFY_REMOTE_DECODER_STATUS& remoteDecoder);
	//�Ƴ�һ��������
	void RemoveRemoteDecoder(const IVS_CHAR* remoteDecoderID);
    //����ǰ��¼���ļ����� 
    IVS_INT32 GetPUPlayBackFileName(const char* pCameraCode, const IVS_TIME_SPAN* pTimeSpan, IVS_RECORD_INFO &stRecordInfo);
    //�Զ��ָ�
    IVS_INT32 AutoResumeEx(const AUTO_RESUME_PARAM* pResumeParam);
public:
	//�����Զ��ָ�
	void AddAutoResume(CDecoderChannel* pCh, CUMW_BREAKDOWN_REASON iReason);
private:
	//����Ƿ����Զ���������.�У��ر�RTSP������true;��,����false.
	bool RemoveAutoResume(const char* szDecoderCode, unsigned int uiChannel);
	//����ǽ�ص�����
	static IVS_INT32 __stdcall HandleNotifyCallBack(CUMW_NOTIFY_INFO* pstNotifyInfo, void* pUserData);
	//�����ʼ��������ͨ���ķ�������
	void InitDecoderNotify(const CUMW_NOTIFY_INFO* pstNotifyInfo, const void* pUserData);
    //��ѯָ���Ľ������Ƿ�����
    bool IsOnline(const char* pDecoderID);
	//��ѯָ���Ľ������Ƿ����ߣ��Ƿ���ָ����ͨ��
	IVS_INT32 CheckDecoder(const char* pDecoderID, IVS_UINT32 uiChannel);
	//�����������쳣����
	IVS_INT32 DecoderStreamError(const char* pDecoderID, IVS_UINT32 uiChannel);
	//�ϱ��Զ��ָ��¼� bStartResume �Ƿ��������ָ� TRUE-�����ָ� FALSE-�ָ���ɣ������iErrCode
	void ReportAutoResumeEvnet(const AUTO_RESUME_PARAM* pResumeParam, IVS_INT32 iErrCode = IVS_SUCCEED, IVS_BOOL bStartResume=TRUE);
    //ȡ�õ�ǰϵͳʱ�䣬����Ϊ��λ
    time_t GetCurrentTime() const;
    //��ͨ������ָ��ŵ�����������
    void AddToIdleList(CDecoderChannel* pDecoderChannel);

    //��������������Ƿ񻹴���ָ���ļ�ֵ����������
    bool IsAutoResumeParaExist(const std::string& key);
    //ɾ�����������������
    void DelAutoResumePara(const std::string& key);

    //���������쳣�������Զ�����
    void DecoderStreamError(CDecoderChannel* pCh, CUMW_BREAKDOWN_REASON emReason);
    //�����Զ��������̺߳���
    static void __cdecl AddAutoResumeThread(LPVOID lpvoid);

    //��ʼӲ��ʵ���̺߳���
    static void __cdecl AutoResumeThreadFun(LPVOID lpvoid);

private:
	//////////////////////////////////////////////////////////////////////////
	//CUʹ�ö��OCX������initTVWallֻ��ע��һ��ocx��ȡ�¼�
	//Ϊ��ʹ����ǽ���ocx���ܻ�ȡ���¼���ʹ��Map��Ŷ����¼���OCX��Ϣ
	//PCUMW_CALLBACK_NOTIFY m_pFunCallBack;       //�ص�����ָ��
	//void* m_pUserData;                          //�ϲ���ô����Ĳ�����һ����thisָ��
	TVWALL_CALLBACK_MAP m_callbackMap;			//�ϲ�ص���ϢMAP
	VOS_Mutex* m_pCallBackMapMutex;					   //��������ʼ��Map��
	//////////////////////////////////////////////////////////////////////////

	CUserMgr* m_pUserMgr;

	REMOTE_DECODER_INIT_INFO_MAP m_remoteDecodeInitInfoMap;//Զ�̽�������ʼ����Ϣ��
	VOS_Mutex* m_pInitDecoderMapMutex;					   //��������ʼ��Map��

	TVWALL_DEC_CH_MAP m_tvwallDecChMap;			//����ǽ������ͨ��MAP
	VOS_Mutex* m_pDecChMapMutex;					//ͨ��MAP��;

	REMOTE_DECODER_MAP m_onlineDecoderMap;	    //���߽�������Ϣ
    VOS_Mutex* m_pOnlineDecoderMapMutex;		//���߽�������ϢMAP��;

    AUTO_RESUME_MAP m_autoResumeMap;            //�Զ��ָ�����MAP����Ҫ�Զ��ָ��Ĳ��Ų����������ڴ�MAP��
    VOS_Mutex* m_pAutoResumeMapMutex;           //�Զ��ָ�MAP��
    CNVSTimer m_timer;                          //�Զ��ָ���ʱ��
    CAutoResumeTrigger m_AutoResumeTigger;    //�Զ��ָ�������
    bool m_bTimerInit;                          //��ʱ���Ƿ��Ѿ���ʼ��

	IVS_ULONG m_ulAsyncSeq;						//�����ǽģ��ͨ���첽ҵ�����

	//CSDKDecoderMgr m_SDKDecoderMgr;				//SDK�������������

	//��������ǽ�ɹ���ͨ���б�
	typedef std::map<IVS_ULONG,CDecoderChannel*> CHANNEL_MAP;
	typedef CHANNEL_MAP::iterator CHANNEL_MAP_ITER;
	CHANNEL_MAP m_mapPlayChannel;//ʵ���б�

    TVWALL_IDLE_CH_MAP m_IdleChMap;            //���н�����ͨ����������
    VOS_Mutex* m_pIdleChMapMutex;              //���н�����ͨ�������������

public:
	CDecoderChannel* GetChannel(IVS_ULONG ulHandle);
	IVS_ULONG GetTVWallHandlebyRtspHandle(IVS_ULONG ulHandle);
	void DeleteChannel(IVS_ULONG ulHandle);//ɾ��ĳͨ��

    VOS_Mutex* m_pHandleMutex;		          //�����;
};


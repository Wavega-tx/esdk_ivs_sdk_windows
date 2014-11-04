/********************************************************************
 filename    :    DecoderChannel.h
 author      :    g00209332
 created     :    2012/12/19
 description :    ������ͨ���࣬��Ե����������Ƶ��ʵ�����طţ���ǽ��ǽ����
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/12/27 ��ʼ�汾
*********************************************************************/
#pragma once
#include "hwsdk.h"
#include "SDKDef.h"
#include "IVS_SDKTVWall.h"
#include "rtsp_client_api.h"


//��StartRealPlay��ʹ��
#define CHECK_STOP_FLAG_FOR_START() \
{\
    do\
    {\
    if(m_bReqStop)\
        {\
        HandlePlayException();\
        (void)StopChannel();\
        BP_DBG_LOG("Recive stop request, stop play��AsyncSeq=%lu", m_ulAsyncSeq);\
        return IVS_TVWALL_STOP_FLAG;\
        }\
    } while (false);\
}

//��StartRealPlay��ʹ��
#define CHECK_STOP_FLAG_FOR_PLAY() \
{\
    do\
    {\
    if(m_bReqStop)\
        {\
        if (0 != iHandle)\
            {\
            (void)RTSP_CLIENT_SendTeardownMsg(iHandle, mediaRsp.szURL, 1);\
            (void)RTSP_CLIENT_FreeHandle(iHandle);\
            }\
            BP_DBG_LOG("CDecoderChannel::StartRealPlay, Recive stop request, stop play��AsyncSeq=%lu", m_ulAsyncSeq);\
            return IVS_TVWALL_STOP_FLAG;\
        }\
    } while (false);\
}

//����ǰ״̬
typedef enum _wnd_status
{
	WND_STATUS_IDLE,		//����
	WND_STATUS_STARTING,	//��������
	WND_STATUS_PLAYING     //���ڲ���
}WND_STATUS;

//��������
typedef enum
{
	PLAY_TYPE_NONE = 0,	//�޲���
	PLAY_TYPE_REAL = 1,			//ʵ��
	PLAY_TYPE_REPLAY = 2		//�ط�
}PLAY_TYPE;

//�ط�����
typedef enum
{
	REPLAY_TYPE_PLATFORM = 0,	//ƽ̨�ط�
	REPLAY_TYPE_PU = 1,			//ǰ�˻ط�
	REPLAY_TYPE_BACKUP = 2      //���ݷ������ط�
}REPLAY_TYPE;

//����ǽ�����rtsp������Ϣ
typedef struct
{
	IVS_ULONG   ulAsyncSeq;                   //�첽ͨ�����
	IVS_INT32   iPlayType;                    //�������ͣ�0 �޲��ţ�1 ʵ����2 �ط�
	IVS_INT32   iHandle;                      //RTSP����Handle
	IVS_UINT32  uiChannelNo;                  //������ͨ����
	IVS_UINT32  uiStreamType;                 //�������ͣ� 1-��������2-������1��3-������2���� 
    IVS_UINT32  uiProtocolType;               //Э�� 1-UDP 2-TCP
	IVS_BOOL	bDirectFirst;				  //�Ƿ�ֱ������
	IVS_CHAR    szDecoderID[CUMW_DECODER_ID_LEN+1];//������ID
	IVS_CHAR    szCameraCode[IVS_DEV_CODE_LEN +1];  //��ͷCODE
	IVS_CHAR    szScuUrl[URL_LEN+1];            //SCUý��URL
	IVS_CHAR    szTimeSpan[TIME_SPAN_LEN+1];       // �ط�ʱ������
	IVS_FLOAT   fSpeed;							// �ط�����
	IVS_BOOL    bException;                    //�������쳣���ȴ�����
}ST_WND_CONNECT_INFO;

//���Ų���
typedef struct
{
	IVS_CHAR szCameraCode[IVS_DEV_CODE_LEN +1];//�����CODE
	IVS_CHAR szDomainCode[IVS_DEV_CODE_LEN +1];//�����
	IVS_CHAR szNVRCode[IVS_DEV_CODE_LEN +1];   //NVR����
	PLAY_TYPE		iPlayType;                    //��������
	IVS_UINT32      uiStreamType;       // �������ͣ�ֵ�ο� IVS_STREAM_TYPE
	IVS_UINT32      uiProtocolType;     // Э�����ͣ�1-UDP 2-TCP��Ĭ��Ϊ1
	IVS_BOOL        bDirectFirst;       // �Ƿ�ֱ�����ȣ�0-�� 1-�ǣ�Ĭ��Ϊ0
	IVS_BOOL        bMultiCast;         // �Ƿ��鲥��0-������1-�鲥��Ĭ��Ϊ0
	IVS_TIME_SPAN   stTimeSpan;         // ʵ������������ʱ��
	IVS_FLOAT       fSpeed;             // �ط�����
	REPLAY_TYPE     iReplayType;		// �ط�����
}TVWALL_PLAY_PARAM;

//�Զ��ָ�����
typedef struct  
{
    IVS_BOOL bAutoResuming;                    //�����Զ��ָ�
	IVS_INT32 iPlayType;                       //��������
    IVS_INT32   iHandle;                       //RTSP����Handle
	IVS_CHAR szCameraCode[IVS_DEV_CODE_LEN +1];//�����CODE
    IVS_CHAR szScuUrl[URL_LEN+1];              //SCUý��URL
    IVS_TVWALL_PARAM stTVWallParam;            //����ǽ����
    IVS_REALPLAY_PARAM stRealplayParam;        //ʵ������
	CUMW_BREAKDOWN_REASON iReson;			   //�쳣ԭ�� 1-���������� 2-��Ƶ���ж�
}AUTO_RESUME_PARAM;

class CTVWallMgr; //lint !e763
class CDecoderChannel
{
public:
	CDecoderChannel(IVS_ULONG ulAsyncSeq, 
					const IVS_CHAR* pDecoderID, 
					IVS_UINT32 uiChannel,
					CTVWallMgr* pTVWallMgr);
	virtual ~CDecoderChannel(void);
	// ��������ǽʵʱ���
	IVS_INT32 StartRealPlay(const IVS_CHAR* pCameraCode, 
                            const IVS_REALPLAY_PARAM& stRealplayParam, 
                            const IVS_TVWALL_PARAM& stTVWallParam);

    // ��������ǽ¼��ط�
	IVS_INT32 StartPlayBack(const IVS_CHAR* pCameraCode, 
                                    const IVS_PLAYBACK_PARAM& stPlaybackParam, 
                                    const IVS_TVWALL_PARAM& stTVWallParam, 
									REPLAY_TYPE iReplayType,
									const IVS_CHAR* pDomainCode=NULL,
									const IVS_CHAR* pNVRCode=NULL,
                                    const IVS_CHAR* pFileName=NULL);

	// ֹͣ����
    IVS_INT32 StopPlay();
	// ��֡����
	IVS_INT32 PlaybackFrameStepForward();
	// ��ͣ�ط�
	IVS_INT32 PausePlayback();
	// �ָ��ط�
	IVS_INT32 ResumePlayback();
	// ��������
	IVS_INT32 StartSound();
	// ֹͣ����
	IVS_INT32 StopSound();
	// ��������
	IVS_INT32 SetVolume(IVS_UINT32 uiVolumeValue);
public:
	//��ȡAsyncSeq
	IVS_ULONG GetAsyncSeq() const;
    //��ȡ������ID
    const IVS_CHAR* GetDecoderID() const;
	//��ȡͨ����
	IVS_UINT32 GetChannel() const;
    //��ȡ������ǽ��Camera
    const IVS_CHAR* GetCameraCode() const;
    //�Ƿ���������
    bool IsStarting() const;
    //�Ƿ����ڲ���
    bool IsPlaying() const;
    //��ȡ�Զ��ָ�����
    void GetAutoResumeParam(AUTO_RESUME_PARAM* pParam) const;
	//�����쳣
	void SetException(bool bException=true);
protected:
	CDecoderChannel(void);//pc-lint
	// ��ʼ��ͨ��
	virtual IVS_INT32 InitChannel(IVS_UINT32 uiProtocolType, MEDIA_ADDR& stDecoderAddr) = 0;
	// ����ͨ����ʼ����
	virtual IVS_INT32 StartChannel(const std::string strURL, const MEDIA_ADDR stSourceMediaAddr) = 0;
	// ����ͨ������
	virtual IVS_INT32 StopChannel() = 0;
	// ����ͨ������
	virtual IVS_INT32 StartChannelSound() = 0;
	// ֹͣͨ������
	virtual IVS_INT32 StopChannelSound() = 0;
	// ����ͨ����������
	virtual IVS_INT32 SetChannelVolume(IVS_UINT32 uiVolumeValue) = 0;

	//�����쳣�������״̬���������ã����캯�����쳣����ʱ���ã�
	void HandlePlayException();
	//����URL,pKey����Ҫ�õ���ֵ
	IVS_INT32 ParseURL(const char* pUrl, const char* pKey, std::string& strValue);
private:
	/**************************************************************************
    * name       : DealRuningPlayHard
    * description:  ��鲢����ǰ����Ĳ���
                    ���������Ͳ�ͬ����ر�ԭ�в��ţ�������尴ʵ���ͻط�ȥ����
                    ����ʵ��ʱ��
                        ����ǰ������ʵ����ֱ�ӷ���true
                        ������ʵ����������ʵ���ľ�ͷID��ͬ��������ͬ����رյ�ǰʵ�������� true
                        ������ʵ����������ʵ���ľ�ͷID��ͬ����رյ�ǰʵ�������� true
                        ������ʵ����������ʵ���ľ�ͷID��ͬ��������ͬ������ false���������迪���µ�ʵ����ֱ�ӷ���
                    ����ط�ʱ��
                        ����ͷID��ͬ��ֱ�ӷ���FALSE���������迪���µĻط�
                        ����ͷID��ͬ����رյ�ǰ�طţ����� true
    * input      : ulPlayType �µĲ����������ͣ�1Ϊʵ����2Ϊ�ط�
    * output     : NA
    * return     : RET_OK-�ɹ�     RET_FAIL-ʧ��
    * remark     : NA
    **************************************************************************/
    bool DealRuningPlayHard(const IVS_CHAR *pCameraCode, IVS_UINT32 uiStreamType, IVS_INT32 iPlayType, IVS_BOOL bDirect);

    //��ʼ����
    IVS_INT32 StartPlay(const TVWALL_PLAY_PARAM& stPlayParam,
                        const IVS_TVWALL_PARAM& stTVWallParam,
                        const IVS_CHAR* pFileName = NULL);
	//֪ͨ����(rtsp֪ͨmu��֪ͨ���������룩
	IVS_INT32 NotifyPlay(const TVWALL_PLAY_PARAM& stPlayParam, const IVS_TVWALL_PARAM& stTVWallParam, 
						const ST_MEDIA_RSP& mediaRsp, const MEDIA_ADDR& stDecoderAddr);
    //��ȡURL������iPlayType�ж���ʵ�����ǻط�
	IVS_INT32 GetPlayURL(const TVWALL_PLAY_PARAM& stPlayParam,
                        ST_MEDIA_RSP& stMediaRsp, const IVS_CHAR* pFileName = NULL);
public:
    ST_WND_CONNECT_INFO m_wndConnectInfo;	//������Ϣ
    IVS_ULONG m_ulHandle;
    WND_STATUS m_wndStatus;					//����״̬
    bool m_bReqStop;						//����ֹͣ
protected:
	IVS_ULONG m_ulAsyncSeq;					//�����ǽģ��ͨ���첽ҵ�����
    std::string m_strDecoderID;             //������ID
    IVS_UINT32 m_uiChannel;                 //������ͨ����
	bool m_bStoping;						//����ֹͣ
	CTVWallMgr* m_pTVWallMgr;				//����ǽ�������
public:
	IVS_ULONG GetHandle()const {return m_ulHandle;}
};

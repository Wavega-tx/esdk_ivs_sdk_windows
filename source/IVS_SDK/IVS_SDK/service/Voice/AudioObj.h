#pragma once

#include "MediaBase.h"
#include "SDKDef.h"
#include <map>
#include <string>

// �����㲥��Ϣ
typedef struct _stBROADCAST_RTSP_INFO
{
    char    szUrl[IVS_URL_LEN];
    char    ServiceIP[IVS_IP_LEN];
    ULONG   ServicePort;
    ULONG   RtspHandle;
    ULONG   PlayHandle;
    ULONG   NetSourceHandle;
	// ULONG   MediaBaseHandle;
}BROADCAST_RTSP_INFO;

typedef std::map<std::string, BROADCAST_RTSP_INFO*>  CODE_HANDLE_MAP;
typedef CODE_HANDLE_MAP::iterator   CODE_HANDLE_ITER;

class CMediaBase;

class CAudioObj : public CMediaBase
{//lint !e1790
public:
    CAudioObj(void);
    ~CAudioObj(void); //lint !e1509

	//��ʼ�����Խ�
    int StartVoiceTalkback(unsigned int uiProtocolType,
                           IVS_BOOL bDirectFirst,
                           const char* pCameraCode);
	
	//ֹͣ�����Խ�
	int StopVoiceTalkback();
 
	//��ȡЭ������
	ULONG GetProtocolType(char *pProtocol, ULONG ulProtocolType);

	//��ӹ㲥��ͷ
	int AddBroadcastDevice(const IVS_CHAR* pCameraCode);

	//ɾ���㲥��ͷ
	int DeleteBroadcastDevice(const IVS_CHAR* pCameraCode);

	//��ʼ��·�㲥���ڲ�����
	int StartSingleBroadcast(int ulBroadcastType, const IVS_CHAR* pCameraCode,
                             const IVS_CHAR* fileNmae=NULL,int iCycle=-1);

	//ֹͣ��·�����㲥���ڲ�����
	int StopSingleVoiceBroadcast(const BROADCAST_RTSP_INFO* pMediaRsp);

	//��ʼ�����㲥
	int StartRealBroadcast();

	//ֹͣ�����㲥
	int StopRealBroadcast();

	//���CameraCode��HANDLE��������
	void AddCodeHandle(const IVS_CHAR* pCameraCode, BROADCAST_RTSP_INFO* pRtspInfo);

	//��ȡCameraCode��HANDLE��������
	BROADCAST_RTSP_INFO* GetHandleByCode(const IVS_CHAR* pCameraCode);

	//ɾ��CameraCode��HANDLE��������
	void DeleteCodeHandle(const IVS_CHAR* pCameraCode);

    // ���map
    void ClearUpCodeHandle();

	//�������
	void ClearUp();

	//��ʼ�ļ������㲥
	int StartFileBroadcast(const IVS_CHAR* pFileName,IVS_INT32 iCycle);

    //ֹͣ�ļ������㲥
	int StopFileBroadcast();

	//�жϹ㲥�ļ��Ƿ���WAV��ʽ�ģ��Ƿ���0�����򷵻ظ��������룬�ڲ�����
	int CheckFileExt(const char*pFileName);

    void SetHandle(unsigned long ulHandle);

	inline IVS_SERVICE_TYPE GetServiceType()
	{
		return m_enServiceType;
	}//lint !e1762

	inline void SetServiceType(IVS_SERVICE_TYPE enServiceType)
	{
		m_enServiceType = enServiceType;
	}

	inline unsigned long GetPlayHandle() const { return m_ulHandle; }

	bool IsExistTalkBackRtspHandle(IVS_ULONG ulRtspHandle) const;

	bool IsExistBroadCastRtspHandle(IVS_ULONG ulRtspHandle);

    bool IsExistTalkBackNetHandle(IVS_ULONG ulNetHandle) const;

    bool IsExistBroadCastNetHandle(IVS_ULONG ulNetHandle);

	// int GetBroadCastVoiceCodeByRtspHandle(IVS_ULONG ulRtspHandle, std::string& strVoiceCode);

    int GetBroadCastDeviceCodeByConnHandle(IVS_ULONG ulNetHandle, std::string& strDeviceCode);

private:
    /******************************************************************
	function : StartPlay
	description: ��ʼ�����Խ�ͨ��
	input : pstMediaPara��ý����Ϣ��pCameraCode���������ţ�stParam�����Ų���
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    int StartPlay(const IVS_MEDIA_PARA* pstMediaPara, 
                  const char* pCameraCode, 
                  const START_PLAY_PARAM& stParam);

    int StopPlay(const BROADCAST_RTSP_INFO* pRtspInfo) const;

    int DoGetUrl(IVS_SERVICE_TYPE enServiceType);

    // add by wanglei 00165153:2013.5.3 ���ڻ�ȡָ�������֧�ֵ���Ƶ�����ʽ
    int GetSupportAudioType(const char* pszCameraCode, unsigned int& unAudioType);

    int GetSupportAudioType(unsigned int &uAudioType);

    int CheckAudioType();

	
private:
	//ý��������������������㲥���п���ͨ�������豸�������µĹ㲥�豸����Ҫ�õ��˲���
	IVS_MEDIA_PARA m_mediaPara;      
	//�ļ����������������������㲥���п���ͨ�������豸�������µĹ㲥�豸����Ҫ�õ��˲���
	char m_fileName[FILE_NAME_LEN+1];
	//�ļ��㲥�������������������㲥���п���ͨ�������豸�������µĹ㲥�豸����Ҫ�õ��˲��� 0 ��ѭ������ 1 ѭ������
	int m_iFileBroadcastCycle;
	//�㲥����
	int m_iBroadcastType;
	CODE_HANDLE_MAP m_CodeHandle;		      //CameraCode��HANDLE��MAP
	CRITICAL_SECTION m_csCodeHandleLock;      //CameraCode��HANDLE��MAP����

	//�Ƿ��Ѿ������㲥��־��δ����Ϊfalse������Ϊtrue
	bool m_bIsBroadcastOpenedFlag;//��Ҫ������

    static unsigned int m_stcAudioType;
};

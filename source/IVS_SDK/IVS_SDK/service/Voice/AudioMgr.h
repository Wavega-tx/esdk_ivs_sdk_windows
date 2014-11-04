#pragma once
// #include "IVSSDKDef.h"
#include "StreamData.h"
#include "AudioObj.h"

class CAudioMgr 
{
public:
	CAudioMgr(void);
	virtual ~CAudioMgr(void);
	//�������
	void ClearUp();

	//�����û�����
	//void SetUserObj(void* pUserObj);

    void SetUserMgr(CUserMgr* pUserMgr);

	//��ʼ�����Խ�
    IVS_INT32 StartVoiceTalkback(const IVS_VOICE_TALKBACK_PARAM* pTalkbackParam,
                                 const IVS_CHAR* pCameraCode,
                                 IVS_ULONG* pHandle);
	
	//ֹͣ�����Խ�
	IVS_INT32 StopVoiceTalkback(INT32 iTalkbackHandle);

	//��������豸���㲥��
	IVS_INT32 AddBroadcastDevice(const IVS_CHAR* pCameraCode);	

    //ɾ�������豸
	IVS_INT32 DeleteBroadcastDevice(const IVS_CHAR* pCameraCode);

	//��ʼʵʱ�����㲥
	IVS_INT32 StartRealBroadcast();

    //ֹͣʵʱ�����㲥
	IVS_INT32 StopRealBroadcast();

	//��ʼ�ļ������㲥
	IVS_INT32 StartFileBroadcast(const IVS_CHAR* pFileName,IVS_INT32 iCycle);

    //ֹͣ�ļ������㲥
	IVS_INT32 StopFileBroadcast();

    //��������
    IVS_INT32 SetVolume(unsigned int uiPlayHandle, unsigned int uiVolumeValue);

    //��ȡ����
    IVS_INT32 GetVolume(unsigned int uiPlayHandle, unsigned int* puiVolumeValue);

	CAudioObj* GetAudioObj(IVS_ULONG ulHandle);

	IVS_ULONG GetTalkBackPlayHandleByRtspHandle(IVS_ULONG ulRtspHandle);

	IVS_ULONG GetBroadCastPlayHandleByRtspHandle(IVS_ULONG ulRtspHandle);

    IVS_ULONG GetTalkBackPlayHandleByNetHandle(IVS_ULONG ulNetHandle);

    IVS_ULONG GetBroadCastPlayHandleByNetHandle(IVS_ULONG ulNetHandle);

    // ����RTSP��NetSource��Handle��ȡ��Ӧ��VoiceCode
    IVS_INT32 GetBroadCastDeviceCodeByConnHandle(IVS_ULONG ulRtspOrNetHandle, std::string& strDeviceCode);
	//IVS_INT32 GetBroadCastVoiceCodeByRtspHandle(IVS_ULONG ulRtspHandle, std::string& strVoiceCode);
    IVS_INT32 GetTalkBackDeviceCodeByConnHandle(IVS_ULONG ulRtspOrNetHandle, std::string& strDeviceCode);

	bool IsBroadCast(IVS_ULONG ulPlayHandle) const;
private:
    CAudioObj* GetFreeTalkBack(IVS_ULONG& ulHandle);

protected:
	CStreamData m_StreamData;

	//ָ���û�����
	//void *m_pUserObj;

	CAudioObj m_audioObj;

    CUserMgr *m_pUserMgr;
	
    typedef std::map<unsigned long, CAudioObj*> HandleTalkBackMap;
    typedef HandleTalkBackMap::iterator HandleTalkBackMapIter;
    HandleTalkBackMap m_HanleTalkBackMap;

    VOS_Mutex* m_pHandleTalkBackMutex;
};



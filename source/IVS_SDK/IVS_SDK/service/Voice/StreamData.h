#pragma once
#include <map>
// #include "IVSSDKDef.h"
#include "Lock.h"
#include "hwsdk.h"
//#include "IVS_MLIB.h"

#define BASE_HANDLE_TALK_BACK 1000       // �����Խ�����Сhandleֵ

//�Զ���ͬ���࣬������Ҫ����ɿ�ƽ̨
class CStreamLock
{
public:
	CStreamLock();
    CStreamLock(HANDLE hHandle);
    ~CStreamLock();

private:
    HANDLE m_hHandle;
};

// ����Rtsp������Ϣ
typedef struct _stRTSP_INFO
{
    int     iHandle;
    int     refCont;
    char    szUrl[IVS_URL_LEN];
    bool    bIsTcpDirect;
}RTSP_INFO;
 
typedef struct tagInnerStreamInfo
{
    void   *pStream;
    HANDLE lEventHandle; //���ڶ�����ʵ��ָ��pStream����ͬ������������֧�ֶ��̵߳���
	long   lStreamHandle;
	HWND   hWnd;
    char   szDeviceID[IVS_DEV_CODE_LEN+1];
	RTSP_INFO voiceRtspInfo;
}INNER_STREAM_INFO,*pINNER_STREAM_INFO;

class CStreamData
{
public:
	CStreamData(void);
	~CStreamData(void);
	long CreateHandle();

	//����pStreamΪ���������ʵ��
	long AddStream(long lHandle, void *pStream, HWND /*hWnd*/, char* szDeviceID,RTSP_INFO* pRtspInfo);
	void* RemoveStream(long lHandle);
	void* GetStream(long lHandle);
	void* GetStream(char* pszDeviceID);
	void* GetFirstStream();
	BOOL FindStream(long lHandle);
	BOOL FindStream(char* /*pszDeviceID*/);
	long GetHandleByID(char* /*pszDeviceID*/);

private:
    //CCriticalSection m_MapLock;
	CIVSMutex m_criticalSectionEx;
    //CMap <long, long, INNER_STREAM_INFO*, INNER_STREAM_INFO*> m_MapStream;
	std::map <long,  INNER_STREAM_INFO*> m_MapStream;

};

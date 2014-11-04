/********************************************************************
 filename    :    RealPlay.h
 author      :    s00191067
 created     :    2012/11/19
 description :    ʵ������ʵ��ҵ��;
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/11/19 ��ʼ�汾
*********************************************************************/
#ifndef __REALPLAY_H__
#define __REALPLAY_H__
// #include <list>
#include "MediaBase.h"
#include "vos.h"
#include "SDKDef.h"
#include "vos_atomic.h"

#include <Windows.h>

typedef std::list<CAMERA_PLAY_INFO*>             CAMERA_PLAY_INFO_LIST;
typedef std::list<CAMERA_PLAY_INFO*>::iterator   CAMERA_PLAY_INFO_LIST_ITER;

typedef enum {
	STATUS_READY   = 0x01,
	STATUS_PLAYING = 0x02,
	STATUS_STOPPED = 0x03,
	STATUS_FINISHED = 0x04
}PLAY_STATUS;

class CRealPlay : public CMediaBase
{
public:
	CRealPlay(void);
	virtual ~CRealPlay(void);
    int StartPlay(IVS_REALPLAY_PARAM* pRealplayPara, const IVS_CHAR* pCameraCode, const START_PLAY_PARAM& stParam);//lint !e1411
	int ReStartStream(const IVS_MEDIA_PARA* pMediaPara, const IVS_CHAR* pCameraCode, const START_PLAY_PARAM& stParam);//lint !e1411

	virtual int StopStream();
public:

    /******************************************************************
	function : StartRealPlay
	description: ��ʼʵ��;
	input : pRealplayPara,ʵ��������Ϣ��pCameraCode���������ţ�hWnd��������
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	int StartRealPlay(IVS_REALPLAY_PARAM* pRealplayPara, const IVS_CHAR* pCameraCode,HWND hWnd);

    /******************************************************************
	function : ReStartRealPlay
	description: ��ʼʵ��(��������);
	input : pMediaPara,ý�������Ϣ��pCameraCode����������
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	int ReStartRealPlay(const IVS_MEDIA_PARA* pMediaPara, const IVS_CHAR* pCameraCode);

	/******************************************************************
	function : StartRealPlayCBRaw
	description: ��ʼʵ��(������);
	input : pRealplayPara,ʵ��������Ϣ��pCameraCode���������ţ�fRealPlayCallBackRaw���������ص���pUserData���û���Ϣ��pHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	int StartRealPlayCBRaw(IVS_REALPLAY_PARAM* pRealplayPara, const IVS_CHAR* pCameraCode, RealPlayCallBackRaw fRealPlayCallBackRaw, void* pUserData );

	/******************************************************************
	function : StartRealPlayCBFrame
	description: ��ʼʵ��(YUV��);
	input : pRealplayPara,ʵ��������Ϣ��pCameraCode���������ţ�fRealPlayCallBackFrame��YUV�����ص���pUserData���û���Ϣ��pHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	int StartRealPlayCBFrame(IVS_REALPLAY_PARAM* pRealplayPara, const IVS_CHAR* pCameraCode, RealPlayCallBackFrame fRealPlayCallBackFrame, void* pUserData );

	/******************************************************************
	function : StopRealPlay
	description: ֹͣʵ��;
	input : ulHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	int StopRealPlay();

	/******************************************************************
	function : AddPlayWindow
	description: ��Ӳ��Ŵ���;
	input : hWnd����������ulHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	int AddPlayWindow(HWND hWnd);
	/******************************************************************
	function : DelPlayWindow
	description: ɾ�����Ŵ���;
	input : hWnd����������ulHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	int DelPlayWindow(HWND hWnd);

	/*****************************************************************************
	�������ƣ�AddPlayWindowPartial
	�������������Ӿֲ�ͼ�񲥷Ŵ���
	���������hWnd��       ���ھ��
			 pPartial��   �ֲ�ͼƬ����
	���������NA
	�� �� ֵ��int
	****************************************************************************/
	int AddPlayWindowPartial(HWND hWnd, void* pPartial);

	/*****************************************************************************
	�������ƣ�UpdateWindowPartial
	�������������¾ֲ��Ŵ�Դ����
	���������hWnd��       ���ھ��
			 pPartial��   �ֲ�����
	���������NA
	�� �� ֵ��int
	****************************************************************************/
	int UpdateWindowPartial(HWND hWnd, void* pPartial);

	/******************************************************************
	function : IsExistHWnd
	description: �жϴ��ھ���Ƿ����;
	input : hWnd��������
	output : NA
	return : ����-True��������-False
	*******************************************************************/
	bool IsExistHWnd(const HWND& hwnd);

	/******************************************************************
	function : DelHWndfromList
	description: ɾ��WndList�е�Wnd;;
	input : hWnd��������
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    int DelHWndfromList(const HWND& hwnd);

	/******************************************************************
	function : GetCameraCode
	description: ��ȡCameraCode
	input : ��
	output : NA
	return : ��������
	*******************************************************************/
	char* GetCameraCode();

	void SetHandle(unsigned long ulHandle);

	unsigned long GetHandle();

    void DoRawFrameCallBack(IVS_RAW_FRAME_INFO* pstRawFrameInfo, IVS_CHAR* pFrame, IVS_UINT32 uFrameSize);

    void DoFrameCallBack(IVS_INT32 iStreamType, IVS_VOID* pFrameInfo);

    /******************************************************************
     function   : ReuseStream
     description: ���ø���
     input      : HWND hWnd �µĴ��ھ��;
     output     : NA
     return     : int
    *******************************************************************/
    int ReuseStream(HWND hWnd);

    virtual void RetSet();

    bool IsExistStream(const char* pszCameraID, IVS_STREAM_TYPE eStreamType) const;

	/******************************************************************
	function : PlaySound
	description: ������·����;
	input : uiPlayHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    int PlaySound();

	/******************************************************************
	function : StopSound
	description: ֹͣ������·����;
	input : uiPlayHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    int StopSound();

	/******************************************************************
	function : SetVolume
	description: ��������;
	input : uiVolumeValue������ֵ��uiPlayHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    int SetVolume(unsigned int uiVolumeValue);

	/******************************************************************
	function : GetVolume
	description: ��ȡ����;
	input : puiVolumeValue������ֵ��uiPlayHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    int GetVolume(unsigned int* puiVolumeValue);
	
	/******************************************************************
	function : GetPlayWindows
	description: ��ȡ���Ŵ���;
	input : pBuf����������size������������
	output : NA
	return : �ڲ���ʵ���Ĵ�����Ŀ
	*******************************************************************/
	int GetPlayWindows(HWND *pBuf, int size);

	/******************************************************************
	function : GetCameraPlayInfo
	description: ���ز��ŵĲ���;
	input : cameraPlayInfo����ǰ�ڲ��ŵĴ��ڵ���Ϣ
	output : NA
	return : ��
	*******************************************************************/
	void GetCameraPlayInfo(CAMERA_PLAY_INFO& cameraPlayInfo)const;

	long GetRef();
	long PutRef();

private:
    /******************************************************************
	 function   : GetUrlRealPlay
	 description: ��SCU��ȡý��url;
	 input      : int & iProtocolType
	 input      : ST_MEDIA_RSP * mediaRsp
	 output     : NA
	 return     : int
	*******************************************************************/
	int DoGetUrlRealPlay();

	IVS_INT32 GetLocalIpPort();

private:
	typedef std::list<HWND> WndList;            
	typedef WndList::iterator WndListIter;

    WndList                 m_WndList;          // ��Ⱦ�����б�;
	CAMERA_PLAY_INFO        m_CameraPlayInfo;   // ������Ϣ
    RealPlayCallBackRaw m_fRealPlayCallBackRaw;         // �������Ļص�����
    void* m_pRealPlayUserDataRaw;
    RealPlayCallBackFrame m_fRealPlayCallBackFrame;     // �����֡���ݵĻص�����
    void* m_pRealPlayUserDataFrame;
    VOS_Mutex*          m_pRealPlayRawCBMutex;              // �������ص���;
    VOS_Mutex*          m_pRealPlayFrameDecoderCBMutex;   // �������ص���;      
#ifdef WIN32
	vos_atomic_t        m_RefCounter;
#else
	atomic_t			m_RefCounter;
#endif

	PLAY_STATUS         m_nStatus;
};

#endif


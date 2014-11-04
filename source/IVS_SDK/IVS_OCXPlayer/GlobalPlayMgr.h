/********************************************************************
 filename	    : PlayMgr.h
 author		    : w00210470
 created		: 2013/01/07
 description	: �����ž����OCX�����ϵ
 copyright	    : Copyright (C) 2011-2015
 history	    : 2012/01/07 ��ʼ�汾
*********************************************************************/
#ifndef __IVS_OCX_PLAY_MGR_H__
#define __IVS_OCX_PLAY_MGR_H__

#include "Lock.h"
#include "SDKDef.h"

class CVideoPane;     //lint !e763
class CGlobalPlayMgr; //lint !e763

#define BTN_LOCAL_RECORD  0x01


// ���Ű�ť״̬
typedef struct {
	bool bRecording;
	unsigned long PlayState;
	int  Volume;
} PLAY_STATE;

typedef std::list<CVideoPane*> PANEL_LIST;//lint !e761
typedef PANEL_LIST::iterator   PANEL_ITER;//lint !e761

class CPlayObject
{
	friend CGlobalPlayMgr;
public:
	void InsertPanel(CVideoPane *pPanel);
	void DeletePanel(CVideoPane *pPanel);
	void SetCamID(const TCHAR *szCamID);
	void SetStreamType(unsigned long t) { m_uStreamType = t; }
	unsigned long  GetPalyState()const { return m_PanelState.PlayState; }
	int  GetRef()const { return m_ref; }
	CPlayObject(unsigned long handle, bool bRealStream);
	~CPlayObject();
private:
	CPlayObject();
private:
    int           m_ref;
	bool          m_bRealStream;
	unsigned int  m_uStreamType;
	unsigned long m_ulHandle;
	PLAY_STATE    m_PanelState;
	PANEL_LIST    m_Panels;
	TCHAR          m_CamID[IVS_DEV_CODE_LEN + 1];
};

// ʵʱ��Ƶ��������
typedef struct {
    unsigned long  playHandle;
	unsigned int   StreamType;
	TCHAR           CamID[IVS_DEV_CODE_LEN + 1];
} CAM_REAL_STREAM;

// CGlobalPlayMgr
// ȫ�־�̬�࣬����PlayHandle �� CVideoPane ����
class CGlobalPlayMgr
{
public:
	static CGlobalPlayMgr& Instance();
	~CGlobalPlayMgr();

	int  Init();
	void CleanUp()const;

	void DumpPlayMsg(std::string &strBuf);
	int RegRealPlayHandle(unsigned long Handle, CVideoPane *pPanel, const TCHAR *szCamera, unsigned int Type);
	int RegPlayBackHandle(unsigned long Handle, CVideoPane *pPnel, const TCHAR *szCam);

	int  RegDownLoadHandle(unsigned long Handle);
	void UnRegDownLoadHandle(unsigned long Handle);

	int UnRegPlayHandle(unsigned long Handle, CVideoPane *pPanel);

	void SyncButtonState(unsigned long Handle, int btn, long value);

	void SetDisplayScale(int uType)const;
	int  GetDisplayScale()const { return m_ScaleType; }
	unsigned long  GetPlayState(unsigned long Handle);

    unsigned long TryRealStreamShare(const TCHAR *szCamID, unsigned int StreamType, CVideoPane *pPanel);
	inline void EnableStreamShare(bool b) { m_bStreamShare = b; }

	void HandleLocalRecordException(unsigned long Handle, int EventID, void *EventData, size_t DataSize, void *pUserData);
	void HandleRealPlayException(unsigned long Handle, int EventID, void *EventData, size_t DataSize, void *pUserData);
	void HandlePlayBackException(unsigned long Handle, int EventID, void *EventData, size_t DataSize, void *pUserData);
	void HandleDownLoadEvent(unsigned long Handle, int EventID, void *EventData, size_t DataSize, void *pUserData);
	void HandleWaterAlarmEvent(unsigned long Handle, int EventID, void *EventData, size_t DataSize, void *pUserData);

	int HandleEvent(unsigned long Handle, int EventID, void *EventData, size_t DataSize, void *pUserData)const;
	int DispatchEvent(unsigned long Handle, long EventID, void *data)const;

private:
	CGlobalPlayMgr();
	CGlobalPlayMgr(const CGlobalPlayMgr& rhs);

private:
	// ����¼���쳣�����Լ��¼��ϱ�

	void PostRecordSplitEvent(CIVS_OCXPlayerCtrl *pOcxCtrl,   long EventID, IVS_LOCAL_RECORD_SUCCESS_INFO *pInfo, unsigned long ulHandle)const;
	void PostRecordGenericEvent(CIVS_OCXPlayerCtrl *pOcxCtrl, long EventID, int code, int PanelID)const;
	void PostPlayBackEvent(bool bLocal, CIVS_OCXPlayerCtrl *pOcxCtrl, int Exceptioncode, unsigned long WindID)const;
	void PostDownLoadEvent(CIVS_OCXPlayerCtrl *pOcxCtrl, long EventID, unsigned long ulHandle, int Code)const;
	
	void PostWaterAlarmEvent(CIVS_OCXPlayerCtrl *pOcxCtrl,long EventID, IVS_ALARM_WATERMARK_INFO *pInfo)const;
    // ʵ������쳣�����¼��ϱ�

private:
	// helper functions
	CPlayObject *GetPlayObject(unsigned long Handle);

private:
	// �ڲ���Ա����
	typedef std::map<unsigned long, CPlayObject*> PLAY_HANDLE_MAP;
	typedef PLAY_HANDLE_MAP::iterator PLAY_HANDLE_ITER;

//	typedef std::map<std::string, CAM_REAL_STREAM> REAL_CAM_MAP;
//	typedef REAL_CAM_MAP::iterator                 REAL_CAM_ITER;

	typedef std::list<CAM_REAL_STREAM> REAL_CAM_LIST;
	typedef REAL_CAM_LIST::iterator    REAL_CAM_ITER;

	typedef std::list<unsigned long>   DOWNLOAD_LIST;
	typedef DOWNLOAD_LIST::iterator    DOWNLOAD_ITER;


    int             m_ScaleType;         // �Ƿ�ȫ���ڲ��ţ�ȫ�ֱ�־
	bool            m_bStreamShare;
	bool            m_bInited;
	PLAY_HANDLE_MAP m_HandleMap;

	//REAL_CAM_MAP    m_RealStreamMap;
    REAL_CAM_LIST   m_RealStreamList;

    DOWNLOAD_LIST   m_DownLoads;

	CIVSMutex       m_Mutex;

};


class CGlobalSoundMgr
{
public:
	static CGlobalSoundMgr& Instance();
	~CGlobalSoundMgr();
	void SetSoundingPanel(CVideoPane *pPanel);
	void Reset() { m_pSoundingPanel = NULL; }
	CVideoPane *GetSoundingPanel() { return m_pSoundingPanel; }

private:
	CGlobalSoundMgr();
	CVideoPane *m_pSoundingPanel;
	CIVSMutex	m_Mutex;

};
#endif  // __IVS_OCX_PLAY_MGR_H__


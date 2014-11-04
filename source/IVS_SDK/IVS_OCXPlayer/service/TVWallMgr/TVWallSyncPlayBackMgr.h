/********************************************************************
 filename	    : TVWallSyncPlayBackMgr.h 
 author		    : f00152768
 created		: 2013/03/21
 description	: ����ǽͬ���ط�����ǽ����
 copyright	    : Copyright (C) 2011-2015
 history	    : 2013/03/21 ��ʼ�汾
*********************************************************************/
#ifndef _H_TVWALL_SYNC_PLAYBACK_MGR_H_
#define _H_TVWALL_SYNC_PLAYBACK_MGR_H_
#include "windows.h"
#include "hwsdk.h"
#include <list>
#include "SyncPlayBackMgr.h" 

class CVideoPane;

/*
 * FormatTime
 * yyyyMMddHHmmss
 */
#define FORMAT_TIME(t, buf)                                   \
	do {                                                      \
       struct tm _tm;                                         \
       gmtime_s(&_tm, &(t));                                  \
       _snprintf(buf, 14, "%04d%02d%02d%02d%02d%02d",         \
	         _tm.tm_year + 1900, _tm.tm_mon + 1, _tm.tm_mday, \
	         _tm.tm_hour, _tm.tm_min, _tm.tm_sec);            \
	} while (0)

//ͬ����ǽ����
class CTVWallSyncTask
{
public:
	CTVWallSyncTask(ULONG ulTVwallID, 
		IVS_INT32 uiSessionID,
		CVideoPane* pVideoPane,
		const IVS_CHAR *pCamID, 
		const IVS_PLAYBACK_PARAM *pPlaybackParam, 
		const IVS_TVWALL_PARAM* pTVWallParam,
		const REC_TIME_SPAN& timeSpan);
	~CTVWallSyncTask();

public:
	IVS_INT32 StartTVWallPlayBack();
	IVS_INT32 StopTVWallPlayBack();
	IVS_INT32 GetSessionID(void) const {return m_SessionID;}
	ULONG GetCurWndID() const {return m_ulCurWndID;}
private:
	ULONG m_TVwallID;//����ǽID
	ULONG m_ulCurWndID;//��ǰ�Ĵ���ID
	IVS_CHAR m_CameraID[IVS_DEV_CODE_LEN+1];//�����code
	IVS_INT32 m_SessionID;//SessionID
	IVS_PLAYBACK_PARAM m_PlayBackParam;
	unsigned long long m_llStartTime;
	unsigned long long m_llEndTime;
	unsigned long long m_llCurTime;//��ǰʱ��
	bool m_bIsHasOnTVWall;//�Ƿ�����ǽ
	CVideoPane* m_pVideoPane;//��ǰ����
public:
    IVS_TVWALL_PARAM m_TVWallParam;
public:
	const unsigned long long& GetStartTime() const {return m_llStartTime;}
	const unsigned long long& GetEndTime() const {return m_llEndTime;}
	bool  GetOnTVwallStatus() const {return m_bIsHasOnTVWall;};
	void SetCurTime(unsigned long long ullcurTime){m_llCurTime = ullcurTime;}
	CVideoPane* GetVideoPane() {return m_pVideoPane;}
private:
	CTVWallSyncTask(){}//lint !e1744
};

typedef std::list<CTVWallSyncTask*> TVWALL_SYNC_TASK;
typedef TVWALL_SYNC_TASK::iterator TVWALL_SYNC_TASK_ITER;

//
class CTVWallSyncPlayBackMgr
{
private:
	CTVWallSyncPlayBackMgr();
	~CTVWallSyncPlayBackMgr();
public:
	IVS_INT32 StartTVwallSyncPlayBack();//��������ǽ�ط�ͬ����ǽ
	IVS_INT32 StopTVwallSyncPlayBack();//ֹͣ����ǽ�ط�ͬ����ǽ
	IVS_INT32 StopTVWallPlayBackByWndID(ULONG ulWndID);//ֹͣ�����Ӧ����ǽ
	IVS_INT32 InsertTask(ULONG ulTVwallID, 
		IVS_INT32 uiSessionID,
		CVideoPane* pVideoPane,
		const char *pCamID, 
		const IVS_TVWALL_PARAM* pTVWallParam,
		const IVS_PLAYBACK_PARAM *pPlaybackParam,
		const REC_TIME_SPAN& timeSpan);
	unsigned long long GetCurentTime(const CVideoPane *pVideoPane);

    //��ͣ����ǽ��ͬ���ط�
    IVS_INT32 PauseTVWallSyncPlayBack();

    //��������ǽ��ͬ���ط�
    IVS_INT32 ResumeTVWallSyncPlayBack();

    //ָ�������Ƿ��ǵ���ǽ��ͬ���ط�
    bool IsTVWallSyncPlayBack(ULONG ulWndID);
private:
	TVWALL_SYNC_TASK m_Task;//��ǰ�ĵ���ǽ������
	typedef std::map<CVideoPane*,unsigned long long> VIDEOPANE_TIME_MAP;
	typedef VIDEOPANE_TIME_MAP::iterator VIDEOPANE_TIME_MAP_ITER;
	VIDEOPANE_TIME_MAP m_VideoTimeMap;
	IVS_INT32 m_iSessionID;
	static unsigned int _stdcall TVwallSyncScheduleProc(LPVOID lpParameter);
public:
	bool m_bStart;//�Ƿ����߳�
	CIVSMutex    m_TVWallMutex;
	HANDLE       m_hThread;
public:
	static CTVWallSyncPlayBackMgr& Instance();

};

#endif//_H_TVWALL_SYNC_PLAYBACK_MGR_H_

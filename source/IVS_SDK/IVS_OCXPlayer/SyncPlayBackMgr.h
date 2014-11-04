/********************************************************************
 filename	    : SyncPlayBackMgr.h 
 author		    : w00210470
 created		: 2013/01/31
 description	: ͬ���طŹ���
 copyright	    : Copyright (C) 2011-2015
 history	    : 2012/01/31 ��ʼ�汾
*********************************************************************/
#ifndef __IVS_PLAY_BACK_SYNC_H__
#define __IVS_PLAY_BACK_SYNC_H__

#include <time.h>    //time_t
#include <list>
#include <Windows.h> //thread
#include "Lock.h"
#include "hwsdk.h"   //IVS_DEV_CODE_LEN

#define STATE_WAITING  0x00
#define STATE_PLAYING  0x01

// ����ģʽ
enum {
    PLAY_NORMAL = 0,
	PLAY_STEPBK = 1,
	PLAY_STEPFR = 2,
};//lint !e713 !e726


// ¼��ʱ��ڵ�
typedef struct {
  time_t Start;
  time_t End;
} REC_TIME_SPAN;

typedef std::list<REC_TIME_SPAN> REC_TIME_LIST;
typedef REC_TIME_LIST::iterator  REC_TIME_ITER;
typedef REC_TIME_LIST::reverse_iterator REC_TIME_RITER;

class CVideoPane;//lint !e763
class CSyncPlayBackMgr;//lint !e763

// ͬ����������
class CSyncTask
{
	friend CSyncPlayBackMgr;
public:
	CSyncTask(CVideoPane *pPanel, const char *pCamID, unsigned int uProto, float dSpeed);
	~CSyncTask();
	void InsertRecord(REC_TIME_SPAN &Span); 
	unsigned long long GetCurrentTick();
	
	//unsigned int GetRecListSize() { return m_RecList.size(); }

	bool IsFailedAll() const { return m_bFailure;/*(m_uTotalRecs > 0) && (m_uTotalRecs == m_uFailures); */}
	int  GetPanelID()const ;
	void UpdateRecSegment();

private:
	int StartPlayExpired(time_t now, bool bHasRuning);
	bool IsTimeValid(time_t tm);

	int GetFirstRecSpanAfter(time_t tm, REC_TIME_SPAN& RecSpan);
	int GetFirstRecSpanBefore(time_t tm, REC_TIME_SPAN& RecSpan);


	void PostSyncPlayMsg(HWND hWnd, bool bStarted)const;

	int SetPlayTimePlaying(time_t tm);
	int SetPlayTimeWaiting(time_t tm);


	int SetPlaySpeed(float dSpeed);
	int StepForWard();
	int StepBackWard();

	void StopPlay();

	int  PausePlay();
	int  ResumePlay();

	void SetPlayMode(unsigned int mode) ;

	long GetLastError() const     { return m_LastError; }
	void SetLastError(long error) { m_LastError = error; }


private:
	bool               m_bStepPlay;
	bool               m_bPlaying;    // ����״̬
	REC_TIME_ITER      m_Current;     // ��ǰ����ʱ���
	int                m_RecIndex;    // ��ǰ����


	//REC_TIME_RITER     m_CurrentBK;      // ����ģʽ��һ��¼���
	//REC_TIME_ITER      m_CurrentFR;      // ����ģʽ��һ��¼���
		
	unsigned long long m_CurrentTick; // ��ǰ���ŵ�ʱ���
	CVideoPane*    m_pPanel;  // ���ھ��
	unsigned int   m_uProto;  // Э������
    float          m_fSpeed;  // �����ٶ�
	REC_TIME_LIST  m_RecList; // ¼�񲥷�����
	char           m_CamID[IVS_DEV_CODE_LEN + 1];

	unsigned int   m_uTotalRecs;
	unsigned int   m_uFailures;

	unsigned int   m_uPlayMode;
	bool           m_bPause;

    time_t         m_tmLastTry;
    bool           m_bFailure;
	long           m_LastError;

private:
	CSyncTask() {}//lint !e1744
};

typedef std::list<CSyncTask*> TASK_LIST;
typedef TASK_LIST::iterator   TASK_ITER;

// ���Ȳ���
#define SCHED_ACCEL 0x01
#define SCHED_DECEL 0x02

// ʱ������
#define SCHEDULE_TIME_DIFF_USEC   8000


// ͬ���ط��������
class CSyncPlayBackMgr
{
public:
	static CSyncPlayBackMgr &Instance();
	int InsertSyncTask(CVideoPane *pPanel, const char* pCamID, 
		       unsigned int uProto, float dSpeed, REC_TIME_SPAN &pRecord);
	int DeleteSyncPanel(CVideoPane *pPanel);

	int StepPlayBack();
	int StepPlayForward();

	int SetPlaySpeed(float dSpeed);
	float GetPlaySpeed()const { return m_fSpeed; };
	int SetPlayTime(unsigned long offset);
	int GetPlayedTime(IVS_PLAYBACK_TIME &tm)const;
	int StartSyncPlay();
	void StopSyncPlay();
	int PauseSyncPlay();
	int ResumeSyncPlay();
	IVS_BOOL GetPlayStatus() const { return m_bPaused ? 0 : 1; };// 0-��ͣ��1-����;

	void SetLastError(CVideoPane *pPanel, long code);


	void StopAllPlayResource();
private:
	CSyncPlayBackMgr();
	~CSyncPlayBackMgr();

private:
	static unsigned int __stdcall SyncScheduleProc(LPVOID lpParameter);
	int DoSchedule();

	bool CheckSchduleFailureAll();



    // schedule helpers
	unsigned long long GetRefTick();
	unsigned long long GetRefTickBackPlay();

	time_t UpdateWaitingTask();
	time_t UpdateWaitingTaskBackPlay();

	bool VerifyTime(time_t tm);

	void ScheduleTask(unsigned long long refTick);
	void ScheduleTaskBackMode(unsigned long long refTick)const;
    void StartTask(unsigned long long refTick);
	void StopTaskAll();
	void CleanTaskAll();
    
	void PostExceptionEvent(CIVS_OCXPlayerCtrl *pOcxCtrl, int Exceptioncode, unsigned long WindID)const;//lint !e601


private:
	bool         m_bForwarPlay; 
	bool         m_bCursorLeftMove;

    unsigned int m_uPlayMode;

	TASK_LIST    m_TaskRunning;
	TASK_LIST    m_TaskWaiting;
    float        m_fSpeed;
    time_t       m_tmPlayTime;
	time_t       m_LastPlayTime;
	time_t       m_tmStart;
	time_t       m_tmEnd;
	bool         m_bStarted;  
	bool         m_bThreadRun;
	HANDLE       m_hThread;
	unsigned int m_uStrategy;  // ���Ȳ���
	CIVSMutex    m_Mutex;
	bool         m_bPaused;
	bool         m_bTimeSetted;

};

#endif // __IVS_PLAY_BACK_SYNC_H__


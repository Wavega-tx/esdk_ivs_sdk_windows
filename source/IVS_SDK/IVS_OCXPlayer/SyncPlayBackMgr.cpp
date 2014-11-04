#include <afx.h>         // for ASSERT
#include <stdafx.h>
#include <process.h>     // _beginthread, _endthread s
#include "IVS_SDK.h"     // sdk Interface
#include "bp_log.h"
#include "SyncPlayBackMgr.h" 
#include "VideoPane.h"
#include "IVS_OCXPlayerCtrl.h"
#include "TranVideoTitleBar.h"
#include "GlobalPlayMgr.h"
#include "OCXPlayBackXMLProcess.h"

#define INVALID_TIME  0x7FFFFFFF

#define TIME_RETRY   5      // ʧ�����Եȴ�ʱ��5s
#define MAX_RETRY    16     // ���ʧ�����Դ���

/*
 * CSyncPlayBackMgr
 * ����ģʽĬ�����ã�����ͨ����ͣ
 */
//lint -e831 831������Ҫ���

CSyncPlayBackMgr::CSyncPlayBackMgr()
{
	m_tmPlayTime = INVALID_TIME;
	m_tmStart    = INVALID_TIME;
	m_tmEnd      = 0;
	m_bStarted   = false;
	m_bThreadRun = true;
	m_hThread    = NULL;
	m_bForwarPlay = true;
	m_uStrategy  = SCHED_ACCEL/*SCHED_DECEL*/;
	m_uPlayMode = PLAY_NORMAL;
	m_bPaused   = false;

	m_bTimeSetted = true;
	m_LastPlayTime = 0;
	m_bCursorLeftMove = false;
	m_fSpeed = 1.0;

}//lint !e1744

/*
 * CSyncPlayBackMgr
 */
CSyncPlayBackMgr::~CSyncPlayBackMgr()
{
	ASSERT(m_bStarted);//lint !e1551
	if (m_bStarted)//lint !e774
	{
		StopSyncPlay();//lint !e1551
	}
}//lint !e1740

/*
 * Instance
 */
CSyncPlayBackMgr& CSyncPlayBackMgr::Instance()
{
	static CSyncPlayBackMgr Mgr;
	return Mgr;
}

/*
 * InsertSyncTask
 * ����ͬ����������, ��ͬ�������߳�����
 */
int CSyncPlayBackMgr::InsertSyncTask(CVideoPane *pPanel, const char *pCamID,
	        unsigned int uProto, float fSpeed, REC_TIME_SPAN &RecSpan)
{
	ASSERT(NULL != pPanel && NULL != pCamID && pCamID[0]);//lint !e831
    if (NULL == pPanel || NULL == pCamID || !pCamID[0])//lint !e774 !e831
	{
		BP_RUN_LOG_INF("Isnert SyncTask", "Parameters Invalid"); 
		return IVS_PARA_INVALID;
	}
	
	ASSERT(!m_bStarted);
	if (m_bStarted)//lint !e774
	{
		BP_RUN_LOG_INF("Isnert SyncTask", "SyncPlay already Started"); 
		return IVS_FAIL;
	}
	
	CAutoLock __lock(m_Mutex);

	CSyncTask *pTask = NULL;
	TASK_ITER iter = m_TaskWaiting.begin();
	TASK_ITER end  = m_TaskWaiting.end();
	for (; iter != end; iter++)
	{
		if ((*iter)->m_pPanel == pPanel)
		{
			pTask = *iter;
			break;
		}
	}
	
	if (NULL == pTask)
	{
		try {
		    pTask = new CSyncTask(pPanel, pCamID, uProto, fSpeed);
		}
		catch(...)
		{
		    BP_RUN_LOG_INF("Create SyncTask", "Out of mem"); 
			return IVS_FAIL;
		}
		m_TaskWaiting.push_front(pTask);
	}

	ASSERT(pTask->m_fSpeed == fSpeed);//lint !e777
	ASSERT(pTask->m_uProto == uProto);//lint !e777

	pPanel->SetSyncPlayBackMode(true);
    pTask->InsertRecord(RecSpan);
	if (RecSpan.Start < m_tmStart)
	{
		m_tmStart = RecSpan.Start;
	}

	if (RecSpan.End > m_tmEnd)//lint !e777
	{
		m_tmEnd = RecSpan.End;
	}

	m_fSpeed     = fSpeed;
	m_tmPlayTime = m_tmStart;

	return IVS_SUCCEED;//lint !e429
}

/*
 * DeleteSyncPanel
 * ɾ�����ڵ�ͬ����������
 */
int CSyncPlayBackMgr::DeleteSyncPanel(CVideoPane *pPanel)
{
	ASSERT(NULL != pPanel);//lint !e774
	if (NULL == pPanel)//lint !e774
	{
		BP_RUN_LOG_INF("Delete SyncPanel", "Panel=NULL"); 
		return IVS_FAIL;
	}
	
	BP_RUN_LOG_INF("Delete SyncPanel", "Panel=%08x", pPanel); 
	pPanel->SetSyncPlayBackMode(false);

	CAutoLock __lock(m_Mutex);
	TASK_ITER _iter = m_TaskRunning.begin();
	TASK_ITER _end  = m_TaskRunning.end();
	for (; _iter != _end; _iter++)
	{
		CSyncTask *p = *_iter;
		ASSERT(p);
		if (p->m_pPanel == pPanel)
		{
			delete p;
			m_TaskRunning.erase(_iter);
			return IVS_SUCCEED;
		}
    }

	TASK_ITER iter = m_TaskWaiting.begin();
	TASK_ITER end  = m_TaskWaiting.end();
	for (; iter != end; iter++)
	{
		CSyncTask *p = *iter;
		ASSERT(p);
		if (p->m_pPanel == pPanel)
		{
			delete p;
			m_TaskWaiting.erase(iter);
			return IVS_SUCCEED;
		}
    }
	
	return IVS_SUCCEED;
}

/*
 * SetPlaySpeed
 * ��������ÿ��ͨ���Ĳ����ٶ�, 
 * ��������ֱ�ӷ���
 */
int CSyncPlayBackMgr::SetPlaySpeed(float fSpeed)
{
	CAutoLock __lock(m_Mutex);
	
	// ���õȴ����в����ٶ�
	TASK_ITER iter = m_TaskWaiting.begin();
	TASK_ITER end  = m_TaskWaiting.end();
	for (; iter != end; iter++)
	{
		CSyncTask *p = *iter;
		(void)p->SetPlaySpeed(fSpeed);
	}
	
	// ���ò��Ŷ��в����ٶ�
    TASK_ITER _iter = m_TaskRunning.begin();
	TASK_ITER _end  = m_TaskRunning.end();
	for (; _iter != _end; _iter++)
	{
		CSyncTask *p = *_iter;
        int result = p->SetPlaySpeed(fSpeed);
		if (IVS_SUCCEED != result)
		{
			BP_RUN_LOG_INF("Sync Mgr Set PlaySpeed", "error=[%d]", result);
			return result;
    	}
	}

    m_fSpeed = fSpeed;
	return IVS_SUCCEED;
}

/*
 * StepPlayBack
 * ����ÿ��ͨ�������ط�
 */
int CSyncPlayBackMgr::StepPlayBack()
{
	CAutoLock __lock(m_Mutex);

	TASK_ITER _iter = m_TaskRunning.begin();
	TASK_ITER _end  = m_TaskRunning.end();
	for (; _iter != _end; _iter++)
	{
		CSyncTask *p = *_iter;
		int result = p->StepBackWard();
		if (IVS_SUCCEED != result)
		{
			BP_RUN_LOG_INF("Sync Mgr Set Step BackWard", "error=[%d]", result);
			//return result;
		}
	}

	if (m_uPlayMode != PLAY_STEPBK)
	{
		TASK_ITER iter = m_TaskWaiting.begin();
		TASK_ITER end  = m_TaskWaiting.end();
		for (; iter != end; iter++)
		{
			CSyncTask *p = *iter;
			p->SetPlayMode(PLAY_STEPBK);
		}
		m_uPlayMode = PLAY_STEPBK;
	}

	m_fSpeed = -1.0;

	return IVS_SUCCEED;
}

/*
 * StepPlayForward
 * ����ÿ��ͨ������ǰ��
 */
int CSyncPlayBackMgr::StepPlayForward()
{
	CAutoLock __lock(m_Mutex);

	TASK_ITER _iter = m_TaskRunning.begin();
	TASK_ITER _end  = m_TaskRunning.end();
	for (; _iter != _end; _iter++)
	{
		CSyncTask *p = *_iter;
		int result = p->StepForWard();
		if (IVS_SUCCEED != result)
		{
			BP_RUN_LOG_INF("Sync Mgr Set Step ForWard", "error=[%d]", result);
			//return result;
		}
	}

	if (m_uPlayMode != PLAY_STEPFR)
	{
		TASK_ITER iter = m_TaskWaiting.begin();
		TASK_ITER end  = m_TaskWaiting.end();
		for (; iter != end; iter++)
		{
			CSyncTask *p = *iter;
			p->SetPlayMode(PLAY_STEPFR);
		}
		m_uPlayMode = PLAY_STEPFR;
	}

	m_fSpeed = 1.0;

	return IVS_SUCCEED;
}

/*
 * SetPlayTime
 * ���ò���ͨ���Ĳ���ʱ��, ������ʱ��������Ч
 * ������ͨ�����뵽�ȴ������У��ɵ����߳�����
 */
int CSyncPlayBackMgr::SetPlayTime(unsigned long offset)
{
	CAutoLock __lock(m_Mutex);
    TASK_ITER _iter = m_TaskRunning.begin();
	TASK_ITER _end  = m_TaskRunning.end();
	for (; _iter != _end; )
	{
		CSyncTask *p = *_iter;
        if (IVS_SUCCEED != p->SetPlayTimePlaying(m_tmStart + offset))
		{
			m_TaskRunning.erase(_iter++);
			m_TaskWaiting.push_back(p);
			continue;
		}
		_iter++;
	}

	TASK_ITER iter = m_TaskWaiting.begin();
	TASK_ITER end  = m_TaskWaiting.end();
	for (; iter != end; iter++)
	{
		CSyncTask *p = *iter;
		(void)p->SetPlayTimeWaiting(m_tmStart + offset);
	}

	time_t target = m_tmStart + offset;
	if (target < m_tmPlayTime)
	{
		m_bCursorLeftMove = true;
	}

	BP_RUN_LOG_INF("DoSchedule", "SetPlayTime Tick = %llu prev = %llu",
		m_tmStart + offset, m_tmPlayTime);

	m_tmPlayTime = m_tmStart + offset;

	if (m_TaskRunning.empty())
	{
		m_bTimeSetted = true;
	}

	return IVS_SUCCEED;
}

/*
 * GetPlayTime
 * ��ȡ����ʱ��
 */
int CSyncPlayBackMgr::GetPlayedTime(IVS_PLAYBACK_TIME &IvsTm)const
{
    //CAutoLock __lock(m_Mutex);

	struct tm _tm;
	gmtime_s(&_tm, &m_tmStart);
	snprintf(IvsTm.cPlayBackStartTime, sizeof(IvsTm.cPlayBackStartTime)- 1, 
		"%04d%02d%02d%02d%02d%02d", _tm.tm_year + 1900, _tm.tm_mon + 1, _tm.tm_mday,
		_tm.tm_hour, _tm.tm_min, _tm.tm_sec);

	gmtime_s(&_tm, &m_tmEnd);
	snprintf(IvsTm.cPlayBackEndTime, sizeof(IvsTm.cPlayBackEndTime)- 1, 
			"%04d%02d%02d%02d%02d%02d", _tm.tm_year + 1900, _tm.tm_mon + 1, _tm.tm_mday,
			_tm.tm_hour, _tm.tm_min, _tm.tm_sec);

	gmtime_s(&_tm, &m_tmPlayTime);

	snprintf(IvsTm.cPlayBackTime, sizeof(IvsTm.cPlayBackTime)- 1, 
		"%04d%02d%02d%02d%02d%02d", _tm.tm_year + 1900, _tm.tm_mon + 1, _tm.tm_mday,
		_tm.tm_hour, _tm.tm_min, _tm.tm_sec);

	return IVS_SUCCEED;
}

/*
 * StartSyncPlay
 */
int CSyncPlayBackMgr::StartSyncPlay()
{
	ASSERT(!m_bStarted);//lint !e774
	if (m_bStarted)//lint !e774
	{
		BP_RUN_LOG_INF("Start SyncPlay", "already Started");
		return IVS_SUCCEED;
	}

	ASSERT(!m_TaskWaiting.empty());
    if (m_TaskWaiting.empty())
	{
		BP_RUN_LOG_INF("Start SyncPlay", "Sync Task Empty");
		return IVS_FAIL;
	}
	m_bThreadRun = true;
    m_hThread = (HANDLE)_beginthreadex(NULL, 0, SyncScheduleProc, this, 0, NULL);
	if (NULL == m_hThread)
	{
		BP_RUN_LOG_INF("Start SyncPlay", "Create Schedule Thread Fail");
		return IVS_FAIL;
	}
	
	m_bStarted = true;
	return IVS_SUCCEED;
}

/*
 * PauseSyncPlay
 * ��ͣͬ���ط�
 */
int CSyncPlayBackMgr::PauseSyncPlay()
{
	CAutoLock __lock(m_Mutex);
	TASK_ITER _iter = m_TaskRunning.begin();
	TASK_ITER _end  = m_TaskRunning.end();
	bool bFail = false;
	int  iRet  = IVS_SUCCEED;


	for (; _iter != _end; _iter++)
	{
		CSyncTask *p = *_iter;
		iRet = p->PausePlay();
		if (IVS_SUCCEED != iRet)
		{
            BP_RUN_LOG_INF("Pause SyncPlay", 
				"Pause Panel[%d] fail", p->GetPanelID());
			bFail = true;
			break;
    	}
	}

	if (bFail)
	{
		TASK_ITER iter = m_TaskRunning.begin();
		TASK_ITER end  = _iter;
		for ( ; iter != end; iter++)
		{
			CSyncTask *p = *iter;
			(void)p->ResumePlay();
		}
		return iRet;
	}

	/*
	 * DTS2014010809034 
	 * Waiting״̬�Ĳ�����������Pause��־ 
	 */
	TASK_ITER iter = m_TaskWaiting.begin();
	TASK_ITER end  = m_TaskWaiting.end();
	for (; iter != end; iter++)
	{
		CSyncTask *p = *iter;
		(void)p->PausePlay();
	}

	m_bPaused = true;
	return IVS_SUCCEED;
}

/*
 * ResumeSyncPlay
 * ��ͣͬ���ط�
 */
int CSyncPlayBackMgr::ResumeSyncPlay()
{
	CAutoLock __lock(m_Mutex);
	TASK_ITER _iter = m_TaskRunning.begin();
	TASK_ITER _end  = m_TaskRunning.end();
	for (; _iter != _end; _iter++)
	{
		CSyncTask *p = *_iter;
		if (IVS_SUCCEED != p->ResumePlay())
		{
			BP_RUN_LOG_INF("Resume SyncPlay", 
				"Resume Panel[%d] fail", p->GetPanelID());
		}
	}

	/*
	 * DTS2014010809034 
	 * �ȴ�״̬���������Pause��־
	 */
	TASK_ITER iter = m_TaskWaiting.begin();
	TASK_ITER end  = m_TaskWaiting.end();
	for (; iter != end; iter++)
	{
		CSyncTask *p = *iter;
		(void)p->ResumePlay();
	}
	
	m_bPaused = false;
	return IVS_SUCCEED;
}

/*
 * StopSyncPlay
 */
void CSyncPlayBackMgr::StopSyncPlay()
{
	if (!m_bStarted)
	{
	   return;	
	}


	m_bThreadRun = false;
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	m_hThread  = NULL;
	m_bStarted = false;
	m_tmPlayTime = INVALID_TIME;
	m_tmStart    = INVALID_TIME;

	// ����ֲ��Ŵ���������Դ
	//StopAllPlayResource();

	CleanTaskAll();

	m_tmPlayTime = INVALID_TIME;
	m_tmStart    = INVALID_TIME;
	m_tmEnd      = 0;
	m_bStarted   = false;
	m_bThreadRun = true;
	m_hThread    = NULL;
	m_bForwarPlay = true;
	m_uStrategy  = SCHED_ACCEL;
	m_uPlayMode = PLAY_NORMAL;
	m_bPaused   = false;

	m_bTimeSetted = true;
}

/*
 * CleanTask
 * ������������
 */
void CSyncPlayBackMgr::CleanTaskAll()
{
	ASSERT(!m_bStarted);
	
	TASK_ITER _iter = m_TaskRunning.begin();
	TASK_ITER _end  = m_TaskRunning.end();
	for (; _iter != _end; _iter++)
	{
		CSyncTask *p = *_iter;
		CVideoPane *pPanel = p->m_pPanel;
		pPanel->SetSyncPlayBackMode(false);
		delete p;
    }

	TASK_ITER iter = m_TaskWaiting.begin();
	TASK_ITER end  = m_TaskWaiting.end();
	for (; iter != end; iter++)
	{   
		CSyncTask *p = *iter;
		CVideoPane *pPanel = p->m_pPanel;
		pPanel->SetSyncPlayBackMode(false);
		delete p;
	}
	m_TaskRunning.clear();
	m_TaskWaiting.clear();
}

/*
 * StopAllPlayResource
 * ����������Դ�������ֲ��Ŵ�������
 */
void CSyncPlayBackMgr::StopAllPlayResource()
{
	ASSERT(!m_bStarted);
	CIVS_OCXPlayerCtrl *pCtrl = NULL;
	TASK_ITER _iter = m_TaskRunning.begin();
	TASK_ITER _end  = m_TaskRunning.end();
	for (; _iter != _end; _iter++)
	{
		CSyncTask *p = *_iter;
		CVideoPane *pPanel = p->m_pPanel;
		pCtrl = pPanel->GetOCXPlayerCtrl();
		pCtrl->StopPlayResource(pPanel);
    }

	TASK_ITER iter = m_TaskWaiting.begin();
	TASK_ITER end  = m_TaskWaiting.end();
	for (; iter != end; iter++)
	{   
		CSyncTask *p = *iter;
		CVideoPane *pPanel = p->m_pPanel;
		pCtrl = pPanel->GetOCXPlayerCtrl();
		pCtrl->StopPlayResource(pPanel);
	}
}

/* 
 * PostExceptionEvent
 * �����쳣�¼�
 */
void CSyncPlayBackMgr::PostExceptionEvent(CIVS_OCXPlayerCtrl *pOcxCtrl, 
	    int Exceptioncode, unsigned long WindID)const//lint !e18
{//lint !e18 !e1039
	 PostMessage(pOcxCtrl->GetSafeHwnd(), WM_EVENT_REMOTE_PLAYBACK_CALLBACK,
		 (WPARAM)WindID, (LPARAM)Exceptioncode);//lint !e18
}//lint !e818


/*
 * SyncScheduleProc
 * ͬ�����ſ����߳�
 */
unsigned int __stdcall CSyncPlayBackMgr::SyncScheduleProc(LPVOID lpParameter)
{
	CSyncPlayBackMgr *pSyncMgr = (CSyncPlayBackMgr *)lpParameter;
	ASSERT(NULL != pSyncMgr);//lint !e774
	if (NULL == pSyncMgr)//lint !e774
	{
		_endthreadex(0);
		return 0;
	}
    
	while (pSyncMgr->m_bThreadRun)
	{
    	if (IVS_SUCCEED != pSyncMgr->DoSchedule())
		{
			if (!pSyncMgr->m_TaskWaiting.empty())
			{
				CSyncTask  *pTask  = pSyncMgr->m_TaskWaiting.front();
				CVideoPane *pPanel = pTask->m_pPanel;
				CIVS_OCXPlayerCtrl *pCtrl = pPanel->GetOCXPlayerCtrl();

     			if (NULL != pCtrl)
				{
					// ����ͨ��¼��ȫ��ʧ�ܣ����ϱ��쳣��CU
					if (pSyncMgr->CheckSchduleFailureAll())
					{
                        pSyncMgr->PostExceptionEvent(pCtrl, IVS_PLAYER_RET_RTSP_ABNORMAL_STOP, 0);//lint !e1705
					}
					else
					{
						pSyncMgr->PostExceptionEvent(pCtrl, IVS_PLAYER_RET_RTSP_NORMAL_STOP, 0);//lint !e1705
					}
    			}
    		}
			break;
		}
		Sleep(100);
	}

	pSyncMgr->StopTaskAll();
	_endthreadex(0);
	return 0;
}

/*
 * GetRefTick
 * ��ȡ�ο�ʱ��, ���Ŷ���Ϊ�գ����صȴ����е����粥��ʱ��
 * ���ͨ�����٣���¼����ʱ�����ֵ
 * ����ͨ�����٣���¼����ʱ����Сֵ
 */
unsigned long long CSyncPlayBackMgr::GetRefTick()
{
	if (m_TaskRunning.empty())
	{
		return (unsigned long long)m_tmPlayTime * 1000;
	}

	unsigned long long RefTick = 
		(SCHED_ACCEL == m_uStrategy) ? 0 : (unsigned long long)(-1);
	
	TASK_ITER iter = m_TaskRunning.begin();
	TASK_ITER end  = m_TaskRunning.end();
	for (; iter != end; iter++)
	{
		CSyncTask *pTask = *iter;
 		unsigned long long t = pTask->GetCurrentTick(); 
		if (SCHED_ACCEL == m_uStrategy)
		{
			if (t > RefTick)
			{
				RefTick = t;
			}
		}
		else
		{
			if (t < RefTick)
			{
                RefTick = t;
			}
		}
	}
	return RefTick;
}

/*
 * GetRefTickBackPlay
 * ����ģʽ�»�ȡ�ο�ʱ��
 *
 */
unsigned long long CSyncPlayBackMgr::GetRefTickBackPlay()
{
	if (m_TaskRunning.empty())
	{
		return (unsigned long long)m_tmPlayTime * 1000;
	}

	unsigned long long RefTick = 
		(SCHED_ACCEL == m_uStrategy) ? (unsigned long long)(-1) : 0;

	TASK_ITER iter = m_TaskRunning.begin();
	TASK_ITER end  = m_TaskRunning.end();
	for (; iter != end; iter++)
	{
		CSyncTask *pTask = *iter;
		unsigned long long t = pTask->GetCurrentTick(); 
		if (SCHED_ACCEL == m_uStrategy)
		{
			if (t < RefTick)
			{
				RefTick = t;
			}
		}
		else
		{
			if (t > RefTick)
			{
				RefTick = t;
			}
		}
	}
	return RefTick;
}

/*
 * ScheduleTask
 * �������ڲ����е�����
 * ����ǰ����¼��ʱ��ν�����������뵽�ȴ�������
 */
void CSyncPlayBackMgr::ScheduleTask(unsigned long long reftick)
{
	if (m_bPaused)
	{
		return;
	}

	TASK_ITER iter = m_TaskRunning.begin();
	TASK_ITER end  = m_TaskRunning.end();
	for (; iter != end; )
	{
		CSyncTask  *pTask  = *iter;
		CVideoPane *pPanel = pTask->m_pPanel; 
		if (pTask->GetLastError())
		{	
			(void)IVS_SDK_StopPlatformPlayBack(0, pPanel->GetPlayHandle()); 
			(void)CGlobalPlayMgr::Instance().UnRegPlayHandle(pPanel->GetPlayHandle(), pPanel);

        	m_TaskRunning.erase(iter++);
		    m_TaskWaiting.push_back(pTask);
			BP_RUN_LOG_INF("DoSchedule", "ScheduleTask %08x %llu %u %u %ld", pTask->m_pPanel, 
				reftick, m_TaskRunning.size(), m_TaskWaiting.size(), pTask->GetLastError());
			
			pTask->m_bPlaying = false;
     		long Error = pTask->GetLastError();

			if (IVS_PLAYER_RET_RTSP_NORMAL_STOP == Error)
			{
				pTask->UpdateRecSegment();
			}

			pTask->SetLastError(0);
			pPanel->SetPlayHandle(0);

			continue;
		}

		if (m_uPlayMode != PLAY_NORMAL)
		{
			iter++;
			continue;
		}

    	long long diff  = (long long)(pTask->m_CurrentTick - reftick);
		bool      bFast = (m_fSpeed > 0) ? (diff > 0) : (diff < 0);
        
		diff = diff < 0 ? -diff : diff;
		if (diff < SCHEDULE_TIME_DIFF_USEC)
		{
			//(void)pTask->ResumePlay();
			iter++;
    		continue;
		}

		if (SCHED_ACCEL == m_uStrategy && !bFast)
		{
			(void)pTask->SetPlayTimePlaying((time_t)(reftick/1000));
			BP_RUN_LOG_INF("DoSchedule", "Sync To Fasted %08x %llu %llu", 
				pTask->m_pPanel, pTask->m_CurrentTick, reftick);
    	}
		iter++;
	}
}




/*
 * ScheduleTaskBackMode
 * ����ģʽͬ������
 */
void CSyncPlayBackMgr::ScheduleTaskBackMode(unsigned long long reftick)const
{

}//lint !e715


bool CSyncPlayBackMgr::VerifyTime(time_t tm)
{
	TASK_ITER iter = m_TaskWaiting.begin();
	TASK_ITER end  = m_TaskWaiting.end();
	for (; iter != end; iter++)
	{
		CSyncTask *pTask = *iter;
		if (pTask->IsTimeValid(tm))
		{
			return true;
		}
	}
	return false;
}


/*
 * UpateWaitingTask
 * ���д���ͬһ������ѭ����ֹͣ
 * ���ҵȴ���������Сʱ��ֵ
 */
time_t CSyncPlayBackMgr::UpdateWaitingTask()
{
    ASSERT(m_TaskRunning.empty());

    time_t tmStart = INVALID_TIME;

	TASK_ITER iter = m_TaskWaiting.begin();
	TASK_ITER end  = m_TaskWaiting.end();
	for (; iter != end; iter++)
	{
		CSyncTask *pTask = *iter;
    	REC_TIME_SPAN rts;
		if (IVS_SUCCEED != pTask->GetFirstRecSpanAfter(m_tmPlayTime, rts))
		{
            continue;
		}

		// ȡstarttime ��Сֵ
		if (tmStart > rts.Start)
		{
			tmStart = rts.Start; 	
		}
	}
	return tmStart;
}

/*
 * UpdateWaitingTaskBackPlay
 * ����
 */
time_t CSyncPlayBackMgr::UpdateWaitingTaskBackPlay()
{
	ASSERT(m_TaskRunning.empty());

	time_t tmEnd = 0;

	TASK_ITER iter = m_TaskWaiting.begin();
	TASK_ITER end  = m_TaskWaiting.end();
	for (; iter != end; iter++)
	{
		CSyncTask *pTask = *iter;
		REC_TIME_SPAN rts;
		if (IVS_SUCCEED != pTask->GetFirstRecSpanBefore(m_tmPlayTime, rts))
		{
			continue;
		}

		// ȡendtime���ֵ
		if (tmEnd < rts.End)
		{
			tmEnd = rts.End; 	
		}
	}
	return 0 == tmEnd ? INVALID_TIME : tmEnd;
}

/*
 * StartTask
 * �������ڵ�����
 */
void CSyncPlayBackMgr::StartTask(unsigned long long refTick)
{
	time_t now     =  static_cast<long long>(refTick) / 1000;
	TASK_ITER iter = m_TaskWaiting.begin();
	TASK_ITER end  = m_TaskWaiting.end();
	for (; iter != end; )
	{
		CSyncTask *pTask = *iter;
		if (IVS_SUCCEED == pTask->StartPlayExpired(now, !!m_TaskRunning.size()))
		{
			iter = m_TaskWaiting.erase(iter);
			m_TaskRunning.push_back(pTask);
			continue;
		}
		iter++;
	}
}

/*
 * StopTaskAll
 * ֹͣ���в�������
 */
void CSyncPlayBackMgr::StopTaskAll()
{
	TASK_ITER iter = m_TaskRunning.begin();
	TASK_ITER end  = m_TaskRunning.end();
	for (; iter != end; iter++)
	{
		CSyncTask *pTask = *iter;
		pTask->StopPlay();
	}
}

/*
 * DoSchedule
 * ��ȡ����ͨ������С(���)����ʱ��
 * ����ͬ�����Ȳ��Խ��е��� 
 */
int CSyncPlayBackMgr::DoSchedule()
{
	CAutoLock __lock(m_Mutex);
	unsigned long long tick = m_fSpeed > 0 ? GetRefTick() : GetRefTickBackPlay();

	BP_RUN_LOG_INF("DoSchedule", "tick = %llu", tick);

    if (m_bCursorLeftMove)
	{
		time_t t = (long long)tick / 1000;
		if (t < (m_tmPlayTime + 1))
		{
			m_bCursorLeftMove = false;
		}
		return IVS_SUCCEED;
	}

	m_tmPlayTime = (long long)tick / 1000;
	
	ScheduleTask(tick);
	
	if (m_TaskRunning.empty())
	{
		bool bMoveToNext = true;

		if (m_bTimeSetted)
		{
			m_bTimeSetted = false;
			if (VerifyTime(m_tmPlayTime))
			{
				bMoveToNext = false;
			}
		}
	
		if (bMoveToNext/*!VerifyTime(m_tmPlayTime)*/)
		{
	        if (m_fSpeed > 0)
			{
				/*
				 * DTS2013071301473 
				 * ��ǰ������ɵ�һ��¼�����ȫ��ʧ�ܣ�m_tmPlayTimeΪ֮ǰ¼����ʼʱ��
				 * ���������ҵ��������¼������ѭ��
				 */
				m_tmPlayTime += 1;
				m_tmPlayTime = UpdateWaitingTask(); 
			}
			else
			{
				m_tmPlayTime -= 1;
				m_tmPlayTime = UpdateWaitingTaskBackPlay(); 
			}
    		
			if (m_tmPlayTime == INVALID_TIME)
			{
				return IVS_FAIL;
			}
			tick = (unsigned long long)m_tmPlayTime * 1000;
		}
	}
	StartTask(tick);
	return IVS_SUCCEED;
}

/*
 * CheckSchduleFailureAll
 * ����Ƿ�ȫ������ʧ�ܣ��ϱ��澯
 */
bool CSyncPlayBackMgr::CheckSchduleFailureAll()
{
	TASK_ITER iter = m_TaskWaiting.begin();
	TASK_ITER end  = m_TaskWaiting.end();
	for (; iter != end; iter++)
	{
		CSyncTask *pTask = *iter;
		if (!pTask->IsFailedAll())
		{
			return false;
		}
	}
	return true;
}

void CSyncPlayBackMgr::SetLastError(CVideoPane *pPanel, long code)
{
    CAutoLock __lock(m_Mutex);

	BP_RUN_LOG_INF("DoSchedule", "SetLastError %08x %ld", pPanel, code);

	TASK_ITER iter = m_TaskRunning.begin();
	TASK_ITER end  = m_TaskRunning.end();
	for (; iter != end; iter++)
	{
		CSyncTask *pTask = *iter;
		if (pTask->m_pPanel == pPanel)
		{
			pTask->SetLastError(code);
            return;
    	}
	}
}


/***************************************************************************/

CSyncTask::CSyncTask(CVideoPane *pPanel, const char *pCamID, 
	          unsigned int uProto, float fSpeed)
{
	ASSERT(NULL != pPanel);
	ASSERT(NULL != pCamID && pCamID[0]);

    m_pPanel = pPanel;
	m_uProto = uProto;
	m_fSpeed = fSpeed;

	strncpy(m_CamID, pCamID, sizeof(m_CamID) - 1);
    m_bPlaying    = false; 
    m_Current     = m_RecList.begin();
	m_RecIndex    = 0;
	
	
	m_CurrentTick = 0;

	m_uFailures  = 0;
	m_uTotalRecs = 0;
	m_bPause     = false;
	m_bStepPlay  = false;

	m_tmLastTry  = 0;
    m_bFailure   = false;
	m_LastError  = 0;
	m_uPlayMode = PLAY_NORMAL;
}


CSyncTask::~CSyncTask()
{
	if (m_bPlaying)
	{
		StopPlay();//lint !e1551
		m_bPlaying = false;
	}
}//lint !e1740


/*
 * InsertRecord
 * ����¼��ʱ���
 */
void CSyncTask::InsertRecord(REC_TIME_SPAN &Span)
{
    //sort insert
	REC_TIME_ITER iter = m_RecList.begin();
	REC_TIME_ITER end  = m_RecList.end();
	for (; iter != end; iter++)
	{
		REC_TIME_SPAN &Ts = *iter;
		if (Ts.Start > Span.Start)
		{
			m_RecList.insert(iter, Span);
			m_Current = m_RecList.begin();
			m_uTotalRecs++;
			return;
		}
	}
	m_RecList.push_back(Span);
    m_Current = m_RecList.begin();

	m_uTotalRecs++;
}//lint !e1764

/*
 * GetCurrentTick
 * ��ȡ��ǰ����ʱ��
 */
unsigned long long CSyncTask::GetCurrentTick()
{
	ASSERT(m_bPlaying);

	unsigned long long tick = 0;
	unsigned long handle = m_pPanel->GetPlayHandle();

	CIVS_OCXPlayerCtrl *pCtrl = m_pPanel->GetOCXPlayerCtrl();
	ASSERT(NULL != pCtrl);//lint !e774
	if (NULL != pCtrl)//lint !e774
	{
		int sid = pCtrl->GetSDKSessionID();
		if (IVS_SUCCEED == IVS_SDK_GetCurrentFrameTick(sid, handle, &tick))
		{
			BP_RUN_LOG_INF("DoSchedule", "GetCurrentFrameTick %08x %llu", m_pPanel, tick);

			if (0 == tick)
			{
			    if (m_fSpeed > 0)
				{
					REC_TIME_SPAN &ts = *m_Current;
					tick = (unsigned long long)ts.Start * 1000;
				}
				else
				{
					REC_TIME_SPAN &ts = *m_Current;
					tick = (unsigned long long)ts.End * 1000;
				}
    		}
		}
		else
		{
#if 0
			REC_TIME_SPAN &ts = *m_Current;
			if (m_fSpeed > 0)
			{
                tick = (unsigned long long)ts.End * 1000;
			}
			else
			{
				tick = (unsigned long long)ts.Start * 1000;
			}
#endif 
			tick = m_CurrentTick; 
		}
	}
	m_CurrentTick = tick;
	return m_CurrentTick;
}


bool CSyncTask::IsTimeValid(time_t tm)
{
	if (m_bFailure && (time(NULL) - m_tmLastTry < TIME_RETRY))
	{
		BP_RUN_LOG_INF("Is TimeValid", "Invalid on Last Failure");
		return false;
	}
	
    if (m_fSpeed > 0 && (m_RecIndex > (int)(m_uTotalRecs - 1)))
	{
		BP_RUN_LOG_INF("Is TimeValid", "Reach End of List");
		return false;
	}

	if (m_fSpeed < 0 && (m_RecIndex < 0))
	{
		BP_RUN_LOG_INF("Is TimeValid", "Reach head of List");
		return false;
	}
	
	REC_TIME_ITER iter = m_RecList.begin();
	REC_TIME_ITER end  = m_RecList.end();
	for (; iter != end; iter++)
	{
		REC_TIME_SPAN rts = *iter;
		if (m_fSpeed > 0)
		{
			if (rts.Start <= tm && rts.End > tm)
			{
				return true;
			}
		}
		else
		{
			if (rts.Start < tm && rts.End >= tm)
			{
				return true;
			}
		}
	}
	return false;
}


/*
 * GetFirstRecSpanAfter
 * ��ȡ���ڵ���tm�ĵ�һ����Чʱ���
 */
int CSyncTask::GetFirstRecSpanAfter(time_t tm, REC_TIME_SPAN &RecSpan)
{
	if (m_bFailure && (time(NULL) - m_tmLastTry < TIME_RETRY))
	{
		BP_RUN_LOG_INF("SyncTask", "Get FirstRecSpan After in Retry Time");
		return IVS_FAIL;
	}

	if (m_RecIndex > (int)(m_uTotalRecs - 1))
	{
		BP_RUN_LOG_INF("SyncTask", "Get FirstRecSpan After Reach End of List");
		return IVS_FAIL;
	}

	int idx = 0;
	REC_TIME_ITER iter = m_RecList.begin();
	REC_TIME_ITER end  = m_RecList.end();
	for (; iter != end; iter++, idx++)
	{
		REC_TIME_SPAN rts = *iter;
		if (rts.Start >= tm)
		{
			RecSpan = rts;
            m_Current  = iter;
			m_RecIndex = idx;
			
			return IVS_SUCCEED;
		}
	}
	
	m_Current  = --m_RecList.end();
	m_RecIndex = (int)m_uTotalRecs;

	return IVS_FAIL;
}


/*
 * GetFirstRecSpanBefore
 * ����ʱ��ȡǰһ��¼��ʱ���
 */
int CSyncTask::GetFirstRecSpanBefore(time_t tm, REC_TIME_SPAN& RecSpan)
{
	if (m_bFailure && (time(NULL) - m_tmLastTry < TIME_RETRY))
	{
		BP_RUN_LOG_INF("SyncTask", "Get FirstRecSpan Before  Retry Time");
		return IVS_FAIL;
	}

	if (m_RecIndex < 0)
	{
		BP_RUN_LOG_INF("SyncTask", "Get FirstRecSpan Before ReachHead of List");
		return IVS_FAIL;
	}

	int idx = (int)(m_uTotalRecs - 1);
	REC_TIME_RITER iter = m_RecList.rbegin();
	REC_TIME_RITER end  = m_RecList.rend();
	for (; iter != end; iter++, idx--)
	{
		REC_TIME_SPAN rts = *iter;
		if (rts.End <= tm)
		{
			m_Current  = --iter.base();
			m_RecIndex = idx;
			RecSpan = rts;
			return IVS_SUCCEED;
		}
	}

	m_Current  = m_RecList.begin();
	m_RecIndex = -1;

	return IVS_FAIL;
}

/*
 * UpdateRecSegment
 * ����¼��ֹͣ�����¼���ָ��
 */
void CSyncTask::UpdateRecSegment()
{
    if (m_fSpeed > 0)
	{
	    ASSERT(m_Current != m_RecList.end());
    	if (m_RecIndex == (int)(m_uTotalRecs - 1))
		{
			BP_RUN_LOG_INF("SyncTask", "Update Segment FR end Reached");
		}
		else
		{
			m_Current++;
		}
		m_RecIndex++;
	}
	else
	{
		if (0 == m_RecIndex)
		{
			BP_RUN_LOG_INF("SyncTask", "Update Segment BK end Reached");
		}
		else
		{
			m_Current--;
		}
		m_RecIndex--;
	}
}

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


/*
 * StartPlayExpired
 * ������ʱ������
 */
int CSyncTask::StartPlayExpired(time_t now, bool bRunning)
{
	/*
	 * û�����ڲ��ŵ�¼��
	 * ������ͷͨ��ʧ�ܴ�������MAX_RETRY����������
	 */
	if (!bRunning && m_uFailures >= MAX_RETRY)
	{
		return IVS_FAIL;
	}
	
	/*
	 * ����ͨ������ʧ�ܣ���ǰʱ��δ��������ʱ��
	 * ����ʧ�ܣ�����ϵͳ����
	 */
	if (m_bFailure && (time(NULL) - m_tmLastTry < TIME_RETRY))
	{
		BP_RUN_LOG_INF("SyncTask", "Start After in Retry Time");
		return IVS_FAIL;
	}

	if (m_RecIndex < 0 || m_RecIndex > (int)(m_uTotalRecs - 1))
	{
		BP_RUN_LOG_INF("SyncTask", "All Over");
		return IVS_FAIL;
	}

	/* ���ҿ���������¼��� */


	bool bWithIn = false;


	if (m_fSpeed > 0)
	{
		REC_TIME_ITER iter = m_Current;
		REC_TIME_ITER end  = m_RecList.end();
		int idx = m_RecIndex;
     	for (; iter != end; iter++, idx++)
		{
			REC_TIME_SPAN rts = *iter;
			if (rts.Start > now)
			{
				m_Current  = iter;
				m_RecIndex = idx;
				return IVS_FAIL;
			}
					
			if (now < rts.End && now >= rts.Start)
			{
				m_Current  = iter;
				m_RecIndex = idx;
				bWithIn = true;
				break;
			}
		}

		if (!bWithIn)
		{
			m_Current  = --m_RecList.end();
			m_RecIndex = (int)m_uTotalRecs;
			return IVS_FAIL;
		}
	}
	else
	{
		REC_TIME_ITER tmp = m_Current;
		tmp++;
		
		REC_TIME_RITER __iter (tmp);
		REC_TIME_RITER __end  = m_RecList.rend();
		int idx = m_RecIndex;
		for (; __iter != __end; __iter++, idx--)
		{
			REC_TIME_SPAN rts = *__iter;
			if (rts.End < now)
			{
				m_Current  = --__iter.base();
				m_RecIndex = idx;
				return IVS_FAIL;
    		}
				
			if (now <= rts.End && now > rts.Start)
			{
	            m_Current  = --__iter.base();
				m_RecIndex = idx;
				bWithIn = true;
				break;
			}
    	}

		if (!bWithIn)
		{
			m_Current  = m_RecList.begin();
			m_RecIndex = -1;
			return IVS_FAIL;
		}
	}

	if (m_RecIndex < 0 || m_RecIndex > (int)(m_uTotalRecs - 1))
	{
		BP_RUN_LOG_INF("DoSchedule", "Not in All timeSpan %llu", now);
		return IVS_FAIL;
	}

	REC_TIME_SPAN span = *m_Current;
	m_tmLastTry = time(NULL);

	CIVS_OCXPlayerCtrl *pCtrl = m_pPanel->GetOCXPlayerCtrl();
	ASSERT(NULL != pCtrl);//lint !e774
	if (NULL != pCtrl)//lint !e774
	{
		BP_RUN_LOG_INF("DoSchedule", "StartPlatformPlay  %08x %llu %llu", m_pPanel, span.Start, span.End);

		IVS_PLAYBACK_PARAM Param;
		memset(&Param, 0, sizeof(Param));
		Param.fSpeed = m_fSpeed;
		Param.uiProtocolType = m_uProto;
		FORMAT_TIME(span.Start, Param.stTimeSpan.cStart);
		FORMAT_TIME(span.End, Param.stTimeSpan.cEnd);

		unsigned long handle = 0;
		int sid = pCtrl->GetSDKSessionID();
		HWND hWnd = m_pPanel->GetVideoWnd();

		int result = IVS_SDK_StartPlatformPlayBack(sid, m_CamID, &Param, hWnd, &handle);
		if (IVS_SUCCEED != result)
		{
			m_uFailures++;
			m_bFailure  = true;
            
    		BP_RUN_LOG_INF("Start Play Expired", "Start Platform Play Back error=[%d] Del TimeSpan[%s-%s]", 
				result, Param.stTimeSpan.cStart, Param.stTimeSpan.cEnd);

            // modi by wKF71044 at [2013-7-3 13:53:40] �ϱ��¼�
            pCtrl->PostMessage(WM_EVENT_REMOTE_PLAYBACK_CALLBACK,
                (WPARAM)m_pPanel->GetPaneID(), (LPARAM)result);

			return IVS_FAIL;
		}

		int scale = (int)pCtrl->GetVideoPaneMgr()->GetDispalyScale();
		(void)IVS_SDK_SetDisplayScale(sid, handle, scale);

		if (now > span.Start)
		{
			IVS_SDK_SetPlayBackTime(sid, handle, (unsigned int)(now - span.Start));
		}

	   	m_pPanel->SetPlayHandle(handle);
		m_pPanel->SetPlayState(VIDEO_PLAY_STATE_VOD);
		m_pPanel->SetServiceType(SERVICE_TYPE_PLAYBACK);
		
		// TODO ��Ҫ�����첽����ʵ�֣�ˢ�½���
		PostSyncPlayMsg(m_pPanel->m_hWnd, true);

		CGlobalPlayMgr &Mgr = CGlobalPlayMgr::Instance();
		(void)Mgr.RegPlayBackHandle(handle, m_pPanel, m_CamID);

	//	m_Current  = iter;
		m_bPlaying = true;

		if (m_bPause)
		{
			unsigned __int64 NowMs = (unsigned __int64)now * 1000;
			int cnt = 20;
			while (--cnt > 0)
			{
				unsigned __int64 tick = 0;
				(void)IVS_SDK_GetCurrentFrameTick(sid, handle, &tick);
				if (tick != NowMs)
				{
					break;
				}
				Sleep(50);
			}
			IVS_SDK_PlayBackPause(sid, handle);
		}

		if (PLAY_STEPBK == m_uPlayMode)
		{
			(void)StepBackWard();
		}
		else if (PLAY_STEPFR == m_uPlayMode)
		{
			(void)StepForWard();
		}
	}
	m_bFailure  = false; 
	m_uFailures = 0;

	return IVS_SUCCEED;
}

/*
 * StopPlay
 * �رյ�ǰ����
 */
void CSyncTask::StopPlay()
{
	ASSERT(m_bPlaying);//lint !e774 !e831
	if (!m_bPlaying)//lint !e774
	{
		return;
	}

	unsigned long handle = m_pPanel->GetPlayHandle();
	if (0 == handle)
	{
		return;
	}

	CIVS_OCXPlayerCtrl *pCtrl = m_pPanel->GetOCXPlayerCtrl();
	ASSERT(NULL != pCtrl);//lint !e774
	if (NULL != pCtrl)//lint !e774
	{
		int sid = pCtrl->GetSDKSessionID();
		(void)IVS_SDK_StopPlatformPlayBack(sid, handle);
		m_pPanel->SetPlayHandle(0);
		m_pPanel->SetPlayState(VIDEO_PLAY_STATE_NOT_PLAYING);
		PostSyncPlayMsg(m_pPanel->m_hWnd, false);

		CGlobalPlayMgr &Mgr = CGlobalPlayMgr::Instance();
		(void)Mgr.UnRegPlayHandle(handle, m_pPanel);
	}
	m_bPlaying = false;
}

int CSyncTask::PausePlay()
{
	if (!m_bPlaying)
	{
		m_bPause = true;
		return IVS_SUCCEED;
	}

	if (m_bPause)
	{
		return IVS_SUCCEED;
	}

	unsigned long handle = m_pPanel->GetPlayHandle();
	if (0 == handle)
	{
		return IVS_FAIL;
	}

	CIVS_OCXPlayerCtrl *pCtrl = m_pPanel->GetOCXPlayerCtrl();
	ASSERT(NULL != pCtrl);//lint !e774
	if (NULL != pCtrl)//lint !e774
	{
		int sid = pCtrl->GetSDKSessionID();
		int iRet = IVS_SDK_PlayBackPause(sid, handle);
		if (iRet != IVS_SUCCEED)
		{
			return iRet;
		}
	}
	
	m_bPause = true;
	return IVS_SUCCEED;
}

/*
 * ResumePlay
 *
 */
int CSyncTask::ResumePlay()
{
	if (!m_bPlaying)
	{
		m_bPause = false;
		return IVS_SUCCEED;
	}

	if (!m_bPause && !m_bStepPlay)
	{
		return IVS_SUCCEED;
	}

	m_bStepPlay = false;

	unsigned long handle = m_pPanel->GetPlayHandle();
	if (0 == handle)
	{
		return IVS_FAIL;
	}

	CIVS_OCXPlayerCtrl *pCtrl = m_pPanel->GetOCXPlayerCtrl();
	ASSERT(NULL != pCtrl);//lint !e774
	if (NULL != pCtrl)//lint !e774
	{
		int sid = pCtrl->GetSDKSessionID();
		IVS_SDK_PlayBackResume(sid, handle);
	}

	m_bPause = false;
	return IVS_SUCCEED;
}


/*
 * SetPlayTime
 * ����ͨ������ʱ��
 */
int CSyncTask::SetPlayTimePlaying(time_t tm)
{
    /*
	 * ��鵱ǰ����¼�������Ƿ���� tm
	 * ������ֱ������¼�񲥷�ʱ��
	 * ����رյ�ǰ¼�񡢽��ɵ����̹߳���
	 */
	ASSERT(m_bPlaying);//lint !e774 !e831
	if (!m_bPlaying)//lint !e774
	{
		return IVS_FAIL;
	}

    ASSERT(m_Current != m_RecList.end());
    REC_TIME_SPAN &s = *m_Current;
	if (s.Start <= tm && s.End > tm)
	{
		unsigned long handle = m_pPanel->GetPlayHandle();
		CIVS_OCXPlayerCtrl *pCtrl = m_pPanel->GetOCXPlayerCtrl();
		ASSERT(NULL != pCtrl);//lint !e774
		if (NULL != pCtrl)//lint !e774
		{
			int sid = pCtrl->GetSDKSessionID();
			unsigned int offset = (unsigned int)(tm - s.Start);
			IVS_SDK_SetPlayBackTime(sid, handle, offset);
			return IVS_SUCCEED;
		}
	}
	
	StopPlay();
	return IVS_FAIL;
}


int CSyncTask::SetPlayTimeWaiting(time_t tm)
{
    /*
	 * ��鵱ǰ����¼�������Ƿ���� tm
	 * ������ֱ������¼�񲥷�ʱ��
	 * ����رյ�ǰ¼�񡢽��ɵ����̹߳���
	 */
	ASSERT(!m_bPlaying);//lint !e774
	if (m_bPlaying)//lint !e774
	{
		return IVS_FAIL;
	}

	/* ���ò���ʱ�䣬���������͵����� */
	REC_TIME_SPAN rts = m_RecList.front();
	if (tm < rts.Start)
	{
		m_Current  = m_RecList.begin();
		m_RecIndex = m_fSpeed > 0 ? 0 : -1;
		return IVS_SUCCEED;
    }

	rts = m_RecList.back();
	if (tm > rts.End)
	{
		m_Current  = --m_RecList.end();
		m_RecIndex = (int)(m_fSpeed > 0 ? m_uTotalRecs : m_uTotalRecs - 1);
		return IVS_SUCCEED;
	}

	int idx = 0;
	REC_TIME_ITER iter = m_RecList.begin();
	REC_TIME_ITER end  = m_RecList.end();

	for (; iter != end; iter++, idx++)
	{
		REC_TIME_SPAN __rts = *iter;
        if (__rts.Start <= tm && __rts.End >= tm)
	    {
            m_Current  = iter;
		    m_RecIndex = idx;
		    return IVS_SUCCEED;
	    }

	    if (tm < __rts.Start)
	    {
		    if (m_fSpeed > 0)
		    {
			    m_Current  = iter;
			    m_RecIndex = idx;
			    return IVS_SUCCEED;
		    }
		    else
		    {
			    m_Current  = --iter;
			    m_RecIndex = idx - 1;
			    return IVS_SUCCEED;
		    }
	    }
	}
    
	return IVS_SUCCEED;
}



/*
 * SetPlaySpeed
 * ���õ�ǰͨ���Ĳ����ٶ�
 */
int CSyncTask::SetPlaySpeed(float fSpeed)
{
	m_uPlayMode = PLAY_NORMAL;
	m_bStepPlay = false;

	if (!m_bPlaying)//lint !e774
	{
		BP_RUN_LOG_INF("SyncTask Set PlaySpeed", 
			"Set Speed While Waiting %s", fSpeed);
        m_fSpeed = fSpeed;
		return IVS_SUCCEED;
	}

	unsigned long handle = m_pPanel->GetPlayHandle();
    CIVS_OCXPlayerCtrl *pCtrl = m_pPanel->GetOCXPlayerCtrl();
    ASSERT(NULL != pCtrl);//lint !e774 !e831
	if (NULL != pCtrl)//lint !e774 !e831
	{
		int sid = pCtrl->GetSDKSessionID();
	    m_fSpeed = fSpeed;
		return IVS_SDK_SetPlayBackSpeed(sid, handle, fSpeed);
	}
	return IVS_FAIL;
}

/*
 * StepForWard
 * ����ǰ��
 */
int CSyncTask::StepForWard()
{
	ASSERT(m_bPlaying);//lint !e774 !e831
	if (!m_bPlaying)//lint !e774
	{
		BP_RUN_LOG_INF("SyncTask Set StepForWard", "Not Playing");
		return IVS_FAIL;
	}

	m_bStepPlay = true;
	SetPlayMode(PLAY_STEPFR);

	unsigned long handle = m_pPanel->GetPlayHandle();
	CIVS_OCXPlayerCtrl *pCtrl = m_pPanel->GetOCXPlayerCtrl();
	ASSERT(NULL != pCtrl);//lint !e774 !e831
	if (NULL != pCtrl)//lint !e774 !e831
	{
		int sid = pCtrl->GetSDKSessionID();
		return IVS_SDK_PlayBackFrameStepForward(sid, handle);
	}
	return IVS_FAIL;
}

/*
 * StepBackWard
 * ��������
 */
int CSyncTask::StepBackWard()
{
	ASSERT(m_bPlaying);//lint !e774 !e831
	if (!m_bPlaying)//lint !e774
	{
		BP_RUN_LOG_INF("SyncTask Set StepBackWard", "Not Playing");
		return IVS_FAIL;
	}

	m_bStepPlay = true;
	SetPlayMode(PLAY_STEPBK);

	unsigned long handle = m_pPanel->GetPlayHandle();
	CIVS_OCXPlayerCtrl *pCtrl = m_pPanel->GetOCXPlayerCtrl();
	ASSERT(NULL != pCtrl);//lint !e774 !e831
	if (NULL != pCtrl)//lint !e774 !e831
	{
		int sid = pCtrl->GetSDKSessionID();
		return IVS_SDK_PlayBackFrameStepBackward(sid, handle);
	}
	return IVS_FAIL;
}

/*
 * PostSyncPlayMsg
 * ����������ֹͣ��Ϣ������
 */
void CSyncTask::PostSyncPlayMsg(HWND hWnd, bool bStarted)const
{
	PostMessage(hWnd, WM_SYNC_PLAYBACK_MSG, (WPARAM)bStarted, (LPARAM)0);
}

int CSyncTask::GetPanelID()const
{
	if (m_pPanel == NULL)
	{
		return -1;
	}
	return (int)(m_pPanel->GetPaneID());
}

void CSyncTask::SetPlayMode( unsigned int mode )
{
	m_uPlayMode = mode; 

	if (PLAY_STEPBK == mode)
	{
		m_fSpeed = -1.0;
	}
	else if (PLAY_STEPFR == mode)
	{
		m_fSpeed = 1.0;
	}
}



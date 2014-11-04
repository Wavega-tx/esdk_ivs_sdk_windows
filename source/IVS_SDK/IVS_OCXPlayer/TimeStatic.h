#ifndef _TIME_STATIC_H__
#define _TIME_STATIC_H__

// CTimeStatic
//��߿ؼ�����
//#include "CTimeStaticMacro.h"
#include "math.h"

CONST INT32 CURRENT_HOUR   =    0  ;               //Ĭ�ϵ�ǰСʱ
CONST INT32 CURRENT_MIN    =    0  ;               //Ĭ�ϵ�ǰ����
CONST INT32 START_HOUR     =    0   ;               //Ĭ����ʼСʱ
CONST INT32 START_MIN      =    0   ;               //Ĭ����ʼ����
CONST INT32 END_HOUR       =    24  ;               //Ĭ�Ͻ���Сʱ
CONST INT32 END_MIN        =    0   ;               //Ĭ�Ͻ�������
CONST INT32 TEXT_SPACE     =    2   ;               //Ĭ��Сʱ�ı����
CONST INT32 TEXT_MIN       =    10  ;               //Ĭ�Ϸ����߼��

CONST INT32 LENGTH_HOUR    =    4;                  //Ĭ��Сʱ�߳���ϵ��
CONST INT32 LENGTH_MIN     =    9;                  //Ĭ�Ϸ����߳���ϵ��
CONST INT32 LENGTH_MIDMIN  =    6;                  //Ĭ�Ϸ����м��߳���ϵ��

CONST INT32 MINUTE_COUNT   =    60;                 //ÿСʱ60����
CONST INT32 HOUR_COUNT     =    24;                 //ÿ��24Сʱ
CONST INT32 SCOND_COUNT    =    3600;                //ÿСʱ3600��
CONST INT32 MINUTE_GAP     =    5;                  //�����ߵĿ�϶
CONST INT32 SCOND_GAP      =    15;                 //���ߵĿ�϶
CONST INT32 TEXT_GAP       =    10;                 //����֮��Ŀ�϶
CONST INT32 NONIUS_WIDTH   =    10;                 //�α��
CONST INT32 NONIUS_HRIGHT  =    10;                 //�α��
CONST INT32 NONIUS_LEFT_OFFSET     =    6;          //��ƫ��
CONST INT32 NONIUS_BOTTOM_OFFSET   =    14;         //��ƫ��
CONST INT32 MAX_SCOND_NUM          =    12+1;       //С�̶�ʱʮ��������Ŀ
CONST INT32 WHEEL_SCAL             =    120;        //������ÿ�ι����ƶ�ֵ

typedef struct
{
	int iHour;
	int iMinute;
	int iScond;
}ST_TIME;

typedef struct
{
	ST_TIME stStartTime;
	ST_TIME stEndTime;
}ST_LAYERTIME;

typedef struct
{
	double dLeft;
	double dRight;
	int iType; //0 ��ʾ�ǻ�ɫ�� 1��ʾ����ɫ��
	ST_LAYERTIME stTime;
}ST_AREAINFO;

typedef enum
{
	TYPE_YELLO = 0, // ��ɫ��
	TYPE_GREEN  // ��ɫ��
}EM_LAYERTIME;

//������Ϣ�Ľṹ��
typedef struct
{
	ST_TIME stStartTime;   //��ǰ����Ŀ�ʼʱ��
	ST_TIME stEndTime;     //��ǰ����Ľ���ʱ��
	ST_TIME stCurrentTime; //��ǰ���е�ʱ��
	int iType;             //��ǰ���������
}ST_TIMER_MSG;

#define  RECORD_PLAY_TIME_ID 5
//�̶�����
typedef enum
{
	TYPE_SCALE_SMALL = 0, //С�̶�
	TYPE_SCALE_BIG        //��̶�
}EM_SCALE_TYPE;

class CPlayTimerBar;
class CTimeStatic : public CTranslucentStatic
{
	DECLARE_DYNAMIC(CTimeStatic)

public:
	CTimeStatic();
	virtual ~CTimeStatic();
	virtual void Render(Gdiplus::Graphics& g);
	void Clear();
	void ClearYelloList();
	void ClearGreenList();
	void ClearAreaList();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	ST_TIME PointToTime(const CPoint &point) const;
	void MiniteToTime(ST_TIME &oldtime,const int iMinute) const;
	double TimeToPointX(const ST_TIME &stStarttime);
	bool TimeToRight(ST_TIME &oldtime,const ST_TIME &righttime)const;
	bool TimeToleft(ST_TIME &oldtime,const ST_TIME &lefttime)const;
	void TimeTansZero(ST_TIME &sttime)const;
	void FindRecord();

	void ScondToTime(ST_TIME &oldtime,const int iScond)const;
public:
	bool SetBarTime(const ST_TIME &stStarttime,const ST_TIME &stEndtime);//lint !e830
	bool SetCurrentTime(const ST_TIME &time);//lint !e830
	void TransTime(ST_TIME &newtime,const ST_TIME &oldtime)const;//����ʱ��
	ST_TIME GetCurrentTime() const;                      //��ȡ��ǰʱ��
	void AddLayeTimeRange(int iType,const ST_LAYERTIME &stLayerTime);   //����ʱ���
	void SetPlayTimerBar(CPlayTimerBar* pPlayTimerBar);

	void InitTimeRange();

	void MovePaneBig(const CPoint& point);
	void MovePaneSmall(const CPoint& point);
	BOOL IsCanPlayBack(ST_TIMER_MSG &stTimerMsg);
	void StartRecordPlayTimer();
	void StopRecordPlayTime();
	//���Ž���
	BOOL IsPlayEnd();
    BOOL IsCurrentPlayEnd();
    void ResetTimer()const;
	void SetCurrentPaneToActivePane();
public:
	CWnd* GetVideoPane() const { return m_pVideoPane; } //lint !e1763  MFC�Զ����ɵĵĺ���
	void SetVideoPane(CWnd * pVideoPane){m_pVideoPane = pVideoPane;}
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
public:
	void Initialize();                      //��ʼ��;
	void Initialize2();                     //С�̶ȳ�ʼ��
protected:
	CWnd* m_pVideoPane;
private:
	bool   m_bTrack;
	ST_TIME m_stCurrentTime;          // ��ǰʱ��;
	ST_TIME m_stStartTime;            // ��ʼʱ��;
	ST_TIME m_stEndTime;              // ��ֹʱ��;
	ST_TIME m_stSaveCurrentTime;
    ST_TIME m_stStartPlayTime;        // ��ʼ���ŵ�ʱ��;
	CRect m_cRect;                    // �ؼ�����;

	PINT m_pHourLineX;               // Сʱ�������ָ��;
	PINT m_pMinLineX;                // ���ӵ������ָ��;
	PINT m_pScondLinX;               // ʮ�����ߺ�����ָ��;
	int m_iHourNum;                   // Сʱ����Ŀ;
	int m_iMinNum;                    // ��������Ŀ;
	int m_iHourLength;                // Сʱ�߳�;
	int m_iMinLength;                 // �����߳�;
	int m_iMidMinLength;              // �����м��߳�;
	double m_dHourSpace;              // Сʱ���;
	double m_dMinSpace;               // ���Ӽ��;
	double m_dSconSpace;              // ʮ������֮��ļ��

	bool m_bIsMouseFlag;              // ����־;
	bool m_bIsInit;                   // �Ƿ��ʼ��;
	double m_iCurrentPointPos;           // ���浱ǰ�α�����
	double m_iLstPointPos;            // �����ϴε��α�λ��
	int m_iLastXPos;                  // �����ϴ��ƶ���X����
	CPlayTimerBar* m_pPlayTimerBar;   // ���游���ڵ�ָ��
	int m_iSizeType;                  // �̶�����
	bool m_bIsTimer;                  // ��ʱ������

	double m_dScondGap;               // ÿ���ڵ�ǰ�����ϵĳ���

    //ST_AREAINFO m_AreaInfo;           // ��ǰ��������
    ST_LAYERTIME m_Layer;             // ��ǰ��������

	CList<ST_LAYERTIME*,ST_LAYERTIME*> m_ListYello;
	CList<ST_LAYERTIME*,ST_LAYERTIME*> m_ListGreen;
	CList<ST_AREAINFO*,ST_AREAINFO*> m_ListArea;
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	HANDLE m_hThread;
    static DWORD WINAPI TimerThread(LPVOID pParam);
	void AddOneScond();
    void SetStartPlayTime(const ST_TIME &stPlaytime);
    bool IsInCurrentArea(const CPoint &point);
    bool IsInCurrentArea(const ST_TIME &time);
    bool FindNextPlayRange(const ST_TIME &CurTime, ST_TIME &NextTime);
    void GetTimeMsg(const ST_TIME& stTime, ST_TIMER_MSG &stTimerMsg);
	void SetCurStreamStop() { m_bCurStreamStop = true; };

private:

	bool m_bCurStreamStop;





};

#endif


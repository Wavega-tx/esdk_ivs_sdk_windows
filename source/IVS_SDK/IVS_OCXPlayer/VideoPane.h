/*****************************************************************
 filename    :    VideoPane.h
 author      :    ykf75928
 created     :    2013/01/15
 description :    ������
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013/01/15 ��ʼ�汾
*****************************************************************/
#pragma once

#include "Resource.h"
#include "VideoStatic.h"
#include "TranVideoRealtimeBar.h"
#include "TranAnalysisToolBar.h"
#include "TranVideoTitleBar.h"
#include "TranTVWallStatusBar.h"
#include "IVS_IADrawAPI.h"
#include "IVS_IATrackAPI.h"
#include "TurnCruisePane.h"
#include "DitalPtzDlg.h"
#include "TranZoomDirctDlg.h"
#include "TranZoomBar.h"
#include "TranZoomTitleBar.h"
#include "PlayTimerBar.h"

const short  PANE_STATE_NOT_ACTIVATE         =  0  ;
const short  PANE_STATE_ACTIVATE             =  1  ;

const short  PANE_STATE_WARNING              =  10 ;
const short  WARN_TIMER_ID                   =  1  ;
const short  MAGNIFY_TIMER_ID                =  2  ;

const short  VIDEO_PLAY_STATE_NOT_PLAYING    =  0  ;
const short  VIDEO_PLAY_STATE_REALTIME       =  1  ;
const short  VIDEO_PLAY_STATE_VOD            =  2  ;
const short  VIDEO_PLAY_STATE_LOCALFILE      =  3  ;
const short  VIDEO_PLAY_STATE_RESTARTING     =  4  ;
const short  VIDEO_PLAY_STATE_STARTING       =  5  ;
const short  VIDEO_PLAY_STATE_EMAP           =  6  ;

const short  CIF_WIDTH                       =  352;
const short  CIF_HEIGHT                      =  288;

typedef enum 
{
	TVWALL_STATUS_NONE = 0,				//δ��ǽ
	TVWALL_STATUS_START = 1,			//��ʼ��ǽ
	TVWALL_STATUS_PLAY = 2,				//���ڲ���
	TVWALL_STATUS_VIDEO_BROKEN = 3,		//��Ƶ����
	TVWALL_STATUS_DEC_DISCONN = 4		//����������
}TVWALL_STATUS;

typedef enum
{
	TVWALL_BAR_STAUS_HIDE = 0,//����
	TVWALL_BAR_STAUS_OFFLINE = 1,//����
	TVWALL_BAR_STAUS_ONLINE = 2//����
}TVWALLBAR_STATUS;

typedef struct
{
	HWND window;
	int x;
	int y;
	int cx;
	int cy;
}WindowMoveInfo;


// CVideoPane dialog

#ifndef _CVIDEOPANEMGR_DECLARATION_
#define _CVIDEOPANEMGR_DECLARATION_
class CVideoPaneMgr;
#endif

#ifndef _CIVS_OCXPLAYERCTRL_DECLARATION_
#define _CIVS_OCXPLAYERCTRL_DECLARATION_
class CIVS_OCXPlayerCtrl;
#endif

class CVideoPane : public CDialog
{
	DECLARE_DYNAMIC(CVideoPane)

public:
	CVideoPane(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVideoPane();

    CVideoPaneMgr * m_pVideoPaneMgr;
    COleControl * m_pOcxCtrl;
    std::vector<WindowMoveInfo> m_LISTMOVE;
// Dialog Data
	enum { IDD = IDD_DLG_VIDEOPANE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    void BeginPaneDrag(void)const;
    //  
    void ChangePaneSize(void)const;
    void ResetPaneSize();

    // �����������͵ײ�������
    void CreateBar(void);
    void CreateAnalysisBar();
	void CreateBarAnalysisBar();
    void CreateRealTimeBar();
	void CreateTVWallStatusBar()const;
	void CreatemTurnCruisePane();
	void DeleteTurnCruisePane();
	void CreateDigtalPtz();
	void DeleteDigtalPtz();
	void CreateZoomDirctDlg();
	void DeleteZoomDirctDlg();
	void CreateZoomBar();
	void DeleteZoomBar();
	void CreateZoomTitleBar();
	void CreateTimerBar();
	void DeleteZoomTitleBar();
	void DeleteTimerBar();

    // ɾ�������ؼ�
    void DeleteSoundCtrl( const CPoint &point ) const; 
	void DeleteTalkbackSound( const CPoint &point ) const;
private:
    // ����ID
    ULONG m_ulPaneID;
public:
    ULONG GetPaneID() const { return m_ulPaneID; }
    void SetPaneID(ULONG val) { m_ulPaneID = val; }
private:
    // �Ƿ��ǻ����
    ULONG m_ulIsActivate;
public:
    void SetNotActivate()   { m_ulIsActivate = PANE_STATE_NOT_ACTIVATE;}
    void SetActivate()      { m_ulIsActivate = PANE_STATE_ACTIVATE; }
private:
    // ����״̬��ʵ�����طš������ļ���δ����
    ULONG m_ulPlayState;
public:
    ULONG GetPlayState() const { return m_ulPlayState; }
    void SetPlayState(ULONG val) { m_ulPlayState = val; }

    void GetPlayBackCurrentTime(ST_TIME& PlayTime);

private:
    // ����ʱ���ڼ���߿��������ٴ�
    ULONG m_ulWarnCounter;
    // ����ʱ�߿�������������
    ULONG m_ulMaxWarnCount;
    // ��¼�û��Ƿ�����·��Ƶ
    bool m_bIsSoundOn;
public:
    bool IsSoundOn() const { return m_bIsSoundOn; }
    void IsSoundOn(bool val) { m_bIsSoundOn = val; }
private:
    // ��¼��ǰ������·��Ƶ����
    ULONG m_ulAudioVolume;
public:
    ULONG GetAudioVolume() const { return m_ulAudioVolume; }
    void SetAudioVolume(ULONG val) { m_ulAudioVolume = val; }

	void IsDrag(ULONG val) { m_ulIsDrag = val; }
private:
    // �������
    CTranVideoTitleBar *m_pRealTimeTitleBar;
    // ʵ������������
    CTranVideoRealtimeBar *m_pRealTimeToolBar;
    // ���ܷ���������
    CTranAnalysisToolBar *m_pAnalysisToolBar;
	// ����ǽͨ��״̬��
	CTranTVWallStatusBar *m_pTVWallStatusBar;
	bool m_bIsShowTVWallBar;

	//��ѵ��ť����
	CTurnCruisePane *m_pTurnCruisePane;
	//������̨
	CDitalPtzDlg *m_pDigtalDlg;
	// �Ŵ���ƶ�
	CTranZoomDirctDlg *m_pZoomDirctDlg;
	// �Ŵ󹤾���
	CTranZoomBar* m_pZoomBar;
	// �Ŵ������
	CTranZoomTitleBar* m_pZoomTitleBar;
	// ��ʱ�طŰ�ť
	CPlayTimerBar* m_pPlayTimerBar;

	Gdiplus::Image* m_pTVWallOnlineImage;
	Gdiplus::Image* m_pTVWallOfflineImage;

	//�����Ƿ����Drag
	ULONG m_ulIsDrag;
	//��˫���رհ�ť���еĹ�ܲ���
	bool m_ulIsStopButtonDrag;
public:
	void SetStopDrag(bool bFlag) {m_ulIsStopButtonDrag = bFlag;}
public:
	ULONG GetWndDragStatus()const{return m_ulIsDrag;}
	bool GetTVWallStatusBarStatus()const {return m_bIsShowTVWallBar;}
public:
    CTranVideoTitleBar* GetVideoTitleBar() const { return m_pRealTimeTitleBar; }			//lint !e1763
    CTranVideoRealtimeBar* GetVideoRealtimeBar() const { return m_pRealTimeToolBar; }	//lint !e1763
    CTranAnalysisToolBar* GetAnalysisToolBar() const { return m_pAnalysisToolBar; }	//lint !e1763
	CTranTVWallStatusBar* GetTVWallStatusBar() const { return m_pTVWallStatusBar; } //lint !e1763
	CTurnCruisePane* GetTurnCruisePane() const { return m_pTurnCruisePane;}//lint !e1763
	CDitalPtzDlg* GetDigtalPtzDlg() const {return m_pDigtalDlg;} //lint !e1763
	CTranZoomDirctDlg* GetZoomDirctDlg() const {return m_pZoomDirctDlg;} //lint !e1763
	CTranZoomBar* GetZoomBar() const {return m_pZoomBar;} //lint !e1763
	CTranZoomTitleBar* GetZoomTitleBar() const {return m_pZoomTitleBar;} //lint !e1763
	CPlayTimerBar* GetPlayTimerBar() const {return m_pPlayTimerBar;} //lint !e1763
public:
	void SetTVWallStatusBarStatus(ULONG ulBarStatus);//���õ���ǽͨ��״̬
	void SetTVWallStatus(ULONG ulPaneStatus);//���õ���ǽ����״̬
private:
    // ��קʱ�������ж��Ƿ��ǵ�һ�ν��봰��
    bool m_isFirstEnter;
    // ��¼������ParentWnd����ռ��λ��
    CRect m_PaneRect;
public:
    void GetPaneRect( CRect * paneRect )const;
    void SetPaneRect(const CRect &paneRect );
private:
    // ��¼������ParentWnd����ռ��λ�ã��������񣬰����������͵ײ���������
    CRect m_PaneRectFull;
public:
    void SetPaneRectFull(const CRect &paneRect);
    void GetPaneRectFull( CRect * paneRect )const;
 private:
    // ��ʾ����
    ULONG m_ulDisplayScale;
public:
    ULONG GetDisplayScale() const { return m_ulDisplayScale; }
    void SetDisplayScale(ULONG val) { m_ulDisplayScale = val; }
private:
    // 3D��̨��λ�ĵ�һ����
    CPoint m_3DPTZStartPoint;
    // 3D��̨�Ƿ���
    ULONG m_ulIs3DPTZOpen;
public:
    ULONG Get3DPTZState() const { return m_ulIs3DPTZOpen; }
    void Set3DPTZState(ULONG val);
	void Set3DPTZStateOnly(ULONG val) { m_ulIs3DPTZOpen = val; }
private:
    // �Ƿ�����¼��
    ULONG m_ulIsRealtimeRecording;
public:
    ULONG GetRealtimeRecordState() const { return m_ulIsRealtimeRecording; }
    void SetRealtimeRecordState(ULONG val) { m_ulIsRealtimeRecording = val; }
private:
    // �Ƿ�����ץ��
    ULONG m_ulIsRealtimeCapturing;
public:
    ULONG GetRealtimeCaptureState() const { return m_ulIsRealtimeCapturing; }
    void SetRealtimeCaptureState(ULONG val) { m_ulIsRealtimeCapturing = val; }
private:
    // �Ƿ�����ע���ǩ
    ULONG m_ulIsRealtimeLabling;
public:
    ULONG GetRealtimeLabelState() const { return m_ulIsRealtimeLabling; }
    void SetRealtimeLabelState(ULONG val) { m_ulIsRealtimeLabling = val; }
private:
    // �Ƿ����������Խ�
    ULONG m_ulIsRealtimeTalkbacking;
public:
    ULONG GetRealtimeTalkbackState() const { return m_ulIsRealtimeTalkbacking; }
    void SetRealtimeTalkbackState(ULONG val) { m_ulIsRealtimeTalkbacking = val; }
private:
    // �Ƿ����ڲ�������
    ULONG m_ulIsRealtimeSounding;
public:
    ULONG GetRealtimeSoundState() const { return m_ulIsRealtimeSounding; }
    void SetRealtimeSoundState(ULONG val) { m_ulIsRealtimeSounding = val; }
	
	//����ʱ��Ϊˢ��͸��Bar
	void SetTranslucentStatus(BOOL val) {m_bIsHideTranslucentBar = val;}
private:
    // �Ƿ����ھֲ��Ŵ�,Դ����
    ULONG m_ulIsRealtimeMagnifying;
	// Ŀ�Ĵ��ڷŴ�״̬
	ULONG m_ulIsDstMagnify;

	// �Ŵ�����
	ULONG m_MagnifyNum;
	// ��ʾ���ֵļ���,��1-3ѭ��ʹ��
	ULONG m_MagnifyNameCount[MAGNIFY_MAX_NUM];
	std::string m_strZoomName;
	// �����ɫ
	IA_COLOR m_IAColor;
	// Դ����
	CVideoPane* m_pMagnifySrcPane;
	// ��ռ��Ϊ�Ŵ󴰿ڵ�֮ǰ��״̬�����ڻָ�
	ULONG m_ulPrvePlayState;
	// ����Դ���ڱ��Ŵ������ٷֱ�
	IVS_RECT m_IArectPercent;
	// ������ǰ��handle
	ULONG m_ulPrvePlayHandle;
	// ������¼��طŸ��ǵ�ʵ���ľ��
	ULONG m_ulInstanstRealPlayHandle;
	// ����¼��طŵľ��
	ULONG m_ulInstanstPlayBackHandle;
	// �Ƿ��Ѿ��ڿ���¼��ط�
	BOOL m_bIsInstanstPlayBack;
	// ��¼����Ĺ�������״̬
	ULONGLONG m_ulToolBarStatus;
	// �Ƿ�����ͣ״̬
	BOOL m_bIsInstansPlayPause;
	// ��ͼ�Ľṹ��
	IA_GRAPHICS m_szGraphics[MAGNIFY_MAX_NUM];
	// ���ڻָ��ĸ���
	ULONG m_ResumeMagnifyNum;
	// �Ƿ������ֲ��Ŵ�������Ҫ�ָ��ĳ���
	bool m_bOpendMagnifyStatus;
	// �Ƿ���3D���ֲ��Ŵ�ռ��
	bool m_bOpend3DPTZStatus;
	// �������ֱ���
	SIZE m_szOpendMagnifySize;
	// ����һ����ק�������ƶ����û�ɹ������ͷ�����˻�ͼ
	ULONG m_DrawMoveSuccessSum;
	// ���滭ͼ��ɺ����꣬����ģ�����¼�
	POINT m_MagnifyLastPoint;
	// ��������
	TYPE_WND m_ulPrveTypeWnd;

	// �ڷŴ�����¹ر�ʵ����Ҫ�ָ������������
	std::string m_strOpendMagnifyCameraID;
	// �ֱ���
	LONG m_DrawHeight;
	LONG m_DrawWidth;
public:
	afx_msg LRESULT OnMagnifyCloseAll(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMagnifyClose(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMagnifyRateChange(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMagnifyRectMove(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnInstantReplayMove(WPARAM wParam,LPARAM pParam);
	afx_msg LRESULT OnDeleteTurnCruise(WPARAM wParam,LPARAM pParam);
    afx_msg LRESULT OnInstantReplayStart(WPARAM wParam, LPARAM pParam);
	afx_msg LRESULT onInstantReplaySegStop(WPARAM wParam, LPARAM pParam);


	IVS_INT32 MoveSrcGraphicRect(int iDirect, ULONG ulGraphicGroup);
	IVS_INT32 ChangeSrcGraphicRect(int iDirect, ULONG ulGraphicGroup);
	void UpdataMagnifyWindow(ULONG ulGraphicGroup);
	void TransIARect2Percent(const CRect& rect, CRect& percentRect);
	void SortIARect2Point(POINT& p1, POINT& p2, POINT& p3, POINT& p4, const CRect& changeRect)const;
	IVS_INT32 GetIARectByGroup(ULONG uiGraphicGroup, CRect& rect);
	void GetIARectByGraphics(const IA_GRAPHICS& graphicsTemp, CRect& rect)const;
    ULONG GetRealtimeMagnifyState() const { return m_ulIsRealtimeMagnifying; }
    // ����ʵ���������ֲ��Ŵ�״̬
    ULONG SetRealtimeMagnifyState(ULONG ulState);
	void SetMagnifySrcPane(CVideoPane* pSrc) { m_pMagnifySrcPane = pSrc;}
	CVideoPane* GetMagnifySrcPane() { return m_pMagnifySrcPane;}
	IVS_INT32 GetMediaSize(SIZE& size);// ��ȡͼ��Ĵ�С
	void MagnifyMouseDown( const CPoint &point );
	void MagnifyMouseUp( const CPoint &point );
	void StartMagnifyByGraphicPoint(const CPoint &point);
	void FinishMagnifyByGraphicPoint(const CPoint &point);
	ULONG GetDstMagnifyState() const { return m_ulIsDstMagnify; }
	void SetDstMagnifyState(ULONG val) { m_ulIsDstMagnify = val; }
	ULONG GetPrvePlayState() const { return m_ulPrvePlayState; }
	void SetPrvePlayState(ULONG val) { m_ulPrvePlayState = val; }
	TYPE_WND GetPrveTypeWnd() const { return m_ulPrveTypeWnd; }
	void SetPrveTypeWnd(TYPE_WND val) { m_ulPrveTypeWnd = val; }
	ULONG GetMagnifyPlayHandle() const { return m_ulPrvePlayHandle; }
	void SetMagnifyPlayHandle(ULONG val) { m_ulPrvePlayHandle = val; }
	bool GetOpendMagnifyStatus() const { return m_bOpendMagnifyStatus; }
	void SetOpendMagnifyStatus(bool val) { m_bOpendMagnifyStatus = val; }
	SIZE GetOpendMagnifySize() const { return m_szOpendMagnifySize; }
	void SetOpendMagnifySize(const SIZE& sz) { m_szOpendMagnifySize = sz; }
	const std::string& GetOpendMagnifyCameraID() const{ return m_strOpendMagnifyCameraID; }
	void SetOpendMagnifyCameraID( const std::string& strCameraID ) { m_strOpendMagnifyCameraID = strCameraID; }
	bool GetOpend3DPTZStatus() const { return m_bOpend3DPTZStatus; }
	void SetOpend3DPTZStatus(bool val) { m_bOpend3DPTZStatus = val; }
	int GetFreeNameNum(ULONG ulGraphicGroup);
	// ������id�ͷŵ�ռ�õ�����
	void FreeNameNum(ULONG ulGraphicGroup);
	void ResumeAllMagnifyRect();
	void ResumeMagnifyRectByGraphic(const IA_GRAPHICS& graphicsTemp);
	// ���洰�ڵ�����
	void SetZoomName(const std::string& strZoomName) { m_strZoomName = strZoomName; }
	std::string& GetZoomName() { return m_strZoomName; }//lint !e1536
	// ˢ��tip
	void UpdateAllTipText();
	IVS_RECT GetIArectPercent() const { return m_IArectPercent; }
	void SetIArectPercent(const CRect& val){
		m_IArectPercent.top = val.top;
		m_IArectPercent.bottom = val.bottom;
		m_IArectPercent.left = val.left;
		m_IArectPercent.right = val.right;
	};
	int GetMagnifyRateByIARect(const CRect& IArect);
	LONG GetDrawHeight() const { return m_DrawHeight; }
	void SetDrawHeight(LONG val) { m_DrawHeight = val; }
	LONG GetDrawWidth() const { return m_DrawWidth; }
	void SetDrawWidth(LONG val) { m_DrawWidth = val; }
	void HideZoomWnd();
private:
    // �Ƿ�������̨����
    ULONG m_ulIsRealtimePtzing;
public:
    ULONG GetRealtimePtzState() const { return m_ulIsRealtimePtzing; }
    void SetRealtimePtzState(ULONG val);
private:
    // �Ƿ����ڸ澯
    ULONG m_ulIsRealtimeAlarming; //lint !e1764
public:
    ULONG GetRealtimeAlarmState() const { return m_ulIsRealtimeAlarming; }
    void SetRealtimeAlarmState(ULONG val) { m_ulIsRealtimeAlarming = val; }
private:
    // �Ƿ�������Ƶ��ǽ
    ULONG m_ulIsRealtimeWalling;
public:
    ULONG GetRealtimeWallState() const { return m_ulIsRealtimeWalling; }
    void SetRealtimeWallState(ULONG val) { m_ulIsRealtimeWalling = val; }
private:
    // �Ƿ��������ܷ����������
    ULONG m_ulIsRealtimeAnalysising;
public:
    ULONG GetRealtimeAnalysisState() const { return m_ulIsRealtimeAnalysising; }
    void SetRealtimeAnalysisState(ULONG val) { m_ulIsRealtimeAnalysising = val; }
private:
    // �Ƿ�������ʾʵ��������
    ULONG m_ulIsRealtimeBarShowing;
public:
    ULONG GetRealtimeBarState() const { return m_ulIsRealtimeBarShowing; }
    void SetRealtimeBarState(ULONG val) { m_ulIsRealtimeBarShowing = val; }
private:
    // �Ƿ�����¼��ط�ҳ���в���
    ULONG m_ulIsRecordPlaying;
public:
    ULONG GetRecordPlayState() const { return m_ulIsRecordPlaying; }
    void SetRecordPlayState(ULONG val) { m_ulIsRecordPlaying = val; }
private:
    // �Ƿ�����¼��ط�ҳ����ץ��
    ULONG m_ulIsRecordCaptureing;
public:
    ULONG GetRecordCaptureState() const { return m_ulIsRecordCaptureing; }
    void SetRecordCaptureState(ULONG val) { m_ulIsRecordCaptureing = val; }
private:
    // �Ƿ�����¼��ط�ҳ���б�ǩ
    ULONG m_ulIsRecordLabeling;
public:
    ULONG GetRecordLabelState() const { return m_ulIsRecordLabeling; }
    void SetRecordLabelState(ULONG val) { m_ulIsRecordLabeling = val; }
private:
    // �Ƿ�����¼��ط�ҳ��������
    ULONG m_ulIsRecordSounding;
public:
    ULONG GetRecordSoundState() const { return m_ulIsRecordSounding; }
    void SetRecordSoundState(ULONG val) { m_ulIsRecordSounding = val; }
private:
    // �Ƿ�����¼��ط�ҳ���зŴ�
    ULONG m_ulIsRecordMagnifying;
public:
    ULONG GetRecordMagnifyState() const { return m_ulIsRecordMagnifying; }
    void SetRecordMagnifyState(ULONG val) { m_ulIsRecordMagnifying = val; }
private:
    // �Ƿ�����¼��ط�ҳ������Ƶ��ǽ
    ULONG m_ulIsRecordWalling;
public:
    ULONG GetRecordWallState() const { return m_ulIsRecordWalling; }
    void SetRecordWallState(ULONG val) { m_ulIsRecordWalling = val; }
private:
    // �Ƿ�����¼��ط�ҳ�������ܷ����������
    ULONG m_ulIsRecordAnalysising;
public:
    ULONG GetRecordAnalysisState() const { return m_ulIsRecordAnalysising; }
    void SetRecordAnalysisState(ULONG val) { m_ulIsRecordAnalysising = val; }
private:
    // �Ƿ�������ʾ¼��طŹ�����
    ULONG m_ulIsRecordBarShowing;
public:
    ULONG GetRecordBarState() const { return m_ulIsRecordBarShowing; }
    void SetRecordBarState(ULONG val) { m_ulIsRecordBarShowing = val; }
private:
    // �Ƿ�������ʾ���ܷ���������
    ULONG m_ulIsAnalysisBarShowing;
public:
    void SetAnalysisBarState(ULONG val) { m_ulIsAnalysisBarShowing = val; }
    ULONG GetAnaylysisBarState() const { return m_ulIsAnalysisBarShowing; }

private:
	bool m_bSyncMode;
public:
	void SetSyncPlayBackMode(bool bSync) {  m_bSyncMode = bSync; }
	bool IsSyncPlayBack()const { return m_bSyncMode; } 

private:
	//�Ƿ���ѵ����
	bool m_bIsTurnCruise;
	//��ѵ״̬
	ULONG m_ulTurnCruiseStyle;
	//��һ��״̬
	ULONG m_ulPrveTurnCruiseStyle;
	//��������
	TYPE_WND m_ulTypeWnd;
	//��ʱ�ط�Bar״̬
	ULONG m_ulInstantReplayStyle;
	//�Ƿ�չʾ����Bar
	ULONG m_ulAllBarStatus;
public:
	//������ѵ����
	void SetTurnCruiseStatus(bool val) {m_bIsTurnCruise = val;}
	bool GetTurnCruiseStatus()const {return m_bIsTurnCruise;}
	LONG DeleteTurnCruiseStatus();
	//������ѵ״̬
	void SetTurnCruiseStyle(ULONG val) {m_ulTurnCruiseStyle = val;}
	ULONG GetTurnCruiseStyle() const { return m_ulTurnCruiseStyle; }
	// ������ѵ״̬
	ULONG GetPrveTurnCruiseStyle() const { return m_ulPrveTurnCruiseStyle; }
	void SetPrveTurnCruiseStyle(ULONG val) { m_ulPrveTurnCruiseStyle = val; }

	void InvalidateCruiseBar();
	//���ô�������
	void SetTypeWnd(TYPE_WND val) {m_ulTypeWnd = val;}
	TYPE_WND GetTypeWnd()const {return m_ulTypeWnd;}
	//���ü�ʱ�ط�
	void ShowInstantReplayBar(ULONG val);
	ULONG GetShowInstantStatus()const{return m_ulInstantReplayStyle;}
	//�ָ�ʵ��
	void ResumeRealPlay();
	//��ʼ¼��ط�
	LONG StartRecordPlay();
	//ֹͣ¼��ط�
	LONG StopRecordPlay();
	// ��ͣ¼��ط�
	LONG PauseRecordPlay();
	//��������Bar��չʾ״̬
	void SetAllBarStatus(ULONG val){m_ulAllBarStatus = val;}
	//��������Bar
	void HideAllBar();
	//������������Bar
	void HideTwoBar();
private:

    // �Ƿ����ڻط�ҳ��������
    ULONG m_ulIsReplaySounding;
public:
    ULONG GetReplaySoundState() const { return m_ulIsReplaySounding; }
    void SetReplaySoundState(ULONG val) { m_ulIsReplaySounding = val; }
	void SetBarWindowsLong()const;
private:
    // �Ƿ�ʼ���ܷ�����ͼ
    bool m_bIsAnalysisDrawing;
    // ���״̬
    int m_ulCursorState;
public:
    int GetCursorState() const { return m_ulCursorState; }
    void SetCursorState(int val) { m_ulCursorState = val; }
private:
    //  
    CPoint m_startPoint;
    //  
    CPoint m_lastPoint;

    // ���ڼ�¼��갴�º��Ƿ��ƶ���������˫��ʱ��ѡ
    bool m_bIsMoving;
    // ���������״̬
    bool m_bIsButtonDown;
    // ���β���
    bool m_bIsFirst;
    // �޸����ܷ��������ƶ���־λ
    bool m_bIsMovable;
    // ��ǰ���Ĵ�������
    WND_TYPE m_ulWndType;
public:
    WND_TYPE GetWndType() const { return m_ulWndType; }
    void SetWndType(WND_TYPE val) { m_ulWndType = val; }
private:
    // �ͻ�������
    IVS_CLIENT_TYPE m_ulClientType;
public:
    IVS_CLIENT_TYPE GetClientType() const { return m_ulClientType; }
    void SetClientType(IVS_CLIENT_TYPE val) { m_ulClientType = val; }
private:
    //  
    bool m_bIsLocalRecord;

	// �Ƿ��Ǳ����������˱���¼��
	// ����������£��رմ���ʱ �ж��Ƿ��Ǳ����ڴ���������¼��
    bool m_bLocalRecordTrigger; 
public:
    bool GetIsLocalRecord() const { return m_bIsLocalRecord; }
    void SetIsLocalRecord(bool val) { m_bIsLocalRecord = val; }
	bool GetIsLocalRecordTrigger() const { return m_bLocalRecordTrigger; }
	void SetIsLocalRecordTrigger(bool b) { m_bLocalRecordTrigger = b; }

private:
    //  
    bool m_bIsPlatRecord;
public:
    bool GetIsPlatRecord() const { return m_bIsPlatRecord; }
    void SetIsPlatRecord(bool val) { m_bIsPlatRecord = val; }
private:
    //  
    bool m_bIsPuRecord;
public:
    bool GetIsPuRecord() const { return m_bIsPuRecord; }
    void SetIsPuRecord(bool val) { m_bIsPuRecord = val; }
private:
    //
    TCHAR m_szCameraID[IVS_DEV_CODE_LEN + 1];
public:
    const TCHAR* GetCameraID() const{ return m_szCameraID; }
	void SetCameraID( const TCHAR* szCameraID );

private:
    // �Ƿ���ʾ������־
    bool m_bIsResume;

    //�Ƿ���ʾ��ǽ������Ļ��־
    bool m_bIsTvWallBg;

	//�Ƿ����ǽ
	bool m_bIsTvWall;
public:
    bool GetResumeState() const { return m_bIsResume; }
    void SetResumeState(bool val) { m_bIsResume = val; }
public:
    bool GetTvWallBgState() const { return m_bIsTvWallBg; }
    void SetTvWallBgState(bool val) { m_bIsTvWallBg = val; }
public:
	bool GetTvWallState() const { return m_bIsTvWall; }
	void SetTvWallState(bool val) { m_bIsTvWall = val; }

private:
    // �Ƿ�ı䴰�񲼾�
    ULONG m_ulIsWndLayoutChange;
public:
    ULONG GetIsWndLayoutChange() const { return m_ulIsWndLayoutChange; }
    void SetIsWndLayoutChange(ULONG val) { m_ulIsWndLayoutChange = val; }
private:
    // ���������
    ULONG m_ulCameraType;
public:
    ULONG GetCameraType() const { return m_ulCameraType; }
    void SetCameraType(ULONG val) { m_ulCameraType = val; }
private:
    // �Ƿ�����Ѳ
    TCHAR m_szToolbarStatus[33];
    // �Ƿ�����Ѳ
    bool m_bIsTurning;
    // ��һ��¼��������״̬
    bool m_bLastSound;
public:
    void SetLastSoundState(bool val) { m_bLastSound = val; }
private:
    //  
    bool m_bStartPlayState;
public:
    bool GetStartPlayState() const { return m_bStartPlayState; }
    void SetStartPlayState(bool val) { m_bStartPlayState = val; }
private:
    //  
    ULONG m_ulPlayHandle;
public:
    ULONG GetPlayHandle() const { return m_ulPlayHandle; }
    void SetPlayHandle(ULONG val) { m_ulPlayHandle = val; }

public:
    void SetServiceType(IVS_SERVICE_TYPE enServiceType){m_enServiceType = enServiceType;}
    IVS_SERVICE_TYPE GetServiceType()const{ return m_enServiceType;}
private:
    IVS_SERVICE_TYPE m_enServiceType;
public:
	void PTZ3DMouseUp( const CRect &mainRect);
	void SetBackGroundBitmap(const CRect &rect);
	void InvalidateBar();
	void PTZ3DMouseDown( const CPoint &point );
	BOOL PointIsVideoRect( const CPoint &point );
	void GetDisplayScaleRect( CRect &mainRect );

	virtual BOOL OnInitDialog();
    afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnPaint();
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg LRESULT OnSyncPlayBackMsg(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnResumeRealPlayOKMsg(WPARAM wParam,LPARAM lParam);

	afx_msg LRESULT OnAsyncStreamMsg(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnAsyncEventMsg(WPARAM wParam,LPARAM lParam);
	
public:
    bool IsAnalysisDrawing() const { return m_bIsAnalysisDrawing; }
    void IsAnalysisDrawing(bool val) { m_bIsAnalysisDrawing = val; }

public:
    // �ж����ָ���Ƿ�����Ļ����
	bool IsInCenter( const CPoint &point )const;
    void HideVideo(void);
	BOOL IsPtInWnd( const CWnd* pWnd, const CPoint &point )const;
	bool IsPtNotInVideoWnd( const CPoint &point )const;
	bool MoveEagleEye()const;
	void MoveVideo();
	void MoveVideoPane();

	//����ǽ���һ������һ��������Ϊ4���Ӵ���,�����߷ָ�
	//void SplitTVWallPaneToFour();
	//��ʾ����ǽ���һ�� �ָ���
	//void ShowTVWallSplitLine();

    HWND GetVideoWnd(void) const{ return m_Video.GetSafeHwnd(); }
    // ���ù�����״̬
	void SetToolbarEnableStatus( ULONGLONG dwToolbarBtn, ULONGLONG lButtonStatus );
	void SetToolBarBtnVisableStatus(ULONGLONG dwToolbarBtn);
	ULONGLONG GetToolBarBtnVisableStatus(void)const;
	void ReSetToolBarStatus();
    // ����Ϊ�����
    void SetActivePane(void);
    // �߿������븴λ
    void SetBorder(ULONG ulBorderState);

    void SetActiveBorder();

    //���þ����״̬
    ULONG SetWarnState(ULONG ulWarnState,ULONG ulWarnSeconds);
    // ������̨�������ͼƬ
	void SetPtzCursor( const CPoint &point )const;
    // ������̨���ƶ���
	void SetPtzControl(const CPoint &point, const TCHAR *pCameraCode, const TCHAR *pMode, const TCHAR *pSpeed);
    void SetPtzControl(const CPoint &point);
    //
    void OnRealtimeLButtonUp( const CPoint &point);
    // ����ʵ���������а�ť״̬
    void SetToolBarBtnState(BOOL bState)const;
    // ���ñ�������ť״̬ͼƬ
    void SetTitleButtonPic(bool bState)const;
    // ����ʵ���������в���״̬ͼƬ
    void SetRealtimePlayPic(ULONG ulState)const;

    void ResetPane();

    ULONG SetPartMagnifyState( ULONG ulState );

	void OnRealtimePaneMouseMove( const CPoint &point, const CRect &mainRect );

    void ResetBottomButton();

	//����ˮӡ״̬
	void SetWaterMarkStatus(ULONG ulStatus);
private:
    // ����Ƶ����
    CVideoStatic m_Video;
	CVideoStatic m_EagleEye;
    CRect m_TitleBarRect;
    CRect m_ToolBarRect;
	CRect m_TVWallStatusBarRect;//����ǽͨ��״̬ͼ����
	CRect m_TurnCruisePaneRect;
	CRect m_DigtalRect;
	CRect m_pZoomDirctRect;
	CRect m_ZoomBarRect;
	CRect m_ZoomTitleBarRect;
	CRect m_TimerBarRect;
public:
    void ResetBarPosition(void);
    void SetBarRect();

	void DeleteBar();
	CPoint GetInfoButtonRightBottomPosition()const;

    CIVS_OCXPlayerCtrl *GetOCXPlayerCtrl();
    CVideoPaneMgr *GetVideoPaneMgr();

    bool IsExistPlayHandle(unsigned long ulPlayHandle)const;

/************************************************************************/
/* draw  begin  add by zhangshufan00227377                              */
/************************************************************************/
private:
	std::string m_strDevName;

public:
	void		 SetDevName(const std::string &strDevName) {m_strDevName = strDevName; }
	std::string& GetDevName() {return m_strDevName; }//lint !e1536
	ULONG GetDrawHandle() const {return m_ulDrawHandle; }
private:

	ULONG			m_ulCurrentGraphicType;		// ������ѡ�е�ͼ������
	ULONG			m_ulCurrentGraphicGroup;	// ��ǰ�����ͼ����
	ULONG			m_ulGraphicState;		// ͼ��״̬����ͼ���޸ģ�
	bool			m_bIsDrawing;			// �Ƿ����ڻ�ͼ��δ���
	IA_GRAPHICS		m_CurrentGraphic;		// �洢CU���ý����Ŀ�ʼ��ͼ������
	ULONG			m_ulGroupDrawMaxNum;	// ����ͼ���������ͼ�θ���
    bool            m_bDrawEnabled;
	bool            m_bOverlayEnabled;
	ULONG			m_ulDrawHandle;			// ���ܷ�����ͼ���
	ULONG			m_ulCursorType;

	void DrawLButtonDown(const CPoint &point);
	void DrawLButtonUp(const CPoint &point) ;
	void DrawMouseMove(const CPoint &point) ;
	void DrawRButtonDown(const CPoint &point);
	void DrawRButtonUp(const CPoint &point);

public:
	// ��ʼ��ͼ����ͼ������
	LONG SetCurrentGraphicAttribute(const IA_GRAPHICS *pGraphicAttribute, ULONG ulGroupDrawMaxNum);
	
	// ���õ�ǰͼ������
	VOID SetCurrentGraphicType(ULONG ulGraphicType);

	// ���õ�ǰͼ������
	VOID SetCurrentGraphicGroup(ULONG ulGraphicGroup);

	// ���õ�ǰ��ͼ״̬
	VOID SetCurrentGraphicState(ULONG ulGraphicState);

	// ��ȡ��ǰ��ͼ״̬
	ULONG GetCurrentGraphicState()const;

	// �ж��Ƿ�����ɾ��
	BOOL CheckAllowDelete();
	
	// ����ɾ���ؼ�״̬
	VOID SetDeleteButtonState(BOOL bAllowDelete);

	// �������ܷ�����ͼ
	IVS_INT32 EnableDraw(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle);

	// �ر����ܷ�����ͼ
	IVS_INT32 DisableDraw(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle);

	// ����ͼ��
	IVS_INT32 SetGraphics(const IA_GRAPHICS *pGraphics, ULONG ulNum, BOOL bPersent);

	//�������ܷ���ͼ����֮��Ĺ�ϵ
	IVS_INT32 SetRelationShip(ULONG ulGraphicGroupA, ULONG ulGraphicGroupB, RELATION_SHIP_TYPE ulRelationShipType);

	// ��ȡͼ��
	IVS_INT32 GetGraphics(IA_GRAPHICS *pGraphics, ULONG *pulNum);
	
	// ��ȡѡ�е����ܷ���ͼ��
	IVS_INT32 GetGraphicSelected(IA_GRAPHICS *pGraphic);

	// �����ȡ���ܷ���ͼ��
	IVS_INT32 GetGraphicsByGroup(ULONG ulGraphicGroup, IA_GRAPHICS *pGraphics, ULONG *pulNum);

	// ɾ��ѡ��ͼ��
	IVS_INT32 DeleteGraphicsSelected();

	// ɾ��ָ��ͼ���������ͼ��
	IVS_INT32 DeleteGraphicsByGroup(ULONG ulGraphicGroup);

	// ɾ������ͼ��
	IVS_INT32 DeleteGraphics();

	//���ܷ�������ĳͼ����Ļ�ͼ�����������ڻ��ĸ���ͼ�Σ�ֱ�Ӷ�����
	IVS_INT32 StopDrawGraphics(ULONG ulGraphicGroup);
	IVS_INT32 StopDrawGraphicWhileDraw(ULONG ulGraphicGroup);

	//ѡ��ָ��ͼ���������ͼ�Σ�׼���޸�
	IVS_INT32 SelectGraphicByGroup(ULONG ulGraphicGroup);

	// ����ѡ��ͼ��
	IVS_INT32 SelectGraphicByPoint(const POINT &CurrentPoint);

	//ȡ��ѡ�����ܷ���ͼ��
	IVS_INT32 SelectCancel();
    
    //ȡ��ѡ�����ܷ���ͼ��
    IVS_INT32 SelectCancelWhileDraw();

	// ��ȡ��ǰѡ��ͼ��ID
	ULONG GetCurrentGraphicGroup()const{ return m_ulCurrentGraphicGroup; }
	
	VOID GetCursorType(const POINT &Point);
	VOID SetCursorType()const;

private:

	ULONG	m_ulOverlayHandle;		// ���ܷ����켣���Ӿ��
	ULONG	m_ulRequestID;			// �켣����ID

public:

	// �������ܷ����켣����
	IVS_INT32 EnableOverlay(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, ULONG ulRuleType);

	// �ر����ܷ����켣����
	IVS_INT32 DisableOverlay(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle);
	
	// ���ܷ����켣���Ӳ���켣��Ϣ
	IVS_INT32 InsertIALayoutMessage(const IA_LAYOUT_MESSAGE &LayoutMessage);

	// ��ȡ��ǰ�켣��Ϣ����
	IVS_INT32 GetCurrentElementNum();


/************************************************************************/
/*  draw end                                                            */
/************************************************************************/
	
/************************************************************************/
/* Capture  begin  add by zhangshufan00227377                           */
/************************************************************************/
public:
	IVS_INT32 LocalSnapshot(IVS_INT32 iSessionID, IVS_LOCAL_CAPTURE_PARAM &LocalCaptureParam);
	IVS_INT32 LocalSnapshotToDir(IVS_INT32 iSessionID, IVS_UINT32 uiPictureFormat, const IVS_CHAR *pFileName);
	IVS_VOID StopSerialSnapshot();

	//����¼����Ƶ����
	IVS_INT32 SetVideoEffect(IVS_INT32 iSessionID, IVS_LONG lBrightValue,
		IVS_LONG lContrastValue, IVS_LONG lSaturationValue,
		IVS_LONG lHueValue);

private:
	IVS_INT32 CheckLocalCaptureParam(const IVS_LOCAL_CAPTURE_PARAM &LocalCaptureParam)const;
	IVS_INT32 GetTime(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_LOCAL_CAPTURE_PARAM &LocalCaptureParam)const;
	IVS_INT32 BuildFileName(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_LOCAL_CAPTURE_PARAM &LocalCaptureParam);
	IVS_INT32 BuildFullPath(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_LOCAL_CAPTURE_PARAM &LocalCaptureParam);
	IVS_INT32 SingleCapture(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_LOCAL_CAPTURE_PARAM &LocalCaptureParam);
	IVS_INT32 StartSerialCapture(IVS_INT32 iSessionID, const IVS_LOCAL_CAPTURE_PARAM &LocalCaptureParam);
	IVS_INT32 CaptureRetToCU(IVS_INT32 iResult, IVS_LOCAL_CAPTURE_PARAM &LocalCaptureParam);
	static IVS_INT32 ReachTimeInterval(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, const IVS_LOCAL_CAPTURE_PARAM &LocalCaptureParam, BOOL &bReachTimeInterval);
	static IVS_UINT32 __stdcall CaptureThreadProc(void *pParameter);
	
	IVS_LOCAL_CAPTURE_PARAM m_LocalCaptureParam;
	HANDLE					m_CaptureThreadHandle;
	bool					m_bCaptureThreadRun;
	IVS_INT32				m_iSessionID;

/************************************************************************/
/*  Capture end				                                            */
/************************************************************************/

	HBITMAP m_hPaneBKGBitmap;
    HBITMAP m_hPaneALARMBitmap;
public:
	void EnableALLButton(BOOL isEnable);

/*
 * add by w00210470
 * ����ʵ������±�������ͷ��Ϣ�����������ü��
 */
public:
	bool VerifySameStream(const char *szCamID, unsigned int type)const;
	void SetRealStreamInfo(const char *szCamID, unsigned int type);

    //����ͼƬ��ͬʱҪ���ֱ���(ͼƬ����Ϊ4:3��
    int m_nZoomWidth;
    int m_nZoomHeigth;

    void SetAnalysisBarIsStartPlay(BOOL IsStartPlay);
	void HideTranslucenWnd(void);
private:
	char         m_CamReal[IVS_DEV_CODE_LEN + 1];
	unsigned int m_uiStreamType;
public:
    const char *GetCamReal() const{ return m_CamReal; }


/* 
 * ʵ���첽����ֹͣ
 */
public:
     
	LONG StartRealSteamAsync(IVS_REALPLAY_PARAM &realplayParam, LPCTSTR pCameraID, 
		BOOL bLayoutChange, ULONG ulStreamOpenMode, unsigned long tranid);

	int StopRealStreamAsync();
	int StopRealStreamSync();
	void StopRealStream();

	static unsigned int __stdcall BegRealPlayProc(LPVOID lpParameter);

private:

	enum MSG_TYPE {
		ASY_MSG_NONE          = 0,
		ASY_MSG_STOP_SUCCESS  = 1,
		ASY_MSG_START_SUCCESS = 2,
		ASY_MSG_START_FAIL    = 3
	};

    enum THREAD_STATE {
		THREAD_RUNNING = 0,
		THREAD_EXITING = 1
	};

	class  REALPLAY_CMD {
	public:
		unsigned long transid;
		bool bStop;
		char CamID[IVS_DEV_CODE_LEN + 1];
		IVS_REALPLAY_PARAM Param;

		REALPLAY_CMD()
		{
			transid = 0;
			bStop  = 0;
			CamID[0] = '\0';
			memset(&Param, 0, sizeof(Param));
		}

		REALPLAY_CMD (const REALPLAY_CMD& rhs)
		{
			transid = rhs.transid;
			bStop   = rhs.bStop;
			memcpy(CamID, rhs.CamID, sizeof(CamID));
			memcpy(&Param, &rhs.Param, sizeof(Param));
		}
	};

	typedef std::list<REALPLAY_CMD> REALPLY_CMD_LIST;

	CIVSMutex        m_CmdLock;
	unsigned int     m_uCmdSeq;
	REALPLY_CMD_LIST m_CmdList;
    HANDLE  m_hWorkThread;
	int     m_ThreadState;

public:
	//virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);//lint !e1768
	BOOL TransStrToTime(const CString &str,ST_TIME* pTime)const;
	void SetInstansPlayStatus(BOOL val){m_bIsInstanstPlayBack = val;}
	BOOL GetInstansPlayStatus()const{return m_bIsInstanstPlayBack;}
	void ShowInstantBar();
private:
	unsigned long long m_ullWinAlarmFlag;//�Ƿ�Ը澯��ť�û�
public:
	void SetAlarmButtonStatus(unsigned long long istatus);
	//������ǩ��ť״̬
	void SetBookmarkButtonStatus(unsigned long long istatus);
    void SetStartPlayTime();
	void SetSkin();
	void SetTitleBarStatus(ULONG ulStatusType, ULONG ulShow);
	void SetTitleBarInitStatus();
    ULONG GetTVWallStatus()const;
private:
	BOOL m_bIsMoveVideoWindow;
	BOOL m_bIsHideTranslucentBar;
	bool m_bIsBarCreated;
	bool m_bIsRefresheed;
	ULONG m_ulTVBarStatus;

	ULONG m_ulShowCruiseTrackStatus;
	ULONG m_ulShowAlarmStatus;
	ULONG m_ulShowRecordStatus;
	ULONG m_ulShowSingleFrameStatus;
};

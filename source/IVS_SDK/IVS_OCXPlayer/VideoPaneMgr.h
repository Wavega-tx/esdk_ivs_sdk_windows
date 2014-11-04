/*****************************************************************
 filename    :    VideoPaneMgr.h
 author      :    ykf75928
 created     :    2013/01/15
 description :    ���������
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013/01/15 ��ʼ�汾
*****************************************************************/
#pragma once

#include "Lock.h"
#include "common_struct.h"
#include "VideoPane.h"
#include "ParentWnd.h"
#include "OCXTVWallXMLProcess.h"

/*lint �Celib(129) */

// using namespace std;

#define INTERNATIONAL_WORD_LEN   (100) // ���ʻ����ʳ���
const double RATIO_PTZ_GRID = 0.35;
const LONG INTERNATIONAL_WORD_NUM = 27;   // ���ʻ����ʸ���

class CVideoPaneMgr
{
public:
    CVideoPaneMgr(void);
    ~CVideoPaneMgr(void);
private:

private:
    // �����ھ�������ڴ�������ʱ�趨�丸����
    CWnd *m_pParentWnd;
    COleControl* m_pOcxCtrl;       //Ctrl��ָ��
public:
    const COleControl* GetOcxCtrl() const { return m_pOcxCtrl; }
    ULONG SetOcxCtrl(COleControl* val);
	CWnd *GetParentWnd(){return m_pParentWnd;}
private:    // ��������
    WND_TYPE m_ulWndType ;
public:
    ENUMDEF::WND_TYPE GetWndType() const { return m_ulWndType; } 
    void SetWndType(ENUMDEF::WND_TYPE val){ m_ulWndType = val; }  
    bool isAnalysisWndType()const;
private:
	IVS_WIND_LAYOUT m_windLayOut;
	IVS_ULONG m_ulTVWallID;			//��ǰ�����ĵ���ǽ���
public:
	//���ص�ǰ�����ĵ���ǽID
	IVS_ULONG GetCurTVWallID()const {return m_ulTVWallID;}
public:
	IVS_WIND_LAYOUT GetWindLayOut() const { return m_windLayOut; }
	void SetWindLayOut(IVS_WIND_LAYOUT val) { m_windLayOut = val; }
private:

private:
    // ��ʾ�Ĵ������
    ULONG m_ulShowWndNum;
public:
	ULONG GetNumOfDisplayPane() const { return m_ulShowWndNum; }
	void SetNumOfDisplayPane(ULONG val) { m_ulShowWndNum = val; }
public:
    // �Ƿ�Ŵ�
    LONG m_ulIsMagnify;

    //����map��
    MAP_VIDEOPANE m_videoPaneMap;
    //����map�����
    CIVSMutex m_csVideoPaneLock;

	ULONG SetWarnState( ULONG ulWndID, ULONG ulWarnState, ULONG ulWarnSeconds );

    //�ײ���������ťȨ��
    TCHAR  m_strToolbarStatus[33];

public:
	ULONG GetToolbarBtnNum() const { return m_uToolbarBtnNum; }

    // ��ȡ�Ŵ�״̬
    LONG GetMagnifyState() const { return m_ulIsMagnify; }
    // ��ȡȫ��״̬
    ULONG GetFullScreenState() const { return m_ulIsFullScreen; }
	void SetFullScreenState(ULONG ulFullScreen) { m_ulIsFullScreen = ulFullScreen; }
    void SetColor(ENUMDEF::COLOR_MODE ulColor);
private:
    // ��ǰ�����ID��Ĭ��Ϊ1,��Χ��1-64
    ULONG m_ulActivePaneID;
public:
	ULONG GetActivePaneID() const { return m_ulActivePaneID; }
	//���û����ID
	void SetActivePaneID(ULONG val) { m_ulActivePaneID = val; }
private:
    // ��ǰ�����Ĵ���ID
    ULONG m_ulOperatePaneID;
    // �ͻ�������
    IVS_CLIENT_TYPE m_ulClientType;
public:
    // ��ÿͻ�������
    IVS_CLIENT_TYPE GetClientType() const { return m_ulClientType; }
    // ���ÿͻ�������
    LONG SetClientType(IVS_CLIENT_TYPE ulClientType);
private:
    //�Ƿ�ȫ��;
    ULONG m_ulIsFullScreen;
    // �Ƿ���ʾ���⹤����
    bool m_IsShowTitleBar;
	bool m_isShowToolBar;
	bool m_isChangeToolBar;	//����Ҫ��ʾ�Ĺ������Ƿ��б仯
public:
	// �Ƿ���ʾ���߾���
	ULONG SetToolbar(ULONG ulToolBar);
    //�������ܷ�����toolbar
    ULONG SetIAToolbar( ULONG ulToolBar );
    // ��ȡ��������ʾ״̬
    bool GetTitleBarShowState() const { return m_IsShowTitleBar; }
	bool GetToolBarShowState() const { return m_isShowToolBar; }
	bool GetShowToolBar() const { return (m_isShowToolBar && ((0 != m_uToolbarBtnNum )|| (0 != m_uAnalysisBtnNum))); }
	void SetShowToolBar(bool val) { m_isShowToolBar = val; }
private:
    // ��������ʾ״̬
    ULONG m_uToolbarBtnNum;
    ULONG m_uAnalysisBtnNum;
    // ��ʾ����
    ULONG m_ulDispalyAspectRatio;
private:
    // �Ƿ��Զ��任����
    LONG m_lAutoChangeLayout;
	// ���汻�Ŵ�ʱ�Ĳ��֣��������ŵȲ��������˳�
	IVS_WIND_LAYOUT m_MagnifyLayOut;
	// 1_1���ֵ�paneָ�룬���ڵ������޷������ı���
	CVideoPane* m_pVideoPane_1_1;
	//�Ƿ������񽻻�
	ULONG ulEnableExchangePaneStatus;
	ULONG m_ulParentHanlde;
	ULONG m_ulOldParentHanlde;
	//��ǰƤ������
	ULONG m_ulSkinType;
public:
    LONG GetAutoChangeLayout() const { return m_lAutoChangeLayout; }
    void SetAutoChangeLayout(LONG val) { m_lAutoChangeLayout = val; }

	//����Ƥ������
	ULONG GetParentHwnd()const {return m_ulParentHanlde;}
	void SetSkin(ULONG val);
	ULONG GetSkinType()const{return m_ulSkinType;}
public:
    LONG Init(void);
    //  �Ŵ����С������Ϊ�Ƿ����ڲ���
	void ChangeSize( ULONG ulIsPlaying );
    // ��С����
    void Minify(void);
    // �Ŵ󴰸�
    void Magnify(CRect& rect);
    // ����Ƿ���Խ�������

    ULONG CheckIfCanExchange(const CVideoPane * pFirstPane) const;

    // ɾ������SoundCtrl��Talkback�Ի���ɾ��Replay�����Ի���
    void DeleteSoundCtrl(void);
    // ɾ��SoundCtrl��Talkback�Ի���, ���Ի������ɾ��Replay�����Ի���
    void DeleteSoundCtrlForOther(void);
    //  �������񣬲���Ϊ��׽���ɿ��������Ĵ���

    void ExchangePane(ULONG ulMouseUpPaneID);

    // ��ȡ�����ָ��
    CVideoPane* GetActiveWndPointer(void);
    // ��ȡ���д���

    ULONG GetAllVideoPane(MAP_VIDEOPANE & videoPaneMap) const;

    // ���ݾ����ȡ����ID
	ULONG GetWindIDByHWND( const HWND &ulWnd );
    // ������������ȡ����ID
	ULONG GetWndIDByMouse( void );
    // ��ȡ���еĴ���(�Ǹ澯����)
	ULONG GetWndIDFree( void );
    // ��ȡָ������ָ��
    CVideoPane* GetWndPointerByHandle(long lHandle);
    // ��ȡָ������ָ��
	CVideoPane* GetWndPointerByHWND( const HWND hWnd );
    // ��ȡָ������ָ��
	CVideoPane* GetWndPointerByID(ULONG ulWndID);
	// �����豸�����ȡָ������ָ��
	CVideoPane* GetWndPointerByCameraCode(const std::string& strCameraCode);
	// ��ȡ�ֲ��Ŵ󴰿�
	ULONG GetMagnifyFreeWnd(ULONG ulPaneID);
	// ��ȡ�ĸ����ڿ����˾ֲ��Ŵ�
	bool GetMagnifyOpenWnd(ULONG& ulWndID);
    void HideVideoPane(void);
	// ȡ�����д��ڵľֲ��Ŵ�״̬
	void CancleAllMagnifyWnd();
	void SetMagnifyLayOut(IVS_WIND_LAYOUT layout){ m_MagnifyLayOut = layout; }
	IVS_WIND_LAYOUT GetMagnifyLayOut()const{ return m_MagnifyLayOut; }
    // �ж��Ƿ��������Խ�
    BOOL IsTalkback(void);
    // ��������Ӧ�Ի���
    void OnCurrentLocker(const CPoint &point)const;
    void OnHideAllLocker(void);
    void OnLButtonDblClk(UINT nFlags, CPoint point);
    void OnLButtonDown( UINT nFlags, CPoint point );
    BOOL OnMouseWheel(UINT nFlags, short zDelta, const CPoint& pt);
    void RefreshAllWindow(void)const;
    // �ָ�ȫ��
    void ResetFullScreen(void);
    // ��λ������Ϣ

    ULONG Set3DPTZState(ULONG ulWndID, ULONG ulIsStart3D);

    // �ɴ�����ã���������ѡ���ı߿���ɫ
	void SetActivatePane( ULONG PaneID );
	ULONG SetAddAnalysisResult(ULONG ulWinID, bool bState);
	ULONG SetAlarmState(ULONG ulWndID, bool bIsOpen);

    // ���ð�ť״̬����ť�ûң�
	void SetButtonStatusOff(ULONG ulWinID, LONG ulButtonType);
    // ���ð�ť״̬����ť������
	void SetButtonStatusOn( ULONG ulWinID, LONG ulButtonType);
	ULONG SetCaptureState( ULONG ulWndID, bool bIsOpen );
    // �����豸����

    ULONG SetDevName(ULONG ulWndID, LPCSTR pDevName);
	IVS_INT32 SetDisplayScale(IVS_INT32 iSessionID, ULONG ulDisplayScale);
	ULONG     GetDispalyScale() const {return m_ulDisplayScale;}

    void SetFullScreen(void);
    // ��������
    LONG SetLanguage(LPCTSTR pLanguage);

    //����ƽ������Ĳ��֣�1,4,9�ȣ�

    void SetLayoutAsAverage( const CRect &rect, ULONG ulLayOutNum ) ;
    ULONG SetManifyWindow(ULONG ulWndID, ULONG ulManifyState);
    ULONG SetMicVolume(ULONG ulVolumeValue) const;
	ULONG GetMicVolume(void) const;

    // ���²���ǰ���¼�����¼��һЩ����
	ULONG SetParameter(ULONG ulLayoutType);
	ULONG SetRecordState(ULONG ulWndID, bool bIsOpen);
	ULONG SetSoundState( ULONG ulWndID, bool bIsOpen );
	ULONG SetSoundStateToFalse(ULONG ulWndID);
    // ���ù�����Ȩ��
    LONG SetToolbarEnableStatus( ULONG ulWndID ,ULONGLONG dwToolbarBtn, ULONGLONG lButtonStatus );
    // ���ݴ����λ�����ݲ�����Ƶ���Ŵ�������������߻�����
	void SetVideoPanePosition(CRect& rect) ;
    // ���������Խ�����
    LONG SetVoiceTalkVolume(LONG nCoefficient) const;
    // ������·��Ƶ����
    LONG SetVoiceVolume(LONG ulVolumeValue) const;
    // ���þ���״̬
	ULONG SetWarnStateSetWarnState( ULONG ulWndID, ULONG ulWarnState, ULONG ulWarnSeconds );
    // ������·��Ƶ����

    ULONG SetWaveAudio(ULONG ulWndID, ULONG ulVolumeValue);

    // ���ô��񲼾֣��������Ϊλ�������벼������
	LONG SetWndLayout(CRect rect, ULONG ulLayoutType);
	// ���ݵ���ǽ������ò���
	LONG SetTVWallWndLayout(const CRect& rect, IVS_ULONG ulTVWallID);
    // ���ô����Զ��岼�֣��������Ϊλ�������벼������
	LONG SetCustomLayout( CRect rect, RECTLIST &videoRects, const IVS_UINT32 uX,const IVS_UINT32 uY );
    // ���ô������ʽ����, 

    ULONG SetLadderLayout(CRect rect,ULONG ulPaneNUm );
    // ��ʾ/���ر��⹤����
	void ShowTitleBar(  bool isShow );
	// ��ʾ/���ع�����
	void ShowToolBar( bool isShow);

    ULONG StartVoiceTalkback(const TCHAR* pVoiceCode, ULONG ulDecoderType, ULONG ulStreamType);
    ULONG StopAllRealPlay(void);
    ULONG StopAllSound(ENUMDEF::WND_TYPE val);

    // �ر���·����
    void StopSoundCtrlForOther(bool bCloseAll);
    // �ر������Խ�,���Ի������
    void StopTalkbackSoundForOther(void);

    ULONG StopVoiceTalkback(void) const;

	//������ѵ״̬
	ULONG SetTurnCruiseStyle(ULONG ulWndID,ULONG ulStyle=0);

	//��������͸������
	void HideAllTranslucenWnd(void);
private:

    // ȫ��ǰ����Ļ��С
    WINDOWPLACEMENT m_struOldWndpl;
	ULONG           m_ulDisplayScale;

public:
    //��ɫ���ò���
    COLORREF m_VideoPaneBKGColor;
    COLORREF m_ActiveEdgeColor;
    COLORREF m_NotActiveEdgeColor;
    COLORREF m_TitleBarBKGColor;
    COLORREF m_BottomBarBKGColor;
    COLORREF m_RectangleBKGColor;
    COLORREF m_TextColor;
    LONG m_ulShadeStartRGB;
    LONG m_ulActiveEdgeRed;
    LONG m_ulActiveEdgeGreen;
    LONG m_ulActiveEdgeBlue;

    void ResetBarPosition(void);

	bool isPaneExistByID(ULONG ulPaneId);

private:
	ULONGLONG  m_ullToolBarBtnVisableStatus;
public:
	ULONGLONG GetToolBarVisableStatus() const { return m_ullToolBarBtnVisableStatus; }
	void SetToolBarVisableStatus(ULONGLONG val);

    void HideAllBarOnce();
	void SetAllBarWindowsLong(ULONG ulHandle);
    IVS_INT32 GetHwndbyPlayHandle(unsigned long ulPlayHandle, HWND& hWnd);
    bool IsLegalityWndID( ULONG lWndID )const;
public:
	/******************************************************************
	function : SetTVWallPaneChannelStatus
	description: ���õ���ǽ����ͨ��״̬
	input : ULONG ulWndID
	input : ULONG ulStatus
	output : NA
	return : void
	*******************************************************************/
	void SetTVWallPaneChannelStatus(ULONG ulWndID,ULONG ulStatus);

	/******************************************************************
	function : SetTVWallPaneStatus
	description: ���õ���ǽ����״̬
	input : ULONG ulWndID
	input : ULONG ulPaneStatus
	output : NA
	return : void
	*******************************************************************/
	void SetTVWallPaneStatus(ULONG ulWndID,ULONG ulPaneStatus);

	void StartSerialCaptureAll();
private:
	WND_TYPE m_TVWallWndType;
public:
	void SetTVWallWndType(WND_TYPE wndType) {m_TVWallWndType = wndType;}
	WND_TYPE GetTVWallWndType()const {return m_TVWallWndType;}
	void ResetTVWallStatus();
	void DoButtonFilter(LONG& ulWndID);
public:
	typedef enum{
		SPLIT_1 = 1,
		SPLIT_4,
		SPLIT_9,
		SPLIT_16
	}SPLIT_WND_TYPE;
	typedef std::map<ULONG,CRect> PaneIDRectMap;//��Ҫ���ѵĴ���ӳ���
	PaneIDRectMap m_MapID1Rect;
	PaneIDRectMap m_MapID4Rect;
	PaneIDRectMap m_MapID9Rect;
	PaneIDRectMap m_MapID16Rect;
	RECTLIST m_SplitTo1RectList;//���Ϊ1�����������б�
	RECTLIST m_SplitTo4RectList;//���Ϊ4�����������б�
	RECTLIST m_SplitTo9RectList;//���Ϊ9������������б�
	RECTLIST m_SplitTo16RectList;//���Ϊ16������������б�
	typedef std::map<ULONG,TVWall_VPANEID> WNDID_MAP;//�����Ӧ��ϵ��
	WNDID_MAP m_MapWindowPaneID;
	LONG SplitTVWallWnd();//��ִ���
	void AddSplitRectMap(const SPLIT_WND_TYPE& emType,const CRect &rc);//���Ӳ�ִ��ڵ�ID��ӳ���
	bool ResetVideoPanePositon(const ULONG& ulPaneID,const CRect& rc);
	void GetRealTVwallPaneID(const TVWall_VPANEID_LIST& listPaneID,std::vector<ULONG>& vecRealPaneID);//�������ⴰ��ID�б��ȡ��ʵ�Ĵ����б�
	void GetRealTVwallPaneIDByParentID(const std::vector<ULONG>& vecPaneID,std::vector<ULONG>& vecRealPaneID);//���ݸ�����ID��ȡ���е���ʵ����
	bool GetVirtualIDByRealID(const ULONG& ulPaneID,TVWall_VPANEID& vID);//������ʵ�Ĵ���ID��ȡ����Ĵ���id
	ULONG GetRealIDByVirtualID(const TVWall_VPANEID& stPaneID);//��������Ĵ���ID��ȡ��ʵ�Ĵ���id
	void UpdateTVWallPaneStatus();//ˢ�µ���ǽ����һЩ�ӿؼ�����ʾ��ɾ��δ�󶨽������ĵ���ǽͼ��
	void SetALLMagnifyBtnStatus(const CVideoPane* pVedioPane,ULONGLONG lButtonStatus);//// ���ó���ǰ�������������������
	void EnableExchangePane(ULONG ulStatus){ulEnableExchangePaneStatus = ulStatus;} //�Ƿ������񽻻�
	ULONG MoveOCXToRect(CRect& rect);
	ULONG FullScreenKeyUp(ULONG ulMsgKey);
    void NotifyOtherSoundOpen(ULONG ulWinID, bool isOpen);
    bool IsSoundStart(const CString& strCameraCode);
	void HideAllTitileBar();
	void HideAllToolBar();
	void HideShellTrayWnd(BOOL bShow)const;
    void UpdateTVWallOnlineStatus();
    ULONG GetTVWallPaneStatus(ULONG ulWndID);
	void DragMoveOutofOCX();
};


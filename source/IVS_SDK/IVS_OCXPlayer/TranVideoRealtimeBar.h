/*****************************************************************
 filename    :    TranVideoRealtimeBar.h
 author      :    ykf75928
 created     :    2013/01/15
 description :    ʵ����¼�񹤾���
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013/01/15 ��ʼ�汾
*****************************************************************/
#pragma once

#include <bitset>
#include "TranslucentDialog.h"
#include "translucentbutton.h"
#include "VideoSoundCtrl.h"
#include "IVSMenu.h"
#include "afxwin.h"

// CTranslucentVideoRealtimeBar dialog
//  MFC Dialog ����Ĭ�Ϲ��캯��

const ULONG SOUNDCTRL_TIMESPAN_LEN = 10;
const ULONG SOUNDCTRL_SLIDER_INTERVAL = 10; // ���������
const ULONG SOUNDCTRL_SLIDER_LEN = 100;     // ����������
const ULONG SOUNDCTRL_SLIDER_HEIGHT = 145;  // �������߶�
const ULONG SOUNDCTRL_WIDTH = 30;           // �����Ի���Ŀ��
#define  VOLUME_FULL                 0xFFFFFFFF

#define WM_SETSOUND_STATE (WM_USER + 1314)

class CTranVideoRealtimeBar : public CTranslucentDialog
{
    DECLARE_DYNAMIC(CTranVideoRealtimeBar)

public:
    virtual ~CTranVideoRealtimeBar();
	CTranVideoRealtimeBar(Gdiplus::Image* pImage, CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    enum { IDD = IDD_DLG_TVIDEOREALTIMEBAR};

    virtual void OnInitChildrenWnds();

	void SetButtonPicture(CTranslucentButton &tButton,const std::string &strButtonName);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
	CIVSMenu* m_pMenu;
	
    std::vector<CTranslucentButton *> m_btnVector;
#pragma region BTN
    CTranslucentButton m_buttonLocker;
	//ץ��
    CTranslucentButton m_buttonSnapShot;
	//����¼��
    CTranslucentButton m_buttonLocalRecord;
	//��ǩ
    CTranslucentButton m_buttonBookMark;
	//�ֲ��Ŵ�
    CTranslucentButton m_buttonMagnify;
	//��ʱ�ط�
	CTranslucentButton m_buttonInstantBackPlay;
	//����
    CTranslucentButton m_buttonPlaySound;
	//�Խ�
    CTranslucentButton m_buttonTalkback;
	//��Ƶ��ǽ
    CTranslucentButton m_buttonTVWall;
	//���ø澯����
	CTranslucentButton m_buttonSetWardWnd;
	//�ƾ�����
	CTranslucentButton m_buttonPtz;
	//�������ܷ���
	CTranslucentButton m_buttonSuperIntel;
	//�򿪵��ӵ�ͼ
	CTranslucentButton m_buttonEMap;
	//һ���ศ
	CTranslucentButton m_buttonMasterMAuxiliary;
	//��������/��������
	CTranslucentButton m_buttonNetQuality;
    //����3D��̨
    CTranslucentButton m_BtnPtz3D;
	//����ʵ����ť
	CTranslucentButton m_btnReturn;
	//��ʼ/��ͣ¼��ť
	CTranslucentButton m_btnStart;
#pragma endregion BTN

	// �Ƿ���չ��ʾ��ť
	bool m_isExtend;
	//CMenu
    IVS_INT32 m_iNumBtn;
    //ʵ��ҳ������������
    CVideoSoundCtrl * m_pVideoSoundCtrl;
	ULONG   m_ulSoundCtrlValue;          //��¼������ֵ
	BOOL    m_bSoundCtrlState;           //��������״̬ 
	BOOL    m_bIsInstansPlay;            //����Ǽ�ʱ�ط���Ҫ������ť
	BOOL    m_bIsStartPlay;              //����Ѿ������ط�
public:
    BOOL GetSoundCtrlState() const { return m_bSoundCtrlState; }
    void SetSoundCtrlState(BOOL val) { m_bSoundCtrlState = val; }
	void SetInstansPlayBack(BOOL val) {m_bIsInstansPlay = val;}
	void SetIsStartPlay(BOOL val);
	void SetRecordPlayStop(BOOL val);
private:

    BOOL    m_bSoundState;               //����״̬ �Ƿ��� 
	bool    m_bRecordStarted;
    bool    m_bPtz3DStarted;             //�Ƿ���3D

    CVideoSoundCtrl * m_pVideoTalkbackSound; //ʵ��ҳ���������Խ�
    ULONG   m_ulTalkbackSoundValue;          //��¼������ֵ
	bool	m_bShowTalkbackSoundCtrl; // �Ƿ���ʾ����������

	bool	m_bTalkbackSoundCreated; // �Ƿ��Ѿ�����
	bool	m_bSoundCreated; // �Ƿ��Ѿ�����
	bool	m_bMenuCreated; // �Ƿ��Ѿ�����
public:
	//BOOL GetTalkbackCtrlState() const { return m_bTalkbackCtrlState; }
	void SetTalkbackCtrlState(bool state);
	bool GetShowTalkbackSoundCtrl() const { return m_bShowTalkbackSoundCtrl; }
	void SetShowTalkbackSoundCtrl(bool val) { m_bShowTalkbackSoundCtrl = val; }
	
public:
    ULONG GetTalkbackSoundValue() const { return m_ulTalkbackSoundValue; }
    void SetTalkbackSoundValue(ULONG val) { m_ulTalkbackSoundValue = val; }
private:

    BOOL    m_bTalkbackSoundState;           //��������״̬
public:
    BOOL GetTalkbackSoundState() const { return m_bTalkbackSoundState; }
    void SetTalkbackSoundState(BOOL val) { m_bTalkbackSoundState = val; }
private:

    BOOL    m_bTalkbackSlienceState;         //����״̬
    BOOL    m_bIsTurning;                    //��Ѳ״̬
//private:
//	//�������ڱ������ĸ������Ѿ����Ǵ�����Ҫ��¼�¶�Ӧ�Ĵ���
//	CWnd* m_pVideoPane;
//public:
//    CWnd* GetVideoPane() const { return m_pVideoPane; } //lint !e1763  MFC�Զ����ɵĵĺ���
//	void SetVideoPane(CWnd * pVideoPane){m_pVideoPane = pVideoPane;}

private:
    // �����Ի���λ��
    ULONG   m_ulSoundCtrlLeft;
public:
    ULONG GetSoundCtrlLeft() const { return m_ulSoundCtrlLeft; }
private:
    ULONG   m_ulSoundCtrlTop;
public:
    ULONG GetSoundCtrlTop() const { return m_ulSoundCtrlTop; }
private:

public:
    afx_msg void OnPaint();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBnClickedButtonRealtimeCapture();
    afx_msg void OnBnClickedButtonRealtimeRecord();
    afx_msg void OnBnClickedButtonRealtimeLabel();
    afx_msg void OnBnClickedButtonRealtimeMagnify();
    afx_msg void OnBnClickedButtonRealtimeSound();
    afx_msg void OnBnClickedButtonRealtimeTalkback();
    afx_msg void OnBnClickedButtonRealtimeExtent();
	afx_msg void OnBnClickedBtnToolRealtimeInstantBackPlay();
	afx_msg void OnBnClickedBtnToolRealtimeTvwall();
	afx_msg void OnBnClickedBtnToolRealtimeSetWarnWnd();
	afx_msg void OnBnClickedBtnToolRealtimePtz();
	afx_msg void OnBnClickedBtnToolRealtimeSuperIntel();
	afx_msg void OnBnClickedBtnToolRealtimeEMap();
	afx_msg void OnBnClickedBtnToolRealtimeNetQuality();
    afx_msg void OnBnClickedBtnToolRealtimePtz3d();
	afx_msg void OnBnClickedBtnToolRealtimeMasterAuxiliary();
	afx_msg void OnBnClickedBtnToolRealtimeReturn();
	afx_msg void OnBnClickedBtnToolRealtimeStart();
	afx_msg	void OnMenuCmd(UINT nID);
    
public:
	virtual BOOL OnInitDialog();
    void CreateSoundCtrl();
    // ɾ���������ƶԻ���
    void DeleteSoundCtrl();
	void DeletePopMenuBar();
    // ��������Ի���
    const CVideoSoundCtrl* GetSoundCtrl() const{ return m_pVideoSoundCtrl;}
    //�ѹ�������ť��С����Ϊ0

    void ZeroSize();
    // ��������
    void ChangeSoundCtrlValue(ULONG ulType, ULONG ulValue);
    void InitSoundCtrl(ULONG ulType);
    // ��������
    void  StartSound();
    void  StopSound();
    ULONG GetSoundCtrlValue()const{return m_ulSoundCtrlValue;}
    void  SetSoundCtrlValue(ULONG ulValue){m_ulSoundCtrlValue = ulValue;}
    // ����״̬
    void  SetSoundSlienceFlag(BOOL bState);
    BOOL  GetSoundSlienceFlag();
    ULONG GetSoundState();
    void  DrawButton(IVS_INT32 iBtnNum = 6);
    // ���Clientģ��
    ULONG GetClientMode() const;
    // Talkback Sound
    // ɾ��Talkback�������ƶԻ���
    void DeleteTalkbackCtrl();
	void CreateTalkbackCtrl();
    IVS_INT32 StartTalkbackSound();
    IVS_INT32 StopTalkbackSound();
	void CreatePopToolBar();

	void SetRecordBtnState(bool state);
    // ����3D��λ��ť״̬
    void SetPtz3DBtnState(bool state);

	void SetMagnifyBtnState(bool state);
	void SetPtzBtnState(bool state);
	

private:
	//��Ƶ��������ģʽ������(��������)���������������ȣ�
	LONG m_lPalyQualityMode;
	LONG GetPalyQualityMode() const {return m_lPalyQualityMode;}
	void SetPalyQualityMode(LONG val) { m_lPalyQualityMode = val;}
	//����������ť�Ƿ���õ�״̬��¼
	BOOL m_bIsButtonEnable;
private:
    ULONGLONG  m_ullToolBarBtnVisableStatus;
public:
    ULONGLONG GetToolBarVisableStatus() const { return m_ullToolBarBtnVisableStatus; }
    void SetToolBarVisableStatus(ULONGLONG val) { m_ullToolBarBtnVisableStatus = val; }
public:
    std::bitset<TOOLBARBTNNUM> GetBitsetButtonVisableStatus() const;//lint !e1048
private:
	std::bitset<TOOLBARBTNNUM> m_bitsetButtonUsableStatus;//lint !e1048
	std::bitset<TOOLBARBTNNUM> m_bitsetButtonCompetenceStatus;//lint !e1048
public:
	void SetToolbarEnableStatus( ULONGLONG dwToolbarBtn, ULONGLONG lButtonStatus );
	
protected:
	afx_msg LRESULT OnIdcMenu(WPARAM wParam, LPARAM lParam);

	void RoutineButtonHideMenuEvent( WPARAM wParam );
	void RoutineButtonShowMenuEvent( WPARAM wParam );

	void SetCurrentPaneToActivePane();
public:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

    ULONG GetTalkbackSoundState();//lint !e1762
    BOOL IsTalkback();
    void SetSoundPic(ULONG ulState);
    void EnableALLButton(BOOL isEnable);

	//add by w00210470
	void Reset();

	void ResetBottomButton();

	HWND GetMagnifyBtnHwnd()const {return m_buttonMagnify.GetSafeHwnd(); }
	HWND GetPTZBtnHwnd()const {return m_buttonPtz.GetSafeHwnd(); }
	HWND GetTalkbackBtnHwnd()const {return m_buttonTalkback.GetSafeHwnd(); }
    HWND GetReturnBtnHwnd()const {return m_btnReturn.GetSafeHwnd(); }
	HWND GetRecordBtnHwnd()const {return m_buttonLocalRecord.GetSafeHwnd(); }
public:
    afx_msg LRESULT OnSetSoundState(WPARAM wParam, LPARAM lParam);

	void UpdateAllButtonSkin();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};//lint !e1712  MFC�Զ����ɵĵĺ���

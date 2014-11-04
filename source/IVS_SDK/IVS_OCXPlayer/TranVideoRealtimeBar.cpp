// DemoTranslucentDlg.cpp : implementation file
//
#include "stdafx.h"
#include "TranVideoRealtimeBar.h"
#include <typeinfo>
#include "IVS_OCXPlayerCtrl.h"
#include "VideoPane.h"
#include "VideoPaneMgr.h"
#include "IVS_OCXPlayerCtrl.h"
#include "LayoutMgr.h"
#include "FunctionObject.h"
#include "OCXRecordXMLProcess.h"
#include "OCXCapturePictureXMLProcess.h"
#include "ivs_xml.h "
#include "PostEventXML.h"
#include "IVS_Trace.h"
#include "GlobalPlayMgr.h"
#include "constant.h"
// CTranslucentVideoRealtimeBar dialog
//
IMPLEMENT_DYNAMIC(CTranVideoRealtimeBar, CTranslucentDialog) //lint !e18  MFC�Զ����ɵĵĺ���
CTranVideoRealtimeBar::CTranVideoRealtimeBar(Gdiplus::Image* pImage, CWnd* pParent)
	: CTranslucentDialog(CTranVideoRealtimeBar::IDD, pImage, pParent)
	,m_pMenu(nullptr)
	,m_isExtend(false)
	,m_iNumBtn(MAXNUM_SHOW_BUTTON)
	,m_pVideoSoundCtrl(nullptr)
	,m_ulSoundCtrlValue(SOUNDCTRL_SLIDER_LEN/2)
	,m_bSoundCtrlState(FALSE)
	,m_bIsInstansPlay(FALSE)
	,m_bIsStartPlay(TRUE)
	,m_bSoundState(FALSE)
	,m_bRecordStarted(FALSE)
	,m_pVideoTalkbackSound(nullptr)
	,m_ulTalkbackSoundValue(SOUNDCTRL_SLIDER_LEN/2)
	,m_bTalkbackSoundState(BOTTOMBAR_SOUND_OPEN)
	,m_bTalkbackSlienceState(FALSE)
	,m_bIsTurning(FALSE)
	,m_bIsButtonEnable(FALSE)
	,m_bTalkbackSoundCreated(false)//lint !e1729
	,m_bSoundCreated(false)
	,m_bMenuCreated(false)
	,m_lPalyQualityMode(PLAY_QUALITY_DEFINITION)
	,m_bPtz3DStarted(false)//lint !e1729
	,m_bShowTalkbackSoundCtrl(false)
	,m_ulSoundCtrlLeft(0)
	,m_ulSoundCtrlTop(0)
	,m_ullToolBarBtnVisableStatus(0)
{
	m_btnVector.clear();
	(void)m_bitsetButtonUsableStatus.set();
	(void) m_bitsetButtonCompetenceStatus.set();
}

CTranVideoRealtimeBar::~CTranVideoRealtimeBar()
{
// 	IVS_DELETE(m_pVideoSoundCtrl); //lint !e1551 IVS_DELETE �궨��
// 	IVS_DELETE(m_pMenu);           //lint !e1551 IVS_DELETE �궨��
	DeleteSoundCtrl();//lint !e1551
	DeletePopMenuBar();//lint !e1551
} //lint !e1740 m_pVideoPane ���ⲿ���� �����ͷ�

void CTranVideoRealtimeBar::DoDataExchange(CDataExchange* pDX)
{
	CTranslucentDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_TOOL_REALTIME_EXTEND,				m_buttonLocker);
	DDX_Control(pDX, IDC_BTN_TOOL_REALTIME_CAPTURE,				m_buttonSnapShot);
	DDX_Control(pDX, IDC_BTN_TOOL_REALTIME_RECORD,				m_buttonLocalRecord);
	DDX_Control(pDX, IDC_BTN_TOOL_REALTIME_LABEL,				m_buttonBookMark);
	DDX_Control(pDX, IDC_BTN_TOOL_REALTIME_MAGNIFY,				m_buttonMagnify);
	DDX_Control(pDX, IDC_BTN_TOOL_REALTIME_SOUND,				m_buttonPlaySound);
	DDX_Control(pDX, IDC_BTN_TOOL_REALTIME_TALKBACK,			m_buttonTalkback);
	DDX_Control(pDX, IDC_BTN_TOOL_REALTIME_TVWALL,				m_buttonTVWall);
	DDX_Control(pDX, IDC_BTN_TOOL_REALTIME_INSTANT_BACK_PLAY,	m_buttonInstantBackPlay);
	DDX_Control(pDX, IDC_BTN_TOOL_REALTIME_SUPER_INTEL,			m_buttonSuperIntel);
	DDX_Control(pDX, IDC_BTN_TOOL_REALTIME_E_MAP,				m_buttonEMap);
	DDX_Control(pDX, IDC_BTN_TOOL_REALTIME_SET_WARN_WND,		m_buttonSetWardWnd);
	DDX_Control(pDX, IDC_BTN_TOOL_REALTIME_MASTER_AUXILIARY,	m_buttonMasterMAuxiliary);
	DDX_Control(pDX, IDC_BTN_TOOL_REALTIME_PTZ,					m_buttonPtz);
	DDX_Control(pDX, IDC_BTN_TOOL_REALTIME_NET_QUALITY,			m_buttonNetQuality);
	DDX_Control(pDX, IDC_BTN_TOOL_REALTIME_PTZ3D,               m_BtnPtz3D);
	DDX_Control(pDX, IDC_BTN_TOOL_REALTIME_RETURN,              m_btnReturn);
	DDX_Control(pDX, IDC_BTN_TOOL_REALTIME_START,               m_btnStart);
}

BEGIN_MESSAGE_MAP(CTranVideoRealtimeBar, CTranslucentDialog)
    ON_WM_PAINT()
    ON_WM_CREATE()
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BTN_TOOL_REALTIME_CAPTURE, &CTranVideoRealtimeBar::OnBnClickedButtonRealtimeCapture)
    ON_BN_CLICKED(IDC_BTN_TOOL_REALTIME_RECORD, &CTranVideoRealtimeBar::OnBnClickedButtonRealtimeRecord)
    ON_BN_CLICKED(IDC_BTN_TOOL_REALTIME_LABEL, &CTranVideoRealtimeBar::OnBnClickedButtonRealtimeLabel)
    ON_BN_CLICKED(IDC_BTN_TOOL_REALTIME_MAGNIFY, &CTranVideoRealtimeBar::OnBnClickedButtonRealtimeMagnify)
    ON_BN_CLICKED(IDC_BTN_TOOL_REALTIME_SOUND, &CTranVideoRealtimeBar::OnBnClickedButtonRealtimeSound)
    ON_BN_CLICKED(IDC_BTN_TOOL_REALTIME_TALKBACK, &CTranVideoRealtimeBar::OnBnClickedButtonRealtimeTalkback)
    ON_BN_CLICKED(IDC_BTN_TOOL_REALTIME_EXTEND, &CTranVideoRealtimeBar::OnBnClickedButtonRealtimeExtent)
	ON_BN_CLICKED(IDC_BTN_TOOL_REALTIME_NET_QUALITY, &CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimeNetQuality)
	ON_BN_CLICKED(IDC_BTN_TOOL_REALTIME_MASTER_AUXILIARY, &CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimeMasterAuxiliary)
	ON_BN_CLICKED(IDC_BTN_TOOL_REALTIME_INSTANT_BACK_PLAY, &CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimeInstantBackPlay)
	ON_BN_CLICKED(IDC_BTN_TOOL_REALTIME_TVWALL, &CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimeTvwall)
	ON_BN_CLICKED(IDC_BTN_TOOL_REALTIME_SET_WARN_WND, &CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimeSetWarnWnd)
	ON_BN_CLICKED(IDC_BTN_TOOL_REALTIME_PTZ, &CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimePtz)
	ON_BN_CLICKED(IDC_BTN_TOOL_REALTIME_SUPER_INTEL, &CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimeSuperIntel)
	ON_BN_CLICKED(IDC_BTN_TOOL_REALTIME_E_MAP, &CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimeEMap)
	ON_BN_CLICKED(IDC_BTN_TOOL_REALTIME_PTZ3D, &CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimePtz3d)
	ON_BN_CLICKED(IDC_BTN_TOOL_REALTIME_RETURN, &CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimeReturn)
	ON_BN_CLICKED(IDC_BTN_TOOL_REALTIME_START, &CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimeStart)
	ON_MESSAGE(IDC_MENU, &CTranVideoRealtimeBar::OnIdcMenu)
    ON_MESSAGE(WM_SETSOUND_STATE, &CTranVideoRealtimeBar::OnSetSoundState)
	//ON_COMMAND_RANGE()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

// Qualifier: ��������PNG�ؼ��� PNG ͼƬ��Դ
void CTranVideoRealtimeBar::OnInitChildrenWnds()
{
	//��չ
	SetButtonPicture(m_buttonLocker,      _T("Extend"));
	//ץ��
	SetButtonPicture(m_buttonSnapShot,	  _T("SnapShot"));
	//����¼��
	SetButtonPicture(m_buttonLocalRecord, _T("LocalRecord"));
	//��ǩ
	SetButtonPicture(m_buttonBookMark,	  _T("BookMark"));
	//�ֲ��Ŵ�
	SetButtonPicture(m_buttonMagnify,	  _T("Zoom"));
	//��ʱ�ط�
	SetButtonPicture(m_buttonInstantBackPlay,   _T("PlayRecord"));
	//����
	SetButtonPicture(m_buttonPlaySound,    _T("PlayNoSound"));
	//�Խ�
	SetButtonPicture(m_buttonTalkback,	   _T("TalkBack"));
	//������ǽ
	SetButtonPicture(m_buttonTVWall,		_T("VideoTVW"));
	//���ø澯����
	SetButtonPicture(m_buttonSetWardWnd,	_T("AlarmWin"));
	//�ƾ�����
	SetButtonPicture(m_buttonPtz,			_T("PTZ"));
	//�������ܷ���
	SetButtonPicture(m_buttonSuperIntel,	_T("IA"));
	//�򿪵��ӵ�ͼ
	SetButtonPicture(m_buttonEMap,			_T("OpenMap"));
	//һ���ศ
	SetButtonPicture(m_buttonMasterMAuxiliary,	_T("WindowMain"));
	//��������/��������
	SetButtonPicture(m_buttonNetQuality,    _T("PlayQuality"));
    //3D��λ
    SetButtonPicture(m_BtnPtz3D,            _T("PTZ3D"));
	//����
	SetButtonPicture(m_btnReturn,      _T("ReturnBack"));
	//��ʼ����
	SetButtonPicture(m_btnStart,	  _T("StartBack"));

	UpdateView();
}

void CTranVideoRealtimeBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	DrawButton(m_iNumBtn);
	UpdateView();
}

int CTranVideoRealtimeBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CTranslucentDialog::OnCreate(lpCreateStruct) == -1)
        return -1;

    return 0;
}

HBRUSH CTranVideoRealtimeBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CTranslucentDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    return hbr;
}

BOOL CTranVideoRealtimeBar::OnInitDialog()
{
    CTranslucentDialog::OnInitDialog();
    m_btnVector.push_back(&m_buttonSnapShot);
    m_btnVector.push_back(&m_buttonLocalRecord);
    m_btnVector.push_back(&m_buttonBookMark);
    m_btnVector.push_back(&m_buttonMagnify);
	m_btnVector.push_back(&m_buttonInstantBackPlay);
    m_btnVector.push_back(&m_buttonPlaySound);
    m_btnVector.push_back(&m_buttonTalkback);
    m_btnVector.push_back(&m_buttonTVWall);
	m_btnVector.push_back(&m_buttonSetWardWnd);
	m_btnVector.push_back(&m_buttonPtz);
	m_btnVector.push_back(&m_buttonSuperIntel);
	m_btnVector.push_back(&m_buttonEMap);
	m_btnVector.push_back(&m_buttonMasterMAuxiliary);
	m_btnVector.push_back(&m_buttonNetQuality);
    m_btnVector.push_back(&m_BtnPtz3D);

    EndBuffered();
    return TRUE;  // return TRUE unless you set the focus to a control
}

void CTranVideoRealtimeBar::CreateSoundCtrl()
{
    if (nullptr == m_pVideoSoundCtrl)
    {
        m_pVideoSoundCtrl = IVS_NEW((CVideoSoundCtrl*&)m_pVideoSoundCtrl);
        CHECK_POINTER_VOID(m_pVideoSoundCtrl);
        m_pVideoSoundCtrl->SetSoundCtrlType(SOUNDCTRL_WND_TYPE_SOUND);
        (void)m_pVideoSoundCtrl->Create(IDD_DLG_SOUNDCTRL);
		m_bSoundCreated = true;
    }
    m_bSoundCtrlState = TRUE;
}

void CTranVideoRealtimeBar::CreateTalkbackCtrl()
{
	if (nullptr == m_pVideoTalkbackSound)
	{
		m_pVideoTalkbackSound = IVS_NEW((CVideoSoundCtrl*&)m_pVideoTalkbackSound);
		CHECK_POINTER_VOID(m_pVideoTalkbackSound);
		m_pVideoTalkbackSound->SetSoundCtrlType(SOUNDCTRL_WND_TYPE_TALKBACK);
		(void)m_pVideoTalkbackSound->Create(IDD_DLG_SOUNDCTRL);
		m_bTalkbackSoundCreated = true;
	}
	m_bSoundCtrlState = TRUE;
}

void CTranVideoRealtimeBar::DeleteSoundCtrl()
{
	if (m_bSoundCreated)
	{
		m_bSoundCreated = false;
		IVS_DELETE(m_pVideoSoundCtrl);
	}
	m_bSoundCtrlState = FALSE;
}

void CTranVideoRealtimeBar::DeletePopMenuBar()
{
	if (m_bMenuCreated)
	{
		m_bMenuCreated = false;
		IVS_DELETE(m_pMenu);
	}
}

void CTranVideoRealtimeBar::OnBnClickedButtonRealtimeCapture()
{
	IVS_INFO_TRACE("");
	SetCurrentPaneToActivePane();

    CVideoPane *pVideoPane = dynamic_cast<CVideoPane *>(m_pVideoPane);
    CHECK_POINTER_VOID(pVideoPane);

    CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
    CHECK_POINTER_VOID(pCtrl);
// 	pCtrl->PostToolBarClickEventMsg(2,IVS_TOOLBAR_SNAPSHOT, pVideoPane->GetPaneID());

	(void)pCtrl->IVS_OCX_LocalSnapshot((LONG)pVideoPane->GetPaneID());
}

void CTranVideoRealtimeBar::OnBnClickedButtonRealtimeRecord()
{
    SetCurrentPaneToActivePane();
    CVideoPane *pVideoPane = dynamic_cast<CVideoPane *>(m_pVideoPane);
    CHECK_POINTER_VOID(pVideoPane);

    CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
    CHECK_POINTER_VOID(pCtrl);
    pCtrl->PostToolBarClickEventMsg(2,IVS_TOOLBAR_LOCAL_RECORD, pVideoPane->GetPaneID());

    if (!pVideoPane->GetIsLocalRecord())
    {
		unsigned long result = pCtrl->IVS_OCX_StartLocalRecord(NULL, (LONG)pVideoPane->GetPaneID());
        if (IVS_SUCCEED == result)
        {
            pCtrl->PostLocRecEventGeneric(5, (int)pVideoPane->GetPaneID()); // 5 Ϊ¼��ʼ
        }
		else if (IVS_PLAYER_RET_DISK_SPACE_ERROR == result)
		{
	        pCtrl->PostLocRecEventGeneric(3, (int)pVideoPane->GetPaneID()); // 3 :���̿ռ���
		}
		else
		{
			pCtrl->PostLocRecEventGeneric(0, (int)pVideoPane->GetPaneID()); // 0 Ϊͨ�ô���
		}
    } 
    else 
    {
        (void)pCtrl->IVS_OCX_StopLocalRecord((LONG)pVideoPane->GetPaneID());
        pCtrl->PostLocRecEventGeneric(1, (int)pVideoPane->GetPaneID());     // 1 Ϊ¼��ֹͣ
    }
}

void CTranVideoRealtimeBar::OnBnClickedButtonRealtimeLabel()
{
	SetCurrentPaneToActivePane();

	CVideoPane *pVideoPane = dynamic_cast<CVideoPane *>(m_pVideoPane);
	CHECK_POINTER_VOID(pVideoPane);

    //֪ͨCU������������OCX�Ľӿ�
    CVideoPaneMgr* pVideoPaneMgr = pVideoPane->GetVideoPaneMgr();
    CHECK_POINTER_VOID(pVideoPaneMgr);

    //û����ʵ�����¼�������
    if (VIDEO_PLAY_STATE_NOT_PLAYING == pVideoPane->GetPlayState())
    {
        return;
    }

    // ɾ���������ƶԻ���
    pVideoPaneMgr->DeleteSoundCtrl();
    pVideoPane->GetOCXPlayerCtrl()->PostToolBarClickEventMsg(2,IVS_TOOLBAR_BOOKMARK, pVideoPane->GetPaneID());
}

// �����ֲ��Ŵ�
void CTranVideoRealtimeBar::OnBnClickedButtonRealtimeMagnify()
{
	SetCurrentPaneToActivePane();
	CVideoPane *pVideoPane = dynamic_cast<CVideoPane *>(m_pVideoPane);
	CHECK_POINTER_VOID(pVideoPane);

	CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
	CHECK_POINTER_VOID(pCtrl);

	// �Ѿ��Ǿֲ��Ŵ��ˣ��رվֲ��Ŵ�
	if (MAGNIFY_OPEN == pVideoPane->GetRealtimeMagnifyState())
	{
		std::string strButStatus = CToolsHelp::Int2Str(MAGNIFY_CLOSE);
		pCtrl->PostToolBarClickEventMsg(2,IVS_TOOLBAR_ZOOM, pVideoPane->GetPaneID(), strButStatus);//lint !e1036

		// ɾ��Դ���ڵ����в�ֹͣ�������ڵķŴ�Ч��
		::SendMessage(pVideoPane->GetSafeHwnd(), WM_MAGNIFY_CLOSE_ALL, 0, 0);
		(void)pVideoPane->SetRealtimeMagnifyState(MAGNIFY_CLOSE);
		SetMagnifyBtnState(false);
		// �ر����ܷ�����ͼ
		(void)pCtrl->IVS_OCX_DisableDraw(pVideoPane->GetPaneID());
		// �ָ����д��ڵľֲ��Ŵ�ť
		pCtrl->GetVideoPaneMgr()->SetALLMagnifyBtnStatus(pVideoPane, 1);//lint !e747
		return;
	}
	// TODO ��������ҲӦ������������ֹ���������¼��쳣
	std::string strButStatus = CToolsHelp::Int2Str(MAGNIFY_OPEN);
	pCtrl->PostToolBarClickEventMsg(2,IVS_TOOLBAR_ZOOM, pVideoPane->GetPaneID(), strButStatus);//lint !e1036

	// �Ѿ���PTZ,��Ҫ�ر���̨
	if (BOTTOMBAR_PTZ_OPEN == pVideoPane->GetRealtimePtzState())
	{
		// ɾ���ƾ�����
		pVideoPane->DeleteDigtalPtz();
		pVideoPane->SetRealtimePtzState(BOTTOMBAR_PTZ_CLOSE);
		SetPtzBtnState(false);
	}

	//û����ʵ�����¼�������
	if (VIDEO_PLAY_STATE_NOT_PLAYING == pVideoPane->GetPlayState() || VIDEO_PLAY_STATE_RESTARTING == pVideoPane->GetPlayState())
	{
		return;
	}
	// ���������⡣�����������ڵľֲ��Ŵ�ť
	if (NULL != pCtrl->GetVideoPaneMgr())
	{
		pCtrl->GetVideoPaneMgr()->SetALLMagnifyBtnStatus(pVideoPane,0);//lint !e747
	}

	// ������ͼ
	(void)pCtrl->IVS_OCX_EnableDraw(pVideoPane->GetPaneID());
	// ���þֲ��Ŵ�״̬
	(void)pVideoPane->SetRealtimeMagnifyState(MAGNIFY_OPEN);
	pCtrl->GetVideoPaneMgr()->SetMagnifyLayOut(pCtrl->GetVideoPaneMgr()->GetWindLayOut());
	// ���ð�ť����״̬
	SetMagnifyBtnState(true);
}

// ����ʵ����������������ť
void CTranVideoRealtimeBar::OnBnClickedButtonRealtimeSound()
{
    SetCurrentPaneToActivePane();
    CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
    CHECK_POINTER_VOID(pVideoPane);
    CVideoPaneMgr * pVideoPaneMgr = pVideoPane->GetVideoPaneMgr();
    CHECK_POINTER_VOID(pVideoPaneMgr);
    // ɾ������Ӧ��С�Ի���

	CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
	CHECK_POINTER_VOID(pCtrl);
	pCtrl->PostToolBarClickEventMsg(2,IVS_TOOLBAR_PLAY_SOUND, pVideoPane->GetPaneID());

    if (!m_bitsetButtonUsableStatus.test(6))
    {
        return;
    }
    if (!m_bitsetButtonCompetenceStatus.test(6))
    {
        return;
    }

    // ɾ���������ƶԻ���
    pVideoPaneMgr->DeleteSoundCtrlForOther();
    // ɾ�����������Խ��Ի���
    if (m_bSoundCtrlState)
    {
        // ɾ�����������Ի���
        DeleteSoundCtrl();
    } else {
        // ����������Խ�,��������
        if(IsTalkback())
        {
            // modi by wKF71044 at [2013-6-4 16:30:52] DTS2013052309584
            // �������ⵥҪ����Ҫ�ϱ���CU
            pCtrl->PostSoundEvent();
            return;
        }
        DeleteTalkbackCtrl();
        CreateSoundCtrl();

        if (NULL != m_pVideoSoundCtrl)
        {
            CRect rect;
            m_buttonPlaySound.GetWindowRect(&rect);

            rect.left   -= static_cast<long>((SOUNDCTRL_WIDTH - static_cast<ULONG>(rect.Width()))/2);
            rect.right  = rect.left + static_cast<long>(SOUNDCTRL_WIDTH);
            rect.bottom -= 20;
            rect.top   = rect.bottom - static_cast<long>(SOUNDCTRL_SLIDER_HEIGHT);
            m_ulSoundCtrlLeft = static_cast<ULONG>(rect.left);
            m_ulSoundCtrlTop  = static_cast<ULONG>(rect.top);
            m_pVideoSoundCtrl->SetVideopBar(this);
            m_pVideoSoundCtrl->MoveWindow(rect);
            m_pVideoSoundCtrl->ShowWindow(SW_SHOW);
            m_bSoundCtrlState = TRUE;
        }
        //��ǰ�Ǿ���״̬��ʾδ��������������һ�δ洢��ֵ�������ȡϵͳ��ǰ����
//         if(!m_bSoundState)
//         {
//             DWORD ulVolume = 0;
//             (void)waveOutGetVolume(0, &ulVolume);
//             ulVolume = ulVolume / (VOLUME_FULL / 100);
//             m_ulSoundCtrlValue = ulVolume;
//         }
        InitSoundCtrl(SOUNDCTRL_WND_TYPE_SOUND);
		if (NULL != m_pVideoSoundCtrl)
		{
			m_pVideoSoundCtrl->Invalidate(TRUE);
		}
    }
}

void CTranVideoRealtimeBar::OnBnClickedButtonRealtimeTalkback()
{
	IVS_INT32 iRet = IVS_FAIL;
	std::string strResult;
	SetCurrentPaneToActivePane();
    CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
    CHECK_POINTER_VOID(pVideoPane);
    CVideoPaneMgr * pVideoPaneMgr = pVideoPane->GetVideoPaneMgr();
    CHECK_POINTER_VOID(pVideoPaneMgr);

	CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
	CHECK_POINTER_VOID(pCtrl);
	
	// ɾ���������ƶԻ���
	pVideoPaneMgr->DeleteSoundCtrlForOther();

	// �Ѿ������˶Խ����رնԽ�
	if (BOTTOMBAR_TALKBACK_OPEN == pVideoPane->GetRealtimeTalkbackState())
	{
		iRet = StopTalkbackSound();
		
		if (IVS_SUCCEED != iRet)
		{
			strResult = "1#" + CToolsHelp::Int2Str(iRet);
			// ���׹رյ�״̬
			pCtrl->PostToolBarClickEventMsg(2,IVS_TOOLBAR_TALKBACK, pVideoPane->GetPaneID(), strResult.c_str());
			return;
		}
		// ɾ�������Ի���
		DeleteTalkbackCtrl();
		strResult = "0#" + CToolsHelp::Int2Str(iRet);
		// ���׹رյ�״̬
		pCtrl->PostToolBarClickEventMsg(2,IVS_TOOLBAR_TALKBACK, pVideoPane->GetPaneID(), strResult.c_str());
		return;
	}

    // ����������Խ�
    if("" != CIVS_OCXPlayerCtrl::m_strVoiceTalkbackCameraCode)
	{
		// modi by wKF71044 at [2013-6-4 16:30:52] DTS2013052308679 
		// �������ⵥҪ����Ҫ�ϱ���CU
		pCtrl->PostTalkbackEvent();
		return;
	}
    DeleteSoundCtrl();
	CreateTalkbackCtrl();
	if (NULL != m_pVideoTalkbackSound)
	{
		m_pVideoTalkbackSound->ShowWindow(SW_HIDE);
	}
	iRet = StartTalkbackSound();
	if (IVS_SUCCEED != iRet)
	{
		// ���״�������ʾ
		strResult = "1#" + CToolsHelp::Int2Str(iRet);
		pCtrl->PostToolBarClickEventMsg(2,IVS_TOOLBAR_TALKBACK, pVideoPane->GetPaneID(),strResult.c_str());
		return;
	}

    //  modify by wanglei 00165153:2013.6.10 DTS2013060606556 
    // ���������Խ�����Ҫ�ر���һ��·����
	CGlobalSoundMgr &SoundMgr = CGlobalSoundMgr::Instance();
	SoundMgr.SetSoundingPanel(NULL);

	strResult = "0#" + CToolsHelp::Int2Str((int)pCtrl->m_ulVoiceTalkbackHandle);//lint !e1705
	// ���׿���״̬��ֱ�ӰѾ��Ҳ����ȥ
	pCtrl->PostToolBarClickEventMsg(2,IVS_TOOLBAR_TALKBACK, pVideoPane->GetPaneID(),strResult.c_str());

	if (NULL != m_pVideoTalkbackSound && m_bShowTalkbackSoundCtrl)
	{
		m_pVideoTalkbackSound->ShowWindow(SW_SHOW);
		CRect rect;
		m_buttonTalkback.GetWindowRect(&rect);
		CRect rect1;
		this->GetClientRect(&rect1);
		rect.left -= static_cast<long>((SOUNDCTRL_WIDTH - static_cast<ULONG>(rect.Width()))/2);
		rect.right = rect.left + static_cast<long>(SOUNDCTRL_WIDTH);
		rect.bottom -= 20;
		rect.top   = rect.bottom - static_cast<long>(SOUNDCTRL_SLIDER_HEIGHT);
		m_ulSoundCtrlLeft = static_cast<ULONG>(rect.left);
		m_ulSoundCtrlTop  = static_cast<ULONG>(rect.top);
		m_pVideoTalkbackSound->SetVideopBar(this);
		m_pVideoTalkbackSound->MoveWindow(rect);
		m_pVideoTalkbackSound->ShowWindow(SW_SHOW);
		m_bTalkbackSoundState = TRUE;
	}
	if (NULL != m_pVideoTalkbackSound)
	{
		m_pVideoTalkbackSound->SetTalkbackSilencePic(TRUE);

		m_pVideoTalkbackSound->Invalidate(TRUE);
		// ����ڹر���������£��رնԽ������¿��������,sleepʱ����̻ᵼ���޷���������
 		Sleep(500);
		m_pVideoTalkbackSound->PostMessage(WM_BITMAPSLIDER_MOVED,IDC_SLIDER_SOUNDCTRL,0);
	}
}

void CTranVideoRealtimeBar::DrawButton(IVS_INT32 iBtnNum)
{
    CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
    CHECK_POINTER_VOID(pVideoPane);
    CVideoPaneMgr * m_pVideoPaneMgr = pVideoPane->GetVideoPaneMgr();
    CHECK_POINTER_VOID(m_pVideoPaneMgr);

    CRect rect;
    m_pVideoPane->GetClientRect(&rect);
	//GetClientRect(rect);

	IVS_INT32 iButtonOffSetNUm = 1;	//��1�Ǹ������������ؼ���λ��
	IVS_INT32 iButtonMaxShowNum = MAXNUM_SHOW_BUTTON;//7��ʾʵ�����湤������ʼ��ʾ7����ť
	int nBtnLeftIndex = rect.left + 1;
    const IVS_INT32 nAllowBtnNum = rect.Width() / REALTIMEBAR_BUTTON_WITDH - iButtonOffSetNUm;

	IVS_INT32 _iBtnNum = 0;
	if (nAllowBtnNum >= iBtnNum )
	{
		_iBtnNum = iBtnNum;
	}else if (nAllowBtnNum > iButtonMaxShowNum) {
		_iBtnNum = iButtonMaxShowNum;
	}else{
		_iBtnNum = nAllowBtnNum;
	}

	FunctionObjectDrawButtonPosition Menu =
		for_each(m_btnVector.begin(),m_btnVector.end(),
		FunctionObjectDrawButtonPosition(this,_iBtnNum,nBtnLeftIndex));

	iBtnNum = static_cast<int >(GetBitsetButtonVisableStatus().count());
	if(m_pVideoPaneMgr->GetCurTVWallID() == 0)//����ǵ���ǽ�Ĺ�������ʾ
	{
		if (iBtnNum < MAXNUM_SHOW_BUTTON &&iBtnNum*16<rect.Width())
		{
			m_buttonLocker.ShowWindow(SW_HIDE);
		}
		else
		{
			m_buttonLocker.ShowWindow(SW_SHOW);
			m_buttonLocker.MoveWindow(Menu.value(), 2, 16, 16);
		}
	}
	else//�������ǽ�Ĺ�������ʾ
	{
		IVS_INT32 iMaxShowButtonNum = 4;//ʵ���µĵ���ǽ��ʾ4����ť
		if(pVideoPane->GetToolBarBtnVisableStatus() == 7)//¼������ĵ���ǽ��ʾ3����ť
		{
			iMaxShowButtonNum = 3;
		}
		if (nAllowBtnNum >= iMaxShowButtonNum)
		{
			m_buttonLocker.ShowWindow(SW_HIDE);
		}
		else
		{
			m_buttonLocker.ShowWindow(SW_SHOW);
			m_buttonLocker.MoveWindow(Menu.value(), 2, 16, 16);
		}
	}

	if (m_bIsInstansPlay)
	{
		//��ʱ�طŰ�ť���⴦��
		m_btnReturn.ShowWindow(SW_SHOW);
		m_btnReturn.MoveWindow(0, 2, 16, 16);
		m_btnStart.ShowWindow(SW_SHOW);
		m_btnStart.MoveWindow(REALTIMEBAR_BUTTON_WITDH, 2, 16, 16);
	}
	else
	{
		m_btnReturn.ShowWindow(SW_HIDE);
		m_btnStart.ShowWindow(SW_HIDE);
	}
	// չ����ʱ��һ�¶Խ��Ƿ���������չ����ʱ������ε�����
	if (BOTTOMBAR_TALKBACK_OPEN == pVideoPane->GetRealtimeTalkbackState())
	{
		m_buttonPlaySound.EnableWindow(FALSE);
	}

	if (FULL_SCREEN == m_pVideoPaneMgr->GetFullScreenState())
	{
		(void)::SetWindowLong(this->GetSafeHwnd(),GWL_HWNDPARENT,(LONG)(0));
	}
	else
	{
		(void)::SetWindowLong(this->GetSafeHwnd(),GWL_HWNDPARENT,m_pVideoPaneMgr->GetParentHwnd());//lint !e713
	}
}//lint !e550 !e529

ULONG CTranVideoRealtimeBar::GetClientMode() const
{
	CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
    CHECK_POINTER(pVideoPane,CLIENT_PC);
    return (ULONG)pVideoPane->GetClientType();
}

void CTranVideoRealtimeBar::OnBnClickedButtonRealtimeExtent()
{
	SetCurrentPaneToActivePane();
	CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
	CHECK_POINTER_VOID(pVideoPane);
	CVideoPaneMgr * pVideoPaneMgr = pVideoPane->GetVideoPaneMgr();
	CHECK_POINTER_VOID(pVideoPaneMgr);
	// ɾ���������ƶԻ���
	pVideoPaneMgr->DeleteSoundCtrlForOther();
	if (m_isExtend)
	{
		m_iNumBtn = MAXNUM_SHOW_BUTTON;
		DrawButton(m_iNumBtn);
		m_isExtend = false;
		//��չ
		UnregisterTranslucentWnd(&m_buttonLocker);
		SetButtonPicture(m_buttonLocker, "Extend");
		UpdateView();
	} else{
		m_iNumBtn = static_cast<int >(GetBitsetButtonVisableStatus().count());
		DrawButton(m_iNumBtn);
		CreatePopToolBar();
		UpdateView();
	}
}

void CTranVideoRealtimeBar::CreatePopToolBar()
{
	//IVS_DELETE(m_pMenu);
	// ֻ�д��������ʱ������һ�£���ֹ����ı䵯��������
	CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
	CHECK_POINTER_VOID(pVideoPane);
	CVideoPaneMgr * pVideoPaneMgr = pVideoPane->GetVideoPaneMgr();
	CHECK_POINTER_VOID(pVideoPaneMgr);
	CUtility::SetVideoPaneMgr(pVideoPaneMgr);

	DeletePopMenuBar();
	m_pMenu = new CIVSMenu();

	m_pMenu->ClearButtonData();
	(void)m_pMenu->AppendMenuItem("",ID_MENU_HEADER,0,0,0,NULL, MENU_NORMAL|MENU_BORDLINE,"");

	(void)std::for_each(m_btnVector.begin(),m_btnVector.end(),FunctionObjectToolBarAppendItem(this));

	CPoint pt;
	GetCursorPos(&pt);

	(void)m_pMenu->AppendMenuItem("",ID_MENU_TRAIL , 0,0,0, NULL, MENU_NORMAL|MENU_BORDLINE,"");
	(void)m_pMenu->CreateGroupMenu(this,CRect(0,0,173,290),IDC_MENU,0,1);

	if (m_pMenu->GetItemNum() > 2)
	{
		m_pMenu->ShowMenuPos(pt.x,pt.y,LEFTBOTTOM);
		m_isExtend = false;
		//��չ
		UnregisterTranslucentWnd(&m_buttonLocker);
		SetButtonPicture(m_buttonLocker, "Extend");
	}
	else{
		m_isExtend = true;
		//��չ
		UnregisterTranslucentWnd(&m_buttonLocker);
		SetButtonPicture(m_buttonLocker, "Retracted");
	}

	//�Բ��õĲ˵������ûҲ���
	std::vector<CTranslucentButton *>::const_iterator it;
	for (it = m_btnVector.begin(); it != m_btnVector.end(); ++it)
	{
		CTranslucentWnd* translucentWnd = *it;
		m_pMenu->EnableMenuStatus((unsigned int)translucentWnd->GetDlgCtrlID(),m_bIsButtonEnable);
	}
	//�澯,һ���ศ����ͼ
	m_pMenu->EnableMenuStatus(IDC_BTN_TOOL_REALTIME_SET_WARN_WND,TRUE);
	m_pMenu->EnableMenuStatus(IDC_BTN_TOOL_REALTIME_MASTER_AUXILIARY,TRUE);
	m_pMenu->EnableMenuStatus(IDC_BTN_TOOL_REALTIME_E_MAP,TRUE);
	m_bMenuCreated = true;
} //lint !e550

void CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimeNetQuality()
{
	SetCurrentPaneToActivePane();
	//������Ƶ�����ȡ������Ȱ�ť
	CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
	CHECK_POINTER_VOID(pVideoPane);

	CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
	CHECK_POINTER_VOID(pCtrl);
	pCtrl->PostToolBarClickEventMsg(2,IVS_TOOLBAR_PLAY_QUALITY, pVideoPane->GetPaneID());

	//û����ʵ�����¼�������
	if (VIDEO_PLAY_STATE_NOT_PLAYING == pVideoPane->GetPlayState())
	{
		return;
	}
	ULONG ulWinID = pVideoPane->GetPaneID();
	LONG  lPalyQualityMode = PLAY_QUALITY_FLUENCY; //Ĭ����������;
	//�����ǰģʽΪ��������
	if (PLAY_QUALITY_FLUENCY == m_lPalyQualityMode)
	{
		SetButtonPicture(m_buttonNetQuality, "PlayQuality"); //���ð�ťΪ��������
		lPalyQualityMode = m_lPalyQualityMode;
		m_lPalyQualityMode = PLAY_QUALITY_DEFINITION;    //�޸ĵ�ǰ����Ϊ��������
	}
	else
	{
		SetButtonPicture(m_buttonNetQuality, "PlayNet");// ��������
		lPalyQualityMode = m_lPalyQualityMode;
		m_lPalyQualityMode = PLAY_QUALITY_FLUENCY;       //�޸ĵ�ǰ����Ϊ��������
	}
	if (NULL != pVideoPane->GetOCXPlayerCtrl())
	{
		pVideoPane->GetOCXPlayerCtrl()->IVS_OCX_SetPlayQualityMode(ulWinID,lPalyQualityMode);
	}
}

void CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimeMasterAuxiliary()
{
    SetCurrentPaneToActivePane();
    CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
    CHECK_POINTER_VOID(pVideoPane);
    CVideoPaneMgr * pVideoPaneMgr = pVideoPane->GetVideoPaneMgr();
    CHECK_POINTER_VOID(pVideoPaneMgr);

	CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
	CHECK_POINTER_VOID(pCtrl);
	pCtrl->PostToolBarClickEventMsg(2,IVS_TOOLBAR_WINDOW_MAIN, pVideoPane->GetPaneID());

	ULONG _ulPaneNUm = pVideoPaneMgr->GetNumOfDisplayPane();

	if (_ulPaneNUm < LADDERLAYOUTMINWINDOWSNUM || _ulPaneNUm > LADDERLAYOUTMAXWINDOWSNUM){
		return;
	}
	CRect _Rect;
    pCtrl->GetClientRect(&_Rect);
	ULONG ulLadderLayoutWinNum = 0;

	if (LayoutMgr.isLadderLayout_6(_ulPaneNUm)){
		ulLadderLayoutWinNum = 6;
		(void)pVideoPaneMgr->SetWndLayout(_Rect,WIND_LAYOUT_6_3);
	}else if (LayoutMgr.isLadderLayout_8(_ulPaneNUm))
	{
		ulLadderLayoutWinNum = 8;
		(void)pVideoPaneMgr->SetWndLayout(_Rect,WIND_LAYOUT_8_1);
	}else if (LayoutMgr.isLadderLayout_10(_ulPaneNUm))
	{
		ulLadderLayoutWinNum = 10;
		(void)pVideoPaneMgr->SetWndLayout(_Rect,WIND_LAYOUT_10_2);
	}else if (LayoutMgr.isLadderLayout_12(_ulPaneNUm))
	{
		ulLadderLayoutWinNum = 12;
		(void)pVideoPaneMgr->SetWndLayout(_Rect,WIND_LAYOUT_12_2);
	}else if (LayoutMgr.isLadderLayout_16(_ulPaneNUm))
	{
		ulLadderLayoutWinNum = 16;
		(void)pVideoPaneMgr->SetWndLayout(_Rect,WIND_LAYOUT_16_2);
	} else if (LayoutMgr.isLadderLayout_20(_ulPaneNUm))
	{
		ulLadderLayoutWinNum = 20;
		(void)pVideoPaneMgr->SetWndLayout(_Rect,WIND_LAYOUT_20_1);
	}
	std::string strPostXML;
	PostEventXMLImpl.GetLadderLayoutEventMsg(strPostXML,ulLadderLayoutWinNum);

	pCtrl->IVS_OCX_Windows_Event(EVENT_LADDER_LAYOUT_WINDOW,strPostXML.c_str());
	// ����������񵽵�һ���������Ĵ���
	pVideoPaneMgr->ExchangePane(1);
}

void CTranVideoRealtimeBar::SetToolbarEnableStatus( ULONGLONG dwToolbarBtn, ULONGLONG lButtonStatus )
{
	std::bitset<TOOLBARBTNNUM> bitsetButtonStatus(dwToolbarBtn);
	IVS_UINT32 _iCount = 0;

	size_t s = m_btnVector.size();

	for (_iCount = 0; _iCount < TOOLBARBTNNUM && _iCount < s; _iCount++)
	{
		if (bitsetButtonStatus.test(_iCount))
		{
			m_btnVector[_iCount]->EnableWindow(static_cast<BOOL>(lButtonStatus));
			m_btnVector[_iCount]->Invalidate(TRUE);
		}
	}
}

void CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimeInstantBackPlay()
{
	SetCurrentPaneToActivePane();
	CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
	CHECK_POINTER_VOID(pVideoPane);

	// ����ֲ��Ŵ����ˣ���Ҫ�ر�
	if (MAGNIFY_OPEN == pVideoPane->GetRealtimeMagnifyState())
	{
		// ɾ��Դ���ڵ����в�ֹͣ�������ڵķŴ�Ч��
		::PostMessage(pVideoPane->GetSafeHwnd(), WM_MAGNIFY_CLOSE_ALL, 0, 0);
		(void)pVideoPane->SetRealtimeMagnifyState(MAGNIFY_CLOSE);
		SetMagnifyBtnState(false);
		CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
		if (NULL != pCtrl)
		{
			// �ر����ܷ�����ͼ
			(void)pCtrl->IVS_OCX_DisableDraw(pVideoPane->GetPaneID());
		}
	}

	// ����ƾ������ˣ���Ҫ�ر�
	if (BOTTOMBAR_PTZ_OPEN == pVideoPane->GetRealtimePtzState())
	{
		// ����һ������¼����Ŵ�ť�������ر��Լ�������ʹ��ͬ����sendmessage,��Ҫʹ��postmessage
		::SendMessage(this->GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(IDC_BTN_TOOL_REALTIME_PTZ, BN_CLICKED), (LPARAM)this->GetPTZBtnHwnd());
	}

	// ��ѵ��ť
	if (TYPE_WND_TURNCTUISE == pVideoPane->GetTypeWnd())
	{
		pVideoPane->SetPrveTurnCruiseStyle(pVideoPane->GetTurnCruiseStyle());
		pVideoPane->SetTurnCruiseStyle(0);
		pVideoPane->SetPrveTypeWnd(pVideoPane->GetTypeWnd());
	}

	// �Ѿ������˶Խ����رնԽ�
	if (BOTTOMBAR_TALKBACK_OPEN == pVideoPane->GetRealtimeTalkbackState())
	{
		(void)StopTalkbackSound();
		DeleteTalkbackCtrl();
		// ���׹رյ�״̬
		CIVS_OCXPlayerCtrl *pOcx = pVideoPane->GetOCXPlayerCtrl();
		if (pOcx)
		{
			std::string strResult = "0#" + CToolsHelp::Int2Str(IVS_SUCCEED);
			pOcx->PostToolBarClickEventMsg(2,IVS_TOOLBAR_TALKBACK, pVideoPane->GetPaneID(), strResult.c_str());
		}
	}
	 
	//������ʱ�طű��
	pVideoPane->ShowInstantReplayBar(1);
	CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
	CHECK_POINTER_VOID(pCtrl);
	pCtrl->PostToolBarClickEventMsg(2,IVS_TOOLBAR_PLAY_RECORD, pVideoPane->GetPaneID());
	pVideoPane->ShowInstantBar();
	pVideoPane->Invalidate();
}

void CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimeTvwall()
{
	SetCurrentPaneToActivePane();
	CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
	CHECK_POINTER_VOID(pVideoPane);
	CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
	CHECK_POINTER_VOID(pCtrl);
	pCtrl->PostToolBarClickEventMsg(2,IVS_TOOLBAR_VIDEO_TVW, pVideoPane->GetPaneID());
}

void CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimeSetWarnWnd()
{
	SetCurrentPaneToActivePane();
	CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
	CHECK_POINTER_VOID(pVideoPane);
	CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
	CHECK_POINTER_VOID(pCtrl);
	pCtrl->PostToolBarClickEventMsg(2,IVS_TOOLBAR_ALARM_WIN, pVideoPane->GetPaneID());
}

void CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimePtz()
{
	SetCurrentPaneToActivePane();
	CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
	CHECK_POINTER_VOID(pVideoPane);
	CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
	CHECK_POINTER_VOID(pCtrl);

	// �Ѿ���PTZ
	if (BOTTOMBAR_PTZ_OPEN == pVideoPane->GetRealtimePtzState())
	{
		// ��ѵ��ť
		if (TYPE_WND_TURNCTUISE == pVideoPane->GetPrveTypeWnd())
		{
			pVideoPane->SetTypeWnd(pVideoPane->GetPrveTypeWnd());
			pVideoPane->SetPrveTypeWnd(TYPE_WND_NOMAL);
			pVideoPane->SetTurnCruiseStyle(pVideoPane->GetPrveTurnCruiseStyle());
			pVideoPane->SetPrveTurnCruiseStyle(0);
			pVideoPane->ResetBarPosition();
		}

		std::string strButStatus = CToolsHelp::Int2Str(BOTTOMBAR_PTZ_CLOSE);
		pCtrl->PostToolBarClickEventMsg(2,IVS_TOOLBAR_PTZ, pVideoPane->GetPaneID(), strButStatus);//lint !e1036

		// ɾ���ƾ�����
		pVideoPane->DeleteDigtalPtz();
		pVideoPane->SetRealtimePtzState(BOTTOMBAR_PTZ_CLOSE);
		SetPtzBtnState(false);
		return;
	}
	// ��ѵ��ť
	if (TYPE_WND_TURNCTUISE == pVideoPane->GetTypeWnd())
	{
		pVideoPane->SetPrveTurnCruiseStyle(pVideoPane->GetTurnCruiseStyle());
		pVideoPane->SetTurnCruiseStyle(0);
		pVideoPane->SetPrveTypeWnd(pVideoPane->GetTypeWnd());
		// ���ô�������
		pVideoPane->SetTypeWnd(TYPE_WND_NOMAL);
	}

	std::string strButStatus = CToolsHelp::Int2Str(BOTTOMBAR_PTZ_OPEN);
	pCtrl->PostToolBarClickEventMsg(2,IVS_TOOLBAR_PTZ, pVideoPane->GetPaneID(), strButStatus);//lint !e1036

	// ����ֲ��Ŵ����ˣ���Ҫ�ر�
	if (MAGNIFY_OPEN == pVideoPane->GetRealtimeMagnifyState())
	{
		// ɾ��Դ���ڵ����в�ֹͣ�������ڵķŴ�Ч��
		::PostMessage(pVideoPane->GetSafeHwnd(), WM_MAGNIFY_CLOSE_ALL, 0, 0);
		(void)pVideoPane->SetRealtimeMagnifyState(MAGNIFY_CLOSE);
		SetMagnifyBtnState(false);
		// �ر����ܷ�����ͼ
		(void)pCtrl->IVS_OCX_DisableDraw(pVideoPane->GetPaneID());

		// �ָ����д��ڵľֲ��Ŵ�ť
		pCtrl->GetVideoPaneMgr()->SetALLMagnifyBtnStatus(pVideoPane, 1);//lint !e747
	}

	//û����ʵ�����¼�������
	if (VIDEO_PLAY_STATE_NOT_PLAYING == pVideoPane->GetPlayState() || VIDEO_PLAY_STATE_RESTARTING == pVideoPane->GetPlayState())
	{
		return;
	}
	// �����ƾ�
	pVideoPane->CreateDigtalPtz();
	pVideoPane->ResetBarPosition();
	// �����ƾ�״̬
	pVideoPane->SetRealtimePtzState(BOTTOMBAR_PTZ_OPEN);
	SetPtzBtnState(true);
}

void CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimeSuperIntel()
{
	SetCurrentPaneToActivePane();
	CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
	CHECK_POINTER_VOID(pVideoPane);
	CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
	CHECK_POINTER_VOID(pCtrl);
	pCtrl->PostToolBarClickEventMsg(2,IVS_TOOLBAR_IA, pVideoPane->GetPaneID());
}

void CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimeEMap()
{
	SetCurrentPaneToActivePane();
	CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
	CHECK_POINTER_VOID(pVideoPane);
	CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
	CHECK_POINTER_VOID(pCtrl);
	//pVideoPane->SetPlayState(VIDEO_PLAY_STATE_EMAP);
	pVideoPane->Invalidate(TRUE);

	CRect rect;
	pVideoPane->GetClientRect(rect);
	pCtrl->PostToolBarEMapClickEventMsg(2,IVS_TOOLBAR_OPEN_MAP, (ULONG)pVideoPane->GetSafeHwnd(),rect);
}

afx_msg LRESULT CTranVideoRealtimeBar::OnIdcMenu(WPARAM wParam, LPARAM /*lParam*/)
{
	RoutineButtonShowMenuEvent(wParam);
	RoutineButtonHideMenuEvent(wParam);

	return IVS_SUCCEED;
}

void CTranVideoRealtimeBar::SetCurrentPaneToActivePane()
{
	CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
	CHECK_POINTER_VOID(pVideoPane);
	pVideoPane->SetActivePane();
}

/******************************************************************
//function : RoutineButtonShowMenuEvent
//description:
//input : WPARAM wParam
//output : NA
//return : void
*******************************************************************/
void CTranVideoRealtimeBar::RoutineButtonShowMenuEvent( WPARAM wParam )
{
	if( IDC_BTN_TOOL_REALTIME_CAPTURE == wParam ){
		OnBnClickedButtonRealtimeCapture();
	} else if( IDC_BTN_TOOL_REALTIME_RECORD == wParam ){
		OnBnClickedButtonRealtimeRecord();
	} else if( IDC_BTN_TOOL_REALTIME_LABEL == wParam ){
		OnBnClickedButtonRealtimeLabel();
	} else if( IDC_BTN_TOOL_REALTIME_MAGNIFY == wParam ){
		OnBnClickedButtonRealtimeMagnify();
	} else if( IDC_BTN_TOOL_REALTIME_SOUND == wParam ){
		OnBnClickedButtonRealtimeSound();
	} else if( IDC_BTN_TOOL_REALTIME_TALKBACK == wParam ){
		OnBnClickedButtonRealtimeTalkback();
	} else if( IDC_BTN_TOOL_REALTIME_INSTANT_BACK_PLAY == wParam ){
		OnBnClickedBtnToolRealtimeInstantBackPlay();
	}
}

/******************************************************************
//function : RoutineButtonHideMenuEvent
//description:
//input : WPARAM wParam
//output : NA
//return : void
*******************************************************************/
void CTranVideoRealtimeBar::RoutineButtonHideMenuEvent( WPARAM wParam )
{
	if( IDC_BTN_TOOL_REALTIME_NET_QUALITY == wParam ){
		OnBnClickedBtnToolRealtimeNetQuality();
	} else if( IDC_BTN_TOOL_REALTIME_MASTER_AUXILIARY == wParam ){
		OnBnClickedBtnToolRealtimeMasterAuxiliary();
	} else if( IDC_BTN_TOOL_REALTIME_TVWALL == wParam ){
		OnBnClickedBtnToolRealtimeTvwall();
	} else if( IDC_BTN_TOOL_REALTIME_SET_WARN_WND == wParam ){
		OnBnClickedBtnToolRealtimeSetWarnWnd();
	} else 	if( IDC_BTN_TOOL_REALTIME_PTZ == wParam ){
		OnBnClickedBtnToolRealtimePtz();
	} else 	if( IDC_BTN_TOOL_REALTIME_SUPER_INTEL == wParam ){
		OnBnClickedBtnToolRealtimeSuperIntel();
	} else 	if( IDC_BTN_TOOL_REALTIME_E_MAP == wParam ){
		OnBnClickedBtnToolRealtimeEMap();
    } else 	if( IDC_BTN_TOOL_REALTIME_PTZ3D == wParam ){
        OnBnClickedBtnToolRealtimePtz3d();
    }
}

/******************************************************************
function : CTranslucentButton
description:
output : NA
return : void
*******************************************************************/
void CTranVideoRealtimeBar::SetButtonPicture(
	CTranslucentButton &tButton,
	const std::string &strButtonName)
{
	// �������� ��ȡ��Ӧ����Դ�ļ��е���Ϣ
	std::string strToolTip;
	if (CI18N::GetInstance().GetResourceLanguage(strButtonName, strToolTip))
	{
		tButton.UpdateTipText(strToolTip);
	}
	// ��ӵ�tip��
	(void)m_TipMap.insert(std::make_pair(strButtonName, &tButton));

	TCHAR pcImageList[TWS_BUTTON_NUM][MAX_PATH]={0x0};

	memset(pcImageList,0x0,256*5);
	
	/*
	 * DTS2013122007506  modify by w00210470 TO BE FIXED!!
	 * ����������������һ�����ܷ����쳣���ݲ��Բ���
	 * LoadImageList �ڲ����Ӵ��벶��
	 * ToolBarGetPNGButtonByName ȫ�ֲ���
	 */
	try 
	{
		(void)SkinMgr.ToolBarGetPNGButtonByName(strButtonName,pcImageList);
	}
	catch(...)
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "ToolBar GetPNGButtonByName", "Exception");
	}

	(void)tButton.LoadImageList(pcImageList);
	tButton.SetStrFunctionName(strButtonName);
	RegisterTranslucentWnd(&tButton);
}

void CTranVideoRealtimeBar::OnMouseMove(UINT nFlags, CPoint point)
{
    CVideoPane *pVideoPane = dynamic_cast<CVideoPane *>(m_pVideoPane);
    CHECK_POINTER_VOID(pVideoPane);
    pVideoPane->OnMouseMove(nFlags, point);
    CTranslucentDialog::OnMouseMove(nFlags, point);
}

void CTranVideoRealtimeBar::OnLButtonDown(UINT nFlags, CPoint point)
{
    SetCurrentPaneToActivePane();
    CTranslucentDialog::OnLButtonDown(nFlags, point);
}

void CTranVideoRealtimeBar::SetRecordBtnState(bool state)
{
	CVideoPane* pVideoPane = dynamic_cast <CVideoPane*>(m_pVideoPane);
	if (NULL != pVideoPane)
	{
		pVideoPane->GetVideoTitleBar()->SetRecordStatus(state ? 1 : 0);
	}
    m_bRecordStarted = state;
    m_buttonLocalRecord.SetChecked(state);
    m_buttonLocalRecord.Invalidate(TRUE);
	if (NULL != pVideoPane)
	{
		if (pVideoPane->GetVideoTitleBar())
		{
			pVideoPane->GetVideoTitleBar()->Invalidate(TRUE);
		}
	}
}

void CTranVideoRealtimeBar::SetMagnifyBtnState(bool state)
{
	//m_bMagnifyStarted = state;
	m_buttonMagnify.SetChecked(state);
	m_buttonMagnify.Invalidate(TRUE);
}

void CTranVideoRealtimeBar::SetTalkbackCtrlState(bool state)
{
	//m_bMagnifyStarted = state;
	m_buttonTalkback.SetChecked(state);
	m_buttonTalkback.Invalidate(TRUE);
}

void CTranVideoRealtimeBar::SetPtzBtnState(bool state)
{
	//m_bMagnifyStarted = state;
	m_buttonPtz.SetChecked(state);
	m_buttonPtz.Invalidate(TRUE);
}

void CTranVideoRealtimeBar::SetPtz3DBtnState(bool state)
{
    m_bPtz3DStarted = state;
    m_BtnPtz3D.SetChecked(state);
    m_BtnPtz3D.Invalidate(TRUE);
}

ULONG CTranVideoRealtimeBar::GetTalkbackSoundState()
{
    CVideoPane* pVideoPane = dynamic_cast <CVideoPane*>(m_pVideoPane);
    CHECK_POINTER(pVideoPane,IVS_FAIL);
    return pVideoPane->GetRealtimeTalkbackState();
}

BOOL CTranVideoRealtimeBar::IsTalkback()
{
    CVideoPane* pVideoPane = dynamic_cast <CVideoPane*>(m_pVideoPane);
    CHECK_POINTER(pVideoPane,TRUE);
    CVideoPaneMgr * pVideoPaneMgr = pVideoPane->GetVideoPaneMgr();
    CHECK_POINTER(pVideoPaneMgr,TRUE);
    return pVideoPaneMgr->IsTalkback();
}

// ����������ťͼƬ
void CTranVideoRealtimeBar::SetSoundPic(ULONG ulState)
{
    CVideoPane *pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
    CHECK_POINTER_VOID(pVideoPane);

    if (BOTTOMBAR_SOUND_CLOSE == GetSoundState())
    {
        // ����
        SetButtonPicture(m_buttonPlaySound,"PlayNoSound");
        if (BUTTON_NORMAL == ulState)
        {
            if (m_bSoundCtrlState)
            {
                m_bSoundState = FALSE;
                pVideoPane->IsSoundOn(false);
            }
        } else if (BUTTON_CLICKED == ulState) {
            m_bSoundState = TRUE;

            pVideoPane->IsSoundOn(true);
        } else {
            m_bSoundState = FALSE;

            pVideoPane->IsSoundOn(false);
        }
    } else if(BOTTOMBAR_SOUND_OPEN == GetSoundState()) {
        SetButtonPicture(m_buttonPlaySound,"PlaySound");
        m_bSoundState = TRUE;

        pVideoPane->IsSoundOn(true);
    }

    //m_ulStatusSound = ulState;
    UpdateView();
}

void CTranVideoRealtimeBar::InitSoundCtrl( ULONG ulType )
{
    switch(ulType)
    {
    case SOUNDCTRL_WND_TYPE_SOUND:
        CHECK_POINTER_VOID(m_pVideoSoundCtrl);
        m_pVideoSoundCtrl->SetSliderPosition(SOUNDCTRL_SLIDER_LEN - m_ulSoundCtrlValue);
        m_pVideoSoundCtrl->SetSoundSilencePic(FALSE);
        m_pVideoSoundCtrl->SetSoundCtrlType(SOUNDCTRL_WND_TYPE_SOUND);

        break;
    case SOUNDCTRL_WND_TYPE_TALKBACK:
        CHECK_POINTER_VOID(m_pVideoTalkbackSound);

        m_pVideoTalkbackSound->SetSliderPosition(SOUNDCTRL_SLIDER_LEN - m_ulTalkbackSoundValue);
        m_pVideoTalkbackSound->SetTalkbackSilencePic(TRUE);
        m_pVideoTalkbackSound->SetSoundCtrlType(SOUNDCTRL_WND_TYPE_TALKBACK);

        break;
    default:
        break;
    }
}

void CTranVideoRealtimeBar::ChangeSoundCtrlValue( ULONG ulType, ULONG ulValue )
{
	IVS_DEBUG_TRACE("Type: %lu, Value: %lu", ulType, ulValue);

	CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
	CHECK_POINTER_VOID(pVideoPane);
	ULONG ulWinID = pVideoPane->GetPaneID();

	CVideoPaneMgr * pVideoPaneMgr = pVideoPane->GetVideoPaneMgr();
	CHECK_POINTER_VOID(pVideoPaneMgr);
 // TODO �����ͣ������
	if (BOTTOMBAR_SOUND_OPEN == pVideoPane->GetRealtimeSoundState() || BOTTOMBAR_TALKBACK_OPEN == pVideoPane->GetRealtimeTalkbackState())
	{
		DWORD ulVolumeValue = 0;
		ulVolumeValue = (ULONG)(((float)0xFFFF / 100) * (float)ulValue);
		ulVolumeValue = (ulVolumeValue << 16) + ulVolumeValue;
		CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
		CHECK_POINTER_VOID(pCtrl);

		if(IVS_SUCCEED != pCtrl->SetVolume(ulWinID, ulType, ulVolumeValue))
		{
			return;
		}
		
	}
}

ULONG CTranVideoRealtimeBar::GetSoundState()
{
    CVideoPane* pVideoPane = dynamic_cast <CVideoPane*>(m_pVideoPane);
    CHECK_POINTER(pVideoPane,IVS_OPERATE_MEMORY_ERROR);

    return pVideoPane->GetRealtimeSoundState();
}

void CTranVideoRealtimeBar::StartSound()
{
	IVS_DEBUG_TRACE("");

    CVideoPane* pVideoPane = dynamic_cast <CVideoPane*>(m_pVideoPane);
    CHECK_POINTER_VOID(pVideoPane);
    ULONG ulWinID = pVideoPane->GetPaneID();

    CVideoPaneMgr * pVideoPaneMgr = pVideoPane->GetVideoPaneMgr();
    CHECK_POINTER_VOID(pVideoPaneMgr);
	CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
	CHECK_POINTER_VOID(pCtrl);
    // ����������Խ����򲻿�����·����
    if(BOTTOMBAR_TALKBACK_OPEN == pVideoPaneMgr->IsTalkback())
    {
        //BP_RUN_LOG_ERR(IVS_OCX_RET_VOICE_TALKBACK_HAS_PLAYED_ERROR,"","can't open realtime sound, when talkback is open");
        // modi by wKF71044 at [2013-6-4 16:30:52] DTS2013052309584
        // �������ⵥҪ����Ҫ�ϱ���CU
        pCtrl->PostSoundEvent();
        return;
    }

    CGlobalSoundMgr &SoundMgr = CGlobalSoundMgr::Instance();
    SoundMgr.SetSoundingPanel(NULL); //�ر���·����
    // ������·����֮ǰ�ȹر��������ڵ���·����
    pVideoPaneMgr->StopSoundCtrlForOther(true);

    if (BOTTOMBAR_SOUND_CLOSE == pVideoPane->GetRealtimeSoundState())
    {
		// ��������
        IVS_INT32 iRet = (IVS_INT32)pCtrl->StartSound(ulWinID);
        if (IVS_SUCCEED != iRet)
        {
            //  modify by wanglei 00165153:2013.6.10 DTS2013060606556 
            // ����Ѿ����������Խ�������Ҫ֪ͨCU��·����ʧ��ԭ��
            if (iRet == IVS_PLAYER_RET_TALKBACK_OPENED_ERROR)
            {
                pCtrl->PostSoundEvent();
            }
            return;

        }
		//if(IVS_SUCCEED != pVideoPane->GetOCXPlayerCtrl()->StartSound(ulWinID))
		//{
		//	return;
		//}

		pVideoPane->SetRealtimeSoundState(BOTTOMBAR_SOUND_OPEN);
		pVideoPane->IsSoundOn(true);
		m_bSoundState = TRUE;

        // modify by wanglei 00165153:2013.6.28 DTS2013062702103
        // ��Ҫ�ϱ���·������ǰ״̬������¼��ط����λ�������
        CString xmlToCU;
        xmlToCU ="";
        //CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
        //CHECK_POINTER_VOID(pCtrl);
        PostEventXMLImpl.GetVolumeEventMsg(xmlToCU, pVideoPane->GetPaneID(), 1, 0);
        pCtrl->IVS_OCX_Windows_Event(EVENT_SOUND_STATUS, (LPCTSTR)xmlToCU);

		//����
		SetButtonPicture(m_buttonPlaySound,    "PlaySound");
		UpdateView();
		//CGlobalSoundMgr &SoundMgr = CGlobalSoundMgr::Instance();
		SoundMgr.SetSoundingPanel(pVideoPane);
		if (NULL != m_pVideoSoundCtrl)
		{
			m_pVideoSoundCtrl->SetSoundSilencePic(TRUE);
		}

        // ֪ͨ������ͬ������Ĵ�����������Ϊ����״̬
        //pVideoPaneMgr->NotifyOtherSoundOpen(ulWinID, true);
	}
}

void CTranVideoRealtimeBar::StopSound()
{
	IVS_DEBUG_TRACE("");

    CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
    CHECK_POINTER_VOID(pVideoPane);
    ULONG ulWinID = pVideoPane->GetPaneID();

    CVideoPaneMgr * pVideoPaneMgr = pVideoPane->GetVideoPaneMgr();
    CHECK_POINTER_VOID(pVideoPaneMgr);
	CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
	CHECK_POINTER_VOID(pCtrl);
    if (BOTTOMBAR_SOUND_OPEN == pVideoPane->GetRealtimeSoundState())
    {
		pVideoPane->SetRealtimeSoundState(BOTTOMBAR_SOUND_CLOSE);
		pVideoPane->IsSoundOn(false);
		m_bSoundState = FALSE;

		// modify for �˴����ܷ��أ�Ҫ֮������Ǿ��ٷ���
		// CHECK_POINTER_VOID(m_pVideoSoundCtrl);
		// m_pVideoSoundCtrl->SetSoundSilencePic(FALSE);

		if (m_pVideoSoundCtrl)
		{
			m_pVideoSoundCtrl->SetSoundSilencePic(FALSE);
		}

		//����
		SetButtonPicture(m_buttonPlaySound,"PlayNoSound");
		UpdateView();


// 		CGlobalSoundMgr &SoundMgr = CGlobalSoundMgr::Instance();
// 		SoundMgr.SetSoundingPanel(NULL);

        // �ر�����
        if(IVS_SUCCEED != pCtrl->StopSound(ulWinID))
        {
            return;
        }
        //pVideoPaneMgr->NotifyOtherSoundOpen(ulWinID, false);
    }
}

std::bitset<TOOLBARBTNNUM> CTranVideoRealtimeBar::GetBitsetButtonVisableStatus() const
{
    std::bitset<TOOLBARBTNNUM> BtnVisableStatus(m_ullToolBarBtnVisableStatus);
    return BtnVisableStatus;
}

void CTranVideoRealtimeBar::DeleteTalkbackCtrl()
{
	if (m_bTalkbackSoundCreated)
	{
		m_bTalkbackSoundCreated = false;
		IVS_DELETE(m_pVideoTalkbackSound);
	}
    //m_bTalkbackCtrlState = FALSE;
}

void CTranVideoRealtimeBar::EnableALLButton( BOOL isEnable )
{
	std::vector<CTranslucentButton *>::const_iterator it;
	for (it = m_btnVector.begin(); it != m_btnVector.end(); ++it)
	{
		CTranslucentWnd* translucentWnd = *it;
		translucentWnd->EnableWindow(isEnable);
	}
	//���水ť״̬
	m_bIsButtonEnable = isEnable;
	UpdateView();
}

IVS_INT32 CTranVideoRealtimeBar::StopTalkbackSound()
{
	CVideoPane* pVideoPane = dynamic_cast <CVideoPane*>(m_pVideoPane);
	CHECK_POINTER(pVideoPane, IVS_FAIL);
	CVideoPaneMgr * pVideoPaneMgr = pVideoPane->GetVideoPaneMgr();
	CHECK_POINTER(pVideoPaneMgr, IVS_FAIL);
	CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
	CHECK_POINTER(pCtrl, IVS_FAIL);
// 	if (BOTTOMBAR_TALKBACK_CLOSE == pVideoPane->GetRealtimeTalkbackState())
// 	{
// 		(void)pVideoPane->GetOCXPlayerCtrl()->StopTalkback(pVideoPane->GetPaneID());
// 	}
	IVS_INT32 iRet = IVS_FAIL;
	if (BOTTOMBAR_TALKBACK_OPEN == pVideoPane->GetRealtimeTalkbackState())
	{
		// ����������ť
		m_buttonPlaySound.EnableWindow(TRUE);
		// �رնԽ�
		iRet = (IVS_INT32)pCtrl->StopTalkback(pVideoPane->GetPaneID());
		if(IVS_SUCCEED != iRet)
		{
			return iRet;
		}

		pVideoPane->SetRealtimeTalkbackState(BOTTOMBAR_TALKBACK_CLOSE);
		//pVideoPane->IsSoundOn(false);
		SetTalkbackCtrlState(false);
		m_bTalkbackSoundState = TRUE;

		// modify for �˴����ܷ��أ�Ҫ֮������Ǿ��ٷ���
		// CHECK_POINTER_VOID(m_pVideoSoundCtrl);
		// m_pVideoSoundCtrl->SetSoundSilencePic(FALSE);

		if (m_pVideoTalkbackSound)
		{
			m_pVideoTalkbackSound->SetTalkbackSilencePic(FALSE);
		}

		//����
		//SetButtonPicture(m_buttonTalkback,"NoTalkBack");

        // �ر��������ڵ�����
        pVideoPaneMgr->StopSoundCtrlForOther(true);
		UpdateView();


// 		CGlobalSoundMgr &SoundMgr = CGlobalSoundMgr::Instance();
// 		SoundMgr.SetSoundingPanel(NULL);
	}



	return IVS_SUCCEED;
}

IVS_INT32 CTranVideoRealtimeBar::StartTalkbackSound()
{
	IVS_DEBUG_TRACE("");
	IVS_INT32 iRet = IVS_FAIL;
	CVideoPane* pVideoPane = dynamic_cast <CVideoPane*>(m_pVideoPane);
	CHECK_POINTER(pVideoPane, IVS_FAIL);
	CVideoPaneMgr * pVideoPaneMgr = pVideoPane->GetVideoPaneMgr();
	CHECK_POINTER(pVideoPaneMgr, IVS_FAIL);
	CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
	CHECK_POINTER(pCtrl,IVS_FAIL);
	// ���������Խ� �ȹر��������ڵ������Խ�
	pVideoPaneMgr->StopTalkbackSoundForOther();
    // �ر��������ڵ�����
    pVideoPaneMgr->StopSoundCtrlForOther(true);
	if (BOTTOMBAR_TALKBACK_CLOSE == pVideoPane->GetRealtimeTalkbackState())
	{
		// �����Խ������ʧ�ܣ�ֱ�Ӳ���������κζ���
		iRet = (IVS_INT32)pCtrl->StartTalkback(pVideoPane->GetPaneID());
		if (IVS_SUCCEED != iRet)
		{
			return iRet;
		}
		// ������·������ť
		this->StopSound();
		m_buttonPlaySound.EnableWindow(FALSE);

		pVideoPane->SetRealtimeTalkbackState(BOTTOMBAR_TALKBACK_OPEN);
		//pVideoPane->IsSoundOn(true);
		SetTalkbackCtrlState(true);
		m_bTalkbackSoundState = FALSE;
		CHECK_POINTER(m_pVideoTalkbackSound, IVS_FAIL);
		m_pVideoTalkbackSound->SetTalkbackSilencePic(TRUE);
		//����
		//SetButtonPicture(m_buttonTalkback,    "TalkBack");
		UpdateView();
//      ȫ��ͬ��
// 		CGlobalSoundMgr &SoundMgr = CGlobalSoundMgr::Instance();
// 		SoundMgr.SetSoundingPanel(pVideoPane);
	}	
	return IVS_SUCCEED;
}

void CTranVideoRealtimeBar::ResetBottomButton()
{
	m_isExtend = false;
	m_iNumBtn = MAXNUM_SHOW_BUTTON;
	DrawButton();
	//��չ
	UnregisterTranslucentWnd(&m_buttonLocker);
	SetButtonPicture(m_buttonLocker,		_T("Extend"));
	//����
	//SetButtonPicture(m_buttonPlaySound,    _T("PlayNoSound"));
	//�Խ�
	//SetButtonPicture(m_buttonTalkback,	   _T("TalkBack"));
	UpdateView();
}

void CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimePtz3d()
{
    // TODO: Add your control notification handler code here
    SetCurrentPaneToActivePane();
    CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
    CHECK_POINTER_VOID(pVideoPane);
    CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
    CHECK_POINTER_VOID(pCtrl);
    pCtrl->PostToolBarClickEventMsg(2,IVS_TOOLBAR_PTZ3D, pVideoPane->GetPaneID());

    // ����3D�ƾ�
    //�ж��Ƿ����ڲ��ţ�û�ڲ�����������3D״̬
    if (VIDEO_PLAY_STATE_NOT_PLAYING == pVideoPane->GetPlayState())
    {
        BP_RUN_LOG_ERR(IVS_OCX_RET_VIDEOPANE_ISNOT_PLAYING_ERROR,"", "Video Pane Manager set 3D PTZ error,pane is not playing video.");
        return;
    }
    ULONG ulIsStart3D = pVideoPane->Get3DPTZState();
    if (PTZ_3D_CLOSE == ulIsStart3D) //lint !e737 ������
    {
        ulIsStart3D = PTZ_3D_OPEN;
        SetPtz3DBtnState(true);
    }
    else if (PTZ_3D_OPEN == ulIsStart3D) //lint !e737 ������
    {
        ulIsStart3D = PTZ_3D_CLOSE;
        SetPtz3DBtnState(false);
    }
    //���ô���Ĺ��������豸����
    pVideoPane->Set3DPTZState(ulIsStart3D);
}


void CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimeReturn()
{
	// TODO: Add your control notification handler code here
	SetCurrentPaneToActivePane();
	CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
	CHECK_POINTER_VOID(pVideoPane);

	//����һ�����ذ�ť������¼�
	CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
	CHECK_POINTER_VOID(pCtrl);
	pCtrl->PostToolBarClickEventMsg(2,IVS_TOOLBAR_EXIT_PLAY_RECORD, pVideoPane->GetPaneID());
	//�ָ�ʵ��
	pVideoPane->ResumeRealPlay();

	// ��ѵ��ť
	if (TYPE_WND_TURNCTUISE == pVideoPane->GetPrveTypeWnd())
	{
		pVideoPane->SetTypeWnd(pVideoPane->GetPrveTypeWnd());
		pVideoPane->SetPrveTypeWnd(TYPE_WND_NOMAL);
		pVideoPane->SetTurnCruiseStyle(pVideoPane->GetPrveTurnCruiseStyle());
		pVideoPane->SetPrveTurnCruiseStyle(0);
		pVideoPane->ResetBarPosition();
	}
}


void CTranVideoRealtimeBar::OnBnClickedBtnToolRealtimeStart()
{
	// TODO: Add your control notification handler code here
	CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
	CHECK_POINTER_VOID(pVideoPane);
	//�Ѿ��ڲ����˾�ֹͣ����Ϊ������״̬
	if (m_bIsStartPlay)
	{
		//ֹͣ���ţ��ɹ�����0
		if (0 != pVideoPane->PauseRecordPlay())
		{
			return;
		}
		SetRecordPlayStop(TRUE);
		m_bIsStartPlay = FALSE;
		//SetIsStartPlay(FALSE);
		////ֹͣ����
		//m_bIsStartPlay = FALSE;
		////����Ϊ�����Ű�ťͼƬ
		//SetButtonPicture(m_btnStart,	  _T("StartBack"));
	}
	else
	{
		//��ʼ����,�ɹ�����0
        pVideoPane->SetStartPlayTime();
		if (0 != pVideoPane->StartRecordPlay())
		{
			return;
		}
		SetRecordPlayStop(FALSE);
		m_bIsStartPlay = TRUE;
		//SetIsStartPlay(TRUE);
		////��ʼ����
		//m_bIsStartPlay = TRUE;
		////����Ϊ��ֹͣ��ťͼƬ
		//SetButtonPicture(m_btnStart,	  _T("StopBack"));
	}

	//SetIsStartPlay(m_bIsStartPlay);

	UpdateView();
}

void CTranVideoRealtimeBar::SetIsStartPlay(BOOL val)
{
	if (val)
	{
		SetButtonPicture(m_btnStart,	  _T("StartBack"));
	}
	else
	{
		SetButtonPicture(m_btnStart,	  _T("StopBack"));
	}
	m_bIsStartPlay = !val;
	UpdateView();
}

void CTranVideoRealtimeBar::SetRecordPlayStop(BOOL val)
{
	if (val)
	{
		SetButtonPicture(m_btnStart,	  _T("StartBack"));
		m_bIsStartPlay = FALSE;
	}
	else
	{
		SetButtonPicture(m_btnStart,	  _T("StopBack"));
		m_bIsStartPlay = TRUE;
	}

	UpdateView();
}

LRESULT CTranVideoRealtimeBar::OnSetSoundState( WPARAM wParam, LPARAM lParam )
{
    IVS_DEBUG_TRACE("");

    CVideoPane* pVideoPane = dynamic_cast <CVideoPane*>(m_pVideoPane);
    CHECK_POINTER(pVideoPane, 0);

    CVideoPaneMgr * pVideoPaneMgr = pVideoPane->GetVideoPaneMgr();
    CHECK_POINTER(pVideoPaneMgr, 0);

    if ( 0 == lParam )
    {
        // ����������Խ����򲻿�����·����
        if(BOTTOMBAR_TALKBACK_OPEN == pVideoPaneMgr->IsTalkback())
        {
            //BP_RUN_LOG_ERR(IVS_OCX_RET_VOICE_TALKBACK_HAS_PLAYED_ERROR,"","can't open realtime sound, when talkback is open");
            return 0;
        }

        if (BOTTOMBAR_SOUND_CLOSE == pVideoPane->GetRealtimeSoundState())
        {
            pVideoPane->SetRealtimeSoundState(BOTTOMBAR_SOUND_OPEN);
            pVideoPane->IsSoundOn(true);
            m_bSoundState = TRUE;

            if (m_pVideoSoundCtrl)
            {
                m_pVideoSoundCtrl->SetSoundSilencePic(TRUE);
            }

            //����
            SetButtonPicture(m_buttonPlaySound,    "PlaySound");
            UpdateView();
        }
    }
    else if ( 1 == lParam )
    {
        if (BOTTOMBAR_SOUND_OPEN == pVideoPane->GetRealtimeSoundState())
        {
            pVideoPane->SetRealtimeSoundState(BOTTOMBAR_SOUND_CLOSE);
            pVideoPane->IsSoundOn(false);
            m_bSoundState = FALSE;

            if (m_pVideoSoundCtrl)
            {
                m_pVideoSoundCtrl->SetSoundSilencePic(FALSE);
            }

            //����
            SetButtonPicture(m_buttonPlaySound,"PlayNoSound");
            UpdateView();
        }
    }
    
    return 0;
}//lint !e715


void CTranVideoRealtimeBar::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (VK_RETURN == nChar)
	{
		return;
	}

	CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(m_pVideoPane);
	CHECK_POINTER_VOID(pVideoPane);

	CVideoPaneMgr * pVideoPaneMgr = pVideoPane->GetVideoPaneMgr();
	CHECK_POINTER_VOID(pVideoPaneMgr);

	if (pVideoPaneMgr->GetActivePaneID()!=pVideoPane->GetPaneID())
	{
		return;
	}

	CWnd *pWnd = pVideoPaneMgr->GetParentWnd();
	CHECK_POINTER_VOID(pWnd);

	::PostMessage(pWnd->GetSafeHwnd(),WM_KEYUP,static_cast<LONG>(nChar),0);//lint !e732
	//CTranslucentDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}//lint !e715

// VideoSoundCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "VideoSoundCtrl.h"
#include "IVS_OCXPlayerCtrl.h"
#include "VideoPaneMgr.h"
#include "GlobalPlayMgr.h"
#include "PostEventXML.h"
#include "IVS_Trace.h"
#include "afxdialogex.h"

// CVideoSoundCtrl dialog

IMPLEMENT_DYNAMIC(CVideoSoundCtrl, CDialog)

CVideoSoundCtrl::CVideoSoundCtrl(CWnd* pParent /*=NULL*/)
    : CDialog(CVideoSoundCtrl::IDD, pParent)
    ,m_ulWndType(SOUNDCTRL_WND_TYPE_MAX)
    , m_pRealTimeToolBar(nullptr)
    ,m_ulSliderPosition(SOUNDCTRL_SLIDER_LEN / 2)
    ,m_ubCheckSoundCtrlStatus(FALSE)
{
}

CVideoSoundCtrl::~CVideoSoundCtrl()
{
	m_ulWndType   = SOUNDCTRL_WND_TYPE_MAX;
	m_pRealTimeToolBar   = nullptr;
	m_ulSliderPosition = SOUNDCTRL_SLIDER_LEN / 2;
	m_ubCheckSoundCtrlStatus = FALSE;
} //lint !e1540  MFC����

void CVideoSoundCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SOUNDCTRL, m_buttonSilence);
	DDX_Control(pDX, IDC_SLIDER_SOUNDCTRL, m_sliderSoundCtrl);
}

BEGIN_MESSAGE_MAP(CVideoSoundCtrl, CDialog)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_MESSAGE((WM_BITMAPSLIDER_MOVED), &CVideoSoundCtrl::OnBitmapSliderMoved)
	ON_BN_CLICKED(IDC_BUTTON_SOUNDCTRL, &CVideoSoundCtrl::OnBnClickedButtonSoundCtrl)
END_MESSAGE_MAP()

// CVideoSoundCtrl message handlers
BOOL CVideoSoundCtrl::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_sliderSoundCtrl.SetVertical(TRUE);
	m_ulSliderPosition = SOUNDCTRL_SLIDER_LEN / 2;
	m_sliderSoundCtrl.SetPos(static_cast<int>(m_ulSliderPosition));
	m_sliderSoundCtrl.SetRange(0, SOUNDCTRL_SLIDER_LEN);
	m_sliderSoundCtrl.SetTicFreq(SOUNDCTRL_SLIDER_INTERVAL);

	//��Ƥ��
	int iSkinType = GetSkinType();
	if (1 == iSkinType)
	{
		m_buttonSilence.LoadBitmaps(IDB_BITMAP_NOSOUND_CLICK, IDB_BITMAP_NOSOUND_NORMAL, IDB_BITMAP_NOSOUND_FOCUS);
	}
	else if (2 == iSkinType)
	{
		m_buttonSilence.LoadBitmaps(IDB_BITMAP_NOSOUND_FOCUS_WHITE, IDB_BITMAP_NOSOUND_FOCUS_WHITE, IDB_BITMAP_NOSOUND_FOCUS_WHITE);
	}

    DWORD dwExStyle = static_cast<DWORD>(::GetWindowLong(m_hWnd, GWL_EXSTYLE));
    VERIFY((dwExStyle & WS_EX_APPWINDOW) == 0);
    ::SetWindowLong(m_hWnd, GWL_EXSTYLE, static_cast<long >(dwExStyle | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE ));
	return TRUE;
}

void CVideoSoundCtrl::OnPaint()
{   
	CPaintDC dc(this); // device context for painting
	CHECK_POINTER_VOID(m_pRealTimeToolBar);
	CTranVideoRealtimeBar *pRealTimeToolBar = dynamic_cast<CTranVideoRealtimeBar*>(m_pRealTimeToolBar);
	CHECK_POINTER_VOID(pRealTimeToolBar);
	CVideoPane * pVideoPane = dynamic_cast<CVideoPane*>(pRealTimeToolBar->GetVideoPane());
	CHECK_POINTER_VOID(pVideoPane);
	CVideoPaneMgr * pVideoPaneMgr = pVideoPane->GetVideoPaneMgr();
	CHECK_POINTER_VOID(pVideoPaneMgr); 
	// ������Ϊǳ�һ�ɫ
	CRect rect;
	this->GetClientRect(rect);

	dc.FillSolidRect(rect, pVideoPaneMgr->m_BottomBarBKGColor);

	int iX = rect.left + (rect.Width()-16)/2;
	int iY = (rect.bottom - 2) - 16;
	m_buttonSilence.MoveWindow(iX, iY, 16, 16);

	// Slider
	iX = rect.left + (rect.Width()-13)/2;
	iY = rect.top  + 10;
	
// 	if (SOUNDCTRL_WND_TYPE_TALKBACK == m_ulWndType)
// 	{   
// 		if (BOTTOMBAR_TALKBACK_OPEN == (pRealTimeToolBar->GetTalkbackSoundState()))
// 		{
// 			m_buttonSilence.LoadBitmaps(IDB_BITMAP_NOTALKBACK_CLICK, IDB_BITMAP_NOTALKBACK_NORMAL, IDB_BITMAP_NOTALKBACK_FOCUS);
// 		} else {
// 			m_buttonSilence.LoadBitmaps(IDB_BITMAP_TALKBACK_CLICK, IDB_BITMAP_TALKBACK_CLICK, IDB_BITMAP_TALKBACK_FOCUS);
// 		}
// 	}

	HBITMAP hBitmapChannel = NULL;
	HBITMAP hBitmapChannelActive = NULL;
	//��Ƥ��
	int iSkinType = GetSkinType();
	if (1 == iSkinType)
	{
		hBitmapChannel = m_sliderSoundCtrl.StretchImage(IDB_BITMAP_SOUNDCTRL_SLIDER_CHANNEL, 13, rect.Height()-40);
		hBitmapChannelActive = m_sliderSoundCtrl.StretchImage(IDB_BITMAP_SOUNDCTRL_SLIDER_CHANNEL_ACTIVE, 13, rect.Height()-40);
	}
	else if (2 == iSkinType)
	{
		hBitmapChannel = m_sliderSoundCtrl.StretchImage(IDB_BITMAP_SOUNDCTRL_SLIDER_CHANNEL_WHITE, 13, rect.Height()-40);
		hBitmapChannelActive = m_sliderSoundCtrl.StretchImage(IDB_BITMAP_SOUNDCTRL_SLIDER_CHANNEL_ACTIVE_WHITE, 13, rect.Height()-40);
	}

	//HBITMAP hBitmapChannel = m_sliderSoundCtrl.StretchImage(IDB_BITMAP_SOUNDCTRL_SLIDER_CHANNEL, 13, rect.Height()-40);
	//HBITMAP hBitmapChannelActive = m_sliderSoundCtrl.StretchImage(IDB_BITMAP_SOUNDCTRL_SLIDER_CHANNEL_ACTIVE, 13, rect.Height()-40);
	(void)m_sliderSoundCtrl.SetBitmapChannelEx(hBitmapChannel, hBitmapChannelActive);
	(void)m_sliderSoundCtrl.SetBitmapThumb(IDB_BITMAP_SIGN_NORMAL, IDB_BITMAP_SIGN_CLICK, TRUE);

	m_sliderSoundCtrl.DrawFocusRect(FALSE);
	m_sliderSoundCtrl.MoveWindow(iX, iY, 13, rect.Height()-40);
	m_sliderSoundCtrl.SetPos(static_cast<int>(m_ulSliderPosition));

	return;
}

int CVideoSoundCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	return IVS_SUCCEED;
}

HBRUSH CVideoSoundCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (pDC!=NULL && nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));
	}

	return HBRUSH(GetStockObject(HOLLOW_BRUSH));
}//lint !e1762 !e818 !e715

// ����
void CVideoSoundCtrl::OnBnClickedButtonSoundCtrl()
{
	IVS_DEBUG_TRACE("");

    CHECK_POINTER_VOID(m_pRealTimeToolBar);
	CTranVideoRealtimeBar *pVideoReakToolBar= dynamic_cast<CTranVideoRealtimeBar*>(m_pRealTimeToolBar);
    CHECK_POINTER_VOID(pVideoReakToolBar);

    CVideoPane *pVideoPane = dynamic_cast<CVideoPane *>(pVideoReakToolBar->GetVideoPane());
    CHECK_POINTER_VOID(pVideoPane);

	CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
	CHECK_POINTER_VOID(pCtrl);

	CString xmlToCU;
	xmlToCU ="";


	switch(m_ulWndType)
	{
	case SOUNDCTRL_WND_TYPE_SOUND:     // ��·����
		{		
// 			CGlobalSoundMgr &SoundMgr = CGlobalSoundMgr::Instance();

			 //����������Խ�,��������
			if(pVideoReakToolBar->IsTalkback())
			{
                BP_RUN_LOG_INF("IVS_OCX::","TALKBACK PLAYE");
				return;
			}

			if (BOTTOMBAR_SOUND_OPEN == pVideoReakToolBar->GetSoundState())
			{   	
				BP_RUN_LOG_INF("IVS_OCX::","Stop Sound in Video Sound Ctrl");

    			pVideoReakToolBar->StopSound();

				/*
				 * DTS2014021302165 
				 * ȥ��panel��ȫ��ע��
				 */
				CGlobalSoundMgr &SoundMgr = CGlobalSoundMgr::Instance();
				if (SoundMgr.GetSoundingPanel() == pVideoPane)
				{
				    SoundMgr.Reset();
				}

				PostEventXMLImpl.GetVolumeEventMsg(xmlToCU, pVideoPane->GetPaneID(), 0, 0);
				pCtrl->IVS_OCX_Windows_Event(EVENT_SOUND_STATUS, (LPCTSTR)xmlToCU);
// 				SoundMgr.SetSoundingPanel(NULL);
				
				//(void)pVideoPaneMgr->GetOcxCtrl()->PostToolBarClickEventMsg(TOOLBAR_BTN_SOUND, BUTTON_NORMAL, pVideoReakToolBar->GetWndID());
			} else {

				BP_RUN_LOG_INF("IVS_OCX::","Start Sound in Video Sound Ctrl");

				// ��������
				pVideoReakToolBar->StartSound();
				PostEventXMLImpl.GetVolumeEventMsg(xmlToCU, pVideoPane->GetPaneID(), 1, 0);
				pCtrl->IVS_OCX_Windows_Event(EVENT_SOUND_STATUS, (LPCTSTR)xmlToCU);
				//(void)pVideoPaneMgr->GetOcxCtrl()->PostToolBarClickEventMsg(TOOLBAR_BTN_SOUND, BUTTON_CLICKED, pVideoReakToolBar->GetWndID());
// 				SoundMgr.SetSoundingPanel(pVideoPane);
				// ��������
				pVideoReakToolBar->ChangeSoundCtrlValue(SOUNDCTRL_WND_TYPE_SOUND, SOUNDCTRL_SLIDER_LEN - m_ulSliderPosition);
				PostEventXMLImpl.GetVolumeEventMsg(xmlToCU, pVideoPane->GetPaneID(), 2, SOUNDCTRL_SLIDER_LEN - m_ulSliderPosition);
				pCtrl->IVS_OCX_Windows_Event(EVENT_SOUND_STATUS, (LPCTSTR)xmlToCU);
			}			
		}
		break;   // end case SOUNDCTRL_WND_TYPE_SOUND:
	case SOUNDCTRL_WND_TYPE_TALKBACK:	// �����Խ�
		{			
			// ���ֻ���������Խ�����������Խ��Ŀ������û��ϵ
			if (BOTTOMBAR_TALKBACK_OPEN == (pVideoReakToolBar->GetTalkbackSoundState()))
			{				
				BP_RUN_LOG_INF("IVS_OCX::","Stop Talkback Sound in Video Sound Ctrl");
				
				//pVideoReakToolBar->ChangeSoundCtrlValue(SOUNDCTRL_WND_TYPE_TALKBACK, SOUNDCTRL_SLIDER_LEN);
				SetTalkbackSilencePic(!m_ubCheckSoundCtrlStatus);//lint !e730
				if (!m_ubCheckSoundCtrlStatus)
				{
					pVideoReakToolBar->ChangeSoundCtrlValue(SOUNDCTRL_WND_TYPE_TALKBACK, 0);
				}
				else
				{
					// ��������
					pVideoReakToolBar->ChangeSoundCtrlValue(SOUNDCTRL_WND_TYPE_TALKBACK, SOUNDCTRL_SLIDER_LEN - m_ulSliderPosition);
				}
				//pVideoReakToolBar->StopTalkbackSound();
				//(void)pVideoPaneMgr->GetOcxCtrl()->PostToolBarClickEventMsg(TOOLBAR_BTN_TALKBACK, BUTTON_NORMAL, pVideoReakToolBar->GetWndID());
			} 
// 			else {
// 				// ������������Խ�,��ر�������·����	
// 				/*pVideoPaneMgr->StopSoundCtrlForOther();*/
// 				// �ر��Լ�����·����
// 				pVideoReakToolBar->StopSound();
// 				// ��������
// 				//pVideoReakToolBar->StartTalkbackSound();
// 				SetTalkbackSilencePic(false);
// 				// ��������
// 				pVideoReakToolBar->ChangeSoundCtrlValue(SOUNDCTRL_WND_TYPE_TALKBACK, SOUNDCTRL_SLIDER_LEN - m_ulSliderPosition);	
// 			}
		}
		break;   // end case SOUNDCTRL_WND_TYPE_TALKBACK:	
	default:     // ����
		break;
	}
}

// �������ڱ������ĸ������Ѿ����Ǵ�����Ҫ��¼�¶�Ӧ�Ĵ���
void CVideoSoundCtrl::SetVideopBar(CWnd * pVideoBar)
{
    CHECK_POINTER_VOID(pVideoBar);
    m_pRealTimeToolBar = pVideoBar;
}

// �������ƶ����¼�
LRESULT CVideoSoundCtrl::OnBitmapSliderMoved(WPARAM wParam, LPARAM lParam)
{
	(void)AFX_MANAGE_STATE(AfxGetStaticModuleState());
	(void)wParam;
	(void)lParam;

	if (wParam == IDC_SLIDER_SOUNDCTRL)
	{
		ProcessSliderMoved();
	}

	return 0;
}

// ���Ž��Ȼ����������¼�
void CVideoSoundCtrl::ProcessSliderMoved()
{
	IVS_DEBUG_TRACE("");

	(void)AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CHECK_POINTER_VOID(m_pRealTimeToolBar);
    CTranVideoRealtimeBar *pVideoBar = dynamic_cast<CTranVideoRealtimeBar*>(m_pRealTimeToolBar);
    CHECK_POINTER_VOID(pVideoBar);

	CVideoPane *pVideoPane = dynamic_cast<CVideoPane *>(pVideoBar->GetVideoPane());
	CHECK_POINTER_VOID(pVideoPane);

	CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
	CHECK_POINTER_VOID(pCtrl);

	CString xmlToCU;
	xmlToCU ="";

	// ���¶Բ�ͬ����������
	switch(m_ulWndType)
	{
	case SOUNDCTRL_WND_TYPE_SOUND:
		{
			// ����������Խ�,��������
			if(pVideoBar->IsTalkback())
			{
				return;
			}

			// ��������Ϊ����״̬��Ҫ��StartSound������
			//if(BOTTOMBAR_SOUND_CLOSE == (pVideoBar->GetSoundState()))
			//{
			//	pVideoBar->StartSound();
			//}

			// �������ֶ�����
			m_ulSliderPosition = (ULONG) m_sliderSoundCtrl.GetPos();
			pVideoBar->ChangeSoundCtrlValue(SOUNDCTRL_WND_TYPE_SOUND, SOUNDCTRL_SLIDER_LEN - m_ulSliderPosition);
			pVideoBar->SetSoundCtrlValue(SOUNDCTRL_SLIDER_LEN - m_ulSliderPosition);

			PostEventXMLImpl.GetVolumeEventMsg(xmlToCU, pVideoPane->GetPaneID(), 2, SOUNDCTRL_SLIDER_LEN - m_ulSliderPosition);
			pCtrl->IVS_OCX_Windows_Event(EVENT_SOUND_STATUS, (LPCTSTR)xmlToCU);


			//// ������������ڵײ�ʱ��ҲҪ��StopSound�ر������������Ӱ����������
			if(SOUNDCTRL_SLIDER_LEN == m_ulSliderPosition)
			{
				pVideoBar->StopSound();
				PostEventXMLImpl.GetVolumeEventMsg(xmlToCU, pVideoPane->GetPaneID(), 0, 0);
				pCtrl->IVS_OCX_Windows_Event(EVENT_SOUND_STATUS, (LPCTSTR)xmlToCU);
				
// 				CGlobalSoundMgr &SoundMgr = CGlobalSoundMgr::Instance();
// 				SoundMgr.SetSoundingPanel(NULL);
			}
		}
		break;
	case SOUNDCTRL_WND_TYPE_TALKBACK:
		{
			//// ��������Ϊ����״̬��Ҫ��StartTalkbackSound������
			//if(BOTTOMBAR_SOUND_CLOSE == pVideoBar->GetTalkbackSoundState())
			//{
			//	//m_sliderSoundCtrl.SetPos(m_ulSliderPosition);
			//	//break;
			//	pVideoBar->StartTalkbackSound();
			//}

			// �������ֶ�����
			m_ulSliderPosition = (ULONG) m_sliderSoundCtrl.GetPos();
			pVideoBar->ChangeSoundCtrlValue(SOUNDCTRL_WND_TYPE_TALKBACK, SOUNDCTRL_SLIDER_LEN - m_ulSliderPosition);
			pVideoBar->SetTalkbackSoundValue(SOUNDCTRL_SLIDER_LEN - m_ulSliderPosition);
			SetTalkbackSilencePic(TRUE);
			// ������������ڵײ�ʱ��ҲҪ��StopTalkbackSound�ر������������Ӱ����������
			//if(SOUNDCTRL_SLIDER_LEN == m_ulSliderPosition)
			//{
			//	((CVideoRealtimeBar*)m_pRealTimeToolBar)->StopTalkbackSound();
			//}

			//// ������������ڵײ�ʱ��ҲҪ��StopSound�ر������������Ӱ����������
			if(SOUNDCTRL_SLIDER_LEN == m_ulSliderPosition)
			{
				//TODO  �޸�ͼ��
				//pVideoBar->ChangeSoundCtrlValue(SOUNDCTRL_WND_TYPE_TALKBACK, 0);
				SetTalkbackSilencePic(FALSE);
				//pVideoBar->StopTalkbackSound();
// 				PostEventXMLImpl.GetVolumeEventMsg(xmlToCU, pVideoPane->GetPaneID(), 0, 0);
// 				pCtrl->IVS_OCX_Windows_Event(EVENT_SOUND_STATUS, (LPCTSTR)xmlToCU);

				// 				CGlobalSoundMgr &SoundMgr = CGlobalSoundMgr::Instance();
				// 				SoundMgr.SetSoundingPanel(NULL);
			}
		}
		break;	
	default:
        BP_RUN_LOG_ERR(IVS_FAIL,"","IVS_OCX .error= %lu",m_ulWndType);
		break;
	}
	
	return;
}

// ����������ťͼƬ
void CVideoSoundCtrl::SetSoundSilencePic(BOOL bState)
{   
    CHECK_POINTER_VOID(m_pRealTimeToolBar);
    CTranVideoRealtimeBar *pRealTimeToolBar = dynamic_cast<CTranVideoRealtimeBar*>(m_pRealTimeToolBar);
    CHECK_POINTER_VOID(pRealTimeToolBar);
	int iSkinType = GetSkinType();
	switch(pRealTimeToolBar->GetSoundState())
	{
	case BOTTOMBAR_SOUND_CLOSE:
		{
			if (1 == iSkinType)
			{
				if (bState)
				{
					m_buttonSilence.LoadBitmaps(IDB_BITMAP_NOSOUND_NORMAL, IDB_BITMAP_NOSOUND_CLICK, IDB_BITMAP_NOSOUND_FOCUS);
				} else {
					m_buttonSilence.LoadBitmaps(IDB_BITMAP_NOSOUND_CLICK, IDB_BITMAP_NOSOUND_NORMAL, IDB_BITMAP_NOSOUND_FOCUS);
				}
			}
			else if (2 == iSkinType)
			{
				if (bState)
				{
					m_buttonSilence.LoadBitmaps(IDB_BITMAP_NOSOUND_FOCUS_WHITE, IDB_BITMAP_NOSOUND_FOCUS_WHITE, IDB_BITMAP_NOSOUND_FOCUS_WHITE);
				} else {
					m_buttonSilence.LoadBitmaps(IDB_BITMAP_NOSOUND_FOCUS_WHITE, IDB_BITMAP_NOSOUND_FOCUS_WHITE, IDB_BITMAP_NOSOUND_FOCUS_WHITE);
				}
			}
		}
		break;
	case BOTTOMBAR_SOUND_OPEN:
		{
			if (1 == iSkinType)
			{
				if (bState)
				{
					m_buttonSilence.LoadBitmaps(IDB_BITMAP_SOUND_CLICK, IDB_BITMAP_SOUND_CLICK, IDB_BITMAP_SOUND_FOCUS);
				} else {
					m_buttonSilence.LoadBitmaps(IDB_BITMAP_SOUND_CLICK, IDB_BITMAP_SOUND_CLICK, IDB_BITMAP_SOUND_FOCUS);
				}
			}
			else if (2 == iSkinType)
			{
				if (bState)
				{
					m_buttonSilence.LoadBitmaps(IDB_BITMAP_SOUND_FOCUS_WHITE, IDB_BITMAP_SOUND_FOCUS_WHITE, IDB_BITMAP_SOUND_FOCUS_WHITE);
				} else {
					m_buttonSilence.LoadBitmaps(IDB_BITMAP_SOUND_FOCUS_WHITE, IDB_BITMAP_SOUND_FOCUS_WHITE, IDB_BITMAP_SOUND_FOCUS_WHITE);
				}
			}
		}
		break;
	default:
		break;
	}

	m_buttonSilence.Invalidate(FALSE);
}

// ����������ťͼƬ
void CVideoSoundCtrl::SetTalkbackSilencePic(BOOL bState)
{   
    CHECK_POINTER_VOID(m_pRealTimeToolBar);
    CTranVideoRealtimeBar *pVideoBar = dynamic_cast<CTranVideoRealtimeBar*>(m_pRealTimeToolBar);
    CHECK_POINTER_VOID(pVideoBar);
	m_ubCheckSoundCtrlStatus  = bState;
	int iSkinType = GetSkinType();
	if (1 == iSkinType)
	{
		if (bState)
		{
			m_buttonSilence.LoadBitmaps(IDB_BITMAP_TALKBACK_CLICK, IDB_BITMAP_TALKBACK_CLICK, IDB_BITMAP_TALKBACK_FOCUS);
		} else {
			//m_buttonSilence.LoadBitmaps(IDB_BITMAP_NOTALKBACK_CLICK, IDB_BITMAP_NOTALKBACK_NORMAL, IDB_BITMAP_NOTALKBACK_FOCUS);
			m_buttonSilence.LoadBitmaps(IDB_BITMAP_NOTALKBACK_NORMAL, IDB_BITMAP_NOTALKBACK_CLICK, IDB_BITMAP_NOTALKBACK_FOCUS);
		}
	}
	else if (2 == iSkinType)
	{
		if (bState)
		{
			m_buttonSilence.LoadBitmaps(IDB_BITMAP_TALKBACK_FOCUS_WHITE, IDB_BITMAP_TALKBACK_FOCUS_WHITE, IDB_BITMAP_TALKBACK_FOCUS_WHITE);
		} else {
			//m_buttonSilence.LoadBitmaps(IDB_BITMAP_NOTALKBACK_CLICK, IDB_BITMAP_NOTALKBACK_NORMAL, IDB_BITMAP_NOTALKBACK_FOCUS);
			m_buttonSilence.LoadBitmaps(IDB_BITMAP_NOTALKBACK_NORMAL_WHITE, IDB_BITMAP_NOTALKBACK_FOCUS_WHITE, IDB_BITMAP_NOTALKBACK_FOCUS_WHITE);
		}
	}



// 	switch(pVideoBar->GetTalkbackSoundState())
// 	{
// 	case BOTTOMBAR_TALKBACK_CLOSE:
// 		if (bState)
// 		{
// 			m_buttonSilence.LoadBitmaps(IDB_BITMAP_NOTALKBACK_NORMAL, IDB_BITMAP_NOTALKBACK_CLICK, IDB_BITMAP_NOTALKBACK_FOCUS);
// 		} else {
// 			m_buttonSilence.LoadBitmaps(IDB_BITMAP_NOTALKBACK_CLICK, IDB_BITMAP_NOTALKBACK_NORMAL, IDB_BITMAP_NOTALKBACK_FOCUS);
// 		}
// 
// 		break;
// 	case BOTTOMBAR_TALKBACK_OPEN:
// 		if (TRUE == bState)
// 		{
// 			m_buttonSilence.LoadBitmaps(IDB_BITMAP_TALKBACK_CLICK, IDB_BITMAP_TALKBACK_CLICK, IDB_BITMAP_TALKBACK_FOCUS);
// 		} else {
// 			m_buttonSilence.LoadBitmaps(IDB_BITMAP_TALKBACK_CLICK, IDB_BITMAP_TALKBACK_CLICK, IDB_BITMAP_TALKBACK_FOCUS);
// 		}
// 
// 		break;
// 	default:
// 		break;
// 	}

	m_buttonSilence.Invalidate(FALSE);
}

void CVideoSoundCtrl::SetSliderPosition(ULONG ulPos)
{
	m_ulSliderPosition = ulPos;
}

// ���Ϳ���
ULONG CVideoSoundCtrl::GetSoundCtrlType()
{
	return m_ulWndType;
}//lint !e1762

void CVideoSoundCtrl::SetSoundCtrlType(ULONG ulType)
{
	if(SOUNDCTRL_WND_TYPE_MAX <= ulType)
	{
        BP_RUN_LOG_ERR(IVS_FAIL,"","set sound type err, type value err code:%lu", ulType)
		return;
	}
	m_ulWndType = ulType;
}

int CVideoSoundCtrl::GetSkinType()
{
	CHECK_POINTER(m_pRealTimeToolBar,1);
	CTranVideoRealtimeBar *pRealTimeToolBar = dynamic_cast<CTranVideoRealtimeBar*>(m_pRealTimeToolBar);
	CHECK_POINTER(pRealTimeToolBar,1);
	CVideoPane * pVideoPane = dynamic_cast<CVideoPane*>(pRealTimeToolBar->GetVideoPane());
	CHECK_POINTER(pVideoPane,1);
	CIVS_OCXPlayerCtrl *pCtrl = pVideoPane->GetOCXPlayerCtrl();
	CHECK_POINTER(pCtrl,1);

	return static_cast<int>(pCtrl->g_ulSkinType);//lint !e1705
}


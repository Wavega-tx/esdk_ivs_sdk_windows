// VideoPane.cpp : implementation file
//
#include "stdafx.h"
#include "VideoPane.h"
#include "VideoPaneMgr.h"
#include "IVS_OCXPlayerCtrl.h"
#include "TranVideoRealtimeBar.h"
#include "TranAnalysisToolBar.h"
#include "TranVideoTitleBar.h"
#include "PostEventXML.h"
#include "GlobalIAMessageMgr.h"
#include "OCXDrawXMLProcess.h"
#include "OCXCapturePictureXMLProcess.h"
#include "OCXPtzControlXMLProcess.h"
#include "OCXRealPlayXMLProcess.h"
#include "IVS_Trace.h"
#include "GlobalPlayMgr.h"

#ifdef DEBUG
#pragma comment (lib, "IVS_IADrawd.lib")
#else
#pragma comment (lib, "IVS_IADraw.lib")
#endif

// �жϻ�ͼ��༭��ʼ�Ƿ�ɹ�
#define CHECK_DRAW_START_SUCCESS(s)\
	do { if(IVS_SUCCEED == s) {m_bIsDrawing = true;} \
		else {m_ulGraphicState = GRAPHICS_STATE_EDITING;}}while(0)

// CVideoPane dialog
#define MAX_CREATE_BAR_COUNT 3
IMPLEMENT_DYNAMIC(CVideoPane, CDialog)

CVideoPane::CVideoPane(CWnd* pParent )
	: CDialog(CVideoPane::IDD, pParent)
    , m_pVideoPaneMgr(nullptr)
    , m_pOcxCtrl(nullptr)
    , m_ulPaneID(0)
    , m_ulIsActivate(PANE_STATE_NOT_ACTIVATE)
    , m_ulPlayState(VIDEO_PLAY_STATE_NOT_PLAYING)
    , m_ulWarnCounter(0)
    , m_ulMaxWarnCount(0)
    , m_bIsSoundOn(false)
    , m_ulAudioVolume(0)
    , m_pRealTimeTitleBar(nullptr)
    , m_pAnalysisToolBar(nullptr)
    , m_pRealTimeToolBar(nullptr)//lint !e1729
	, m_pTVWallStatusBar(nullptr)
	, m_pTurnCruisePane(nullptr)
	, m_pDigtalDlg(nullptr)
	, m_pZoomDirctDlg(nullptr)
	, m_pZoomBar(nullptr)
	, m_pZoomTitleBar(nullptr)
	, m_pPlayTimerBar(nullptr)
	, m_ulIsDrag(1)
	, m_ulIsStopButtonDrag(false)
    , m_isFirstEnter(false)
    , m_ulDisplayScale(0)
    , m_3DPTZStartPoint(0)
    , m_ulIs3DPTZOpen(false)
    , m_ulIsRealtimeRecording(0)
    , m_ulIsRealtimeCapturing(0)
    , m_ulIsRealtimeLabling(0)
    , m_ulIsRealtimeTalkbacking(0)
    , m_ulIsRealtimeSounding(0)
    , m_ulIsRealtimeMagnifying(0)
    , m_ulIsRealtimePtzing(0)
    , m_ulIsRealtimeAlarming(0)
    , m_ulIsRealtimeWalling(0)
    , m_ulIsRealtimeAnalysising(0)
    , m_ulIsRealtimeBarShowing(0)
    , m_ulIsRecordPlaying(0)
    , m_ulIsRecordCaptureing(0)
    , m_ulIsRecordLabeling(0)
    , m_ulIsRecordSounding(0)
    , m_ulIsRecordMagnifying(0)
    , m_ulIsRecordWalling(0)
    , m_ulIsRecordAnalysising(0)
    , m_ulIsRecordBarShowing(0)
    , m_ulIsAnalysisBarShowing(0)
	, m_bSyncMode(false)
	, m_bIsTurnCruise(false)
	, m_ulTurnCruiseStyle(0)
	, m_ulTypeWnd(TYPE_WND_NOMAL)
	, m_ulInstantReplayStyle(0)
	, m_ulAllBarStatus(1)
    , m_ulIsReplaySounding(0) //lint !e1729
    , m_bIsAnalysisDrawing(false)
    , m_ulCursorState(0)
    , m_startPoint(0)
    , m_lastPoint(0)
    , m_bIsMoving(false)
    , m_bIsButtonDown(false)
    , m_bIsFirst(false)
    , m_bIsMovable(false)
    , m_ulWndType(WND_TYPE_NORMAL)
    , m_ulClientType(CLIENT_PC)
    , m_bIsLocalRecord(false)
	, m_bLocalRecordTrigger(false)
    , m_bIsPlatRecord(false)
    , m_bIsPuRecord(false)
    , m_bIsResume(false)
	, m_bIsTvWallBg(false)
	, m_bIsTvWall(false)
    , m_ulIsWndLayoutChange(0)
    , m_ulCameraType(0)
    , m_bIsTurning(false)
    , m_bLastSound(false)
    , m_bStartPlayState(false)
    , m_ulPlayHandle(0)
    , m_enServiceType(SERVICE_TYPE_REALVIDEO)
	, m_ulCurrentGraphicType(0)
	, m_ulCurrentGraphicGroup(0)
	, m_ulGraphicState(0)
	, m_bIsDrawing(false)
	, m_ulGroupDrawMaxNum(0)
    , m_bDrawEnabled(false)
	, m_bOverlayEnabled(false)
	, m_ulDrawHandle(0)
	, m_ulCursorType(CURSOR_TYPE_ARROW)
	, m_ulOverlayHandle(0)
	, m_ulRequestID(0)
	, m_CaptureThreadHandle(NULL)
	, m_bCaptureThreadRun(false)
	, m_iSessionID(0)
    , m_hPaneBKGBitmap(nullptr)
    , m_hPaneALARMBitmap(nullptr)
	, m_nZoomWidth(0)
	, m_nZoomHeigth(0)
	, m_uiStreamType(0)
	, m_MagnifyNum(0)//lint !e1729
	, m_pMagnifySrcPane(NULL)
	, m_ulIsDstMagnify(0)//lint !e1729
	, m_ulPrvePlayState(VIDEO_PLAY_STATE_NOT_PLAYING)
	, m_ulPrvePlayHandle(0)
	, m_ulPrveTypeWnd(TYPE_WND_NOMAL)
	, m_DrawHeight(0)
	, m_DrawWidth(0)
	, m_ResumeMagnifyNum(0)//lint !e1729
	, m_ulInstanstRealPlayHandle(0) //lint !e1729
	, m_ulInstanstPlayBackHandle(0)
	, m_bIsInstanstPlayBack(FALSE)
	, m_ulToolBarStatus(0x3fff)
	, m_bIsInstansPlayPause(FALSE)
	, m_bOpendMagnifyStatus(false)
	, m_bOpend3DPTZStatus(false)
	, m_DrawMoveSuccessSum(1)// 1ֵ
	, m_ullWinAlarmFlag(1)//��ʼʱ�澯��ť����
	, m_bIsMoveVideoWindow(FALSE)
	, m_bIsHideTranslucentBar(FALSE)
	, m_bIsBarCreated(false)
	, m_ulPrveTurnCruiseStyle(0)//lint !e1729
	, m_bIsRefresheed(false)
	, m_ulTVBarStatus(TVWALL_BAR_STAUS_HIDE)
	, m_ulShowCruiseTrackStatus(0)
	, m_ulShowAlarmStatus(0)
	, m_ulShowRecordStatus(0)
	, m_ulShowSingleFrameStatus(0)
    , m_uCmdSeq(0)//lint !e1729
	, m_hWorkThread(NULL)
    , m_ThreadState(THREAD_EXITING)
{
    memset(m_szCameraID,0x0, IVS_DEV_CODE_LEN + 1);
    memset(m_szToolbarStatus,0x0,sizeof(m_szToolbarStatus));
	memset(&m_CurrentGraphic,0x0,sizeof(IA_GRAPHICS));
	memset(m_CamReal, 0, sizeof(m_CamReal));
	memset(&m_LocalCaptureParam, 0, sizeof(m_LocalCaptureParam));
	memset(m_MagnifyNameCount,0,sizeof(m_MagnifyNameCount));
	memset(m_szGraphics,0,MAGNIFY_MAX_NUM* sizeof(IA_GRAPHICS));
	m_IAColor.Alpha = 255;
	m_IAColor.Blue = 246;
	m_IAColor.Red = 41;
	m_IAColor.Green = 154 ;
	m_pTVWallOfflineImage = nullptr;
	m_pTVWallOnlineImage = nullptr;
	m_bIsShowTVWallBar = false;
	memset(&m_IArectPercent,0,sizeof(IVS_RECT));
	memset(&m_MagnifyLastPoint,0,sizeof(POINT));
	memset(&m_szOpendMagnifySize,0,sizeof(SIZE));
}

CVideoPane::~CVideoPane()
{
	try
	{
		if (NULL != m_CaptureThreadHandle)
		{
			WaitForSingleObject(m_CaptureThreadHandle, INFINITE);
			CloseHandle(m_CaptureThreadHandle);
			m_CaptureThreadHandle = NULL;
		}
	}
	catch(...)
	{//lint !e1775

	}

	IVS_DELETE(m_pRealTimeToolBar); //lint !e1551
	IVS_DELETE(m_pRealTimeTitleBar);	//lint !e1551
    IVS_DELETE(m_pAnalysisToolBar);//lint !e1551
	m_pAnalysisToolBar = nullptr;
	IVS_DELETE(m_pTVWallStatusBar);//lint !e1551
	IVS_DELETE(m_pTurnCruisePane); //lint !e1551
	IVS_DELETE(m_pZoomBar); //lint !e1551
	IVS_DELETE(m_pZoomTitleBar); //lint !e1551
	IVS_DELETE(m_pZoomDirctDlg); //lint !e1551
	try
	{
		DeleteTimerBar();
	}
	catch (...)
	{
		;
	}
	
    m_pVideoPaneMgr = nullptr;
    m_pOcxCtrl = nullptr;
    DeleteObject(m_hPaneBKGBitmap);
    DeleteObject(m_hPaneALARMBitmap);
	m_bCaptureThreadRun = false;
	if (NULL != m_ulDrawHandle)
	{
		DRAW_HANDLE hDrawHandle = (DRAW_HANDLE *)m_ulDrawHandle;
		(void)Draw_FreeHandle(hDrawHandle);
	}
	if (NULL != m_ulOverlayHandle)
	{
		IATRACK_HANDLE hOverlayHandle = (IATRACK_HANDLE *)m_ulOverlayHandle;
		(void)IATrack_FreeHandle(hOverlayHandle);
	}

}   //lint !e1740 m_pVideoPaneMgr m_pOcxCtrl ֻ��ָ�������

void CVideoPane::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_VIDEO, m_Video);
}

BEGIN_MESSAGE_MAP(CVideoPane, CDialog)
    ON_WM_CONTEXTMENU()
    ON_WM_CREATE()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
    ON_WM_PAINT()
    ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
    ON_WM_SETCURSOR()
    ON_WM_SIZE()
    ON_WM_TIMER()
    ON_WM_MOVE()
    ON_WM_MOVING()
	ON_MESSAGE(WM_SYNC_PLAYBACK_MSG, OnSyncPlayBackMsg)
	ON_MESSAGE(WM_MAGNIFY_RECT_MOVE, OnMagnifyRectMove)
	ON_MESSAGE(WM_MAGNIFY_RATE_CHANGE, OnMagnifyRateChange)
	ON_MESSAGE(WM_MAGNIFY_CLOSE, OnMagnifyClose)
	ON_MESSAGE(WM_MAGNIFY_CLOSE_ALL, OnMagnifyCloseAll)
	ON_MESSAGE(WM_TIMERBAR_TIME_MOVE,OnInstantReplayMove)
	ON_MESSAGE(WM_TURNCRUISE_CLOSE,OnDeleteTurnCruise)
    ON_MESSAGE(WM_TIMERBAR_START_PLAY, OnInstantReplayStart)
	ON_MESSAGE(WM_TIMERBAR_PLAYBACK_STOP, onInstantReplaySegStop)
	ON_MESSAGE(WM_RESUME_REALPLAY_OK_MSG, OnResumeRealPlayOKMsg)
	ON_MESSAGE(WM_ASYNC_REALPLAY_MSG, OnAsyncStreamMsg)
	ON_MESSAGE(WM_ASYNC_EVENT_MSG,   OnAsyncEventMsg)

END_MESSAGE_MAP()

// CVideoPane message handlers

void CVideoPane::BeginPaneDrag(void)const
{
    CHECK_POINTER_VOID(m_pVideoPaneMgr);

    //���񲼾�ֻ��һ�Ͳ���Ҫ����
    if(MAGNIFY == m_pVideoPaneMgr->GetMagnifyState())
    {
        return;
    } else {
        ULONG ulWndLayout = m_pVideoPaneMgr->GetNumOfDisplayPane();
        if (WIND_LAYOUT_1_1 == ulWndLayout)
        {
            return;
        }
    }

    CParentWnd* pParentWnd = dynamic_cast <CParentWnd*>(GetParent());
    CHECK_POINTER_VOID(pParentWnd);
    pParentWnd->SetCapture();
    pParentWnd->SetIsDrag(true);
	pParentWnd->StartDrag();
	//CVideoPaneMgr::CheckIfCanExchange �ڴ˷�����Ӧ
	if(PTZ_3D_OPEN == Get3DPTZState() 
		|| BOTTOMBAR_PTZ_OPEN == GetRealtimePtzState()
		|| MAGNIFY_OPEN == GetRealtimeMagnifyState())
	{
		pParentWnd->SetDisableDrag(true);
	}
}

void CVideoPane::ChangePaneSize(void)const
{
    //ULONG ulVideoWidth = 0;
    //ULONG ulVideoHeight = 0;
    //TCHAR pDstCode[32];
    //_itoa((int)(GetVideoWnd()),pDstCode,10);
    ////if (CIVS_PlayerAdapter::instance()->GetVideoResolution(pDstCode,&ulVideoWidth,&ulVideoHeight) == IVS_OCX_RET_OK)
    //{
    //    CRect paneRect;
    //    CRect oldPaneRect;
    //    GetPaneRect(&paneRect);
    //    GetPaneRect(&oldPaneRect);

    //    //ԭ��Ⱥ�ԭ�߶�
    //    const int lOldWidth  = paneRect.Width();
    //    const int lOldHeight = paneRect.Height();

    //    //�ü���Ŀ��
    //    int lCutWidth = lOldWidth;

    //    //�ü���ĸ߶�
    //    int lCutHeight = lOldHeight;

    //    lCutWidth = static_cast<int>(static_cast<ULONG>(lOldHeight) * ulVideoWidth / ulVideoHeight);
    //    lCutHeight = static_cast<int>(static_cast<ULONG>(lOldWidth) * ulVideoHeight / ulVideoWidth);

    //    //�������ұ߽�ֵ
    //    if (0 < lOldWidth - lCutWidth)
    //    {
    //        paneRect.left += (lOldWidth - lCutWidth) / 2;
    //        paneRect.right = paneRect.left + lCutWidth;
    //    } else if (0 < lOldHeight - lCutHeight) {
    //        //�������±߽�ֵ
    //        paneRect.top   += (lOldHeight - lCutHeight) / 2;
    //        paneRect.bottom = paneRect.top + lCutHeight;
    //    }

    //    MoveWindow(&paneRect);
    //    GetParent()->InvalidateRect(&oldPaneRect);
    //    (void)MoveEagleEye();
    //}
}

// �����������͵ײ�������
void CVideoPane::CreateBar(void)
{
	// �����������������ܷ������������ڶ���Ҫ����������
	if (m_bIsBarCreated)
	{
		return;
	}

    if (WND_TYPE_ANALYSIS != m_ulWndType)
    {
        CreateRealTimeBar();
        m_bIsBarCreated = true;
    }
    
	
	//else
 //   {
 //       CreateAnalysisBar();
 //   }
}

void CVideoPane::CreateBarAnalysisBar()
{
	if (WND_TYPE_ANALYSIS == m_ulWndType)
	{
		CreateAnalysisBar();
		EnableALLButton(FALSE);
	}
}

void CVideoPane::DeleteSoundCtrl( const CPoint &point ) const
{
    CHECK_POINTER_VOID(m_pRealTimeToolBar);

    CPoint _pTemp = point;
    ClientToScreen(&_pTemp);
    long lLeft = static_cast<long>(m_pRealTimeToolBar->GetSoundCtrlLeft());
    long lTop = static_cast<long>(m_pRealTimeToolBar->GetSoundCtrlTop());
    if ((_pTemp.x < lLeft - 10)
        || (_pTemp.x > lLeft + 40)
        || (_pTemp.y < lTop - 10))
    {
        m_pRealTimeToolBar->DeleteSoundCtrl();
    }
}

void CVideoPane::DeleteTalkbackSound( const CPoint &point ) const
{
    CHECK_POINTER_VOID(m_pRealTimeToolBar);

    CPoint _pTemp = point;
    ClientToScreen(&_pTemp);
    long lLeft = static_cast<long>(m_pRealTimeToolBar->GetSoundCtrlLeft());
    long lTop = static_cast<long>(m_pRealTimeToolBar->GetSoundCtrlTop());
    if ((_pTemp.x < lLeft - 10)
        || (_pTemp.x > lLeft + 40)
        || (_pTemp.y < lTop - 10))
    {
        m_pRealTimeToolBar->DeleteTalkbackCtrl();
    }
}

void CVideoPane::OnContextMenu(CWnd* pWnd, CPoint point)
{
    // �Ҽ���������һ���ǻ����ĸı䣬һ���ǵ����˵�
    // ���ù���������û������
	CHECK_POINTER_VOID(m_pVideoPaneMgr);
    m_pVideoPaneMgr->SetActivatePane(m_ulPaneID);

    //��win7����߰汾�� ��wm_paint��Ϣ��ǰ����
    //if (CUtility::CheckIsWin7())
    //{
    //    Invalidate(FALSE);
    //}

    CPoint mousePos;
    (void)GetCursorPos(&mousePos);
	
	if (m_ulPlayState != VIDEO_PLAY_STATE_NOT_PLAYING)
	{			
		if (m_TitleBarRect.PtInRect(mousePos))
		{
			return;
		}
	}
    //�ϱ�����Ҽ�����¼�
	GetOCXPlayerCtrl()->PostRightMouseClickEventMsg(0, m_ulPaneID, static_cast<ULONG>(mousePos.x), static_cast<ULONG>(mousePos.y));
    CDialog::OnContextMenu(pWnd,point);
} //lint !e1746  MFC�Զ����ɵĵĺ���

int CVideoPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialog::OnCreate(lpCreateStruct) == -1)
        return -1;

    return IVS_SUCCEED;
}

void CVideoPane::SetBackGroundBitmap(const CRect &rect)
{
    if (m_PaneRect.EqualRect(rect))
    {
        return;
    }

    double scale = ((double)rect.Height()) / ((double)rect.Width());
    if (0.75 <= scale)
    {
        m_nZoomWidth  = rect.Width();
        m_nZoomHeigth = rect.Width() * 3 / 4;
    } else {
        m_nZoomWidth  = rect.Height() * 4 / 3;
        m_nZoomHeigth = rect.Height();
    }
}

// ˫����ʵ�ֱ���
BOOL CVideoPane::OnEraseBkgnd(CDC* pDC)
{
	if (m_ulPlayState == VIDEO_PLAY_STATE_REALTIME || 
		m_ulPlayState == VIDEO_PLAY_STATE_VOD ||
		m_ulPlayState == VIDEO_PLAY_STATE_LOCALFILE)
	{
		return TRUE;
	}

    CHECK_POINTER(m_pVideoPaneMgr,FALSE);
    CRect _Rect;
    GetWindowRect(&_Rect);

    int nWidth = 0;
    int nHeight = 0;

    nWidth  = _Rect.Width();
    nHeight = _Rect.Height();

#if BACKGROUND_CHANGE
    //////////////////////////////////////////////////////////////////////////
    // ��䱳��ɫ
#if 0    
	CDC _MemeryDC;
    CBitmap _MemBitmap;

    (void)_MemeryDC.CreateCompatibleDC (NULL);
    (void)_MemBitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight);

    (void)_MemeryDC.SelectObject(&_MemBitmap);
    _MemeryDC.FillSolidRect(0, 0, nWidth, nHeight, m_pVideoPaneMgr->m_VideoPaneBKGColor);
#endif 
    //////////////////////////////////////////////////////////////////////////
    // ���Ʊ���ͼ

	pDC->FillSolidRect(0, 0, nWidth, nHeight, m_pVideoPaneMgr->m_VideoPaneBKGColor);

	using namespace Gdiplus;

	HDC hdc = pDC->GetSafeHdc();
    Graphics graphics(hdc);
    CParentWnd *pParentWnd = dynamic_cast<CParentWnd *>( GetParent());
    CHECK_POINTER(pParentWnd,FALSE);
    Gdiplus::Image * pImage = nullptr;
    if (BOTTOMBAR_ALARM_CLOSE == GetRealtimeAlarmState())
    {
        pImage = SkinMgr.GetImagePaneBKG();
    } else {
        pImage = SkinMgr.GetImagePaneALARM();
    }

    float _fWidth = static_cast<float>(pImage->GetWidth());
    float _fHeight = static_cast<float>(pImage->GetHeight());
    float _fZoomRatio =  static_cast<float>(m_nZoomWidth) / 800 ;

    graphics.DrawImage(pImage,
        static_cast<float>((nWidth - m_nZoomWidth)/2  + (( 800 - _fWidth) /2) * _fZoomRatio) ,      //lint !e653
        static_cast<float>((nHeight - m_nZoomHeigth)/2  +(( 600 - _fHeight) /2) *_fZoomRatio) ,     //lint !e653
        (_fWidth * m_nZoomWidth ) / 800,                                                            //lint !e653
        (_fHeight * m_nZoomHeigth ) / 600                                                           //lint !e653
        );

    graphics.ReleaseHDC(hdc);

	//(void)pDC->BitBlt(0, 0, nWidth , nHeight , &_MemeryDC, 0, 0, SRCCOPY);
    //(void)_MemBitmap.DeleteObject();
    //(void)_MemeryDC.DeleteDC();
#endif
    //// ����Ϊ������ɫ //////////////
#if 0
    if (VIDEO_PLAY_STATE_NOT_PLAYING != m_ulPlayState &&
		VIDEO_PLAY_STATE_RESTARTING  != m_ulPlayState)
    {
        GetClientRect(_Rect);
        pDC->FillSolidRect(_Rect, pVideoPaneMgr->m_VideoPaneBKGColor);
        return TRUE;
    }

    CDC MemDC;
    CBitmap MemBitmap;

    (void)MemDC.CreateCompatibleDC (NULL);
    (void)MemBitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight);

    (void)MemDC.SelectObject(&MemBitmap);
    MemDC.FillSolidRect(0, 0, nWidth, nHeight, pVideoPaneMgr->m_VideoPaneBKGColor); //lint !e845 !e835

    //// ����Ϊ������ͼ //////////////
   // GetClientRect(rect);
    BITMAP bm;

    CDC dcMem;
    (void)dcMem.CreateCompatibleDC(pDC);
    CBitmap *pOldBMP = nullptr;
    CBitmap * tempBmp;
    if (BOTTOMBAR_ALARM_CLOSE == GetRealtimeAlarmState())
    {
        tempBmp = CBitmap::FromHandle(m_hPaneBKGBitmap);
    } else {
        tempBmp = CBitmap::FromHandle(m_hPaneALARMBitmap);
    }

    if (NULL == tempBmp)
    {
        pDC->FillSolidRect(_Rect, pVideoPaneMgr->m_VideoPaneBKGColor); //lint !e845 !e835
        (void)MemBitmap.DeleteObject();
        (void)MemDC.DeleteDC();
        return TRUE;
    }

    VERIFY(tempBmp->GetObject(sizeof(bm), (LPVOID)&bm));
    pOldBMP = ( CBitmap *)dcMem.SelectObject(tempBmp);
    (void)MemDC.BitBlt((_Rect.right - bm.bmWidth) / 2,
        (_Rect.bottom - bm.bmHeight) / 2,
        bm.bmWidth, bm.bmHeight, &dcMem, 0, 0, SRCCOPY);

    (void)dcMem.SelectObject(pOldBMP);
    (void)pDC->BitBlt(0, 0, nWidth, nHeight, &MemDC, 0, 0, SRCCOPY);

    (void)MemBitmap.DeleteObject();
    (void)MemDC.DeleteDC();
    (void)dcMem.DeleteDC();
#endif // 0

     //// ����Ϊ������ͼ //////////////

    //�Ƿ���ǽ
	if (m_bIsTvWall && !m_bIsResume)
	{
		if (OCX_SKIN_TYPE_BLACK == m_pVideoPaneMgr->GetSkinType())
		{
			pDC->SetTextColor(RGB(255, 255, 255));
			pDC->SetBkColor(RGB(0, 0, 0));//lint !e835 !e845
		}
		else
		{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkColor(RGB(255, 255, 255));//lint !e835 !e845
		}

		std::string strTemp;
		CString cstTemp = m_strDevName.c_str();

		if (m_bIsTvWallBg)
		{
            if (!strTemp.empty())
            {
                cstTemp += ",";
            }

			if (!CI18N::GetInstance().GetResourceLanguage("TvWalling", strTemp))
			{
				cstTemp += "Be on TvWalling";
			} 
			else 
			{
				cstTemp += strTemp.c_str();
			}
		}

        //��ȡ��ǰ�������Ϣ
        TEXTMETRIC tmFont;
        (void)GetTextMetrics(pDC->m_hDC,&tmFont);

        //��ȡ��ǰ���ڵĴ�С
        RECT rc;
		memset(&rc,0,sizeof(RECT));
        this->GetClientRect(&rc);
        int iCharCount = cstTemp.GetLength(); //�ַ�����
        int iTotleLen = iCharCount * tmFont.tmAveCharWidth; //�ܳ���
        int iRows = iTotleLen / (rc.right - rc.left) + 1; //ȫ����ʾ��Ҫ������
        rc.top += rc.bottom/2;
        rc.top -= iRows/2*tmFont.tmHeight;//lint !e834
        rc.top = (rc.top <= 0 ? 0 : rc.top);
        pDC->SetBkMode(TRANSPARENT);
        pDC->DrawText(cstTemp, &rc, DT_NOCLIP|DT_WORDBREAK|DT_CENTER);
	}

    // ��ʾ�Զ�����
    if (m_bIsResume)
    {
		if (OCX_SKIN_TYPE_BLACK == m_pVideoPaneMgr->GetSkinType())
		{
			pDC->SetTextColor(RGB(255, 255, 255));
			pDC->SetBkColor(RGB(0, 0, 0));//lint !e835 !e845
		}
		else
		{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkColor(RGB(255, 255, 255));//lint !e835 !e845
		}

		std::string strTemp;
        if (!CI18N::GetInstance().GetResourceLanguage("ReConnecting", strTemp))
        {
            CString cstTemp = "ReConnecting...";
            (void)pDC->TextOut(nWidth/2 - 40, nHeight/2 - 16, cstTemp);
        } else {
            (void)pDC->TextOut(nWidth/2 - 40, nHeight/2 - 16, strTemp.c_str());
        }
    }

    //InvalidateBar();
    return TRUE;
}//lint !e1762

BOOL CVideoPane::OnInitDialog()
{
    CDialog::OnInitDialog();

	if (m_pVideoPaneMgr == nullptr)
	{
		CIVS_OCXPlayerCtrl * pOcx =  GetOCXPlayerCtrl();
		if(pOcx)
		{
			m_pVideoPaneMgr = pOcx->GetVideoPaneMgr();
		}		
	}
    return TRUE;
}

void CVideoPane::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if ((CURSOR_STATE_PTZ == m_ulCursorState)
        && (FALSE == IsInCenter(point)) || (MAGNIFY_OPEN == m_ulIsRealtimeMagnifying))
    {
        return;
    }

    // ���ù������ķ�����ȷ���ǷŴ�����С����
	CIVS_OCXPlayerCtrl* pCtrl = GetOCXPlayerCtrl();
	CHECK_POINTER_VOID(pCtrl);
    CVideoPaneMgr * pVideoPaneMgr = pCtrl->GetVideoPaneMgr();
    if (NULL != pVideoPaneMgr)
    {
        pVideoPaneMgr->ChangeSize(m_ulPlayState);
    }
}//lint !e715

void CVideoPane::GetDisplayScaleRect( CRect &mainRect )
{
    m_Video.GetClientRect(&mainRect);
	// �ж�״̬�������ȫ��������Ͳ����ٲ����ˣ�add by z00193167
	if (m_pVideoPaneMgr->GetDispalyScale() == DISPLAY_SCALE_FULL)
	{
		return;
	}

    CIVS_OCXPlayerCtrl *pOCXPCtrl = GetOCXPlayerCtrl();
	
    CHECK_POINTER_VOID(pOCXPCtrl);

    IVS_MEDIA_INFO strMediaInfo;
    CPoint pointXY ;
    memset(&strMediaInfo,0,sizeof(IVS_MEDIA_INFO));

    (void)IVS_SDK_GetMediaInfo(pOCXPCtrl->GetSDKSessionID(),GetPlayHandle(), &strMediaInfo);

    IVS_FLOAT fRatio = static_cast<IVS_FLOAT>(strMediaInfo.uiVideoWidth) / static_cast<IVS_FLOAT>(strMediaInfo.uiVideoHeight);
    IVS_FLOAT uiMargin = 0;
    IVS_FLOAT fRectRatio  = static_cast<IVS_FLOAT>(mainRect.Width()) / static_cast<IVS_FLOAT>(mainRect.Height());
    if (fRectRatio > fRatio)
    {
        uiMargin = (mainRect.Width()- strMediaInfo.uiVideoWidth*(static_cast<IVS_FLOAT>(mainRect.Height()) /static_cast<IVS_FLOAT>(strMediaInfo.uiVideoHeight)))/2;
        mainRect.left += static_cast<long>(uiMargin);
        mainRect.right -= static_cast<long>(uiMargin);
    } else if (fRectRatio < fRatio) {
        uiMargin = (mainRect.Height()- strMediaInfo.uiVideoHeight*(static_cast<IVS_FLOAT>(mainRect.Width()) /static_cast<IVS_FLOAT>(strMediaInfo.uiVideoWidth)))/2;
        mainRect.top += static_cast<long>(uiMargin);
        mainRect.bottom -= static_cast<long>(uiMargin);
    }
}

BOOL CVideoPane::PointIsVideoRect( const CPoint &point )
{
    if (m_pVideoPaneMgr->GetDispalyScale() == DISPLAY_SCALE_FULL)
    {
        return TRUE;
    }
    CRect mainRect;
    GetDisplayScaleRect(mainRect);
    return mainRect.PtInRect(point);
}

void CVideoPane::PTZ3DMouseDown( const CPoint &point )
{
    if (!PointIsVideoRect(point))
    {
        return;
    }
    m_bIsButtonDown = true;

	CIVS_OCXPlayerCtrl *pOCXPCtrl = GetOCXPlayerCtrl();
	CHECK_POINTER_VOID(pOCXPCtrl);
	if (IVS_SUCCEED != pOCXPCtrl->IVS_OCX_EnableDraw(m_ulPaneID))
	{
		return;
	}
    CHECK_POINTER_VOID(m_ulDrawHandle);
    UINT scale = m_pVideoPaneMgr->GetDispalyScale();
    BOOL bScaleModeFull = scale == DISPLAY_SCALE_FULL ? TRUE : FALSE;
    (VOID)Draw_SetDisplayScaleMode((DRAW_HANDLE)m_ulDrawHandle, bScaleModeFull);

    m_startPoint    = point;
    m_lastPoint     = point;
    m_bIsButtonDown = true;
    SetCapture();

    IA_COLOR ptz_3dcolor;
    memset(&ptz_3dcolor,0x0,sizeof(IA_COLOR));
    ptz_3dcolor.Alpha = 255;
    ptz_3dcolor.Blue = 246;
    ptz_3dcolor.Red = 41;
    ptz_3dcolor.Green = 154 ;

    IA_GRAPHICS ptz_3dgraphics ;
    memset(&ptz_3dgraphics,0x0,sizeof(IA_GRAPHICS));
    ptz_3dgraphics.ulLineWidth = 2;
    ptz_3dgraphics.LineColor = ptz_3dcolor;
    ptz_3dgraphics.ulGraphicType = IA_GRAPHICS_TYPE_RECTANGLE;
    ptz_3dgraphics.IAPoints[0].x  = m_startPoint.x;
    ptz_3dgraphics.IAPoints[0].y  = m_startPoint.y;
	//modify by limingjie 2013-6-14  ��ʱһ���ûص��������ݸ���,����ɾ
	Sleep(50);
    (void)Draw_RectangleStart((DRAW_HANDLE)m_ulDrawHandle,1,&ptz_3dgraphics);
}

void CVideoPane::OnLButtonDown(UINT nFlags, CPoint point)
{
    SetActivePane();
    CHECK_POINTER_VOID(m_pVideoPaneMgr);
    // ɾ���������ƶԻ���
    m_pVideoPaneMgr->DeleteSoundCtrl();

    switch (m_ulWndType)
    {
    case WND_TYPE_REALTIME:
        if(PTZ_3D_OPEN == m_ulIs3DPTZOpen)
        {
            PTZ3DMouseDown(point);
        }
		else if (MAGNIFY_OPEN == m_ulIsRealtimeMagnifying)
		{
			MagnifyMouseDown(point);
			SetCapture();
		}
		else if (CURSOR_STATE_PTZ == m_ulCursorState) {
            // ����ʵ������������̨���ƹ���
            m_bIsButtonDown = true;
            SetPtzCursor(point);
            SetPtzControl(point);
            SetCapture();
        } else {
			if (m_ulIsDrag)
			{
				if (!m_ulIsStopButtonDrag)
				{
					BeginPaneDrag(); 
				}
				else
				{
					m_ulIsStopButtonDrag = false;
				}
			}
        }
        break;
    default:
        break;
    }

	// ******��ͼ����******
    if (m_bDrawEnabled)
    {
    	DrawLButtonDown(point);
    }
}//lint !e715

void CVideoPane::OnLButtonUp(UINT nFlags, CPoint point)
{
    CHECK_POINTER_VOID(m_pVideoPaneMgr);

    switch (m_ulWndType)
    {
    case WND_TYPE_REALTIME:
		if (MAGNIFY_OPEN == m_ulIsRealtimeMagnifying)
		{
			MagnifyMouseUp(point);
			ReleaseCapture();
		}
        OnRealtimeLButtonUp(point);

        break;
    default:
        break;
    }
    m_bIsButtonDown = false;

    // ******��ͼ����******
    if (m_bDrawEnabled && m_bIsDrawing)
    {
        DrawLButtonUp(point);
    }
}//lint !e715

// ��ȡͼ�ε����꣬ʹ�ðٷֱȱ�ʾ
IVS_INT32 CVideoPane::GetIARectByGroup(ULONG uiGraphicGroup, CRect& rect)
{
	IVS_DEBUG_TRACE("");
	IVS_INT32 iRet = IVS_FAIL;
	IA_GRAPHICS graphicsTemp;
	memset(&graphicsTemp,0,sizeof(IA_GRAPHICS));
	ULONG ulNum = 1;// ��ȡһ��ͼ��
	iRet = Draw_GetGraphicsByGroup((DRAW_HANDLE)m_ulDrawHandle, uiGraphicGroup, &graphicsTemp, &ulNum);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "GetGraphics falied", "na");
		return iRet;
	}

	GetIARectByGraphics(graphicsTemp, rect);
	return IVS_SUCCEED;
}

// ��ȡͼ�ε��������
void CVideoPane::GetIARectByGraphics(const IA_GRAPHICS& graphicsTemp, CRect& rect)const
{
	rect.left = graphicsTemp.IAPoints[0].x > graphicsTemp.IAPoints[1].x ? graphicsTemp.IAPoints[1].x : graphicsTemp.IAPoints[0].x;
	rect.left = rect.left > graphicsTemp.IAPoints[2].x ? graphicsTemp.IAPoints[2].x : rect.left;
	rect.left = rect.left > graphicsTemp.IAPoints[3].x ? graphicsTemp.IAPoints[3].x : rect.left;

	rect.top = graphicsTemp.IAPoints[0].y > graphicsTemp.IAPoints[1].y ? graphicsTemp.IAPoints[1].y : graphicsTemp.IAPoints[0].y;
	rect.top = rect.top > graphicsTemp.IAPoints[2].y ? graphicsTemp.IAPoints[2].y : rect.top;
	rect.top = rect.top > graphicsTemp.IAPoints[3].y ? graphicsTemp.IAPoints[3].y : rect.top;

	rect.right = graphicsTemp.IAPoints[0].x > graphicsTemp.IAPoints[1].x ? graphicsTemp.IAPoints[0].x : graphicsTemp.IAPoints[1].x;
	rect.right = rect.right > graphicsTemp.IAPoints[2].x ? rect.right : graphicsTemp.IAPoints[2].x;
	rect.right = rect.right > graphicsTemp.IAPoints[3].x ? rect.right : graphicsTemp.IAPoints[3].x;

	rect.bottom = graphicsTemp.IAPoints[0].y > graphicsTemp.IAPoints[1].y ? graphicsTemp.IAPoints[0].y : graphicsTemp.IAPoints[1].y;
	rect.bottom = rect.bottom > graphicsTemp.IAPoints[2].y ? rect.bottom : graphicsTemp.IAPoints[2].y;
	rect.bottom = rect.bottom > graphicsTemp.IAPoints[3].y ? rect.bottom : graphicsTemp.IAPoints[3].y;
}

// ת���ɰٷֱȱ�ʾ
void CVideoPane::TransIARect2Percent(const CRect& rect, CRect& percentRect)
{
	SIZE sz = {0};
	(void)GetMediaSize(sz);

	if ((0 >= sz.cx) || (0 >= sz.cy))
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "Video Size is Zero","NA");
		return;
	}

	percentRect.bottom = (LONG)(rect.bottom*100 / sz.cy);
	percentRect.top = (LONG)(rect.top*100 / sz.cy);
	percentRect.left = (LONG)(rect.left*100 / sz.cx);
	percentRect.right = (LONG)(rect.right*100 / sz.cx);
}

void CVideoPane::OnMouseMove(UINT nFlags, CPoint point)
{
    CRect mainRect;
    m_Video.GetClientRect(&mainRect);
    switch (m_ulWndType)
    {
    case WND_TYPE_REALTIME:
        OnRealtimePaneMouseMove(point, mainRect);
        break;
    default:
        break;
    }

	// ******��ͼ����******
	if (GRAPHICS_STATE_DRAWING == m_ulGraphicState)// ��ͼ״̬
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	}else {
		POINT CurrentPoint;
		CurrentPoint.x = point.x;
		CurrentPoint.y = point.y;
		GetCursorType(CurrentPoint);
		SetCursorType();
	}
    if (m_bDrawEnabled && m_bIsDrawing)
    {
	    DrawMouseMove(point);
    }
}//lint !e715

BOOL CVideoPane::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // BOOL bRet = FALSE;
	// ����˴����Ǳ�ռ��Ϊ�Ŵ�״̬�Ļ����Ϳ�ʼ�Ŵ�
    if(BOTTOMBAR_MAGNIFY_OPEN == m_ulIsDstMagnify)
    {
		if (m_pZoomBar!=NULL)
		{
			// ֱ��ģ�����Ŵ�ť
			if (0 > zDelta) // ���ع�����С
			{
				// ֱ�ӷ���һ����ť��Ϣ���Ŵ󴰿�
				::PostMessage(m_pZoomBar->GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(IDC_BTN_SHRINK, BN_CLICKED), (LPARAM)m_pZoomBar->GetZoomInHwnd());
			} else {        // ��Զ�����Ŵ�
				::PostMessage(m_pZoomBar->GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(IDC_BTN_MAGNIFY, BN_CLICKED), (LPARAM)m_pZoomBar->GetZoomOutHwnd());
			}
		}
		return TRUE;
    }

    // ����ʵ������������̨���ƹ���
    if (CURSOR_STATE_PTZ != m_ulCursorState)
    {
        return FALSE;
    }
    // ��ȡ��̨����
    TCHAR szMode[5] = {0};
    TCHAR szSpeed[5] = {0};
    if (!CToolsHelp::Memcpy(szMode,5,_T("2"), 1))
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"IVS_OCX::","VideoPane Failed");
        return FALSE;
    }

    LONG ulRet = IVS_SUCCEED;
    BSTR b = GetOCXPlayerCtrl()->IVS_OCX_GetLocalPTZConfig();
	char* pb = _com_util::ConvertBSTRToString(b);
	if (NULL == pb)
	{
		::SysFreeString(b);
		return FALSE;
	}
	std::string strXML = pb;
	// �ڴ��ͷ�
	::SysFreeString(b);
	delete[] pb;

    LONG LResult = 0;
    UINT32 lSpeed = 0;

    ulRet = PostEventXMLImpl.GetPTZConfig(strXML,&LResult,&lSpeed);
    if (IVS_SUCCEED != ulRet && LResult != IVS_SUCCEED)
    {
        BP_RUN_LOG_ERR(ulRet,_T("IVS_OCX::"),_T("Get PTZ_Speed value fail, PTZ_Speed use default value."));
        if (!CToolsHelp::Memcpy(szSpeed,5,_T("1"), 1))
        {
            BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"IVS_OCX::","VideoPane.Failed");
            return FALSE;
        }
    }

    TCHAR szCameraCode[IVS_DEV_CODE_LEN] = {0};
    if(!CToolsHelp::Memcpy(szCameraCode,IVS_DEV_CODE_LEN,GetCameraID(),strlen(GetCameraID())))
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"IVS_OCX::","VideoPane .Failed");
    }
	IVS_UINT32 uiLockStatus = 0;
    if (0 > zDelta) // ���ع�����С
    {
        ulRet = IVS_SDK_PtzControl(GetOCXPlayerCtrl()->GetSDKSessionID(),szCameraCode, PTZ_LENS_ZOOM_OUT, szMode, szSpeed, &uiLockStatus);
    } else {        // ��Զ�����Ŵ�
        ulRet = IVS_SDK_PtzControl(GetOCXPlayerCtrl()->GetSDKSessionID(),szCameraCode, PTZ_LENS_ZOOM_IN, szMode, szSpeed, &uiLockStatus);
    }

    if (IVS_SUCCEED != ulRet)
    {
        // ��CU�ϱ��쳣
        SetRealtimePtzState(BOTTOMBAR_PTZ_CLOSE);
        //            m_pRealTimeToolBar->SetPtzPic(BUTTON_NORMAL);
        SetCursorState(CURSOR_STATE_CLOSE);
        // TODO :     m_pRealTimeToolBar->SendPtzResult(ulRet);

        return ulRet;
    }
	
    (void)IVS_SDK_PtzControl(GetOCXPlayerCtrl()->GetSDKSessionID(),szCameraCode, PTZ_STOP, szMode, szSpeed, &uiLockStatus);
    return ulRet;
} //lint !e1746  MFC�Զ����ɵĵĺ���

void CVideoPane::OnPaint()
{
    // �Ӵ���Ϊ��ɫ����
    CPaintDC dc(this);

	// ��ʼ���߿�Ҫ�������Ƿ��ǻ����
    SetBorder(m_ulIsActivate);

    if (m_ulPlayState == VIDEO_PLAY_STATE_NOT_PLAYING ||
		(m_ulPlayState == VIDEO_PLAY_STATE_RESTARTING && GetTypeWnd() != TYPE_WND_INSTANTBACKPLAY))
    {
        m_Video.ShowWindow(SW_HIDE);
    } else {
        m_Video.ShowWindow(SW_SHOW);
    }	
}

void CVideoPane::OnRButtonDown(UINT nFlags, CPoint point)
{
    // ɾ������Ӧ��С�Ի���
    if (NULL != m_pVideoPaneMgr)
    {
        m_pVideoPaneMgr->DeleteSoundCtrl();
    }

	// ******��ͼ����******
    if (m_bDrawEnabled)
    {
        DrawRButtonDown(point);
    }

    CPoint mousePos;
    (void)GetCursorPos(&mousePos);

	if (m_ulPlayState != VIDEO_PLAY_STATE_NOT_PLAYING)
	{			
		if (m_TitleBarRect.PtInRect(mousePos))
		{
			return;
		}
	}
    //�ϱ�����Ҽ�����¼�
    GetOCXPlayerCtrl()->PostRightMouseClickEventMsg(0, m_ulPaneID, static_cast<ULONG>(mousePos.x), static_cast<ULONG>(mousePos.y));
    CDialog::OnRButtonDown(nFlags,point);
}

void CVideoPane::OnRButtonUp(UINT nFlags, CPoint point)
{
	// ******��ͼ����******
    if (m_bDrawEnabled && m_bIsDrawing)
    {
        DrawRButtonUp(point);
    }
	CDialog::OnRButtonUp(nFlags,point);
}

BOOL CVideoPane::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CVideoPane::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);
    if (m_Video.GetSafeHwnd()!=NULL)
    {
        MoveVideo();
    }
}

void CVideoPane::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case WARN_TIMER_ID:
		{
			m_ulWarnCounter++;

			// Ĭ�Ͼ���1���ӣ�1���������Σ����Լ���Ϊ120ʱ������ȡ��
			if (0 != m_ulMaxWarnCount)
			{
				if (m_ulMaxWarnCount == m_ulWarnCounter)
				{
					KillTimer(WARN_TIMER_ID);
					m_ulWarnCounter = 0;
				}
				else
				{
					//������û��ʾ��������
					if (FALSE == this->IsWindowVisible())
					{
						break;
					}

					// ������Ϊż������ʾ���������ʾ�ڿ�
					if (0 == (m_ulWarnCounter % 2))
					{
						SetBorder(PANE_STATE_WARNING);
					}
					else
					{
						SetBorder(PANE_STATE_NOT_ACTIVATE);
					}
				}
			}
			else
			{
				//������û��ʾ��������
				if (FALSE == this->IsWindowVisible())
				{
					break;
				}

				// ������Ϊż������ʾ���������ʾ�ڿ�
				if (0 == (m_ulWarnCounter % 2))
				{
					SetBorder(PANE_STATE_WARNING);
				}
				else
				{
					SetBorder(PANE_STATE_NOT_ACTIVATE);
				}

				if (10000 < m_ulWarnCounter)
				{
					m_ulWarnCounter = 0;
				}
			}

			// ��wm_paint��Ϣ��ǰ����
			this->GetParent()->UpdateWindow();

			break;
		}
	case MAGNIFY_TIMER_ID:
		SetActivePane();
		KillTimer(MAGNIFY_TIMER_ID);
		break;
	default:
		{}
	}

    CDialog::OnTimer(nIDEvent);
}

// �ж����ָ���Ƿ�����Ļ����
bool CVideoPane::IsInCenter( const CPoint &point )const
{
    CRect rect;
    GetClientRect(rect);

    double leftline   = (double) RATIO_PTZ_GRID * rect.Width();
    double rightline  = (double) (1.0 - RATIO_PTZ_GRID) * rect.Width();
    double topline    = (double) RATIO_PTZ_GRID * rect.Height();
    double bottomline = (double) (1.0 - RATIO_PTZ_GRID) * rect.Height();
    if ((point.x > leftline)
        && (point.x < rightline)
        && (point.y > topline)
        && (point.y < bottomline))
    {
        return true;
    }

    return false;
}

void CVideoPane::Set3DPTZState( ULONG ulIsStart3D )
{
    CHECK_POINTER_VOID(m_pRealTimeToolBar);

    if (ulIsStart3D == PTZ_3D_OPEN)
    {
        if (m_ulIsRealtimePtzing == BOTTOMBAR_PTZ_OPEN)
        {
           //m_pRealTimeToolBar->SetPtzPic(BUTTON_NORMAL);
            // ɾ���ƾ�����
            DeleteDigtalPtz();
            SetRealtimePtzState(BOTTOMBAR_PTZ_CLOSE);
            m_pRealTimeToolBar->SetPtzBtnState(false);
            SetCursorState(CURSOR_STATE_CLOSE);
        }
        if (MAGNIFY_OPEN == m_ulIsRealtimeMagnifying)
        {
			// ����һ������¼����Ŵ�ť�������ر��Լ�
			::SendMessage(m_pRealTimeToolBar->GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(IDC_BTN_TOOL_REALTIME_MAGNIFY, BN_CLICKED), (LPARAM)m_pRealTimeToolBar->GetMagnifyBtnHwnd());
        }

        //3D�Ŵ����ƶ����ô���Ϊ��Ƶԭʼ����
        //if (m_ulDisplayScale == DISPLAY_ORIGINAL)
        //{
        //    HideVideo();
        //    ChangePaneSize();
        //}
        SetCursorState(CURSOR_STATE_3DPTZ);
    } else {
        HideVideo();
        //��ԭ��ʾ����
        ResetPaneSize();
        SetCursorState(CURSOR_STATE_CLOSE);
		SetOpend3DPTZStatus(false);
    }
    m_ulIs3DPTZOpen = ulIsStart3D;
}

void CVideoPane::SetCameraID( const TCHAR* szCameraID )
{
    CHECK_POINTER_VOID(szCameraID);
    if(!CToolsHelp::Strncpy(m_szCameraID, IVS_DEV_CODE_LEN + 1, szCameraID, strnlen(szCameraID,IVS_DEV_CODE_LEN)))
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,_T("IVS_OCX::"),_T("VideoPane .Failed"));
    }
}

void CVideoPane::SetPaneRect(const CRect &paneRect )
{
	m_PaneRect = paneRect;
}

void CVideoPane::SetPaneRectFull(const CRect &paneRect)
{
	m_PaneRectFull = paneRect;
}

void CVideoPane::GetPaneRect( CRect * paneRect )const
{
    CHECK_POINTER_VOID(paneRect);
	*paneRect = m_PaneRect;
}

void CVideoPane::GetPaneRectFull( CRect * paneRect )const
{
    CHECK_POINTER_VOID(paneRect);
	*paneRect = m_PaneRectFull;
}

ULONG CVideoPane::SetRealtimeMagnifyState( ULONG val )
{
    return SetPartMagnifyState(val);
}

// ����ʵ���������зŴ�״̬
void CVideoPane::SetRealtimePtzState( ULONG val )
{
    m_ulIsRealtimePtzing = val;
}

void CVideoPane::HideVideo(void)
{
    m_Video.ShowWindow(SW_HIDE);
}

BOOL CVideoPane::IsPtInWnd( const CWnd* pWnd, const CPoint &point )const
{
    CRect _Rect;
    pWnd->GetWindowRect(&_Rect);
    pWnd->ScreenToClient(&_Rect);
    return _Rect.PtInRect(point);
}

bool CVideoPane::IsPtNotInVideoWnd( const CPoint &point )const
{
    bool bIsPtNotInVideoWnd = true;
    if ((NULL != m_pRealTimeTitleBar) && m_pRealTimeTitleBar->IsWindowVisible())
    {
        bIsPtNotInVideoWnd = IsPtInWnd(m_pRealTimeTitleBar,point)? true : false;
    }
    if ((NULL != m_pRealTimeToolBar) && m_pRealTimeToolBar->IsWindowVisible())
    {
        bIsPtNotInVideoWnd = IsPtInWnd(m_pRealTimeToolBar,point) ? true : false;
    }

    return bIsPtNotInVideoWnd;
}

// Returns:   bool �ƶ�ӥ�۴����Ƿ�ɹ�
//                 ��ԭ���ڿ���С��100ʱʧ��
bool CVideoPane::MoveEagleEye()const
{
    CRect videoPaneRect;
    GetClientRect(videoPaneRect);

    //ӥ�۴�����СΪ100*100
    if ((videoPaneRect.Width() <= 100) || (videoPaneRect.Height() <= 100))
    {
        return false;
    }

    RECT eagleEyeRect;
    eagleEyeRect.bottom = videoPaneRect.bottom;
    eagleEyeRect.top    = (videoPaneRect.bottom/3)*2;
    eagleEyeRect.left   = (videoPaneRect.right/3)*2;
    eagleEyeRect.right  = videoPaneRect.right;
    //ӥ�۴�����СΪ100*100
    eagleEyeRect.top = (eagleEyeRect.bottom-eagleEyeRect.top) < 100 ? (videoPaneRect.bottom - 100) : eagleEyeRect.top;
    eagleEyeRect.left = (eagleEyeRect.right-eagleEyeRect.left) < 100 ? (videoPaneRect.right - 100) : eagleEyeRect.left;
// TODO :m_EagleEye.MoveWindow(&eagleEyeRect);
    //m_EagleEye.BringWindowToTop();

    return true;
}

void CVideoPane::MoveVideo()
{
    long dwStyle = ::GetWindowLong(m_Video.GetSafeHwnd(), GWL_STYLE);
    dwStyle |= WS_CLIPSIBLINGS;
    (void)::SetWindowLong(m_Video.GetSafeHwnd(), GWL_STYLE, dwStyle);
    CRect videoPaneRect;
    GetClientRect(videoPaneRect);

// 	if (m_ulWarnCounter > 0)
// 	{
		m_Video.MoveWindow(CRect(videoPaneRect.left + 2,videoPaneRect.top + 2,videoPaneRect.Width() -3,videoPaneRect.Height()-3));
// 	}
// 	else
// 	{
// 		m_Video.MoveWindow(&videoPaneRect);
// 	}
}
/*
void CVideoPane::SplitTVWallPaneToFour()
{
	CRect videoPaneRect;
	GetClientRect(videoPaneRect);
	CRect rect1;
	rect1.top = videoPaneRect.top;
	rect1.left = videoPaneRect.left;
	rect1.bottom = videoPaneRect.top + videoPaneRect.Height()/2;
	rect1.right = videoPaneRect.left + videoPaneRect.Width()/2;
	

	CRect rect2;
	rect2.right = videoPaneRect.right;
	rect2.top = videoPaneRect.top;
	rect2.left = rect1.right;
	rect2.bottom = rect1.bottom;

	CRect rect3;
	rect3.top = rect1.bottom;
	rect3.right = rect1.right;
	rect3.bottom = videoPaneRect.bottom;
	rect3.left = videoPaneRect.left;

	CRect rect4;
	rect4.left = rect1.right;
	rect4.top = rect1.bottom;
	rect4.bottom = videoPaneRect.bottom;
	rect4.right = videoPaneRect.right;

	m_Video.MoveWindow(&rect1);
	m_Video2.MoveWindow(&rect2);
	m_Video3.MoveWindow(&rect3);
	m_Video4.MoveWindow(&rect4);
	m_Video.ShowWindow(SW_SHOW);
	m_Video2.ShowWindow(SW_SHOW);
	m_Video3.ShowWindow(SW_SHOW);
	m_Video4.ShowWindow(SW_SHOW);
}
void CVideoPane::ShowTVWallSplitLine()
{
	CRect videoPaneRect;
	GetClientRect(videoPaneRect);
	int x= videoPaneRect.left+videoPaneRect.Width()/2;
	CPoint A(x,videoPaneRect.top);
	CPoint B(x,videoPaneRect.bottom);
	int y = videoPaneRect.top+videoPaneRect.Height()/2;
	CPoint C(videoPaneRect.left,y);
	CPoint D(videoPaneRect.right,y);

	//������
	CDC * dc;
	dc = GetDC();
	CHECK_POINTER_VOID(dc);
	CVideoPaneMgr * pVideoPaneMgr = GetVideoPaneMgr();
	CHECK_POINTER_VOID(pVideoPaneMgr);
	dc->SetBkColor(pVideoPaneMgr->m_VideoPaneBKGColor);

	CPen pen;
	CPen * pOldPen = NULL;
	(void)pen.CreatePen(PS_DOT, 1, RGB(44,44,44));
	pOldPen = dc->SelectObject(&pen);	
	dc->MoveTo(A);
	dc->LineTo(B);
	dc->MoveTo(C);
	dc->LineTo(D);
	dc->SelectObject(pOldPen);
	(void)pen.DeleteObject();
	(void)ReleaseDC(dc);
}
*///ȥ������ǽ���и�
void CVideoPane::MoveVideoPane()
{
    if (MAGNIFY_OPEN == m_ulIsRealtimeMagnifying || MAGNIFY_OPEN == m_ulIsRecordMagnifying || PTZ_3D_OPEN == m_ulIs3DPTZOpen)
    {
        ChangePaneSize();
    } else {
        MoveWindow(m_PaneRect);
    }
    //Invalidate();
}

void CVideoPane::SetToolbarEnableStatus( ULONGLONG dwToolbarBtn, ULONGLONG lButtonStatus )
{
    if (m_pRealTimeToolBar != NULL)
    {
        m_pRealTimeToolBar->SetToolbarEnableStatus(dwToolbarBtn,lButtonStatus);
    }

	if (m_pAnalysisToolBar != NULL)
	{
		m_pAnalysisToolBar->SetToolbarEnableStatus(dwToolbarBtn,lButtonStatus);
	}
}

void CVideoPane::SetToolBarBtnVisableStatus(ULONGLONG dwToolbarBtn)
{
	if (m_pRealTimeToolBar != NULL)
	{
		m_pRealTimeToolBar->SetToolBarVisableStatus(dwToolbarBtn);
		m_ulToolBarStatus = dwToolbarBtn;
	}

	if (m_pAnalysisToolBar != NULL)
	{
		m_pAnalysisToolBar->SetToolBarVisableStatus(dwToolbarBtn);
	}
}
ULONGLONG CVideoPane::GetToolBarBtnVisableStatus(void)const
{
	return m_ulToolBarStatus;
}

// ����Ϊ�����
void CVideoPane::SetActivePane(void)
{
    CHECK_POINTER_VOID(m_pVideoPaneMgr);

    CPoint mousePos;
    (void)GetCursorPos(&mousePos);
    //�ϱ�����������¼�
    GetOCXPlayerCtrl()->PostLeftMouseClickEventMsg(0, m_ulPaneID, static_cast<ULONG>(mousePos.x), static_cast<ULONG>(mousePos.y));
    if (PANE_STATE_ACTIVATE == m_ulIsActivate)
    {
        //�Ѿ��ǻ����ֱ�ӻ�ȡ���㣬����ֲ��Ŵ�״̬�����޷���Ӧ���ֲ���
        if (FULL_SCREEN == m_pVideoPaneMgr->GetFullScreenState())
        {
            CWnd *pParent =  GetParent();
            CHECK_POINTER_VOID(pParent);
            (void)pParent->SetForegroundWindow();
            pParent->BringWindowToTop();
            pParent->SetFocus();
			
			SetFocus();// add by z00193167 ʹ��paneΪ���㣬���ֿ�����Ӧ
        } else {
            (void)GetOCXPlayerCtrl()->SetFocus();
			SetFocus();// add by z00193167 ʹ��paneΪ���㣬���ֿ�����Ӧ
        }
        //return;//�����أ�����ȫ����������ESC�����˳� 2014-5-7 ������ 
    }
	else
	{
		CVideoPane* pOldActivePane = m_pVideoPaneMgr->GetActiveWndPointer();
		CHECK_POINTER_VOID(pOldActivePane);
		pOldActivePane->HideTwoBar();
	}
    // ���ù���������û������
    m_pVideoPaneMgr->SetActivatePane(m_ulPaneID);

    if (BOTTOMBAR_MAGNIFY_OPEN == GetDstMagnifyState())
	{
		SetFocus();
	}
}

// �߿������븴λ
void CVideoPane::SetBorder( ULONG ulBorderState )
{
	if (!IsWindowVisible())
	{
		return;
	}

	CRect parentWndRect, childWndRect, borderRect;

	CHECK_POINTER_VOID(m_pVideoPaneMgr);

	GetWindowRect(childWndRect);
	GetParent()->GetWindowRect(parentWndRect);

	borderRect.left   = m_PaneRect.left + 1 ;
	borderRect.right  = m_PaneRect.right - 1;
	borderRect.top    = m_PaneRect.top + 1;
	borderRect.bottom = m_PaneRect.bottom - 1;

	//if (m_Video.IsWindowVisible())
	//{
	//	m_Video.MoveWindow(&borderRect);
	//}

	CDC * dc;
	dc = GetParent()->GetDC();
	CHECK_POINTER_VOID(dc);

	//��ˢ�����������
	CBrush brush;
	(void)brush.CreateStockObject(NULL_BRUSH);
	CBrush * pOldBrush = dc->SelectObject(&brush);

	//����
	CPen pen,NoActivePen;
	CPen * pOldPen;

	//���ݴ���״̬����ʾ�߿���ɫ��ͬ
	if (PANE_STATE_WARNING == ulBorderState)
	{
		(void)pen.CreatePen(PS_SOLID, 1, COLOR_WARING_EDGE); //lint !e845 !e835
		pOldPen = dc->SelectObject(&pen);
		if (!m_bIsMoveVideoWindow)
		{
			MoveVideo();
			m_bIsMoveVideoWindow = TRUE;
		}
	} else {
		(void)pen.CreatePen(PS_SOLID, 2, RGB(70,70,70));
		pOldPen = dc->SelectObject(&pen);
		m_bIsMoveVideoWindow = FALSE;
	}
	// ������
	(void)dc->Rectangle(borderRect);

	if (PANE_STATE_ACTIVATE == ulBorderState)
	{
//         if (VIDEO_PLAY_STATE_NOT_PLAYING != GetPlayState())
//         {
//             MoveWindow(m_PaneRect.left+3, m_PaneRect.top+3, m_PaneRect.Width()-6, m_PaneRect.Height()-6);
//         }
		SetActiveBorder();
	} else {
        MoveWindow(m_PaneRect.left, m_PaneRect.top, m_PaneRect.Width(), m_PaneRect.Height());
		//m_PaneRect.InflateRect(PANE_EDGE_PIXEL,PANE_EDGE_PIXEL);
		//m_PaneRect.DeflateRect(1,1);
		borderRect.InflateRect(1,1);
		(void)NoActivePen.CreatePen(PS_SOLID, 1, RGB(24,24,24));
		pOldPen = dc->SelectObject(&NoActivePen);
		(void)dc->Rectangle(borderRect);
	}

	// �ָ�ԭ���Ļ�ˢ�ͻ���
	(void)dc->SelectObject(pOldPen);
	(void)dc->SelectObject(pOldBrush);

	(void)pen.DeleteObject();
	(void)NoActivePen.DeleteObject();

	(void)brush.DeleteObject();
	(void)ReleaseDC(dc);
}

void CVideoPane::SetActiveBorder()
{
	CIVS_OCXPlayerCtrl* pCtrl = GetOCXPlayerCtrl();
	CHECK_POINTER_VOID(pCtrl);
    CVideoPaneMgr * pVideoPaneMgr = pCtrl->GetVideoPaneMgr();
    CHECK_POINTER_VOID(pVideoPaneMgr);

    CRect borderRect;

	borderRect = m_PaneRect;
	//m_PaneRect.InflateRect(1,1);
	//m_PaneRect.DeflateRect(PANE_EDGE_PIXEL,PANE_EDGE_PIXEL);
	borderRect.DeflateRect(1,1);

    CDC * dc;
    dc = GetParent()->GetDC();
    CHECK_POINTER_VOID(dc);

    //��ˢ�����������
    CBrush brush;
    (void)brush.CreateStockObject(NULL_BRUSH);
    CBrush * pOldBrush = dc->SelectObject(&brush);

    //����
    CPen pen;
    CPen * pOldPen;
	bool bCreatePen = false;
    if (pVideoPaneMgr->m_VideoPaneBKGColor ==  RGB(0,0,0))
    {

        //(void)pen.CreatePen(PS_SOLID, 1, RGB(nRed - i * 20, nGreen - i * 40, nBlue - i * 50));
    } else {
		bCreatePen = true;
        (void)pen.CreatePen(PS_SOLID, 2, RGB(41, 154 , 246));
    }

    pOldPen = dc->SelectObject(&pen);
    borderRect.DeflateRect(0, 0);
    (void)dc->Rectangle(borderRect);

    // �ָ�ԭ���Ļ�ˢ�ͻ���
    (void)dc->SelectObject(pOldPen);
    (void)dc->SelectObject(pOldBrush);

    (void)brush.DeleteObject();
	if (bCreatePen)
	{
		(void)pen.DeleteObject();
	}
    (void)ReleaseDC(dc);
}

 //���þ����״̬
ULONG CVideoPane::SetWarnState( ULONG ulWarnState,ULONG ulWarnSeconds )
{
    if (WARNING == ulWarnState)
    {
        if (VIDEO_PLAY_STATE_NOT_PLAYING == m_ulPlayState)
        {
            BP_RUN_LOG_ERR(IVS_OCX_RET_SET_WARN_STATE_ERROR, "","Video pane set warn state error,video pane is not playing video.");
            return IVS_OCX_RET_SET_WARN_STATE_ERROR;
        }

        m_ulWarnCounter  = 0;
        m_ulMaxWarnCount = ulWarnSeconds * 2; //һ��������
        (void)SetTimer(WARN_TIMER_ID, 500, NULL);
    } else {
        m_ulWarnCounter = 0;
        (void)KillTimer(WARN_TIMER_ID);
        SetBorder(m_ulIsActivate);

        //��win7����߰汾�� ��wm_paint��Ϣ��ǰ����
		if (CUtility::CheckIsWin7() && (IsWindowVisible()))
		{
			Invalidate(FALSE);
		}
    }

    return IVS_SUCCEED;
}

void CVideoPane::SetPtzCursor( const CPoint &point )const
{
    CRect rect;
    GetClientRect(rect);

    // ���ǵ����ָ���СΪ32*32��ָ���Ƶ��ұߺ��±߾�߽�С��32����ʱ��ָ�벻��
    if ((point.x > rect.right - 32)
        || (point.y > rect.bottom - 32))
    {
        return;
    }

    if (600 < rect.Width())
    {
        // ��ʾָ���ͼ��
        if (m_bIsButtonDown)
        {
            //SetPtzCursorButtonDownBig(point);
        } else {
            //SetPtzCursorUnButtonDownBig(point);
        }
    } else {
        // ��ʾָ��Сͼ��
        if (m_bIsButtonDown)
        {
            //SetPtzCursorButtonDown(point);
        } else {
            //SetPtzCursorUnButtonDown(point);
        }
    }
}

void CVideoPane::SetPtzControl( const CPoint &point, const TCHAR *pCameraCode, const TCHAR *pMode, const TCHAR *pSpeed )
{
    CRect rect;
    GetClientRect(rect);

    double leftline   = (double) RATIO_PTZ_GRID * rect.Width();
    double rightline  = (double) (1.0 - RATIO_PTZ_GRID) * rect.Width();
    double topline    = (double) RATIO_PTZ_GRID * rect.Height();
    double bottomline = (double) (1.0 - RATIO_PTZ_GRID) * rect.Height();
    if (point.x < leftline)
    {
        if (point.y < topline)
        {
            BSTR b = GetOCXPlayerCtrl()->IVS_OCX_PtzControl(pCameraCode, PTZ_UP_LEFT, pMode, pSpeed);
			::SysFreeString(b);
        } else if (point.y > bottomline) {
            BSTR b = GetOCXPlayerCtrl()->IVS_OCX_PtzControl(pCameraCode, PTZ_DOWN_LEFT, pMode, pSpeed);
			::SysFreeString(b);
        } else {
            BSTR b = GetOCXPlayerCtrl()->IVS_OCX_PtzControl(pCameraCode, PTZ_LEFT, pMode, pSpeed);
			::SysFreeString(b);
        }
    } else if (point.x > rightline) {
        if (point.y < topline)
        {
            BSTR b = GetOCXPlayerCtrl()->IVS_OCX_PtzControl(pCameraCode, PTZ_UP_RIGHT, pMode, pSpeed);
			::SysFreeString(b);
        } else if (point.y > bottomline) {
            BSTR b = GetOCXPlayerCtrl()->IVS_OCX_PtzControl(pCameraCode, PTZ_DOWN_RIGHT, pMode, pSpeed);
			::SysFreeString(b);
        } else {
            BSTR b = GetOCXPlayerCtrl()->IVS_OCX_PtzControl(pCameraCode, PTZ_RIGHT, pMode, pSpeed);
			::SysFreeString(b);
        }
    } else {
        if (point.y < topline)
        {
            BSTR b = GetOCXPlayerCtrl()->IVS_OCX_PtzControl(pCameraCode, PTZ_UP, pMode, pSpeed);
			::SysFreeString(b);
        } else if (point.y > bottomline) {
            BSTR b = GetOCXPlayerCtrl()->IVS_OCX_PtzControl(pCameraCode, PTZ_DOWN, pMode, pSpeed);
			::SysFreeString(b);
        }
    }
    return ;
}

void CVideoPane::SetPtzControl( const CPoint &point )
{
    CRect rect;
    GetClientRect(rect);

    // ���ǵ����ָ���СΪ32*32��ָ���Ƶ��ұߺ��±߾�߽�С��32����ʱ��ָ�벻��
    if ((point.x > rect.right - 32)
        || (point.y > rect.bottom - 32))
    {
        return ;
    }

    // ��ȡ��̨����
    TCHAR szMode[5] = {0};
    TCHAR szSpeed[5] = {0};
    if (!CToolsHelp::Memcpy(szMode,5,_T("2"), 1))
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"IVS_OCX::","VideoPane.Failed");
        return;
    }

    LONG ulRet = IVS_SUCCEED;
    BSTR b = GetOCXPlayerCtrl()->IVS_OCX_GetLocalPTZConfig();
	char* pb = _com_util::ConvertBSTRToString(b);
	if (NULL == pb)
	{
		::SysFreeString(b);
		return;
	}
	std::string strXML = pb;
	// �ڴ��ͷ�
	::SysFreeString(b);
	delete[] pb;

    LONG LResult = 0;
    UINT32 lSpeed = 0;

    ulRet = PostEventXMLImpl.GetPTZConfig(strXML,&LResult,&lSpeed);
    if (IVS_SUCCEED != ulRet && LResult != IVS_SUCCEED)
    {
        if (!CToolsHelp::Memcpy(szSpeed,5,_T("1"), 1))
        {
            BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,_T("IVS_OCX::"),_T("VideoPane Set Ptz Control.Failed"));
            return;
        }
    }

    TCHAR szCameraCode[IVS_DEV_CODE_LEN] = {0};
    CString strCameraCode(GetCameraID());
    if (!CToolsHelp::Memcpy(szCameraCode,IVS_DEV_CODE_LEN,strCameraCode.GetBuffer(0),static_cast<unsigned int>(strCameraCode.GetLength())))
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,_T("IVS_OCX::"),_T("VideoPane.Failed"));
        return;
    }

    SetPtzControl(point, szCameraCode, szMode, szSpeed);

    if (IVS_SUCCEED != ulRet)
    {
        CHECK_POINTER_VOID(m_pRealTimeToolBar);

        // ��CU�ϱ��쳣
        SetRealtimePtzState(BOTTOMBAR_PTZ_CLOSE);
        //m_pVideoRealtimeBar->SetPtzPic(BUTTON_NORMAL);
        SetCursorState(CURSOR_STATE_CLOSE);
        //m_pVideoRealtimeBar->SendPtzResult(ulRet);
    }

    return ;
}

void CVideoPane::OnRealtimeLButtonUp( const CPoint &point )
{
    CHECK_POINTER_VOID(m_ulDrawHandle);
    UINT scale = m_pVideoPaneMgr->GetDispalyScale();
    BOOL bScaleModeFull = scale == DISPLAY_SCALE_FULL ? TRUE : FALSE;
    (VOID)Draw_SetDisplayScaleMode((DRAW_HANDLE)m_ulDrawHandle, bScaleModeFull);

    // ʵ������������̨���ƹ���
    if (CURSOR_STATE_PTZ == m_ulCursorState)
    {
        ReleaseCapture();
        TCHAR szCameraCode[IVS_DEV_CODE_LEN] = {0};
        CString strCameraCode = GetCameraID();
        if(!CToolsHelp::Memcpy(szCameraCode,IVS_DEV_CODE_LEN,strCameraCode.GetBuffer(0),static_cast<unsigned int>(strCameraCode.GetLength())))
        {
            BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,_T("IVS_OCX::"),_T("CVideoPane::OnRealtimeLButtonUp.Failed"));
            return;
        }

        // ��ȡ��̨����
        TCHAR szMode[5] = {0};
        TCHAR szSpeed[5] = {0};
        if (!CToolsHelp::Memcpy(szMode,5,_T("2"), 1))
        {
            BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"IVS_OCX::","CVideoPane .Failed");
            return;
        }

        LONG ulRet = IVS_SUCCEED;
        BSTR b = GetOCXPlayerCtrl()->IVS_OCX_GetLocalPTZConfig();
		char* pb = _com_util::ConvertBSTRToString(b);
		if (NULL == pb)
		{
			::SysFreeString(b);
			return;
		}
        std::string strXML = pb;
		// �ڴ��ͷ�
		::SysFreeString(b);
		delete[] pb;

        LONG LResult = 0;
        UINT32 lSpeed = 0;

        ulRet = PostEventXMLImpl.GetPTZConfig(strXML,&LResult,&lSpeed);
        if (IVS_SUCCEED != ulRet && LResult != IVS_SUCCEED)
        {
            if(!CToolsHelp::Memcpy(szSpeed,5,_T("1"), 1))
            {
            	BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"IVS_OCX::","VideoPane.Failed");
                return;
            }
        }

        SetPtzCursor(point);
        BSTR bc = GetOCXPlayerCtrl()->IVS_OCX_PtzControl(strCameraCode.GetBuffer(), PTZ_STOP, szMode, szSpeed);
		::SysFreeString(bc);
        if (IVS_SUCCEED != ulRet)
        {
            CHECK_POINTER_VOID(m_pRealTimeToolBar);

            SetRealtimePtzState(BOTTOMBAR_PTZ_CLOSE);
            //m_pRealTimeToolBar->SetPtzPic(BUTTON_NORMAL);
            SetCursorState(CURSOR_STATE_CLOSE);
            //m_pRealTimeToolBar->SendPtzResult(ulRet);
        }
    } else if (m_bIsButtonDown) {
        CRect mainRect;
        GetDisplayScaleRect(mainRect);
		
		CRect fullRect;
		m_Video.GetClientRect(&fullRect);
		
		//�������ڲ���ȫ����ʱ����Ҫ��ת��
        m_startPoint.x -=  (fullRect.Width() - mainRect.Width())/2;
        m_startPoint.y -=  (fullRect.Height() - mainRect.Height())/2;

		m_lastPoint.x = point.x > mainRect.right ? mainRect.right : (point.x < mainRect.left ? mainRect.left : point.x);
		m_lastPoint.y = point.y > mainRect.bottom ? mainRect.bottom : (point.y < mainRect.top ? mainRect.top : point.y);
		m_lastPoint.x -= (fullRect.Width() - mainRect.Width())/2;
		m_lastPoint.y -= (fullRect.Height() - mainRect.Height())/2;

        if (MAGNIFY_OPEN == m_ulIsRealtimeMagnifying)
        {
			(void)Draw_RectangleFinish((DRAW_HANDLE)m_ulDrawHandle,point,0);
        } else if (PTZ_3D_OPEN == m_ulIs3DPTZOpen) {
            (void)Draw_RectangleFinish((DRAW_HANDLE)m_ulDrawHandle,point,0);
			(void)DeleteGraphics();
			CIVS_OCXPlayerCtrl* pCtrl = GetOCXPlayerCtrl();
			if (NULL != pCtrl)
			{
				(void)pCtrl->IVS_OCX_DisableDraw(m_ulPaneID);
			}
            PTZ3DMouseUp(mainRect);
        }
        m_bIsButtonDown = false;
        m_bIsMoving = false;
        ReleaseCapture();
    }
}

void CVideoPane::SetToolBarBtnState( BOOL bState )const
{
    CHECK_POINTER_VOID(m_pRealTimeToolBar);
}//lint !e715

void CVideoPane::SetTitleButtonPic( bool bState )const
{
    CHECK_POINTER_VOID(m_pRealTimeTitleBar);
}//lint !e715

void CVideoPane::SetRealtimePlayPic( ULONG ulState )const
{
    CHECK_POINTER_VOID(m_pRealTimeToolBar);
    if (WND_TYPE_REALTIME != m_ulWndType)
    {
        return;
    }
}//lint !e715

void CVideoPane::ResetPane()
{
    SetPlayState(VIDEO_PLAY_STATE_NOT_PLAYING);
	SetPrvePlayState(VIDEO_PLAY_STATE_NOT_PLAYING);
    ResetBottomButton();
    Set3DPTZState(PTZ_3D_CLOSE);
	StopSerialSnapshot();
    if (WND_TYPE_ANALYSIS != m_ulWndType)
    {
		if (m_pRealTimeTitleBar != nullptr)
		{
			m_pRealTimeTitleBar->ZeroSize();
			m_pRealTimeTitleBar->SetDevName(std::string(""));
		}
    }
	SetDevName(std::string(""));
    m_bIsPuRecord = FALSE;
    m_bIsLocalRecord =FALSE;
	m_bLocalRecordTrigger = FALSE;
    m_bIsPlatRecord = FALSE;
	m_bIsResume     = FALSE;
	EnableALLButton(FALSE);
	SetAnalysisBarIsStartPlay(FALSE);
    
    m_Video.ShowWindow(SW_HIDE);

    if (IsWindowVisible())
    {
        // ���»��±���
        //Invalidate(TRUE);
    }

    memset(m_szCameraID, 0, IVS_DEV_CODE_LEN + 1 );
	//����͸��Bar
	HideTranslucenWnd();
}

ULONG CVideoPane::SetPartMagnifyState( ULONG ulState )
{
	m_ulIsRealtimeMagnifying = ulState;
    return IVS_SUCCEED;
}

void CVideoPane::ResetPaneSize()
{
    CRect paneRect;
    GetPaneRect(&paneRect);
    MoveWindow(&paneRect);
    if (IsWindowVisible())
    {
        GetParent()->InvalidateRect(&paneRect);
    }
}

void CVideoPane::OnRealtimePaneMouseMove( const CPoint &point, const CRect &mainRect )
{
    if (0 == m_ulDrawHandle)
    {
        // �˴����ܼ���־;
        return;
    }
    UINT scale = m_pVideoPaneMgr->GetDispalyScale();
    BOOL bScaleModeFull = scale == DISPLAY_SCALE_FULL ? TRUE : FALSE;
    (VOID)Draw_SetDisplayScaleMode((DRAW_HANDLE)m_ulDrawHandle, bScaleModeFull);

    CHECK_POINTER_VOID(m_pRealTimeToolBar);
    if (m_pRealTimeToolBar->GetSoundCtrlState())
    {
        DeleteSoundCtrl(point);      //lint !e522
    } else if (m_pRealTimeToolBar->GetTalkbackSoundState()) {
        DeleteTalkbackSound(point);  //lint !e522
    }
    if (m_bIsButtonDown && m_startPoint!=point)
    {
        m_lastPoint.x = point.x > mainRect.right ? mainRect.right : (point.x < mainRect.left ? mainRect.left : point.x);
        m_lastPoint.y = point.y > mainRect.bottom ? mainRect.bottom : (point.y < mainRect.top ? mainRect.top : point.y);
        if (MAGNIFY_OPEN == m_ulIsRealtimeMagnifying)
        {
			if (GRAPHICS_STATE_EDITING == m_ulGraphicState)
			{
				(void)Draw_ModifyShapeUpDate((DRAW_HANDLE)m_ulDrawHandle,m_ulCurrentGraphicGroup,point);
			}
			else if (GRAPHICS_STATE_DRAWING == m_ulGraphicState)
			{
				m_DrawMoveSuccessSum *= (ULONG)Draw_RectangleUpDate((DRAW_HANDLE)m_ulDrawHandle,point);
			}
        } else if (PTZ_3D_OPEN == m_ulIs3DPTZOpen) {
            (void)Draw_RectangleUpDate((DRAW_HANDLE)m_ulDrawHandle,point);
        }
    }
}//lint !e1762

void CVideoPane::ResetBottomButton()
 {
	if (m_pRealTimeToolBar != nullptr)
	{
		m_pRealTimeToolBar->ResetBottomButton(); //lint !e613
	}

	if (m_pAnalysisToolBar != nullptr)
	{
		m_pAnalysisToolBar->ResetBottomButton();//lint !e613
	}

    if (WND_TYPE_REALTIME == m_ulWndType)
    {
        // ��ԭ���Ű�ť
        SetPlayState(VIDEO_PLAY_STATE_NOT_PLAYING);

        // ��ԭ¼��״̬
        SetRealtimeRecordState(BOTTOMBAR_RECORD_CLOSE);

        // ��ԭץ��״̬
        SetRealtimeCaptureState(BOTTOMBAR_CAPTURE_CLOSE);

        //// ��ԭ��ǩ״̬
        SetRealtimeLabelState(BOTTOMBAR_LABEL_CLOSE);

        // ��ԭ�����Խ�״̬
        SetRealtimeTalkbackState(BOTTOMBAR_TALKBACK_CLOSE);

        // ��ԭ������ť״̬
        SetRealtimeSoundState(BOTTOMBAR_SOUND_CLOSE);

        // ��ԭ�ֲ��Ŵ�״̬
        if (m_ulIsRealtimeMagnifying == MAGNIFY_OPEN)
        {
            HideVideo();
            (void)SetRealtimeMagnifyState(MAGNIFY_CLOSE);
        }

        // ��ԭ��̨����״̬
        SetRealtimePtzState(BOTTOMBAR_PTZ_CLOSE);

        // ��ԭ��Ƶ��ǽ״̬
        SetRealtimeWallState(BOTTOMBAR_WALL_CLOSE);

        // ��ԭ���ܷ����������״̬
        SetRealtimeAnalysisState(BOTTOMBAR_ANALYSIS_CLOSE);

    } else if (WND_TYPE_RECORD == m_ulWndType) {
        // ��ԭ���Ű�ť
        SetPlayState(VIDEO_PLAY_STATE_NOT_PLAYING);
        // ��ԭץ��״̬
        SetRecordCaptureState(BOTTOMBAR_CAPTURE_CLOSE);
        // ��ԭ��ǩ״̬
        SetRecordLabelState(BOTTOMBAR_LABEL_CLOSE);
        // ��ԭ������ť״̬
        SetRecordSoundState(BOTTOMBAR_SOUND_CLOSE);
        // ��ԭ�ֲ��Ŵ�״̬
        if (m_ulIsRecordMagnifying == MAGNIFY_OPEN)
        {
            HideVideo();
            SetRecordMagnifyState(MAGNIFY_CLOSE);
        }

        // ��ԭ��Ƶ��ǽ״̬
        SetRecordWallState(BOTTOMBAR_WALL_CLOSE);

        // ��ԭ���ܷ����������
        SetRecordAnalysisState(BOTTOMBAR_ANALYSIS_CLOSE);
    }
}

void CVideoPane::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
}

void CVideoPane::OnMoving(UINT fwSide, LPRECT pRect)
{
    CDialog::OnMoving(fwSide, pRect);
}
// description: ����Bar��λ��
void CVideoPane::ResetBarPosition( void )
{
	if (!m_bIsHideTranslucentBar)
	{
		if (!IsWindowVisible())return;
	}
	if (m_bIsRefresheed)
	{
		return;
	}

	//�����ж�����Ƿǲ���״̬��return
	BOOL bIsNeedRefresh = TRUE;
	if (WND_TYPE_ANALYSIS != m_ulWndType)
	{
		if (m_ulPlayState == VIDEO_PLAY_STATE_NOT_PLAYING /*||*/
			/*mVIDEO_PLAY_STATE_RESTARTING_ulPlayState ==*/  /*|| m_ulPlayState == VIDEO_PLAY_STATE_EMAP*/)
		{
			//return;
			bIsNeedRefresh = FALSE;
		}
	}

	if ( 0 == m_ulAllBarStatus)
	{
		HideAllBar();
		return;
	}
	
	CHECK_POINTER_VOID(m_pVideoPaneMgr);
	ULONG ulFullState = m_pVideoPaneMgr->GetFullScreenState();
	bool ulTitleBarState = m_pVideoPaneMgr->GetTitleBarShowState();
	bool ulToolBarState = m_pVideoPaneMgr->GetToolBarShowState();

	BOOL bIsNeedDisplay = TRUE;
	if (PANE_STATE_ACTIVATE != m_ulIsActivate)
	{
		bIsNeedDisplay = FALSE;
	}

    SetBarRect();

	//����toolbar/titlebar/tvwallbar����ʾλ��
	m_LISTMOVE.clear();
	WindowMoveInfo info;
	if(m_pRealTimeTitleBar != nullptr && bIsNeedRefresh && bIsNeedDisplay)
	{
		if(ulTitleBarState && WND_TYPE_REALTIME == m_ulWndType)
		{
			info.window = m_pRealTimeTitleBar->GetSafeHwnd();
			info.x = m_TitleBarRect.left;
			info.y = m_TitleBarRect.top;
			info.cx = m_TitleBarRect.Width();
			info.cy = m_TitleBarRect.Height();
			m_LISTMOVE.push_back(info);
			m_pRealTimeTitleBar->SetDevName(m_strDevName);
			if (TYPE_WND_TURNCTUISE == GetTypeWnd())
			{
				InvalidateCruiseBar();
			}
		}
		else
		{
			m_pRealTimeTitleBar->ShowWindow(SW_HIDE);
		}
	}
	if(m_pRealTimeToolBar != nullptr && bIsNeedRefresh && bIsNeedDisplay)
	{
		if(ulToolBarState && WND_TYPE_REALTIME == m_ulWndType)
		{
			info.window = m_pRealTimeToolBar->GetSafeHwnd();
			info.x = m_ToolBarRect.left;
			info.y = m_ToolBarRect.top;
			info.cx = m_ToolBarRect.Width();
			info.cy = m_ToolBarRect.Height();
			m_LISTMOVE.push_back(info);
		}
		else
		{
			m_pRealTimeToolBar->ShowWindow(SW_HIDE);
		}
	}

	if(m_pTVWallStatusBar)
	{
		if(m_bIsShowTVWallBar)
		{
			info.window = m_pTVWallStatusBar->GetSafeHwnd();
			info.x = m_TVWallStatusBarRect.left;
			info.y = m_TVWallStatusBarRect.top;
			info.cx = m_TVWallStatusBarRect.Width();
			info.cy = m_TVWallStatusBarRect.Height();
			m_LISTMOVE.push_back(info);
		}
		else
		{
			m_pTVWallStatusBar->ShowWindow(SW_HIDE);
		}
	}

	//�����ƶ�toolbar/titlebar/tvwallbar
	int iSize = (int)m_LISTMOVE.size();
	if(iSize > 0)
	{
		m_bIsRefresheed = true;
		HWND hhwnd = (NOT_FULL_SCREEN==ulFullState)?(CWnd::wndNoTopMost.GetSafeHwnd()):(CWnd::wndTopMost.GetSafeHwnd());
		HDWP defer_window_pos_info =::BeginDeferWindowPos(iSize);
		if (!defer_window_pos_info)  
			return;
		HWND tempHwnd  = nullptr;
		for (int i = 0; i < iSize; ++i) 
		{  
			const WindowMoveInfo& move = m_LISTMOVE[i];//lint !e732
			tempHwnd = move.window;
			defer_window_pos_info = ::DeferWindowPos(defer_window_pos_info,
				tempHwnd, 
				hhwnd,
				move.x,
				move.y,
				move.cx,
				move.cy, 
				SWP_SHOWWINDOW|SWP_NOACTIVATE);  
			if (!defer_window_pos_info)    
				return;

			CWnd* pWnd =  CWnd::FromHandle(tempHwnd);
			if(pWnd)
			{
				pWnd->SendMessage(WM_PAINT,NULL,NULL);
			}
			
		}
		::EndDeferWindowPos(defer_window_pos_info);
		m_bIsRefresheed = false;
	}

	if (TYPE_WND_TURNCTUISE == GetTypeWnd())
	{
		bIsNeedRefresh = true;
	}

	if (!bIsNeedRefresh)
	{
		return;
	}
	
	//���´����toolbar/titlebar/tvwallbar֮���͸������
	if (FULL_SCREEN == ulFullState)
	{
		if (m_pVideoPaneMgr->GetShowToolBar())
		{
			if (WND_TYPE_ANALYSIS == m_ulWndType)
			{
				CHECK_POINTER_VOID(m_pAnalysisToolBar);
				m_pAnalysisToolBar->MoveWindow(m_ToolBarRect);
				(void)m_pAnalysisToolBar->SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE | SWP_SHOWWINDOW  | SWP_NOACTIVATE);
				m_pAnalysisToolBar->Invalidate(TRUE);
			}
		} 
		else 
		{
			if (m_pAnalysisToolBar!= nullptr)
			{
				m_pAnalysisToolBar->ShowWindow(SW_HIDE);
			}
		}

		if (m_pTurnCruisePane != nullptr && ::IsWindow(m_pTurnCruisePane->GetSafeHwnd()) && m_ulTurnCruiseStyle)
		{
            m_pTurnCruisePane->ZeroSize();
			m_pTurnCruisePane->MoveWindow(m_TurnCruisePaneRect);
			(void)m_pTurnCruisePane->SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE | SWP_SHOWWINDOW  | SWP_NOACTIVATE);
			m_pTurnCruisePane->Invalidate(TRUE);
		}
		else
		{
			if (m_pTurnCruisePane != nullptr)
			{
				m_pTurnCruisePane->ShowWindow(SW_HIDE);
			}
		}

		if (m_pDigtalDlg != nullptr && ::IsWindow(m_pDigtalDlg->GetSafeHwnd()))
		{
			m_pDigtalDlg->MoveWindow(m_DigtalRect);
			(void)m_pDigtalDlg->SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE | SWP_SHOWWINDOW  | SWP_NOACTIVATE);
			m_pDigtalDlg->Invalidate(TRUE);
		}
		if (BOTTOMBAR_MAGNIFY_OPEN == m_ulIsDstMagnify)
		{
			if (m_pZoomDirctDlg != nullptr && ::IsWindow(m_pZoomDirctDlg->GetSafeHwnd()))
			{
				m_pZoomDirctDlg->MoveWindow(m_pZoomDirctRect);
				(void)m_pZoomDirctDlg->SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE | SWP_SHOWWINDOW  | SWP_NOACTIVATE);
				m_pZoomDirctDlg->Invalidate(TRUE);
			}
			if (m_pZoomBar != nullptr && ::IsWindow(m_pZoomBar->GetSafeHwnd()))
			{
				m_pZoomBar->MoveWindow(m_ZoomBarRect);
				(void)m_pZoomBar->SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE | SWP_SHOWWINDOW  | SWP_NOACTIVATE);
				m_pZoomBar->Invalidate(TRUE);
			}
			if (m_pZoomTitleBar != nullptr && ::IsWindow(m_pZoomTitleBar->GetSafeHwnd()))
			{
				m_pZoomTitleBar->MoveWindow(m_ZoomTitleBarRect);
				(void)m_pZoomTitleBar->SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE | SWP_SHOWWINDOW  | SWP_NOACTIVATE);
				m_pZoomTitleBar->Invalidate(TRUE);
			}

			// ���ظ��ֹ�����
			if (m_pRealTimeTitleBar != NULL)
			{
				m_pRealTimeTitleBar->ShowWindow(SW_HIDE);
			}
			if (m_pRealTimeToolBar != NULL)
			{
				m_pRealTimeToolBar->ShowWindow(SW_HIDE);
			}
		}
		if (m_pPlayTimerBar != nullptr && ::IsWindow(m_pPlayTimerBar->GetSafeHwnd())&&m_ulInstantReplayStyle)
		{
			m_pPlayTimerBar->MoveWindow(m_TimerBarRect);
			(void)m_pPlayTimerBar->SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE | SWP_SHOWWINDOW  | SWP_NOACTIVATE);
			m_pPlayTimerBar->Invalidate(TRUE);
		}
		else
		{
			if (m_pPlayTimerBar != nullptr)
			{
				m_pPlayTimerBar->ShowWindow(SW_HIDE);
			}
		}
	}
	else
	{
		if (m_pVideoPaneMgr->GetShowToolBar())
		{
			if (WND_TYPE_ANALYSIS == m_ulWndType)
			{
				CHECK_POINTER_VOID(m_pAnalysisToolBar);
				m_pAnalysisToolBar->MoveWindow(m_ToolBarRect);
				(void)m_pAnalysisToolBar->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE | SWP_SHOWWINDOW  | SWP_NOACTIVATE);
				m_pAnalysisToolBar->Invalidate(TRUE);
			}
		} 
		else 
		{
			if (m_pAnalysisToolBar!= nullptr)
			{
				m_pAnalysisToolBar->ShowWindow(SW_HIDE);
			}
		}
		if (m_pTurnCruisePane != nullptr && ::IsWindow(m_pTurnCruisePane->GetSafeHwnd()) && m_ulTurnCruiseStyle)
		{
            m_pTurnCruisePane->ZeroSize();
			m_pTurnCruisePane->MoveWindow(m_TurnCruisePaneRect);
			(void)m_pTurnCruisePane->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE | SWP_SHOWWINDOW  | SWP_NOACTIVATE);
			m_pTurnCruisePane->Invalidate(TRUE);
		}
		else
		{
			if (m_pTurnCruisePane != nullptr)
			{
				m_pTurnCruisePane->ShowWindow(SW_HIDE);
			}
		}

		if (m_pDigtalDlg != nullptr && ::IsWindow(m_pDigtalDlg->GetSafeHwnd()))
		{
			m_pDigtalDlg->MoveWindow(m_DigtalRect);
			(void)m_pDigtalDlg->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE | SWP_SHOWWINDOW  | SWP_NOACTIVATE);
			m_pDigtalDlg->Invalidate(TRUE);
		}
		else
		{
			if (m_pDigtalDlg != nullptr)
			{
				m_pDigtalDlg->ShowWindow(SW_HIDE);
				m_pDigtalDlg->Invalidate(TRUE);
			}
		}
		if (BOTTOMBAR_MAGNIFY_OPEN == m_ulIsDstMagnify)
		{
			if (m_pZoomDirctDlg != nullptr && ::IsWindow(m_pZoomDirctDlg->GetSafeHwnd()))
			{
				m_pZoomDirctDlg->MoveWindow(m_pZoomDirctRect);
				(void)m_pZoomDirctDlg->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE | SWP_SHOWWINDOW  | SWP_NOACTIVATE);
				m_pZoomDirctDlg->Invalidate(TRUE);
			}
			else
			{
				if (m_pZoomDirctDlg != nullptr)
				{
					m_pZoomDirctDlg->ShowWindow(SW_HIDE);
					m_pZoomDirctDlg->Invalidate(TRUE);
				}
			}
			if (m_pZoomBar != nullptr && ::IsWindow(m_pZoomBar->GetSafeHwnd()))
			{
				m_pZoomBar->MoveWindow(m_ZoomBarRect);
				(void)m_pZoomBar->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE | SWP_SHOWWINDOW  | SWP_NOACTIVATE);
				m_pZoomBar->Invalidate(TRUE);
			}
			else
			{
				if (m_pZoomBar != NULL)
				{
					m_pZoomBar->ShowWindow(SW_HIDE);
					m_pZoomBar->Invalidate(TRUE);
				}
			}
			if (m_pZoomTitleBar != NULL && ::IsWindow(m_pZoomTitleBar->GetSafeHwnd()))
			{
				m_pZoomTitleBar->MoveWindow(m_ZoomTitleBarRect);
				(void)m_pZoomTitleBar->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE | SWP_SHOWWINDOW  | SWP_NOACTIVATE);
				m_pZoomTitleBar->Invalidate(TRUE);
			}
			else
			{
				if (m_pZoomTitleBar != NULL)
				{
					m_pZoomTitleBar->ShowWindow(SW_HIDE);
					m_pZoomTitleBar->Invalidate(TRUE);
				}
			}
			// ���ظ��ֹ�����
			if (m_pRealTimeTitleBar != NULL && ::IsWindow(m_pRealTimeTitleBar->GetSafeHwnd()))
			{
				m_pRealTimeTitleBar->ShowWindow(SW_HIDE);
			}
			if (m_pRealTimeToolBar != NULL && ::IsWindow(m_pRealTimeToolBar->GetSafeHwnd()))
			{
				m_pRealTimeToolBar->ShowWindow(SW_HIDE);
			}
		}
		if (m_pPlayTimerBar != NULL && ::IsWindow(m_pPlayTimerBar->GetSafeHwnd())&&m_ulInstantReplayStyle)
		{
			m_pPlayTimerBar->MoveWindow(m_TimerBarRect);
			(void)m_pPlayTimerBar->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE | SWP_SHOWWINDOW  | SWP_NOACTIVATE);
			m_pPlayTimerBar->ResetTimer();
			m_pPlayTimerBar->Invalidate(TRUE);
		}
		else
		{
			if (m_pPlayTimerBar != NULL)
			{
				m_pPlayTimerBar->ShowWindow(SW_HIDE);
			}
		}
	}
}

void CVideoPane::HideAllBar()
{
	if (m_pRealTimeTitleBar!=NULL)
	{
		m_pRealTimeTitleBar->ShowWindow(SW_HIDE);
		m_pRealTimeTitleBar->MoveWindow(0,0,0,0);
	}
	if(m_pRealTimeToolBar!=NULL)
	{
		m_pRealTimeToolBar->MoveWindow(0,0,0,0);
		m_pRealTimeToolBar->ShowWindow(SW_HIDE);
	}
	if (m_pAnalysisToolBar!=NULL)
	{
		m_pAnalysisToolBar->MoveWindow(0,0,0,0);
		m_pAnalysisToolBar->ShowWindow(SW_HIDE);
	}
	if (m_pTVWallStatusBar!=NULL)
	{
		m_pTVWallStatusBar->MoveWindow(0,0,0,0);
		m_pTVWallStatusBar->ShowWindow(SW_HIDE);
	}
	if (m_pTurnCruisePane!=NULL)
	{
		m_pTurnCruisePane->MoveWindow(0,0,0,0);
		m_pTurnCruisePane->ShowWindow(SW_HIDE);
	}
	if (m_pDigtalDlg!=NULL)
	{
		m_pDigtalDlg->MoveWindow(0,0,0,0);
		m_pDigtalDlg->ShowWindow(SW_HIDE);
	}
	if (m_pZoomDirctDlg!=NULL)
	{
		m_pZoomDirctDlg->MoveWindow(0,0,0,0);
		m_pZoomDirctDlg->ShowWindow(SW_HIDE);
	}
	if (m_pZoomTitleBar!=NULL)
	{
		m_pZoomTitleBar->MoveWindow(0,0,0,0);
		m_pZoomTitleBar->ShowWindow(SW_HIDE);
	}
	if (m_pZoomBar!=NULL)
	{
		m_pZoomBar->MoveWindow(0,0,0,0);
		m_pZoomBar->ShowWindow(SW_HIDE);
	}
	if (m_pPlayTimerBar!=NULL)
	{
		m_pPlayTimerBar->MoveWindow(0,0,0,0);
		m_pPlayTimerBar->ShowWindow(SW_HIDE);
	}
}


void CVideoPane::HideTwoBar()
{
	if (m_pRealTimeTitleBar!=NULL)
	{
		m_pRealTimeTitleBar->ShowWindow(SW_HIDE);
		//m_pRealTimeTitleBar->MoveWindow(0,0,0,0);
	}
	if(m_pRealTimeToolBar!=NULL)
	{
		//m_pRealTimeToolBar->MoveWindow(0,0,0,0);
		m_pRealTimeToolBar->ShowWindow(SW_HIDE);
	}
}

CPoint CVideoPane::GetInfoButtonRightBottomPosition()const
{
	CHECK_POINTER(m_pRealTimeTitleBar,CPoint(0,0));
	return m_pRealTimeTitleBar->GetInfoButtonRightBottomPosition();
}

void CVideoPane::DeleteBar()
{
	IVS_DELETE(m_pRealTimeToolBar); //lint !e1551
	IVS_DELETE(m_pRealTimeTitleBar);	//lint !e1551
    IVS_DELETE(m_pAnalysisToolBar); //lint !e1551
	IVS_DELETE(m_pTVWallStatusBar); //lint !e1551
}

void CVideoPane::InvalidateBar()
{
    if(m_pRealTimeTitleBar != NULL && m_pRealTimeTitleBar->IsWindowVisible())
    {
        m_pRealTimeTitleBar->Invalidate(TRUE);
    }
    if(m_pRealTimeToolBar != NULL)
    {
        m_pRealTimeToolBar->Invalidate(TRUE);
    }
    if(m_pAnalysisToolBar != NULL)
    {
        m_pAnalysisToolBar->Invalidate(TRUE);
    }
	if(m_pTVWallStatusBar != NULL)
	{
		m_pTVWallStatusBar->Invalidate(TRUE);
	}
	if (m_pTurnCruisePane != NULL)
	{
		m_pTurnCruisePane->Invalidate(TRUE);
	}
	if (m_pDigtalDlg != NULL)
	{
		m_pDigtalDlg->Invalidate(TRUE);
	}
	if (m_pZoomDirctDlg != NULL)
	{
		m_pZoomDirctDlg->Invalidate(TRUE);
	}
	if (m_pZoomBar != NULL)
	{
		m_pZoomBar->Invalidate(TRUE);
	}
	if (m_pZoomTitleBar != NULL)
	{
		m_pZoomTitleBar->Invalidate(TRUE);
	}
}

CIVS_OCXPlayerCtrl *CVideoPane::GetOCXPlayerCtrl()
{
    if (nullptr == m_pOcxCtrl)
    {
		CHECK_POINTER(GetParent(),nullptr);
        m_pOcxCtrl = dynamic_cast<COleControl *>(GetParent()->GetParent());
        CHECK_POINTER(m_pOcxCtrl,nullptr);
    }

    CIVS_OCXPlayerCtrl *pOcxCtrl = dynamic_cast<CIVS_OCXPlayerCtrl *>(m_pOcxCtrl);
    CHECK_POINTER(pOcxCtrl,nullptr);

    return pOcxCtrl;
}

CVideoPaneMgr *CVideoPane::GetVideoPaneMgr()
{
    return m_pVideoPaneMgr;
}

void CVideoPane::CreateRealTimeBar()
{
	int iCount = 0;
	if (GetVideoTitleBar() == nullptr)
	{
		do
		{
			// ����������
			Gdiplus::Image* pImage = nullptr;
			pImage = SkinMgr.GetImageTitleBarBKG();
			if (NULL == pImage)
			{
				return;
			}
			iCount++;
			m_pRealTimeTitleBar = new CTranVideoTitleBar(pImage);
			if (NULL == m_pRealTimeTitleBar)
			{
				//���ٴ�������
				if (iCount >= MAX_CREATE_BAR_COUNT)
				{
					return;
				}
				continue;
			}
			m_pRealTimeTitleBar->SetVideoPane(this);
			(void)m_pRealTimeTitleBar->Create(IDD_DLG_TVIDEOTITLEBAR);
			//m_pRealTimeTitleBar->ZeroSize();
			m_pRealTimeTitleBar->ShowWindow(SW_HIDE);
			SetTitleBarInitStatus();
			break;
		}while(1);//lint !e506
	}
    
	iCount = 0;
	if (GetVideoRealtimeBar() == nullptr)
	{
		do 
		{
			// ����ʵ��������
			Gdiplus::Image * pToolImage = nullptr;
			pToolImage = SkinMgr.GetImageBarBKG();
			if (NULL == pToolImage)
			{
				return;
			}
			iCount++;
			
			try
			{
				m_pRealTimeToolBar = new CTranVideoRealtimeBar(pToolImage);
			}
			catch(...)
			{
				m_pRealTimeToolBar = NULL;
			}

			if (NULL == m_pRealTimeToolBar)
			{
				//���ٴ�������
				if (iCount >= MAX_CREATE_BAR_COUNT)
				{
					return;
				}
				continue;
			}
			m_pRealTimeToolBar->SetToolBarVisableStatus(m_pVideoPaneMgr->GetToolBarVisableStatus());
			m_pRealTimeToolBar->SetVideoPane(this);                             //lint !e1013 !e534
			(void)m_pRealTimeToolBar->Create(IDD_DLG_TVIDEOREALTIMEBAR);
			m_pRealTimeToolBar->ShowWindow(SW_HIDE);
			break;
		} while (1);//lint !e506
	}

    EnableALLButton(FALSE);
}
void CVideoPane::CreateTVWallStatusBar()const
{	/*
	if(nullptr != m_pTVWallStatusBar)
	{
		return;
	}
	m_pTVWallOfflineImage = SkinMgr.GetImageTVWallOfflineStatusBarBKG();
	m_pTVWallOnlineImage = SkinMgr.GetImageTVWallOnlineStatusBarBKG();
	if(nullptr == m_pTVWallOfflineImage || nullptr == m_pTVWallOnlineImage)
	{
		return;
	}
	try
	{
		m_pTVWallStatusBar = new CTranTVWallStatusBar(m_pTVWallOfflineImage);
	}
	catch(...)
	{
		m_pTVWallStatusBar = NULL;
	}
	
	CHECK_POINTER_VOID(m_pTVWallStatusBar);
	m_pTVWallStatusBar->SetVideoPane(this);  
	(void)m_pTVWallStatusBar->Create(IDD_DLG_TVWALL_STATUSBAR);		
	m_pTVWallStatusBar->ShowWindow(SW_HIDE);*/
}
void CVideoPane::SetTVWallStatusBarStatus(ULONG ulBarStatus)
{
	if(!m_pTVWallStatusBar)
	{
		return;
	}
	switch (ulBarStatus)
	{
	case TVWALL_BAR_STAUS_HIDE:
		{
			m_pTVWallStatusBar->MoveWindow(0,0,0,0);
			m_pTVWallStatusBar->ShowWindow(SW_HIDE);
			m_bIsShowTVWallBar = false;
			break;
		}
	case TVWALL_BAR_STAUS_OFFLINE:
		{
			CHECK_POINTER_VOID(m_pTVWallOfflineImage);
			m_pTVWallStatusBar->SetBackGroudPictrue(m_pTVWallOfflineImage);
			m_pTVWallStatusBar->MoveWindow(m_TVWallStatusBarRect);
			m_bIsShowTVWallBar = true;
			break;
		}
	case TVWALL_BAR_STAUS_ONLINE:
		{
			CHECK_POINTER_VOID(m_pTVWallOnlineImage);
			m_pTVWallStatusBar->SetBackGroudPictrue(m_pTVWallOnlineImage);
			m_pTVWallStatusBar->MoveWindow(m_TVWallStatusBarRect);
			m_bIsShowTVWallBar = true;
			break;
		}
	default:
		m_pTVWallStatusBar->ShowWindow(SW_HIDE);
	}
	m_pTVWallStatusBar->Invalidate(TRUE);
	m_ulTVBarStatus = ulBarStatus;
}
//���õ���ǽ��ǽ״̬
void CVideoPane::SetTVWallStatus(ULONG ulPaneStatus)
{
	if(m_pRealTimeTitleBar!=NULL)
	{
		m_pRealTimeTitleBar->SetTVWallStatus(ulPaneStatus);
		m_pRealTimeTitleBar->Invalidate(TRUE);
	}
}

ULONG CVideoPane::GetTVWallStatus()const
{
    if(m_pRealTimeTitleBar!=NULL)
    {
        return m_pRealTimeTitleBar->GetTVWallStatus();
    }
    return 0;
}

void CVideoPane::CreatemTurnCruisePane()
{
	if (NULL == m_pTurnCruisePane)
	{
		//��ȡ·��
		std::string strBackGrondPath;
		strBackGrondPath.clear();
		(void)SkinMgr.TranslucentDlgGetBackGroundPngPath(strBackGrondPath);
		if(strBackGrondPath.empty())return;
		
		m_pTurnCruisePane = new CTurnCruisePane(strBackGrondPath.c_str(),this,m_ulTurnCruiseStyle);
		CHECK_POINTER_VOID(m_pTurnCruisePane);
		m_pTurnCruisePane->SetVideoPane(this);
		m_pTurnCruisePane->Create(IDD_DLG_TURNCRUISEPANE);
		if(::IsWindow(m_pTurnCruisePane->GetSafeHwnd()))
		{
			m_pTurnCruisePane->ShowWindow(SW_HIDE);
		}
		if (m_bIsTurnCruise)
		{
			if(m_pRealTimeTitleBar!=NULL)
			{
				m_pRealTimeTitleBar->SetTurnCruiseStatus(1);
				m_pRealTimeTitleBar->Invalidate(TRUE);
			}
		}

		ResetBarPosition();
	}
	else
	{
		m_pTurnCruisePane->SetCruisePaneStyle(m_ulTurnCruiseStyle);
		ResetBarPosition();
	}
}

void CVideoPane::CreateDigtalPtz()
{
	if (nullptr == m_pDigtalDlg)
	{
		//��ȡ·��,TODO ����Ӧ�ò���Ҫ���͸���ȵı�����ֻ��Ҫһ��ȫ͸��
		std::string strBackGrondPath;
		strBackGrondPath.clear();
		(void)SkinMgr.TranslucentDlgGetBackGroundPngPath(strBackGrondPath);
		if(strBackGrondPath.empty())return;

		m_pDigtalDlg = new CDitalPtzDlg(strBackGrondPath.c_str(),this);
		CHECK_POINTER_VOID(m_pDigtalDlg);
		m_pDigtalDlg->SetVideoPane(this);
		m_pDigtalDlg->Create(IDD_DLG_DITTAL_PTZ);
		if(::IsWindow(m_pDigtalDlg->GetSafeHwnd()))
		{
			m_pDigtalDlg->ShowWindow(SW_SHOW);
		}
	}
}

void CVideoPane::DeleteDigtalPtz()
{
	if(nullptr != m_pDigtalDlg)
	{
		IVS_DELETE(m_pDigtalDlg);
	}
}

void CVideoPane::CreateZoomDirctDlg()
{
	if (nullptr == m_pZoomDirctDlg)
	{
		//��ȡ·��,TODO ����Ӧ�ò���Ҫ���͸���ȵı�����ֻ��Ҫһ��ȫ͸��
		std::string strBackGrondPath;
		strBackGrondPath.clear();
		(void)SkinMgr.TranslucentDlgGetBackGroundPngPath(strBackGrondPath);
		if(strBackGrondPath.empty())return;

		m_pZoomDirctDlg = new CTranZoomDirctDlg(strBackGrondPath.c_str(),this);
		CHECK_POINTER_VOID(m_pZoomDirctDlg);
		m_pZoomDirctDlg->SetVideoPane(this);
		m_pZoomDirctDlg->Create(IDD_DLG_DITTAL_PTZ);
		if(::IsWindow(m_pZoomDirctDlg->GetSafeHwnd()))
		{
			m_pZoomDirctDlg->MoveWindow(CRect(0,0,0,0));
			m_pZoomDirctDlg->ShowWindow(SW_HIDE);
		}
		(void)m_pZoomDirctDlg->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE |SWP_NOACTIVATE |SWP_HIDEWINDOW);
	}
}

void CVideoPane::DeleteZoomDirctDlg()
{
	if(nullptr != m_pZoomDirctDlg)
	{
		IVS_DELETE(m_pZoomDirctDlg);
	}
}

void CVideoPane::CreateZoomBar()
{
	if (nullptr == m_pZoomBar)
	{
		//��ȡ·��
// 		std::string strBackGrondPath;
// 		strBackGrondPath.clear();
// 		(void)SkinMgr.TitleBarGetBackGroundPngPath(strBackGrondPath);
// 		if(strBackGrondPath.empty())return;
		std::string strBackGrondPath;
		strBackGrondPath.clear();
		(void)SkinMgr.ToolBarGetBackGroundPngPath(strBackGrondPath);
		if(strBackGrondPath.empty())return;

		m_pZoomBar = new CTranZoomBar(strBackGrondPath.c_str(),this);
		CHECK_POINTER_VOID(m_pZoomBar);
		m_pZoomBar->SetVideoPane(this);
		m_pZoomBar->Create(IDD_DLG_ZOOMBAR);
		if(::IsWindow(m_pZoomBar->GetSafeHwnd()))
		{
			m_pZoomBar->MoveWindow(CRect(0,0,0,0));
			m_pZoomBar->ShowWindow(SW_HIDE);
		}
		(void)m_pZoomBar->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE |SWP_NOACTIVATE |SWP_HIDEWINDOW);
	}
}

void CVideoPane::DeleteZoomBar()
{
	if(nullptr != m_pZoomBar)
	{
		IVS_DELETE(m_pZoomBar);
	}
}

void CVideoPane::HideZoomWnd()
{
	if (NULL != m_pZoomDirctDlg)
	{
		m_pZoomDirctDlg->ShowWindow(SW_HIDE);
		m_pZoomDirctDlg->Invalidate(TRUE);
	}

	if (NULL != m_pZoomBar)
	{
		m_pZoomBar->ShowWindow(SW_HIDE);
		m_pZoomBar->Invalidate(TRUE);
	}

	if (NULL != m_pZoomTitleBar)
	{
		m_pZoomTitleBar->ShowWindow(SW_HIDE);
		m_pZoomTitleBar->Invalidate(TRUE);
	}
}

void CVideoPane::CreateZoomTitleBar()
{
	if (nullptr == m_pZoomTitleBar)
	{
		//��ȡ·��
		// 		std::string strBackGrondPath;
		// 		strBackGrondPath.clear();
		// 		(void)SkinMgr.TitleBarGetBackGroundPngPath(strBackGrondPath);
		// 		if(strBackGrondPath.empty())return;
		std::string strBackGrondPath;
		strBackGrondPath.clear();
		(void)SkinMgr.ToolBarGetBackGroundPngPath(strBackGrondPath);
		if(strBackGrondPath.empty())return;

		m_pZoomTitleBar = new CTranZoomTitleBar(strBackGrondPath.c_str(),this);
		CHECK_POINTER_VOID(m_pZoomTitleBar);
		m_pZoomTitleBar->SetVideoPane(this);
		m_pZoomTitleBar->Create(IDD_DLG_ZOOMTITLEBAR);
		if(::IsWindow(m_pZoomTitleBar->GetSafeHwnd()))
		{
			m_pZoomTitleBar->MoveWindow(CRect(0,0,0,0));
			m_pZoomTitleBar->ShowWindow(SW_HIDE);
		}
		(void)m_pZoomTitleBar->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE |SWP_NOACTIVATE |SWP_HIDEWINDOW);
	}
}

void CVideoPane::DeleteZoomTitleBar()
{
	if(nullptr != m_pZoomTitleBar)
	{
		IVS_DELETE(m_pZoomTitleBar);
	}
}

void CVideoPane::CreateAnalysisBar()
{
    // �������ܷ���������
    //std::string strBackGrondPath;
    //strBackGrondPath.clear();
    //(void)SkinMgr.ToolBarGetBackGroundPngPath(strBackGrondPath);
    //if(strBackGrondPath.empty())return;
	Gdiplus::Image * pToolImage = nullptr;
	pToolImage = SkinMgr.GetImageBarBKG();
	if (NULL == pToolImage)
	{
		return;
	}

	if (m_pAnalysisToolBar!=nullptr)
	{
		IVS_DELETE(m_pAnalysisToolBar);
		m_pAnalysisToolBar = nullptr;
	}

    m_pAnalysisToolBar = new CTranAnalysisToolBar(pToolImage);
    CHECK_POINTER_VOID(m_pAnalysisToolBar);
    m_pAnalysisToolBar->SetToolBarVisableStatus(m_pVideoPaneMgr->GetToolBarVisableStatus());
    m_pAnalysisToolBar->SetVideoPane(this);                             //lint !e1013 !e534
    (void)m_pAnalysisToolBar->Create(IDD_DLG_ANALYSISTOOLBAR);
    m_pAnalysisToolBar->ShowWindow(SW_HIDE);
    (void)m_pAnalysisToolBar->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE | SWP_NOACTIVATE |SWP_HIDEWINDOW);
}

void CVideoPane::CreateTimerBar()
{
	std::string strBackGrondPath;
	strBackGrondPath.clear();
	(void)SkinMgr.InstantReplayBackGroudPngPath(strBackGrondPath);
	if(strBackGrondPath.empty())return;

	m_pPlayTimerBar = new CPlayTimerBar(strBackGrondPath.c_str());
	CHECK_POINTER_VOID(m_pPlayTimerBar);

	(void)m_pPlayTimerBar->Create(IDD_DLG_TIMERBAR);
	m_pPlayTimerBar->SetVideoPane(this);
	(void)m_pPlayTimerBar->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE |SWP_HIDEWINDOW);
	m_pPlayTimerBar->ShowWindow(SW_HIDE);

    // modi by wKF71044 at [2013-5-24 21:45:39]�Ѿ��ڶԻ����ʼ��ʱ������InitTimeRange
	//m_pPlayTimerBar->InitTimeRange();
	m_pPlayTimerBar->Invalidate(TRUE);
}

void CVideoPane::ShowInstantReplayBar(ULONG val)
{
	if (1 == val)
	{
		if (nullptr == m_pPlayTimerBar)
		{
			CreateTimerBar();
		}
	}
	CHECK_POINTER_VOID(m_pPlayTimerBar);
	CHECK_POINTER_VOID(m_pRealTimeToolBar);
	m_ulInstantReplayStyle = val;
	m_pPlayTimerBar->ResetRange();
	m_pRealTimeToolBar->SetToolBarVisableStatus(static_cast<ULONGLONG>(0x0000));
	m_pRealTimeToolBar->SetInstansPlayBack(TRUE);
	// ���ô�������
	SetTypeWnd(TYPE_WND_INSTANTBACKPLAY);
	//����¼��
	CIVS_OCXPlayerCtrl * pOcx =  GetOCXPlayerCtrl();
	CHECK_POINTER_VOID(pOcx);

	//һ�μ���200��¼��
	IVS_UINT32 uiTotal = 200;
	IVS_UINT32 uiBufSize = sizeof(IVS_RECORD_INFO_LIST)+sizeof(IVS_RECORD_INFO)*uiTotal;
	IVS_INT32 iRet = IVS_FAIL;
	IVS_CHAR* pRecordListBuf = IVS_NEW(pRecordListBuf, uiBufSize);
	if (NULL == pRecordListBuf)
	{
		iRet = static_cast<IVS_INT32>(IVS_ALLOC_MEMORY_ERROR);
		BP_RUN_LOG_ERR(iRet,"Show Instant ReplayBar Query Record List Failed.", "Reason:New pRecordListBuf Memory Failed.");
		return;
	}

	IVS_INDEX_RANGE stIndex;
	memset(&stIndex,0,sizeof(IVS_INDEX_RANGE));
	stIndex.uiFromIndex = static_cast<IVS_UINT32>(1);
	stIndex.uiToIndex = static_cast<IVS_UINT32>(200);
	IVS_RECORD_INFO_LIST* pRecordList = (IVS_RECORD_INFO_LIST*)pRecordListBuf;//lint !e826
	memset(pRecordList,0,uiBufSize);
	IVS_TIME_SPAN stTimeSpan;
	memset(&stTimeSpan,0,sizeof(IVS_TIME_SPAN));

	CTime timeCurrent = CTime::GetCurrentTime();
	CString strStartTime;
	CString strEndTime;

	struct tm local;
	memset(&local, 0, sizeof(tm));
	time_t currenttime;
	//ת��UTCʱ��
	currenttime = timeCurrent.GetTime();
	gmtime_s(&local, &currenttime);
	currenttime = mktime(&local);

	time_t starttime;
	local.tm_hour -= 1;
	starttime = mktime(&local);

	CTime newCurrentTime(currenttime);
	CTime newStartTime(starttime);

	strStartTime.Format("%04d%02d%02d%02d%02d%02d",newStartTime.GetYear(),newStartTime.GetMonth(),newStartTime.GetDay(),newStartTime.GetHour(),
		newStartTime.GetMinute(),newStartTime.GetSecond());
	strEndTime.Format("%04d%02d%02d%02d%02d%02d",newCurrentTime.GetYear(),newCurrentTime.GetMonth(),newCurrentTime.GetDay(),newCurrentTime.GetHour(),
		newCurrentTime.GetMinute(),newCurrentTime.GetSecond());

	if(!CToolsHelp::Memcpy(stTimeSpan.cStart,IVS_TIME_LEN,strStartTime,static_cast<unsigned int>(strStartTime.GetLength())))
	{
        IVS_DELETE(pRecordListBuf, MUILI);
		return;
	}
	if(!CToolsHelp::Memcpy(stTimeSpan.cEnd,IVS_TIME_LEN,strEndTime,static_cast<unsigned int>(strEndTime.GetLength())))
	{
        IVS_DELETE(pRecordListBuf, MUILI);
		return;
	}
	//ֻ��ƽ̨¼��
	if (nullptr == m_pPlayTimerBar)
	{
		IVS_DELETE(pRecordListBuf, MUILI);
		return;
	}

	//m_pPlayTimerBar->InitTimeRange();
	ST_TIME stFirstStartTime;
	memset(&stFirstStartTime,0,sizeof(ST_TIME));
	BOOL bHasRecord = FALSE;

	iRet = IVS_SDK_GetRecordList(pOcx->GetSDKSessionID(),m_szCameraID,static_cast<IVS_INT32>(0),&stTimeSpan,&stIndex,pRecordList,uiBufSize);
	if (IVS_SUCCEED == iRet)
	{
		int iTotalNum = (int)(pRecordList->uiTotal);
		int iLastScond = 24*3600;
		for (int iIndex=0;iIndex<iTotalNum && iIndex < (int)stIndex.uiToIndex;iIndex++)
		{
			IVS_RECORD_INFO sRecordInfo = pRecordList->stRecordInfo[iIndex];
			// ��ʼ������,�����̵����� //
			ST_LAYERTIME stGreen;
			memset(&stGreen,0,sizeof(ST_TIME));
			CString strStartTimeTemp;
			CString strEndTimeTemp;
			strStartTimeTemp.Format("%s",sRecordInfo.stTime.cStart);
			strEndTimeTemp.Format("%s",sRecordInfo.stTime.cEnd);

			if (!TransStrToTime(strStartTimeTemp,&stGreen.stStartTime))
			{
				continue;
			}

			if (!TransStrToTime(strEndTimeTemp,&stGreen.stEndTime))
			{
				continue;
			}

			int iScond = stGreen.stStartTime.iHour*3600+stGreen.stStartTime.iMinute*60+stGreen.stStartTime.iScond;
			if (iScond < iLastScond)
			{
				stFirstStartTime.iHour = stGreen.stStartTime.iHour;
				stFirstStartTime.iMinute = stGreen.stStartTime.iMinute;
				stFirstStartTime.iScond = stGreen.stStartTime.iScond;
				iLastScond = iScond;
				bHasRecord = TRUE;
                TRACE("recordStartTime:%d:%d:%d\n", stFirstStartTime.iHour, stFirstStartTime.iMinute, stFirstStartTime.iScond);
			}

			m_pPlayTimerBar->AddLayeTimeRange(1,stGreen);
            //m_pPlayTimerBar->UpdateView();
		}
	}
	IVS_DELETE(pRecordListBuf, MUILI);
	m_pPlayTimerBar->SetPlayCurrentTime(stFirstStartTime);
    m_pPlayTimerBar->SetStartPlayTime();
	//m_pPlayTimerBar->ResetTimer();
	//PostMessage(WM_RESET_INSTANTBAR_MSG,WPARAM(1),LPARAM(0));
	//ResetBarPosition();
	//m_pPlayTimerBar->ResetTimer();
	//m_pPlayTimerBar->SetFocus();
	//m_pPlayTimerBar->Invalidate(TRUE);
	m_pPlayTimerBar->MoveWindow(CRect(0,0,0,0));
	m_pPlayTimerBar->ShowWindow(SW_HIDE);
	//ResetBarPosition();
	//::PostMessage(m_pPlayTimerBar->m_hWnd,WM_LBUTTONUP,0,0);
	//m_pPlayTimerBar->ResetTimer();
	//PostMessage(WM_RESET_INSTANTBAR,0,0);

	//��ʼ�ط�
	if (bHasRecord)
	{
		//m_pPlayTimerBar->SetPlayCurrentTime(stFirstStartTime);
		(void)StartRecordPlay();
	}
	else
	{
		m_ulInstanstRealPlayHandle = GetPlayHandle();
		IVS_SDK_DelPlayWnd(GetOCXPlayerCtrl()->GetSDKSessionID(), GetPlayHandle(), GetVideoWnd());
	}
}


BOOL CVideoPane::TransStrToTime(const CString &str,ST_TIME* pTime)const
{
	if (NULL == pTime)
	{
		return FALSE;
	}

	if (str.GetLength()!=14)
	{
		return FALSE;
	}

	CString strTemp = str;
	CString strYear = strTemp.Mid(0,4);
	CString strMonth = strTemp.Mid(4,2);
	CString strDate = strTemp.Mid(6,2);
	CString strHour = strTemp.Mid(8,2);
	CString strMinute = strTemp.Mid(10,2);
	CString strScond = strTemp.Mid(12,2);

	int iYear = 0;
	int iMonth = 0;
	int iDate = 0;
	int iHour = 0;
	int iMinute = 0;
	int iScond = 0;

	(void)sscanf_s(strYear, STR_FORMAT_D, &iYear);
	(void)sscanf_s(strMonth, STR_FORMAT_D, &iMonth);
	(void)sscanf_s(strDate, STR_FORMAT_D, &iDate);
	(void)sscanf_s(strHour, STR_FORMAT_D, &iHour);
	(void)sscanf_s(strMinute, STR_FORMAT_D, &iMinute);
	(void)sscanf_s(strScond, STR_FORMAT_D, &iScond);

	CTime timeCurrent(iYear,iMonth,iDate,iHour,iMinute,iScond);
	//currenttime = timeCurrent.GetTime();


	//struct tm *local;
	time_t currenttime;
	//ת��UTCʱ��
	currenttime = timeCurrent.GetTime();
	//��ȡ����ʱ��
	SYSTEMTIME locTm,sysTm;
	GetLocalTime(&locTm);
	GetSystemTime(&sysTm);
	CTime timeLocal;
	CTime timeSystem;
	timeLocal = CTime((int)locTm.wYear,(int)locTm.wMonth,(int)locTm.wDay,
		(int)locTm.wHour,(int)locTm.wMinute,(int)locTm.wSecond,-1);
	timeSystem = CTime((int)sysTm.wYear,(int)sysTm.wMonth,(int)sysTm.wDay,
		(int)sysTm.wHour,(int)sysTm.wMinute,(int)sysTm.wSecond,-1);

	time_t localttime;
	time_t systemttime;
	localttime = timeLocal.GetTime();
	systemttime = timeSystem.GetTime();
	int iTimeZoom = (int)((localttime - systemttime)/3600);

	currenttime += iTimeZoom*3600;//lint !e647
	CTime newCurrentTime(currenttime);

	pTime->iHour = newCurrentTime.GetHour();
	pTime->iMinute = newCurrentTime.GetMinute();
	pTime->iScond = newCurrentTime.GetSecond();
	return TRUE;
}

void CVideoPane::SetBarRect()
{
    LONG width  = m_PaneRectFull.Width();

    m_TitleBarRect.top = m_PaneRectFull.top;
    m_TitleBarRect.left = m_PaneRectFull.left;
    m_TitleBarRect.bottom = m_PaneRectFull.top + TITLEBAR_HEIGHT;
    m_TitleBarRect.right = m_PaneRectFull.left + width;

	m_ToolBarRect.top = m_PaneRectFull.bottom - BOTTOMBAR_HEIGHT;
	m_ToolBarRect.left = m_PaneRectFull.left;
	m_ToolBarRect.bottom = m_PaneRectFull.bottom;
	m_ToolBarRect.right = m_PaneRectFull.left + width;

    // modi by wKF71044 at [2013-7-25 11:15:43] ʼ�ձ����ڵײ����������棬��ʹû��ʾ��ʱ��
	m_TVWallStatusBarRect.bottom = (m_PaneRectFull.bottom - 2) - BOTTOMBAR_HEIGHT;
	m_TVWallStatusBarRect.right = m_PaneRectFull.right - 4;
	m_TVWallStatusBarRect.top = m_TVWallStatusBarRect.bottom - TVWALLSTATUSBAR_HEIGHT;
	m_TVWallStatusBarRect.left = m_TVWallStatusBarRect.right - TVWALLSTATUSBAR_WIDTH;

	m_TurnCruisePaneRect = m_PaneRectFull;

	m_DigtalRect = m_PaneRectFull;
	if (m_pVideoPaneMgr->GetTitleBarShowState())
	{
		m_DigtalRect.top = m_PaneRectFull.top + TITLEBAR_HEIGHT;
	}
	
	if (m_pVideoPaneMgr->GetToolBarShowState())
	{
		m_DigtalRect.bottom = m_PaneRectFull.bottom - BOTTOMBAR_HEIGHT;
		//�����ʾtoolbar�������Ƶ���ǽBar
// 		m_TVWallStatusBarRect.bottom = m_ToolBarRect.bottom - BOTTOMBAR_HEIGHT;
// 		m_TVWallStatusBarRect.top = m_TVWallStatusBarRect.bottom - TVWALLSTATUSBAR_HEIGHT;
	}

	m_pZoomDirctRect.top = m_PaneRectFull.top + TITLEBAR_HEIGHT;
	m_pZoomDirctRect.left = m_PaneRectFull.left;
	m_pZoomDirctRect.bottom = m_PaneRectFull.bottom - BOTTOMBAR_HEIGHT;
	m_pZoomDirctRect.right = m_PaneRectFull.left + width;

	m_ZoomBarRect.top = m_PaneRectFull.bottom - BOTTOMBAR_HEIGHT;
	m_ZoomBarRect.left = m_PaneRectFull.left;
	m_ZoomBarRect.bottom = m_PaneRectFull.bottom;
	m_ZoomBarRect.right = m_PaneRectFull.left + width;

	m_ZoomTitleBarRect.top = m_PaneRectFull.top;
	m_ZoomTitleBarRect.left = m_PaneRectFull.left;
	m_ZoomTitleBarRect.bottom = m_PaneRectFull.top + TITLEBAR_HEIGHT;
	m_ZoomTitleBarRect.right = m_PaneRectFull.left + width;

	m_TimerBarRect.top = m_PaneRectFull.bottom - 3*BOTTOMBAR_HEIGHT;
	m_TimerBarRect.left = m_PaneRectFull.left;
	if (m_pVideoPaneMgr->GetToolBarShowState())
	{
		m_TimerBarRect.bottom = m_PaneRectFull.bottom - BOTTOMBAR_HEIGHT;
	}
	else
	{
		m_TimerBarRect.bottom = m_PaneRectFull.bottom;
	}
	
	m_TimerBarRect.right = m_PaneRectFull.left + width;

    //CParentWnd* pParentWnd = dynamic_cast <CParentWnd*>(GetParent());
	CParentWnd* pParentWnd = (CParentWnd*)m_pVideoPaneMgr->GetParentWnd();//lint !e1774
    CHECK_POINTER_VOID(pParentWnd);
    pParentWnd->ClientToScreen(&m_TitleBarRect);
    pParentWnd->ClientToScreen(&m_ToolBarRect);
	pParentWnd->ClientToScreen(&m_TVWallStatusBarRect);
	pParentWnd->ClientToScreen(&m_TurnCruisePaneRect);
	pParentWnd->ClientToScreen(&m_DigtalRect);
	pParentWnd->ClientToScreen(&m_pZoomDirctRect);
	pParentWnd->ClientToScreen(&m_ZoomBarRect);
	pParentWnd->ClientToScreen(&m_ZoomTitleBarRect);
	pParentWnd->ClientToScreen(&m_TimerBarRect);
}

bool CVideoPane::IsExistPlayHandle(unsigned long ulPlayHandle)const
{
    CHECK_EQUAL(ulPlayHandle,0,false);

    if (m_ulPlayHandle == ulPlayHandle)
    {
        return true;
    }
    return false;
}

void CVideoPane::PTZ3DMouseUp(const CRect &mainRect)
{
    CIVS_OCXPlayerCtrl *p_OcxCtrl = GetOCXPlayerCtrl();
    CHECK_POINTER_VOID(p_OcxCtrl);
    //LONG ulRet = IVS_SUCCEED;

    //������̨��λ��Ϣ
    CPoint firstPoint,secondPoint;
    double width = (double)mainRect.Width();
	double height = (double)mainRect.Height();

	//����������ͬ��Ϊ��ѡ������Ϊ��ѡ
	//if (m_lastPoint == m_startPoint )
	//{
	//    double minEdge = min(min(m_lastPoint.x,m_lastPoint.y),min(width - m_lastPoint.x,height - m_lastPoint.y));
	//    firstPoint.x = (LONG)((CIF_WIDTH / width) * (m_lastPoint.x - minEdge));
	//    firstPoint.y = (LONG)((CIF_HEIGHT / height) * (m_lastPoint.y - minEdge));
	//    secondPoint.x = (LONG)((CIF_WIDTH / width) * (m_lastPoint.x + minEdge));
	//    secondPoint.y = (LONG)((CIF_HEIGHT / height) * (m_lastPoint.y + minEdge));
	//} else {
	firstPoint.x = (LONG)((CIF_WIDTH / width) * m_startPoint.x);
	firstPoint.y = (LONG)((CIF_HEIGHT / height) * m_startPoint.y);
	secondPoint.x = (LONG)((CIF_WIDTH / width) * m_lastPoint.x);
	secondPoint.y = (LONG)((CIF_HEIGHT / height) * m_lastPoint.y);
	//}

	//����sdk�Ľӿ�
	TCHAR para[16]={0};
	if(!CToolsHelp::Snprintf(para,16,_T("%d,%d\0"), firstPoint.x, firstPoint.y))//lint !e840
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,_T("IVS_OCX::"),_T("VideoPane ptz3d mouse up.Failed"));
		return;
	}
    CString csPara1(para);
    if(!CToolsHelp::Snprintf(para,16,"%d,%d\0", secondPoint.x, secondPoint.y))//lint !e840
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,_T("IVS_OCX::"),_T("VideoPane ptz3d mouse up.Failed"));
        return;
    }

    CString csPara2(para);

    TCHAR szCameraCode[IVS_DEV_CODE_LEN] = {0};
    CString strCameraCode(GetCameraID());
    if(!CToolsHelp::Memcpy(szCameraCode,IVS_DEV_CODE_LEN,strCameraCode.GetBuffer(0),static_cast<size_t>(strCameraCode.GetLength())))
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,_T("IVS_OCX::"),_T("VideoPane ptz3d mouse up.Failed.Failed"));
        return;
    }

    if (!m_bIsButtonDown)
    {
        return;
    }

	IVS_UINT32 uiLockStatus = 0;
	IVS_INT32 iRet = IVS_SDK_PtzControl(GetOCXPlayerCtrl()->GetSDKSessionID(),szCameraCode, PTZ_FAST_LOCATE, csPara1.GetBuffer(0), csPara2.GetBuffer(0), &uiLockStatus);

	// ������ɹ�����Ҫ����Ϣ��CU
	if (IVS_SUCCEED != iRet)
	{
		CXml xml;
		(void)xml.AddDeclaration("1.0","UTF-8","");
		(void)xml.AddElem("Content");
		(void)xml.IntoElem();
		(void)xml.AddChildElem("ResultCode");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(CToolsHelp::Int2Str(iRet).c_str());
		(void)xml.AddElem("LockStatus");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(CToolsHelp::Int2Str((IVS_INT32)uiLockStatus).c_str());

		unsigned int len = 0;
		const char* pXml = xml.GetXMLStream(len);
		CHECK_POINTER_VOID(pXml);
		p_OcxCtrl->IVS_OCX_Windows_Event(IVS_EVENT_PTZ_LOCK, pXml);
	}

}

// ��ʼ��ͼ����ͼ������
LONG CVideoPane::SetCurrentGraphicAttribute(const IA_GRAPHICS *pGraphicAttribute, ULONG ulGroupDrawMaxNum)
{
	CHECK_POINTER(pGraphicAttribute, IVS_PARA_INVALID);
	CHECK_POINTER(m_pAnalysisToolBar, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_ulDrawHandle, IVS_FAIL);
	DRAW_HANDLE h = (DRAW_HANDLE *)m_ulDrawHandle;

    if(!CToolsHelp::Memcpy(&m_CurrentGraphic,sizeof(IA_GRAPHICS), pGraphicAttribute, sizeof(IA_GRAPHICS)))
    {
    	BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"IVS_OCX::","VideoPane set current graphic attribute.Faile");
        return IVS_ALLOC_MEMORY_ERROR;
    }

	m_ulGraphicState		= GRAPHICS_STATE_EDITING;
	m_ulCurrentGraphicGroup = pGraphicAttribute->ulGraphicGroup;
	m_ulCurrentGraphicType  = pGraphicAttribute->ulGraphicType;
	m_ulGroupDrawMaxNum		= ulGroupDrawMaxNum;

	if (!Draw_IsFinishDrawGraphicGroup(h, m_ulCurrentGraphicGroup, m_ulGroupDrawMaxNum))
	{
		m_pAnalysisToolBar->SetToolbarEnableStatus((ULONGLONG)(0x01 | 0x02), (ULONGLONG)TRUE);
	}

	return IVS_SUCCEED;
}

VOID CVideoPane::SetCurrentGraphicType(ULONG ulGraphicType)
{
	m_ulCurrentGraphicType = ulGraphicType;
}

VOID CVideoPane::SetCurrentGraphicGroup(ULONG ulGraphicGroup)
{
	m_ulCurrentGraphicGroup = ulGraphicGroup;
}

VOID CVideoPane::SetCurrentGraphicState(ULONG ulGraphicState)
{
	m_ulGraphicState = ulGraphicState;
}

ULONG CVideoPane::GetCurrentGraphicState()const
{
	return m_ulGraphicState;
}

VOID CVideoPane::SetDeleteButtonState(BOOL bAllowDelete)
{
	CHECK_POINTER_VOID(m_pAnalysisToolBar);

	if (bAllowDelete)
	{
		m_pAnalysisToolBar->SetToolbarEnableStatus((ULONGLONG)0x04, (ULONGLONG)TRUE);
	}else{
		m_pAnalysisToolBar->SetToolbarEnableStatus((ULONGLONG)0x04, (ULONGLONG)FALSE);
	}
}

static IVS_VOID __SDK_CALL DrawProc(IVS_VOID *hDc, IVS_VOID *rc, ULONG64 ulFrameID, IVS_VOID *pUser)
{
	CVideoPane* pVideoPane = (CVideoPane*)pUser;
	CHECK_POINTER_VOID(pVideoPane);
	DRAW_HANDLE h = (DRAW_HANDLE)pVideoPane->GetDrawHandle();
	if (!h)
	{
		return;
	}
	// �ֱ��ʷ����仯���˳��ֲ��Ŵ�
	RECT* pR = (RECT *)rc;
	LONG lDrawHeight = pVideoPane->GetDrawHeight();
	LONG lDrawWidth = pVideoPane->GetDrawWidth();
	if (NULL != pR)
	{
		do 
		{
			// ��һ�� ����ֵ
			if (lDrawHeight == 0 && lDrawWidth == 0)
			{
				lDrawHeight = pR->bottom - pR->top;
				lDrawWidth = pR->right - pR->left;
				pVideoPane->SetDrawHeight(lDrawHeight);
				pVideoPane->SetDrawWidth(lDrawWidth);
				break;
			}

			// �ж��Ƿ�ı�,�رվֲ��Ŵ�
			if (lDrawHeight != (pR->bottom - pR->top) || lDrawWidth != (pR->right - pR->left))
			{
                // DTS2013071805291 
                // ֻ���ھֲ��Ŵ��Ѿ�������ǰ���£��Ž���PostMessage��ز����������豸����->�ƶ�������ã���������쳣���ֱ��ʱ䶯�����ͻ��������
                if (MAGNIFY_OPEN == pVideoPane->GetRealtimeMagnifyState())
                {
                    //::PostMessage(pVideoPane->GetVideoRealtimeBar()->GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(IDC_BTN_TOOL_REALTIME_MAGNIFY, BN_CLICKED), (LPARAM)pVideoPane->GetVideoRealtimeBar()->GetMagnifyBtnHwnd());
                    ::PostMessage(pVideoPane->GetSafeHwnd(), WM_MAGNIFY_CLOSE_ALL, 0, 0);
                    pVideoPane->SetDrawHeight(0);
                    pVideoPane->SetDrawWidth(0);
                }
			}
		} while (0); //lint !e717
	}

	(void)Draw_ShowGraphics(h, (HDC)hDc, (RECT *)rc);
}//lint !e818 !e715

static IVS_VOID __SDK_CALL OverlayProc(IVS_VOID *hDc, IVS_VOID *rc, ULONG64 ulFrameID, IVS_VOID *pUser)
{
    DRAW_HANDLE h = (DRAW_HANDLE)pUser;

    if (!h)
    {
        return;
    }
    (void)IATrack_ShowIATrack(h, (HDC)hDc, (RECT *)rc, ulFrameID);
}//lint !e818

// �������ܷ�����ͼ
IVS_INT32 CVideoPane::EnableDraw(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle)
{
	HWND hMsgWnd = GetVideoWnd();
	CHECK_POINTER(hMsgWnd, IVS_FAIL);

	if (!m_ulDrawHandle)
	{
		DRAW_HANDLE  h = Draw_GetHandle();
		if (NULL == h)
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Enable Draw", "No Memory");
			return IVS_ALLOC_MEMORY_ERROR;
		}
		LONG lRet = Draw_Init(h, hMsgWnd);
		if (IVS_SUCCEED != lRet)
		{
			return lRet;
		}
		m_ulDrawHandle = (unsigned long)h ;
	}

    m_bDrawEnabled = true;
	//return IVS_SDK_SetDrawCallBack(iSessionID, ulPlayHandle, DrawProc, (void *)m_ulDrawHandle);
	return IVS_SDK_SetDrawCallBackEx(iSessionID, ulPlayHandle, GetVideoWnd(), DrawProc, (void *)this);
}

// �ر����ܷ�����ͼ
IVS_INT32 CVideoPane::DisableDraw(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle)
{
	if (NULL == m_ulDrawHandle)
	{
		return IVS_SUCCEED;
	}
    m_bDrawEnabled = false;
	// ɾ���ص�
	//return  IVS_SDK_SetDrawCallBack(iSessionID, ulPlayHandle, NULL, NULL);

 	return IVS_SDK_SetDrawCallBackEx(iSessionID, ulPlayHandle, GetVideoWnd(), NULL, NULL);
}

// ����ͼƬ
IVS_INT32 CVideoPane::SetGraphics(const IA_GRAPHICS *pGraphics, ULONG ulNum, BOOL bPersent)
{
	CHECK_POINTER(m_ulDrawHandle, IVS_FAIL);
	CHECK_POINTER(pGraphics, IVS_PARA_INVALID);
	if (0 == ulNum)
	{
		return IVS_PARA_INVALID;
	}
	DRAW_HANDLE h = (DRAW_HANDLE *)m_ulDrawHandle;
	return Draw_SetGraphics(h, pGraphics, ulNum, bPersent);
}

//�������ܷ���ͼ����֮��Ĺ�ϵ
IVS_INT32 CVideoPane::SetRelationShip(ULONG ulGraphicGroupA, ULONG ulGraphicGroupB, RELATION_SHIP_TYPE ulRelationShipType)
{
	CHECK_POINTER(m_ulDrawHandle, IVS_FAIL);
	DRAW_HANDLE h = (DRAW_HANDLE *)m_ulDrawHandle;
	return Draw_SetRelationShip(h, ulGraphicGroupA, ulGraphicGroupB, ulRelationShipType);
}

// ��ȡͼ��
IVS_INT32 CVideoPane::GetGraphics(IA_GRAPHICS *pGraphics, ULONG *pulNum)
{
	CHECK_POINTER(m_ulDrawHandle, IVS_FAIL);
	CHECK_POINTER(pGraphics, IVS_PARA_INVALID);
	if (0 == pulNum)
	{
		return IVS_PARA_INVALID;
	}

	DRAW_HANDLE h = (DRAW_HANDLE *)m_ulDrawHandle;
	return Draw_GetGraphicsAll(h, pGraphics, pulNum);
}

// ��ȡѡ�е����ܷ���ͼ��
IVS_INT32 CVideoPane::GetGraphicSelected(IA_GRAPHICS *pGraphic)
{
	CHECK_POINTER(m_ulDrawHandle, IVS_FAIL);
	CHECK_POINTER(pGraphic, IVS_PARA_INVALID);
	DRAW_HANDLE h = (DRAW_HANDLE *)m_ulDrawHandle;
	return Draw_GetGraphicSelected(h, pGraphic);
}

// �����ȡ���ܷ���ͼ��
IVS_INT32 CVideoPane::GetGraphicsByGroup(ULONG ulGraphicGroup, IA_GRAPHICS *pGraphics, ULONG *pulNum)
{
	CHECK_POINTER(m_ulDrawHandle, IVS_FAIL);
	CHECK_POINTER(pGraphics, IVS_PARA_INVALID);
	if (0 == pulNum)
	{
		return IVS_PARA_INVALID;
	}

	DRAW_HANDLE h = (DRAW_HANDLE *)m_ulDrawHandle;
	return Draw_GetGraphicsByGroup(h, ulGraphicGroup, pGraphics, pulNum);
}

// ɾ��ѡ��ͼƬ
IVS_INT32 CVideoPane::DeleteGraphicsSelected()
{
	CHECK_POINTER(m_ulDrawHandle, IVS_FAIL);
	DRAW_HANDLE h = (DRAW_HANDLE *)m_ulDrawHandle;
	IVS_INT32 iRet = Draw_DeleteGraphicsSelected(h);
    if ((IVS_SUCCEED == iRet) && (NULL != m_pAnalysisToolBar))
    {
		if (Draw_IsFinishDrawGraphicGroup(h, m_ulCurrentGraphicGroup, m_ulGroupDrawMaxNum))
		{
			m_pAnalysisToolBar->SetToolbarEnableStatus((ULONGLONG)(0x01 | 0x02), (ULONGLONG)FALSE);
		} else {
			m_pAnalysisToolBar->SetToolbarEnableStatus((ULONGLONG)(0x01 | 0x02), (ULONGLONG)TRUE);
		}
		m_pAnalysisToolBar->SetToolbarEnableStatus((ULONGLONG)0x04, (ULONGLONG)FALSE);
    }
    return iRet;
}

// ɾ��ָ��ͼ���������ͼ��
IVS_INT32 CVideoPane::DeleteGraphicsByGroup(ULONG ulGraphicGroup)
{
	CHECK_POINTER(m_ulDrawHandle, IVS_FAIL);
    CHECK_POINTER(m_pAnalysisToolBar, IVS_OPERATE_MEMORY_ERROR);
    if (ulGraphicGroup == m_ulCurrentGraphicGroup)
    {
        m_ulGraphicState = GRAPHICS_STATE_NONE;
        m_ulCurrentGraphicGroup = 0;
        m_pAnalysisToolBar->SetToolbarEnableStatus((ULONGLONG)(0x01 | 0x02 | 0x04), (ULONGLONG)FALSE);

    }
	DRAW_HANDLE h = (DRAW_HANDLE *)m_ulDrawHandle;
	return Draw_DeleteGraphicsByGroup(h, ulGraphicGroup);
}

// ɾ������ͼƬ
IVS_INT32 CVideoPane::DeleteGraphics()
{
	CHECK_POINTER(m_ulDrawHandle, IVS_FAIL);
	DRAW_HANDLE h = (DRAW_HANDLE *)m_ulDrawHandle;
	return Draw_DeleteGraphics(h);
}

//���ܷ�������ĳͼ����Ļ�ͼ�����������ڻ��ĸ���ͼ�Σ�ֱ�Ӷ�����
IVS_INT32 CVideoPane::StopDrawGraphics(ULONG ulGraphicGroup)
{
	CHECK_POINTER(m_ulDrawHandle, IVS_FAIL);
	CHECK_POINTER(m_pAnalysisToolBar, IVS_OPERATE_MEMORY_ERROR);

	m_ulGraphicState	   = GRAPHICS_STATE_NONE;
	m_ulCurrentGraphicType = IA_GRAPHICS_TYPE_ILLEGAL;
	m_ulGroupDrawMaxNum	   = 0;
	memset(&m_CurrentGraphic, 0, sizeof(IA_GRAPHICS));
	m_pAnalysisToolBar->SetToolbarEnableStatus((ULONGLONG)(0x01 | 0x02), (ULONGLONG)FALSE);

	DRAW_HANDLE h = (DRAW_HANDLE *)m_ulDrawHandle;
	return Draw_StopDrawGraphics(h, ulGraphicGroup);
}
//���ܷ�������ĳͼ����Ļ�ͼ�����������ڻ��ĸ���ͼ�Σ�ֱ�Ӷ�����
IVS_INT32 CVideoPane::StopDrawGraphicWhileDraw(ULONG ulGraphicGroup)
{
	CHECK_POINTER(m_ulDrawHandle, IVS_FAIL);
// 	m_ulGraphicState	   = GRAPHICS_STATE_NONE;
// 	m_ulCurrentGraphicType = IA_GRAPHICS_TYPE_ILLEGAL;
// 	m_ulGroupDrawMaxNum	   = 0;
// 	memset(&m_CurrentGraphic, 0, sizeof(IA_GRAPHICS));

	DRAW_HANDLE h = (DRAW_HANDLE *)m_ulDrawHandle;
	return Draw_StopDrawGraphics(h, ulGraphicGroup);
}

//ѡ��ָ��ͼ���������ͼ�Σ�׼���޸�
IVS_INT32 CVideoPane::SelectGraphicByGroup(ULONG ulGraphicGroup)
{
	CHECK_POINTER(m_ulDrawHandle, IVS_FAIL);
	m_ulGraphicState = GRAPHICS_STATE_EDITING; 
	m_ulCurrentGraphicGroup = ulGraphicGroup;

	DRAW_HANDLE h = (DRAW_HANDLE *)m_ulDrawHandle;
	BOOL bAllowDelete = FALSE;
	IVS_INT32 iRet = Draw_SelectGraphicByGroup(h, ulGraphicGroup, &bAllowDelete);
	SetDeleteButtonState(bAllowDelete);
	
	return iRet;
}

// ����ѡ��ͼ��
IVS_INT32 CVideoPane::SelectGraphicByPoint(const POINT &CurrentPoint)
{
	CHECK_POINTER(m_ulDrawHandle, IVS_OPERATE_MEMORY_ERROR);
// 	CHECK_POINTER_VOID(m_pAnalysisToolBar);  �ƶ����ȱ༭ͼ��ʱ�����ܷ���������
	CIVS_OCXPlayerCtrl *pPlayerCtrl = GetOCXPlayerCtrl();
	CHECK_POINTER(pPlayerCtrl, IVS_OPERATE_MEMORY_ERROR);

	// ���ݵ�����ѡ��ͼ��
	BOOL bAllowDelete = FALSE;
	ULONG ulSelectedGraphicGroup = 0;
	CString xmlToCU;
	IVS_INT32 iResult = Draw_SelectGraphicByPoint((DRAW_HANDLE)m_ulDrawHandle, 
							CurrentPoint, &ulSelectedGraphicGroup, &bAllowDelete);
	if (IVS_SUCCEED == iResult)// ѡ��ͼ��
	{
		m_ulGraphicState = GRAPHICS_STATE_EDITING;
        if (NULL != m_pAnalysisToolBar)
        {
			// ѡ�л�ͼͼ���飬��δ�ﵽ���ͼ������ͼ��������
			if ((ulSelectedGraphicGroup == m_CurrentGraphic.ulGraphicGroup) 
				&& (!Draw_IsFinishDrawGraphicGroup((DRAW_HANDLE)m_ulDrawHandle, m_CurrentGraphic.ulGraphicGroup, m_ulGroupDrawMaxNum)))
			{
				m_pAnalysisToolBar->SetToolbarEnableStatus((ULONGLONG)(0x01 | 0x02), (ULONGLONG)TRUE);
			} else { 
				m_pAnalysisToolBar->SetToolbarEnableStatus((ULONGLONG)(0x01 | 0x02), (ULONGLONG)FALSE);
			}

			COCXDrawXMLProcess::SelectGraphicGetXML(xmlToCU, ulSelectedGraphicGroup);
            pPlayerCtrl->IVS_OCX_Windows_Event(EVENT_INTELLIGENT_SELECT_GRAPHIC, (LPCTSTR)xmlToCU);
            SetDeleteButtonState(bAllowDelete);
        }
		m_ulCurrentGraphicGroup = ulSelectedGraphicGroup;
	} else { // û��ѡ��ͼ��
        if (NULL != m_pAnalysisToolBar)
        {
			m_pAnalysisToolBar->SetToolbarEnableStatus((ULONGLONG)(/*0x01 | 0x02 | */0x04), (ULONGLONG)FALSE);
			// �ǻ�ͼͼ����ȥѡ��ʱ����CU����ȥѡ����Ϣ
			if (m_ulCurrentGraphicGroup && (m_ulCurrentGraphicGroup != m_CurrentGraphic.ulGraphicGroup))
			{
				COCXDrawXMLProcess::UnSelectGraphicGetXML(xmlToCU);
				pPlayerCtrl->IVS_OCX_Windows_Event(EVENT_INTELLIGENT_UNSELECT_GRAPHIC, (LPCTSTR)xmlToCU);
			}
        }
		m_ulGraphicState = GRAPHICS_STATE_NONE;
		m_ulCurrentGraphicGroup = 0;
	}
    return iResult;
}

//ȡ��ѡ�����ܷ���ͼ��
IVS_INT32 CVideoPane::SelectCancel()
{
	CHECK_POINTER(m_ulDrawHandle, IVS_FAIL);
	CHECK_POINTER(m_pAnalysisToolBar, IVS_OPERATE_MEMORY_ERROR);

	m_ulGraphicState = GRAPHICS_STATE_NONE;
    m_ulCurrentGraphicGroup = 0;
	m_pAnalysisToolBar->SetToolbarEnableStatus((ULONGLONG)(0x01 | 0x02 | 0x04), (ULONGLONG)FALSE);

	DRAW_HANDLE h = (DRAW_HANDLE *)m_ulDrawHandle;
	return Draw_SelectCancel(h);
}

//ȡ��ѡ�����ܷ���ͼ��
IVS_INT32 CVideoPane::SelectCancelWhileDraw()
{
    CHECK_POINTER(m_ulDrawHandle, IVS_FAIL);

    DRAW_HANDLE h = (DRAW_HANDLE *)m_ulDrawHandle;
    return Draw_SelectCancel(h);
}

VOID CVideoPane::GetCursorType(const POINT &Point)
{
	if (NULL == m_ulDrawHandle)
	{
		return;
	}
	DRAW_HANDLE h = (DRAW_HANDLE *)m_ulDrawHandle;
	m_ulCursorType = (ULONG)Draw_GetCursorType(h, Point);
}

VOID CVideoPane::SetCursorType()const
{
	if (NULL == m_ulDrawHandle)
	{
		return;
	}

	switch (m_ulCursorType)
	{
	case CURSOR_TYPE_ARROW:
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		break;
	case CURSOR_TYPE_SIZEALL:
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
		break;
	case CURSOR_TYPE_SIZENESW:
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
		break;
	case CURSOR_TYPE_SIZENS:
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
		break;
	case CURSOR_TYPE_SIZENWSE:
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
		break;
	case CURSOR_TYPE_SIZEWE:
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		break;
	default:
		break;
	}
}


// �ж�ѡ��ͼ���Ƿ�ɹ�
// #define CHECK_DRAW_SELECT_SUCCESS(s)
// 	do { if(IVS_SUCCEED == s) m_ulGraphicState = GRAPHICS_STATE_EDITING;}while(0)

void CVideoPane::DrawLButtonDown(const CPoint &point )
{
	CHECK_POINTER_VOID(m_ulDrawHandle);
    CHECK_POINTER_VOID(m_pVideoPaneMgr);
    UINT scale = m_pVideoPaneMgr->GetDispalyScale();
	BOOL bScaleModeFull = scale == DISPLAY_SCALE_FULL ? TRUE : FALSE;
	(VOID)Draw_SetDisplayScaleMode((DRAW_HANDLE)m_ulDrawHandle, bScaleModeFull);
	SetCursorType();
	POINT CurrentPoint;
	CurrentPoint.x = point.x;
	CurrentPoint.y = point.y;
	LONG lRet = IVS_FAIL;
	if (GRAPHICS_STATE_DRAWING == m_ulGraphicState)// ��ͼ״̬
	{
		m_CurrentGraphic.IAPoints[0] = CurrentPoint;
		m_CurrentGraphic.ulGraphicType  = m_ulCurrentGraphicType;
// 		m_CurrentGraphic.ulGraphicGroup = m_ulCurrentGraphicGroup;
		switch (m_ulCurrentGraphicType)
		{
		case IA_GRAPHICS_TYPE_STRAIGHT_LINE:
			lRet = Draw_StraightLineStart((DRAW_HANDLE)m_ulDrawHandle, m_ulGroupDrawMaxNum, &m_CurrentGraphic);
			CHECK_DRAW_START_SUCCESS(lRet);
			break;
		case IA_GRAPHICS_TYPE_RECTANGLE:
			lRet = Draw_RectangleStart((DRAW_HANDLE)m_ulDrawHandle, m_ulGroupDrawMaxNum, &m_CurrentGraphic);
			CHECK_DRAW_START_SUCCESS(lRet);
			break;
		case IA_GRAPHICS_TYPE_POLYGON:
			if (!m_bIsDrawing)// ��û�п�ʼ��
			{
                lRet = Draw_PolygonStart((DRAW_HANDLE)m_ulDrawHandle, m_ulGroupDrawMaxNum, &m_CurrentGraphic);
                CHECK_DRAW_START_SUCCESS(lRet);
			} else {// ���ڻ�ͼ
				lRet = Draw_PolygonSetPoint((DRAW_HANDLE)m_ulDrawHandle, CurrentPoint, m_CurrentGraphic.ulGraphicGroup);
// 			    CHECK_DRAW_START_SUCCESS(lRet);
            }
			break;
		case IA_GRAPHICS_TYPE_UP_ULINE:
		case IA_GRAPHICS_TYPE_DOWN_ULINE:
			if (!m_bIsDrawing)// ��û�п�ʼ��
			{
				lRet = Draw_ULineStart((DRAW_HANDLE)m_ulDrawHandle, m_ulGroupDrawMaxNum, &m_CurrentGraphic);
                CHECK_DRAW_START_SUCCESS(lRet);
			} else { // ���ڻ�ͼ
				lRet = Draw_ULineSetPoint((DRAW_HANDLE)m_ulDrawHandle, CurrentPoint, m_CurrentGraphic.ulGraphicGroup);
//              CHECK_DRAW_START_SUCCESS(lRet);
			}
			break;
		default:
			break;
		}
	}
	else if (GRAPHICS_STATE_EDITING == m_ulGraphicState)// �༭״̬
	{
		if (!m_bIsDrawing)
		{
			(void)Draw_SelectCancel((DRAW_HANDLE)m_ulDrawHandle);
	// 		BOOL bAllowDelete = FALSE;
	//      (void)Draw_SelectGraphicByPoint((DRAW_HANDLE)m_ulDrawHandle, CurrentPoint, &m_ulCurrentGraphicGroup, &bAllowDelete);
	// 		SetDeleteButtonState(bAllowDelete);
			lRet = (LONG)SelectGraphicByPoint(CurrentPoint);
			if (IVS_SUCCEED != lRet)
				return;
			lRet = Draw_StartModifyGraphics((DRAW_HANDLE)m_ulDrawHandle, CurrentPoint, LEFT_BUTTON_DOWN, m_ulCurrentGraphicGroup);
			CHECK_DRAW_START_SUCCESS(lRet);
		}
	}
    else
    {
		(void)SelectGraphicByPoint(CurrentPoint);
	}
}  //lint !e438  lRet ��ʹ�õ�

void CVideoPane::DrawLButtonUp(const CPoint &point)
{
	// ���¼���CU�Ķ���
	CString xmlToCU = "";
	CIVS_OCXPlayerCtrl *pCtrl = this->GetOCXPlayerCtrl();

	CHECK_POINTER_VOID(m_ulDrawHandle);
    CHECK_POINTER_VOID(m_pVideoPaneMgr);
    UINT scale = m_pVideoPaneMgr->GetDispalyScale();
	BOOL bScaleModeFull = scale == DISPLAY_SCALE_FULL ? TRUE : FALSE;
	(VOID)Draw_SetDisplayScaleMode((DRAW_HANDLE)m_ulDrawHandle, bScaleModeFull);

    LONG lRet = IVS_FAIL;
	POINT CurrentPoint;
	CurrentPoint.x = point.x;
	CurrentPoint.y = point.y;
	if (GRAPHICS_STATE_DRAWING == m_ulGraphicState)// ��ͼ״̬
	{
		switch (m_ulCurrentGraphicType)
		{
		case IA_GRAPHICS_TYPE_STRAIGHT_LINE:
			lRet = Draw_StraightLineFinish((DRAW_HANDLE)m_ulDrawHandle, CurrentPoint, m_CurrentGraphic.ulGraphicGroup);
// 			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
// 			m_ulCursorType = CURSOR_TYPE_ARROW;
			m_bIsDrawing = false;
			m_ulGraphicState = GRAPHICS_STATE_EDITING;
            SetDeleteButtonState(m_CurrentGraphic.bAllowDelete);
			break;
		case IA_GRAPHICS_TYPE_RECTANGLE:
			lRet = Draw_RectangleFinish((DRAW_HANDLE)m_ulDrawHandle, CurrentPoint, m_CurrentGraphic.ulGraphicGroup);
// 			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
// 			m_ulCursorType = CURSOR_TYPE_ARROW;
			// �׽����¼���CU add by z00193167
			if (NULL != pCtrl)
			{
				pCtrl->IVS_OCX_Windows_Event(IVS_EVENT_IA_DRAW_OVER,(LPCTSTR)xmlToCU);
			}

			m_bIsDrawing = false;
			m_ulGraphicState = GRAPHICS_STATE_EDITING;
			if ((NULL != m_pAnalysisToolBar) 
				&& (Draw_IsFinishDrawGraphicGroup((DRAW_HANDLE)m_ulDrawHandle, m_CurrentGraphic.ulGraphicGroup, m_ulGroupDrawMaxNum)))
			{
				m_pAnalysisToolBar->SetToolbarEnableStatus((ULONGLONG)(0x01 | 0x02), (ULONGLONG)FALSE);
			}
            SetDeleteButtonState(m_CurrentGraphic.bAllowDelete);
			break;
		case IA_GRAPHICS_TYPE_POLYGON:// ���̧��Զ���λ�ͼ��������
		case IA_GRAPHICS_TYPE_UP_ULINE:// ���̧���U���߻�ͼ��������
		case IA_GRAPHICS_TYPE_DOWN_ULINE:
            lRet = IVS_SUCCEED;
            break;
		default:
			break;
		}
        if (IVS_SUCCEED != lRet)
        {
            (void)StopDrawGraphicWhileDraw(m_ulCurrentGraphicGroup);
            m_bIsDrawing = false;
            m_ulGraphicState = GRAPHICS_STATE_EDITING;
        }
	}
	else if (GRAPHICS_STATE_EDITING == m_ulGraphicState)// �༭״̬
	{
		(void)Draw_ModifyFinish((DRAW_HANDLE)m_ulDrawHandle, m_ulCurrentGraphicGroup, CurrentPoint);
		m_bIsDrawing = false;

		// �׽����¼���CU add by z00193167
		if (NULL != pCtrl)
		{
			pCtrl->IVS_OCX_Windows_Event(IVS_EVENT_IA_DRAW_OVER,(LPCTSTR)xmlToCU);
		}
	}
}

void CVideoPane::DrawMouseMove(const CPoint &point)
{
    CHECK_POINTER_VOID(m_ulDrawHandle);
    CHECK_POINTER_VOID(m_pVideoPaneMgr);
    UINT scale = m_pVideoPaneMgr->GetDispalyScale();
	BOOL bScaleModeFull = scale == DISPLAY_SCALE_FULL ? TRUE : FALSE;
	(VOID)Draw_SetDisplayScaleMode((DRAW_HANDLE)m_ulDrawHandle, bScaleModeFull);

	POINT CurrentPoint;
	CurrentPoint.x = point.x;
	CurrentPoint.y = point.y;

	if (GRAPHICS_STATE_DRAWING == m_ulGraphicState)// ��ͼ״̬
	{
		switch (m_ulCurrentGraphicType)
		{
		case IA_GRAPHICS_TYPE_STRAIGHT_LINE:
			(void)Draw_StraightLineUpDate((DRAW_HANDLE)m_ulDrawHandle, CurrentPoint);
			break;
		case IA_GRAPHICS_TYPE_RECTANGLE:
			(void)Draw_RectangleUpDate((DRAW_HANDLE)m_ulDrawHandle, CurrentPoint);
			break;
		case IA_GRAPHICS_TYPE_POLYGON:
			(void)Draw_PolygonUpDate((DRAW_HANDLE)m_ulDrawHandle, CurrentPoint, m_ulCurrentGraphicGroup);
			break;
		case IA_GRAPHICS_TYPE_UP_ULINE:
		case IA_GRAPHICS_TYPE_DOWN_ULINE:
			(void)Draw_ULineUpDate((DRAW_HANDLE)m_ulDrawHandle, CurrentPoint);
			break;
		default:
			break;
		}
	}
	else if (GRAPHICS_STATE_EDITING == m_ulGraphicState)// �༭״̬
	{
		(void)Draw_ModifyShapeUpDate((DRAW_HANDLE)m_ulDrawHandle, m_ulCurrentGraphicGroup, CurrentPoint);
	}
}

void CVideoPane::DrawRButtonDown(const CPoint &point)
{
	CHECK_POINTER_VOID(m_ulDrawHandle);
    CHECK_POINTER_VOID(m_pVideoPaneMgr);
	LONG lRet = IVS_FAIL;
    UINT scale = m_pVideoPaneMgr->GetDispalyScale();;
	BOOL bScaleModeFull = scale == DISPLAY_SCALE_FULL ? TRUE : FALSE;
	(VOID)Draw_SetDisplayScaleMode((DRAW_HANDLE)m_ulDrawHandle, bScaleModeFull);
	SetCursorType();

	POINT CurrentPoint;
	CurrentPoint.x = point.x;
	CurrentPoint.y = point.y;
	if (GRAPHICS_STATE_DRAWING == m_ulGraphicState)// ��ͼ״̬
	{
		if (!m_bIsDrawing)// ���ڻ�ͼ
		{
			return;
		}
		switch (m_ulCurrentGraphicType)
		{
		case IA_GRAPHICS_TYPE_POLYGON:
			lRet = Draw_PolygonFinish((DRAW_HANDLE)m_ulDrawHandle, CurrentPoint, m_CurrentGraphic.ulGraphicGroup);
			m_ulCursorType = CURSOR_TYPE_ARROW;
// 			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
// 			m_ulCursorType = CURSOR_TYPE_ARROW;
			m_bIsDrawing = false;
			m_ulGraphicState = GRAPHICS_STATE_EDITING;
			if ((NULL != m_pAnalysisToolBar) 
				&& (Draw_IsFinishDrawGraphicGroup((DRAW_HANDLE)m_ulDrawHandle, m_CurrentGraphic.ulGraphicGroup, m_ulGroupDrawMaxNum)))
			{
				m_pAnalysisToolBar->SetToolbarEnableStatus((ULONGLONG)(0x01 | 0x02), (ULONGLONG)FALSE);
			}
            SetDeleteButtonState(m_CurrentGraphic.bAllowDelete);
			break;
		case IA_GRAPHICS_TYPE_UP_ULINE:
		case IA_GRAPHICS_TYPE_DOWN_ULINE:
// 			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
// 			m_ulCursorType = CURSOR_TYPE_ARROW;
			lRet = Draw_ULineFinish((DRAW_HANDLE)m_ulDrawHandle, CurrentPoint, m_CurrentGraphic.ulGraphicGroup);
			m_bIsDrawing = false;
			m_ulGraphicState = GRAPHICS_STATE_EDITING;
            SetDeleteButtonState(m_CurrentGraphic.bAllowDelete);
			break;
		default:
			break;
		}
		if (IVS_SUCCEED != lRet)
		{
			(void)StopDrawGraphicWhileDraw(m_ulCurrentGraphicGroup);
			m_bIsDrawing = false;
			m_ulGraphicState = GRAPHICS_STATE_EDITING;
		}
	}
	else if (GRAPHICS_STATE_EDITING == m_ulGraphicState)// �༭״̬
	{
		if (!m_bIsDrawing)
		{
            (void)Draw_SelectCancel((DRAW_HANDLE)m_ulDrawHandle);
// 			BOOL bAllowDelete = FALSE;
//          (void)Draw_SelectGraphicByPoint((DRAW_HANDLE)m_ulDrawHandle, CurrentPoint, &m_ulCurrentGraphicGroup, &bAllowDelete);
// 			SetDeleteButtonState(bAllowDelete);
			lRet = (LONG)SelectGraphicByPoint(CurrentPoint);
            if (IVS_SUCCEED != lRet)
                return;

			lRet = Draw_StartModifyGraphics((DRAW_HANDLE)m_ulDrawHandle, CurrentPoint, RIGHT_BUTTON_DOWN, m_ulCurrentGraphicGroup);
			CHECK_DRAW_START_SUCCESS(lRet);
		}
	}
}

void CVideoPane::DrawRButtonUp(const CPoint &point)
{
	CHECK_POINTER_VOID(m_ulDrawHandle);
    CHECK_POINTER_VOID(m_pVideoPaneMgr);
    UINT scale = m_pVideoPaneMgr->GetDispalyScale();
	BOOL bScaleModeFull = scale == DISPLAY_SCALE_FULL ? TRUE : FALSE;
	(VOID)Draw_SetDisplayScaleMode((DRAW_HANDLE)m_ulDrawHandle, bScaleModeFull);

	POINT CurrentPoint;
	CurrentPoint.x = point.x;
	CurrentPoint.y = point.y;

    if (GRAPHICS_STATE_EDITING == m_ulGraphicState)// �༭״̬
    {
		(void)Draw_ModifyFinish((DRAW_HANDLE)m_ulDrawHandle, m_ulCurrentGraphicGroup, CurrentPoint);
		m_bIsDrawing = false;
    }
}

// �������ܷ����켣����
IVS_INT32 CVideoPane::EnableOverlay(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, ULONG ulRuleType)
{
	IVS_DEBUG_TRACE("SessionID: %d, PlayHandle: %lu, RuleType: %lu", iSessionID, ulPlayHandle, ulRuleType);
	HWND hMsgWnd = GetVideoWnd();
	CHECK_POINTER(hMsgWnd, IVS_FAIL);

	if (!m_ulOverlayHandle)
	{
		IATRACK_HANDLE  h = IATrack_GetHandle();
		if (NULL == h)
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Enable Overlay", "No Memory");
			return IVS_ALLOC_MEMORY_ERROR;
		}

		if (VIDEO_PLAY_STATE_NOT_PLAYING == GetPlayState())
		{
			BP_RUN_LOG_ERR(VIDEO_PLAY_STATE_NOT_PLAYING, "Enable Overlay", "No Playing");
			return VIDEO_PLAY_STATE_NOT_PLAYING;
		}

		LONG lRet = IATrack_Init(h, hMsgWnd, GetPlayState());
		if (IVS_SUCCEED != lRet)
		{
			return lRet;
		}
		m_ulOverlayHandle = (unsigned long)h ;
	}

	m_bOverlayEnabled = true;

	CGlobalIAMessageMgr &IAMessageMgr = CGlobalIAMessageMgr::Instance();
	m_ulRequestID = IAMessageMgr.StartOverlay(iSessionID, this, ulRuleType);
	if (IA_INVALID_REQUEST_ID == m_ulRequestID)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "Enable Overlay", "Start Overlay Failed");
		return IVS_FAIL;
	}

	return IVS_SDK_SetDrawCallBackEx(iSessionID, ulPlayHandle, hMsgWnd, OverlayProc, (void *)m_ulOverlayHandle);
}

// �ر����ܷ����켣����
IVS_INT32 CVideoPane::DisableOverlay(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle)
{
	IVS_DEBUG_TRACE("SessionID: %d, PlayHandle: %lu", iSessionID, ulPlayHandle);
	if (NULL == m_ulOverlayHandle)
	{
		return IVS_SUCCEED;
	}

	CGlobalIAMessageMgr &IAMessageMgr = CGlobalIAMessageMgr::Instance();
	IVS_INT32 iRet = IAMessageMgr.StopOverlay(iSessionID, this, m_ulRequestID);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Disable Overlay", "Stop Overlay Failed");
		return iRet;
	}

	// ɾ���ص�
	HWND hMsgWnd = GetVideoWnd();
	CHECK_POINTER(hMsgWnd, IVS_FAIL);
	(void)IVS_SDK_SetDrawCallBackEx(iSessionID, ulPlayHandle, hMsgWnd, NULL, NULL);
	IATRACK_HANDLE h = (IATRACK_HANDLE *)m_ulOverlayHandle;
	(void)IATrack_FreeHandle(h);

	m_ulOverlayHandle = NULL;
	m_bOverlayEnabled = false;
	return IVS_SUCCEED;
}

// ���ܷ����켣���Ӳ���켣��Ϣ
IVS_INT32 CVideoPane::InsertIALayoutMessage(const IA_LAYOUT_MESSAGE &LayoutMessage)
{
	IVS_DEBUG_TRACE("");
	CHECK_POINTER(m_ulOverlayHandle, IVS_FAIL);
	IATRACK_HANDLE h = (IATRACK_HANDLE *)m_ulOverlayHandle;
	return IATrack_InsertIALayoutMessage(h, &LayoutMessage);
}

// ��ȡ��ǰ�켣��Ϣ����
IVS_INT32 CVideoPane::GetCurrentElementNum()
{
	CHECK_POINTER(m_ulOverlayHandle, IVS_FAIL);
	IATRACK_HANDLE h = (IATRACK_HANDLE *)m_ulOverlayHandle;
	return IATrack_GetCurrentElementNum(h);
}

// ����ץ��
IVS_INT32 CVideoPane::LocalSnapshot(IVS_INT32 iSessionID, IVS_LOCAL_CAPTURE_PARAM &LocalCaptureParam)
{
	IVS_INT32 iRet = CheckLocalCaptureParam(LocalCaptureParam);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Local Snapshot", "Check Local Capture Param failed");
		return iRet;
	}

	IVS_ULONG ulPlayHandle = 0;
	if (BOTTOMBAR_MAGNIFY_OPEN == m_ulIsDstMagnify)
	{
		ulPlayHandle = GetMagnifyPlayHandle();
	} else {
		ulPlayHandle = GetPlayHandle();
	}

	// ����SDKץ��
	if (CAPTURE_MODE_SINGLE == LocalCaptureParam.uiSnapshotMode)
	{
		iRet = SingleCapture(iSessionID, ulPlayHandle, LocalCaptureParam);
	} else {
		iRet = StartSerialCapture(iSessionID, LocalCaptureParam);
	}

	return iRet;
}


//����ͼ��ץ�Ĳ����浽ָ��·��
IVS_INT32 CVideoPane::LocalSnapshotToDir(IVS_INT32 iSessionID, 
	IVS_UINT32 uiPictureFormat, 
	const IVS_CHAR *pFileName)
{
	CHECK_POINTER(pFileName, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("");

	IVS_ULONG ulPlayHandle = IVS_SUCCEED;
	if (BOTTOMBAR_MAGNIFY_OPEN == m_ulIsDstMagnify)
	{
		ulPlayHandle = GetMagnifyPlayHandle();
	}
	else
	{
		ulPlayHandle = GetPlayHandle();
	}

	return IVS_SDK_LocalSnapshot(iSessionID, ulPlayHandle, uiPictureFormat, pFileName);
}


IVS_INT32 CVideoPane::SetVideoEffect(IVS_INT32 iSessionID, IVS_LONG lBrightValue,
	IVS_LONG lContrastValue, IVS_LONG lSaturationValue,
	IVS_LONG lHueValue)
{
	IVS_DEBUG_TRACE("");

	IVS_ULONG ulPlayHandle = IVS_SUCCEED;
	if (BOTTOMBAR_MAGNIFY_OPEN == m_ulIsDstMagnify)
	{
		ulPlayHandle = GetMagnifyPlayHandle();
	} 
	else
	{
		ulPlayHandle = GetPlayHandle();
	}

	return IVS_SDK_SetVideoEffect(iSessionID, ulPlayHandle, lBrightValue, lContrastValue, lSaturationValue, lHueValue);
}


IVS_VOID CVideoPane::StopSerialSnapshot()
{

	m_bCaptureThreadRun = false;
	try
	{
		if (NULL != m_CaptureThreadHandle)
		{
			CloseHandle(m_CaptureThreadHandle);
			m_CaptureThreadHandle = NULL;
		}
	}
	catch(...)
	{//lint !e1775

	}
	
	memset(&m_LocalCaptureParam, 0, sizeof(m_LocalCaptureParam));
	
	CTranVideoRealtimeBar *pRealTimeToolBar = m_pRealTimeToolBar;
	CTranZoomBar		  *pZoomBar			= m_pZoomBar;

	if (NULL != pZoomBar)
	{
		pZoomBar->SetSnapShotEnableStatus(TRUE);
	} 
	else if (NULL != pRealTimeToolBar && VIDEO_PLAY_STATE_NOT_PLAYING != GetPlayState())
	{
		IVS_DEBUG_TRACE("Play State: %d", GetPlayState());
		pRealTimeToolBar->SetToolbarEnableStatus((ULONGLONG)0x01, (ULONGLONG)TRUE);
	}
}

IVS_INT32 CVideoPane::CheckLocalCaptureParam(const IVS_LOCAL_CAPTURE_PARAM &LocalCaptureParam)const
{
	if (('\0' == LocalCaptureParam.cCapturePath[0]) 
		|| (IVS_FILE_NAME_LEN <= strlen(LocalCaptureParam.cCapturePath)))
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "Check Local Capture Param", "Capture Path Err");
		return IVS_FAIL;
	}
	if (CAPTURE_MODE_OTHERS <= LocalCaptureParam.uiSnapshotMode)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "Check Local Capture Param", "Snapshot Mode Err");
		return IVS_FAIL;
	}
	if (0 == LocalCaptureParam.uiSnapshotCount || LocalCaptureParam.uiSnapshotCount > 50)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "Check Local Capture Param", "Snapshot Count Err");
		return IVS_FAIL;
	}
	if (0 == LocalCaptureParam.uiSnapshotInterval || LocalCaptureParam.uiSnapshotInterval > 10)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "Check Local Capture Param", "Snapshot Interval Err");
		return IVS_FAIL;
	}
	if (CAPTURE_FORMAT_NONE == LocalCaptureParam.uiSnapshotFormat 
		|| CAPTURE_FORMAT_OTHERS <= LocalCaptureParam.uiSnapshotFormat)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "Check Local Capture Param", "Snapshot Format Err");
		return IVS_FAIL;
	}
	if (0 == LocalCaptureParam.uiNameRule || CAPTURE_RULE_OTHERS <= LocalCaptureParam.uiNameRule)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "Check Local Capture Param", "Snapshot Name Rule Err");
		return IVS_FAIL;
	}

	return IVS_SUCCEED;
}

IVS_INT32 CVideoPane::GetTime(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_LOCAL_CAPTURE_PARAM &LocalCaptureParam)const
{
	// ��ȡ��ǰ����״̬��ʵ�� �� ¼��
	LocalCaptureParam.uiPlayState = (IVS_UINT32)GetPlayState();

	_tzset();
	struct tm tm;
	memset(&tm, 0, sizeof(tm));
	TM stTM; // ��ʱʱ���ַ�������
	memset(&stTM, 0, sizeof(TM));
	time_t t_Time = 0;

	TIME_ZONE_INFORMATION tzi = {0};
	GetTimeZoneInformation(&tzi);
	long tz = tzi.Bias / 60;     

	IVS_INT32 iResult = IVS_FAIL;
	switch (LocalCaptureParam.uiPlayState)
	{
	case VIDEO_PLAY_STATE_REALTIME:
		time_t tmBeg;
		memset(&tmBeg, 0, sizeof(tmBeg));
		time( &tmBeg );
		localtime_s(&tm, &tmBeg);  // ת��Ϊ����ʱ��ṹ��
// 		gmtime_s(&tm, &tmBeg);     // ת��Ϊ����ʱ��ṹ��
		break;
	case VIDEO_PLAY_STATE_VOD:
		// ��ȡ¼��ǰʱ��
		IVS_PLAYBACK_TIME stPlaybackTime;
		memset(&stPlaybackTime, 0, sizeof(IVS_PLAYBACK_TIME));
		iResult = IVS_SDK_GetPlayBackTime(iSessionID, ulPlayHandle, &stPlaybackTime);
		if (IVS_SUCCEED != iResult)
		{
			BP_RUN_LOG_ERR(iResult, "Get Current Time", "Get Play Back Time Failed");
			return iResult;
		}

		memcpy(stTM.Year, &stPlaybackTime.cPlayBackTime[0], 4);
		tm.tm_year = atoi(stTM.Year) - 1900;
		memcpy(stTM.Month, &stPlaybackTime.cPlayBackTime[4], 2);
		tm.tm_mon = atoi(stTM.Month) - 1;
		memcpy(stTM.Day, &stPlaybackTime.cPlayBackTime[6], 2);
		tm.tm_mday = atoi(stTM.Day);
		memcpy(stTM.Hour, &stPlaybackTime.cPlayBackTime[8], 2);
		tm.tm_hour = atoi(stTM.Hour) - tz;
		memcpy(stTM.Min, &stPlaybackTime.cPlayBackTime[10], 2);
		tm.tm_min = atoi(stTM.Min);
		memcpy(stTM.Sec, &stPlaybackTime.cPlayBackTime[12], 2);
		tm.tm_sec = atoi(stTM.Sec);

		t_Time = mktime(&tm);
		if ((time_t)-1 == t_Time)
		{
			BP_RUN_LOG_ERR(IVS_FAIL, "Get Current Time", "Make Time Failed");
			return IVS_FAIL;
		}
		localtime_s(&tm, &t_Time);
#if 0
		// �� YYYYMMDDhhmmss ת���� YYYY-MM-DD-hh-mm-ss
		(void)CToolsHelp::Memcpy(&LocalCaptureParam.cCurrentTime[0],  4, 
			&stPlaybackTime.cPlayBackTime[0],  4);
		LocalCaptureParam.cCurrentTime[4]  = '-';
		(void)CToolsHelp::Memcpy(&LocalCaptureParam.cCurrentTime[5],  2, 
			&stPlaybackTime.cPlayBackTime[4],  2);
		LocalCaptureParam.cCurrentTime[7]  = '-';
		(void)CToolsHelp::Memcpy(&LocalCaptureParam.cCurrentTime[8],  2, 
			&stPlaybackTime.cPlayBackTime[6],  2);
		LocalCaptureParam.cCurrentTime[10] = '-';
		(void)CToolsHelp::Memcpy(&LocalCaptureParam.cCurrentTime[11], 2, 
			&stPlaybackTime.cPlayBackTime[8],  2);
		LocalCaptureParam.cCurrentTime[13] = '-';
		(void)CToolsHelp::Memcpy(&LocalCaptureParam.cCurrentTime[14], 2, 
			&stPlaybackTime.cPlayBackTime[10], 2);
		LocalCaptureParam.cCurrentTime[16] = '-';
		(void)CToolsHelp::Memcpy(&LocalCaptureParam.cCurrentTime[17], 2, 
			&stPlaybackTime.cPlayBackTime[12], 2);
		LocalCaptureParam.cCurrentTime[19] = '\0';
#endif
		break;
	default:
		BP_RUN_LOG_ERR(IVS_FAIL, "Get Current Time", "Play State Invalid");
		return IVS_FAIL;
	}
	(void)_snprintf_s(LocalCaptureParam.cCurrentTime, 
		sizeof(LocalCaptureParam.cCurrentTime) - 1, 
		"%4d-%02d-%02d-%02d-%02d-%02d", 
		tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, 
		tm.tm_hour, tm.tm_min, tm.tm_sec);
	if (0 < tm.tm_isdst)
	{
		(void)_snprintf_s(LocalCaptureParam.cCurrentTime, 
			sizeof(LocalCaptureParam.cCurrentTime) - 1, 
			"%s-DST", LocalCaptureParam.cCurrentTime);
	}

	return IVS_SUCCEED;
}

IVS_INT32 CVideoPane::BuildFileName(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_LOCAL_CAPTURE_PARAM &LocalCaptureParam)
{
	IVS_INT32 iRet = IVS_FAIL;

	// ��ȡ���������
#if 0
	iRet = IVS_SDK_GetCameraName(iSessionID, ulPlayHandle, 
		IVS_CAMERA_NAME_LEN + 1, LocalCaptureParam.cCameraName);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Build File Name", "Get Camera Name Failed");
		return iRet;
	}
#else
	char cName[IVS_CAMERA_NAME_LEN + 1] = {0};
	std::string strDevName = "";
	if (BOTTOMBAR_MAGNIFY_OPEN != m_ulIsDstMagnify)
	{
		if (NULL != m_pRealTimeTitleBar)
		{
			strDevName = m_pRealTimeTitleBar->GetDevName();
		} else {
			strDevName = GetDevName();
		}
	} else {
		if (NULL != m_pZoomTitleBar)
		{
			if (NULL != GetMagnifySrcPane())
			{
				strDevName = GetMagnifySrcPane()->GetDevName() + m_pZoomTitleBar->GetZoomTitleName();//lint !e613
			}
		}
	}
	if (!CToolsHelp::Strncpy(cName, IVS_CAMERA_NAME_LEN, 
								strDevName.c_str(), strDevName.length()))
	{
		BP_RUN_LOG_ERR(iRet, "Build File Name", "Get Camera Name Failed");
		return iRet;
	}
	int iIndex = 0;
	for (int i = 0; i < IVS_CAMERA_NAME_LEN; i++)
	{
		if (':' == cName[i])
		{
           
            // modify by wanglei 00165153:2013.6.30 DTS2013063000144 
            // ����ǽ�������ԣ�������...�� ��ʼ������":",��Ҫ�ظ��ַ��Ժ��ȡ��������":"�������������һ���ַ�����󣨣�������Ļ����
            iIndex = i + 1;
			// iIndex = i + 2;
		}
		if ('>' == cName[i])
		{
			cName[i] = '-';
			break;
		}
	}
	if (!CToolsHelp::Memcpy(LocalCaptureParam.cCameraName, IVS_CAMERA_NAME_LEN, 
		&cName[iIndex], strlen(&cName[iIndex])))
	{
		BP_RUN_LOG_ERR(iRet, "Build File Name", "Get Camera Name Failed");
		return iRet;
	}

#endif


	// ��ȡ��ǰ����ʱ��
	iRet = GetTime(iSessionID, ulPlayHandle, LocalCaptureParam);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Build File Name", "Get Time Failed");
		return iRet;
	}
	const char* FILE_SUFFIX[] = {"jpg", "bmp"};
// 	if (BOTTOMBAR_MAGNIFY_OPEN == m_ulIsDstMagnify)	
// 	{
// 		m_strZoomName.c_str() = '\0';
// 	}
	// ������������ƴ�ļ���
	switch (LocalCaptureParam.uiNameRule)
	{
	case CAPTURE_RULE_NAME_NUM_TIME:
		(void)_snprintf_s(LocalCaptureParam.cFileName, IVS_FILE_NAME_LEN, "%s_%03u_%s.%s", 
			LocalCaptureParam.cCameraName, /*m_strZoomName.c_str(),*/ LocalCaptureParam.uiPictureNum, 
			LocalCaptureParam.cCurrentTime, FILE_SUFFIX[LocalCaptureParam.uiSnapshotFormat - 1]); 
		break;
	case CAPTURE_RULE_NAME_TIME_NUM:
		(void)_snprintf_s(LocalCaptureParam.cFileName, IVS_FILE_NAME_LEN, "%s_%s_%03u.%s", 
			LocalCaptureParam.cCameraName, /*m_strZoomName.c_str(),*/ LocalCaptureParam.cCurrentTime, 
			LocalCaptureParam.uiPictureNum, FILE_SUFFIX[LocalCaptureParam.uiSnapshotFormat - 1]); 
		break;
	case CAPTURE_RULE_TIME_NAME_NUM:
		(void)_snprintf_s(LocalCaptureParam.cFileName, IVS_FILE_NAME_LEN, "%s_%s_%03u.%s", 
			LocalCaptureParam.cCurrentTime, LocalCaptureParam.cCameraName, /*m_strZoomName.c_str(),*/ 
			LocalCaptureParam.uiPictureNum, FILE_SUFFIX[LocalCaptureParam.uiSnapshotFormat - 1]); 
		break;
	default:
		BP_RUN_LOG_ERR(IVS_FAIL, "Build File Name", "Name Rule Invalid");
		return IVS_FAIL;
	}
	return IVS_SUCCEED;
}

IVS_INT32 CVideoPane::BuildFullPath(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_LOCAL_CAPTURE_PARAM &LocalCaptureParam) 
{
	IVS_INT32 iRet = BuildFileName(iSessionID, ulPlayHandle, LocalCaptureParam);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Build Full Path", "Build File Name failed");
		return iRet;
	}

	unsigned int iPathLenth = strlen(LocalCaptureParam.cCapturePath);
	if (IVS_FILE_NAME_LEN <= iPathLenth)
	{
		BP_RUN_LOG_ERR(IVS_NAME_INVALID, "Build Full Path", "Path Lenth TOO LONG");
		return IVS_NAME_INVALID;
	}
	if ('\\' != LocalCaptureParam.cCapturePath[iPathLenth - 1])
	{
		LocalCaptureParam.cCapturePath[iPathLenth]     = '\\';
		LocalCaptureParam.cCapturePath[iPathLenth + 1] = '\0';
	}
	if (IVS_FILE_NAME_LEN < strlen(LocalCaptureParam.cCapturePath) + strlen(LocalCaptureParam.cFileName))
	{		
		BP_RUN_LOG_ERR(IVS_NAME_INVALID, "Build Full Path", "File Name TOO LONG");
		return IVS_NAME_INVALID;
	}
	(void)_snprintf_s(LocalCaptureParam.cFullPath, IVS_FILE_NAME_LEN, "%s%s", 
		LocalCaptureParam.cCapturePath, LocalCaptureParam.cFileName); 

	return IVS_SUCCEED;
}

IVS_INT32 CVideoPane::SingleCapture(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_LOCAL_CAPTURE_PARAM &LocalCaptureParam)
{
	// ƴװ�ļ�·�����ļ���
	LocalCaptureParam.uiPictureNum = 1;
	IVS_INT32 iRet = BuildFullPath(iSessionID, ulPlayHandle, LocalCaptureParam);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Single Capture", "Build Full Path failed");
		return CaptureRetToCU(iRet, LocalCaptureParam);
	}
	if (BOTTOMBAR_MAGNIFY_OPEN != m_ulIsDstMagnify)
	{
		iRet = IVS_SDK_LocalSnapshot(iSessionID, ulPlayHandle, 
			LocalCaptureParam.uiSnapshotFormat, LocalCaptureParam.cFullPath);
	} else {
		iRet = IVS_SDK_LocalSnapshotRect(iSessionID, ulPlayHandle, 
			LocalCaptureParam.uiSnapshotFormat, LocalCaptureParam.cFullPath, &m_IArectPercent);
	}

	return CaptureRetToCU(iRet, LocalCaptureParam);
}

IVS_INT32 CVideoPane::StartSerialCapture(IVS_INT32 iSessionID, const IVS_LOCAL_CAPTURE_PARAM &LocalCaptureParam)
{
	if (NULL != m_CaptureThreadHandle)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "Start Serial Capture", "Serial Capture Exist");
		return IVS_OCX_RET_CAPTURE_CREATE_THREAD_ERROR;
	}
	
	(void)CToolsHelp::Memcpy(&m_LocalCaptureParam, sizeof(IVS_LOCAL_CAPTURE_PARAM), 
		&LocalCaptureParam, sizeof(IVS_LOCAL_CAPTURE_PARAM));
	
	m_bCaptureThreadRun = true;
	m_CaptureThreadHandle = (HANDLE)_beginthreadex(NULL, 0, CaptureThreadProc, this, 0, NULL);
	ASSERT(m_CaptureThreadHandle);
	if (NULL == m_CaptureThreadHandle)//lint !e774
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "Start Serial Capture", "Begin Thread Failed");
		m_bCaptureThreadRun = false;
		return IVS_FAIL;
	}
	return IVS_SUCCEED;
}//lint !e715

IVS_INT32 CVideoPane::CaptureRetToCU(IVS_INT32 iResult, IVS_LOCAL_CAPTURE_PARAM &LocalCaptureParam) 
{
	// ƴװ����CU��Ϣ��XML
	CIVS_OCXPlayerCtrl *pPlayerCtrl = GetOCXPlayerCtrl();
	CHECK_POINTER(pPlayerCtrl, IVS_OPERATE_MEMORY_ERROR);

	CXml xml;
	LONG lRetCode = EVENT_LOCAL_CAPTURE_FAILED;
	IVS_CHAR *pFullPath = NULL;
	if (IVS_SUCCEED == iResult)
	{
		lRetCode = EVENT_LOCAL_CAPTURE_SUCCEED;
		pFullPath = LocalCaptureParam.cFullPath;
	}
	if (IVS_PLAYER_RET_DISK_SPACE_ERROR == iResult)
	{
		lRetCode = EVENT_LOCAL_CAPTURE_NO_SPACE;
	}
	IVS_INT32 iRet = OCXCapturePictureXMLProcess::LocalCaptureBuildXMLGeneric(lRetCode, m_ulPaneID, pFullPath, xml);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Capture Return To CU", "Build Generic XML Failed");
		return iRet;
	}
	unsigned int uiLength = 0;
	const TCHAR* pszXML = xml.GetXMLStream(uiLength);
	pPlayerCtrl->IVS_OCX_Windows_Event(lRetCode, (LPCTSTR)pszXML);
	return iRet;
}

IVS_INT32 CVideoPane::ReachTimeInterval(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, const IVS_LOCAL_CAPTURE_PARAM &LocalCaptureParam, BOOL &bReachTimeInterval)
{
	bReachTimeInterval = FALSE;
	
	time_t tmCurrent; // ��ǰʱ��
	memset(&tmCurrent, 0, sizeof(tmCurrent));
	time_t tmLast;    // �ϴ�ץ��ʱ��
	memset(&tmLast, 0, sizeof(tmLast));

	tm stCurrentTime; // ��ǰʱ��int�ṹ��
	memset(&stCurrentTime, 0, sizeof(tm));
	tm stLastTime;    // �ϴ�ץ��ʱ��int�ṹ��
	memset(&stLastTime, 0, sizeof(tm));

	TM stTempTimeArr; // ��ʱʱ���ַ�������
	memset(&stTempTimeArr, 0, sizeof(TM));

	TIME_ZONE_INFORMATION tzi = {0};
	GetTimeZoneInformation(&tzi);
	long tz = tzi.Bias / 60;     

	IVS_INT32 iResult = IVS_FAIL;
	// ��ȡ��ǰʱ��time_t
	switch (LocalCaptureParam.uiPlayState)
	{
	case VIDEO_PLAY_STATE_REALTIME:
		time( &tmCurrent );
		localtime_s(&stCurrentTime, &tmCurrent);  // ת��Ϊ����ʱ��ṹ��
		break;
	case VIDEO_PLAY_STATE_VOD:
		// ��ȡ¼��ǰʱ��
		IVS_PLAYBACK_TIME stPlaybackTime;
		memset(&stPlaybackTime, 0, sizeof(IVS_PLAYBACK_TIME));
		iResult = IVS_SDK_GetPlayBackTime(iSessionID, ulPlayHandle, &stPlaybackTime);
		if (IVS_SUCCEED != iResult)
		{
			BP_RUN_LOG_ERR(iResult, "Reach Time Interval", "Get Play Back Time Failed");
			return iResult;
		}
		memcpy(stTempTimeArr.Year, &stPlaybackTime.cPlayBackTime[0], 4);
		stCurrentTime.tm_year = atoi(stTempTimeArr.Year) - 1900;
		memcpy(stTempTimeArr.Month, &stPlaybackTime.cPlayBackTime[4], 2);
		stCurrentTime.tm_mon = atoi(stTempTimeArr.Month) - 1;
		memcpy(stTempTimeArr.Day, &stPlaybackTime.cPlayBackTime[6], 2);
		stCurrentTime.tm_mday = atoi(stTempTimeArr.Day);
		memcpy(stTempTimeArr.Hour, &stPlaybackTime.cPlayBackTime[8], 2);
		stCurrentTime.tm_hour = atoi(stTempTimeArr.Hour) - tz;
		memcpy(stTempTimeArr.Min, &stPlaybackTime.cPlayBackTime[10], 2);
		stCurrentTime.tm_min = atoi(stTempTimeArr.Min);
		memcpy(stTempTimeArr.Sec, &stPlaybackTime.cPlayBackTime[12], 2);
		stCurrentTime.tm_sec = atoi(stTempTimeArr.Sec);
		tmCurrent = mktime(&stCurrentTime);
		if ((time_t)-1 == tmCurrent)
		{
			BP_RUN_LOG_ERR(IVS_FAIL, "Reach Time Interval", "Make VOD Time Failed");
			return IVS_FAIL;
		}

		break;
	default:
		BP_RUN_LOG_ERR(IVS_FAIL, "Reach Time Interval", "Play State Invalid");
		return IVS_FAIL;
	}

	// ��ȡ�ϴ�ץ��ʱ��time_t
	memset(&stTempTimeArr, 0, sizeof(TM));
	memcpy(stTempTimeArr.Year, &LocalCaptureParam.cCurrentTime[0], 4);
	stLastTime.tm_year = atoi(stTempTimeArr.Year) - 1900;
	memcpy(stTempTimeArr.Month, &LocalCaptureParam.cCurrentTime[5], 2);
	stLastTime.tm_mon = atoi(stTempTimeArr.Month) - 1;
	memcpy(stTempTimeArr.Day, &LocalCaptureParam.cCurrentTime[8], 2);
	stLastTime.tm_mday = atoi(stTempTimeArr.Day);
	memcpy(stTempTimeArr.Hour, &LocalCaptureParam.cCurrentTime[11], 2);
	stLastTime.tm_hour = atoi(stTempTimeArr.Hour);
	memcpy(stTempTimeArr.Min, &LocalCaptureParam.cCurrentTime[14], 2);
	stLastTime.tm_min = atoi(stTempTimeArr.Min);
	memcpy(stTempTimeArr.Sec, &LocalCaptureParam.cCurrentTime[17], 2);
	stLastTime.tm_sec = atoi(stTempTimeArr.Sec);
	char cDST[4] = {0};
	memcpy(cDST, &LocalCaptureParam.cCurrentTime[20], 3);
	if (!strcmp("DST\0", cDST))// DSTʱ����Ҫ��ȥһ��Сʱ
	{
		stLastTime.tm_hour--;
	}
	tmLast = mktime(&stLastTime);
	if ((time_t)-1 == tmLast)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "Reach Time Interval", "Make LAST Time Failed");
		return IVS_FAIL;
	}

	double iElapsedTime = difftime(tmCurrent, tmLast);
	if (iElapsedTime >= (double)LocalCaptureParam.uiSnapshotInterval)
	{
		bReachTimeInterval = TRUE;
	}

	return IVS_SUCCEED;

}

IVS_UINT32 __stdcall CVideoPane::CaptureThreadProc(void *pParameter)
{
	ASSERT(pParameter);
	if (NULL == pParameter)//lint !e774
	{
		_endthreadex(0);
		BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "Serial Capture Thread Process", 
			"Parameter Point NULL");
		return IVS_OPERATE_MEMORY_ERROR;
	}
	IVS_DEBUG_TRACE("");

	unsigned int uiCount = 0;
	CVideoPane *pVideoPanel = (CVideoPane *)pParameter;

	// ������ץ�İ�ť�û�
	CTranVideoRealtimeBar *pRealTimeToolBar = pVideoPanel->m_pRealTimeToolBar;
	CTranZoomBar		  *pZoomBar			= pVideoPanel->m_pZoomBar;

	if (NULL == pRealTimeToolBar && NULL == pZoomBar)
	{
		BP_DBG_LOG("Serial Capture Thread Process: ToolBar is NULL");
		(void)pVideoPanel->SingleCapture(pVideoPanel->m_iSessionID, 
									pVideoPanel->GetPlayHandle(), 
									pVideoPanel->m_LocalCaptureParam);
		pVideoPanel->StopSerialSnapshot();
		_endthreadex(0);
		return 0;
	}
	if (NULL != pZoomBar)
	{
		pZoomBar->SetSnapShotEnableStatus(FALSE);
	} else {
		pRealTimeToolBar->SetToolbarEnableStatus((ULONGLONG)0x01, (ULONGLONG)FALSE);//lint !e613
	}

	IVS_ULONG ulPlayHandle = 0;
	if (BOTTOMBAR_MAGNIFY_OPEN == pVideoPanel->m_ulIsDstMagnify)
	{
		ulPlayHandle = pVideoPanel->GetMagnifyPlayHandle();
	} else {
		ulPlayHandle = pVideoPanel->GetPlayHandle();
	}


	while (pVideoPanel->m_bCaptureThreadRun)
	{
		// Check����״̬

		pVideoPanel->m_LocalCaptureParam.uiPictureNum = ++uiCount;
		IVS_INT32 iRet = pVideoPanel->BuildFullPath(pVideoPanel->m_iSessionID, 
			ulPlayHandle, pVideoPanel->m_LocalCaptureParam);
		if (IVS_SUCCEED != iRet)
		{
			BP_RUN_LOG_ERR(iRet, "Serial Capture", "Build Full Path failed");
		}
		BP_RUN_LOG_INF("Serial Capture", "Picture Name is %s", 
			pVideoPanel->m_LocalCaptureParam.cFullPath);

		if (BOTTOMBAR_MAGNIFY_OPEN != pVideoPanel->m_ulIsDstMagnify)
		{
			iRet = IVS_SDK_LocalSnapshot(pVideoPanel->m_iSessionID, 
					ulPlayHandle, 
					pVideoPanel->m_LocalCaptureParam.uiSnapshotFormat, 
					pVideoPanel->m_LocalCaptureParam.cFullPath);
		} else {
			iRet = IVS_SDK_LocalSnapshotRect(pVideoPanel->m_iSessionID, 
					ulPlayHandle, 
					pVideoPanel->m_LocalCaptureParam.uiSnapshotFormat, 
					pVideoPanel->m_LocalCaptureParam.cFullPath, 
					&pVideoPanel->m_IArectPercent);
		}

		(void)pVideoPanel->CaptureRetToCU(iRet, pVideoPanel->m_LocalCaptureParam);

		if (IVS_PLAYER_RET_DISK_SPACE_ERROR == iRet)
		{
			BP_RUN_LOG_ERR(iRet, "Serial Capture", "No Space");
			break;
		}
		if (pVideoPanel->m_LocalCaptureParam.uiSnapshotCount 
			== pVideoPanel->m_LocalCaptureParam.uiPictureNum)
		{
			BP_RUN_LOG_INF("Serial Capture", "Serial Capture Finished, Picture Num: %u", 
				pVideoPanel->m_LocalCaptureParam.uiPictureNum);
			break;
		}

        // zhangshufan �ſ� 2013.4.27
        // modify by wanglei 00165153:2013.5.3 DTS2013042702886 ��Ϊʱ����ץ�ģ�0,1������zhangshufan��λ��
#if 1 // ץ�����ֹͣ�������
		for (IVS_UINT32 i = 0; i < pVideoPanel->m_LocalCaptureParam.uiSnapshotInterval; i++)
		{
			if (!pVideoPanel->m_bCaptureThreadRun)
			{
				BP_RUN_LOG_INF("Serial Capture", "External Interrupt");
				if (NULL != pRealTimeToolBar)
				{
					pRealTimeToolBar->SetToolbarEnableStatus((ULONGLONG)0x01, (ULONGLONG)TRUE);
				}
				pVideoPanel->StopSerialSnapshot();
				_endthreadex(0);
				return 0;
			}
			Sleep(1000);// ֹͣ1s
		}
#else
//#endif
		// ѭ���жϾ����ϴ�ץ��ʱ���Ƿ񵽴���ʱ��
		BOOL bReachTimeInterval = FALSE;
		while (!bReachTimeInterval)
		{
			(void)ReachTimeInterval(pVideoPanel->m_iSessionID, ulPlayHandle, 
				pVideoPanel->m_LocalCaptureParam, bReachTimeInterval);
			if (!pVideoPanel->m_bCaptureThreadRun)
			{
				BP_RUN_LOG_INF("Serial Capture", "External Interrupt");
// 				pRealTimeToolBar->SetToolbarEnableStatus((ULONGLONG)0x01, (ULONGLONG)TRUE);
// 				if (NULL != pZoomBar)
// 				{
// 					pZoomBar->SetSnapShotEnableStatus(TRUE);
// 				} else {
// 					pRealTimeToolBar->SetToolbarEnableStatus((ULONGLONG)0x01, (ULONGLONG)TRUE);
// 				}
				pVideoPanel->StopSerialSnapshot();
				_endthreadex(0);
				return 0;
			}
			Sleep(100);// ֹͣ100ms
		}
#endif
	}
// 	pRealTimeToolBar->SetToolbarEnableStatus((ULONGLONG)0x01, (ULONGLONG)TRUE);
// 	if (NULL != pZoomBar)
// 	{
// 		pZoomBar->SetSnapShotEnableStatus(TRUE);
// 	} else {
// 		pRealTimeToolBar->SetToolbarEnableStatus((ULONGLONG)0x01, (ULONGLONG)TRUE);
// 	}
	pVideoPanel->StopSerialSnapshot();
	_endthreadex(0);
	return 0;
}

void CVideoPane::EnableALLButton( BOOL isEnable )
{
	SetAllBarStatus(1);
	if (m_pRealTimeTitleBar != NULL)
	{
		m_pRealTimeTitleBar->EnableALLButton(isEnable);
	}

	if (m_pRealTimeToolBar != NULL)
	{
		m_pRealTimeToolBar->EnableALLButton(isEnable);
	}

	if (m_pAnalysisToolBar != NULL)
	{
		m_pAnalysisToolBar->EnableALLButton(isEnable);
	}

	if (NULL == m_pAnalysisToolBar)
	{
		//�澯��ť���û�
		SetToolbarEnableStatus(IVS_TOOLBAR_ALARM_WIN,m_ullWinAlarmFlag);//lint !e747
		//���ӵ�ͼ��ť���û�
		SetToolbarEnableStatus(IVS_TOOLBAR_OPEN_MAP,1);//lint !e747
		//3D��ť���û�
		SetToolbarEnableStatus(IVS_TOOLBAR_PTZ3D,1);//lint !e747
		// 1���񲼾֣��ֲ��Ŵ�ťֽ��
		CIVS_OCXPlayerCtrl *pPlayerCtrl = GetOCXPlayerCtrl();
		CHECK_POINTER_VOID(pPlayerCtrl);
		CVideoPaneMgr* pVideoPaneMgr =  pPlayerCtrl->GetVideoPaneMgr();
		CHECK_POINTER_VOID(pVideoPaneMgr);
		if (WIND_LAYOUT_1_1 == pVideoPaneMgr->GetWindLayOut())
		{
			SetToolbarEnableStatus(IVS_TOOLBAR_ZOOM,0);//lint !e747
		}
	}
	//������ť��ˢ��
	ResetBarPosition();
}

void CVideoPane::SetAnalysisBarIsStartPlay( BOOL IsStartPlay )
{
    if (m_pAnalysisToolBar != NULL)
    {
        m_pAnalysisToolBar->SetIsStartPay(IsStartPlay);
    }
}

void CVideoPane::DeleteTurnCruisePane()
{
	if(nullptr != m_pTurnCruisePane)
	{
		//IVS_DELETE(m_pTurnCruisePane);
		//m_pTurnCruisePane = nullptr;
		SetTurnCruiseStyle(0);
		m_pTurnCruisePane->ShowWindow(SW_HIDE);
	}
}

afx_msg LRESULT CVideoPane::OnDeleteTurnCruise(WPARAM wParam,LPARAM pParam)
{
	(void)DeleteTurnCruiseStatus();
	return 1;
}//lint !e715

void CVideoPane::DeleteTimerBar()
{
	if (nullptr != m_pPlayTimerBar)
	{
		IVS_DELETE(m_pPlayTimerBar);
		m_pPlayTimerBar = nullptr;
	}
}

/*
 * VerifySameStream
 * 
 */
bool CVideoPane::VerifySameStream(const char *szCamID, unsigned int type)const
{
    ASSERT(NULL != szCamID);
	if (NULL == szCamID)//lint !e774
	{
		return false;
	}
	return (!strcmp(szCamID, m_CamReal) && (type == STREAM_TYPE_UNSPECIFIED || type == m_uiStreamType));
}

void CVideoPane::SetRealStreamInfo(const char *szCamID, unsigned int type)
{
	(void)CToolsHelp::Strncpy(m_CamReal, sizeof(m_CamReal), szCamID, IVS_DEV_CODE_LEN);
	m_uiStreamType = type;
}

LONG CVideoPane::DeleteTurnCruiseStatus()
{
	SetTurnCruiseStatus(false);
	if(m_pRealTimeTitleBar!=nullptr)
	{
		m_pRealTimeTitleBar->SetTurnCruiseStatus(0);
		//m_pRealTimeTitleBar->Invalidate(TRUE);
	}
	CTurnCruisePane * pTurnCruisePane = m_pTurnCruisePane;
	CHECK_POINTER(pTurnCruisePane,IVS_PARA_INVALID);
	DeleteTurnCruisePane();
	ResetBarPosition();
	return IVS_SUCCEED;
}

void CVideoPane::InvalidateCruiseBar()
{
	if(NULL != m_pRealTimeTitleBar)
	{
		m_pRealTimeTitleBar->SetTurnCruiseStatus(1);
		m_pRealTimeTitleBar->Invalidate(TRUE);
	}
}

afx_msg LRESULT CVideoPane::OnSyncPlayBackMsg(WPARAM wParam,LPARAM lParam)
{
	BOOL bStarted = (BOOL)wParam;
	if (bStarted)
	{
		SetTitleButtonPic(true);
		SetRealtimePlayPic(BUTTON_CLICKED);
		SetToolBarBtnState(TRUE);
		EnableALLButton(TRUE);

		// �ж�֮ǰ�ǲ��ǿ������Ŵ�״̬������ǷŴ�״̬ �ָ�֮ǰ��
		GetOCXPlayerCtrl()->ResumeMagnifyStatus(this);

		/*
		 *  DTS2013081907805
		 *  �ô���ͬ���ط�����������Ϊ�Ŵ󴰸�ɾ��֮
		 */
		if (BOTTOMBAR_MAGNIFY_OPEN == GetDstMagnifyState())
		{
			CVideoPane* pSrcVideoPane = GetMagnifySrcPane();
			if (NULL != pSrcVideoPane)
			{
				::SendMessage(pSrcVideoPane->GetSafeHwnd(), WM_MAGNIFY_CLOSE, 0, (LONG)this);
			}
			/*
			 * WM_MAGNIFY_CLOSE�����л�ָ���ǰ����״̬Ϊ�Ŵ�ʼʱ��״̬
			 * ��������Ϊ�ط�״̬��������Ƶ������ʾ
			 */
			SetPlayState(VIDEO_PLAY_STATE_VOD);
		}
	}
	else
	{
		// �ر�������Դ
		GetOCXPlayerCtrl()->StopPlayResource(this);

		std::string title;
		std::string devName;
		if (NULL != m_pRealTimeTitleBar)
		{
			title = m_pRealTimeTitleBar->GetDevName();
			devName = GetDevName();
		}
		//TODO
		ResetPane();

		if (NULL != m_pRealTimeTitleBar)
		{
			m_pRealTimeTitleBar->SetDevName(title);
			SetDevName(devName);
		}
	}
	Invalidate(TRUE);
	return 0;
}//lint !e715

afx_msg LRESULT CVideoPane::OnResumeRealPlayOKMsg(WPARAM wParam,LPARAM lParam)
{
	if (VIDEO_PLAY_STATE_RESTARTING == GetPlayState())
	{
		SetResumeState(false);
		SetPlayState(VIDEO_PLAY_STATE_REALTIME);
		ResetBarPosition();
		Invalidate(TRUE);
	}
	return 0;
}//lint !e715
void CVideoPane::HideTranslucenWnd(void)
{
	if (m_pRealTimeTitleBar != NULL)
	{
		m_pRealTimeTitleBar->ShowWindow(SW_HIDE);
		//m_pRealTimeTitleBar->MoveWindow(0,0,0,0);
	}
	if(m_pRealTimeToolBar!= NULL)
	{
		m_pRealTimeToolBar->ShowWindow(SW_HIDE);
		//m_pRealTimeToolBar->MoveWindow(0,0,0,0);
	}
	//���ܷ������Ų���Ҫ���ع�����
	//if (m_pAnalysisToolBar!= NULL)
	//{
	//	m_pAnalysisToolBar->ShowWindow(SW_HIDE);
	//}
	//if (m_pTVWallStatusBar != NULL)
	//{
	//	m_pTVWallStatusBar->ShowWindow(SW_HIDE);
	//	//m_pTVWallStatusBar->MoveWindow(0,0,0,0);
	//}
	//if (m_pTurnCruisePane!= NULL)
	//{
	//	m_pTurnCruisePane->ShowWindow(SW_HIDE);
	//	//m_pTurnCruisePane->MoveWindow(0,0,0,0);
	//}
	if (m_pDigtalDlg != NULL)
	{
		m_pDigtalDlg->ShowWindow(SW_HIDE);
		//m_pDigtalDlg->MoveWindow(0,0,0,0);
	}
	if (m_pZoomDirctDlg != NULL)
	{
		m_pZoomDirctDlg->ShowWindow(SW_HIDE);
		//m_pZoomDirctDlg->MoveWindow(0,0,0,0);
	}
	if (m_pZoomTitleBar != NULL)
	{
		m_pZoomTitleBar->ShowWindow(SW_HIDE);
		//m_pZoomTitleBar->MoveWindow(0,0,0,0);
	}
	if (m_pZoomBar != NULL)
	{
		m_pZoomBar->ShowWindow(SW_HIDE);
		//m_pZoomBar->MoveWindow(0,0,0,0);
	}
}

// ��Ӧ�Ŵ󴰿ڵ��ķ����ƾ��¼�,TODO �����ƶ�����������
afx_msg LRESULT CVideoPane::OnMagnifyRectMove(WPARAM wParam,LPARAM lParam)
{
	int iDirect = (int)wParam;
	ULONG ulGraphicGroup = (ULONG)lParam;
	// �ƶ�Դ���ڵľ���
	if (IVS_SUCCEED != MoveSrcGraphicRect(iDirect, ulGraphicGroup))
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "move src rect failed", "NA");
		return IVS_FAIL;
	}
	// �޸ķŴ󴰿�,���˳���ܵߵ�
	UpdataMagnifyWindow(ulGraphicGroup);
	return 0;
}

// �����ϱ߽�ֵ
#define FIX_MORE_BOUND( val, bound, step)   \
	do{                                               \
	if ((val + step) > bound) { val = bound; }         \
	else { val += step; }                 \
	}while(0)

// �����±߽�ֵ
#define FIX_LESS_BOUND( val, bound, step)   \
	do{                                               \
	if ((val - step) < bound) { val = bound; }         \
	else { val -= step; }                 \
	}while(0)

// �ƶ�Դ���ڵľ��Σ�Ĭ��1%
IVS_INT32 CVideoPane::MoveSrcGraphicRect(int iDirect, ULONG ulGraphicGroup)
{
	// ��ȡԭ����ͼ��
	IA_GRAPHICS graphicsTemp;
	memset(&graphicsTemp, 0, sizeof(IA_GRAPHICS));
	ULONG ulNum = 1;// ��ȡһ��ͼ��
	Draw_GetGraphicsByGroup((DRAW_HANDLE)m_ulDrawHandle, ulGraphicGroup, &graphicsTemp, &ulNum);
	SIZE sz = {0};
	(void)GetMediaSize(sz);
	int invalx = sz.cx/100;
	
	switch (iDirect)
	{
	case DIRCT_BTN_LEFT:
		// �ж��Ƿ�Խ�����
		CHECK_SUCCESS_RET(!(graphicsTemp.IAPoints[0].x <= 0), IVS_FAIL);
		CHECK_SUCCESS_RET(!(graphicsTemp.IAPoints[1].x <= 0), IVS_FAIL);
		CHECK_SUCCESS_RET(!(graphicsTemp.IAPoints[2].x <= 0), IVS_FAIL);
		CHECK_SUCCESS_RET(!(graphicsTemp.IAPoints[3].x <= 0), IVS_FAIL);
		CHECK_SUCCESS_RET(!(graphicsTemp.IAPoints[4].x <= 0), IVS_FAIL);
		// ��ȡƫ��
		for (int i=0;i<5;i++)
		{
			if ((graphicsTemp.IAPoints[i].x - invalx) < 0) 
			{ 
				invalx = graphicsTemp.IAPoints[i].x;
				break;
			}  
		}
		FIX_LESS_BOUND(graphicsTemp.IAPoints[0].x, 0, invalx);
		FIX_LESS_BOUND(graphicsTemp.IAPoints[1].x, 0, invalx);
		FIX_LESS_BOUND(graphicsTemp.IAPoints[2].x, 0, invalx);
		FIX_LESS_BOUND(graphicsTemp.IAPoints[3].x, 0, invalx);
		FIX_LESS_BOUND(graphicsTemp.IAPoints[4].x, 0, invalx);
		break;
	case DIRCT_BTN_TOP:
		// �ж��Ƿ�Խ�Ͻ���
		CHECK_SUCCESS_RET(!(graphicsTemp.IAPoints[0].y <= 0), IVS_FAIL);
		CHECK_SUCCESS_RET(!(graphicsTemp.IAPoints[1].y <= 0), IVS_FAIL);
		CHECK_SUCCESS_RET(!(graphicsTemp.IAPoints[2].y <= 0), IVS_FAIL);
		CHECK_SUCCESS_RET(!(graphicsTemp.IAPoints[3].y <= 0), IVS_FAIL);
		CHECK_SUCCESS_RET(!(graphicsTemp.IAPoints[4].y <= 0), IVS_FAIL);
		// ��ȡƫ��
		for (int i=0;i<5;i++)
		{
			if ((graphicsTemp.IAPoints[i].y - invalx) < 0) 
			{ 
				invalx = graphicsTemp.IAPoints[i].y;
				break;
			}  
		}
		FIX_LESS_BOUND(graphicsTemp.IAPoints[0].y, 0, invalx);
		FIX_LESS_BOUND(graphicsTemp.IAPoints[1].y, 0, invalx);
		FIX_LESS_BOUND(graphicsTemp.IAPoints[2].y, 0, invalx);
		FIX_LESS_BOUND(graphicsTemp.IAPoints[3].y, 0, invalx);
		FIX_LESS_BOUND(graphicsTemp.IAPoints[4].y, 0, invalx);
		break;
	case DIRCT_BTN_RIGHT:
		// �ж��Ƿ�Խ�ҽ���
		CHECK_SUCCESS_RET(!(graphicsTemp.IAPoints[0].x >= sz.cx), IVS_FAIL);
		CHECK_SUCCESS_RET(!(graphicsTemp.IAPoints[1].x >= sz.cx), IVS_FAIL);
		CHECK_SUCCESS_RET(!(graphicsTemp.IAPoints[2].x >= sz.cx), IVS_FAIL);
		CHECK_SUCCESS_RET(!(graphicsTemp.IAPoints[3].x >= sz.cx), IVS_FAIL);
		CHECK_SUCCESS_RET(!(graphicsTemp.IAPoints[4].x >= sz.cx), IVS_FAIL);
		// ��ȡƫ��
		for (int i=0;i<5;i++)
		{
			if ((graphicsTemp.IAPoints[i].x + invalx) > sz.cx) 
			{ 
				invalx = sz.cx - graphicsTemp.IAPoints[i].x;
				break;
			}  
		}
		FIX_MORE_BOUND(graphicsTemp.IAPoints[0].x, sz.cx, invalx);
		FIX_MORE_BOUND(graphicsTemp.IAPoints[1].x, sz.cx, invalx);
		FIX_MORE_BOUND(graphicsTemp.IAPoints[2].x, sz.cx, invalx);
		FIX_MORE_BOUND(graphicsTemp.IAPoints[3].x, sz.cx, invalx);
		FIX_MORE_BOUND(graphicsTemp.IAPoints[4].x, sz.cx, invalx);
		break;
	case DIRCT_BTN_BOTTOM:
		// �ж��Ƿ�Խ�½���
		CHECK_SUCCESS_RET(!(graphicsTemp.IAPoints[0].y >= sz.cy), IVS_FAIL);
		CHECK_SUCCESS_RET(!(graphicsTemp.IAPoints[1].y >= sz.cy), IVS_FAIL);
		CHECK_SUCCESS_RET(!(graphicsTemp.IAPoints[2].y >= sz.cy), IVS_FAIL);
		CHECK_SUCCESS_RET(!(graphicsTemp.IAPoints[3].y >= sz.cy), IVS_FAIL);
		CHECK_SUCCESS_RET(!(graphicsTemp.IAPoints[4].y >= sz.cy), IVS_FAIL);
		// ��ȡƫ��
		for (int i=0;i<5;i++)
		{
			if ((graphicsTemp.IAPoints[i].y + invalx) > sz.cy) 
			{ 
				invalx = sz.cy - graphicsTemp.IAPoints[i].y;
				break;
			}  
		}
		FIX_MORE_BOUND(graphicsTemp.IAPoints[0].y, sz.cy, invalx);
		FIX_MORE_BOUND(graphicsTemp.IAPoints[1].y, sz.cy, invalx);
		FIX_MORE_BOUND(graphicsTemp.IAPoints[2].y, sz.cy, invalx);
		FIX_MORE_BOUND(graphicsTemp.IAPoints[3].y, sz.cy, invalx);
		FIX_MORE_BOUND(graphicsTemp.IAPoints[4].y, sz.cy, invalx);
		break;
	default:
		return IVS_FAIL;
	}
	// ɾ��ԭ���ľ��ο�
	Draw_DeleteGraphicsByGroup((DRAW_HANDLE *)m_ulDrawHandle, ulGraphicGroup);
	Draw_SetGraphics((DRAW_HANDLE *)m_ulDrawHandle, &graphicsTemp, 1, 0);
	return IVS_SUCCEED;
}


// ���·Ŵ󴰿�
void CVideoPane::UpdataMagnifyWindow(ULONG ulGraphicGroup)
{
	// �ٷֱȣ��޸����꣬��ʼ�Ŵ�
	CRect IArect;
	(void)GetIARectByGroup(ulGraphicGroup, IArect);
	CRect IArectPercent;
	TransIARect2Percent(IArect, IArectPercent);

	CVideoPane* pMagnifyPane = (CVideoPane*)ulGraphicGroup;
	IVS_SDK_UpdateWindowPartial(GetOCXPlayerCtrl()->GetSDKSessionID(), m_ulPlayHandle, pMagnifyPane->GetVideoWnd(), &IArectPercent);
	pMagnifyPane->SetIArectPercent(IArectPercent);
//	pMagnifyPane->SetPlayHandle(m_ulPlayHandle);
//	pMagnifyPane->SetPlayState(VIDEO_PLAY_STATE_REALTIME);
	pMagnifyPane->ResetBarPosition();
}

// ��ȡ����ͼ��Ĵ�С
IVS_INT32 CVideoPane::GetMediaSize(SIZE& size)
{
	// ͨ��MediaInfo����ȡͼ���С������sz
	IVS_MEDIA_INFO strMediaInfo;
	memset(&strMediaInfo,0,sizeof(IVS_MEDIA_INFO));
	IVS_INT32 iRet = IVS_SDK_GetMediaInfo(GetOCXPlayerCtrl()->GetSDKSessionID(), m_ulPlayHandle, &strMediaInfo);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "get media info failed","NA");
		return iRet;
	}

	size.cx = (LONG)strMediaInfo.uiVideoWidth;
	size.cy = (LONG)strMediaInfo.uiVideoHeight;
	return IVS_SUCCEED;
}

// ��Ӧ�ٷֱȸı��¼�
afx_msg LRESULT CVideoPane::OnMagnifyRateChange(WPARAM wParam,LPARAM lParam)
{
	int iRate = (int)wParam;
	ULONG ulGraphicGroup = (ULONG)lParam;

	// �޸ķŴ󴰿ڵ���ʾ
	if (IVS_SUCCEED != ChangeSrcGraphicRect(iRate, ulGraphicGroup))
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "change src rect failed", "NA");
		return IVS_FAIL;
	}

	UpdataMagnifyWindow(ulGraphicGroup);
	return 0;
}

// �޸ķŴ󴰿ڵ���ʾ
IVS_INT32 CVideoPane::ChangeSrcGraphicRect(int iRate,ULONG ulGraphicGroup)
{
	IVS_INT32 iRet = IVS_FAIL;
	// ������ȥ��չ
	SIZE sz = {0};
	iRet = GetMediaSize(sz);
	CHECK_NOT_EQUAL(IVS_SUCCEED, iRet, iRet);

	int iChangedx = sz.cx*100/iRate; // �ٷֱȳ��ȣ��޸ĺ��
	int iChangedy = sz.cy*100/iRate;

	// ͼ������
	CRect IArect;
	(void)GetIARectByGroup(ulGraphicGroup, IArect);

	// ������С���Ȼ�ȡ���ĵ�
	int centx = IArect.left + IArect.Width()/2;
	int centy = IArect.top  + IArect.Height()/2;

	CRect changeRect;
	changeRect.left =  centx -  iChangedx/2;
	changeRect.right = centx +  iChangedx/2;
	changeRect.top = centy -  iChangedy/2;
	changeRect.bottom = centy +  iChangedy/2;

	// ��Խ�紦��.��������
	if (changeRect.left < 0)
	{
		//changeRect.left += (changeRect.Width() - IArect.Width())/2;
		changeRect.right -= changeRect.left;
		changeRect.left = 0;
		
	}
	// ��Խ�紦��.��������
	if(changeRect.top < 0)
	{
		changeRect.bottom -= changeRect.top;
		changeRect.top = 0;
		
	}
	// ��Խ�紦��.��������
	if (changeRect.right > sz.cx)
	{
		changeRect.left -= changeRect.right - sz.cx;
		changeRect.right = sz.cx;
	}
	// ��Խ�紦��.��������
	if (changeRect.bottom > sz.cy)
	{
		changeRect.top -= changeRect.bottom - sz.cy;
		changeRect.bottom = sz.cy;
	}

	// ��ȡԭ����ͼ��
	IA_GRAPHICS graphicsTemp;
	ULONG ulNum = 1;// ��ȡһ��ͼ��
	Draw_GetGraphicsByGroup((DRAW_HANDLE)m_ulDrawHandle, ulGraphicGroup, &graphicsTemp, &ulNum);

	SortIARect2Point(graphicsTemp.IAPoints[0], graphicsTemp.IAPoints[1], graphicsTemp.IAPoints[2], graphicsTemp.IAPoints[3], changeRect);
	// ��������
	graphicsTemp.IAPoints[4] = graphicsTemp.IAPoints[0];

	// ɾ��ԭ���ľ��ο�
	Draw_DeleteGraphicsByGroup((DRAW_HANDLE *)m_ulDrawHandle, ulGraphicGroup);
	Draw_SetGraphics((DRAW_HANDLE *)m_ulDrawHandle, &graphicsTemp, 1, 0);
	return IVS_SUCCEED;
}

// ���ݾ��Σ������ĸ������
void CVideoPane::SortIARect2Point(POINT& p1, POINT& p2, POINT& p3, POINT& p4, const CRect& changeRect)const
{
	if (p1.x > p2.x)
	{
		p1.x = changeRect.right;
		p2.x = changeRect.left;
		if (p1.x > p3.x)
		{
			p3.x = p2.x;
			p4.x = p1.x;
		}
		else
		{
			p3.x = p1.x;
			p4.x = p2.x;
		}
	}
	else if (p1.x < p2.x)
	{
		p1.x = changeRect.left;
		p2.x = changeRect.right;
		if (p1.x < p3.x)
		{
			p3.x = p2.x;
			p4.x = p1.x;
		}
		else
		{
			p3.x = p1.x;
			p4.x = p2.x;
		}
	}
	else
	{
		if (p1.x > p3.x)
		{
			p1.x = changeRect.right;
			p2.x = changeRect.right;
			p3.x = changeRect.left;
			p4.x = changeRect.left;
		}
		else
		{
			p1.x = changeRect.left;
			p2.x = changeRect.left;
			p3.x = changeRect.right;
			p4.x = changeRect.right;
		}
	}


	if (p1.y > p2.y)
	{
		p1.y = changeRect.bottom;
		p2.y = changeRect.top;
		if (p1.y > p3.y)
		{
			p3.y = p2.y;
			p4.y = p1.y;
		}
		else
		{
			p3.y = p1.y;
			p4.y = p2.y;
		}
	}
	else if (p1.y < p2.y)
	{
		p1.y = changeRect.top;
		p2.y = changeRect.bottom;
		if (p1.y < p3.y)
		{
			p3.y = p2.y;
			p4.y = p1.y;
		}
		else
		{
			p3.y = p1.y;
			p4.y = p2.y;
		}
	}
	else
	{
		if (p1.y > p3.y)
		{
			p1.y = changeRect.bottom;
			p2.y = changeRect.bottom;
			p3.y = changeRect.top;
			p4.y = changeRect.top;
		}
		else
		{
			p1.y = changeRect.top;
			p2.y = changeRect.top;
			p3.y = changeRect.bottom;
			p4.y = changeRect.bottom;
		}
	}
}

// ��Ӧ�رվֲ��Ŵ��¼� wParam ���͵�pane lParam ��id
afx_msg LRESULT CVideoPane::OnMagnifyClose(WPARAM wParam,LPARAM lParam)
{
	ULONG ulGraphicGroup = (ULONG)lParam;
	CVideoPane* pMagnifyPane = (CVideoPane*)ulGraphicGroup;
	CHECK_POINTER(pMagnifyPane, 0);
	// ɾ�����ο�,TODO �����Ƿ��ͷź���Ķ���
	if (IVS_SUCCEED != Draw_DeleteGraphicsByGroup((DRAW_HANDLE *)m_ulDrawHandle, ulGraphicGroup))
	{
		BP_RUN_LOG_ERR(IVS_FAIL,"close failed","group id not true");
		return 0;
	}
	// �ر�����ץ���߳�
	BP_DBG_LOG("PrvePlayState: %d, PlayState: %d", pMagnifyPane->GetPrvePlayState(), pMagnifyPane->GetPlayState());
	unsigned long iPlayStatus = CGlobalPlayMgr::Instance().GetPlayState(pMagnifyPane->GetPlayHandle());
	if (VIDEO_PLAY_STATE_RESTARTING == pMagnifyPane->GetPrvePlayState())
	{
		pMagnifyPane->SetPlayState(iPlayStatus);
	}
	else
	{
		pMagnifyPane->SetPlayState(pMagnifyPane->GetPrvePlayState());
	}
	// ���ô�������,������ڻ��Ǿֲ��Ŵ�״̬���Ͱ�֮ǰ�����û�ȥ��Ҫ��������;���޸��ˣ�����Ͳ���ԭ��
	if (TYPE_WND_MAGNIFY == pMagnifyPane->GetTypeWnd())
	{
		pMagnifyPane->SetTypeWnd(pMagnifyPane->GetPrveTypeWnd());
	}

	pMagnifyPane->SetPrvePlayState(VIDEO_PLAY_STATE_NOT_PLAYING);
	BP_DBG_LOG("PrvePlayState: %d, PlayState: %d", 
	pMagnifyPane->GetPrvePlayState(), pMagnifyPane->GetPlayState());
	pMagnifyPane->StopSerialSnapshot();

	// ����pane�ľֲ��Ŵ�����
	pMagnifyPane->SetDstMagnifyState(BOTTOMBAR_MAGNIFY_CLOSE);
// 	pMagnifyPane->DeleteZoomDirctDlg();
// 	pMagnifyPane->DeleteZoomBar();
// 	pMagnifyPane->DeleteZoomTitleBar();
	// ���ع�����
	pMagnifyPane->HideZoomWnd();
	IVS_SDK_DelPlayWnd(GetOCXPlayerCtrl()->GetSDKSessionID(), pMagnifyPane->GetMagnifyPlayHandle(), pMagnifyPane->GetVideoWnd());
	if (BOTTOMBAR_ALARM_OPEN != pMagnifyPane->GetRealtimeAlarmState())
	{
		// �ָ�������֮ǰ�Ĵ���
		IVS_SDK_AddPlayWnd(GetOCXPlayerCtrl()->GetSDKSessionID(), pMagnifyPane->GetPlayHandle(), pMagnifyPane->GetVideoWnd());
	}
	pMagnifyPane->SetMagnifyPlayHandle(0);//�����֮ǰ��
	// �ֲ��Ŵ��Ƴ���ʱ������ǰ����״̬λ��

	pMagnifyPane->Invalidate(TRUE);
	pMagnifyPane->ResetBarPosition();
	std::string strTemp = "";
	pMagnifyPane->SetZoomName(strTemp);
	m_MagnifyNum--;
	m_ulCurrentGraphicGroup = 0;
	this->FreeNameNum(ulGraphicGroup);
	
	// �ָ�3D״̬
	if (pMagnifyPane->GetOpend3DPTZStatus())
	{
		pMagnifyPane->Set3DPTZStateOnly(PTZ_3D_OPEN);
	}

	// �ж���������ǲ������Զ��������Զ���������£���һ����������
	if (VIDEO_PLAY_STATE_RESTARTING == iPlayStatus)
	{
		pMagnifyPane->SetPlayState(VIDEO_PLAY_STATE_RESTARTING);
		pMagnifyPane->SetResumeState(true);
		pMagnifyPane->Invalidate(TRUE);
	}
	
	// ��ѵ��ť
	if (TYPE_WND_TURNCTUISE == pMagnifyPane->GetPrveTypeWnd())
	{
		pMagnifyPane->SetTurnCruiseStyle(pMagnifyPane->GetPrveTurnCruiseStyle());
		pMagnifyPane->SetPrveTurnCruiseStyle(0);
		pMagnifyPane->ResetBarPosition();
	}

	return 0;
}//lint !e715

// ��Ӧ�ر����оֲ��Ŵ��¼�
afx_msg LRESULT CVideoPane::OnMagnifyCloseAll(WPARAM wParam,LPARAM lParam)
{
	// ��ȡ����ͼ�Σ���ÿ�����͹ر���Ϣ,����������ڵľֲ��Ŵ������������´ο��Իָ�
	ULONG num = 3;
	(void)this->GetGraphics(m_szGraphics, &num);
	m_ResumeMagnifyNum = num;
	for (ULONG i=0; i < num; i++)
	{
		::SendMessage(GetSafeHwnd(), WM_MAGNIFY_CLOSE, 0, (int)m_szGraphics[i].ulGraphicGroup);
	}
	// ���ⷢ���쳣������²��ָܻ����
	m_MagnifyNum = 0;
	return 0;
}//lint !e715

// ��ʱ�ط��϶����λ���ڲ��ſ�ʼʱ��֮ǰ����������Чʱ���ڣ���Ҫ���¿����ط�
LRESULT CVideoPane::OnInstantReplayStart(WPARAM wParam, LPARAM pParam)
{
    //StopRecordPlay();
    if (m_bIsInstanstPlayBack)
    {
        IVS_UINT32 ulRet = (IVS_UINT32)IVS_SDK_StopPlatformPlayBack(GetOCXPlayerCtrl()->GetSDKSessionID(),m_ulInstanstPlayBackHandle);
        if (IVS_SUCCEED == ulRet)
        {
            (void)m_pPlayTimerBar->StopRecordPlay();
            //m_pRealTimeToolBar->SetRecordPlayStop(TRUE);
            m_bIsInstansPlayPause = FALSE;
        }
    }
    SetStartPlayTime();
    (void)StartRecordPlay();
    return IVS_SUCCEED;
}//lint !e715

//��ʱ�طű���϶������������ݵ�������
LRESULT CVideoPane::OnInstantReplayMove(WPARAM wParam,LPARAM pParam)
{
    IVS_UINT32 ulTime = (IVS_UINT32)pParam;
    TRACE("�϶����ʱ��:%u\n", ulTime);
    IVS_INT32 iRet = IVS_FAIL;
    if (m_bIsInstanstPlayBack)
    {
        if (!m_bIsInstansPlayPause)
        {
            iRet = IVS_SDK_SetPlayBackTime(GetOCXPlayerCtrl()->GetSDKSessionID(),m_ulInstanstPlayBackHandle,ulTime);
            if (IVS_SUCCEED == iRet)
            {
                (void)m_pPlayTimerBar->StartRecordPlay();
            }
            else
            {
                BP_RUN_LOG_ERR(iRet,"CVideoPane","Set Play Back Time is Error %d time is %d",iRet,ulTime);
            }
        }
    }
    return iRet;
}//lint !e715

// ��ʱ�ط�¼��ν����¼�����
LRESULT CVideoPane::onInstantReplaySegStop(WPARAM wParam, LPARAM pParam)
{
	ASSERT(m_ulInstanstPlayBackHandle == (ULONG)wParam);
	if (m_pPlayTimerBar)
	{
		m_pPlayTimerBar->PostMessage(WM_TIMERBAR_PLAYBACK_STOP, 0, 0);
	}

	return 0;
}//lint !e715


void CVideoPane::ResumeRealPlay()
{
	////ɾ��¼�񲥷ŵ�Wnd
	if (m_bIsInstansPlayPause)
	{
		(void)StartRecordPlay();
	}

	IVS_SDK_DelPlayWnd(GetOCXPlayerCtrl()->GetSDKSessionID(), m_ulInstanstPlayBackHandle, GetVideoWnd());
	if (m_bIsInstanstPlayBack)
	{
		IVS_UINT32 ulRet = (IVS_UINT32)IVS_SDK_StopPlatformPlayBack(GetOCXPlayerCtrl()->GetSDKSessionID(),m_ulInstanstPlayBackHandle);
		if (IVS_SUCCEED != ulRet)
		{
			BP_RUN_LOG_ERR((int)ulRet,"CVideoPane Resume RealPlay failed","Error Code is %d",ulRet);
			//return;
		}
	}
	////�ָ�ʵ����Wnd
	//IVS_SDK_AddPlayWnd(GetOCXPlayerCtrl()->GetSDKSessionID(), ulHandle, GetVideoWnd());
	IVS_SDK_AddPlayWnd(GetOCXPlayerCtrl()->GetSDKSessionID(), m_ulInstanstRealPlayHandle, GetVideoWnd());
	//���ؼ�ʱ�ط����
	if (m_pPlayTimerBar != nullptr)
	{
		(void)m_pPlayTimerBar->StopRecordPlay();
	}
	m_ulInstantReplayStyle = FALSE;
	ReSetToolBarStatus();
	m_bIsInstanstPlayBack = FALSE;
	m_bIsInstansPlayPause = FALSE;
	//SetToolBarBtnVisableStatus(static_cast<ULONGLONG>(m_ulToolBarStatus));
	m_pRealTimeToolBar->SetInstansPlayBack(FALSE);
	// ���ô�������
	SetTypeWnd(TYPE_WND_NOMAL);
	m_pRealTimeToolBar->SetIsStartPlay(TRUE);

	ResetBarPosition();

	// �ж���������ǲ������Զ��������Զ���������£���һ����������
	unsigned long iPlayStatus = CGlobalPlayMgr::Instance().GetPlayState(GetPlayHandle());
	if (VIDEO_PLAY_STATE_RESTARTING == iPlayStatus)
	{
		SetPlayState(VIDEO_PLAY_STATE_RESTARTING);
		SetResumeState(true);
		Invalidate(TRUE);
	}
}

void CVideoPane::ReSetToolBarStatus()
{
	CHECK_POINTER_VOID(GetVideoPaneMgr());
	SetToolBarBtnVisableStatus(static_cast<ULONGLONG>(GetVideoPaneMgr()->GetToolBarVisableStatus()));
}

LONG CVideoPane::StartRecordPlay()
{
	CHECK_POINTER(m_pRealTimeToolBar,IVS_PARA_INVALID);

	m_pRealTimeToolBar->SetInstansPlayBack(TRUE);
	//if (!m_bIsInstanstPlayBack)
	//{
	CIVS_OCXPlayerCtrl * pOcx =  GetOCXPlayerCtrl();
	CHECK_POINTER(pOcx,0);

	IVS_INT32 iRet = IVS_FAIL;
	if (!m_bIsInstansPlayPause)
	{
		ST_TIMER_MSG stTimerMsg;
		memset(&stTimerMsg,0,sizeof(ST_TIMER_MSG));
		if(!m_pPlayTimerBar->IsCanPlayBack(stTimerMsg))
		{
            ST_TIME NextTime;
            if (m_pPlayTimerBar->FindNextPlayRange(stTimerMsg.stCurrentTime, NextTime))
            {
                m_pPlayTimerBar->SetPlayCurrentTime(NextTime);
                m_pPlayTimerBar->SetStartPlayTime();
                m_pPlayTimerBar->GetTimeMsg(NextTime, stTimerMsg);
            }
            else
            {
			    IVS_SDK_DelPlayWnd(GetOCXPlayerCtrl()->GetSDKSessionID(), GetPlayHandle(), GetVideoWnd());
                return -1;
            }
			
		}

		CTime time = CTime::GetCurrentTime();
		CString strStartTime;
		CString strEndTime;

		struct tm local;
		memset(&local, 0, sizeof(tm));
		time_t currenttime;
		//ת��UTCʱ��
		CTime timeStart(time.GetYear(),time.GetMonth(),time.GetDay(),
			stTimerMsg.stCurrentTime.iHour,stTimerMsg.stCurrentTime.iMinute,stTimerMsg.stCurrentTime.iScond);
		currenttime = timeStart.GetTime();
		gmtime_s(&local, &currenttime);
		currenttime = mktime(&local);

		time_t endttime;
		CTime timeEnd(time.GetYear(),time.GetMonth(),time.GetDay(),
			stTimerMsg.stEndTime.iHour,stTimerMsg.stEndTime.iMinute,stTimerMsg.stEndTime.iScond);
		endttime = timeEnd.GetTime();
		memset(&local, 0, sizeof(tm));
		gmtime_s(&local,&endttime);
		endttime = mktime(&local);

		CTime newStartTime(currenttime);
		CTime newEndTime(endttime);


		strStartTime.Format("%04d%02d%02d%02d%02d%02d",newStartTime.GetYear(),newStartTime.GetMonth(),newStartTime.GetDay(),
			newStartTime.GetHour(),newStartTime.GetMinute(),newStartTime.GetSecond());
		strEndTime.Format("%04d%02d%02d%02d%02d%02d",newEndTime.GetYear(),newEndTime.GetMonth(),newEndTime.GetDay(),
			newEndTime.GetHour(),newEndTime.GetMinute(),newEndTime.GetSecond());

		IVS_PLAYBACK_PARAM stPlaybackParam;
		memset(&stPlaybackParam, 0, sizeof(IVS_PLAYBACK_PARAM));
		stPlaybackParam.fSpeed = 1.0;
		stPlaybackParam.uiProtocolType = 2;
		(void)CToolsHelp::Memcpy(stPlaybackParam.stTimeSpan.cStart,IVS_TIME_LEN,strStartTime,static_cast<size_t>(strStartTime.GetLength()));
		(void)CToolsHelp::Memcpy(stPlaybackParam.stTimeSpan.cEnd,IVS_TIME_LEN,strEndTime,static_cast<size_t>(strEndTime.GetLength()));
		//��Ҫ���浱ǰ��¼�񲥷ž����ֹͣ����ʱʹ��
		ULONG ulHandle = 0;
		m_ulInstanstRealPlayHandle = GetPlayHandle();
		IVS_SDK_DelPlayWnd(GetOCXPlayerCtrl()->GetSDKSessionID(), GetPlayHandle(), GetVideoWnd());
		iRet = IVS_SDK_StartPlatformPlayBack(GetOCXPlayerCtrl()->GetSDKSessionID(), m_szCameraID, &stPlaybackParam, GetVideoWnd(), &ulHandle);
		if (IVS_SUCCEED == iRet)
		{
			CGlobalPlayMgr::Instance().RegPlayBackHandle(ulHandle, this, m_szCameraID);
			m_ulInstanstPlayBackHandle = ulHandle;
			m_bIsInstanstPlayBack = TRUE;
		}
		else
		{
			BP_RUN_LOG_ERR(iRet,"CVideoPane","Start Record Play is Error %d",iRet);
		}
	}
	else
	{
		iRet = IVS_SDK_PlayBackResume(GetOCXPlayerCtrl()->GetSDKSessionID(),m_ulInstanstPlayBackHandle);
		if (IVS_SUCCEED == iRet)
		{
			m_bIsInstansPlayPause = FALSE;
		}	
	}
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet,"CVideoPane","Start Record Play is Error %d",iRet);
		return iRet;
	}
    Sleep(100); //��ͣ0.1�룬Ҫ��Ȼ������ʱ����ʧ��
	(void)m_pPlayTimerBar->StartRecordPlay();
	m_pRealTimeToolBar->SetRecordPlayStop(FALSE);
	return iRet;
}

LONG CVideoPane::StopRecordPlay()
{
	if (m_bIsInstanstPlayBack)
	{
		IVS_UINT32 ulRet = (IVS_UINT32)IVS_SDK_StopPlatformPlayBack(GetOCXPlayerCtrl()->GetSDKSessionID(),m_ulInstanstPlayBackHandle);
		(void)CGlobalPlayMgr::Instance().UnRegPlayHandle(m_ulInstanstPlayBackHandle, this);
		
		if (IVS_SUCCEED == ulRet)
		{
			(void)m_pPlayTimerBar->StopRecordPlay();
			m_pRealTimeToolBar->SetRecordPlayStop(TRUE);
			m_bIsInstansPlayPause = FALSE;
			//ResetBarPosition();
			Invalidate(TRUE);
			return 0;
		}
	}

	return 0;
}

LONG CVideoPane::PauseRecordPlay()
{
	IVS_UINT32 ulRet = IVS_FAIL;
	if (!m_bIsInstansPlayPause)
	{
		ulRet = (IVS_UINT32)IVS_SDK_PlayBackPause(GetOCXPlayerCtrl()->GetSDKSessionID(),m_ulInstanstPlayBackHandle);
		if (IVS_SUCCEED == ulRet)
		{
			(void)m_pPlayTimerBar->StopRecordPlay();
			m_pRealTimeToolBar->SetRecordPlayStop(TRUE);
			m_bIsInstansPlayPause = TRUE;
			//ResetBarPosition();
			//Invalidate(TRUE);
			return 0;
		}
	}

	//IVS_SDK_StopPlatformPlayBack(GetOCXPlayerCtrl()->GetSDKSessionID(),m_ulInstanstPlayBackHandle);
	//m_bIsInstanstPlayBack = FALSE;

	return (long)ulRet;
}

// �Ŵ����갴�£����еľֲ��Ŵ���Ӷ��������ߣ���Ҫ����ȥ����
void CVideoPane::MagnifyMouseDown( const CPoint &point )
{
	if (!PointIsVideoRect(point))
	{
		BP_RUN_LOG_INF("point not in rect","na");
		return;
	}
	m_bIsButtonDown = true;
	m_DrawMoveSuccessSum = 1;
	// ���û�ͼģʽ
	CHECK_POINTER_VOID(m_ulDrawHandle);
	CHECK_POINTER_VOID(m_pVideoPaneMgr);
	UINT scale = m_pVideoPaneMgr->GetDispalyScale();
	BOOL bScaleModeFull = scale == DISPLAY_SCALE_FULL ? TRUE : FALSE;
	(VOID)Draw_SetDisplayScaleMode((DRAW_HANDLE)m_ulDrawHandle, bScaleModeFull);

	// �鿴�Ƿ�ѡ����ͼ�Σ����ѡ���˾ͽ���༭״̬������ʼ���»�ͼ
	LONG lRet;
	BOOL bAllowDelete = FALSE;
	ULONG ulSelectedGraphicGroup = 0;
	lRet = Draw_SelectGraphicByPoint((DRAW_HANDLE)m_ulDrawHandle, point, &ulSelectedGraphicGroup, &bAllowDelete);
	if (IVS_SUCCEED == lRet)// ѡ��ͼ��
	{
		m_ulGraphicState = GRAPHICS_STATE_EDITING;
		m_ulCurrentGraphicGroup = ulSelectedGraphicGroup;
	} 
	else // û��ѡ��ͼ�� 
	{ 
		// �жϵ�ǰ��״̬,���û�в��Ż��������� �ͷ�����β���
		if (VIDEO_PLAY_STATE_REALTIME != m_ulPlayState && VIDEO_PLAY_STATE_VOD != m_ulPlayState && VIDEO_PLAY_STATE_LOCALFILE != m_ulPlayState)
		{
			BP_RUN_LOG_ERR(IVS_FAIL, "PlayState Not realytime or record", "NA");
			return;
		}

		// ��ȡ���е�pane�������ȡʧ�ܣ����ٻ�������������������Ͳ��ٻ�.���㴰��ĸ���
		ULONG  uPaneID = m_pVideoPaneMgr->GetMagnifyFreeWnd(m_ulPaneID);
		if (IVS_OCX_RET_GET_INFO_ERROR == uPaneID || ((m_MagnifyNum >= MAGNIFY_MAX_NUM) || (m_MagnifyNum >= m_pVideoPaneMgr->GetNumOfDisplayPane() - 1)))
		{
			m_ulGraphicState = GRAPHICS_STATE_NONE;
			m_ulCurrentGraphicGroup = 0;
		}
		else
		{
			CVideoPane* pMagnifyPane = m_pVideoPaneMgr->GetWndPointerByID(uPaneID);
			CHECK_POINTER_VOID(pMagnifyPane);
			m_ulGraphicState = GRAPHICS_STATE_DRAWING;
			m_ulCurrentGraphicGroup = 0;
			// ׼���µ�����
			IA_GRAPHICS* pMagnifyGraphics = IVS_NEW(pMagnifyGraphics);
			memset(pMagnifyGraphics,0x0,sizeof(IA_GRAPHICS));
			pMagnifyGraphics->ulLineWidth = 2;
			pMagnifyGraphics->LineColor = m_IAColor;
			pMagnifyGraphics->ulGraphicType = IA_GRAPHICS_TYPE_RECTANGLE;
			pMagnifyGraphics->ulGraphicGroup = (ULONG)pMagnifyPane; // ָ����Ϊgroupid
			pMagnifyGraphics->IAPoints[0].x  = point.x;
			pMagnifyGraphics->IAPoints[0].y  = point.y;
			pMagnifyGraphics->bAllowModifySize = TRUE;
			pMagnifyGraphics->bKeepRectShape  = TRUE;
			// ��������,����Ҫ��ȡ�ֲ��Ŵ�Ĺ��ʻ����ƣ�Ȼ�����>������
			std::string strZoom;
			(void)CI18N::GetInstance().GetResourceLanguage("Zoom", strZoom);
			m_strZoomName =  strZoom + CToolsHelp::Int2Str(this->GetFreeNameNum(pMagnifyGraphics->ulGraphicGroup));
			(void)CToolsHelp::Memcpy(pMagnifyGraphics->GraphicGroupName, sizeof(pMagnifyGraphics->GraphicGroupName), m_strZoomName.c_str(), strlen(m_strZoomName.c_str()));
			pMagnifyGraphics->NamePosition = 2;

			(void)CToolsHelp::Memcpy(&m_CurrentGraphic, sizeof(IA_GRAPHICS), pMagnifyGraphics, sizeof(IA_GRAPHICS));
			m_ulGroupDrawMaxNum = 1;
			m_ulCurrentGraphicGroup = pMagnifyGraphics->ulGraphicGroup;//��ʼ����ID
		}
	}

	if (GRAPHICS_STATE_DRAWING == m_ulGraphicState)// ��ͼ״̬
	{
		lRet = Draw_RectangleStart((DRAW_HANDLE)m_ulDrawHandle, m_ulGroupDrawMaxNum, &m_CurrentGraphic);
		CHECK_DRAW_START_SUCCESS(lRet);
	}
	else if (GRAPHICS_STATE_EDITING == m_ulGraphicState)// �༭״̬
	{
		if (!m_bIsDrawing)
		{
			(void)Draw_SelectCancel((DRAW_HANDLE)m_ulDrawHandle);

			// ���ݵ�����ѡ��ͼ��
			bAllowDelete = FALSE;
			ulSelectedGraphicGroup = 0;
			lRet = Draw_SelectGraphicByPoint((DRAW_HANDLE)m_ulDrawHandle, point, &ulSelectedGraphicGroup, &bAllowDelete);
			if (IVS_SUCCEED == lRet)// ѡ��ͼ��
			{
				m_ulGraphicState = GRAPHICS_STATE_EDITING;
				m_ulCurrentGraphicGroup = ulSelectedGraphicGroup;
			} else { // û��ѡ��ͼ��
				m_ulGraphicState = GRAPHICS_STATE_NONE;
				m_ulCurrentGraphicGroup = 0;
			}
			if (IVS_SUCCEED != lRet) return;
			
			lRet = Draw_StartModifyGraphics((DRAW_HANDLE)m_ulDrawHandle, point, LEFT_BUTTON_DOWN, m_ulCurrentGraphicGroup);
			CHECK_DRAW_START_SUCCESS(lRet);
		}
	}
	else
	{
		BP_RUN_LOG_INF("GraphicState not correct","na");
		return;
	}
	return;
}

void CVideoPane::StartMagnifyByGraphicPoint(const CPoint &point)
{
	m_bIsButtonDown = true;
	m_DrawMoveSuccessSum = 1;
	// ���û�ͼģʽ
	CHECK_POINTER_VOID(m_ulDrawHandle);
	CHECK_POINTER_VOID(m_pVideoPaneMgr);
	UINT scale = m_pVideoPaneMgr->GetDispalyScale();
	BOOL bScaleModeFull = scale == DISPLAY_SCALE_FULL ? TRUE : FALSE;
	(VOID)Draw_SetDisplayScaleMode((DRAW_HANDLE)m_ulDrawHandle, bScaleModeFull);

	// �鿴�Ƿ�ѡ����ͼ�Σ����ѡ���˾ͽ���༭״̬������ʼ���»�ͼ
	LONG lRet;
	BOOL bAllowDelete = FALSE;
	ULONG ulSelectedGraphicGroup = 0;
	lRet = Draw_SelectGraphicByGraphicPoint((DRAW_HANDLE)m_ulDrawHandle, point, &ulSelectedGraphicGroup, &bAllowDelete);
	if (IVS_SUCCEED == lRet)// ѡ��ͼ��
	{
		m_ulGraphicState = GRAPHICS_STATE_EDITING;
		m_ulCurrentGraphicGroup = ulSelectedGraphicGroup;
	} 
	else // û��ѡ��ͼ�� 
	{ 
		// �жϵ�ǰ��״̬,���û�в��Ż��������� �ͷ�����β���
		if (VIDEO_PLAY_STATE_REALTIME != m_ulPlayState && VIDEO_PLAY_STATE_VOD != m_ulPlayState && VIDEO_PLAY_STATE_LOCALFILE != m_ulPlayState)
		{
			BP_RUN_LOG_ERR(IVS_FAIL, "PlayState Not realytime or record", "NA");
			return;
		}

		// ��ȡ���е�pane�������ȡʧ�ܣ����ٻ�������������������Ͳ��ٻ�.���㴰��ĸ���
		ULONG  uPaneID = m_pVideoPaneMgr->GetMagnifyFreeWnd(m_ulPaneID);
		if (IVS_OCX_RET_GET_INFO_ERROR == uPaneID || ((m_MagnifyNum >= MAGNIFY_MAX_NUM) || (m_MagnifyNum >= m_pVideoPaneMgr->GetNumOfDisplayPane() - 1)))
		{
			m_ulGraphicState = GRAPHICS_STATE_NONE;
			m_ulCurrentGraphicGroup = 0;
		}
		else
		{
			CVideoPane* pMagnifyPane = m_pVideoPaneMgr->GetWndPointerByID(uPaneID);
			CHECK_POINTER_VOID(pMagnifyPane);
			m_ulGraphicState = GRAPHICS_STATE_DRAWING;
			m_ulCurrentGraphicGroup = 0;
			// ׼���µ�����
			IA_GRAPHICS* pMagnifyGraphics = IVS_NEW(pMagnifyGraphics);
			memset(pMagnifyGraphics,0x0,sizeof(IA_GRAPHICS));
			pMagnifyGraphics->ulLineWidth = 2;
			pMagnifyGraphics->LineColor = m_IAColor;
			pMagnifyGraphics->ulGraphicType = IA_GRAPHICS_TYPE_RECTANGLE;
			pMagnifyGraphics->ulGraphicGroup = (ULONG)pMagnifyPane; // ָ����Ϊgroupid
			pMagnifyGraphics->IAPoints[0].x  = point.x;
			pMagnifyGraphics->IAPoints[0].y  = point.y;
			pMagnifyGraphics->bAllowModifySize = TRUE;
			pMagnifyGraphics->bKeepRectShape  = TRUE;
			// ��������,����Ҫ��ȡ�ֲ��Ŵ�Ĺ��ʻ����ƣ�Ȼ�����>������
			std::string strZoom;
			(void)CI18N::GetInstance().GetResourceLanguage("Zoom", strZoom);
			m_strZoomName =  strZoom + CToolsHelp::Int2Str(this->GetFreeNameNum(pMagnifyGraphics->ulGraphicGroup));
			(void)CToolsHelp::Memcpy(pMagnifyGraphics->GraphicGroupName, sizeof(pMagnifyGraphics->GraphicGroupName), m_strZoomName.c_str(), strlen(m_strZoomName.c_str()));
			pMagnifyGraphics->NamePosition = 2;

			(void)CToolsHelp::Memcpy(&m_CurrentGraphic, sizeof(IA_GRAPHICS), pMagnifyGraphics, sizeof(IA_GRAPHICS));
			m_ulGroupDrawMaxNum = 1;
			m_ulCurrentGraphicGroup = pMagnifyGraphics->ulGraphicGroup;//��ʼ����ID
		}
	}

	if (GRAPHICS_STATE_DRAWING == m_ulGraphicState)// ��ͼ״̬
	{
		lRet = Draw_RectangleStartByGraphicPoint((DRAW_HANDLE)m_ulDrawHandle, m_ulGroupDrawMaxNum, &m_CurrentGraphic);
		CHECK_DRAW_START_SUCCESS(lRet);
	}
	else if (GRAPHICS_STATE_EDITING == m_ulGraphicState)// �༭״̬
	{
		if (!m_bIsDrawing)
		{
			(void)Draw_SelectCancel((DRAW_HANDLE)m_ulDrawHandle);

			// ���ݵ�����ѡ��ͼ��
			bAllowDelete = FALSE;
			ulSelectedGraphicGroup = 0;
			lRet = Draw_SelectGraphicByGraphicPoint((DRAW_HANDLE)m_ulDrawHandle, point, &ulSelectedGraphicGroup, &bAllowDelete);
			if (IVS_SUCCEED == lRet)// ѡ��ͼ��
			{
				m_ulGraphicState = GRAPHICS_STATE_EDITING;
				m_ulCurrentGraphicGroup = ulSelectedGraphicGroup;
			} else { // û��ѡ��ͼ��
				m_ulGraphicState = GRAPHICS_STATE_NONE;
				m_ulCurrentGraphicGroup = 0;
			}
			if (IVS_SUCCEED != lRet) return;

			lRet = Draw_StartModifyGraphicsByGraphicPoint((DRAW_HANDLE)m_ulDrawHandle, point, LEFT_BUTTON_DOWN, m_ulCurrentGraphicGroup);
			CHECK_DRAW_START_SUCCESS(lRet);
		}
	}
	else
	{
		BP_RUN_LOG_INF("GraphicState not correct","na");
		return;
	}
	return;
}
// �Ŵ����굯��
void CVideoPane::MagnifyMouseUp( const CPoint &point )
{
	CHECK_POINTER_VOID(m_ulDrawHandle);
	CHECK_POINTER_VOID(m_pVideoPaneMgr);
	if (0 == m_ulCurrentGraphicGroup)
	{
		return;
	}
	UINT scale = m_pVideoPaneMgr->GetDispalyScale();
	BOOL bScaleModeFull = scale == DISPLAY_SCALE_FULL ? TRUE : FALSE;
	(VOID)Draw_SetDisplayScaleMode((DRAW_HANDLE)m_ulDrawHandle, bScaleModeFull);

	POINT CurrentPoint;
	CurrentPoint.x = point.x;
	CurrentPoint.y = point.y;

	LONG lRet = IVS_FAIL;
	if (GRAPHICS_STATE_DRAWING == m_ulGraphicState)// ��ͼ״̬
	{
		m_MagnifyNum++; //����+1
		// ̸���ʱ��ֱ�Ӳ��룬��ֹ���ֵ��»�������������������������ƶ���ʱ���Ѿ�����
		lRet = Draw_InsertCurrentToGraphicList((DRAW_HANDLE)m_ulDrawHandle);
		m_bIsDrawing = false;
		m_ulGraphicState = GRAPHICS_STATE_EDITING;

		if (0 != m_DrawMoveSuccessSum || IVS_SUCCEED != lRet)
		{
			BP_RUN_LOG_INF("draw rect finish","failed");
			Draw_StopDrawGraphics((DRAW_HANDLE *)m_ulDrawHandle, m_ulCurrentGraphicGroup);
			Draw_DeleteGraphicsByGroup((DRAW_HANDLE *)m_ulDrawHandle, m_ulCurrentGraphicGroup);
			m_bIsDrawing = false;
			m_ulGraphicState = GRAPHICS_STATE_EDITING;
			m_MagnifyNum--;
			FreeNameNum(m_ulCurrentGraphicGroup); //TODO �������¼ӣ���Ҫ��֤
			m_strZoomName = "";//�����
			m_ulCurrentGraphicGroup = 0;
			return;
		}
		
		// ���ﻭͼ��������Ҫ���������ҵ������

		// ��ȡ�����е���ʼ�����ֹ��,ʹ�ðٷֱȱ�ʾ
		CRect IArect;
		(void)GetIARectByGroup(m_ulCurrentGraphicGroup, IArect);
		CRect IArectPercent;
		TransIARect2Percent(IArect, IArectPercent);

		// �����ԷŴ������Ϊ800%(Ҳ����12.5)��������Ҫ�޶����ο����С���ȣ��������ĵ�ȥ����Ŵ�������С��������0.5���
		// �����Ĭ�ϳ���ȣ�ֻҪһ�����ɡ�
		if (IArectPercent.Width() <= 12 || IArectPercent.Height() <= 12)
		{
			(void)ChangeSrcGraphicRect(800, m_ulCurrentGraphicGroup);
			// ���¸�������
			(void)GetIARectByGroup(m_ulCurrentGraphicGroup, IArect);
			TransIARect2Percent(IArect, IArectPercent);
		}

		// ���� groupid��ȡpane
		CVideoPane* pMagnifyPane = (CVideoPane*)m_ulCurrentGraphicGroup;
		CHECK_POINTER_VOID(pMagnifyPane);

        // add by wanglei 00165153:2013.7.15 DTS2013071209101 
        // �ر�����ץ��
        // �ô���������pMagnifyPane->CreateZoomBar();ǰ������ᵼ��ץ��COXͼ���޷��ָ�
        pMagnifyPane->StopSerialSnapshot();



		pMagnifyPane->CreateZoomTitleBar();
		pMagnifyPane->CreateZoomDirctDlg();
		pMagnifyPane->CreateZoomBar();
		pMagnifyPane->SetMagnifySrcPane(this);
		// ����pane�ľֲ��Ŵ�����
		pMagnifyPane->SetDstMagnifyState(BOTTOMBAR_MAGNIFY_OPEN);
		
		// �ر�һЩ���ֲ��Ŵ�ռ�ô��ڵ�״̬�����缴ʱ�طţ��ƾ���
		if (pMagnifyPane->GetShowInstantStatus())
		{
			::SendMessage(pMagnifyPane->GetVideoRealtimeBar()->GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(IDC_BTN_TOOL_REALTIME_RETURN, BN_CLICKED), (LPARAM)pMagnifyPane->GetVideoRealtimeBar()->GetReturnBtnHwnd());//lint !e613
		}
		if (BOTTOMBAR_PTZ_OPEN == pMagnifyPane->GetRealtimePtzState())
		{
			::SendMessage(pMagnifyPane->GetVideoRealtimeBar()->GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(IDC_BTN_TOOL_REALTIME_PTZ, BN_CLICKED), (LPARAM)pMagnifyPane->GetVideoRealtimeBar()->GetPTZBtnHwnd());//lint !e613
		}
		// ����3D״̬
		if (PTZ_3D_OPEN == pMagnifyPane->Get3DPTZState())
		{
			pMagnifyPane->SetOpend3DPTZStatus(true);
			pMagnifyPane->Set3DPTZStateOnly(PTZ_3D_CLOSE);
		}

		// ע �����Ǹ��ر�֮���ٽ����������
		// ���˸澯�������⣬�����Ķ����Ƴ�����������һ�ε�״̬
		if (BOTTOMBAR_ALARM_OPEN != pMagnifyPane->GetRealtimeAlarmState())
		{
			IVS_SDK_DelPlayWnd(GetOCXPlayerCtrl()->GetSDKSessionID(), pMagnifyPane->GetPlayHandle(), pMagnifyPane->GetVideoWnd());
		}

		// ���洰������
		// ��ѵ��ť
		if (TYPE_WND_TURNCTUISE == pMagnifyPane->GetTypeWnd())
		{
			pMagnifyPane->SetPrveTurnCruiseStyle(pMagnifyPane->GetTurnCruiseStyle());
			pMagnifyPane->SetTurnCruiseStyle(0);
		}
		pMagnifyPane->SetPrveTypeWnd(pMagnifyPane->GetTypeWnd());
		// ���ô�������
		pMagnifyPane->SetTypeWnd(TYPE_WND_MAGNIFY);

		// ����һ�±��Ŵ�֮ǰ��״̬,����ΪԴ���ڵĲ���״̬

		pMagnifyPane->SetPrvePlayState(pMagnifyPane->GetPlayState());
		pMagnifyPane->SetPlayState(GetPlayState());
		pMagnifyPane->SetMagnifyPlayHandle(m_ulPlayHandle);
		IVS_SDK_AddPlayWindowPartial(GetOCXPlayerCtrl()->GetSDKSessionID(), m_ulPlayHandle, pMagnifyPane->GetVideoWnd(), &IArectPercent);
		
		pMagnifyPane->SetIArectPercent(IArectPercent);
		// ��ȡ�Ŵ����
		int iRate = GetMagnifyRateByIARect(IArect);
		if (0 != iRate)
		{
			if (NULL != pMagnifyPane->m_pZoomBar)
			{
				pMagnifyPane->m_pZoomBar->SetMagnifyRate(100*100/iRate);
			}
		}
		
		// ��������
		std::string strZoomName = ">";
		strZoomName += m_strZoomName;
		pMagnifyPane->SetZoomName(m_strZoomName);
		pMagnifyPane->m_pZoomTitleBar->SetZoomTitleName(strZoomName);
		pMagnifyPane->SetTranslucentStatus(TRUE);
		pMagnifyPane->SetAllBarStatus(1);
		pMagnifyPane->ResetBarPosition();// ��ʾ��������,����Ҫ����������Ĳ���״̬������
		pMagnifyPane->SetTranslucentStatus(FALSE);
		pMagnifyPane->Invalidate(TRUE);
		m_strZoomName = "";//�����

		// ����һ����ʱ�����Ƚ���ˢ�³����Ժ�ģ������ʹ�ý����ܹ��ص���ǰ����
		m_MagnifyLastPoint.x = point.x;
		m_MagnifyLastPoint.y = point.y;
		(void)SetTimer(MAGNIFY_TIMER_ID, 200, NULL);
		//ResetBarPosition();
	}
	else if (GRAPHICS_STATE_EDITING == m_ulGraphicState)// �༭״̬
	{
		(void)Draw_ModifyFinish((DRAW_HANDLE)m_ulDrawHandle, m_ulCurrentGraphicGroup, CurrentPoint);
		m_bIsDrawing = false;

		CRect IArect;
		(void)GetIARectByGroup(m_ulCurrentGraphicGroup, IArect);
		// ��ȡ�Ŵ����
		int iRate = GetMagnifyRateByIARect(IArect);

		CVideoPane* pMagnifyPane = (CVideoPane*)m_ulCurrentGraphicGroup;
		CHECK_POINTER_VOID(pMagnifyPane);
		// ����Ŵ��������
		if (iRate < 12)
		{
			(void)ChangeSrcGraphicRect(800, m_ulCurrentGraphicGroup);
			if (NULL != pMagnifyPane->m_pZoomBar)
			{
				pMagnifyPane->m_pZoomBar->SetMagnifyRate(800);
			}
		}
		else
		{
			if (NULL != pMagnifyPane->m_pZoomBar)
			{
				// ���ñ���
				pMagnifyPane->m_pZoomBar->SetMagnifyRate(100*100/iRate);
			}
		}
		UpdataMagnifyWindow(m_ulCurrentGraphicGroup);

		// ��ѵ��ť
		if (TYPE_WND_TURNCTUISE == pMagnifyPane->GetPrveTypeWnd())
		{
			BP_RUN_LOG_INF("m_pTurnCruisePane SW_HIDE","na");
			if (NULL != pMagnifyPane->GetTurnCruisePane())
			{
				pMagnifyPane->GetTurnCruisePane()->ShowWindow(SW_HIDE);//lint !e613
				pMagnifyPane->GetTurnCruisePane()->Invalidate(TRUE);//lint !e613
			}
		}
	}
	return;
}

// ����ֱ�Ӽ���Ŵ�����������϶������������ʾ�����ݷ����仯
int CVideoPane::GetMagnifyRateByIARect(const CRect& IArect)
{
	SIZE sz = {0};
	(void)GetMediaSize(sz);
	if ((0 >= sz.cx) || (0 >= sz.cy))
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "Video Size is Zero","NA");
		return 0;
	}

	return IArect.Width()*100/sz.cx;
}


void CVideoPane::FinishMagnifyByGraphicPoint(const CPoint &point)
{
	UINT scale = m_pVideoPaneMgr->GetDispalyScale();
	BOOL bScaleModeFull = scale == DISPLAY_SCALE_FULL ? TRUE : FALSE;
	(VOID)Draw_SetDisplayScaleMode((DRAW_HANDLE)m_ulDrawHandle, bScaleModeFull);

	LONG lRet = IVS_FAIL;
	if (GRAPHICS_STATE_DRAWING == m_ulGraphicState)// ��ͼ״̬
	{
		m_MagnifyNum++; //����+1
		lRet = Draw_RectangleFinishByGraphicPoint((DRAW_HANDLE)m_ulDrawHandle, point);
		m_bIsDrawing = false;
		m_ulGraphicState = GRAPHICS_STATE_EDITING;

		if (0 != m_DrawMoveSuccessSum || IVS_SUCCEED != lRet)
		{
			BP_RUN_LOG_INF("draw rect finish","failed");
			Draw_StopDrawGraphics((DRAW_HANDLE *)m_ulDrawHandle, m_ulCurrentGraphicGroup);
			Draw_DeleteGraphicsByGroup((DRAW_HANDLE *)m_ulDrawHandle, m_ulCurrentGraphicGroup);
			m_bIsDrawing = false;
			m_ulGraphicState = GRAPHICS_STATE_EDITING;
			m_MagnifyNum--;
			FreeNameNum(m_ulCurrentGraphicGroup); //TODO �������¼ӣ���Ҫ��֤
			m_strZoomName = "";//�����
			return;
		}

		// ���ﻭͼ��������Ҫ���������ҵ������

		// ��ȡ�����е���ʼ�����ֹ��,ʹ�ðٷֱȱ�ʾ
		CRect IArect;
		(void)GetIARectByGroup(m_ulCurrentGraphicGroup, IArect);
		CRect IArectPercent;
		TransIARect2Percent(IArect, IArectPercent);

		// �����ԷŴ������Ϊ800%(Ҳ����12.5)��������Ҫ�޶����ο����С���ȣ��������ĵ�ȥ����Ŵ�������С��������0.5���
		// �����Ĭ�ϳ���ȣ�ֻҪһ�����ɡ�
		if (IArectPercent.Width() <= 12 || IArectPercent.Height() <= 12)
		{
			(void)ChangeSrcGraphicRect(800, m_ulCurrentGraphicGroup);
			// ���¸�������
			(void)GetIARectByGroup(m_ulCurrentGraphicGroup, IArect);
			TransIARect2Percent(IArect, IArectPercent);
		}

		// ���� groupid��ȡpane
		CVideoPane* pMagnifyPane = (CVideoPane*)m_ulCurrentGraphicGroup;
		pMagnifyPane->CreateZoomTitleBar();
		pMagnifyPane->CreateZoomDirctDlg();
		pMagnifyPane->CreateZoomBar();
		pMagnifyPane->SetMagnifySrcPane(this);
		// ����pane�ľֲ��Ŵ�����
		pMagnifyPane->SetDstMagnifyState(BOTTOMBAR_MAGNIFY_OPEN);

		// �ر�һЩ���ֲ��Ŵ�ռ�ô��ڵ�״̬�����缴ʱ�طţ��ƾ���
		if (pMagnifyPane->GetShowInstantStatus())
		{
			::SendMessage(pMagnifyPane->GetVideoRealtimeBar()->GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(IDC_BTN_TOOL_REALTIME_RETURN, BN_CLICKED), (LPARAM)pMagnifyPane->GetVideoRealtimeBar()->GetReturnBtnHwnd());//lint !e613
		}
		if (BOTTOMBAR_PTZ_OPEN == pMagnifyPane->GetRealtimePtzState())
		{
			::SendMessage(pMagnifyPane->GetVideoRealtimeBar()->GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(IDC_BTN_TOOL_REALTIME_PTZ, BN_CLICKED), (LPARAM)pMagnifyPane->GetVideoRealtimeBar()->GetPTZBtnHwnd());//lint !e613
		}

		// ע �����Ǹ��ر�֮���ٽ����������
		// ���˸澯�������⣬�����Ķ����Ƴ�����������һ�ε�״̬
		if (BOTTOMBAR_ALARM_OPEN != pMagnifyPane->GetRealtimeAlarmState())
		{
			IVS_SDK_DelPlayWnd(GetOCXPlayerCtrl()->GetSDKSessionID(), pMagnifyPane->GetPlayHandle(), pMagnifyPane->GetVideoWnd());
		}

		// ����һ�±��Ŵ�֮ǰ��״̬,����ΪԴ���ڵĲ���״̬
		pMagnifyPane->SetPrvePlayState(pMagnifyPane->GetPlayState());
		pMagnifyPane->SetPlayState(GetPlayState());
		pMagnifyPane->SetMagnifyPlayHandle(m_ulPlayHandle);
		IVS_SDK_AddPlayWindowPartial(GetOCXPlayerCtrl()->GetSDKSessionID(), m_ulPlayHandle, pMagnifyPane->GetVideoWnd(), &IArectPercent);

		pMagnifyPane->SetIArectPercent(IArectPercent);
		// ���ñ���, TODO �жϿ��
		int iRate = GetMagnifyRateByIARect(IArect);
		if (0 != iRate)
		{
			if (NULL != pMagnifyPane->m_pZoomBar)
			{
				pMagnifyPane->m_pZoomBar->SetMagnifyRate(100*100/iRate);
			}
		}

		// ��������
		std::string strZoomName = ">";
		strZoomName += m_strZoomName;
		pMagnifyPane->SetZoomName(m_strZoomName);
		pMagnifyPane->m_pZoomTitleBar->SetZoomTitleName(strZoomName);
		pMagnifyPane->SetTranslucentStatus(TRUE);
		pMagnifyPane->SetAllBarStatus(1);
		pMagnifyPane->ResetBarPosition();// ��ʾ��������,����Ҫ����������Ĳ���״̬������
		pMagnifyPane->SetTranslucentStatus(FALSE);
		pMagnifyPane->Invalidate(TRUE);
		m_strZoomName = "";//�����

		//ResetBarPosition();
	}
	else if (GRAPHICS_STATE_EDITING == m_ulGraphicState)// �༭״̬
	{
		(void)Draw_ModifyFinishByGraphicPoint((DRAW_HANDLE)m_ulDrawHandle, m_ulCurrentGraphicGroup, point);
		m_bIsDrawing = false;

		CRect IArect;
		(void)GetIARectByGroup(m_ulCurrentGraphicGroup, IArect);

		CVideoPane* pMagnifyPane = (CVideoPane*)m_ulCurrentGraphicGroup;
		CHECK_POINTER_VOID(pMagnifyPane);
		// ����Ŵ��������
		int iRate = GetMagnifyRateByIARect(IArect);
		if (iRate < 12)
		{
			(void)ChangeSrcGraphicRect(800, m_ulCurrentGraphicGroup);
			if (NULL != pMagnifyPane->m_pZoomBar)
			{
				pMagnifyPane->m_pZoomBar->SetMagnifyRate(800);
			}
		}
		else
		{
			if (NULL != pMagnifyPane->m_pZoomBar)
			{
				// ���ñ���
				pMagnifyPane->m_pZoomBar->SetMagnifyRate(100*100/iRate);
			}
		}
		UpdataMagnifyWindow(m_ulCurrentGraphicGroup);
	}
}
// ��ȡ������ʾ�ĺ�׺����
int CVideoPane::GetFreeNameNum(ULONG ulGraphicGroup)
{
	for (int i=0; i < MAGNIFY_MAX_NUM; i++)
	{
		if (m_MagnifyNameCount[i] == 0)
		{
			m_MagnifyNameCount[i] = ulGraphicGroup;
			return ++i;
		}
	}
	return 0;
}

// ����paneָ���ͷŵ�ռ�õ�����
void CVideoPane::FreeNameNum(ULONG ulGraphicGroup)
{
	for (int i=0; i < MAGNIFY_MAX_NUM; i++)
	{
		if (m_MagnifyNameCount[i] == ulGraphicGroup)
		{
			m_MagnifyNameCount[i] = 0;
			return;
		}
	}
}

// �ָ��ϴιرյ��Ĵ���, �������֮ǰ��Ҫ������ͼ��
void CVideoPane::ResumeAllMagnifyRect()
{
	BP_RUN_LOG_INF("Resume All MagnifyRect","m_ResumeMagnifyNum=%d",m_ResumeMagnifyNum);
	for(ULONG i = 0; i < m_ResumeMagnifyNum; i++)
	{
		IA_GRAPHICS graphicsTemp;
		(void)CToolsHelp::Memcpy(&graphicsTemp, sizeof(IA_GRAPHICS), &m_szGraphics[i], sizeof(IA_GRAPHICS));
		BP_RUN_LOG_INF("resume magnify rect","ulGraphicGroup=%d",graphicsTemp.ulGraphicGroup);
		ResumeMagnifyRectByGraphic(graphicsTemp);
	}
}

// �ָ�һ��������ݽṹ��
void CVideoPane::ResumeMagnifyRectByGraphic(const IA_GRAPHICS& graphicsTemp)
{
	if (graphicsTemp.ulGraphicGroup != 0)
	{
		// ͼ������
		CRect IArect;
		GetIARectByGraphics(graphicsTemp, IArect);

		CPoint ptStartPoint;
		CPoint ptStopPoint;
		ptStartPoint.x = IArect.left;
		ptStartPoint.y = IArect.top;
		ptStopPoint.x = IArect.right;
		ptStopPoint.y = IArect.bottom;

		StartMagnifyByGraphicPoint(ptStartPoint);
		m_DrawMoveSuccessSum = 0;// ȥ���ƶ�״̬�ķ���
		FinishMagnifyByGraphicPoint(ptStopPoint);
	}
}

// �����пؼ�ˢ��Tip
void CVideoPane::UpdateAllTipText()
{
	if (m_pRealTimeTitleBar != NULL)
	{
		m_pRealTimeTitleBar->UpdateAllTipText();
	}
	if(m_pRealTimeToolBar!= NULL)
	{
		m_pRealTimeToolBar->UpdateAllTipText();
	}
	if (m_pAnalysisToolBar!= NULL)
	{
		m_pAnalysisToolBar->UpdateAllTipText();
	}
// 	if (GetTVWallStatusBar()!= nullptr)
// 	{
// 		GetTVWallStatusBar()->ShowWindow();
// 	}
	if (m_pZoomTitleBar != NULL)
	{
		m_pZoomTitleBar->UpdateAllTipText();
	}
	if (m_pZoomBar != NULL)
	{
		m_pZoomBar->UpdateAllTipText();
	}
}

void CVideoPane::SetWaterMarkStatus(ULONG ulStatus)
{
	if (m_pRealTimeTitleBar!=NULL)
	{
		m_pRealTimeTitleBar->SetWatermarkStatus(ulStatus);
	}
}

//BOOL CVideoPane::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
//{
//	// TODO: Add your specialized code here and/or call the base class
//	if (message == SW_HIDE)
//	{
//		HideTranslucenWnd();
//	}
//	return CDialog::OnWndMsg(message, wParam, lParam, pResult);
//}

void CVideoPane::ShowInstantBar()
{
	if (NULL != m_pPlayTimerBar)
	{
		//m_TimerBarRect
		m_pPlayTimerBar->InitTimeRange();
		m_pPlayTimerBar->MoveWindow(m_TimerBarRect);
		m_pPlayTimerBar->ShowWindow(SW_SHOW);
		
		ResetBarPosition();
	}
}

void CVideoPane::SetAlarmButtonStatus(unsigned long long ullstatus)
{
	m_ullWinAlarmFlag = ullstatus;
	SetToolbarEnableStatus(IVS_TOOLBAR_ALARM_WIN,m_ullWinAlarmFlag);//lint !e747
}

void CVideoPane::SetBookmarkButtonStatus(unsigned long long ullstatus)
{
	SetToolbarEnableStatus(IVS_TOOLBAR_BOOKMARK,ullstatus);//lint !e747
}

void CVideoPane::GetPlayBackCurrentTime(ST_TIME& PlayTime)
{   
    IVS_PLAYBACK_TIME stPlaybackTime;
    memset(&stPlaybackTime, 0, sizeof(IVS_PLAYBACK_TIME));

    IVS_INT32 iRet = IVS_SDK_GetPlayBackTime(GetOCXPlayerCtrl()->GetSDKSessionID(), m_ulInstanstPlayBackHandle, &stPlaybackTime);
    if (IVS_SUCCEED == iRet)
    {
        CString strTime = stPlaybackTime.cPlayBackTime;
        (void)TransStrToTime(strTime, &PlayTime);
    }
    
}//lint !e529

void CVideoPane::SetStartPlayTime()
{
    if (nullptr == m_pPlayTimerBar)
    {
        return;
    }
    m_pPlayTimerBar->SetStartPlayTime();//lint !e613
}

void CVideoPane::SetBarWindowsLong()const
{
	CHECK_POINTER_VOID(m_pVideoPaneMgr);
	ULONG ulHwndHandle = m_pVideoPaneMgr->GetParentHwnd();

	if (FULL_SCREEN == m_pVideoPaneMgr->GetFullScreenState())
	{
		ulHwndHandle = 0;
	}

	if (m_pRealTimeTitleBar!=NULL)
	{
		(void)::SetWindowLong(m_pRealTimeTitleBar->GetSafeHwnd(),GWL_HWNDPARENT,(LONG)(ulHwndHandle));
	}
	if(m_pRealTimeToolBar!=NULL)
	{
		(void)::SetWindowLong(m_pRealTimeToolBar->GetSafeHwnd(),GWL_HWNDPARENT,(LONG)(ulHwndHandle));
	}
	if (m_pAnalysisToolBar!=NULL)
	{
		(void)::SetWindowLong(m_pAnalysisToolBar->GetSafeHwnd(),GWL_HWNDPARENT,(LONG)(ulHwndHandle));
	}
	if (m_pTVWallStatusBar!=NULL)
	{
		(void)::SetWindowLong(m_pTVWallStatusBar->GetSafeHwnd(),GWL_HWNDPARENT,(LONG)(ulHwndHandle));
	}
	if (m_pTurnCruisePane!=NULL)
	{
		(void)::SetWindowLong(m_pTurnCruisePane->GetSafeHwnd(),GWL_HWNDPARENT,(LONG)(ulHwndHandle));
	}
	if (m_pDigtalDlg!=NULL)
	{
		(void)::SetWindowLong(m_pDigtalDlg->GetSafeHwnd(),GWL_HWNDPARENT,(LONG)(ulHwndHandle));
	}
	if (m_pZoomDirctDlg!=NULL)
	{
		(void)::SetWindowLong(m_pZoomDirctDlg->GetSafeHwnd(),GWL_HWNDPARENT,(LONG)(ulHwndHandle));
	}
	if (m_pZoomTitleBar!=NULL)
	{
		(void)::SetWindowLong(m_pZoomTitleBar->GetSafeHwnd(),GWL_HWNDPARENT,(LONG)(ulHwndHandle));
	}
	if (m_pZoomBar!=NULL)
	{
		(void)::SetWindowLong(m_pZoomBar->GetSafeHwnd(),GWL_HWNDPARENT,(LONG)(ulHwndHandle));
	}
	if (m_pPlayTimerBar!=NULL)
	{
		(void)::SetWindowLong(m_pPlayTimerBar->GetSafeHwnd(),GWL_HWNDPARENT,(LONG)(ulHwndHandle));
	}
}

void CVideoPane::SetSkin()
{
	if (m_pRealTimeTitleBar!=NULL)
	{
		Gdiplus::Image* pImage = nullptr;
		pImage = SkinMgr.GetImageTitleBarBKG();
		if (NULL == pImage)
		{
			return;
		}

		m_pRealTimeTitleBar->SetBackGroudPictrue(pImage);
		m_pRealTimeTitleBar->UpdateAllButtonSkin();
	}

	if (m_pRealTimeToolBar!=NULL)
	{
		Gdiplus::Image* pImage = nullptr;
		pImage = SkinMgr.GetImageBarBKG();
		if (NULL == pImage)
		{
			return;
		}

		m_pRealTimeToolBar->SetBackGroudPictrue(pImage);
		m_pRealTimeToolBar->OnInitChildrenWnds();
	}

	if (m_pAnalysisToolBar!=NULL)
	{
		Gdiplus::Image* pImage = nullptr;
		pImage = SkinMgr.GetImageBarBKG();
		if (NULL == pImage)
		{
			return;
		}

		m_pAnalysisToolBar->SetBackGroudPictrue(pImage);
		m_pAnalysisToolBar->OnInitChildrenWnds();
	}

	if (m_pTVWallStatusBar!=NULL)
	{
		m_pTVWallOfflineImage = SkinMgr.GetImageTVWallOfflineStatusBarBKG();
		m_pTVWallOnlineImage = SkinMgr.GetImageTVWallOnlineStatusBarBKG();

		SetTVWallStatusBarStatus(m_ulTVBarStatus);
	}

	if (m_pTurnCruisePane!=NULL)
	{
		std::string strBackGrondPath;
		strBackGrondPath.clear();
		(void)SkinMgr.TranslucentDlgGetBackGroundPngPath(strBackGrondPath);
		if(strBackGrondPath.empty())return;

		m_pTurnCruisePane->SetBarPictrue(strBackGrondPath.c_str());
	}

	if (m_pDigtalDlg!=NULL)
	{
		std::string strBackGrondPath;
		strBackGrondPath.clear();
		(void)SkinMgr.TranslucentDlgGetBackGroundPngPath(strBackGrondPath);
		if(strBackGrondPath.empty())return;

		m_pDigtalDlg->SetBarPictrue(strBackGrondPath.c_str());
	}
	if (m_pZoomDirctDlg!=NULL)
	{
		std::string strBackGrondPath;
		strBackGrondPath.clear();
		(void)SkinMgr.TranslucentDlgGetBackGroundPngPath(strBackGrondPath);
		if(strBackGrondPath.empty())return;

		m_pZoomDirctDlg->SetBarPictrue(strBackGrondPath.c_str());
	}
	if (m_pZoomTitleBar!=NULL)
	{
		std::string strBackGrondPath;
		strBackGrondPath.clear();
		(void)SkinMgr.ToolBarGetBackGroundPngPath(strBackGrondPath);
		if(strBackGrondPath.empty())return;

		m_pZoomTitleBar->SetBarPictrue(strBackGrondPath.c_str());
	}
	if (m_pZoomBar!=NULL)
	{
		std::string strBackGrondPath;
		strBackGrondPath.clear();
		(void)SkinMgr.ToolBarGetBackGroundPngPath(strBackGrondPath);
		if(strBackGrondPath.empty())return;

		m_pZoomBar->SetBarPictrue(strBackGrondPath.c_str());

		if (1 == SkinMgr.GetSkinType())
		{
			m_pZoomBar->SetRateTextColor(0xffffffff);
		}
		else
		{
			m_pZoomBar->SetRateTextColor(0xff000000);
		}
	}
	if (m_pPlayTimerBar!=NULL)
	{
		std::string strBackGrondPath;
		strBackGrondPath.clear();
		(void)SkinMgr.InstantReplayBackGroudPngPath(strBackGrondPath);
		if(strBackGrondPath.empty())return;

		m_pPlayTimerBar->SetBarPictrue(strBackGrondPath.c_str());
	}
}

void CVideoPane::SetTitleBarStatus(ULONG ulStatusType, ULONG ulShow)
{
	switch(ulStatusType)
	{
	case OCX_TITLEBAR_CRUISE:
		m_ulShowCruiseTrackStatus = ulShow;
		break;
	case OCX_TITLEBAR_WARNING:
		m_ulShowAlarmStatus = ulShow;
		break;
	case OCX_TITLEBAR_RECORD:
		m_ulShowRecordStatus = ulShow;
		break;
	case OCX_TITLEBAR_FRAME:
		m_ulShowSingleFrameStatus = ulShow;
		break;
	default:
		break;
	}
}

void CVideoPane::SetTitleBarInitStatus()
{
	if (m_pRealTimeTitleBar!=NULL)
	{
		m_pRealTimeTitleBar->SetCruiseTrackStatus(m_ulShowCruiseTrackStatus);
		m_pRealTimeTitleBar->SetAlarmStatus(m_ulShowAlarmStatus);
		m_pRealTimeTitleBar->SetRecordStatus(m_ulShowRecordStatus);
		m_pRealTimeTitleBar->SetSingleFrameStatus(m_ulShowSingleFrameStatus);
	}
}

/*************************************************************************/

LRESULT CVideoPane::OnAsyncEventMsg(WPARAM wParam,LPARAM lParam)
{
	CVideoPaneMgr *pPanelMgr = m_pVideoPaneMgr;
	if (!pPanelMgr)
	{
		return 0;
	}

	CIVS_OCXPlayerCtrl *pOcx = (CIVS_OCXPlayerCtrl *)(pPanelMgr->GetOcxCtrl());
	if (!pOcx)
	{
		return 0;
	}

	CXml xml;
	(void)COCXRealPlayXMLProcess::BuildAsyncRealPlayXml(xml, (long)m_ulPaneID, (long)wParam, (unsigned long)lParam);

    unsigned int len = 0;
	const TCHAR *pData = xml.GetXMLStream(len);
	pOcx->IVS_OCX_Windows_Event(/*IVS_EVENT_REALPLAY_AYSNC*/10030, (LPCTSTR)pData);

	BP_RUN_LOG_INF("OnAsyncEventMsg", "WM_ASYNC_EVENT_MSG %d %d", wParam, lParam);


	return 0;
}


LRESULT CVideoPane::OnAsyncStreamMsg(WPARAM wParam,LPARAM lParam)
{
	CVideoPaneMgr *pPanelMgr = m_pVideoPaneMgr;
	if (!pPanelMgr)
	{
		return 0;
	}
	CIVS_OCXPlayerCtrl *pOcx = (CIVS_OCXPlayerCtrl *)(pPanelMgr->GetOcxCtrl());

	if (!pOcx)
	{
		return 0;
	}




	BP_RUN_LOG_INF("OnAsyncStreamMsg", "POST WM_ASYNC_REALPLAY_MSG %d", wParam);


    switch (wParam)
	{
	case ASY_MSG_START_SUCCESS:
		if (m_pVideoPaneMgr->isAnalysisWndType())
		{
			SetAnalysisBarIsStartPlay(TRUE);
		}

		Set3DPTZState(PTZ_3D_CLOSE);
		SetServiceType(SERVICE_TYPE_REALVIDEO);
		SetTitleButtonPic(true);
		SetRealtimePlayPic(BUTTON_CLICKED);
		EnableALLButton(TRUE);
		SetPlayState(VIDEO_PLAY_STATE_REALTIME);
		m_Video.ShowWindow(SW_SHOW);
		
		break;

	case ASY_MSG_START_FAIL:
		ResetPane();
    	break;

	case ASY_MSG_STOP_SUCCESS:
		ResetPane();
		break;

	default:
		break;
	}
	return 0;
}//lint !e715  lparam ��ʹ��


void CVideoPane::StopRealStream()
{
	if (!m_ulPlayHandle)
	{
		return;
	}

	CVideoPaneMgr *pPanelMgr = m_pVideoPaneMgr;
	CIVS_OCXPlayerCtrl *pOcx = (CIVS_OCXPlayerCtrl *)(pPanelMgr->GetOcxCtrl());
	IVS_INT32 Sid = pOcx->GetSDKSessionID();

	CGlobalPlayMgr &playMgr = CGlobalPlayMgr::Instance();
	if (0 == playMgr.UnRegPlayHandle(m_ulPlayHandle, this))
	{
		IVS_SDK_StopRealPlay(Sid, m_ulPlayHandle);
	}
	else
	{
		IVS_SDK_DelPlayWnd(Sid, m_ulPlayHandle, GetVideoWnd());
	}
	m_ulPlayHandle = 0;
}

/*
 * StopRealStreamAsync
 */
int CVideoPane::StopRealStreamAsync()
{
	IVS_DEBUG_TRACE("StopRealStreamAsync: %x", this);

	bool bStopLater = false;

	if (m_hWorkThread)
	{
		m_CmdLock.Lock();
		if (THREAD_EXITING == m_ThreadState)
		{
			WaitForSingleObject(m_hWorkThread, INFINITE);
			CloseHandle(m_hWorkThread);
		    m_hWorkThread = NULL;
		}
		else
		{
			REALPLAY_CMD cmd;
			cmd.bStop = true;
			cmd.transid = 0xFFFFFFFF;
			
			m_CmdList.push_front(cmd);
			m_uCmdSeq++;
			bStopLater = true;
		}
		m_CmdLock.UnLock();
	}
	
	if (!bStopLater && m_ulPlayHandle)
	{
		StopRealStream();
    }
	ResetPane();

	return IVS_SUCCEED;
}

int CVideoPane::StopRealStreamSync()
{
	IVS_DEBUG_TRACE("StopRealStreamSync: %x", this);

	if (m_hWorkThread)
	{
		WaitForSingleObject(m_hWorkThread, INFINITE);
		CloseHandle(m_hWorkThread);
		m_hWorkThread = NULL;
	}

	if (m_ulPlayHandle)
	{
		StopRealStream();
	}
	ResetPane();
	return IVS_SUCCEED;
}



/*
 * StartRealSteamAsync
 * �첽����ʵ������
 */
LONG CVideoPane::StartRealSteamAsync(IVS_REALPLAY_PARAM &realplayParam, LPCTSTR pCameraID, 
	            BOOL bLayoutChange, ULONG ulStreamOpenMode, unsigned long transid)
{
	ASSERT(m_pVideoPaneMgr);
	CHECK_POINTER(m_pVideoPaneMgr, IVS_OPERATE_MEMORY_ERROR);

	CIVS_OCXPlayerCtrl *pCtrl = (CIVS_OCXPlayerCtrl *)(m_pVideoPaneMgr->GetOcxCtrl());
	CHECK_POINTER(pCtrl, IVS_OPERATE_MEMORY_ERROR);

	if (VIDEO_PLAY_STATE_STARTING == GetPlayState() ||
		VIDEO_PLAY_STATE_REALTIME == GetPlayState() ||
		VIDEO_PLAY_STATE_RESTARTING == GetPlayState())
	{
		if (VerifySameStream(pCameraID, realplayParam.uiStreamType))
		{
			BP_RUN_LOG_INF("Start Real Steam Async", "Same Stream"); 
			return IVS_SUCCEED;
		}
	}

	bool bSpawn = true;

	REALPLAY_CMD cmd;
	strncpy(cmd.CamID, pCameraID, sizeof(cmd.CamID) - 1);
	memcpy(&cmd.Param, &realplayParam, sizeof(cmd.Param));
	cmd.transid = transid;
	cmd.bStop   = false;

	if (m_hWorkThread)
	{
		BP_RUN_LOG_INF("Start Real Steam Async", "before lock"); 
    	m_CmdLock.Lock();
	    m_CmdList.push_front(cmd);
		if (THREAD_EXITING == m_ThreadState)
		{

			BP_RUN_LOG_INF("Start Real Steam Async", "wait thread"); 

			WaitForSingleObject(m_hWorkThread, INFINITE);
			CloseHandle(m_hWorkThread);
			m_hWorkThread = NULL;
		}
		else
		{
			m_uCmdSeq++;
			bSpawn = false;
		}
		m_CmdLock.UnLock();

		BP_RUN_LOG_INF("Start Real Steam Async", "after lock"); 
	}
	else
	{
		m_CmdList.push_front(cmd);
	}

	SetRealStreamInfo(pCameraID, realplayParam.uiStreamType);
	SetPlayState(VIDEO_PLAY_STATE_STARTING);

	if (bSpawn)
	{
		m_ThreadState = THREAD_RUNNING;
		m_hWorkThread = (HANDLE)_beginthreadex(NULL, 0, BegRealPlayProc, this, 0, NULL);
		if (NULL == m_hWorkThread)
		{
			BP_RUN_LOG_INF("Panel StartStream", "Create Thread Fail");
			return IVS_FAIL;
		}

		BP_RUN_LOG_INF("BegRealPlayProc", "Panel %d Exit Thread", GetPaneID());

	}
	return IVS_SUCCEED;
}//lint !e715   bLayoutChange �ݲ�ʹ��

/*
 * �첽����ʵ���̺߳���
 * 
 */
unsigned int __stdcall CVideoPane::BegRealPlayProc(LPVOID lpParameter)
{
	CVideoPane *pVideoPanel = (CVideoPane *)lpParameter;
	CHECK_POINTER(pVideoPanel, IVS_PARA_INVALID);

	CVideoPaneMgr *pPanelMgr = pVideoPanel->m_pVideoPaneMgr;
	CHECK_POINTER(pPanelMgr, IVS_PARA_INVALID);

	CIVS_OCXPlayerCtrl *pOcx = (CIVS_OCXPlayerCtrl *)pPanelMgr->GetOcxCtrl();
	CHECK_POINTER(pOcx, IVS_PARA_INVALID);

	IVS_INT32 Sid = pOcx->GetSDKSessionID();
	int result = IVS_SUCCEED;

	while (true) //lint !e716
	{
		IVS_REALPLAY_PARAM realplayParam;
		memset(&realplayParam, 0, sizeof(realplayParam));

		char CameraID[128] = {0};
		unsigned int  uCmdSeq = 0;
		unsigned long transid = 0;
		bool bCmdStop = false;
		int  MsgType  = ASY_MSG_NONE;

		pVideoPanel->m_CmdLock.Lock();

		ASSERT(!pVideoPanel->m_CmdList.empty());
		if (pVideoPanel->m_CmdList.empty())
		{
			pVideoPanel->m_CmdLock.UnLock();
			break;
		}

		REALPLAY_CMD &cmd = pVideoPanel->m_CmdList.front();
 		memcpy(&realplayParam, &cmd.Param, sizeof(realplayParam));
		strncpy(CameraID, cmd.CamID, sizeof(CameraID) - 1);
		bCmdStop  = cmd.bStop;
		transid   = cmd.transid;
		uCmdSeq   = pVideoPanel->m_uCmdSeq;
		pVideoPanel->m_CmdList.pop_front();
		
		/*
		 * �����߳�ִ�й����в����������ֹͣ����
		 * ����������Ҫ��CU�����¼�֪ͨ
    	 */
		while (!pVideoPanel->m_CmdList.empty())
		{
			REALPLAY_CMD &command = pVideoPanel->m_CmdList.front();
			if (!command.bStop)
			{
				::PostMessage(pVideoPanel->GetSafeHwnd(), 
					    WM_ASYNC_EVENT_MSG, 0, (long)command.transid);
			}
			pVideoPanel->m_CmdList.pop_front();
		}
		pVideoPanel->m_CmdLock.UnLock();

    	if (bCmdStop)
		{
			if (pVideoPanel->m_ulPlayHandle)
			{
				pVideoPanel->StopRealStream();
			}
			MsgType = ASY_MSG_STOP_SUCCESS;
    	}
		else
		{
			BP_DBG_LOG("BegRealPlayProc 0--------------");
			
			
			if (pVideoPanel->m_ulPlayHandle)
			{
				pVideoPanel->StopRealStream();
			}

			BP_DBG_LOG("BegRealPlayProc 1--------------");


			CGlobalPlayMgr &PlayMgr = CGlobalPlayMgr::Instance();
			ULONG ulHandle = PlayMgr.TryRealStreamShare(CameraID, 
				   realplayParam.uiStreamType, pVideoPanel);
			if (0 != ulHandle)
			{
				unsigned long PlayState = PlayMgr.GetPlayState(ulHandle);
				if (VIDEO_PLAY_STATE_RESTARTING == PlayState)
				{   
					if (0 == PlayMgr.UnRegPlayHandle(ulHandle, pVideoPanel))
					{
						(void)IVS_SDK_StopRealPlay(Sid, ulHandle);
					}
					ulHandle = 0;
				}
				else
				{
					BP_RUN_LOG_INF("IVS OCX Start Real Steam", "Share RealStream Handle with %08x", ulHandle);
					IVS_SDK_AddPlayWnd(Sid, ulHandle, pVideoPanel->GetVideoWnd());
					pVideoPanel->SetPlayHandle(ulHandle);

					MsgType = ASY_MSG_START_SUCCESS;
					::PostMessage(pVideoPanel->GetSafeHwnd(), WM_ASYNC_EVENT_MSG, IVS_SUCCEED, (long)transid);
				}
			}

			BP_DBG_LOG("BegRealPlayProc 2--------------");

			if (!ulHandle)
			{

				BP_DBG_LOG("BegRealPlayProc m_ulPlayHandle %08x", pVideoPanel->m_ulPlayHandle );

				result = IVS_SDK_StartRealPlay(Sid, &realplayParam, 
					CameraID, pVideoPanel->GetVideoWnd(), &pVideoPanel->m_ulPlayHandle);
				if (IVS_SUCCEED != result)
				{
					BP_RUN_LOG_INF("Start Real Steam", "error = [%d]", result);
					MsgType = ASY_MSG_START_FAIL;
				}
				else
				{
					(void)CGlobalPlayMgr::Instance().RegRealPlayHandle(pVideoPanel->m_ulPlayHandle, 
						pVideoPanel, CameraID, realplayParam.uiStreamType);
					(void)IVS_SDK_SetDisplayScale(Sid, pVideoPanel->m_ulPlayHandle, 
						(IVS_INT32)pPanelMgr->GetDispalyScale());
					MsgType = ASY_MSG_START_SUCCESS;

					pVideoPanel->SetCameraID(CameraID);
					pVideoPanel->SetRealStreamInfo(CameraID, realplayParam.uiStreamType);
				}
				
				BP_DBG_LOG("BegRealPlayProc 3--------------");

				::PostMessage(pVideoPanel->GetSafeHwnd(), WM_ASYNC_EVENT_MSG, (unsigned int)result, (long)transid);
			}
		}

		BP_DBG_LOG("BegRealPlayProc, before lock");
		pVideoPanel->m_CmdLock.Lock();
		if (pVideoPanel->m_uCmdSeq == uCmdSeq)
		{
			pVideoPanel->m_ThreadState = THREAD_EXITING;
		}
		pVideoPanel->m_CmdLock.UnLock();

		BP_DBG_LOG("BegRealPlayProc, after lock  pVideoPanel->m_ThreadState = %d", pVideoPanel->m_ThreadState);


		if (THREAD_EXITING == pVideoPanel->m_ThreadState)
		{
			if (MsgType != ASY_MSG_NONE)
			{
				::PostMessage(pVideoPanel->GetSafeHwnd(), WM_ASYNC_REALPLAY_MSG, (unsigned int)MsgType, 0);

				BP_RUN_LOG_INF("BegRealPlayProc", "POST WM_ASYNC_REALPLAY_MSG %d", MsgType);
			}
		    break;
		}
    }

	BP_DBG_LOG("BegRealPlayProc Panel %d Exit Thread", pVideoPanel->GetPaneID());

	return 0;
}







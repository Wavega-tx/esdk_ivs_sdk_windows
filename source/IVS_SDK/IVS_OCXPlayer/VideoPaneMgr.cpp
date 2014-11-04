#include "stdafx.h"
#include "IVS_OCXPlayerCtrl.h"
#include "VideoPaneMgr.h"
#include "IVS_OCXPlayerCtrl.h"
#include "VideoPane.h"
#include "TranVideoTitleBar.h"
#include "FunctionObject.h"
#include "ParentWnd.h"
#include "OCXTVWallXMLProcess.h"
#include "I18N.h"
#include "OCXTVWallMgr.h"


CVideoPaneMgr* g_CVideoPaneMgr = NULL;
HHOOK g_LowLevelKeyboardHook = NULL;
LRESULT CALLBACK LowLevelKeyboardProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	KBDLLHOOKSTRUCT *pkbh = (KBDLLHOOKSTRUCT *)lParam;
	if (nCode == HC_ACTION)
	{
		//BOOL bCtrlKeyDown = GetAsyncKeyState(VK_CONTROL)>>((sizeof(SHORT)*8) - 1);
		if ((pkbh->vkCode == VK_TAB && pkbh->flags & LLKHF_ALTDOWN)||
			(pkbh->vkCode == VK_LWIN || pkbh->vkCode == VK_RWIN)||
			(pkbh->vkCode == VK_RETURN))
		{
			return 1;
		}

		if (pkbh->vkCode == VK_RIGHT || pkbh->vkCode == VK_DOWN)
		{
			if (g_CVideoPaneMgr!=NULL)
			{
				g_CVideoPaneMgr->GetParentWnd()->SetFocus();//lint !e613
			}
		}
	}

	return CallNextHookEx(g_LowLevelKeyboardHook,nCode,wParam,lParam);
}

CVideoPaneMgr::CVideoPaneMgr(void)
    : m_pParentWnd(nullptr)
    , m_pOcxCtrl(nullptr)
    , m_ulWndType(WND_TYPE_REALTIME)
	, m_windLayOut(static_cast<IVS_WIND_LAYOUT>(0))
    , m_ulShowWndNum(0)
    , m_ulIsMagnify(MINIFY)
    , m_ulActivePaneID(0)
    , m_ulOperatePaneID(0)
    , m_ulClientType(CLIENT_PC)
    , m_ulIsFullScreen(NOT_FULL_SCREEN)
    , m_IsShowTitleBar(TITLEBAR_SHOW)
	, m_isShowToolBar(false)
	, m_isChangeToolBar(false)
    , m_uToolbarBtnNum(0)
    , m_uAnalysisBtnNum(0)
    , m_ulDispalyAspectRatio(DISPLAY_ASPECT_RATIO_ORIGINAL)
    , m_lAutoChangeLayout( AUTO_CHANGE_LAYOUT)
	, m_ulDisplayScale(1)
    , m_VideoPaneBKGColor(0)
    , m_ActiveEdgeColor  (0)
    , m_NotActiveEdgeColor (0)
    , m_TitleBarBKGColor (0)
    , m_BottomBarBKGColor(0)
    , m_RectangleBKGColor(0)
    , m_TextColor(0)
    , m_ulShadeStartRGB  (0)
    , m_ulActiveEdgeRed  (0)
    , m_ulActiveEdgeGreen(0)
    , m_ulActiveEdgeBlue (0)
	, m_ullToolBarBtnVisableStatus(0)
	, m_pVideoPane_1_1(NULL) //lint !e1729
	, ulEnableExchangePaneStatus(1)
	, m_ulParentHanlde(0)
	, m_ulOldParentHanlde(0)
	, m_ulSkinType(1)
{
    // ��������ֵ
    memset(&m_struOldWndpl, 0, sizeof(m_struOldWndpl));
    m_videoPaneMap.clear();
    memset(m_strToolbarStatus, 0, 33);
	m_MagnifyLayOut = WIND_LAYOUT_4_1;
	m_TVWallWndType = WND_TYPE_REALTIME;
	m_ulTVWallID = 0;
	HideShellTrayWnd(TRUE);
}

CVideoPaneMgr::~CVideoPaneMgr(void)
{
    try
    {
        // ������ֵԭ����ֵ
        m_ulIsFullScreen   = NOT_FULL_SCREEN;
        m_IsShowTitleBar = TITLEBAR_SHOW;
        m_ulWndType        = WND_TYPE_NORMAL;
        m_ulClientType     = CLIENT_PC;
        m_ulIsMagnify      = MINIFY;
        m_ulDispalyAspectRatio = DISPLAY_ASPECT_RATIO_ORIGINAL;
        m_ulActivePaneID   = 0;
        m_ulOperatePaneID  = 0;
        memset(&m_struOldWndpl, 0, sizeof(m_struOldWndpl));
        memset(m_strToolbarStatus, 0, 33);

        //����
        CAutoLock __Locker(m_csVideoPaneLock);

        //ULONG ulRet = 0;

        //ֹͣ�����Խ�
        //(void)StopVoiceTalkback();

       // map����Ĵ���,�ж��Ƿ񲥷ţ��ǵĻ�Ҫ�Ȱ���ֹͣ
        for (MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin(); iter != m_videoPaneMap.end();)
        {
            CVideoPane *pVideoPane = iter->second; //lint !e64
            if (pVideoPane == nullptr)
            {
                continue;
            }
            CIVS_OCXPlayerCtrl *pOcxCtrl = dynamic_cast<CIVS_OCXPlayerCtrl *>(m_pOcxCtrl);

            //ֹͣ����(ʵ��/�ط�)
            if (NULL != pOcxCtrl)
            {
                 (void)pOcxCtrl->StopRealPlayByPane(pVideoPane);
#if 0				
				//�����ʵ��(���)������������
                if (pVideoPane->GetPlayState() == VIDEO_PLAY_STATE_REALTIME || pVideoPane->GetPlayState() == VIDEO_PLAY_STATE_RESTARTING)
                {
                    ulRet = pOcxCtrl->StopRealPlayByPane(pVideoPane);
                    if (ulRet != IVS_SUCCEED)
                    {
                        BP_RUN_LOG_ERR(static_cast<int>(ulRet),"IVS_OCX::"," VideoPane Manager.stopRealPlayByPane failed");
                    }
                    //����ǻط�
                }else if (pVideoPane->GetPlayState() == VIDEO_PLAY_STATE_VOD)
                {
                    //pOcxCtrl->IVS_OCX_StopVodPlay(szWndID);
                }
#endif 
            }
            pVideoPane->DeleteBar();
            delete pVideoPane;
            pVideoPane = nullptr;
            m_videoPaneMap.erase(iter++);
        }

        m_videoPaneMap.clear();
        m_pOcxCtrl = nullptr;

        IVS_DELETE(m_pParentWnd);   //lint !e1551 IVSCommon.h ��������
    }
    catch (...)
    {
    }
}//lint !e1740��OCX�ĸ������ͷ�

void CVideoPaneMgr::SetColor(ENUMDEF::COLOR_MODE ulColor)
{
    //������ɫ����ɫ�ͺ�ɫ
    if (COLOR_GRAY == ulColor)
    {
        m_VideoPaneBKGColor = RGB(230,230,230);
        m_ActiveEdgeColor = RGB(99,153,217);
        m_NotActiveEdgeColor = RGB(220,220,220);
        m_TitleBarBKGColor = RGB(190,190,190);
        m_BottomBarBKGColor = RGB(190,190,190);
        m_RectangleBKGColor = RGB(77,82,86);
        m_ulShadeStartRGB = 220;
        m_TextColor = RGB(0,0,0);
        m_ulActiveEdgeRed = 48;
        m_ulActiveEdgeGreen = 177;
        m_ulActiveEdgeBlue = 243;
    } else {
        m_VideoPaneBKGColor = RGB(70,70,70);
        m_ActiveEdgeColor = RGB(99,153,217);
        m_NotActiveEdgeColor = RGB(70,70,70);
        m_TitleBarBKGColor = RGB(46, 46, 46);
        m_BottomBarBKGColor = RGB(46, 46, 46);
        m_RectangleBKGColor = RGB(77, 82, 86);
        m_ulShadeStartRGB = 80;
        m_TextColor = RGB(255,255,255);
        m_ulActiveEdgeRed = 94;
        m_ulActiveEdgeGreen = 145;
        m_ulActiveEdgeBlue = 190;
    }
}

// Qualifier: �ؼ������ʼ��
LONG CVideoPaneMgr::Init( void )
{
	//Ƥ����������ʼ��
	if (IVS_SUCCEED != SkinMgr.Init())
	{
        BP_RUN_LOG_ERR(IVS_OCX_RET_SKIN_INIT_ERROR,_T("IVS_OCX::"),_T(" VideoPane Manager init.Failed"));
        return IVS_OCX_RET_SKIN_INIT_ERROR;
	}

    if (IVS_SUCCEED != SkinMgr.SetSkin())
    {
        BP_RUN_LOG_ERR(IVS_OCX_RET_SKIN_INIT_ERROR,_T("IVS_OCX::"),_T(" VideoPane Manager init.Failed"));
        return IVS_OCX_RET_SKIN_INIT_ERROR;
    }

	CIVS_OCXPlayerCtrl *pOcxCtrl = dynamic_cast<CIVS_OCXPlayerCtrl *>(m_pOcxCtrl);
	CHECK_POINTER(pOcxCtrl,IVS_FAIL);
	(void)SkinMgr.SetSkinType(static_cast<IVS_INT32>(pOcxCtrl->g_ulSkinType));//lint !e1705
    //����������
    m_pParentWnd = IVS_NEW((CParentWnd*&)m_pParentWnd);
    CHECK_POINTER(m_pParentWnd,IVS_FAIL);

    CParentWnd *pParentWnd = dynamic_cast<CParentWnd *>(m_pParentWnd);
    CHECK_POINTER(pParentWnd,IVS_FAIL);
    pParentWnd->SetVideoPaneMgr(this);
	(void)pParentWnd->Create(IDD_DLG_PARENT, m_pOcxCtrl);
    pParentWnd->ShowWindow(SW_SHOW);

	CRect rect;
	CHECK_POINTER(m_pOcxCtrl,IVS_FAIL);
	m_pOcxCtrl->GetClientRect(rect);
	m_pParentWnd->MoveWindow(rect);
	//(void)MoveOCXToRect(rect);

    CAutoLock __Locker(m_csVideoPaneLock);
    m_videoPaneMap.clear();
    return IVS_SUCCEED;
}

void CVideoPaneMgr::SetSkin(ULONG val)
{
	CString str;
	m_ulSkinType = val;
	if (IVS_SUCCEED != SkinMgr.SetSkinType((IVS_INT32)val))
	{
		BP_RUN_LOG_ERR(IVS_OCX_RET_SKIN_INIT_ERROR,_T("IVS_OCX::"),_T(" VideoPane Manager init.Failed"));
		return;
	}

	(void)for_each(m_videoPaneMap.begin(),
		m_videoPaneMap.end(),
		FunctionObjecVideoPaneSetSkin());
}

// Qualifier: �Ŵ����С������Ϊ�Ƿ����ڲ���
void CVideoPaneMgr::ChangeSize( ULONG ulIsPlaying )
{
    // ����Ϊ1ʱ�����ò���
    if (WIND_LAYOUT_1_1 == m_ulShowWndNum)
    {
        return;
    }
	CVideoPane* pVideoPane = NULL;
    if (MINIFY == m_ulIsMagnify)
    {
        //�ж��Ƿ����ڲ��ţ������ܷŴ󣬵��ǿ�����С���Ŵ�״̬��ֹͣ��Ƶ���ţ����Խ�����С��
        if (VIDEO_PLAY_STATE_NOT_PLAYING == ulIsPlaying)
        {
            BP_RUN_LOG_ERR(IVS_FAIL,"IVS_OCX::"," VideoPane Manager ChangeSize failed . Magnify Failed Pane is not playing video.");
            return;
        }

        CHECK_POINTER_VOID(m_pParentWnd);

        // �Ѵ����С�Ŵ�Ϊ�����ڴ�С
        CRect rect;
        m_pParentWnd->GetClientRect(rect);
        Magnify(rect);
		// ���þֲ��Ŵ�ť
		pVideoPane = GetWndPointerByID(m_ulActivePaneID);
		CHECK_POINTER_VOID(pVideoPane);

		if (NULL != pVideoPane->GetVideoRealtimeBar())
		{
			pVideoPane->GetVideoRealtimeBar()->SetToolbarEnableStatus(IVS_TOOLBAR_ZOOM,0);//lint !e747
		}

    } else {
        Minify();
		// ���þֲ��Ŵ�ť
		pVideoPane = GetWndPointerByID(m_ulActivePaneID);
		CHECK_POINTER_VOID(pVideoPane);
		if (NULL != pVideoPane->GetVideoRealtimeBar())
		{
			pVideoPane->GetVideoRealtimeBar()->SetToolbarEnableStatus(IVS_TOOLBAR_ZOOM,1);//lint !e747
		}
		// �鿴���������Ƿ��������˵�
		LONG lWndID = (LONG)m_ulActivePaneID;
		DoButtonFilter(lWndID);
    }
}

// ��С����
void CVideoPaneMgr::Minify(void)
{
    CHECK_POINTER_VOID(m_pParentWnd);
    m_ulIsMagnify = MAGNIFY;

    // ���²��ִ��񣬴�����Ϊԭ���Ĳ�����
    CRect rect;
    if(NOT_FULL_SCREEN == GetFullScreenState())
    {
        m_pParentWnd->GetClientRect(rect);
    } else {
        CHECK_POINTER_VOID(m_pOcxCtrl);
        HMONITOR hMonitor = MonitorFromWindow(m_pOcxCtrl->GetSafeHwnd(),MONITOR_DEFAULTTONEAREST);
        MONITORINFO info;
        info.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(hMonitor,&info);

        rect.top = info.rcMonitor.top;
        rect.left = info.rcMonitor.left;
        rect.bottom = info.rcMonitor.bottom;
        rect.right = info.rcMonitor.right;
    }

	//m_pParentWnd->Invalidate(TRUE);
	//m_pParentWnd->UpdateWindow();
    if (IVS_SUCCEED != SetWndLayout(rect, m_windLayOut))
    {
        BP_RUN_LOG_ERR(IVS_FAIL,"IVS_OCX::"," VideoPane Manager  minify  Failed");
        return;
    }

    //�ϱ�����������Ϣ,���ڶ������ϴ�����ID
   dynamic_cast<CIVS_OCXPlayerCtrl*>(m_pOcxCtrl)->PostMagnifyOrMinifyEventMsg(0, m_ulActivePaneID, MINIFY);
}

// Qualifier: �Ŵ󴰸�
void CVideoPaneMgr::Magnify(CRect& rect)
{
    m_ulIsMagnify = MAGNIFY;

    //��������Ϊ����ocxctrl onSize��ʱ�򣬻�Ѹ����ڴ�С��С
    CHECK_POINTER_VOID(m_pParentWnd);
    m_pParentWnd->Invalidate(TRUE);
    m_pParentWnd->MoveWindow(rect,FALSE);

    HideVideoPane();
    m_ulOperatePaneID = m_ulActivePaneID;
    SetVideoPanePosition(rect);

    //�ϱ�����������Ϣ,���ڶ������ϴ�����ID
   dynamic_cast<CIVS_OCXPlayerCtrl*>(m_pOcxCtrl)->PostMagnifyOrMinifyEventMsg(0, m_ulActivePaneID, MAGNIFY);
}

// Qualifier: ����Ƿ���Խ�������
//            �������ҵ�������Ƿ���Խ�����
//            ����һ����3D��λ����̨���ƣ����ӷŴ����ܽ���
// ע��ͬ���޸�CVideoPane::BeginPaneDrag�����е����״̬
ULONG CVideoPaneMgr::CheckIfCanExchange(const CVideoPane * pFirstPane) const
{
    CHECK_EQUAL(PTZ_3D_OPEN,pFirstPane->Get3DPTZState(),IVS_FAIL);
    CHECK_EQUAL (BOTTOMBAR_PTZ_OPEN , pFirstPane->GetRealtimePtzState(),IVS_FAIL);
    CHECK_EQUAL (MAGNIFY_OPEN , pFirstPane->GetRealtimeMagnifyState(),IVS_FAIL);
    return IVS_SUCCEED;
}

// Qualifier: ɾ������SoundCtrl��Talkback�Ի���ɾ��Replay�����Ի���
void CVideoPaneMgr::DeleteSoundCtrl(void)
{
    if (isAnalysisWndType()){return;}
    CAutoLock __Locker(m_csVideoPaneLock);

    (void)for_each(m_videoPaneMap.begin(),m_videoPaneMap.end(),
       FunctionObjectDeleteSoundTalkBackCtrlForOther(0));
}

void CVideoPaneMgr::DeleteSoundCtrlForOther(void)
{
    if (isAnalysisWndType()){return;}

    CAutoLock __Locker(m_csVideoPaneLock);
	(void)for_each(m_videoPaneMap.begin(),m_videoPaneMap.end(),
        FunctionObjectDeleteSoundTalkBackCtrlForOther(GetActivePaneID()));
}

//  �������񣬲���Ϊ��׽���ɿ��������Ĵ���
void CVideoPaneMgr::ExchangePane(ULONG ulMouseUpPaneID)
{
    CHECK_POINTER_VOID(m_pParentWnd);
	if (!ulEnableExchangePaneStatus)
	{
		BP_RUN_LOG_INF("ExchangePane","ExchangePane is Disable");
		return;
	}

    // �ж��з�ס�������϶��Ķ���(���ݰ������ͷ�ʱ������ID�Ƿ�ͬ���ж�)�����򽻻����������
    CHECK_EQUAL(m_ulActivePaneID,ulMouseUpPaneID,);
    // �����������ҳ���
    CVideoPane * firstPane = nullptr;
    CVideoPane * secondPane= nullptr;

    // ���Ҵ���ʱ���������������ҵ��ſ��Խ���������������������󣬲����ſ�ʼ
    MAP_VIDEOPANE_ITER firstIter = m_videoPaneMap.find(m_ulActivePaneID);
    CHECK_EQUAL(firstIter,m_videoPaneMap.end(),);

    // �ҵ���һ��,��ʼ�ҵڶ���
    MAP_VIDEOPANE_ITER secondIter = m_videoPaneMap.find(ulMouseUpPaneID);
    CHECK_EQUAL(secondIter,m_videoPaneMap.end(),);

    // ��������¼������
    firstPane  = firstIter->second;  //lint !e64
    secondPane = secondIter->second; //lint !e64

    // �������ҵ�������Ƿ���Խ���������һ����3D��λ����̨���ƣ����ӷŴ����ܽ���
	// ֮��鱻���������ǲ����⼸�֣������������⼸�ֿ��� mod by z00193167
    if(IVS_SUCCEED != CheckIfCanExchange(firstPane))
    {
        return;
    }
	
//     if(IVS_SUCCEED != CheckIfCanExchange(secondPane))
//     {
//         return;
//     }
	
	//����ǵ���ǽ������Ŀ�괰�����϶�
	if(0 != m_ulTVWallID && 1 != secondPane->GetWndDragStatus())
	{
		return;
	}
    // �޸����������: 1.����ID   2.����λ�ô�С��rect��
    //			       3.������(һ����������������������ʵ�����طţ�ֻ�ܷ�����)
    //                 4.�����������Ա  5.����Ǹ澯�������ø澯���񵽱������ݿ�
    //                 6.��������Ļ����ID

    firstPane->SetPaneID(ulMouseUpPaneID);
    secondPane->SetPaneID(m_ulActivePaneID);

    CRect oldFirstRect, oldSecondRect;

    firstPane->GetPaneRectFull(&oldFirstRect);
    secondPane->GetPaneRectFull(&oldSecondRect);

    m_ulOperatePaneID = m_ulActivePaneID;
    SetVideoPanePosition(oldSecondRect);
    m_ulOperatePaneID = ulMouseUpPaneID;
    SetVideoPanePosition(oldFirstRect);

    //�澯���񱣴浽���ݿ�,����ʧ�ܣ�����־��������
    if (firstPane->GetRealtimeAlarmState() != secondPane->GetRealtimeAlarmState())
    {
        if (BOTTOMBAR_ALARM_OPEN == firstPane->GetRealtimeAlarmState())
        {
            //ULONG ulRet = 0;//IVS_SDK_SetAlarmWnd(ulMouseUpPaneID);
            //if (IVS_SUCCEED != ulRet)
            //{
            //    BP_RUN_LOG_ERR("","Set alarm window error,result code:%lu",ulRet);
            //}
            //ulRet = 0;//IVS_SDK_DeleteAlarmWnd(m_ulActivePaneID);
            //if (IVS_SUCCEED != ulRet)
            //{
            //    BP_RUN_LOG_ERR("","Set alarm window error,result code:%lu",ulRet);
            //}
        }
        else
        {
            //ULONG ulRet = 0;//IVS_SDK_SetAlarmWnd(m_ulActivePaneID);
            //if (IVS_SUCCEED != ulRet)
            //{
            //    BP_RUN_LOG_ERR("","Set alarm window error,result code:%lu",ulRet);
            //}
            //ulRet = 0;//IVS_SDK_DeleteAlarmWnd(ulMouseUpPaneID);
            //if (IVS_SUCCEED != ulRet)
            //{
            //    BP_RUN_LOG_ERR("","Set alarm window error,result code:%lu",ulRet);
            //}
        }
    }

    // ��map����������¼
    (void)m_videoPaneMap.erase(firstIter);
    (void)m_videoPaneMap.erase(secondIter);
    //���²���������¼
    (void)m_videoPaneMap.insert(std::make_pair(ulMouseUpPaneID, firstPane));
    (void)m_videoPaneMap.insert(std::make_pair(m_ulActivePaneID, secondPane));

    //�ϱ�����������Ϣ,���ڶ������ϴ�����ID
	CIVS_OCXPlayerCtrl* pCtrl = dynamic_cast<CIVS_OCXPlayerCtrl*>(m_pOcxCtrl);
	CHECK_POINTER_VOID(pCtrl);
    pCtrl->PostExchangeWndEventMsg(0, m_ulActivePaneID, ulMouseUpPaneID);

    //�޸�ͬ���طŴ���ID
    //pCtrl->UpdateSynWndID(m_ulActivePaneID,ulMouseUpPaneID);

    SetActivePaneID(ulMouseUpPaneID);
}

// ��ȡ�����ָ��
CVideoPane *CVideoPaneMgr::GetActiveWndPointer(void)
{
    CVideoPane *pVideoPane = nullptr;
    MAP_VIDEOPANE_ITER iter = m_videoPaneMap.find(m_ulActivePaneID);
    if (iter != m_videoPaneMap.end())
    {
        pVideoPane =  iter->second; //lint !e64
    }
    return pVideoPane;
}

// ��ȡ���д���
ULONG CVideoPaneMgr::GetAllVideoPane(MAP_VIDEOPANE & videoPaneMap) const
{
    videoPaneMap.clear();
    videoPaneMap = m_videoPaneMap;//lint !e1036
    return static_cast<ULONG>(videoPaneMap.size());
}

LONG CVideoPaneMgr::SetClientType(IVS_CLIENT_TYPE ulClientType )
{
    m_ulClientType = ulClientType;

    //����
    CAutoLock __Locker(m_csVideoPaneLock);
    LONG _WndType = m_ulWndType;

    (void)std::for_each(m_videoPaneMap.begin(),m_videoPaneMap.end(),
        [ulClientType,_WndType](const std::pair<LONG, CVideoPane*> &iter){ //lint !e24 !e10 !e26 !e78 !e505 !e48 !e155 !e55
            iter.second->SetClientType(ulClientType);
            if ((WND_TYPE_REALTIME == _WndType)
                && (nullptr != iter.second->GetVideoRealtimeBar()))
            {
                iter.second->GetVideoRealtimeBar()->Invalidate(TRUE);
            }
    }); //lint !e1024 !e119 !e530
    return IVS_SUCCEED;
}

//��ʼ�� ʵ����¼�񹤾���
ULONG CVideoPaneMgr::SetToolbar( ULONG ulToolBar )
{
    std::bitset<TOOLBARBTNNUM> bitSetToolBar(static_cast<ULONGLONG>(ulToolBar));
    m_uToolbarBtnNum = bitSetToolBar.count();

    SetToolBarVisableStatus(static_cast<ULONGLONG>(ulToolBar));
	MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin() ;
	for(;iter != m_videoPaneMap.end();iter++)
	{
		CVideoPane *pVideoPane = iter->second; //lint !e64
		if (pVideoPane != nullptr)
		{
			pVideoPane->SetToolBarBtnVisableStatus(static_cast<ULONGLONG>(ulToolBar));
		}
	}

	RefreshAllWindow();
    return IVS_SUCCEED;
}

//�������ܷ����Ĺ�����
ULONG CVideoPaneMgr::SetIAToolbar( ULONG ulToolBar )
{
    std::bitset<TOOLBARBTNNUM> bitSetToolBar(static_cast<ULONGLONG>(ulToolBar));
    m_uAnalysisBtnNum = bitSetToolBar.count();

    SetToolBarVisableStatus(static_cast<ULONGLONG>(ulToolBar));
	MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin() ;
	for(;iter != m_videoPaneMap.end();iter++)
	{
		CVideoPane *pVideoPane = iter->second; //lint !e64
		if (pVideoPane != nullptr)
		{
			pVideoPane->SetToolBarBtnVisableStatus(static_cast<ULONGLONG>(ulToolBar));
		}
	}
	RefreshAllWindow();
    return IVS_SUCCEED;
}

// ���ݾ����ȡ����ID
ULONG CVideoPaneMgr::GetWindIDByHWND( const HWND &ulWnd )
{
    CAutoLock __Locker(m_csVideoPaneLock);
    ULONG ulWndID = 0;
    MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin() ;

    for(;iter != m_videoPaneMap.end();iter++)
    {
        CVideoPane *pVideoPane = iter->second; //lint !e64
        if( ulWnd == pVideoPane->GetVideoWnd() )
        {
            break;
        }
    }

    if (iter != m_videoPaneMap.end())
    {
        ulWndID =  iter->first;
    }
    return ulWndID;
}

// ��ȡ�ĸ����ڿ����˾ֲ��Ŵ�
bool CVideoPaneMgr::GetMagnifyOpenWnd(ULONG& ulWndID)
{
	CAutoLock __Locker(m_csVideoPaneLock);

	MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin() ;

	for(;iter != m_videoPaneMap.end();iter++)
	{
		CVideoPane *pVideoPane = iter->second; //lint !e64
		if( MAGNIFY_OPEN == pVideoPane->GetRealtimeMagnifyState() )
		{
			ulWndID = pVideoPane->GetPaneID();
			return true;
		}
	}

	return false;
}

// ������������ȡ����ID
ULONG CVideoPaneMgr::GetWndIDByMouse( void )
{
	CPoint point;
	GetCursorPos(&point);

	CRect rect;
	CHECK_POINTER(m_pParentWnd,IVS_OCX_RET_GET_INFO_ERROR);
	m_pParentWnd->GetWindowRect(rect);

	//��겻�ڿؼ���
	if (!rect.PtInRect(point))
	{
		BP_RUN_LOG_ERR(IVS_OCX_RET_GET_INFO_ERROR,_T("IVS_OCX::"),_T(" VideoPaneManager get windows ID by mouse.Failed"));
		return IVS_OCX_RET_POINT_OUT_CLIENT;
	}

	if (MAGNIFY == m_ulIsMagnify)
	{
		//����ǷŴ�ͷ��ص�ǰ����ID
		return m_ulActivePaneID;
	}

    MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin();

    for (ULONG i = 0; i < m_ulShowWndNum; i++)
    {
        if (iter == m_videoPaneMap.end())
        {
            BP_RUN_LOG_ERR(IVS_OCX_RET_GET_INFO_ERROR,_T("IVS_OCX::"),_T(" VideoPaneManager get windows ID by mouse.Failed"));
            return IVS_OCX_RET_GET_INFO_ERROR;
        }
        CVideoPane *pVideoPane = iter->second; //lint !e64
        CHECK_POINTER(pVideoPane,IVS_OCX_RET_GET_INFO_ERROR);
        pVideoPane->GetWindowRect(&rect);
        if (rect.PtInRect(point))
        {
            return pVideoPane->GetPaneID();
        }

        ++iter;
    }
    BP_RUN_LOG_ERR(IVS_OCX_RET_GET_INFO_ERROR,_T("IVS_OCX::"),_T("VideoPaneManager get windows ID by mouse.Failed"));
    return IVS_OCX_RET_GET_INFO_ERROR;
}

// ��ȡ���еĴ���(�Ǹ澯����)
ULONG CVideoPaneMgr::GetWndIDFree( void )
{
    //�����ǰҳ��ĵ�һ�������
    MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin();
    for (ULONG i = 1; i <= MAX_PANE_NUMBER; i++)
    {
        CHECK_EQUAL(iter,m_videoPaneMap.end(),IVS_OCX_RET_GET_INFO_ERROR);

        CVideoPane *pVideoPane = iter->second; //lint !e64
        
        if (m_ulShowWndNum < i)
        {
            BP_RUN_LOG_ERR(IVS_OCX_RET_GET_INFO_ERROR,_T("IVS_OCX::"),"Get wnd id free fail, all windows are busy.");
            return IVS_OCX_RET_GET_INFO_ERROR;
        }
        CHECK_EQUAL( m_lAutoChangeLayout, NOT_AUTO_CHANGE_LAYOUT, IVS_OCX_RET_GET_INFO_ERROR );


        //�жϵ�ǰ�����Ƿ����(�Ȳ���ʵ����Ҳ���ڻط�),��ռ�óɾֲ��Ŵ�Ĵ���Ҳ�����ǿ��д���(TODO Ϊɶ��)
        if (pVideoPane->GetPlayState() == VIDEO_PLAY_STATE_NOT_PLAYING /*|| BOTTOMBAR_MAGNIFY_OPEN == pVideoPane->GetDstMagnifyState()*/)
        {
            //���ж��Ƿ�Ϊ�澯����
            ULONG ulType = pVideoPane->GetRealtimeAlarmState(); //lint !e732 ������
            if (BOTTOMBAR_ALARM_OPEN == ulType)
            {
                ++iter;
                continue;
            }
            return pVideoPane->GetPaneID(); //lint !e732 ������
        }

        ++iter;
    }
    BP_RUN_LOG_ERR(IVS_OCX_RET_GET_INFO_ERROR,_T("IVS_OCX::"),_T("VideoPaneManager get  free windows ID.Failed"));
    return IVS_OCX_RET_GET_INFO_ERROR;
}

// ��ȡ�Ŵ�Ĳ��Ŵ���(ֻҪ���Ǹ澯����,�Ҳ����Լ�������ʹ��)
ULONG CVideoPaneMgr::GetMagnifyFreeWnd(ULONG ulPaneID)
{
	//�����ǰҳ��ĵ�һ�������
	MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin();
	for (ULONG i = 1; i <= MAX_PANE_NUMBER; i++)
	{
		CHECK_EQUAL(iter,m_videoPaneMap.end(),IVS_OCX_RET_GET_INFO_ERROR);

		CVideoPane *pVideoPane = iter->second; //lint !e64

		if (m_ulShowWndNum < i)
		{
			BP_RUN_LOG_ERR(IVS_OCX_RET_GET_INFO_ERROR,_T("IVS_OCX::"),"Get wnd id free fail, all windows are busy.");
			return IVS_OCX_RET_GET_INFO_ERROR;
		}
		CHECK_EQUAL( m_lAutoChangeLayout, NOT_AUTO_CHANGE_LAYOUT, IVS_OCX_RET_GET_INFO_ERROR );

		//�жϵ�ǰ�����Ƿ��Ǹ澯���ڻ������Լ�����,��ѵ����Ҳ��ռ��
		if (BOTTOMBAR_ALARM_OPEN == pVideoPane->GetRealtimeAlarmState() 
			|| ulPaneID == pVideoPane->GetPaneID() 
			|| MAGNIFY_OPEN == pVideoPane->GetRealtimeMagnifyState() 
			|| BOTTOMBAR_MAGNIFY_OPEN == pVideoPane->GetDstMagnifyState()
			|| TYPE_WND_EMAP == pVideoPane->GetTypeWnd()
			|| TYPE_WND_TURNCTUISE == pVideoPane->GetTypeWnd())
		{
			++iter;
			continue;
		}
		return pVideoPane->GetPaneID(); //lint !e732 ������
	}
	BP_RUN_LOG_ERR(IVS_OCX_RET_GET_INFO_ERROR,_T("IVS_OCX::"),_T("VideoPaneManager get  free windows ID.Failed"));
	return IVS_OCX_RET_GET_INFO_ERROR;
}

// ��ȡָ������ָ��
CVideoPane* CVideoPaneMgr::GetWndPointerByHandle(LONG lHandle)
{
    CVideoPane *pVideoPane = nullptr;
    CAutoLock __Locker(m_csVideoPaneLock);
	MAP_VIDEOPANE_ITER iter = find_if(m_videoPaneMap.begin(),m_videoPaneMap.end(),
		std::bind2nd(FunctionObjectVideoPaneHandleIsEqual(),static_cast<ULONG>(lHandle)));

    pVideoPane = nullptr;

    if (iter != m_videoPaneMap.end())
    {
       pVideoPane = iter->second; //lint !e64
    }

    return pVideoPane;
}

// ��ȡָ������ָ��
CVideoPane* CVideoPaneMgr::GetWndPointerByHWND( const HWND hWnd )
{
    CVideoPane *pVideoPane = nullptr;
    CAutoLock __Locker(m_csVideoPaneLock);
    MAP_VIDEOPANE_ITER iter = find_if(m_videoPaneMap.begin(),m_videoPaneMap.end(),
       bind2nd(FunctionObjectVideoPaneVideoWndIsEqual(),hWnd));

    pVideoPane = nullptr;

    if (iter != m_videoPaneMap.end())
    {
        pVideoPane = iter->second; //lint !e64
    } else {
        BP_RUN_LOG_ERR(IVS_FAIL,"IVS_OCX::","VideoPaneManager get windows Pointer by HWND.Failed");
    }

    return pVideoPane;
} //lint !e818

// ��ȡָ������ָ��
CVideoPane* CVideoPaneMgr::GetWndPointerByID( ULONG ulWndID )
{
    if(!IsLegalityWndID(ulWndID))
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "", "Get window pointer by id fail, as parameter is invalid. WndID=%lu.", ulWndID);
        return nullptr;
    }

    CVideoPane *pVideoPane = nullptr;
    CAutoLock __Locker(m_csVideoPaneLock);

    //���ݴ���ID��ѯ ����ָ��
    MAP_VIDEOPANE_ITER iter = m_videoPaneMap.find(ulWndID);

    if (iter != m_videoPaneMap.end())
    {
        pVideoPane =  iter->second;  //lint !e64
    } else {
       BP_RUN_LOG_ERR(IVS_FAIL,"IVS_OCX::","VideoPaneManager get windows Pointer by ID.Failed WndID=%lu.", ulWndID);
    }
    return pVideoPane;
}

// �����豸�����ȡָ������ָ��
CVideoPane* CVideoPaneMgr::GetWndPointerByCameraCode(const std::string& strCameraCode)
{

	CVideoPane *pVideoPane = NULL;
	CAutoLock __Locker(m_csVideoPaneLock);

	MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin();

	// �����д�������
	for ( ; iter != m_videoPaneMap.end(); ++iter)
	{
		// �Ѵ�������
		pVideoPane = iter->second; //lint !e64
		if (nullptr == pVideoPane)
		{
			continue;
		}
		if (0 == strcmp(pVideoPane->GetCameraID(), strCameraCode.c_str()))
		{
			return pVideoPane;
		}
	}
	return NULL;
}

void CVideoPaneMgr::HideVideoPane(void)
{
    CAutoLock __Locker(m_csVideoPaneLock);
    MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin();

    // �����д�������
    for ( ; iter != m_videoPaneMap.end(); ++iter)
    {
        // �Ѵ�������
        CVideoPane *pVideoPane = iter->second; //lint !e64
        if (pVideoPane)
        {
			pVideoPane->ShowWindow(SW_HIDE);	        
			pVideoPane->HideAllBar();
        }
    }
}

// Returns:   bool
//          �������Խ� BOTTOMBAR_TALKBACK_CLOSE
//          û�������Խ� BOTTOMBAR_TALKBACK_OPEN
// Qualifier: �ж��Ƿ��������Խ�
BOOL CVideoPaneMgr::IsTalkback(void)
{
    CHECK_EQUAL( WND_TYPE_ANALYSIS, GetWndType(), BOTTOMBAR_TALKBACK_CLOSE );

    CAutoLock __Locker(m_csVideoPaneLock);
    ULONG i    = 0;
    BOOL  bRet = BOTTOMBAR_TALKBACK_CLOSE;

    MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin();

    while((iter != m_videoPaneMap.end()) && (i<m_ulShowWndNum))
    {
        CVideoPane *pVideoPane = iter->second; //lint !e64
        if (nullptr == pVideoPane)
        {
            continue;
        }
        // ����������Խ�, ���˳�ѭ��
        if(BOTTOMBAR_TALKBACK_OPEN == pVideoPane->GetRealtimeTalkbackState())
        {
            bRet = BOTTOMBAR_TALKBACK_OPEN;
            break;
        }

        ++iter;
        ++i;
    }

    return bRet;
}

// ��������Ӧ�Ի���
void CVideoPaneMgr::OnCurrentLocker(const CPoint &point)const
{
    ////�Ŵ�״̬Ϊ�����񣬲���Ҫ����
    //if (MAGNIFY == m_ulIsMagnify)
    //{
    //    return;
    //}

    //CAutoLock __Locker(m_csVideoPaneLock);
    //CRect rect;

    ////�жϵ�ǰ���������һ�����Ŵ���
    ////i<m_ulLayoutType�������Ƶ�����߿�ʱ����ѡ��δ��ʾ�ڵ�ǰ�����ϵĴ��������
    //ULONG i=0;
    //MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin();

    //while((iter != m_videoPaneMap.end()) && (i<m_ulShowWndNum))
    //{
    //    (void)pVideoPane->GetPaneRectFull(&rect);
    //    if(!rect.PtInRect(point))
    //    {
    //        // RealtimeBar Locker
    //        if(nullptr != pVideoPane->GetVideoRealtimeBar())
    //        {
    //            // TODO :
    //            //pVideoPane->GetVideoRealtimeBar()->DeleteSoundCtrl();
    //            //pVideoPane->GetVideoRealtimeBar()->DeleteTalkbackSound();
    //        }
    //    }

    //    ++iter;
    //    ++i;
    //}
	return;
}//lint !e715

void CVideoPaneMgr::OnHideAllLocker(void)
{
    CAutoLock __Locker(m_csVideoPaneLock);

    ULONG i=0;
    MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin();

    while((iter != m_videoPaneMap.end()) && (i<m_ulShowWndNum))
    {
        CVideoPane *pVideoPane = iter->second; //lint !e64
        if (nullptr == pVideoPane)
        {
            continue;
        }
        // ɾ��RealtimeBar����Ӧ�Ի�����������ƶԻ���
        if(nullptr != pVideoPane->GetVideoRealtimeBar())
        {
            pVideoPane->GetVideoRealtimeBar()->DeleteSoundCtrl();
            pVideoPane->GetVideoRealtimeBar()->DeleteTalkbackCtrl();
        }
        ++iter;
        ++i;
    }
}

void CVideoPaneMgr::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    //�Ŵ�״̬Ϊ�����񣬲���Ҫ����
    if (MAGNIFY == m_ulIsMagnify)
    {
        CVideoPane* pVideoPane = GetActiveWndPointer();
        CHECK_POINTER_VOID(pVideoPane);

        pVideoPane->OnLButtonDblClk(nFlags, point);
    }

    CAutoLock __Locker(m_csVideoPaneLock);
    CRect rect;

    //�жϵ�ǰ���������һ�����Ŵ���
    //i<m_ulLayoutType�������Ƶ�����߿�ʱ����ѡ��δ��ʾ�ڵ�ǰ�����ϵĴ��������
    ULONG i=0;
    MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin();
    for (; iter != m_videoPaneMap.end(),i < m_ulShowWndNum;++iter,i++)
    {
        CVideoPane *pVideoPane = iter->second; //lint !e64
        if (nullptr == pVideoPane)
        {
            continue;
        }
        pVideoPane->GetPaneRect(&rect);
        if (rect.PtInRect(point))
        {
            pVideoPane->OnLButtonDblClk(nFlags,point);
            break;
        }
    }
}

void CVideoPaneMgr::OnLButtonDown( UINT nFlags, CPoint point )
{
    //�Ŵ�״̬Ϊ�����񣬲���Ҫ����
    if (MAGNIFY == m_ulIsMagnify)
    {
        CVideoPane* pVideoPane = GetActiveWndPointer();
        CHECK_POINTER_VOID(pVideoPane);
        pVideoPane->OnLButtonDown(nFlags, point);
        pVideoPane->SetActivePane();
        return;
    }

    CAutoLock __Locker(m_csVideoPaneLock);
    CRect rect;

    //�жϵ�ǰ���������һ�����Ŵ���
    //i<m_ulLayoutType�������Ƶ�����߿�ʱ����ѡ��δ��ʾ�ڵ�ǰ�����ϵĴ��������
    ULONG i=0;

    for (MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin(); iter != m_videoPaneMap.end(),i<m_ulShowWndNum;++iter,i++)
    {
        CVideoPane *pVideoPane = iter->second; //lint !e64
        if (nullptr == pVideoPane)
        {
            continue;
        }
        pVideoPane->GetPaneRect(&rect);
        if (rect.PtInRect(point))
        {
            pVideoPane->SetActivePane();
            break;
        }
    }
}

BOOL CVideoPaneMgr::OnMouseWheel(UINT nFlags, short zDelta, const CPoint& pt)
{
    BOOL bRet = FALSE;
    //�Ŵ�״̬Ϊ�����񣬲���Ҫ����
    CVideoPane* pActiveVideoPane = GetActiveWndPointer();
    CHECK_POINTER(pActiveVideoPane,bRet);
	if ( BOTTOMBAR_MAGNIFY_OPEN == pActiveVideoPane->GetDstMagnifyState())
	{
		bRet = pActiveVideoPane->OnMouseWheel(nFlags, zDelta, pt);
		return TRUE;
	}

    CAutoLock __Locker(m_csVideoPaneLock);
    CRect rect;

    //�жϵ�ǰ���������һ�����Ŵ���
    //i<m_ulLayoutType�������Ƶ�����߿�ʱ����ѡ��δ��ʾ�ڵ�ǰ�����ϵĴ��������
    ULONG i=0;
    for (MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin(); iter != m_videoPaneMap.end(),i<m_ulShowWndNum;++iter,i++)
    {
        CVideoPane *pVideoPane = iter->second; //lint !e64
        if (nullptr == pVideoPane)
        {
            continue;
        }
        pVideoPane->GetPaneRect(&rect);

        if(NULL != m_pParentWnd)
        {
            m_pParentWnd->ClientToScreen(&rect);
        }
        if (rect.PtInRect(pt))
        {
            bRet= pVideoPane->OnMouseWheel(nFlags, zDelta, pt);
            break;
        }
    }
    return bRet;
}

// �˳��ֲ��Ŵ�״̬
void CVideoPaneMgr::CancleAllMagnifyWnd()
{
	//����
	CAutoLock __Locker(m_csVideoPaneLock);

	//�ӿڲ���������ж�
	MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin();
	MAP_VIDEOPANE_ITER end  = m_videoPaneMap.end();

	// �������д��ڣ��Դ��ڷŴ�״̬�Ĵ��ڷ��͹ر���Ϣ
	for (; iter != end; ++iter)
	{
		CVideoPane* pVideoPane = dynamic_cast<CVideoPane *>(iter->second); //lint !e611
		if (NULL == pVideoPane)
		{
			continue;
		}
		if (MAGNIFY_OPEN == pVideoPane->GetRealtimeMagnifyState())
		{
			::PostMessage(pVideoPane->GetVideoRealtimeBar()->GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(IDC_BTN_TOOL_REALTIME_MAGNIFY, BN_CLICKED), (LPARAM)pVideoPane->GetVideoRealtimeBar()->GetMagnifyBtnHwnd());
		}
	}
}

// Qualifier: ˢ�����пɼ�����
void CVideoPaneMgr::RefreshAllWindow(void)const
{
    //CAutoLock __Locker(m_csVideoPaneLock);
    //(void)for_each(m_videoPaneMap.begin(),m_videoPaneMap.end(),
    //   FunctionObjectVideoPaneRefreshWindow());
}

ULONG CVideoPaneMgr::Set3DPTZState(ULONG ulWndID, ULONG ulIsStart3D)
{
    if (!IsLegalityWndID(ulWndID))
    {
        return IVS_PARA_INVALID;
    }

    if (ulIsStart3D != PTZ_3D_CLOSE && ulIsStart3D != PTZ_3D_OPEN)
    {
        return IVS_PARA_INVALID;
    }

    //����
    CAutoLock __Locker(m_csVideoPaneLock);

    MAP_VIDEOPANE_ITER iter = m_videoPaneMap.find(ulWndID);
    if (iter == m_videoPaneMap.end())
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID,"", "Video Pane Manager find video pane error.");
        return IVS_PARA_INVALID; // ��������²��ᵽ������
    }

    CVideoPane * pVideoPane = iter->second; //lint !e64
    CHECK_POINTER(pVideoPane,IVS_FAIL);

    //�ж��Ƿ����ڲ��ţ�û�ڲ�����������3D״̬
    if (VIDEO_PLAY_STATE_NOT_PLAYING == pVideoPane->GetPlayState())
    {
        BP_RUN_LOG_ERR(IVS_OCX_RET_VIDEOPANE_ISNOT_PLAYING_ERROR,"", "Video Pane Manager set 3D PTZ error,pane is not playing video.");
        return IVS_OCX_RET_VIDEOPANE_ISNOT_PLAYING_ERROR;
    }

    //���ô���Ĺ��������豸����
    if (ulIsStart3D != pVideoPane->Get3DPTZState()) //lint !e737 ������
    {
        pVideoPane->Set3DPTZState(ulIsStart3D);
    }

    return IVS_SUCCEED;
}

// Qualifier: �ɴ�����ã���������ѡ���ı߿���ɫ
void CVideoPaneMgr::SetActivatePane( ULONG PaneID )
{
    CHECK_POINTER_VOID(m_pOcxCtrl);
    if (PaneID != m_ulActivePaneID)
    {
        CAutoLock __Locker(m_csVideoPaneLock);   //����

        // ��ǰһ�������߿�λ
        MAP_VIDEOPANE_ITER iter = m_videoPaneMap.find(m_ulActivePaneID);

        // �ҵ���ʾ֮ǰ�л����
        if (iter != m_videoPaneMap.end())
        {
            CVideoPane *pVideoPane = iter->second;  //lint !e64
            CHECK_POINTER_VOID(pVideoPane);
            pVideoPane->SetNotActivate();
            pVideoPane->SetBorder(PANE_STATE_NOT_ACTIVATE);   // ��λ�߿�
			pVideoPane->HideTwoBar();
        }

        // �����µĻ����
        iter = m_videoPaneMap.find(PaneID);

        if (iter != m_videoPaneMap.end())
        {
            CVideoPane *pVideoPane = iter->second;  //lint !e64
            CHECK_POINTER_VOID(pVideoPane);
            pVideoPane->SetActivate();
            pVideoPane->SetBorder(PANE_STATE_ACTIVATE);
			pVideoPane->ResetBarPosition();
        }

		SetActivePaneID(PaneID);

        //�ϱ������ı��¼�
       dynamic_cast<CIVS_OCXPlayerCtrl*>(m_pOcxCtrl)->PostActiveWndChangeEventMsg(0, m_ulActivePaneID);
    }

	CHECK_POINTER_VOID(m_pParentWnd);
    // ����ȫ�����Ѵ�����Ϊǰ��
    if (FULL_SCREEN == m_ulIsFullScreen)
    {
        m_pParentWnd->SetForegroundWindow();
        m_pParentWnd->BringWindowToTop();
        m_pParentWnd->SetFocus();
    } else {
        m_pOcxCtrl->SetFocus();
    }

    //ResetBarPosition();//���ü������Ҫ�������д���
}

ULONG CVideoPaneMgr::SetAddAnalysisResult( ULONG ulWinID, bool bState )
{
    CVideoPane * pVideoPane = GetWndPointerByID(ulWinID);
    CHECK_POINTER(pVideoPane,IVS_OPERATE_MEMORY_ERROR);

    ULONG ulRet = 0;//IVS_SDK_SetSuperposition(pVideoPane->GetVideoWnd(), bState);
    //if (IVS_SUCCEED != ulRet)
    //{
    //    BP_RUN_LOG_ERR("", "Set superposition fail from dll, ErrorCode=%lu.", ulRet);

    //    if (nullptr != pVideoPane->GetVideoRealtimeBar())
    //    {
    //        //if (TRUE == bState)
    //        //{
    //        //	pVideoPane->GetVideoRealtimeBar()->SetAnalysisPic(BUTTON_NORMAL);
    //        //	pVideoPane->SetRealtimeAnalysisState(BOTTOMBAR_ANALYSIS_CLOSE);
    //        //	GetOcxCtrl()->StopAnalysis(pVideoPane->GetPaneID());
    //        //}
    //        //else
    //        //{
    //        //	pVideoPane->GetVideoRealtimeBar()->SetAnalysisPic(BUTTON_CLICKED);
    //        //	pVideoPane->SetRealtimeAnalysisState(BOTTOMBAR_ANALYSIS_OPEN);
    //        //	GetOcxCtrl()->StartAnalysis(pVideoPane->GetPaneID());
    //        //}
    //    }
    //}
    return ulRet;
}//lint !e715

ULONG CVideoPaneMgr::SetAlarmState( ULONG ulWndID, bool bIsOpen )
{
    if (0 == ulWndID)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "", "Video pane manager set alarm state fail, handle is invalid.");
        return IVS_PARA_INVALID;
    }

    CVideoPane * pVideoPane = GetWndPointerByID(ulWndID);
    CHECK_POINTER(pVideoPane,IVS_PARA_INVALID);

    pVideoPane->SetRealtimeAlarmState((bIsOpen?BOTTOMBAR_ALARM_OPEN:BOTTOMBAR_ALARM_CLOSE));
    //CTranVideoRealtimeBar * pVideoRealtimeBar = pVideoPane->GetVideoRealtimeBar();
    //CHECK_POINTER(pVideoRealtimeBar,IVS_PARA_INVALID);

    pVideoPane->Invalidate(TRUE);
    return IVS_SUCCEED;
}

// ���ð�ť״̬����ť�ûң�
void CVideoPaneMgr::SetButtonStatusOff( ULONG ulWinID, LONG ulButtonType )
{
    CVideoPane * pVideoPane = GetWndPointerByID(ulWinID);
    CHECK_POINTER_VOID(pVideoPane);

    switch (ulButtonType)
    {
    case BOTTOMBAR_PLAY:
    case BOTTOMBAR_RECORD:
    case BOTTOMBAR_CAPTURE:
    case BOTTOMBAR_LABEL:
    case BOTTOMBAR_TALKBACK:
    case BOTTOMBAR_SOUND:
    case BOTTOMBAR_MAGNIFY:
    case BOTTOMBAR_PTZ:
    case BOTTOMBAR_ALARM:
    case BOTTOMBAR_ANALYSIS:
        {
            // TODO :            pVideoPane->SetRealtimeAnalysisPic(BUTTON_UNABLE);
        }
        break;
    case TITLEBAR_CLOSE:
        {
            // TODO :            pVideoPane->SetTitleButtonPic(FALSE);
        }
        break;
    default:
        break;
    }
}

// ���ð�ť״̬����ť������
void CVideoPaneMgr::SetButtonStatusOn( ULONG ulWinID, LONG ulButtonType )
{
    CVideoPane * pVideoPane = GetWndPointerByID(ulWinID);
    CHECK_POINTER_VOID(pVideoPane);

    switch (ulButtonType)
    {
    case BOTTOMBAR_PLAY:
    case BOTTOMBAR_RECORD:
    case BOTTOMBAR_CAPTURE:
    case BOTTOMBAR_LABEL:
    case BOTTOMBAR_TALKBACK:
    case BOTTOMBAR_SOUND:
    case BOTTOMBAR_MAGNIFY:
    case BOTTOMBAR_PTZ:
    case BOTTOMBAR_ALARM:
    case BOTTOMBAR_ANALYSIS:
        {
            // TODO :            pVideoPane->SetRealtimeAnalysisPic(BUTTON_NORMAL);
        }
        break;
    case TITLEBAR_CLOSE:
        {
            // TODO :            pVideoPane->SetTitleButtonPic(TRUE);
        }
        break;
    default:
        break;
    }
}

ULONG CVideoPaneMgr::SetCaptureState( ULONG ulWndID, bool bIsOpen )
{
    //�˴������ж�ulWndID��С,���ulWndID����0,GetWndPointerByID�᷵�ؿ�
    CVideoPane * pVideoPane = GetWndPointerByID(ulWndID);
    CHECK_POINTER(pVideoPane,IVS_PARA_INVALID);

    // ���ʵ��ҵ����ֹͣ������Ҫ���°�ť״̬��ֱ�ӷ���
    if (VIDEO_PLAY_STATE_NOT_PLAYING == pVideoPane->GetPlayState())
    {
        return IVS_SUCCEED;
    }

    if (WND_TYPE_REALTIME == GetWndType())
    {
        pVideoPane->SetRealtimeCaptureState((bIsOpen?BOTTOMBAR_CAPTURE_OPEN:BOTTOMBAR_CAPTURE_CLOSE));
        CTranVideoRealtimeBar * pVideoRealtimeBar = pVideoPane->GetVideoRealtimeBar();
        CHECK_POINTER(pVideoRealtimeBar,IVS_PARA_INVALID);

        if (bIsOpen)
        {
            // TODO :            pVideoRealtimeBar->SetCapturePic(BUTTON_CLICKED);
        } else {
            // TODO :            pVideoRealtimeBar->SetCapturePic(BUTTON_NORMAL);
        }
    }
    return IVS_SUCCEED;
}

// �����豸����
ULONG CVideoPaneMgr::SetDevName(ULONG ulWndID, LPCSTR pDevName)
{
    //����
    m_csVideoPaneLock.Lock();//CAutoLock __Locker(m_csVideoPaneLock);

    MAP_VIDEOPANE_ITER iter = m_videoPaneMap.find(ulWndID);
    if (iter == m_videoPaneMap.end())
    {
        m_csVideoPaneLock.UnLock();
        BP_RUN_LOG_ERR(IVS_OCX_RET_SET_DEVNAME_ERROR,"", "Video Pane Manager SetDevName error.");
        return IVS_OCX_RET_SET_DEVNAME_ERROR; // ��������²��ᵽ������
    }

    CVideoPane *pVideoPane = iter->second; //lint !e64
    m_csVideoPaneLock.UnLock();

    CHECK_POINTER(pVideoPane,IVS_OCX_RET_SET_DEVNAME_ERROR);

	pVideoPane->SetDevName(std::string (pDevName));
    pVideoPane->Invalidate();

	CTranVideoTitleBar* pTVideoTitleBar = pVideoPane->GetVideoTitleBar();
    if (nullptr == pTVideoTitleBar)
    {
        BP_RUN_LOG_ERR(IVS_OCX_RET_SET_DEVNAME_ERROR,"", "get video title bar fail.");
        return IVS_SUCCEED;
    }

    //���ô���Ĺ��������豸����	
    pTVideoTitleBar->SetDevName(std::string(pDevName));
    return IVS_SUCCEED;
}

IVS_INT32 CVideoPaneMgr::SetDisplayScale(IVS_INT32 iSessionID, ULONG ulDisplayScale)
{
    //�ӿڲ���������ж�
    m_ulDispalyAspectRatio  = ulDisplayScale;
    MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin();
	MAP_VIDEOPANE_ITER end  = m_videoPaneMap.end();

	m_ulDisplayScale = ulDisplayScale;

	// �������д��ڣ������Ѿ����ŵĴ��ڵĲ��ű���
	for (; iter != end; ++iter)
    {
        CVideoPane* pVideoPane = dynamic_cast<CVideoPane *>(iter->second); //lint !e611
        if (NULL == pVideoPane)
        {
            continue;
        }
    	IVS_ULONG ulPlayHandle = static_cast<IVS_ULONG>(pVideoPane->GetPlayHandle());
		if (ulPlayHandle)
		{
			if (IVS_SUCCEED != IVS_SDK_SetDisplayScale(iSessionID, ulPlayHandle, static_cast<IVS_INT32>(ulDisplayScale)))
			{
				 BP_RUN_LOG_ERR(IVS_FAIL, "", "SetDisplayScale Fail.");
			}
		}
	}
    return IVS_SUCCEED;
}

// Qualifier: �ָ�ȫ��
void CVideoPaneMgr::ResetFullScreen(void)
{
    CHECK_POINTER_VOID(m_pParentWnd);
    if (FULL_SCREEN == m_ulIsFullScreen)
    {
        // �ָ�������ָ�룬����ʾ
        m_pParentWnd->ModifyStyle(WS_POPUP, WS_CHILD, 0);
        CHECK_POINTER_VOID(m_pOcxCtrl);
        m_pParentWnd->SetParent(m_pOcxCtrl);
        m_pOcxCtrl->Invalidate(TRUE);

        m_pOcxCtrl->GetParentOwner()->ShowWindow(SW_SHOW);

        m_pOcxCtrl->SetForegroundWindow();
        m_pParentWnd->SetForegroundWindow();

        m_pOcxCtrl->SetFocus();
        m_pParentWnd->SetWindowPlacement(&m_struOldWndpl);

        CRect rect;
        m_pParentWnd->GetClientRect(rect);

        //�Ƿ��ǷŴ�״̬������ֻ��Ҫ����һ�����񣬷���Ҫ����ȫ������
        if (MAGNIFY == m_ulIsMagnify)
        {
            m_ulOperatePaneID = m_ulActivePaneID;
            SetVideoPanePosition(rect);
        }
        else
        {
            if (IVS_SUCCEED != SetWndLayout(rect, m_windLayOut))
            {
                BP_RUN_LOG_ERR(IVS_FAIL, "","Video Pane Manager reset fullScreen error,set window layout fail.");
            }
        }

        // ���Ϊ��ȫ��
        m_ulIsFullScreen = NOT_FULL_SCREEN;
		m_pOcxCtrl->Invalidate(TRUE);
		SetAllBarWindowsLong(m_ulOldParentHanlde);
        // �¼��ϱ���CU
        dynamic_cast<CIVS_OCXPlayerCtrl*>(m_pOcxCtrl)->PostExitFullScreenEventMsg("EVENT_EXIT_FULL_SCREEN", IVS_SUCCEED, GetActivePaneID());

		if (g_LowLevelKeyboardHook!=NULL)
		{
			UnhookWindowsHookEx(g_LowLevelKeyboardHook);
			g_LowLevelKeyboardHook = NULL;
			g_CVideoPaneMgr = NULL;
		}
		HideShellTrayWnd(TRUE);
    }
}

void CVideoPaneMgr::SetFullScreen(void)
{
    CHECK_POINTER_VOID(m_pParentWnd);
    CHECK_POINTER_VOID(m_pOcxCtrl);

    if (NOT_FULL_SCREEN == m_ulIsFullScreen)
    {
        //����Ϊ������
//       m_pOcxCtrl->ShowWindow(SW_HIDE);
        m_pOcxCtrl->GetParentOwner()->ShowWindow(SW_HIDE);

        m_pParentWnd->SetParent(NULL);
        m_pParentWnd->ModifyStyle(WS_CHILD, WS_POPUP, SWP_NOMOVE | SWP_NOSIZE);
        m_pParentWnd->SetForegroundWindow();
        m_pParentWnd->SetFocus();

        //LONG g_iCurScreenWidth  = GetSystemMetrics(SM_CXSCREEN);
        //LONG g_iCurScreenHeight = GetSystemMetrics(SM_CYSCREEN);

        HMONITOR hMonitor = MonitorFromWindow(m_pOcxCtrl->GetSafeHwnd(),MONITOR_DEFAULTTONEAREST);
        MONITORINFO info;
        info.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(hMonitor,&info);

        long top = info.rcMonitor.top;
        long left = info.rcMonitor.left;
        long bottom = info.rcMonitor.bottom;
        long right = info.rcMonitor.right;

        m_pParentWnd->GetWindowPlacement(&m_struOldWndpl);

        CRect rectFullScreen, parentWndRect, parentClientRect;

        m_pParentWnd->GetWindowRect(parentWndRect);
        m_pParentWnd->GetClientRect(parentClientRect);

        //����߿��С
        LONG border = ((parentWndRect.bottom - parentWndRect.top) - parentClientRect.bottom) / 2;

        rectFullScreen.left = left - border;
        rectFullScreen.top    = top - border;
        rectFullScreen.right  = right;
        rectFullScreen.bottom = bottom;

        //enter into full screen;
        WINDOWPLACEMENT struWndpl;
        struWndpl.length = sizeof(WINDOWPLACEMENT);
        struWndpl.flags   = 0;
        struWndpl.showCmd = SW_SHOWNORMAL;
        struWndpl.rcNormalPosition = rectFullScreen;
        m_pParentWnd->SetWindowPlacement(&struWndpl);
        // �ö�����
		m_pParentWnd->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

        //�Ƿ��ǷŴ�״̬������ֻ��Ҫ����һ�����񣬷���Ҫ����ȫ������
		m_ulOldParentHanlde = m_ulParentHanlde;
		m_ulParentHanlde = 0;
        if (MAGNIFY == m_ulIsMagnify)
        {
            CRect rect;
            rect.left = 0;
            rect.top = 0;
            rect.right = rectFullScreen.right - rectFullScreen.left;
            rect.bottom = rectFullScreen.bottom - rectFullScreen.top;

            m_ulOperatePaneID = m_ulActivePaneID;
            SetVideoPanePosition(rect);
        }
		else 
		{
            if (IVS_SUCCEED != SetWndLayout(rectFullScreen, m_windLayOut))
            {
                BP_RUN_LOG_ERR(IVS_FAIL, "",",set window layout fail.");
            }
        }

		SetAllBarWindowsLong(m_ulParentHanlde);
		// ���Ϊȫ��
		m_ulIsFullScreen = FULL_SCREEN;
		ResetBarPosition();
        // �¼��ϱ���CU
       dynamic_cast<CIVS_OCXPlayerCtrl*>(m_pOcxCtrl)->PostEnterFullScreenEventMsg("EVENT_ENTER_FULL_SCREEN", IVS_SUCCEED, GetActivePaneID());
	   g_CVideoPaneMgr = this;
	   if (g_LowLevelKeyboardHook == NULL)
	   {
		   g_LowLevelKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL,LowLevelKeyboardProc,GetModuleHandle(NULL),0);
	   }
	   HideShellTrayWnd(FALSE);
	   // ���ý��� 2014-4-28 add ȫ��������esc�˳���Ҫ
	   SetActivatePane(m_ulActivePaneID);
    }
}

// ��������
LONG CVideoPaneMgr::SetLanguage(LPCTSTR pLanguage)
{
    if (0 == strlen(pLanguage))
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID,"", "Set language fail, as parameter is invalid.");
        return IVS_PARA_INVALID;
    }
    TCHAR Buffer[BUFSIZE];
    memset(Buffer,0x0,BUFSIZE);
    if(0 ==GetModuleFileName(AfxGetInstanceHandle(),Buffer,BUFSIZE))
    {
        BP_RUN_LOG_ERR(IVS_OCX_RET_SKIN_INIT_ERROR,_T("IVS_OCX::"),_T("Get Module File Name.Failed"));
    }
    // �������ԣ���ȡ�����ļ���·��
    std::string strLanguageXMLPath;
    (void)strLanguageXMLPath.append(Buffer);
    std::string strFolder = strLanguageXMLPath.substr(0,strLanguageXMLPath.rfind(_T("\\")));
    strFolder += I18N_DIR;
	strFolder += pLanguage;
	strFolder += ".xml";
	
	// ����������Ϣ
	if (!CI18N::GetInstance().SetLanguage(strFolder))
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "set language failed", "NA");
		return IVS_FAIL;
	}

    // TODOˢ�°�ťtip��ʾ
    CAutoLock __Locker(m_csVideoPaneLock);

    for (MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin(); iter != m_videoPaneMap.end(); ++iter)
    {
		CVideoPane* pVideoPane = dynamic_cast<CVideoPane*>(iter->second);//lint !e611
		if (NULL == pVideoPane)
		{
			continue;
		}
		pVideoPane->UpdateAllTipText();
    }
    return IVS_SUCCEED;
}

// Qualifier: ����ƽ������Ĳ��֣�1,4,9,16,25,36,49,64�ȣ�
void CVideoPaneMgr::SetLayoutAsAverage( const CRect &rect, ULONG ulLayOutNum )
{
    // ������ʱ�ĵ㣬���ڳ�ʼ��rect��ÿ������һ��
    CPoint topLeftTemp;
    CPoint bottomRightTemp;

    // �����ÿ�еĴ������
    int numberOfWndEachRow = ((int)sqrt((double)ulLayOutNum));

    // ������ƽ����������
    LONG averageX = rect.Width() / numberOfWndEachRow;
    LONG averageY = rect.Height() / numberOfWndEachRow;
    CRect _Rect;

	//����
	LONG remainderX = rect.Width() % numberOfWndEachRow;
	LONG remainderY = rect.Height() % numberOfWndEachRow;
	//ƫ������ԭ���λ��
	LONG xOffset = rect.left;
	LONG yOffset = rect.top;
	LONG ulaverageX = 0;
	LONG ulaverageY = 0;

    // ѭ����������
    for (int nY = 0; nY < numberOfWndEachRow; nY++)
    {
		ulaverageY = averageY;
		if (remainderY>0)
		{
			if (nY < remainderY)
			{
				//�����ű�����С��Ҫ��1
				ulaverageY = averageY + 1;
			}
		}

		topLeftTemp.y = yOffset;
		bottomRightTemp.y = yOffset + ulaverageY;
		yOffset = bottomRightTemp.y;

		xOffset = rect.left;
        for (int nX = 0; nX < numberOfWndEachRow; nX++)
        {
			ulaverageX = averageX;
			if (remainderX>0)
			{
				if (nX < remainderX)
				{
					//�����ű�����С��Ҫ��1
					ulaverageX = averageX + 1;
				}
			}

			topLeftTemp.x = xOffset;
			bottomRightTemp.x = xOffset + ulaverageX;
			xOffset = bottomRightTemp.x;

            _Rect.SetRect(topLeftTemp,bottomRightTemp);
            SetVideoPanePosition(_Rect);
        }
    }
}

ULONG CVideoPaneMgr::SetManifyWindow(ULONG ulWndID, ULONG ulManifyState)
{
    //��������������õ�ʱ�򣬴���Ŵ�Ҳ����Ҫʵ�����ڲ��ţ����Բ�������ʱ��Ĭ�ϴ����ڿ���ʵ��
    if (MAGNIFY == ulManifyState)
    {
        SetActivatePane(ulWndID);
        CHECK_POINTER(m_pParentWnd,IVS_PARA_INVALID);

        // �Ѵ����С�Ŵ�Ϊ�����ڴ�С
        CRect rect;
        m_pParentWnd->GetClientRect(rect);

        // �Ŵ󴰸�
        Magnify(rect);
    }
    else
    {
        Minify();
    }
    return IVS_SUCCEED;
}

ULONG CVideoPaneMgr::SetMicVolume(ULONG ulVolumeValue) const
{
    return IVS_SDK_SetMicVolume(ulVolumeValue);
}//lint !e715

ULONG CVideoPaneMgr::GetMicVolume(void) const
{
	return IVS_SDK_GetMicVolume();
}

// ���²���ǰ���¼�����¼��һЩ����
ULONG CVideoPaneMgr::SetParameter( ULONG ulLayoutType )
{
	CParentWnd* pParentWnd = dynamic_cast<CParentWnd*>(m_pParentWnd);
	CHECK_POINTER(pParentWnd,IVS_FAIL);
    // ĿǰΪֹ������СΪ1���Ϊ64
    if (!LayoutMgr.CheckLayoutType(ulLayoutType))
    {
        return IVS_FAIL;
    }

    // �Ѹ����ڵľ���rect���
	pParentWnd->m_ListCrect.clear();
	pParentWnd->Invalidate(TRUE);

    // ��λ�Ŵ���λ
    m_ulIsMagnify = MINIFY;

    // ������ʾ�Ĵ�����
    // �ж��Ƿ��иı�
    if (m_windLayOut != ulLayoutType)
    {
		ULONG ulLayOutNum = ulLayoutType/10;
		if (ulLayOutNum == 65)	//�Զ��岼��
		{
            CIVS_OCXPlayerCtrl* pocx = dynamic_cast<CIVS_OCXPlayerCtrl*>(m_pOcxCtrl);
            CHECK_POINTER(pocx,IVS_PARA_INVALID);
            IVS_UINT32 uX = 0;
            IVS_UINT32 uY = 0;
			std::string strXML;
            if(pocx->m_pLocalConfig.GetLayoutXML(ulLayoutType,strXML)!= IVS_SUCCEED)
            {
            	return IVS_PARA_INVALID;
            }
			RECTLIST _rectList;
			(void)pocx->m_pLocalConfig.ParseLayoutXML2List(strXML.c_str(), _rectList,uX,uY);
			if (_rectList.empty())
			{
				return IVS_PARA_INVALID;
			}

			m_ulShowWndNum = static_cast<ULONG>(_rectList.size());
		} else if(WIND_LAYOUT_TVW==ulLayoutType)
		{//����ǽ����
			IVS_UINT32 uX = 0;
			IVS_UINT32 uY = 0;
			TVWALL_PANE_LIST paneList;
			IVS_INT32 iRet = COCXTVWallXMLProcess::GetInst().GetTVWallPaneList(m_ulTVWallID, paneList, uX, uY);
			if(IVS_SUCCEED!=iRet || paneList.empty())
			{
				return IVS_PARA_INVALID;
			}
			m_ulShowWndNum = static_cast<ULONG>(paneList.size());
		} else
		{
			m_ulShowWndNum = ulLayOutNum;                  // �����µ���ʾ������
		}

         m_windLayOut = static_cast<IVS_WIND_LAYOUT>(ulLayoutType); //���洰����ʾ����

        //�����󣬻�����ʱ�������ֱ�С�ҵ�ǰ�����ᱻ����ʱ���ѻ�����Ϊ���һ�����񣬷��򲻱�
        //----------------------------------------------------------------------------------------------------------------------------------------------
        if (m_ulShowWndNum < m_ulActivePaneID)
        {
            SetActivatePane(m_ulShowWndNum);
        }
    }
    m_ulOperatePaneID = 1;

    //----------------------------------------------------------------------------------------------------------------------------------------------

    /*  �������������в���Ҫ��ҳ  ***************************************************************************************************************
    //�ı䴰�񲼾�ʱ����Ҫȷ��Ҫ��ʾ��ҳ�룬��ҳ���ɵ�ǰ�Ļ����ȷ��
    m_ulPageNumner = (m_ulActivePaneID - 1) / m_ulShowWndNum + 1;

    //�ò��������ڱ�ʶ�ӵڼ�������ʼ��ҳ�벻ͬ����ʼ��������ͬ
    m_ulOperatePaneID = (m_ulPageNumner - 1) * m_ulShowWndNum + 1; //lint !e845 �˴�Ӧ�жϹ�m_ulLayoutType��Ϊ0
    ************************************************************************************************************************************/
    return IVS_SUCCEED;
}

ULONG CVideoPaneMgr::SetRecordState( ULONG ulWndID, bool bIsOpen )
{
    //�˴������ж�ulWndID��С,���ulWndID����0,GetWndPointerByID�᷵�ؿ�
    CVideoPane * pVideoPane = GetWndPointerByID(ulWndID);
    CHECK_POINTER(pVideoPane,IVS_PARA_INVALID);

    pVideoPane->SetRealtimeRecordState((bIsOpen? BOTTOMBAR_RECORD_OPEN:BOTTOMBAR_RECORD_CLOSE));
    CTranVideoRealtimeBar * pVideoRealtimeBar = pVideoPane->GetVideoRealtimeBar();
    CHECK_POINTER(pVideoRealtimeBar,IVS_PARA_INVALID);

    // TODO :    pVideoRealtimeBar->SetRecordPic(TRUE == bIsOpen ? BUTTON_CLICKED:BUTTON_NORMAL);

    return IVS_SUCCEED;
}

ULONG CVideoPaneMgr::SetSoundState( ULONG ulWndID, bool bIsOpen )
{
    CVideoPane * pVideoPane = GetWndPointerByID(ulWndID);
    CHECK_POINTER(pVideoPane,IVS_PARA_INVALID);

    // ���ڲ�ͬ������  ʵ�����ط�
    switch(pVideoPane->GetWndType())
    {
    case WND_TYPE_REALTIME:
        {
            pVideoPane->SetRealtimeSoundState((bIsOpen?BOTTOMBAR_SOUND_OPEN:BOTTOMBAR_SOUND_CLOSE));

            // modify by wanglei 00165153:2013.6.28
            pVideoPane->IsSoundOn(bIsOpen?true:false);

            CTranVideoRealtimeBar * pVideoRealtimeBar = pVideoPane->GetVideoRealtimeBar();
            CHECK_POINTER(pVideoRealtimeBar,IVS_PARA_INVALID);

            if (bIsOpen)
            {
                pVideoRealtimeBar->SetSoundPic(BUTTON_CLICKED);
            } else {
               pVideoRealtimeBar->SetSoundPic(BUTTON_NORMAL);
            }

            BP_RUN_LOG_ERR(IVS_FAIL,"", "set sound state realtime");
        }

        break;    // end case WND_TYPE_REALTIME:
    default:
        break;
    }

    return IVS_SUCCEED;
}

ULONG CVideoPaneMgr::SetSoundStateToFalse( ULONG ulWndID )
{
    CVideoPane* pActiveVideoPane = GetWndPointerByID(ulWndID);
    CHECK_POINTER(pActiveVideoPane,IVS_OPERATE_MEMORY_ERROR);

    if (ulWndID != m_ulActivePaneID)
    {
        // modify by wanglei 00165153:2013.6.28
        pActiveVideoPane->SetRealtimeSoundState(BOTTOMBAR_SOUND_CLOSE);
        pActiveVideoPane->IsSoundOn(false);

        //ֹͣ��ظ�������Ĭ��ֵ100
        pActiveVideoPane->SetAudioVolume(PLAYER_AUDIO_MAX_VOLUM);
    }

    return IVS_SUCCEED;
}

// ���ù�����Ȩ��
LONG CVideoPaneMgr::SetToolbarEnableStatus( ULONG ulWndID ,ULONGLONG dwToolbarBtn, ULONGLONG lButtonStatus )
{
    //����
    CAutoLock __Locker(m_csVideoPaneLock);

    MAP_VIDEOPANE_ITER iter = m_videoPaneMap.find(ulWndID);
    if (m_videoPaneMap.end() == iter)
    {
        return IVS_FAIL;
    }

    CVideoPane *pVideoPane = iter->second; //lint !e64
    CHECK_POINTER(pVideoPane,IVS_FAIL);

    pVideoPane->SetToolbarEnableStatus(dwToolbarBtn,lButtonStatus);
    return IVS_SUCCEED;
}

// ���ݴ����λ�����ݲ�����Ƶ���Ŵ�������������߻�����
void CVideoPaneMgr::SetVideoPanePosition( CRect& rect)
{
    if (m_ulShowWndNum < m_ulOperatePaneID)return;

	CHECK_POINTER_VOID(m_pParentWnd);
	CAutoLock __Locker(m_csVideoPaneLock);   //����

    CVideoPane * pVideoPane = nullptr;
    MAP_VIDEOPANE_ITER iter = m_videoPaneMap.find(m_ulOperatePaneID);
    if (iter == m_videoPaneMap.end())
    {
        CHECK_POINTER_VOID(IVS_NEW((CVideoPane*&)pVideoPane));

        pVideoPane->Create(IDD_DLG_VIDEOPANE, m_pParentWnd);
        pVideoPane->SetPaneID(m_ulOperatePaneID);
        pVideoPane->SetWndType(m_ulWndType);
        pVideoPane->SetClientType(m_ulClientType);
		
        //pVideoPane->CreateBar();
		// ���ܷ�������Ҫ������
		//pVideoPane->CreateBarAnalysisBar();
		//��Ҫ�������ܷ����Ĺ�����
		if (WND_TYPE_ANALYSIS == m_ulWndType)
		{
			pVideoPane->CreateBarAnalysisBar();
		}

		if (isAnalysisWndType())
        {
            m_IsShowTitleBar = TITLEBAR_HIDE;
        }

		pVideoPane->SetToolBarBtnVisableStatus(m_ullToolBarBtnVisableStatus);
        pVideoPane->SetDisplayScale(m_ulDispalyAspectRatio);
        // ����map�����ڹ���
        (void)m_videoPaneMap.insert(std::make_pair(m_ulOperatePaneID, pVideoPane));

        if (1 == m_ulOperatePaneID)
        {
            // map��ĵ�һ������Ϊ�
            CVideoPane * pVideoPaneFirst = m_videoPaneMap.begin()->second; //lint !e64
            pVideoPaneFirst->SetActivate();

            // ��ǰ�����Ĭ��Ϊ1
            m_ulActivePaneID = 1;
        }

		//pVideoPane->ResetBottomButton();
    } else {
        pVideoPane = iter->second; //lint !e64
    }

	////�������ѵ����������ѵ���
	//if (pVideoPane->GetTurnCruiseStatus())
	//{
	//	pVideoPane->CreatemTurnCruisePane();
	//}
    //����������ʾ����
    pVideoPane->SetPaneRectFull(rect);

	//���ǵ���ǽ������TVWallBar���澯��ťʹ��
	if(0 == m_ulTVWallID)
	{
		pVideoPane->SetTVWallStatusBarStatus(TVWALL_BAR_STAUS_HIDE);
		pVideoPane->SetAlarmButtonStatus((unsigned long long)1);//ʵ��ʱ�澯��ťʹ��
	}
	//�ǵ���ǽ���������δ��ʾ����ʾTVWallBar
	else 
	{
		pVideoPane->CreateTVWallStatusBar();
		if(!pVideoPane->GetTVWallStatusBarStatus())
		{
			pVideoPane->SetTVWallStatusBarStatus(TVWALL_BAR_STAUS_OFFLINE);
		}		
	}

    //�����ʼ���������л���ʾ����zheg
    pVideoPane->SetBackGroundBitmap(rect);

    ////�ֲ��Ŵ����ƶ����ô���Ϊ��Ƶԭʼ����
    pVideoPane->ShowWindow(SW_SHOW);
    pVideoPane->MoveWindow(&rect);
	pVideoPane->SetPaneRect(rect);
	pVideoPane->ResetBarPosition();

	//////////////////////////////////////////////////////////////////////////
	//����ʽ���ְ�ť�Ƿ��û�
	if (m_ulShowWndNum  >= LADDERLAYOUTMINWINDOWSNUM && m_ulShowWndNum <= LADDERLAYOUTMAXWINDOWSNUM)
	{
		pVideoPane->SetToolbarEnableStatus(IVS_TOOLBAR_WINDOW_MAIN,1);//lint !e747
	}else{
		pVideoPane->SetToolbarEnableStatus(IVS_TOOLBAR_WINDOW_MAIN,0);//lint !e747
	}

	//���ӵ�ͼ��ť���û�
	pVideoPane->SetToolbarEnableStatus(IVS_TOOLBAR_OPEN_MAP,1);//lint !e747
    //3D��ť���û�
    pVideoPane->SetToolbarEnableStatus(IVS_TOOLBAR_PTZ3D,1);//lint !e747

    m_ulOperatePaneID++;
}//lint !e1764

LONG CVideoPaneMgr::SetVoiceTalkVolume(LONG nCoefficient) const
{
    return IVS_SUCCEED;
}//lint !e715

// ������·��Ƶ����
LONG CVideoPaneMgr::SetVoiceVolume(LONG ulVolumeValue) const
{
    return IVS_SUCCEED;
}//lint !e715

// Qualifier: ���þ���״̬
ULONG CVideoPaneMgr::SetWarnState( ULONG ulWndID, ULONG ulWarnState, ULONG ulWarnSeconds )
{
    CVideoPane *pVideoPane = this->GetWndPointerByID(ulWndID);
    CHECK_POINTER(pVideoPane,IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pVideoPane->GetVideoTitleBar(),IVS_FAIL);

    return pVideoPane->SetWarnState(ulWarnState, ulWarnSeconds);
}

// ������·��Ƶ����
ULONG CVideoPaneMgr::SetWaveAudio(ULONG ulWndID, ULONG ulVolumeValue)
{
    CVideoPane* pVideoPane = GetWndPointerByID(ulWndID);
    CHECK_POINTER(pVideoPane,IVS_OPERATE_MEMORY_ERROR);

    if (!pVideoPane->GetPlayState() || !pVideoPane->IsSoundOn())
    {
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "","Set wave audio error,video pane is not playing video or audio.");
        //ֻ�ܶԵ�ǰ���ڲ��ŵĻ�����������
        return IVS_OPERATE_MEMORY_ERROR;
    }

    ULONG ulRet = IVS_SUCCEED;//CIVS_PlayerAdapter::instance()->SetWaveAudio(pVideoPane->GetVideoWnd(), ulVolumeValue);
    pVideoPane->SetAudioVolume(ulVolumeValue);

    return ulRet;
}

// Qualifier: ���ô��񲼾�
LONG CVideoPaneMgr::SetWndLayout( CRect rect, ULONG ulLayoutType )
{
    if (!LayoutMgr.CheckLayoutType(ulLayoutType))
    {
        return IVS_PARA_INVALID;
    }
    if (!LayoutMgr.isTVWallLayout(ulLayoutType))
	{//�ǵ���ǽ���֣�������ID����Ϊ0
		m_ulTVWallID = 0;
	}
    // ɾ���������ƶԻ���
    DeleteSoundCtrl();
    CHECK_POINTER(m_pParentWnd,IVS_FAIL);

    if (IVS_SUCCEED != SetParameter(ulLayoutType))
    {
        BP_RUN_LOG_ERR(IVS_FAIL,"et parameter error", "slayout type: %lu ", ulLayoutType);
        return IVS_FAIL;
    }

	HideVideoPane();
    m_pParentWnd->MoveWindow(rect, TRUE);
	m_pParentWnd->Invalidate(TRUE);
	m_pParentWnd->UpdateWindow();

    //ȫ���Ļ�,��Ҫ���⴦��˫�������
    if (FULL_SCREEN == m_ulIsFullScreen)
    {
        rect.bottom = rect.bottom - rect.top;
        rect.right = rect.right - rect.left;
        rect.top = 0;
        rect.left = 0;
    }

	// ������ַ����ı䣬�˳��ֲ��Ŵ�
	if (m_MagnifyLayOut != ulLayoutType)
	{
		// �˳��ֲ��Ŵ�״̬��add by z00193167
		CancleAllMagnifyWnd();
	}
	// ��1�����л�����ʱ��Ҫ���ֲ��Ŵ�ť����
	if (NULL != m_pVideoPane_1_1)
	{
		m_pVideoPane_1_1->SetToolbarEnableStatus(IVS_TOOLBAR_ZOOM,1);//lint !e747
		m_pVideoPane_1_1 = NULL;
	}

    if (LayoutMgr.isRegulationLayOut(m_windLayOut))
    {
        ULONG ulLayOutNum = m_windLayOut/10;
        SetLayoutAsAverage(rect, ulLayOutNum);
		//����
		CAutoLock __Locker(m_csVideoPaneLock);

		// 1���񲼾֣��ֲ��Ŵ�ťֽ��
		if (WIND_LAYOUT_1_1 == m_windLayOut && WND_TYPE_ANALYSIS != GetWndType())
		{
			MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin();
			CVideoPane* pVideoPane = dynamic_cast<CVideoPane *>(iter->second); //lint !e611
			if (NULL != pVideoPane)
			{
				pVideoPane->SetToolbarEnableStatus(IVS_TOOLBAR_ZOOM,0);//lint !e747
				m_pVideoPane_1_1 = pVideoPane;
			}
		}

        return IVS_SUCCEED;
    }else if (LayoutMgr.isLadderLayout(m_windLayOut))
    {
        ULONG ulLayOutNum = m_windLayOut/10;
        return static_cast<LONG>(LayoutMgr.SetLadderLayout(rect,ulLayOutNum,this));
    }else if (LayoutMgr.isAnomalousLayOut(m_windLayOut))
    {
       return  static_cast<LONG>(LayoutMgr.SetAnomalousLayout(rect,m_windLayOut,this));
	}else if (LayoutMgr.isTVWallLayout(m_windLayOut))
	{
		// ��ȡ����ǽ��������
		TVWALL_PANE_LIST paneList;
		RECTLIST rectList;
		IVS_UINT32 uiXNum = 0;
		IVS_UINT32 uiYNum = 0;
		IVS_INT32 iRet = COCXTVWallXMLProcess::GetInst().GetTVWallPaneList(m_ulTVWallID, paneList, uiXNum, uiYNum);
		if(IVS_SUCCEED!=iRet)
		{
			return iRet;
		}
		m_SplitTo1RectList.clear();
		m_SplitTo4RectList.clear();
		m_SplitTo9RectList.clear();
		m_SplitTo16RectList.clear();
		m_MapID1Rect.clear();
		m_MapID4Rect.clear();
		m_MapID9Rect.clear();
		m_MapID16Rect.clear();
		for(TVWALL_PANE_LIST::iterator ite=paneList.begin();ite!=paneList.end();ite++)
		{
			
			rectList.push_back((*ite).rect);
			IVS_UINT32 uiNum = ite->uiChannelNum;
			if(uiNum <= 1)
			{
				m_SplitTo1RectList.push_back((*ite).rect);
			}
			else if(uiNum > 1 && uiNum <= 4)//ͨ����
			{
				m_SplitTo4RectList.push_back((*ite).rect);
			}
			else if(uiNum > 4 && uiNum <= 9)
			{
				m_SplitTo9RectList.push_back((*ite).rect);
			}
			else if(uiNum > 9 && uiNum <= 16)
			{
				m_SplitTo16RectList.push_back((*ite).rect);
			}
		}
		iRet = SetCustomLayout(rect,rectList,uiXNum,uiYNum);
		if(IVS_SUCCEED == iRet)
		{
			//���һ����ִ���
			iRet = SplitTVWallWnd();	

			UpdateTVWallPaneStatus();
            UpdateTVWallOnlineStatus();
		}
		return iRet;
    }else {
        CIVS_OCXPlayerCtrl * pOcx = dynamic_cast<CIVS_OCXPlayerCtrl*>(m_pOcxCtrl);
        CHECK_POINTER(pOcx,IVS_OPERATE_MEMORY_ERROR);
        IVS_UINT32 uX = 0;
        IVS_UINT32 uY = 0;
        std::string strXML;
        if(pOcx->m_pLocalConfig.GetLayoutXML(ulLayoutType,strXML)!= IVS_SUCCEED)
        {
            return IVS_PARA_INVALID;
        }
        RECTLIST Rect_list;

        (void)pOcx->m_pLocalConfig.ParseLayoutXML2List(strXML.c_str(), Rect_list,uX,uY);
        if (Rect_list.empty())
        {
            return IVS_PARA_INVALID;
        }
        return SetCustomLayout(rect,Rect_list,uX,uY);
    }
}

void CVideoPaneMgr::UpdateTVWallOnlineStatus()
{
    TVWALL_PANE_LIST paneList;
    RECTLIST rectList;
    IVS_UINT32 uiXNum = 0;
    IVS_UINT32 uiYNum = 0;
    IVS_INT32 iRet = COCXTVWallXMLProcess::GetInst().GetTVWallPaneList(m_ulTVWallID, paneList, uiXNum, uiYNum);
    if(IVS_SUCCEED!=iRet)
    {
        return ;
    }
    std::list<CUMW_NOTIFY_REMOTE_DECODER_STATUS> decoderIDList;
    COCXTVWallMgr::GetOnlineDecoderIDList(((CIVS_OCXPlayerCtrl*)GetOcxCtrl())->GetSDKSessionID(), decoderIDList);

    for(TVWALL_PANE_LIST::iterator ite=paneList.begin();ite!=paneList.end();ite++)
    {
        IVS_UINT32 uiNum = ite->uiChannelNum;
        TVWall_VPANEID vPaneID = {0};
        vPaneID.ulParentWndID = ite->ulWndID;

        TVWALL_PANE_CHANNEL_LIST::iterator ChannelIter = ite->listChanel.begin();
        for (IVS_UINT32 iNum=1; iNum <=uiNum; iNum++)
        {
            unsigned long ulChannelID = 0;
            if (ite->listChanel.end() != ChannelIter)
            {
                ulChannelID = ChannelIter->ulChannelID;
                ++ChannelIter;
            }

            vPaneID.ulChildWndID = iNum;

            ULONG realPaneID = GetRealIDByVirtualID(vPaneID);
            CVideoPane* pVideoPane= GetWndPointerByID(realPaneID);
            if (NULL == pVideoPane)
            {
                continue;
            }
            pVideoPane->SetTVWallStatusBarStatus(TVWALL_BAR_STAUS_OFFLINE);
            for (std::list<CUMW_NOTIFY_REMOTE_DECODER_STATUS>::iterator DecoderIt = decoderIDList.begin();
                DecoderIt != decoderIDList.end(); ++DecoderIt)
            {
                if ( strncmp(ite->szDecoderID, DecoderIt->szDecoderID, CUMW_DECODER_ID_LEN) == 0 )
                {
                    for (ULONG i=0; i < DecoderIt->ulChannelNum; i++)
                    {
                        if ( ulChannelID == DecoderIt->stChannelInfoArray[i].ulChannelID )
                        {
                            pVideoPane->SetTVWallStatusBarStatus(TVWALL_BAR_STAUS_ONLINE);
                            break;
                        }
                    }
                }
            }
        }
    }
}
void CVideoPaneMgr::UpdateTVWallPaneStatus()
{
	TVWALL_PANE_LIST paneList;
	RECTLIST rectList;
	IVS_UINT32 uiXNum = 0;
	IVS_UINT32 uiYNum = 0;
	IVS_INT32 iRet = COCXTVWallXMLProcess::GetInst().GetTVWallPaneList(m_ulTVWallID, paneList, uiXNum, uiYNum);
	if(IVS_SUCCEED!=iRet)
	{
		return ;
	}
    
	//ɾ��δ�󶨵���ǽͨ��״̬�Ĵ����ͼ��
	for(TVWALL_PANE_LIST::iterator ite=paneList.begin();ite!=paneList.end();ite++)
	{
		IVS_UINT32 uiNum = ite->uiChannelNum;
		TVWall_VPANEID vPaneID = {0};
		vPaneID.ulParentWndID = ite->ulWndID;

		IVS_UINT32 MAX_NUM = 0;
		if(uiNum <= 1)
		{
			MAX_NUM = 1;
		}
		else if(uiNum > 1 && uiNum <= 4)
		{
			MAX_NUM = 4;
		}
		else if(uiNum > 4 && uiNum <= 9)
		{
			MAX_NUM = 9;
		}
		else if(uiNum > 9 && uiNum <= 16)
		{
			MAX_NUM = 16;
		}
		for(IVS_UINT32 i=uiNum+1;i<=MAX_NUM;i++)
		{
			vPaneID.ulChildWndID = i;
			ULONG realPaneID = GetRealIDByVirtualID(vPaneID);
			CVideoPane* pVideoPane= GetWndPointerByID(realPaneID);
			if(pVideoPane)
			{
				pVideoPane->SetTVWallStatusBarStatus(TVWALL_BAR_STAUS_HIDE);
				//�澯��ť�û�
				pVideoPane->SetAlarmButtonStatus((unsigned long long)0);
			}
		}
	}
}
LONG CVideoPaneMgr::SplitTVWallWnd()
{	
	//���Ϊ4������
	m_MapWindowPaneID.clear();
	TVWall_VPANEID wndIDpair = {0};
	for(PaneIDRectMap::iterator it =  m_MapID1Rect.begin();it!=m_MapID1Rect.end();it++)
	{
		ULONG paneID = it->first;
		//CRect rc = it->second;
		/*
		if(!ResetVideoPanePositon(paneID,rc))
		{
			SetVideoPanePosition(rc);
		}
		*/// һ������ʱ����Ҫ�ƶ�λ��
		wndIDpair.ulParentWndID = paneID;
		wndIDpair.ulChildWndID = 1;
		(void)m_MapWindowPaneID.insert(std::make_pair(paneID, wndIDpair));
	}
	for(PaneIDRectMap::iterator it =  m_MapID4Rect.begin();it!=m_MapID4Rect.end();it++)
	{
		ULONG paneID = it->first;
		CRect rc = it->second;
		
		CRect rc1(rc.left,rc.top,rc.left + rc.Width()/2,rc.top + rc.Height()/2);
		CRect rc2(rc1.right + 1,rc.top,rc.right,rc1.bottom);
		CRect rc3(rc.left,rc1.bottom+1,rc1.right,rc.bottom);
		CRect rc4(rc3.right+1,rc3.top,rc2.right,rc.bottom); 
		if(!ResetVideoPanePositon(paneID,rc1))
		{
			SetVideoPanePosition(rc1);
		}
		wndIDpair.ulParentWndID = paneID;
		wndIDpair.ulChildWndID = 1;		
		(void)m_MapWindowPaneID.insert(std::make_pair(paneID, wndIDpair));
		//�����Ĵ�������Ӧ��λ��

        // add by wanglei 00165153:2013.7.26 DTS2013071507953
        // ��SDK_���һ�������һ�����֣���������Ѳ������Ѳ���񲥷Ŵ������
        // ԭ�ȴ����Ų��� ��1�� ��2 5 6 7�� ��3�� ��4��
        // ���ڸĳɡ�1�� ��2 3 4 5�� ��6�� ��7��
        // ԭ���ǵ���ǽ��Ѳ���ñ�ŵ������ʣ���ʽ����
        m_ulOperatePaneID = paneID + 1;

		SetVideoPanePosition(rc2);
		wndIDpair.ulChildWndID = 2;	
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));

		SetVideoPanePosition(rc3);
		wndIDpair.ulChildWndID = 3;	
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc4);
		wndIDpair.ulChildWndID = 4;	
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
	}

	//���Ϊ9������
	for(PaneIDRectMap::iterator it =  m_MapID9Rect.begin();it!=m_MapID9Rect.end();it++)
	{
		ULONG paneID = it->first;
		CRect rc = it->second;
		int WIDTH = rc.Width()/3;
		int HEGHT = rc.Height()/3;
		CRect rc1(rc.left,rc.top,rc.left+WIDTH,rc.top+HEGHT);
		CRect rc2(rc1.right+1,rc.top,rc1.right+1+WIDTH,rc1.bottom);
		CRect rc3(rc2.right+1,rc.top,rc.right,rc1.bottom);

		CRect rc4(rc.left,rc1.bottom+1,rc1.right,rc1.bottom+1+HEGHT);
		CRect rc5(rc2.left,rc2.bottom+1,rc2.right,rc4.bottom);
		CRect rc6(rc3.left,rc3.bottom+1,rc.right,rc4.bottom);

		CRect rc7(rc.left,rc4.bottom+1,rc4.right,rc.bottom);
		CRect rc8(rc5.left,rc7.top,rc5.right,rc.bottom);
		CRect rc9(rc6.left,rc7.top,rc.right,rc.bottom); 

		if(!ResetVideoPanePositon(paneID,rc1))
		{
			SetVideoPanePosition(rc1);
		}		
		wndIDpair.ulParentWndID = paneID;
		wndIDpair.ulChildWndID = 1;		
		(void)m_MapWindowPaneID.insert(std::make_pair(paneID, wndIDpair));
		
		//�����Ĵ�������Ӧ��λ��

        // add by wanglei 00165153:2013.7.26 DTS2013071507953
        // ��SDK_���һ�������һ�����֣���������Ѳ������Ѳ���񲥷Ŵ������
        // ԭ�ȴ����Ų��� ��1�� ��2 5 6 7�� ��3�� ��4��
        // ���ڸĳɡ�1�� ��2 3 4 5�� ��6�� ��7��
        // ԭ���ǵ���ǽ��Ѳ���ñ�ŵ������ʣ���ʽ����
        m_ulOperatePaneID = paneID + 1;

		SetVideoPanePosition(rc2);
		wndIDpair.ulChildWndID = 2;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc3);
		wndIDpair.ulChildWndID = 3;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc4);
		wndIDpair.ulChildWndID = 4;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc5);
		wndIDpair.ulChildWndID = 5;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc6);
		wndIDpair.ulChildWndID = 6;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc7);
		wndIDpair.ulChildWndID = 7;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc8);
		wndIDpair.ulChildWndID = 8;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc9);
		wndIDpair.ulChildWndID = 9;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
	}

	//���Ϊ16������
	for(PaneIDRectMap::iterator it =  m_MapID16Rect.begin();it!=m_MapID16Rect.end();it++)
	{
		ULONG paneID = it->first;
		CRect rc = it->second;

		int WIDTH = rc.Width()/4;
		int HEGHT = rc.Height()/4;
		CRect rc1(rc.left,     rc.top, rc.left+WIDTH,     rc.top+HEGHT);
		CRect rc2(rc1.right+1, rc.top, rc1.right+1+WIDTH, rc1.bottom);
		CRect rc3(rc2.right+1, rc.top, rc2.right+1+WIDTH, rc1.bottom);
		CRect rc4(rc3.right+1, rc.top, rc.right, rc1.bottom);

		CRect rc5(rc1.left,rc1.bottom+1,rc1.right,rc1.bottom+1+HEGHT);
		CRect rc6(rc2.left,rc5.top,rc2.right,rc5.bottom);
		CRect rc7(rc3.left,rc5.top,rc3.right,rc5.bottom);
		CRect rc8(rc4.left,rc5.top,rc4.right,rc5.bottom);

		CRect  rc9(rc1.left,rc5.bottom+1,rc1.right,rc5.bottom+1+HEGHT); 
		CRect rc10(rc2.left,rc9.top,rc2.right,rc9.bottom); 
		CRect rc11(rc3.left,rc9.top,rc3.right,rc9.bottom); 
		CRect rc12(rc4.left,rc9.top,rc4.right,rc9.bottom); 

		CRect rc13(rc1.left,rc9.bottom+1,rc1.right,rc.bottom); 
		CRect rc14(rc2.left,rc13.top,rc2.right,rc.bottom); 
		CRect rc15(rc3.left,rc13.top,rc3.right,rc.bottom); 
		CRect rc16(rc4.left,rc13.top,rc4.right,rc.bottom); 


		//���õ�һ���Ӵ���λ��
		if(!ResetVideoPanePositon(paneID,rc1))
		{
			SetVideoPanePosition(rc1);
		}
		wndIDpair.ulParentWndID = paneID;
		wndIDpair.ulChildWndID = 1;		
		(void)m_MapWindowPaneID.insert(std::make_pair(paneID, wndIDpair));

		//�����Ĵ�������Ӧ��λ��

        // add by wanglei 00165153:2013.7.26 DTS2013071507953
        // ��SDK_���һ�������һ�����֣���������Ѳ������Ѳ���񲥷Ŵ������
        // ԭ�ȴ����Ų��� ��1�� ��2 5 6 7�� ��3�� ��4��
        // ���ڸĳɡ�1�� ��2 3 4 5�� ��6�� ��7��
        // ԭ���ǵ���ǽ��Ѳ���ñ�ŵ������ʣ���ʽ����
        m_ulOperatePaneID = paneID + 1;

		SetVideoPanePosition(rc2);
		wndIDpair.ulChildWndID = 2;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc3);
		wndIDpair.ulChildWndID = 3;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc4);
		wndIDpair.ulChildWndID = 4;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc5);
		wndIDpair.ulChildWndID = 5;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc6);
		wndIDpair.ulChildWndID = 6;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc7);
		wndIDpair.ulChildWndID = 7;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc8);
		wndIDpair.ulChildWndID = 8;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc9);
		wndIDpair.ulChildWndID = 9;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc10);
		wndIDpair.ulChildWndID = 10;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc11);
		wndIDpair.ulChildWndID = 11;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc12);
		wndIDpair.ulChildWndID = 12;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc13);
		wndIDpair.ulChildWndID = 13;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc14);
		wndIDpair.ulChildWndID = 14;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc15);
		wndIDpair.ulChildWndID = 15;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
		SetVideoPanePosition(rc16);
		wndIDpair.ulChildWndID = 16;		
		(void)m_MapWindowPaneID.insert(std::make_pair(m_ulOperatePaneID-1, wndIDpair));
	}

	return IVS_SUCCEED;
}
void CVideoPaneMgr::AddSplitRectMap(const CVideoPaneMgr::SPLIT_WND_TYPE& emType,const CRect &rc)
{
	switch (emType)
	{
	case SPLIT_1:
		{
			(void)m_MapID1Rect.insert(std::make_pair(m_ulOperatePaneID-1,rc));
			break;
		}
	case SPLIT_4:
		{
			(void)m_MapID4Rect.insert(std::make_pair(m_ulOperatePaneID-1,rc));

            // add by wanglei 00165153:2013.7.26 DTS2013071507953
            // ��SDK_���һ�������һ�����֣���������Ѳ������Ѳ���񲥷Ŵ������
            // ԭ�ȴ����Ų��� ��1�� ��2 5 6 7�� ��3�� ��4��
            // ���ڸĳɡ�1�� ��2 3 4 5�� ��6�� ��7��
            // ԭ���ǵ���ǽ��Ѳ���ñ�ŵ������ʣ���ʽ����
            m_ulOperatePaneID += 3;
			break;
		}
	case SPLIT_9:
		{
			(void)m_MapID9Rect.insert(std::make_pair(m_ulOperatePaneID-1,rc));

            // add by wanglei 00165153:2013.7.26 DTS2013071507953
            // ��SDK_���һ�������һ�����֣���������Ѳ������Ѳ���񲥷Ŵ������
            // ԭ�ȴ����Ų��� ��1�� ��2 5 6 7�� ��3�� ��4��
            // ���ڸĳɡ�1�� ��2 3 4 5�� ��6�� ��7��
            // ԭ���ǵ���ǽ��Ѳ���ñ�ŵ������ʣ���ʽ����
            m_ulOperatePaneID += 8;
			break;
		}
	case SPLIT_16:
		{
			(void)m_MapID16Rect.insert(std::make_pair(m_ulOperatePaneID-1,rc));

            // add by wanglei 00165153:2013.7.26 DTS2013071507953
            // ��SDK_���һ�������һ�����֣���������Ѳ������Ѳ���񲥷Ŵ������
            // ԭ�ȴ����Ų��� ��1�� ��2 5 6 7�� ��3�� ��4��
            // ���ڸĳɡ�1�� ��2 3 4 5�� ��6�� ��7��
            // ԭ���ǵ���ǽ��Ѳ���ñ�ŵ������ʣ���ʽ����
            m_ulOperatePaneID += 15;
			break;
		}
	default:
		{
			break;
		}
	}

}
bool CVideoPaneMgr::ResetVideoPanePositon(const ULONG& ulPaneID,const CRect& rc)
{
	//��һ���Ӵ����ƶ�һ��λ��
	CVideoPane * pVideoPane = nullptr;
	MAP_VIDEOPANE_ITER iter = m_videoPaneMap.find(ulPaneID);
	if (iter != m_videoPaneMap.end())
	{
		pVideoPane = iter->second;
		pVideoPane->SetPaneRectFull(rc);
		pVideoPane->SetBackGroundBitmap(rc);
		pVideoPane->SetPaneRect(rc);
		pVideoPane->MoveWindow(&rc);		
		//����ʽ���ְ�ť�Ƿ��û�
		if (m_ulShowWndNum  >= LADDERLAYOUTMINWINDOWSNUM && m_ulShowWndNum <= LADDERLAYOUTMAXWINDOWSNUM)
		{
			pVideoPane->SetToolbarEnableStatus(IVS_TOOLBAR_WINDOW_MAIN,1);//lint !e747
		}
		else
		{
			pVideoPane->SetToolbarEnableStatus(IVS_TOOLBAR_WINDOW_MAIN,0);//lint !e747
		}
		//���ӵ�ͼ��ť���û�
		pVideoPane->SetToolbarEnableStatus(IVS_TOOLBAR_OPEN_MAP,1);//lint !e747
        //3D��ť���û�
        pVideoPane->SetToolbarEnableStatus(IVS_TOOLBAR_PTZ3D,1);//lint !e747
		pVideoPane->ResetBarPosition();
		pVideoPane->ShowWindow(SW_SHOW);
		
		return true;
	}
	else
	{
		return false;
	}
}
void CVideoPaneMgr::GetRealTVwallPaneID(const TVWall_VPANEID_LIST& listPaneID,std::vector<ULONG>& vecRealPaneID)
{
	for(TVWall_VPANEID_LIST::const_iterator iter  = listPaneID.begin();iter != listPaneID.end();iter++)
	{
		for(WNDID_MAP::iterator it = m_MapWindowPaneID.begin();it!=m_MapWindowPaneID.end();it++)
		{
			if(it->second.ulChildWndID == iter->ulChildWndID && it->second.ulParentWndID == iter->ulParentWndID)
			{
				vecRealPaneID.push_back(it->first);
			}
		}
	}
}
void CVideoPaneMgr::GetRealTVwallPaneIDByParentID(const std::vector<ULONG>& vecPaneID,std::vector<ULONG>& vecRealPaneID)
{
	IVS_UINT32 uSize = vecPaneID.size();
	for(IVS_UINT32 i= 0;i<uSize;i++)
	{
		for(WNDID_MAP::iterator iter = m_MapWindowPaneID.begin();iter!=m_MapWindowPaneID.end();iter++)
		{
			if(vecPaneID.at(i) == iter->second.ulParentWndID)
			{
				vecRealPaneID.push_back(iter->first);
			}
		}
	}
}
bool CVideoPaneMgr::GetVirtualIDByRealID(const ULONG& ulPaneID,TVWall_VPANEID& vID)
{
	WNDID_MAP::iterator iter = m_MapWindowPaneID.find(ulPaneID);
	if(iter != m_MapWindowPaneID.end())
	{
		vID = iter->second;
		return true;
	}
	else
	{
		return false;
	}

	
}

ULONG CVideoPaneMgr::GetRealIDByVirtualID(const TVWall_VPANEID& stPaneID)
{
	for(WNDID_MAP::iterator ite = m_MapWindowPaneID.begin();ite!=m_MapWindowPaneID.end();ite++)
	{
		if(stPaneID.ulParentWndID == ite->second.ulParentWndID
			&& stPaneID.ulChildWndID == ite->second.ulChildWndID)
		{
			return ite->first;
		}
	}
	return 0;
}

// ���ݵ���ǽ������ò���
LONG CVideoPaneMgr::SetTVWallWndLayout(const CRect& rect, IVS_ULONG ulTVWallID)
{

	if(0==ulTVWallID)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Set TVWall Layout", "Failed, TVWallID=0");
		return IVS_PARA_INVALID;
	}
	m_ulTVWallID = ulTVWallID;
	return SetWndLayout(rect, WIND_LAYOUT_TVW);

}

ULONG CVideoPaneMgr::StartVoiceTalkback(const TCHAR* pVoiceCode, ULONG ulDecoderType, ULONG ulStreamType)
{
    //��ʼ����ǰ�ر����е���·����
    CHECK_POINTER(m_pOcxCtrl,IVS_FAIL);
    CIVS_OCXPlayerCtrl *pRealPlayCtrl =dynamic_cast<CIVS_OCXPlayerCtrl*>(m_pOcxCtrl)->GetOcxPlayerCtrl(WND_TYPE_REALTIME);
    if (nullptr != pRealPlayCtrl)
    {
        CVideoPaneMgr *pVideoPaneMgr = pRealPlayCtrl->GetVideoPaneMgr();
        if(nullptr != pVideoPaneMgr)
        {
            pVideoPaneMgr->StopSoundCtrlForOther(true);
        }
    }

    CVideoPane* pActiveVideoPane = GetActiveWndPointer();


	//TO
    ULONG ulRet = IVS_SUCCEED;//CIVS_PlayerAdapter::instance()->StartVoiceTalkback(pVoiceCode, ulDecoderType, ulStreamType/*, ulProtocolType*/);
    if (nullptr != pActiveVideoPane)
    {
        // ����ʵ���������������Խ���ť״̬
        CTranVideoRealtimeBar* pVideoRealtimeBar = pActiveVideoPane->GetVideoRealtimeBar();
        if (nullptr == pVideoRealtimeBar)
        {
            BP_RUN_LOG_ERR(IVS_PARA_INVALID, "","Video pane manager start voice talk back fail, Get VideoPane fail.");
            return IVS_PARA_INVALID;
        }
        // TODO :        pVideoRealtimeBar->SetTalkbackPic(BUTTON_CLICKED);
        pActiveVideoPane->SetRealtimeTalkbackState(BOTTOMBAR_TALKBACK_OPEN);

        // ����Talkback״̬����������ͼ��
        // TODO :        if(nullptr != pVideoRealtimeBar->GetTalkbackSound())
        {
            // TODO :            pVideoRealtimeBar->GetTalkbackSound()->SetTalkbackSilencePic(FALSE);
        }

        return IVS_SUCCEED;
    }

    return ulRet;
}//lint !e715

ULONG CVideoPaneMgr::StopAllRealPlay(void)
{
    CHECK_POINTER(m_pOcxCtrl,IVS_FAIL);
    CAutoLock __Locker(m_csVideoPaneLock);

    ULONG ulRet = 0;

    // map����Ĵ���,�ж��Ƿ񲥷ţ��ǵĻ�Ҫ�Ȱ���ֹͣ
    for (MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin(); iter != m_videoPaneMap.end();)
    {
        CVideoPane *pVideoPane = iter->second;
        if (nullptr == pVideoPane)
        {
            continue;
        }
        if ((VIDEO_PLAY_STATE_NOT_PLAYING != pVideoPane->GetPlayState()))
        {
            // ֹͣ����
			CIVS_OCXPlayerCtrl* pCtrl = dynamic_cast<CIVS_OCXPlayerCtrl*>(m_pOcxCtrl);
			if (NULL == pCtrl)
			{
				continue;
			}
            ulRet = pCtrl->StopRealPlayByPane(iter->second);
            if (ulRet != IVS_SUCCEED)
            {
                BP_RUN_LOG_ERR((int)ulRet, "stopallRealPlay stopRealPlayByPane", "NA");
            }
        }
        iter++;
    }
    return IVS_SUCCEED;
}

void CVideoPaneMgr::NotifyOtherSoundOpen(ULONG ulWinID, bool isOpen)
{
    CAutoLock __Locker(m_csVideoPaneLock);
    ULONG i = 0;
    char szCamera[IVS_DEV_CODE_LEN + 1] = {0};
    MAP_VIDEOPANE_ITER iter = m_videoPaneMap.find(ulWinID);
    if(iter != m_videoPaneMap.end())
    {
        CVideoPane *pVideoPane = iter->second;
        if (nullptr == pVideoPane)
        {
            return;
        }

        if ( iter->first == ulWinID)
        {
            (void)CToolsHelp::Strncpy(szCamera, sizeof(szCamera), pVideoPane->GetCamReal(), IVS_DEV_CODE_LEN);
        }
    }
    iter = m_videoPaneMap.begin();
    while((iter != m_videoPaneMap.end()) && (i<m_ulShowWndNum))
    {
        CVideoPane *pVideoPane = iter->second;
        if (nullptr == pVideoPane)
        {
            ++iter;
            ++i;
            continue;
        }
        
        if (pVideoPane->VerifySameStream(szCamera, STREAM_TYPE_UNSPECIFIED))
        {
            CTranVideoRealtimeBar* pBar = pVideoPane->GetVideoRealtimeBar();
            if (nullptr != pBar)
            {
                pBar->PostMessage(WM_SETSOUND_STATE, 0, isOpen?0:1);
            }
            
        }
        ++iter;
        ++i;
    } //end while((iter != m_videoPaneMap.end()) && (i<m_ulShowWndNum))
}

void CVideoPaneMgr::StopSoundCtrlForOther(bool bCloseAll)
{
    if (WND_TYPE_ANALYSIS == GetWndType())
    {
        return;
    }

    CAutoLock __Locker(m_csVideoPaneLock);
    ULONG i = 0;
    MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin();
    while((iter != m_videoPaneMap.end()) && (i<m_ulShowWndNum))
    {
        if(!bCloseAll)
        {
            // ����ǻ��������
            if (GetActivePaneID() == iter->first)
            {
                ++iter;
                ++i;
                continue;
            }
        }

        CVideoPane *pVideoPane = iter->second;
        if (nullptr == pVideoPane)
        {
            ++iter;
            ++i;
            continue;
        }

        if((VIDEO_PLAY_STATE_NOT_PLAYING != pVideoPane->GetPlayState())
            && (nullptr != pVideoPane->GetVideoRealtimeBar()))
        {
            if(BOTTOMBAR_SOUND_OPEN == pVideoPane->GetRealtimeSoundState())
            {
                pVideoPane->GetVideoRealtimeBar()->StopSound();
            }
        }

        pVideoPane->SetLastSoundState(false);

        ++iter;
        ++i;
    } //end while((iter != m_videoPaneMap.end()) && (i<m_ulShowWndNum))
}

// �ر������Խ�,���Ի������
void CVideoPaneMgr::StopTalkbackSoundForOther(void)
{
    if (WND_TYPE_ANALYSIS == GetWndType())
    {
        return;
    }

    CAutoLock __Locker(m_csVideoPaneLock);
    ULONG i = 0;
    MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin();

    while((iter != m_videoPaneMap.end()) && (i<m_ulShowWndNum))
    {
        // ����ǻ��������
        if (GetActivePaneID() == iter->first)
        {
            ++iter;
            ++i;
            continue;
        }
        CVideoPane *pVideoPane = iter->second;
        if (nullptr == pVideoPane)
        {
            continue;
        }

        if(nullptr != pVideoPane->GetVideoRealtimeBar())
        {
            if(BOTTOMBAR_SOUND_OPEN == pVideoPane->GetRealtimeTalkbackState())
            {
                (void)pVideoPane->GetVideoRealtimeBar()->StopTalkbackSound();
            }
        }

        ++iter;
        ++i;
    } //end while((iter != m_videoPaneMap.end()) && (nY<m_ulShowWndNum))
}

ULONG CVideoPaneMgr::StopVoiceTalkback(void) const
{
    ULONG ulRet = IVS_SUCCEED;//CIVS_PlayerAdapter::instance()->StopVoiceTalkback();

    //CVideoPane* pActiveVideoPane = GetActiveWndPointer();
    CVideoPane* pActiveVideoPane = CIVS_OCXPlayerCtrl::m_pVoiceTalkbackVideoPane;
    CHECK_POINTER(pActiveVideoPane,IVS_FAIL);

    // ����ʵ���������������Խ���ť״̬
    CTranVideoRealtimeBar * pVideoRealtimeBar = pActiveVideoPane->GetVideoRealtimeBar();
    CHECK_POINTER(pVideoRealtimeBar,IVS_PARA_INVALID);

    pActiveVideoPane->SetRealtimeTalkbackState(BOTTOMBAR_TALKBACK_CLOSE);
    //pVideoRealtimeBar->SetTalkbackPic(BUTTON_NORMAL);

    // ����Talkback״̬����������ͼ��
    //if(nullptr != pVideoRealtimeBar->GetTalkbackSound())
    {
        // TODO : pVideoRealtimeBar->GetTalkbackSound()->SetTalkbackSilencePic(TRUE);
    }
    return ulRet;
}

// Qualifier: �Զ��岼��
LONG CVideoPaneMgr::SetCustomLayout( CRect rect, RECTLIST &videoRects,const IVS_UINT32 uX,const IVS_UINT32 uY )
{
    IVS_INT32 _uX = static_cast<IVS_INT32>(uX);
    IVS_INT32 _uY = static_cast<IVS_INT32>(uY);
	CParentWnd* pParentWnd = dynamic_cast<CParentWnd*>(m_pParentWnd);
    CHECK_POINTER(pParentWnd,IVS_FAIL);
    // ɾ���������ƶԻ���
    DeleteSoundCtrl();

    // �Ѹ����ڵľ���rect���
    pParentWnd->m_ListCrect.clear();
    pParentWnd->Invalidate(TRUE);

    // ��λ�Ŵ���λ
    m_ulIsMagnify = MINIFY;

    m_ulOperatePaneID = 1;

    HideVideoPane();
    pParentWnd->MoveWindow(rect, TRUE);

    //ȫ���Ļ�,��Ҫ���⴦��˫�������
    if (FULL_SCREEN == m_ulIsFullScreen)
    {
        rect.bottom = rect.bottom - rect.top;
        rect.right = rect.right - rect.left;
        rect.top = 0;
        rect.left = 0;
    }

    // ���¹��������ڸ澯ͼ��״̬
   // RefreshAllAlarmWnd(ulLayoutType);

    if (videoRects.empty())
    {
        return IVS_FAIL;
    }

    //Rect ������֤
    FunctionObjectSumRect _SumRectFO =  for_each(videoRects.begin(),videoRects.end(), FunctionObjectSumRect());

    if (_SumRectFO.value() !=  _uX*_uY )
    {
        return IVS_FAIL;
    }

    m_ulShowWndNum  = videoRects.size() + m_SplitTo4RectList.size()*3 + m_SplitTo9RectList.size()*8 + m_SplitTo16RectList.size()*15;

    // �����С����
    LONG unitWith =  rect.Width() /_uX ;
    LONG unitHeight =  rect.Height() / _uY;

	//����
	LONG remainderX = rect.Width() % _uX;
	LONG remainderY = rect.Height() % _uY;

	CPoint _TopLefPoint  =  rect.TopLeft();
	CPoint _BottomRightPoint;
	CRect _Rect;

	for(RECTLISTITER iter=videoRects.begin();iter!=videoRects.end();iter++)
	{
		CRect &itRect = *iter;

        //������ʱ��ǰ���ÿһ����������һ�����ص㣬ֱ��������������
		LONG lLeftCompensation      = (itRect.left      >= remainderX) ? remainderX : itRect.left;
		LONG lRightCompensation     = (itRect.right     >= remainderX) ? remainderX : itRect.right;
		LONG lTopCompensation       = (itRect.top	    >= remainderY) ? remainderY : itRect.top;
		LONG lBottomCompensation    = (itRect.bottom    >= remainderY) ? remainderY : itRect.bottom;

		_TopLefPoint.x      = (rect.left + itRect.left   * unitWith    + lLeftCompensation);
        _TopLefPoint.y      = (rect.top  + itRect.top    * unitHeight  + lTopCompensation);

		_BottomRightPoint.x = (rect.left + itRect.right  * unitWith    + lRightCompensation);
        _BottomRightPoint.y = (rect.top  + itRect.bottom * unitHeight  + lBottomCompensation);

		_Rect.SetRect(_TopLefPoint,_BottomRightPoint);
		SetVideoPanePosition(_Rect);

		//���ǵ���ǽ����Ҫ���
		if(GetCurTVWallID() == 0)
		{
			continue;
		}
		//��Ҫ��ֵĴ����б�
		for(RECTLIST::iterator it = m_SplitTo1RectList.begin();	it!=m_SplitTo1RectList.end();it++)
		{
			if(it->EqualRect(&itRect))
			{
				AddSplitRectMap(CVideoPaneMgr::SPLIT_1,_Rect);
				break;
			}
		}
		for(RECTLIST::iterator it = m_SplitTo4RectList.begin();	it!=m_SplitTo4RectList.end();it++)
		{
			if(it->EqualRect(&itRect))
			{
				AddSplitRectMap(CVideoPaneMgr::SPLIT_4,_Rect);
				break;
			}
		}
		for(RECTLIST::iterator it = m_SplitTo9RectList.begin();	it!=m_SplitTo9RectList.end();it++)
		{
			if(it->EqualRect(&itRect))
			{
				AddSplitRectMap(CVideoPaneMgr::SPLIT_9,_Rect);
				break;
			}
		}
		for(RECTLIST::iterator it = m_SplitTo16RectList.begin(); it!=m_SplitTo16RectList.end();it++)
		{
			if(it->EqualRect(&itRect))
			{
				AddSplitRectMap(CVideoPaneMgr::SPLIT_16,_Rect);
				break;
			}
		}
	}

    ////////(void)for_each(videoRects.begin(),videoRects.end(),
    ////////    FunctionObjectCustomLayout(rect,_uX,_uY,this));
    return IVS_SUCCEED;
}

// Qualifier: ���ݲ���
ULONG CVideoPaneMgr::SetLadderLayout(CRect rect,ULONG ulPaneNUm)
{
	LONG _ulPaneNUm = static_cast<LONG>(ulPaneNUm -1);
    if(( _ulPaneNUm < 5 ) || (_ulPaneNUm > 25))
    {
        return IVS_PARA_INVALID;
    }
    //�ж� �Ƿ�Ϊ����
    if ((_ulPaneNUm%2)== 0)
    {
        return IVS_PARA_INVALID;
    }

    // ������ƽ����������
    LONG averageX = rect.Width() / ((_ulPaneNUm +1)/2);
    LONG averageY = rect.Height() / ((_ulPaneNUm +1)/2);
    CRect _Rect;

    // ������ʱ�ĵ㣬���ڳ�ʼ��rect��ÿ������һ��
    CPoint topLeftTemp;
    CPoint bottomRightTemp;

    //��һ�� ���Ϸ���󴰸�

    topLeftTemp.x = 0;
    topLeftTemp.y = 0;
    bottomRightTemp.x = averageX* ((_ulPaneNUm-1) / 2);
    bottomRightTemp.y = averageY* ((_ulPaneNUm-1) / 2);

    // ��������
    _Rect.SetRect(topLeftTemp,bottomRightTemp);
    SetVideoPanePosition(_Rect);

    //�Ҳ� ��ulPaneNUm - 1�� / 2 �� ���ŵĴ���
    topLeftTemp.x = averageX* ((_ulPaneNUm-1) / 2);
    bottomRightTemp.x = rect.BottomRight().x;

    for (LONG iCount = 1; iCount <= (_ulPaneNUm-1) / 2; ++iCount)
    {
        topLeftTemp.y = static_cast<LONG>((iCount-1)* averageY);
        bottomRightTemp.y =  static_cast<LONG>(iCount* averageY);
        _Rect.SetRect(topLeftTemp,bottomRightTemp);
        SetVideoPanePosition(_Rect);
    }

    // �ײ� ��ulPaneNUm + 1�� / 2 �� ��ŵĴ���
    topLeftTemp.y = averageY * ((_ulPaneNUm-1) / 2);
    bottomRightTemp.y = rect.BottomRight().y;
    for (LONG iCount = 1; iCount <= (_ulPaneNUm + 1) / 2; ++iCount)
    {
        topLeftTemp.x = (iCount-1) * averageX;
        bottomRightTemp.x = iCount * averageX;
        _Rect.SetRect(topLeftTemp,bottomRightTemp);
        SetVideoPanePosition(_Rect);
    }
    return IVS_SUCCEED;
}

void CVideoPaneMgr::ResetBarPosition(void)
{
    (void)for_each(m_videoPaneMap.begin(),
             m_videoPaneMap.end(),
             FunctionObjecVideoPaneResetBarPosition());
}

void CVideoPaneMgr::ShowToolBar( bool isShow )
{
    CHECK_POINTER_VOID(m_pParentWnd);

    // ״̬��֮ǰһ�����Ͳ����ظ�����
    if ((static_cast<int>(m_isShowToolBar) == static_cast<int>(isShow))  && !m_isChangeToolBar)
    {
        return;
    }

    m_isShowToolBar = isShow;

	if (!m_isShowToolBar)
	{
		HideAllToolBar();
	}
	else
	{
		ResetBarPosition();
	}
}

// ��ʾ���⹤����
void CVideoPaneMgr::ShowTitleBar( bool isShow )
{
    CHECK_POINTER_VOID(m_pParentWnd);

    // �����Ѿ���ʾ�Ļ����Ͳ����ظ�����
    if (static_cast<IVS_INT32>(isShow) == static_cast<IVS_INT32>(m_IsShowTitleBar))
    {
        return;
    }

    m_IsShowTitleBar = isShow;
	if (!m_IsShowTitleBar)
	{
		HideAllTitileBar();
	}
	else
	{
		ResetBarPosition();
	}
}

bool CVideoPaneMgr::isPaneExistByID( ULONG ulPaneId )
{
	MAP_VIDEOPANE_ITER iter = m_videoPaneMap.find(ulPaneId);
	if (iter != m_videoPaneMap.end())return true;
	return false;
}

bool CVideoPaneMgr::isAnalysisWndType()const
{
	return (WND_TYPE_ANALYSIS == GetWndType());
}

void CVideoPaneMgr::HideAllBarOnce()
{
    (void)for_each(m_videoPaneMap.begin(),m_videoPaneMap.end(),
       FunctionObjectVideoPaneHideAllBar());
}

void CVideoPaneMgr::SetAllBarWindowsLong(ULONG ulHandle)
{
	m_ulParentHanlde = ulHandle;
	(void)for_each(m_videoPaneMap.begin(),m_videoPaneMap.end(),
		FunctionObjectVideoPaneSetAllBarWindowsLong());
}

ULONG CVideoPaneMgr::SetOcxCtrl( COleControl* val )
{
    CHECK_POINTER(val,IVS_PARA_INVALID);
    m_pOcxCtrl = val;
    return IVS_SUCCEED;
}

IVS_INT32 CVideoPaneMgr::GetHwndbyPlayHandle(unsigned long ulPlayHandle, HWND& hWnd)
{
    CAutoLock __Locker(m_csVideoPaneLock);

    MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin();
    MAP_VIDEOPANE_ITER iterEnd = m_videoPaneMap.end();

    for (; iter != iterEnd;)
    {
        CVideoPane* pPane = iter->second; //lint !e64
        if (NULL == pPane)
        {
            iter++;
            continue;
        }

        if (pPane->IsExistPlayHandle(ulPlayHandle))
        {
            hWnd = pPane->GetVideoWnd();
            return IVS_SUCCEED;
        }
        iter++;
    }
    return IVS_FAIL;
}

ULONG CVideoPaneMgr::StopAllSound( ENUMDEF::WND_TYPE WndType )
{
    CIVS_OCXPlayerCtrl *pOcx = dynamic_cast<CIVS_OCXPlayerCtrl *>(m_pOcxCtrl);
    CHECK_POINTER(pOcx,IVS_FAIL);

    CAutoLock __Locker(m_csVideoPaneLock);
    MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin();
    MAP_VIDEOPANE_ITER iterEnd = m_videoPaneMap.end();
    for (; iter != iterEnd;iter++)    //�ر�������·����
    {
        CVideoPane *pVideoPane = iter->second; //lint !e64
        if (pVideoPane == NULL)
        {
            continue;
        }
        if((WND_TYPE_REALTIME == WndType) && (BOTTOMBAR_SOUND_OPEN != pVideoPane->GetRealtimeSoundState()))   //ʵ��
        {
            continue;
        } else if((WND_TYPE_RECORD == GetWndType()) && (BOTTOMBAR_SOUND_OPEN != pVideoPane->GetRecordSoundState())){  //¼�� {
            continue;
        }
        ULONG ulRet = static_cast<ULONG>(pOcx->StopSound(iter->first));
        if (IVS_SUCCEED != ulRet)
        {
            BP_RUN_LOG_ERR(static_cast<int>(ulRet),"IVS_OCX::","VideoPaneManager Stop All Sound End.");
            return ulRet;
        }

        (void)SetSoundState(iter->first,false);
         pVideoPane->SetLastSoundState(false);
    }

    return IVS_SUCCEED;
}

bool CVideoPaneMgr::IsLegalityWndID( ULONG lWndID )const
{
    return (MIN_PANE_NUMBER <= lWndID) && (MAX_PANE_NUMBER >= lWndID);
}

void CVideoPaneMgr::SetTVWallPaneChannelStatus(ULONG ulWndID,ULONG ulStatus)
{
	if(LayoutMgr.isTVWallLayout(m_windLayOut))
	{
		CVideoPane* pane = GetWndPointerByID(ulWndID);
		CHECK_POINTER_VOID(pane);
		pane->SetTVWallStatusBarStatus(ulStatus);
	}

}

void CVideoPaneMgr::SetTVWallPaneStatus(ULONG ulWndID,ULONG ulPaneStatus)
{
	if(LayoutMgr.isTVWallLayout(m_windLayOut))
	{
		CVideoPane* pane = GetWndPointerByID(ulWndID);
		CHECK_POINTER_VOID(pane);
		pane->SetTVWallStatus(ulPaneStatus);	
	}
}

ULONG CVideoPaneMgr::GetTVWallPaneStatus(ULONG ulWndID)
{
    if(LayoutMgr.isTVWallLayout(m_windLayOut))
    {
        CVideoPane* pane = GetWndPointerByID(ulWndID);
        CHECK_POINTER(pane, 0);
        return pane->GetTVWallStatus();	
    }
    return 0;
}

ULONG CVideoPaneMgr::SetTurnCruiseStyle(ULONG ulWndID,ULONG ulStyle)
{
	if (0 == ulWndID)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "", "Video pane manager set turn cruise fail, handle is invalid.");
		return IVS_PARA_INVALID;
	}

	CVideoPane * pVideoPane = GetWndPointerByID(ulWndID);
	CHECK_POINTER(pVideoPane,IVS_PARA_INVALID);

	//������ѵ״̬
	pVideoPane->SetTurnCruiseStyle(ulStyle);
	//������ѵ����
	pVideoPane->CreatemTurnCruisePane();
	CTurnCruisePane * pTurnCruisePane = pVideoPane->GetTurnCruisePane();
	CHECK_POINTER(pTurnCruisePane,IVS_PARA_INVALID);
	pVideoPane->Invalidate(TRUE);
	pTurnCruisePane->Invalidate(TRUE);
    pVideoPane->InvalidateCruiseBar();

	return IVS_SUCCEED;
}

void CVideoPaneMgr::HideAllTranslucenWnd(void)
{
	if (m_ulShowWndNum < m_ulOperatePaneID)return;

	CHECK_POINTER_VOID(m_pParentWnd);
	CAutoLock __Locker(m_csVideoPaneLock);   //����

	CVideoPane * pVideoPane = nullptr;
	MAP_VIDEOPANE_ITER iter = m_videoPaneMap.find(m_ulOperatePaneID);

	if (iter == m_videoPaneMap.end())
	{
		CHECK_POINTER_VOID(IVS_NEW((CVideoPane*&)pVideoPane));
		pVideoPane->HideAllBar();
	}
	else {
		pVideoPane = iter->second; //lint !e64
	}
}

void CVideoPaneMgr::StartSerialCaptureAll()
{
	CIVS_OCXPlayerCtrl *pOcxCtrl = dynamic_cast<CIVS_OCXPlayerCtrl *>(m_pOcxCtrl);
	CHECK_POINTER_VOID(pOcxCtrl);
	MAP_VIDEOPANE_ITER iter    = m_videoPaneMap.begin(); 
	MAP_VIDEOPANE_ITER iterEnd = m_videoPaneMap.end(); 
	while(iter != iterEnd)
	{
		ULONG ulPanelID = iter->first;
		if(VIDEO_PLAY_STATE_NOT_PLAYING == iter->second->GetPlayState())
		{
			iter++;
			continue;
		}
		(void)pOcxCtrl->IVS_OCX_LocalSnapshot((LONG)ulPanelID);
		iter++;
	}
}

// ���ó���ǰ�������������������
void CVideoPaneMgr::SetALLMagnifyBtnStatus(const CVideoPane* pVedioPane, ULONGLONG lButtonStatus)
{
	CAutoLock __Locker(m_csVideoPaneLock);   //����
	for (MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin(); iter != m_videoPaneMap.end(); iter++)
	{
		CVideoPane *pTempVideoPane = iter->second; //lint !e64
		
		if (pTempVideoPane == nullptr || pTempVideoPane == pVedioPane || VIDEO_PLAY_STATE_NOT_PLAYING == pTempVideoPane->GetPlayState())
		{
			continue;
		}
		pTempVideoPane->SetToolbarEnableStatus(IVS_TOOLBAR_ZOOM,lButtonStatus);//lint !e747
	}
}

ULONG CVideoPaneMgr::MoveOCXToRect(CRect& rect)
{
	CHECK_POINTER(m_pParentWnd,IVS_FAIL);
	m_pParentWnd->MoveWindow(rect);
	return IVS_SUCCEED;
} //lint !e1764

ULONG CVideoPaneMgr::FullScreenKeyUp(ULONG ulMsgKey)
{
	CHECK_POINTER(m_pOcxCtrl,IVS_FAIL);
	CIVS_OCXPlayerCtrl *pOcxCtrl = dynamic_cast<CIVS_OCXPlayerCtrl *>(m_pOcxCtrl);
	CHECK_POINTER(pOcxCtrl,IVS_FAIL);
	return pOcxCtrl->FullScreenKeyEvent(ulMsgKey);
}

void CVideoPaneMgr::ResetTVWallStatus()
{
	CAutoLock __Locker(m_csVideoPaneLock);   //����
	for (MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin(); iter != m_videoPaneMap.end(); iter++)
	{
		CVideoPane *pTempVideoPane = iter->second; //lint !e64
		if (pTempVideoPane != NULL)
		{
			pTempVideoPane->SetTvWallState(false);
		}
	}
}

void CVideoPaneMgr::DoButtonFilter(LONG& ulWndID)
{
	ULONG ulOpenWndID = 0;
	if (GetMagnifyOpenWnd(ulOpenWndID))
	{
		if ((ULONG)ulWndID != ulOpenWndID)
		{
			// ��ʼ����
			CVideoPane* pVideoPane = GetWndPointerByID((ULONG)ulWndID);
			if (NULL != pVideoPane)
			{
				pVideoPane->SetToolbarEnableStatus(IVS_TOOLBAR_ZOOM,0);//lint !e747
				// ȥ�����򿪵��������֤����ȥ�ָ�
				pVideoPane->SetOpendMagnifyStatus(false);
			}
		}
	}

	if (MAGNIFY == m_ulIsMagnify)
	{
		// ���þֲ��Ŵ�ť
		CVideoPane* pVideoPane = GetWndPointerByID(m_ulActivePaneID);
		if (NULL != pVideoPane)
		{
			pVideoPane->SetToolbarEnableStatus(IVS_TOOLBAR_ZOOM,0);//lint !e747
			// ȥ�����򿪵��������֤����ȥ�ָ�
			pVideoPane->SetOpendMagnifyStatus(false);
		}
	}
}

bool CVideoPaneMgr::IsSoundStart(const CString& strCameraCode)
{
    CAutoLock __Locker(m_csVideoPaneLock);

    ULONG i = 0;
    MAP_VIDEOPANE_ITER iter = m_videoPaneMap.begin();
    while((iter != m_videoPaneMap.end()) && (i<m_ulShowWndNum))
    {
        CVideoPane *pVideoPane = iter->second;
        if (nullptr == pVideoPane)
        {
            ++iter;
            ++i;
            continue;
        }

        if (pVideoPane->VerifySameStream((const char *)strCameraCode, STREAM_TYPE_UNSPECIFIED))
        {
            CTranVideoRealtimeBar* pBar = pVideoPane->GetVideoRealtimeBar();
            if (nullptr != pBar)
            {
                if ( pBar->GetSoundState() == BOTTOMBAR_TALKBACK_OPEN )
                {
                    return true;
                }
            }
        }
        ++iter;
        ++i;
    } //end while((iter != m_videoPaneMap.end()) && (i<m_ulShowWndNum))
    return false;
}

void CVideoPaneMgr::HideAllTitileBar()
{
	(void)for_each(m_videoPaneMap.begin(),m_videoPaneMap.end(),
		FunctionObjectVideoPaneHideAllTitleBar());
}

void CVideoPaneMgr::HideAllToolBar()
{
	(void)for_each(m_videoPaneMap.begin(),m_videoPaneMap.end(),
		FunctionObjectVideoPaneHideAllToolBar());
}

void CVideoPaneMgr::HideShellTrayWnd(BOOL bShow)const
{
	HWND hWnd = FindWindow("Shell_TrayWnd",NULL);
	HWND hStart = FindWindowEx(NULL,NULL,"Button",NULL);
	if (hWnd != NULL)
	{
		if (bShow)
		{
			ShowWindow(hWnd,SW_SHOW);
		}
		else
		{
			ShowWindow(hWnd,SW_HIDE);
		}
	}
	if (hStart != NULL)
	{
		if (bShow)
		{
			ShowWindow(hStart,SW_SHOW);
		}
		else
		{
			ShowWindow(hStart,SW_HIDE);
		}
	}
}

void CVideoPaneMgr::DragMoveOutofOCX()
{
	CIVS_OCXPlayerCtrl *pOcxCtrl = dynamic_cast<CIVS_OCXPlayerCtrl *>(m_pOcxCtrl);
	CHECK_POINTER_VOID(pOcxCtrl);

	ULONG ulActiveID = GetActivePaneID();

	pOcxCtrl->PostPointMoveOutofOCX(ulActiveID);
}

void CVideoPaneMgr::SetToolBarVisableStatus(ULONGLONG val)
{
	if (m_ullToolBarBtnVisableStatus != val)
	{
		m_isChangeToolBar = true;
	}
	m_ullToolBarBtnVisableStatus = val;
}
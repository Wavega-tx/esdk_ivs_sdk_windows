#include "IVS_IATrack.h"
#include "IVSCommon.h"
#include "ivs_log.h"
#include "ivs_error.h"
#include <windows.h>

#define IA_BASE_LINE_WIDTH 3	// �����߿�
#define IA_BASE_FONT_SIZE 20	// ���������С
#define IA_FRAMEID_OFFSET_MAX 200	// ��ʾ�켣ʱ���ʱ��ƫ��
#define IA_ROI_LAST_TIME  60000  // ʵ��������ͣ���ʱ�䣨��λ��ms��
#define IA_ALARM_LAST_TIME  500  // �澯��ͣ���ʱ�䣨��λ��ms��

#define IA_QUERY_TYPE_REALTIME	1
// #define IA_QUERY_TYPE_RECORD	2


CIATrack::CIATrack()
	:m_bInitTag(FALSE)
	,m_ulCurrentFrameID(0)
	,m_IAROIListLength(0)
	,m_IAAlarmedListLength(0)
	,m_hMsgWnd(NULL)
	,m_ulWndWidth(0)
	,m_ulWndHeight(0)
	,m_ulDCWidth(0)
	,m_ulDCHeight(0)
	,m_lLineWidthAdjust(1)
	,m_ulLineWidth(0)
	,m_ulVideoType(0)
	,m_hFont(NULL)
	,m_ulCurrentFontSize(0)
{
	memset(&m_lf, 0, sizeof(LOGFONT));
}

CIATrack::~CIATrack()
{
	if (NULL != m_hFont)
	{
		DeleteObject(m_hFont); 
		m_hFont = NULL;
	}

}

// ��ʼ�����ܷ����켣
LONG CIATrack::InitIATrack(HWND hMsgWnd, ULONG ulVideoType)
{
	if (m_bInitTag)
	{
		IVS_LOG(IVS_LOG_ERR, "Init IA Track", "[%x]Already Inited", this);
		return IVS_FAIL;
	}
	if (!IsWindow(hMsgWnd))
	{
		IVS_LOG(IVS_LOG_ERR, "Init IA Track", "[%x]hMsgWnd NULL", this);
		return IVS_PARA_INVALID;
	}

	m_hMsgWnd				= hMsgWnd;	// ���ھ��
	RECT Wndrect;
    memset(&Wndrect, 0, sizeof(Wndrect));
	GetClientRect(m_hMsgWnd, &Wndrect); 
	m_ulWndWidth  = (ULONG)(Wndrect.right  - Wndrect.left);		// ���ڿ��
	m_ulWndHeight = (ULONG)(Wndrect.bottom - Wndrect.top);		// ���ڸ߶�
	// �жϴ��ڳ����Ƿ�Ϊ0
	if (( 0 == m_ulWndWidth) || (0 == m_ulWndHeight))
	{
		IVS_LOG(IVS_LOG_ERR, "Init IA Track", "[%x]hMsgWnd Size 0", this);
		return IVS_FAIL;
	}

	m_ulCurrentFrameID		= 0;		// ��ǰʱ���
	m_IAROIListLength		= 0;		// ROI�켣��Ϣ�б���
	m_IAAlarmedListLength	= 0;		// Alarmed�켣��Ϣ�б���
	m_ulDCWidth				= 0;		// ��ʾͼ����
	m_ulDCHeight			= 0;		// ��ʾͼ��߶�
	m_lLineWidthAdjust		= 1;		// �߿�ϵ�������ڷֱ��ʵ���
	m_ulLineWidth			= IA_BASE_LINE_WIDTH;		// ͼ���߿�
	m_ulVideoType			= ulVideoType;
	m_hFont					= NULL;		// ������
	m_ulCurrentFontSize		= 0;		// ��ǰ�����С
	m_IAROIList.clear();
	m_IAAlarmedList.clear();
	memset(&m_lf, 0, sizeof(LOGFONT));

	m_bInitTag				= TRUE;
	return IVS_SUCCEED;
}

// �������ܷ����켣��Ϣ
LONG CIATrack::InsertIALayoutMessage(const IA_LAYOUT_MESSAGE *LayoutMessage)
{
	if (NULL == LayoutMessage)
	{
		IVS_LOG(IVS_LOG_ERR, "Insert IA Layout Message", "[%x]LayoutMessage is NULL", this);
		return IVS_PARA_INVALID;
	}
	IVS_LOG(IVS_LOG_DEBUG, "Insert IA Layout Message", 
		"[%x]Current FrameID: %I64u, Insert FrameID: %I64u", 
		this, m_ulCurrentFrameID, LayoutMessage->ulFrameID);
	IVS_LOG(IVS_LOG_DEBUG, "Insert IA Layout Message", 
		"[%x]Is Percent: %lu", this, LayoutMessage->LayoutElement->AlarmPolyline.ulIsPercent);

	LONG lRet = IVS_FAIL;

	// ɾ���ɵ�FrameID�ڵ�
	DeleteExpiredMessage(m_IAROIList, m_IAROIListLength, FALSE);
	DeleteExpiredMessage(m_IAAlarmedList, m_IAAlarmedListLength, TRUE);
	
	IA_LAYOUT_MESSAGE* pInputLayoutMessage = (IA_LAYOUT_MESSAGE*)IVS_NEW((CHAR* &)pInputLayoutMessage, sizeof(IA_LAYOUT_MESSAGE));//lint !e826
	if (NULL == pInputLayoutMessage)
	{
		return IVS_ALLOC_MEMORY_ERROR;
	}
	memset(pInputLayoutMessage, 0, sizeof(IA_LAYOUT_MESSAGE));
	memcpy_s(pInputLayoutMessage, sizeof(IA_LAYOUT_MESSAGE), 
		LayoutMessage, sizeof(IA_LAYOUT_MESSAGE));


	// �½�����IA_LAYOUT_MESSAGE���󣬷ֱ�洢ROI��Alarmed��Ϣ
	IA_LAYOUT_MESSAGE* pROIMessage = (IA_LAYOUT_MESSAGE*)IVS_NEW((CHAR* &)pROIMessage, sizeof(IA_LAYOUT_MESSAGE));//lint !e826
	if (NULL == pROIMessage)
	{
		IVS_DELETE(pInputLayoutMessage, MUILI);
		return IVS_ALLOC_MEMORY_ERROR;
	}
	IA_LAYOUT_MESSAGE* pAlarmedMessage = (IA_LAYOUT_MESSAGE*)IVS_NEW((CHAR* &)pAlarmedMessage, sizeof(IA_LAYOUT_MESSAGE));//lint !e826
	if (NULL == pAlarmedMessage)
	{
		IVS_DELETE(pInputLayoutMessage, MUILI);
		IVS_DELETE(pROIMessage, MUILI);
		return IVS_ALLOC_MEMORY_ERROR;
	}
	memset(pROIMessage, 0, sizeof(IA_LAYOUT_MESSAGE));
	pROIMessage->ulFrameID     = pInputLayoutMessage->ulFrameID;
	pROIMessage->ulFrameWidth  = pInputLayoutMessage->ulFrameWidth;
	pROIMessage->ulFrameHeight = pInputLayoutMessage->ulFrameHeight;
	memcpy_s(pAlarmedMessage, sizeof(IA_LAYOUT_MESSAGE), pROIMessage, sizeof(IA_LAYOUT_MESSAGE));
	
	// �Դ���Ĺ켣��Ϣ�ṹ�����ROI/Alarmed���
	for (ULONG i = 0; i < pInputLayoutMessage->ulLayoutElementNum; i++)
	{
		// ��Element���е������ת��
		lRet = CoordinateTransform(pInputLayoutMessage->LayoutElement[i], 
			pInputLayoutMessage->ulFrameWidth, pInputLayoutMessage->ulFrameHeight);
		if (IVS_SUCCEED != lRet)
		{
			IVS_DELETE(pInputLayoutMessage, MUILI);
			IVS_DELETE(pROIMessage, MUILI);
			IVS_DELETE(pAlarmedMessage, MUILI);
			return lRet;
		}

		if (IA_LAYER_TYPE_ROI == pInputLayoutMessage->LayoutElement[i].ulLayer)
		{
			memcpy_s(&pROIMessage->LayoutElement[pROIMessage->ulLayoutElementNum], 
					sizeof(IA_LAYOUT_MESSAGE), 
					&pInputLayoutMessage->LayoutElement[i], 
					sizeof(IA_LAYOUT_ELEMENT));
			pROIMessage->ulLayoutElementNum++;
			IVS_LOG(IVS_LOG_DEBUG, "Insert IA Layout Message", "[%x]Insert ROI", this);
			POINT Points[IA_POINTS_NUM_MAX];
			memcpy_s(Points, sizeof(POINT) * IA_POINTS_NUM_MAX, 
				pROIMessage->LayoutElement[0].AlarmPolyline.Points, sizeof(POINT) * IA_POINTS_NUM_MAX);
			IVS_LOG(IVS_LOG_DEBUG, "Insert IA Layout Message", 
				"[%x]ROI Points: (%d,%d); (%d,%d); (%d,%d); (%d,%d).", this, 
				Points[0].x, Points[0].y, Points[1].x, Points[1].y, 
				Points[2].x, Points[2].y, Points[3].x, Points[3].y);

		}
		if (IA_LAYER_TYPE_ALARM == pInputLayoutMessage->LayoutElement[i].ulLayer)
		{
			memcpy_s(&pAlarmedMessage->LayoutElement[pAlarmedMessage->ulLayoutElementNum], 
					sizeof(IA_LAYOUT_MESSAGE), 
					&pInputLayoutMessage->LayoutElement[i], 
					sizeof(IA_LAYOUT_ELEMENT));
			pAlarmedMessage->ulLayoutElementNum++;
			IVS_LOG(IVS_LOG_DEBUG, "Insert IA Layout Message", "[%x]Insert Alarm", this);
			POINT Points[IA_POINTS_NUM_MAX];
			memcpy_s(Points, sizeof(POINT) * IA_POINTS_NUM_MAX, 
				pAlarmedMessage->LayoutElement[0].AlarmPolyline.Points, sizeof(POINT) * IA_POINTS_NUM_MAX);
			IVS_LOG(IVS_LOG_DEBUG, "Insert IA Layout Message", 
				"[%x]Alarmed Points: (%d,%d); (%d,%d); (%d,%d); (%d,%d).", this, 
				Points[0].x, Points[0].y, Points[1].x, Points[1].y, 
				Points[2].x, Points[2].y, Points[3].x, Points[3].y);
		}
	}

	// �ֱ���뵽ROI/Alarmed�б�
	lRet = IVS_SUCCEED;
	if (pROIMessage->ulLayoutElementNum) // LayoutMessage�д���ROI��
	{
		lRet = InsertMessageIntoList(*pROIMessage, m_IAROIList, m_IAROIListLength);
		if (IVS_SUCCEED != lRet)
		{
			IVS_DELETE(pInputLayoutMessage, MUILI);
			IVS_DELETE(pROIMessage, MUILI);
			IVS_DELETE(pAlarmedMessage, MUILI);
			return lRet;
		}
	}
	
	if (pAlarmedMessage->ulLayoutElementNum) // LayoutMessage�д���Alarmed��
	{
		lRet = InsertMessageIntoList(*pAlarmedMessage, m_IAAlarmedList, m_IAAlarmedListLength);
	}

	IVS_DELETE(pInputLayoutMessage, MUILI);
	IVS_DELETE(pROIMessage, MUILI);
	IVS_DELETE(pAlarmedMessage, MUILI);
	return lRet;
}

// ��ʾ���ܷ����켣��Ϣ
LONG CIATrack::ShowIATrack(HDC hDC, const RECT *DCRect, ULONG64 ulFrameID)
{
	if (NULL == DCRect)
	{
		IVS_LOG(IVS_LOG_ERR, "Show IA Track", "[%x]Invalid Input", this);
		return IVS_PARA_INVALID;
	}
// 	IVS_LOG(IVS_LOG_INFO, "Show IA Track", "[%x]Current FrameID: %I64u", this, ulFrameID);

	// ���´��ڴ�С
	RECT Wndrect;
    memset(&Wndrect, 0, sizeof(Wndrect));

	if (GetClientRect(m_hMsgWnd, &Wndrect))
	{	
		m_ulWndWidth  = (ULONG)(Wndrect.right  - Wndrect.left);		// ���ڿ��
		m_ulWndHeight = (ULONG)(Wndrect.bottom - Wndrect.top);		// ���ڸ߶�
	}
	// �жϴ��ڳ����Ƿ�Ϊ0
	if (( 0 == m_ulWndWidth) || (0 == m_ulWndHeight))
	{
		IVS_LOG(IVS_LOG_ERR, "Show IA Track", "[%x]hMsgWnd Size 0", this);
		return IVS_FAIL;
	}

	// ����ͼ���С
	ULONG DCWidth  = (ULONG)(DCRect->right - DCRect->left);
	ULONG DCHeight = (ULONG)(DCRect->bottom - DCRect->top);
	if ((DCWidth != m_ulDCWidth) || (DCHeight != m_ulDCHeight))
	{
		m_ulDCWidth  = DCWidth;
		m_ulDCHeight = DCHeight;
 		IVS_LOG(IVS_LOG_INFO, "Show IA Track", "[%x]Image Size Changed", this);
	}
	// �жϻ�ͼDC�����Ƿ�Ϊ0
	if (( 0 == m_ulDCWidth) || (0 == m_ulDCHeight))
	{
		IVS_LOG(IVS_LOG_ERR, "Show IA Track", "[%x]DC Size 0", this);
		return IVS_FAIL;
	}

	if (m_ulDCWidth >= m_ulWndWidth)
	{
		m_lLineWidthAdjust = (LONG)max(1, m_ulDCWidth / m_ulWndWidth);
	} else {
		m_lLineWidthAdjust = - (LONG)(m_ulWndWidth / m_ulDCWidth);
	}
	m_ulLineWidth = (ULONG)max(1, IA_BASE_LINE_WIDTH + m_lLineWidthAdjust);


	if (0 != ulFrameID)
	{
		// ���µ�ǰFrameID
		m_ulCurrentFrameID = ulFrameID;
		// ɾ��FrameID���ڵĹ켣��Ϣ
		DeleteExpiredMessage(m_IAROIList, m_IAROIListLength, FALSE);
		DeleteExpiredMessage(m_IAAlarmedList, m_IAAlarmedListLength, TRUE);
	}

	// ��ʾ�б������ڵ�Ĺ켣��Ϣ
	LONG lRet = ShowIATrackMessage(hDC, m_IAROIList, FALSE); // ��ʾROI�б�
	if (IVS_SUCCEED != lRet) 
	{
		return lRet;
	}
	lRet = ShowIATrackMessage(hDC, m_IAAlarmedList, TRUE);// ��ʾAlarmed�б�

	return lRet;
}



// ��ȡ��ǰ�켣��Ϣ����
LONG CIATrack::GetCurrentElementNum()
{
	return (LONG)m_IAAlarmedListLength;
}


/************************************************************************/
/*								�ڲ�ʵ��                                  */
/************************************************************************/

// ΪMessage�����ڴ�
IA_LAYOUT_MESSAGE* CIATrack::AllocateMessageMemory()
{
	IA_LAYOUT_MESSAGE* p = IVS_NEW(p);
	return p;
}

// ΪMessage�ͷ��ڴ�
VOID CIATrack::ReleaseMessageMemory(IA_LAYOUT_MESSAGE* m)
{
	IVS_DELETE(m);
}


// ����ת��
LONG CIATrack::CoordinateTransform(IA_LAYOUT_ELEMENT &LayoutElement, ULONG ulFrameWidth, ULONG ulFrameHeight)
{
	if (( 0 == ulFrameWidth) || (0 == ulFrameHeight))
	{
		IVS_LOG(IVS_LOG_ERR, "Coordinate Transform", "[%x]FrameSize 0", this);
		return IVS_PARA_INVALID;
	}

	IVS_LOG(IVS_LOG_DEBUG, "Coordinate Transform", 
		"[%x]FrameWidth: %lu, FrameHeight: %lu, DCWidth: %lu, DCHeight: %lu", 
		this, ulFrameWidth, ulFrameHeight, m_ulDCWidth, m_ulDCHeight);

	switch (LayoutElement.ulElementType)
	{
	case IA_ELEMENT_TYPE_TEXT:
		if (LayoutElement.AlarmText.ulIsPercent)
		{
			break;
		}
		LayoutElement.AlarmText.TextPoint.x = 
			LayoutElement.AlarmText.TextPoint.x * (LONG)m_ulDCWidth  / (LONG)ulFrameWidth;
		LayoutElement.AlarmText.TextPoint.y = 
			LayoutElement.AlarmText.TextPoint.y * (LONG)m_ulDCHeight / (LONG)ulFrameHeight;
		break;
	case IA_ELEMENT_TYPE_POLYLINE:
		if (LayoutElement.AlarmPolyline.ulIsPercent)
		{
			break;
		}

		for (ULONG i = 0; i < LayoutElement.AlarmPolyline.ulPointsNum; i++)
		{
			LayoutElement.AlarmPolyline.Points[i].x = 
				LayoutElement.AlarmPolyline.Points[i].x * (LONG)m_ulDCWidth  / (LONG)ulFrameWidth;
			LayoutElement.AlarmPolyline.Points[i].y = 
				LayoutElement.AlarmPolyline.Points[i].y * (LONG)m_ulDCHeight / (LONG)ulFrameHeight;
		}
		break;
	default:
		IVS_LOG(IVS_LOG_ERR, "Coordinate Transform", "[%x]ElementType Invalid", this);
		return IVS_FAIL;
	}
	return IVS_SUCCEED;
}


// ɾ���б��й�����Ϣ
VOID CIATrack::DeleteExpiredMessage(IA_LAYOUT_MESSAGE_LIST &DestList, ULONG &refListLength, BOOL bIsAlarmed)
{
	CAutoLock Lock(m_Mutex);

	IA_LAYOUT_MESSAGE_LIST_REVERSE_ITER rIter = DestList.rbegin();
	IA_LAYOUT_MESSAGE_LIST_REVERSE_ITER rIterNext = DestList.rbegin();

	// ��β��ͷ�����б�
	while (rIter != DestList.rend())
	{
		IA_LAYOUT_MESSAGE *pListMessage = *rIter;
		BOOL bNextExpired = FALSE; // �����һ���ڵ��Ƿ������Ϣ
		rIterNext = rIter;
		rIterNext++;
		ULONG64 ulNextFrameID = 0;
		if (rIterNext != DestList.rend())
		{
			IA_LAYOUT_MESSAGE *pListMessageNext = *rIterNext;
			bNextExpired = pListMessageNext->ulFrameID <= m_ulCurrentFrameID ? TRUE : FALSE;
			ulNextFrameID = pListMessageNext->ulFrameID;
		}

		// Alarmed 500ms���ڣ�ROIʵ����60000ms���ڣ�¼���±������һ֡
		if (((pListMessage->ulFrameID  < m_ulCurrentFrameID - IA_ALARM_LAST_TIME) && bIsAlarmed) 
			|| ((pListMessage->ulFrameID  < m_ulCurrentFrameID - IA_ROI_LAST_TIME) && !bIsAlarmed && IA_QUERY_TYPE_REALTIME == m_ulVideoType) 
			|| bNextExpired)
		{
			IVS_LOG(IVS_LOG_DEBUG, "Delete Expired Message", 
				"[%x]Current FrameID is: %I64u, Deleted FrameID is: %I64u, Next FrameID is: %I64u, Layer Type is: %s", 
				this, m_ulCurrentFrameID, pListMessage->ulFrameID, ulNextFrameID, bIsAlarmed ? "Alarm" : "ROI");
			ReleaseMessageMemory(pListMessage);
			rIter = IA_LAYOUT_MESSAGE_LIST_REVERSE_ITER(DestList.erase((++rIter).base()));
			refListLength--;
			continue;
		}
		else
		{
			// �б�FrameID�Ӵ�С���У���������ǰ�Ƚ�
			break;
		}
	}
	return;
}


// ����Layer��ֺ����Ϣ���뵽��Ӧ���б���
LONG CIATrack::InsertMessageIntoList(const IA_LAYOUT_MESSAGE &refNewMessage, IA_LAYOUT_MESSAGE_LIST &DestList, ULONG &refListLength)
{
	// �ж��б��й켣��Ϣ�ﵽ����
	if (IA_MESSAGE_NUM_MAX <= refListLength)
	{
		IVS_LOG(IVS_LOG_ERR, "Insert Message Into List", "[%x]Reach MAX MessageNum", this);
		return IVS_FAIL;
	}

	// �����½ڵ��ڴ棬����������ڵ�����
	IA_LAYOUT_MESSAGE *pMessage = NULL;
	pMessage = AllocateMessageMemory();
	if (NULL == pMessage)
	{
		IVS_LOG(IVS_LOG_ERR, "Insert Message Into List", "[%x]No Enough Memory", this);
		return IVS_ALLOC_MEMORY_ERROR;
	}
	memcpy_s(pMessage, sizeof(IA_LAYOUT_MESSAGE), 
			&refNewMessage, sizeof(IA_LAYOUT_MESSAGE));


	CAutoLock Lock(m_Mutex);
	
	// ���¹켣��Ϣ����С˳����뵽�б���
	IA_LAYOUT_MESSAGE_LIST_ITER Iter = DestList.begin();
	IA_LAYOUT_MESSAGE_LIST_ITER End  = DestList.end();
	for (; Iter != End; Iter++)
	{
		IA_LAYOUT_MESSAGE *pListMessage = *Iter;
		if (pListMessage->ulFrameID < pMessage->ulFrameID)
		{
			DestList.insert(Iter, pMessage);
			refListLength++;
			IVS_LOG(IVS_LOG_DEBUG, "Insert Message Into List", 
				"[%x]FrameID: %I64u Inserted", this, pMessage->ulFrameID);
			return IVS_SUCCEED;
		}
		if (pListMessage->ulFrameID == pMessage->ulFrameID)
		{
			memcpy_s(pListMessage, sizeof(IA_LAYOUT_MESSAGE), 
				pMessage, sizeof(IA_LAYOUT_MESSAGE));
			IVS_LOG(IVS_LOG_DEBUG, "Insert Message Into List", 
				"[%x]FrameID: %I64u Updated", this, pMessage->ulFrameID);
			ReleaseMessageMemory(pMessage);
			return IVS_SUCCEED;
		}
	}
	// ���б���δ�ҵ���NewMessage��FrameIDС�Ľڵ�
	DestList.push_back(pMessage);
	refListLength++;
	IVS_LOG(IVS_LOG_DEBUG, "Insert Message Into List", 
		"[%x]FrameID: %I64u Inserted", this, pMessage->ulFrameID);

	return IVS_SUCCEED;
}


// ��ʾ�б������ڵ�Ĺ켣��Ϣ
LONG CIATrack::ShowIATrackMessage(HDC hDC, IA_LAYOUT_MESSAGE_LIST &DestList, BOOL bIsAlarmed)
{
	CAutoLock Lock(m_Mutex);

#if 0
    IA_ALARM_POLYLINE polyline;
    memset(&polyline, 0, sizeof(polyline));
    polyline.Points[0].x = 10;
    polyline.Points[0].y = 300;
    polyline.Points[1].x = 600;
    polyline.Points[1].y = 600;
    polyline.ulDirection = 3;
    polyline.ulPointsNum = 2;
    COLORREF ulColor = RGB(0,255,0);
    ShowIATrackPolyline(hDC, polyline, ulColor);
    return 0;
#endif

	if (DestList.empty())
	{
		IVS_LOG(IVS_LOG_DEBUG, "Show IA Track Message", 
			"[%x]%s List Empty", this, bIsAlarmed ? "Alarm" : "ROI");
		return IVS_SUCCEED;
	}
	IA_LAYOUT_MESSAGE_LIST_REVERSE_ITER rIter = DestList.rbegin();
	IA_LAYOUT_MESSAGE *pListMessage = *rIter;
	LONG lRet = IVS_FAIL;

#if 0
	IA_LAYOUT_MESSAGE_LIST_REVERSE_ITER rEndIter = DestList.rend();
	rEndIter--;
	IA_LAYOUT_MESSAGE *pEndListMessage = *rEndIter;

		int textlen = 0;	
		char buf[256] = {0};
		textlen = _snprintf_s(buf, 255, "m_ulCurrentFrameID:%d", m_ulCurrentFrameID);
		TextOut(hDC, 100, 25, buf, textlen);
		textlen = _snprintf_s(buf, 255, "ulFrameID                :%d", pListMessage->ulFrameID);
		TextOut(hDC, 100, 50, buf, textlen);
		textlen = _snprintf_s(buf, 255, "ulFrameIDEnd           :%d", pEndListMessage->ulFrameID);
		TextOut(hDC, 100, 75, buf, textlen);

		textlen = _snprintf_s(buf, 255, "ulFrameID - m_ulCurrentFrameID:%d", 
			pListMessage->ulFrameID - m_ulCurrentFrameID);
		TextOut(hDC, 100, 100, buf, textlen);
		textlen = _snprintf_s(buf, 255, "m_IAROIList.size: %d", 
			m_IAROIList.size());
		TextOut(hDC, 100, 150, buf, textlen);
		textlen = _snprintf_s(buf, 255, "m_IAAlarmedList.size: %d", 
			m_IAAlarmedList.size());
		TextOut(hDC, 100, 200, buf, textlen);
		int textlen = 0;	
		char buf[256] = {0};
		textlen = _snprintf_s(buf, 255, "m_ulDCWidth:%d", m_ulDCWidth);
		TextOut(hDC, 100, 25, buf, textlen);
		textlen = _snprintf_s(buf, 255, "m_ulDCHeight:%d", m_ulDCHeight);
		TextOut(hDC, 100, 50, buf, textlen);
#endif

	// ֻ����IA_FRAMEID_OFFSET_MAX����֮�ڵĹ켣ͼ��
	if ((pListMessage->ulFrameID > m_ulCurrentFrameID) 
		&& (IA_FRAMEID_OFFSET_MAX < (pListMessage->ulFrameID - m_ulCurrentFrameID)) 
		/*&& bIsAlarmed*/)
	{
		return IVS_SUCCEED;
	}
	for (ULONG i = 0; i < pListMessage->ulLayoutElementNum; i++)
	{
		lRet = ShowIATrackElement(hDC, pListMessage->LayoutElement[i]);
		if (IVS_SUCCEED != lRet)
		{
			return lRet;
		}
	}
	return IVS_SUCCEED;
}

// ��ʾ�����켣Element
LONG CIATrack::ShowIATrackElement(HDC hDC, IA_LAYOUT_ELEMENT &LayoutElememt)
{
	LONG lRet = IVS_FAIL;
	switch (LayoutElememt.ulElementType)
	{
	case IA_ELEMENT_TYPE_TEXT:
		lRet = ShowIATrackText(hDC, LayoutElememt.AlarmText, LayoutElememt.ulColor);
		break;
	case IA_ELEMENT_TYPE_POLYLINE:
		lRet = ShowIATrackPolyline(hDC, LayoutElememt.AlarmPolyline, LayoutElememt.ulColor);
		break;
	default:
		IVS_LOG(IVS_LOG_ERR, "Show IA Track Element", "[%x]ElementType Error", this);
		break;
	}
	return lRet;
}


// ��ʾ�켣������Ϣ
LONG CIATrack::ShowIATrackText(HDC hDC, IA_ALARM_TEXT &AlarmText, COLORREF ulColor)
{
	// �������С�����仯ʱ�ٴ�������
	ULONG ulFontSize = (ULONG)max(1, IA_BASE_FONT_SIZE + m_lLineWidthAdjust) 
						* AlarmText.ulTextScale;
	if (ulFontSize != m_ulCurrentFontSize)
	{
		m_ulCurrentFontSize = ulFontSize;
		LONG lRet = CreatFont(/*AlarmText.ulTextScale*/);
		if (IVS_SUCCEED != lRet)
		{
			return lRet;
		}
	}

	IVS_LOG(IVS_LOG_DEBUG, "Show IATrack Text", 
		"[%x]Is Percent: %lu", this, AlarmText.ulIsPercent);

	// ת���ٷֱ�����Ϊͼ������
	if (AlarmText.ulIsPercent)
	{
		IVS_LOG(IVS_LOG_DEBUG, "Show IATrack Text", "[%x]Percent Point: %d, %d", 
			this, AlarmText.TextPoint.x, AlarmText.TextPoint.y);

		AlarmText.TextPoint.x = AlarmText.TextPoint.x * (LONG)m_ulDCWidth  / 100;
		AlarmText.TextPoint.y = AlarmText.TextPoint.y * (LONG)m_ulDCHeight / 100;
		AlarmText.ulIsPercent = 0;
		IVS_LOG(IVS_LOG_DEBUG, "Show IATrack Text", "[%x]After Adjust: %d, %d", 
			this, AlarmText.TextPoint.x, AlarmText.TextPoint.y);

	}

	SetTextColor(hDC, ulColor);	// ������ɫ   
	SetBkMode(hDC, TRANSPARENT);// ����ɫ(͸��)
	SelectObject(hDC,m_hFont);

	TextOut(hDC, AlarmText.TextPoint.x, AlarmText.TextPoint.y, 
		AlarmText.TextContent, (int)AlarmText.ulTextLenth);

	return IVS_SUCCEED;
}

// �����ͷ����
LONG CIATrack::CalcuArrowPoints(const IA_ALARM_POLYLINE& PolyLineInfo, IA_ARROW& ArrowPointSet)
{
    memset(&ArrowPointSet, 0, sizeof(ArrowPointSet));
    LONG dStraightLineWidth  = PolyLineInfo.Points[1].x - PolyLineInfo.Points[0].x;
    LONG dStraightLineHeight = PolyLineInfo.Points[1].y - PolyLineInfo.Points[0].y;
    DOUBLE dLenth = (DOUBLE)dStraightLineWidth * (DOUBLE)dStraightLineWidth 
        + (DOUBLE)dStraightLineHeight * (DOUBLE)dStraightLineHeight;
    LONG lStraightLineLenth  = (LONG)sqrt(dLenth);

    for (unsigned int i = 1; i <= 2; i++)
    {
        if (i & PolyLineInfo.ulDirection)
        {
            ArrowPointSet.ArrowTail[i - 1].x = PolyLineInfo.Points[0].x + dStraightLineWidth  / 2;
            ArrowPointSet.ArrowTail[i - 1].y = PolyLineInfo.Points[0].y + dStraightLineHeight / 2;

            LONG dArrowWidth  = (LONG)pow((DOUBLE)-1, (DOUBLE)(i - 1)) * dStraightLineHeight * ARROW_LENTH / lStraightLineLenth;
            LONG dArrowHeight = (LONG)pow((DOUBLE)-1, (DOUBLE)(i))     * dStraightLineWidth  * ARROW_LENTH / lStraightLineLenth;

            ArrowPointSet.ArrowHead[i - 1].x = ArrowPointSet.ArrowTail[i - 1].x + dArrowWidth;
            ArrowPointSet.ArrowHead[i - 1].y = ArrowPointSet.ArrowTail[i - 1].y + dArrowHeight;

            POINT FourFifthOnArrow = {0, 0};
            FourFifthOnArrow.x = ArrowPointSet.ArrowTail[i - 1].x + dArrowWidth  * 4 / 5;
            FourFifthOnArrow.y = ArrowPointSet.ArrowTail[i - 1].y + dArrowHeight * 4 / 5;

            LONG dArrowHeadWidth  = - dArrowHeight / 5;
            LONG dArrowHeadHeight =   dArrowWidth  / 5;

            ArrowPointSet.ArrowTrangle1[i - 1].x = FourFifthOnArrow.x + dArrowHeadWidth;
            ArrowPointSet.ArrowTrangle1[i - 1].y = FourFifthOnArrow.y + dArrowHeadHeight;

            ArrowPointSet.ArrowTrangle2[i - 1].x = FourFifthOnArrow.x - dArrowHeadWidth;
            ArrowPointSet.ArrowTrangle2[i - 1].y = FourFifthOnArrow.y - dArrowHeadHeight;
        }
    }
    return IVS_SUCCEED;
}

LONG CIATrack::ShowArrowShape(HDC hDC, const IA_ARROW &iaArrow, ULONG ulArrowType)
{
    for (unsigned int i = 1; i <= 2; i++)
    {
        if (i & ulArrowType)
        {
            MoveToEx(hDC, iaArrow.ArrowTail[i - 1].x, iaArrow.ArrowTail[i - 1].y, NULL);
            LineTo(hDC, iaArrow.ArrowHead[i - 1].x, iaArrow.ArrowHead[i - 1].y);
            LineTo(hDC, iaArrow.ArrowTrangle1[i - 1].x, iaArrow.ArrowTrangle1[i - 1].y);
            MoveToEx(hDC, iaArrow.ArrowHead[i - 1].x, iaArrow.ArrowHead[i - 1].y, NULL);
            LineTo(hDC, iaArrow.ArrowTrangle2[i - 1].x, iaArrow.ArrowTrangle2[i - 1].y);
        }
    }
    return IVS_SUCCEED;
}

// ��ʾ�켣ͼ����Ϣ
LONG CIATrack::ShowIATrackPolyline(HDC hDC, IA_ALARM_POLYLINE &AlarmPolyline, COLORREF ulColor)
{
	HBRUSH brush    = (HBRUSH)GetStockObject(NULL_BRUSH);
	HPEN   pen      = CreatePen(PS_SOLID, (int)m_ulLineWidth, ulColor);
	HPEN   oldPen   = (HPEN)SelectObject(hDC, (HGDIOBJ)pen);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, (HGDIOBJ)brush);

// 	IVS_LOG(IVS_LOG_DEBUG, "Show IATrack Polyline", 
// 		"[%x]Is Percent: %lu", this, AlarmPolyline.ulIsPercent);

// 	IVS_LOG(IVS_LOG_DEBUG, "Show IATrack Polyline", "[%x]Original Point[0]: %d, %d", 
// 		this, AlarmPolyline.Points[0].x, AlarmPolyline.Points[0].y);
	// ת���ٷֱ�����Ϊͼ������
	if (AlarmPolyline.ulIsPercent)
	{
		AlarmPolyline.Points[0].x = AlarmPolyline.Points[0].x * (LONG)m_ulDCWidth  / 100;
		AlarmPolyline.Points[0].y = AlarmPolyline.Points[0].y * (LONG)m_ulDCHeight / 100;
		IVS_LOG(IVS_LOG_DEBUG, "Show IATrack Polyline", "[%x]Adjust Point[0]: %d, %d", 
			this, AlarmPolyline.Points[0].x, AlarmPolyline.Points[0].y);
	}

	MoveToEx(hDC, AlarmPolyline.Points[0].x, AlarmPolyline.Points[0].y, NULL);
	for (ULONG i = 1; i < AlarmPolyline.ulPointsNum; i++)
	{
// 		IVS_LOG(IVS_LOG_DEBUG, "Show IATrack Polyline", "[%x]Original Point[%lu]: %d, %d", 
// 			this, i, AlarmPolyline.Points[i].x, AlarmPolyline.Points[i].y);
		if (AlarmPolyline.ulIsPercent)
		{
			AlarmPolyline.Points[i].x = AlarmPolyline.Points[i].x * (LONG)m_ulDCWidth  / 100;
			AlarmPolyline.Points[i].y = AlarmPolyline.Points[i].y * (LONG)m_ulDCHeight / 100;
			IVS_LOG(IVS_LOG_DEBUG, "Show IATrack Polyline", "[%x]Adjust Point[%lu]: %d, %d", 
				this, i, AlarmPolyline.Points[i].x, AlarmPolyline.Points[i].y);
		}
		LineTo(hDC, AlarmPolyline.Points[i].x, AlarmPolyline.Points[i].y);
	}

	AlarmPolyline.ulIsPercent = 0;

    if (AlarmPolyline.ulDirection)
    {
        IA_ARROW ArrowPointSet;
        memset(&ArrowPointSet, 0, sizeof(ArrowPointSet));
        (void)CalcuArrowPoints(AlarmPolyline, ArrowPointSet);
        (void)ShowArrowShape(hDC, ArrowPointSet, AlarmPolyline.ulDirection);
    }



	// �ͷ���Դ
	SelectObject(hDC,oldPen);
	SelectObject(hDC,oldBrush);
	DeleteObject(pen);

	return IVS_SUCCEED;
}


// ��������
LONG CIATrack::CreatFont(/*ULONG ulTextScale*/) 
{
	if (NULL != m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}

// 	GetObject(GetStockObject(ANSI_FIXED_FONT), sizeof(LOGFONT), &m_lf); 

	/****************** Windows��������ṩ���㷨****************************************
	//�����ֵ���豸�ġ��߼��ֱ��ʡ����Ǻ�
 	FLOAT cyDpi = (FLOAT) GetDeviceCaps (hDC, LOGPIXELSY) ;
	//�����ֵ���豸�ġ��߼��ֱ��ʡ����Ǻ�
 	FLOAT cyDpi = (FLOAT) (25.4 * GetDeviceCaps (hDC, VERTRES) / GetDeviceCaps (hDC, VERTSIZE)) ;

 	FLOAT y = 200 * cyDpi / 72;
 	m_lf.lfHeight = - (int)(fabs(y) / 10.0 + 0.5) ; // ��Ҫinclude<math.h>
	*********************************************************************************/


	/*************************MSDN�ṩ���㷨*******************************************
 	m_lf.lfHeight = -MulDiv(20, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	*********************************************************************************/


//   	m_lf.lfHeight = -MulDiv(IA_BASE_FONT_SIZE * (int)ulTextScale, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	memset(&m_lf, 0, sizeof(LOGFONT));
	m_lf.lfHeight = - (LONG)m_ulCurrentFontSize;
// 	m_lf.lfWeight = FW_SEMIBOLD;
	if (12 > m_ulCurrentFontSize)
	{
		m_lf.lfWeight = FW_NORMAL;
	} else if (20 > m_ulCurrentFontSize)
	{
		m_lf.lfWeight = FW_MEDIUM;
	} else if (30 > m_ulCurrentFontSize){
		m_lf.lfWeight = FW_SEMIBOLD;
	} else {
		m_lf.lfWeight = FW_BOLD;
	}

	m_lf.lfCharSet = DEFAULT_CHARSET;
	memcpy_s(m_lf.lfFaceName, sizeof(m_lf.lfFaceName), "Arial\0", strlen("Arial\0"));

	m_hFont = CreateFontIndirect(&m_lf);  

	if (NULL == m_hFont)
	{
		IVS_LOG(IVS_LOG_ERR, "Creat Font", "CreatFont Error");
		return IVS_ALLOC_MEMORY_ERROR;
	}

	return IVS_SUCCEED;
}


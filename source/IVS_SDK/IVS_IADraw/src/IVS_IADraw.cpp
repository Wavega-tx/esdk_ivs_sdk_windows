#include "IVS_IADraw.h"
#include "IVSCommon.h"
#include "ivs_log.h"
#include "ivs_error.h"
#include <math.h>

// ��͸��ͼ����ʾʱ��Ҫ�õ�Msimg32.lib
#pragma comment(lib,"Msimg32.lib")

#define CHECK_RESULT(s, _ret)\
	do { if(s) return _ret;}while(0)


#define CHECK_RESULT_TRUE_LOG(s, _FunctionName, _ErrLog)\
	do { if(s)											\
		{IVS_LOG(IVS_LOG_ERR, _FunctionName, _ErrLog);	\
		return TRUE;}									\
	}while(0)


#define POINT_NUM_OF_RECTANGLE			4			// ���ζ������
#define POINT_NUM_OF_ULINE				4			// U���ߵ���
#define POINTNO_ILLEGAL					-1			// ���޸ĵ�ͼ�ζ����±�Ĭ��ֵ
#define ULine_BOTTOM_WIDTH_MIN			40			// U���ߵײ�ˮƽ����С���
#define CLOSE_TO_LINE_DISTANCE_ON_WND	5			// ����ٽ��������룺X * X + Y * Y
#define CLOSE_TO_LINE_DISTANCE			(m_lDistanceCloseToLine * m_lDistanceCloseToLine)// ���߶��ٽ��������룺X * X + Y * Y
#define MAX_DISTANCE_VAL				0x7FFFFFFF	// ������ֵ��LONG�ͣ�
#define ARROW_LENTH						40
#define FONT_SIZE						20
#define MAX_RESOLUTIONG_RATIO			4096		// ���ֱ���


//���ܷ�����ͼ״̬
#define IA_DONOTHING						0		// ��
#define IA_MODIFING_MOVING_SHAPE			1		// �����ƶ�����ͼ��
#define IA_MODIFING_STRAIGHT_LINE_POINT		2		// �޸�ֱ�ߵ���������
#define IA_MODIFING_RECTANGLE_HORIZON_LINE	3		// �޸ľ���ˮƽ�ߣ����������ƶ���
#define IA_MODIFING_RECTANGLE_VERTICAL_LINE	4		// �޸ľ��δ�ֱ�ߣ����������ƶ���
#define IA_MODIFING_RECTANGLE_POINT			5		// �޸ľ��ζ��㣨����Ŵ���С��
#define IA_MODIFING_POLYGON_MOVING_POINT	6		// �޸Ķ���ε���������
#define IA_MODIFING_POLYGON_DELETE_POINT	7		// ɾ������ε�����
#define IA_MODIFING_POLYGON_ADD_POINT		8		// �ڶ����ĳ��������������
#define IA_MODIFING_ULINE_POINT				9		// �޸�U���߶˵�
#define IA_MODIFING_ULINE_ANGLE				10		// �޸�U����ˮƽ�����˵Ľ�


CIADrawer::CIADrawer()
	:m_ulGraphicID(0)
	,m_ulMaxDrawNum(0)
	,m_ulCurrentState(IA_DONOTHING)
	,m_ulCurrentCursor(CURSOR_TYPE_ARROW)
	,m_ulCurrentGraphicType(IA_GRAPHICS_TYPE_ILLEGAL)
	,m_ulMaxGroupShapeNum(0)
	,m_hMsgWnd(NULL)
	,m_ulWndWidth(0)
	,m_ulWndHeight(0)
	,m_ulDCWidth(0)
	,m_ulDCHeight(0)
	,m_fDiv(1)
	,m_lLineWidthAdjust(1)
	,m_lDistanceCloseToLine(0)
	,m_lArrowLenth(0)
	,m_lFontSize(0)
	,m_bScaleModeFull(FALSE)
	,m_lOffsetX(0)
	,m_lOffsetY(0)
	,m_bActive(FALSE)
	,m_bWindowPoint(FALSE)
	,m_bReversePoint(FALSE)
	,m_lPointNo(POINTNO_ILLEGAL)
	,m_memhdc(NULL)
	,m_hbitmap(NULL)
	,m_pvBits(NULL)
	,m_hFont(NULL)
	,m_ulUlineModifyIndex(0)
{
	memset(&m_IAGraphic, 0, sizeof(IA_INNER_GRAPHICS));
	memset(&m_RecordPoint, 0, sizeof(POINT));
	memset(m_OriginIAPoints, 0, sizeof(POINT) * POINTS_NUM_MAX);
	memset(&m_bf, 0, sizeof(BLENDFUNCTION));
	memset(&m_bmi, 0, sizeof(BITMAPINFO));
	memset(&m_lf, 0, sizeof(LOGFONT));

}

CIADrawer::~CIADrawer()
{
	if (NULL != m_hbitmap)
	{
		DeleteObject(m_hbitmap);
		m_hbitmap = NULL;
	}
	if (NULL != m_memhdc)
	{
		DeleteDC(m_memhdc);
		m_memhdc = NULL;
	}
	if (NULL != m_hFont)
	{
		DeleteObject(m_hFont); 
		m_hFont = NULL;
	}
	
}

// ��ʼ�����ܷ�����ͼ
LONG CIADrawer::InitIADrawer(HWND hMsgWnd)
{
	if (!IsWindow(hMsgWnd))
	{
		IVS_LOG(IVS_LOG_ERR, "Init IA Drawer", "hMsgWnd NULL");
		return IVS_PARA_INVALID;
	}
    m_hMsgWnd = hMsgWnd;

	RECT Wndrect;
    memset(&Wndrect, 0, sizeof(Wndrect));
	GetClientRect(m_hMsgWnd, &Wndrect); 
	m_ulWndWidth  = (ULONG)(Wndrect.right  - Wndrect.left);		// ���ڿ��
	m_ulWndHeight = (ULONG)(Wndrect.bottom - Wndrect.top);		// ���ڸ߶�
	// �жϴ��ڳ����Ƿ�Ϊ0
	if (( 0 == m_ulWndWidth) || (0 == m_ulWndHeight))
	{
		IVS_LOG(IVS_LOG_ERR, "Init IA Drawer", "hMsgWnd Size 0");
// 		return IVS_FAIL;
	}

	m_ulGraphicID		= 0;
	m_ulDCWidth			= 0;
	m_ulDCHeight		= 0;
	m_fDiv				= 1;
	m_lLineWidthAdjust	= 1;
	m_lDistanceCloseToLine = CLOSE_TO_LINE_DISTANCE_ON_WND;
	m_lArrowLenth		= ARROW_LENTH;
	m_lFontSize			= 0;
	m_bScaleModeFull	= FALSE;
	m_lOffsetX			= 0;
	m_lOffsetY			= 0;

	m_ulCurrentState	= IA_DONOTHING;
	m_ulCurrentCursor	= CURSOR_TYPE_ARROW;
	m_ulCurrentGraphicType = IA_GRAPHICS_TYPE_ILLEGAL;
	m_ulMaxDrawNum		= GRAPHICS_NUM_MAX;
	m_ulMaxGroupShapeNum  = 0;
	m_lPointNo			= POINTNO_ILLEGAL;
	m_RecordPoint.x		= 0;
	m_RecordPoint.y		= 0;
	m_bActive			= FALSE;
	m_bWindowPoint		= FALSE;
	m_bReversePoint		= FALSE;

	memset(&m_IAGraphic, 0, sizeof(IA_INNER_GRAPHICS));
	memset(&m_RecordPoint, 0, sizeof(POINT));
	memset(m_OriginIAPoints, 0, sizeof(POINT) * POINTS_NUM_MAX);
	memset(&m_bf, 0, sizeof(BLENDFUNCTION));
	memset(&m_bmi, 0, sizeof(BITMAPINFO));
	memset(&m_lf, 0, sizeof(LOGFONT));
	m_IADrawGraphicsList.clear();
	m_IARelationShipList.clear();
	
	return IVS_SUCCEED;
}

// �����Ƿ���������ʾ
VOID CIADrawer::SetDisplayScaleMode(BOOL bScaleModeFull)
{
	m_bScaleModeFull = bScaleModeFull;
}

// ת����������Ϊͼ������
LONG CIADrawer::WindowPointToGraphicPoint(POINT *point)
{
	IVS_TRACE();
	if ((0 == m_ulWndWidth) || (0 == m_ulWndHeight))
	{
		IVS_LOG(IVS_LOG_ERR, "Window Point To Graphic Point", "Window Size is Zero");
		return IVS_FAIL;
	}

	GetOffset();
	point->x = (point->x - m_lOffsetX) * (LONG)m_ulDCWidth  
		/ ((LONG)m_ulWndWidth - 2 * m_lOffsetX);
	point->y = (point->y - m_lOffsetY) * (LONG)m_ulDCHeight 
		/ ((LONG)m_ulWndHeight - 2 * m_lOffsetY);

	// �жϵ��Ƿ���ͼ�����
	if (IsPointInside(point))
	{
		return IVS_SUCCEED;
	}
	IVS_LOG(IVS_LOG_ERR, "Window Point To Graphic Point", "Point Outside Graphic");
	return IVS_FAIL;
	
}

// �������ܷ���ͼ��
LONG CIADrawer::SetIAGraphics(const IA_GRAPHICS *pGraphics, ULONG ulNum, BOOL bPersent)
{
	IVS_TRACE();
	if ((NULL == pGraphics) || (0 == ulNum))
	{
		IVS_LOG(IVS_LOG_ERR, "Set IA Graphics", "Invalid Argument");
		return IVS_PARA_INVALID;
	}
	// �жϴ����ͼ���������Ƿ��в��Ϸ�ͼ��
	if (!bPersent && IsSetGraphicsInvalid(pGraphics, ulNum))
	{
		return IVS_PARA_INVALID;
	}

	CAutoLock Lock(m_Mutex);

	LONG lRet = IVS_FAIL;
	memset(&m_IAGraphic, 0, sizeof(IA_INNER_GRAPHICS));
	for (ULONG i = 0; i < ulNum; i++)
	{
		// �ж�ͼ�κϷ��󣬽�INNERͼ�ο�����m_IAGraphic�ṹ����
		memcpy_s(&m_IAGraphic.iaGraphic, sizeof(IA_GRAPHICS), &pGraphics[i], sizeof(IA_GRAPHICS));
		m_IAGraphic.ulGraphicsID = m_ulGraphicID++;
		m_IAGraphic.bFinish      = TRUE;
		m_IAGraphic.bEditing	 = FALSE;
		m_IAGraphic.bPersent	 = bPersent;
		if (m_IAGraphic.iaGraphic.ulArrowType && !bPersent)
		{
			GetArrowPoints(m_IAGraphic.iaGraphic);
		}

		lRet = InsertCurrentToList();
	}
	
	return lRet;
}

// �ж����õ����ܷ���ͼ���Ƿ�Ϸ�
BOOL CIADrawer::IsSetGraphicsInvalid(const IA_GRAPHICS *pGraphics, ULONG ulNum)
{
	BOOL bRet = FALSE;
	for (ULONG i = 0; i < ulNum; i++)
	{
		// �ж�ͼ�������Ƿ�Ϸ���ͼ�����ͱ�ŵ�5
		bRet = 5 < pGraphics[i].ulGraphicType;
		CHECK_RESULT_TRUE_LOG(bRet, "Is Set Graphics Invalid", "GraphicType Invalid");

		// ���õ�ͼ�ε�������������
		bRet = POINTS_NUM_MAX < pGraphics[i].ulPointsNum;
		CHECK_RESULT_TRUE_LOG(bRet, "Is Set Graphics Invalid", "PointsNum Invalid");

		// �ҵ�ͼ�α߽����
		RECT rc = {0};
		bRet = !FindEdgeRectangle(pGraphics[i].IAPoints, pGraphics[i].ulPointsNum, rc);
		CHECK_RESULT_TRUE_LOG(bRet, "Is Set Graphics Invalid", "Find Edge Rectangle Failed");

		// �жϾ����Ƿ񳬳����ֱ��ʷ�Χ
		bRet = (0 > rc.left) || (rc.right >= MAX_RESOLUTIONG_RATIO) 
				|| (0 > rc.top) || (rc.bottom >= MAX_RESOLUTIONG_RATIO);
		CHECK_RESULT_TRUE_LOG(bRet, "Is Set Graphics Invalid", "Graphic Outside the Window");

		// �ж����ý�����ͼ���Ƿ����Ѵ��ڵ�ͼ����λ�ù�ϵ�ķǷ��ص�
		bRet = IsForbiddenOverlaped(pGraphics[i].IAPoints, pGraphics[i].ulPointsNum, 
				pGraphics[i].ulGraphicGroup, pGraphics[i].ulGraphicType);
		CHECK_RESULT_TRUE_LOG(bRet, "Is Set Graphics Invalid", "Graphic Illegally Overlaped");

		// �ж�ͼ������λ���Ƿ�Ϸ���ֻ��0��1��2���֣�
		bRet = (pGraphics[i].NamePosition >= GRAPHIC_NAME_OTHER);
		CHECK_RESULT_TRUE_LOG(bRet, "Is Set Graphics Invalid", "NamePosition Invalid");
	}
	return bRet;

}

// �������ܷ���ͼ����֮��Ĺ�ϵ
LONG CIADrawer::SetRelationShip(ULONG ulGraphicGroupA, ULONG ulGraphicGroupB, 
								RELATION_SHIP_TYPE ulRelationShipType)
{
	IVS_TRACE();
	// ��ȡͼ��λ�ù�ϵ���෴��ϵ,�ԳɶԷ�ʽ����ڹ�ϵ�б���
	RELATION_SHIP_TYPE ulRelationShipType_Opposite = 
		GetOppositeType(ulRelationShipType);

	CAutoLock Lock(m_Mutex);
	BOOL bNoRecord = TRUE;
	IA_RELATIONS_LIST_ITER RelationIter    = m_IARelationShipList.begin();
	IA_RELATIONS_LIST_ITER RelationIterEnd = m_IARelationShipList.end();
		
	while (RelationIter != RelationIterEnd)
	{
		IA_RELATIONS *pRelationShip = *RelationIter;
		if (NULL == pRelationShip)
		{
			RelationIter = m_IARelationShipList.erase(RelationIter);
			continue;
		}
		RelationIter++;

		// ������ϵ�б�,��������ͬ�� ��ǰGraphicGroup �� ���GraphicGroup
		if ((ulGraphicGroupA == pRelationShip->ulCurrentGraphicGroup) 
			&& (ulGraphicGroupB == pRelationShip->ulRelativeGraphicGroup))
		{
			// �����ж�Ӧ�������������õ���ͬ,��ֱ�ӷ���
			if (ulRelationShipType == pRelationShip->ulRelationShipType)
			{
				IVS_LOG(IVS_LOG_INFO, "Set RelationShip", "Relation Exists");
				return IVS_SUCCEED;
			}
			// ���ж�Ӧ�����������õĲ�ͬ,���ʶ���м�¼����,����λ�ù�ϵ����
			bNoRecord = FALSE;
			pRelationShip->ulRelationShipType = ulRelationShipType;
			
			// ����������ͼ���������λ�ù�ϵ,����Ҫ�ɶԳ���,����Ҫ��������һ��λ�ù�ϵ����,ֱ�ӷ��� 
			if (ulGraphicGroupA == ulGraphicGroupB)
			{
				IVS_LOG(IVS_LOG_INFO, "Set RelationShip", "Relation Updated");
				return IVS_SUCCEED;
			}
			break;
		}
	}
	// �б���û�������õ�λ�ù�ϵ��¼
	if (bNoRecord)
	{
		LONG lRet = IVS_FAIL;
		// �����õ�λ�ù�ϵ���뵽λ�ù�ϵ�б���
		lRet = InsertIntoRelationShipList(ulGraphicGroupA, ulGraphicGroupB, 
											ulRelationShipType);	

		// ������ͼ�������λ�ù�ϵ,����Ҫ����ɶԵ�λ�ù�ϵ�ڵ�
		if ((IVS_SUCCEED == lRet) && (ulGraphicGroupA != ulGraphicGroupB))
		{
			lRet = InsertIntoRelationShipList(ulGraphicGroupB, ulGraphicGroupA, 
												ulRelationShipType_Opposite);
		}
		// �޼�¼�����꣬�߼��������뷵��
		return lRet;
	}

	// λ�ù�ϵ�б��м�¼��λ�ù�ϵ������Ҫ���¡����Ҳ�������ͼ���������λ�ù�ϵ��
	// ����Ҫ�����б��ҵ������ɶԵĽڵ㣬�޸�����Ӧλ�ù�ϵ����
	RelationIter = m_IARelationShipList.begin();
	while (RelationIter != RelationIterEnd)
	{
		IA_RELATIONS *pRelationShip = *RelationIter;
		if (NULL == pRelationShip)
		{
			RelationIter = m_IARelationShipList.erase(RelationIter);
			continue;
		}
		RelationIter++;
		if ((ulGraphicGroupB == pRelationShip->ulCurrentGraphicGroup) 
			&& (ulGraphicGroupA == pRelationShip->ulRelativeGraphicGroup))
		{
			pRelationShip->ulRelationShipType = ulRelationShipType_Opposite;
			IVS_LOG(IVS_LOG_INFO, "Set RelationShip", "Relation Updated");
			break;
		}
	}
	return IVS_SUCCEED;
}

// ��ȡ�෴��ͼ�����ϵ
RELATION_SHIP_TYPE CIADrawer::GetOppositeType(RELATION_SHIP_TYPE ulRelationShipType) 
{
	RELATION_SHIP_TYPE RetValue = IA_GRAPHICS_RELATION_TYPE_NONE;
	switch (ulRelationShipType)
	{
	case IA_GRAPHICS_RELATION_TYPE_BE_COVERED:
		RetValue = IA_GRAPHICS_RELATION_TYPE_COVER;
		break;
	case IA_GRAPHICS_RELATION_TYPE_COVER:
		RetValue = IA_GRAPHICS_RELATION_TYPE_BE_COVERED;
		break;
	case IA_GRAPHICS_RELATION_TYPE_NOT_CROSS:
		RetValue = IA_GRAPHICS_RELATION_TYPE_NOT_CROSS;
		break;
	case IA_GRAPHICS_RELATION_TYPE_SMALLER:
		RetValue = IA_GRAPHICS_RELATION_TYPE_BIGGER;
		break;
	case IA_GRAPHICS_RELATION_TYPE_BIGGER:
		RetValue = IA_GRAPHICS_RELATION_TYPE_SMALLER;
		break;
	default: // �Ƿ�λ�ù�ϵ���͵ķ���IA_GRAPHICS_RELATION_TYPE_NONE
		break;
	}
	return RetValue;
}

// ��λ�ù�ϵ���뵽λ�ù�ϵ�б���
LONG CIADrawer::InsertIntoRelationShipList(ULONG ulGraphicGroupA, ULONG ulGraphicGroupB, RELATION_SHIP_TYPE ulRelationShipType) 
{
	IA_RELATIONS *pRelationShip = NULL;
	if (NULL == IVS_NEW(pRelationShip))
	{
		IVS_LOG(IVS_LOG_ERR, "Set RelationShip", "No Enough Memory");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	memset(pRelationShip, 0, sizeof(IA_RELATIONS));
	pRelationShip->ulCurrentGraphicGroup  = ulGraphicGroupA;
	pRelationShip->ulRelativeGraphicGroup = ulGraphicGroupB;
	pRelationShip->ulRelationShipType	  = ulRelationShipType;
	m_IARelationShipList.push_back(pRelationShip);
	pRelationShip = NULL;
	return IVS_SUCCEED;
}

// ��ȡ���ܷ���ͼ�Σ�pulNum��Ϊ���Ϊ�ɻ�ȡ�����ͼ��������Ϊ����Ϊ��ȡ����ͼ������
LONG CIADrawer::GetIAGraphics(ULONG ulGraphicGroup, IA_GRAPHICS *pGraphics, ULONG *pulNum)
{
	IVS_TRACE();
	if (NULL == pGraphics || (NULL == pulNum))
	{
		IVS_LOG(IVS_LOG_ERR, "Get IA Graphics", "Invalid Argument");
		return IVS_PARA_INVALID;
	}
    CAutoLock Lock(m_Mutex);
	if (m_IADrawGraphicsList.empty())
	{
		*pulNum = 0;
		IVS_LOG(IVS_LOG_INFO, "Get IA Graphics", "No Graphics");
		return IVS_SUCCEED;
	}

	ULONG ulNum = 0; 
	IA_GRAPHICS_LIST_ITER iter    = m_IADrawGraphicsList.begin();
	IA_GRAPHICS_LIST_ITER iterEnd = m_IADrawGraphicsList.end();
	while (iter != iterEnd)
	{
		IA_INNER_GRAPHICS *pIAGraphic = *iter;
		if (NULL == pIAGraphic)
		{
			iter = m_IADrawGraphicsList.erase(iter);
			continue;
		}
		if (((ulGraphicGroup == pIAGraphic->iaGraphic.ulGraphicGroup) // ��ȡ��Ӧ��GraphicGroupͼ��
			|| !ulGraphicGroup)			// ��GraphicGroupΪ0ʱ����ȡȫ��ͼ��
			&& (ulNum < *pulNum))		// ��GraphicGroup��ͼ���������ڿɽ���ͼ����ʱ�����ٻ�ȡ
		{
			memcpy_s(&pGraphics[ulNum++], sizeof(IA_GRAPHICS), &pIAGraphic->iaGraphic, sizeof(IA_GRAPHICS));
		}
		iter++;
	}
	*pulNum = ulNum;
	return IVS_SUCCEED;
}

// ��ȡѡ�е�ͼ��
LONG CIADrawer::GetIAGraphicSelected(IA_GRAPHICS *pGraphic)
{
	IVS_TRACE();
	if (NULL == pGraphic)
	{
		IVS_LOG(IVS_LOG_ERR, "Get IA Graphics Selected", "Invalid Argument");
		return IVS_PARA_INVALID;
	}
	CAutoLock Lock(m_Mutex);
	if (m_IADrawGraphicsList.empty())
	{
		IVS_LOG(IVS_LOG_INFO, "Get IA Graphics Selected", "No Graphics");
		return IVS_SUCCEED;
	}
	IA_GRAPHICS_LIST_ITER iter    = m_IADrawGraphicsList.begin();
	IA_GRAPHICS_LIST_ITER iterEnd = m_IADrawGraphicsList.end();
	while (iter != iterEnd)
	{
		IA_INNER_GRAPHICS *pIAGraphic = *iter;
		if (NULL == pIAGraphic)
		{
			iter = m_IADrawGraphicsList.erase(iter);
			continue;
		}
		if (pIAGraphic->bEditing) // ��ȡ���ڱ༭ͼ��
		{
			memcpy_s(pGraphic, sizeof(IA_GRAPHICS), &pIAGraphic->iaGraphic, sizeof(IA_GRAPHICS));
			return IVS_SUCCEED;
		}
		iter++;
	}
	IVS_LOG(IVS_LOG_INFO, "Get IA Graphics Selected", "No Selected Graphics");
	return IVS_SUCCEED;
}


// ���ÿ�ʼ��ͼ����ʼ״̬
LONG CIADrawer::SetIAStartStatus(ULONG ulNum, const IA_GRAPHICS *pIAGraphics)
{
	IVS_TRACE();
	CAutoLock Lock(m_Mutex);
	if (m_IADrawGraphicsList.size() >= m_ulMaxDrawNum)
	{
		IVS_LOG(IVS_LOG_ERR, "Set IA Start Position", 
			"%u Reach MAX Graphics Num", m_ulMaxDrawNum);
		return IVS_FAIL;
	}
	
	// ����ǰͼ�������ͼ����¼�� m_ulMaxGroupShapeNum
	m_ulMaxGroupShapeNum = ulNum; 
	// ����ǰͼ�����Ѿ��������ͼ�������ش���
	if (IsFinishDrawGraphicGroup(pIAGraphics->ulGraphicGroup, ulNum))
	{
		IVS_LOG(IVS_LOG_INFO, "Set IA Start Position", "Reach Max Draw Number");
		return IVS_FAIL;
	}

	// �ж���ʼ���Ƿ��ڷǷ�������
	if (IsInForbiddenArea(pIAGraphics->IAPoints[0], pIAGraphics->ulGraphicGroup))
	{
		IVS_LOG(IVS_LOG_ERR, "Set IA Start Position", "Point Invalid");
		return IVS_FAIL;
	}

	memset(&m_IAGraphic, 0, sizeof(IA_INNER_GRAPHICS));
	memcpy_s(&m_IAGraphic.iaGraphic, sizeof(IA_GRAPHICS), pIAGraphics, sizeof(IA_GRAPHICS));
	m_IAGraphic.iaGraphic.ulPointsNum = 1;
	// �ڶ�����������ͬ����ʼ��Ϊ��һ�������꣬����ֱ�ߡ����λ������0,0�����ͼ��
	m_IAGraphic.iaGraphic.IAPoints[1] = pIAGraphics->IAPoints[0];
	m_IAGraphic.iaGraphic.IAPoints[2] = pIAGraphics->IAPoints[0];
	m_IAGraphic.ulGraphicsID	= m_ulGraphicID++;
	m_IAGraphic.bFinish			= FALSE;
	m_IAGraphic.bEditing		= TRUE;
	m_bActive					= TRUE;

	return IVS_SUCCEED;
}

// ���ܷ�����ֱ��
LONG CIADrawer::DrawIAStraightLine(POINT point, BOOL bFinish, BOOL bIsDrawing)
{
	// �ڿ�ʼ��ͼʱ�Ѿ������m_IAGraphic������˵����ʼ��ͼʧ��
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Draw IA Straight Line", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}
	// ����ǰ��������ʼ��ӽ�����ͼʧ�ܷ���
	if (IsCloseToPoint(point, m_IAGraphic.iaGraphic.IAPoints[0]))
	{
		IVS_LOG(IVS_LOG_ERR, "Draw IA Straight Line", "Invalid Point");
		return IVS_PARA_INVALID;
	}
	if (m_IAGraphic.iaGraphic.bKeepHorizon)
	{
		point.y = m_IAGraphic.iaGraphic.IAPoints[0].y;
	}
	m_IAGraphic.iaGraphic.IAPoints[1] = point;
	m_IAGraphic.iaGraphic.ulPointsNum = 2;
	m_IAGraphic.bFinish				  = bFinish;

	if (m_IAGraphic.iaGraphic.ulArrowType)
	{
		GetArrowPoints(m_IAGraphic.iaGraphic);
	}

	// �ж��Ƿ���ɻ�ͼ������ͼ���б��Ƿ�֪ͨ�ص�	
	return FinishAndInsertIntoGraphicList(bIsDrawing);
}

// �����ͷ����
VOID CIADrawer::GetArrowPoints(IA_GRAPHICS &iaGraphic)
{
	IA_ARROW iaArrow;
	memset(&iaArrow, 0, sizeof(iaArrow));
	
	LONG dStraightLineWidth  = iaGraphic.IAPoints[1].x - iaGraphic.IAPoints[0].x;
	LONG dStraightLineHeight = iaGraphic.IAPoints[1].y - iaGraphic.IAPoints[0].y;
	DOUBLE dLenth = (DOUBLE)dStraightLineWidth * (DOUBLE)dStraightLineWidth 
					+ (DOUBLE)dStraightLineHeight * (DOUBLE)dStraightLineHeight;
	LONG lStraightLineLenth  = (LONG)sqrt(dLenth);

	for (unsigned int i = 1; i <= 2; i++)
	{
		if (i & iaGraphic.ulArrowType)
		{
			iaArrow.ArrowTail[i - 1].x = iaGraphic.IAPoints[0].x + dStraightLineWidth  / 2;
			iaArrow.ArrowTail[i - 1].y = iaGraphic.IAPoints[0].y + dStraightLineHeight / 2;

			LONG dArrowWidth  = (LONG)pow((DOUBLE)-1, (DOUBLE)(i - 1)) * dStraightLineHeight * m_lArrowLenth / lStraightLineLenth;
			LONG dArrowHeight = (LONG)pow((DOUBLE)-1, (DOUBLE)(i))     * dStraightLineWidth  * m_lArrowLenth / lStraightLineLenth;

			iaArrow.ArrowHead[i - 1].x = iaArrow.ArrowTail[i - 1].x + dArrowWidth;
			iaArrow.ArrowHead[i - 1].y = iaArrow.ArrowTail[i - 1].y + dArrowHeight;

			POINT FourFifthOnArrow = {0, 0};
			FourFifthOnArrow.x = iaArrow.ArrowTail[i - 1].x + dArrowWidth  * 4 / 5;
			FourFifthOnArrow.y = iaArrow.ArrowTail[i - 1].y + dArrowHeight * 4 / 5;

			LONG dArrowHeadWidth  = - dArrowHeight / 5;
			LONG dArrowHeadHeight =   dArrowWidth  / 5;

			iaArrow.ArrowTrangle1[i - 1].x = FourFifthOnArrow.x + dArrowHeadWidth;
			iaArrow.ArrowTrangle1[i - 1].y = FourFifthOnArrow.y + dArrowHeadHeight;

			iaArrow.ArrowTrangle2[i - 1].x = FourFifthOnArrow.x - dArrowHeadWidth;
			iaArrow.ArrowTrangle2[i - 1].y = FourFifthOnArrow.y - dArrowHeadHeight;

		}
	}
	memcpy_s(&iaGraphic.IAArrow, sizeof(IA_ARROW), &iaArrow, sizeof(IA_ARROW));

	return;
}

// ���ܷ���������
LONG CIADrawer::DrawIARectangle(POINT point, BOOL bFinish, BOOL bIsDrawing)
{
	// �ڿ�ʼ��ͼʱ�Ѿ������m_IAGraphic������˵����ʼ��ͼʧ��
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Draw IA Rectangle", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}
	// ����ǰ��������ʼ��ӽ�����ͼʧ�ܷ���
	if (IsCloseToPoint(point, m_IAGraphic.iaGraphic.IAPoints[0]) 
		|| point.x == m_IAGraphic.iaGraphic.IAPoints[0].x 
		|| point.y == m_IAGraphic.iaGraphic.IAPoints[0].y)
	{
		IVS_LOG(IVS_LOG_ERR, "Draw IA Rectangle", "Invalid Point");
		return IVS_PARA_INVALID;
	}
	if (IVS_SUCCEED != AdjustRectPoint(point))
	{
		IVS_LOG(IVS_LOG_ERR, "Draw IA Rectangle", "Adjust Rect Point Failed");
		return IVS_FAIL;
	}

	// �жϺϷ�ǰ����ʱ������¼��������ֵ
	POINT tmp_point[5];
	tmp_point[0]   = m_IAGraphic.iaGraphic.IAPoints[0];
	tmp_point[1].x = point.x;
	tmp_point[1].y = m_IAGraphic.iaGraphic.IAPoints[0].y;
	tmp_point[2]   = point;
	tmp_point[3].x = m_IAGraphic.iaGraphic.IAPoints[0].x;
	tmp_point[3].y = point.y;
	tmp_point[4]   = m_IAGraphic.iaGraphic.IAPoints[0];
	
	// �ж��Ƿ������е�ͼ��λ�ù�ϵ�Ƿ��ص�
	if (IsForbiddenOverlaped(tmp_point, 5, m_IAGraphic.iaGraphic.ulGraphicGroup, 
							m_IAGraphic.iaGraphic.ulGraphicType)
		|| IsRectangleSizeIllegal(tmp_point, 5, m_IAGraphic.iaGraphic.ulGraphicGroup))
	{
		if (bFinish && bIsDrawing)
		{
			m_bActive = FALSE;
			memset(&m_IAGraphic, 0, sizeof(IA_INNER_GRAPHICS));
		}
		IVS_LOG(IVS_LOG_ERR, "Draw IA Rectangle", "Point Illegal");
		return IVS_FAIL;
	}
	memcpy_s(m_IAGraphic.iaGraphic.IAPoints, 
		sizeof(POINT) * POINTS_NUM_MAX, 
		tmp_point, sizeof(POINT) * 5);
	m_IAGraphic.iaGraphic.ulPointsNum   = 5;
	m_IAGraphic.bFinish				   = bFinish;

	// �ж��Ƿ���ɻ�ͼ������ͼ���б��Ƿ�֪ͨ�ص�
	return FinishAndInsertIntoGraphicList(bIsDrawing);
}

LONG CIADrawer::AdjustRectPoint(POINT &point)
{
	if (!m_IAGraphic.iaGraphic.bKeepRectShape)
	{
		return IVS_SUCCEED;
	}
	if (0 == m_ulDCWidth || 0 == m_ulDCHeight)
	{
		IVS_LOG(IVS_LOG_ERR, "Adjust Rect Point", "DC Size is Zero");
		return IVS_FAIL;
	}
	POINT tmpPoint = point;
	LONG lWidth  = point.x - m_IAGraphic.iaGraphic.IAPoints[0].x;
	LONG lHeight = point.y - m_IAGraphic.iaGraphic.IAPoints[0].y;
	LONG tag = (LONG)m_ulDCWidth * abs(lHeight) - (LONG)m_ulDCHeight * abs(lWidth);
	if (0 < tag)
	{
		lWidth = (abs(lHeight) * (LONG)m_ulDCWidth / (LONG)m_ulDCHeight) 
			* (lWidth / abs(lWidth));
		tmpPoint.x = m_IAGraphic.iaGraphic.IAPoints[0].x + lWidth;
	}
	if (0 > tag)
	{
		lHeight = (abs(lWidth) * (LONG)m_ulDCHeight / (LONG)m_ulDCWidth) 
			* (lHeight / abs(lHeight));
		tmpPoint.y = m_IAGraphic.iaGraphic.IAPoints[0].y + lHeight;
	}
	if (!IsPointInside(&tmpPoint))
	{
		IVS_LOG(IVS_LOG_ERR, "Adjust Rect Point", "Point Illegal");
		return IVS_FAIL;
	}
	point.x = tmpPoint.x;
	point.y = tmpPoint.y;
	return IVS_SUCCEED;
}

// ���ܷ����������
LONG CIADrawer::DrawIAPolygon(POINT point, BOOL bNextPoint, BOOL bFinish, ULONG ulGraphicGroup)
{
	// �ڿ�ʼ��ͼʱ�Ѿ������m_IAGraphic������˵����ʼ��ͼʧ��
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Draw IA Polygon", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}

	// �ж�����������Ƿ�Ϸ�
	if (IVS_SUCCEED != DrawIAPolygonShape(point, bNextPoint, bFinish, ulGraphicGroup))
	{
		if (bFinish)
		{
			m_bActive = FALSE;
			memset(&m_IAGraphic, 0, sizeof(IA_INNER_GRAPHICS));
		}
		IVS_LOG(IVS_LOG_ERR, "Draw IA Polygon", "Draw Polygon Error");
		return IVS_FAIL;
	}

	// �ж��Ƿ���ɻ�ͼ������ͼ���б��Ƿ�֪ͨ�ص�
	return FinishAndInsertIntoGraphicList(TRUE);
}

// ���ܷ�����U����
LONG CIADrawer::DrawIAULine(POINT point, BOOL bNextPoint, BOOL bFinish, BOOL bIsDrawing)
{
	// �ڿ�ʼ��ͼʱ�Ѿ������m_IAGraphic������˵����ʼ��ͼʧ��
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Draw IA ULine", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}

	// ������ʱU���ߵ�������ȷ,��ɾ����ǰͼ��,ֱ�ӷ���
	if (bFinish && (POINT_NUM_OF_ULINE != m_IAGraphic.iaGraphic.ulPointsNum))
	{
		m_bActive = FALSE;
		memset(&m_IAGraphic, 0, sizeof(IA_INNER_GRAPHICS));
		IVS_LOG(IVS_LOG_ERR, "Draw IA ULine", "PointsNum Illegal");
		return IVS_PARA_INVALID;
	}

	// �ж�����U�����Ƿ�Ϸ�
	if (IVS_SUCCEED != DrawIAULineShape(point, bNextPoint, bIsDrawing))
	{
		if (bFinish)
		{
			m_bActive = FALSE;
			memset(&m_IAGraphic, 0, sizeof(IA_INNER_GRAPHICS));
		}
		IVS_LOG(IVS_LOG_ERR, "Draw IA ULine", "Draw ULine Error");
		return IVS_FAIL;
	}
	// �ж��Ƿ���ɻ�ͼ������ͼ���б��Ƿ�֪ͨ�ص�
	return FinishAndInsertIntoGraphicList(bIsDrawing);
}

// ���ܷ�������ĳͼ����Ļ�ͼ�����������ڻ��ĸ���ͼ�Σ�ֱ�Ӷ�����
LONG CIADrawer::StopDrawGraphics(ULONG ulGraphicGroup)
{
	IVS_TRACE();
	CAutoLock Lock(m_Mutex);

	if (m_bActive)
	{
		// �����GraphicGroup�뵱ǰ��һ�£�����ʧ��
		if (ulGraphicGroup != m_IAGraphic.iaGraphic.ulGraphicGroup)
		{
			IVS_LOG(IVS_LOG_INFO, "Stop Draw Graphics", "GraphicGroup Not Match");
			return IVS_FAIL;
		}
		m_bActive = FALSE;
		memset(&m_IAGraphic, 0, sizeof(IA_INNER_GRAPHICS));
	}

	(void)SelectCancel();

	return IVS_SUCCEED;
}


// ��ʾ���ܷ���ͼ��
LONG CIADrawer::ShowIAGraphics(HDC hDC, const RECT *rc)
{
// 	IVS_TRACE();

	// ���»�ͼ����
	LONG lRet = UpdateWnd(hDC, rc);
	if (IVS_SUCCEED != lRet)
	{
		IVS_LOG(IVS_LOG_ERR, "Show IA Graphics", "Update Window Failed");
		return lRet;
	}

	CAutoLock Lock(m_Mutex);

	// ��ʾͼ���б�������ͼ��
	IA_GRAPHICS_LIST_ITER iter    = m_IADrawGraphicsList.begin();
	IA_GRAPHICS_LIST_ITER iterEnd = m_IADrawGraphicsList.end();
	while (iter != iterEnd)
	{
		IA_INNER_GRAPHICS *pIAGraphic = *iter;
		if (NULL == pIAGraphic)
		{
			iter = m_IADrawGraphicsList.erase(iter);
			continue;
		}

		if (pIAGraphic->bPersent)
		{
			for (ULONG i = 0; i < pIAGraphic->iaGraphic.ulPointsNum; i++)
			{
				pIAGraphic->iaGraphic.IAPoints[i].x 
					= (LONG)(((float)pIAGraphic->iaGraphic.IAPoints[i].x 
						/ FLOAT_TO_LONG_COEFF) * m_ulDCWidth);
				pIAGraphic->iaGraphic.IAPoints[i].y 
					= (LONG)(((float)pIAGraphic->iaGraphic.IAPoints[i].y 
						/ FLOAT_TO_LONG_COEFF) * m_ulDCHeight);
			}
			pIAGraphic->bPersent = FALSE;
		}
		if (pIAGraphic->iaGraphic.ulArrowType)
		{
			GetArrowPoints(pIAGraphic->iaGraphic);
		}
		// �ж�ͼ���Ƿ���ͼ���ⲿ
		if (IsShowIAShapeInvalid(*pIAGraphic))
		{
			IVS_DELETE(pIAGraphic);
			iter = m_IADrawGraphicsList.erase(iter);
			IVS_LOG(IVS_LOG_ERR, "Show IA Graphics", "Find Invalid Image and Deleted");
			continue;
		}
		ShowIAShape(hDC, *pIAGraphic);
		iter++;
	}

	// ��ʾ���ڻ���ͼ��
	if (m_bActive)
	{
		if (m_bWindowPoint)
		{
			lRet = IVS_FAIL;
			lRet = WindowPointToGraphicPoint(&m_IAGraphic.iaGraphic.IAPoints[0]);
			lRet = WindowPointToGraphicPoint(&m_IAGraphic.iaGraphic.IAPoints[1]);
			lRet = WindowPointToGraphicPoint(&m_IAGraphic.iaGraphic.IAPoints[2]);

			if (IVS_SUCCEED != lRet)
			{
				IVS_LOG(IVS_LOG_ERR, "Show IA Graphics", "Current Graphic StartPoint Invalid");
				memset(&m_IAGraphic, 0, sizeof(IA_INNER_GRAPHICS));
				m_bActive = FALSE;
				m_bWindowPoint = FALSE;
				return IVS_FAIL;
			}
			m_bWindowPoint = FALSE;
		}
		ShowIAShape(hDC, m_IAGraphic);
	}
	return IVS_SUCCEED;
}


LONG CIADrawer::UpdateWnd(HDC hDC, const RECT *rc) 
{
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
		IVS_LOG(IVS_LOG_ERR, "Update Window", "hMsgWnd Size 0");
		return IVS_FAIL;
	}

	// ����ͼ���С(DC�ߴ�)
	ULONG DCWidth  = (ULONG)(rc->right - rc->left);
	ULONG DCHeight = (ULONG)(rc->bottom - rc->top);
	// �ж�DC�����Ƿ�Ϊ0
	if (( 0 == DCWidth) || (0 == DCHeight))
	{
		IVS_LOG(IVS_LOG_ERR, "Update Window", "hDC Size 0");
		return IVS_FAIL;
	}
	if ((DCWidth != m_ulDCWidth) || (DCHeight != m_ulDCHeight))
	{
		m_ulDCWidth  = DCWidth;
		m_ulDCHeight = DCHeight;

		if (IVS_SUCCEED != CreatBitmapDC(hDC))
		{
			return IVS_FAIL;
		}
		IVS_LOG(IVS_LOG_INFO, "Update Window", "Image Size Changed");
	}
	m_bWindowPoint = FALSE;

	GetOffset();
	m_lDistanceCloseToLine = (LONG)max(2, CLOSE_TO_LINE_DISTANCE_ON_WND * m_fDiv);
	m_lArrowLenth		   = (LONG)max(10, ARROW_LENTH * m_fDiv);
	LONG lFontsize		   = (LONG)max(12, FONT_SIZE * m_fDiv);
	if (lFontsize != m_lFontSize)
	{
		IVS_LOG(IVS_LOG_INFO, "Update Window", "Font Size Changed: %d to %d", 
			m_lFontSize, lFontsize);
		m_lFontSize = lFontsize;
		CreateIAFont();
	}

	return IVS_SUCCEED;
}

void CIADrawer::GetOffset() 
{
	if ((0 == m_ulWndWidth) || (0 == m_ulWndHeight))
	{
		IVS_LOG(IVS_LOG_ERR, "Get Offset", "Window Size is Zero");
		return;
	}
	if ((0 == m_ulDCWidth) || (0 == m_ulDCHeight))
	{
		IVS_LOG(IVS_LOG_INFO, "Get Offset", "DC Size is Zero");
		return;
	}
	m_lOffsetX = 0;
	m_lOffsetY = 0;

	if (!m_bScaleModeFull)
	{	
		LONG tag = (LONG)(m_ulDCWidth * m_ulWndHeight) - (LONG)(m_ulDCHeight * m_ulWndWidth);
		if (0 < tag)
		{
			m_lOffsetX = 0;
			m_lOffsetY = (LONG)(m_ulWndHeight - m_ulDCHeight * m_ulWndWidth / m_ulDCWidth) / 2;
		}
		if (0 > tag)
		{
			m_lOffsetX = (LONG)(m_ulWndWidth - m_ulDCWidth * m_ulWndHeight / m_ulDCHeight) / 2;
			m_lOffsetY = 0;
		}
	}
	float fxDiv = (float)m_ulDCWidth / ((float)m_ulWndWidth - (float)m_lOffsetX * 2);
	float fyDiv = (float)m_ulDCHeight / ((float)m_ulWndHeight - (float)m_lOffsetY * 2);
	float fDiv  = fxDiv < fyDiv ? fyDiv : fxDiv;
	if (0 != fDiv - m_fDiv)
	{
		m_fDiv = fDiv;
		IVS_LOG(IVS_LOG_DEBUG, "Get Offset", "xDiv: %f, yDiv %f, Div: %f", fxDiv, fyDiv, m_fDiv);
	}
}



BOOL CIADrawer::IsShowIAShapeInvalid(const IA_INNER_GRAPHICS &pIAGraphic)
{
	BOOL bRet = FALSE;

	RECT rc = {0};
	bRet = !FindEdgeRectangle(pIAGraphic.iaGraphic.IAPoints, pIAGraphic.iaGraphic.ulPointsNum, rc);
	CHECK_RESULT_TRUE_LOG(bRet, "Is Show IA Shape Invalid", "Find Edge Rectangle Failed");

	// �ж�ͼ�α߽�����Ƿ񳬳�Ŀǰ�ֱ��ʷ�Χ
	bRet = (0 > rc.left) || (rc.right > (LONG)m_ulDCWidth) 
		|| (0 > rc.top) || (rc.bottom > (LONG)m_ulDCHeight);
	CHECK_RESULT_TRUE_LOG(bRet, "Is Show IA Shape Invalid", "Graphic Outside the Window");

	return bRet;
}



// ɾ������ѡ��ͼ��
LONG CIADrawer::DeleteIAGraphicsSelected()
{
	IVS_TRACE();

	CAutoLock Lock(m_Mutex);
	// ����ͼ���б�,ɾ����ǰGraphicGroup������ͼ��
	IA_GRAPHICS_LIST_ITER iter    = m_IADrawGraphicsList.begin();
	IA_GRAPHICS_LIST_ITER iterEnd = m_IADrawGraphicsList.end();
	while (iter != iterEnd)
	{
		IA_INNER_GRAPHICS *pIAGraphic = *iter;
		if (NULL == pIAGraphic)
		{
			iter = m_IADrawGraphicsList.erase(iter);
			continue;
		}

		if (pIAGraphic->bEditing && 
			pIAGraphic->iaGraphic.bAllowDelete)
		{
			IVS_DELETE(pIAGraphic);
			iter = m_IADrawGraphicsList.erase(iter);
			continue;
		}
		iter++;
	}

	return IVS_SUCCEED;
}


// ������������ɾ�����ܷ�������ͼ��
//����������ָ��ͼ����ĵ���ͼ���ڲ���ɾ����ͼ�Σ����ڶ��ͼ���ص�������ɾ����
LONG CIADrawer::DeleteIAGraphicsByPoint(ULONG ulGraphicGroup, POINT point)
{
	IVS_TRACE();
	CAutoLock Lock(m_Mutex);
	
	int count = 0;
	IA_GRAPHICS_LIST_ITER iter    = m_IADrawGraphicsList.begin();
	IA_GRAPHICS_LIST_ITER delIter = m_IADrawGraphicsList.begin();
	IA_GRAPHICS_LIST_ITER iterEnd = m_IADrawGraphicsList.end();
	IA_INNER_GRAPHICS *pIAGraphic = NULL;
	// ����ͼ���б�,���ҵ�����������ͼ��ʱ,��delIter��¼�ڵ�
	while (iter != iterEnd)
	{
		pIAGraphic = *iter;
		if (NULL == pIAGraphic)
		{
			iter = m_IADrawGraphicsList.erase(iter);
			continue;
		}

		if (IsGraphicToDelete(pIAGraphic, point) 
			&& pIAGraphic->iaGraphic.bAllowDelete 
			&& (ulGraphicGroup == pIAGraphic->iaGraphic.ulGraphicGroup))
		{
			delIter = iter;
			count++;	// ��¼����������ͼ�θ���
		}
		iter++;
	}

	// ���ҽ���ֻѡ��һ��ͼ��ʱ��ɾ��,�����ڶ��ͼ���ص����������κβ���
	if (1 == count) 		
	{
		pIAGraphic = *delIter;
		IVS_DELETE(pIAGraphic);
		m_IADrawGraphicsList.erase(delIter);
		return IVS_SUCCEED;
	}	
	IVS_LOG(IVS_LOG_ERR, "Delete IA Graphics", "No Graphics Choosen" );
	return IVS_FAIL;
}

// �жϸ��ݸõ��Ƿ����ɾ����ͼ��
BOOL CIADrawer::IsGraphicToDelete(const IA_INNER_GRAPHICS *pIAGraphics, POINT point) 
{
	BOOL bFind = FALSE;
	LONG dist = MAX_DISTANCE_VAL;
	LONG distcmp = MAX_DISTANCE_VAL;
	switch (pIAGraphics->iaGraphic.ulGraphicType)
	{
	case IA_GRAPHICS_TYPE_STRAIGHT_LINE: // ����ֱ����Ϊѡ��
		dist = DistanceToLine(point, pIAGraphics->iaGraphic.IAPoints[0],
								pIAGraphics->iaGraphic.IAPoints[1]);
		if (CLOSE_TO_LINE_DISTANCE > dist)
		{
			bFind = TRUE;
		}
		break;
	case IA_GRAPHICS_TYPE_UP_ULINE: // ����U��������һ����Ϊѡ��
	case IA_GRAPHICS_TYPE_DOWN_ULINE: // ����U��������һ����Ϊѡ��
		dist    = DistanceToLine(point, pIAGraphics->iaGraphic.IAPoints[0], 
								  pIAGraphics->iaGraphic.IAPoints[1]); 
		distcmp = DistanceToLine(point, pIAGraphics->iaGraphic.IAPoints[1], 
								  pIAGraphics->iaGraphic.IAPoints[2]);
		dist = min(dist, distcmp);
		distcmp = DistanceToLine(point, pIAGraphics->iaGraphic.IAPoints[2], 
								  pIAGraphics->iaGraphic.IAPoints[3]);
		dist = min(dist, distcmp);
		if (CLOSE_TO_LINE_DISTANCE > dist)
		{
			bFind = TRUE;
		}
		break;
	default: // ���ھ��κͶ������Ϊѡ��
		if (IsInShape(pIAGraphics, point))
		{
			bFind = TRUE;
		}
		break;
	}	
	return bFind;
}


// ���ָ��ͼ���������ͼ��
LONG CIADrawer::DeleteGraphicsByGroup(ULONG ulGraphicGroup)
{
	IVS_TRACE();
	CAutoLock Lock(m_Mutex);

	// ����ͼ���б�,ɾ����ǰ��ѡ�е�����ͼ��
	IA_GRAPHICS_LIST_ITER iter    = m_IADrawGraphicsList.begin();
	IA_GRAPHICS_LIST_ITER iterEnd = m_IADrawGraphicsList.end();
	while (iter != iterEnd)
	{
		IA_INNER_GRAPHICS *pIAGraphic = *iter;
		if (NULL == pIAGraphic)
		{
			iter = m_IADrawGraphicsList.erase(iter);
			continue;
		}

		if (pIAGraphic->iaGraphic.ulGraphicGroup == ulGraphicGroup)
		{
			IVS_DELETE(pIAGraphic);
			iter = m_IADrawGraphicsList.erase(iter);
			continue;
		}
		iter++;
	}

	return IVS_SUCCEED;
}

// ������ܷ�������ͼ��
LONG CIADrawer::DeleteIAGraphicsAll()
{
	IVS_TRACE();
	CAutoLock Lock(m_Mutex);

	m_bActive = FALSE;
	memset(&m_IAGraphic, 0, sizeof(IA_INNER_GRAPHICS));

	IA_GRAPHICS_LIST_ITER iter    = m_IADrawGraphicsList.begin();
	IA_GRAPHICS_LIST_ITER iterEnd = m_IADrawGraphicsList.end();
	while (iter != iterEnd)
	{
		IA_INNER_GRAPHICS *pIAGraphic = *iter;
		if (NULL != pIAGraphic)
		{
			IVS_DELETE(pIAGraphic);
		}
		iter = m_IADrawGraphicsList.erase(iter);
	}
	m_ulGraphicID = 0;
	return IVS_SUCCEED;
}


// ������������ѡ�����ܷ�������ͼ�Σ�ѡ���ض������������������ĵ���ͼ�Σ�
LONG CIADrawer::SelectGraphicByPoint(POINT Point, ULONG &ulGraphicGroup, BOOL &bAllowDel)
{
	IA_INNER_GRAPHICS *pGraphMatch = NULL;
	LONG MatchDist = MAX_DISTANCE_VAL;
	CAutoLock Lock(m_Mutex);
	IA_GRAPHICS_LIST_ITER iter = m_IADrawGraphicsList.begin();
	IA_GRAPHICS_LIST_ITER iterEnd  = m_IADrawGraphicsList.end();
	while (iter != iterEnd)
	{
		IA_INNER_GRAPHICS *pIAGraphic = *iter;
		if (NULL == pIAGraphic)
		{
			iter = m_IADrawGraphicsList.erase(iter);
			continue;
		}
		iter++;
		pIAGraphic->bEditing = FALSE;
#if 1
		// �Ǹ�GraphicGroup��ͼ��ֱ������
		if ((ulGraphicGroup != pIAGraphic->iaGraphic.ulGraphicGroup) 
            && (ulGraphicGroup))
		{
			continue;
		}
#endif	
		LONG  distMin  = MAX_DISTANCE_VAL;
		LONG  dist     = 0;
		ULONG i        = 0;
		if (pIAGraphic->iaGraphic.ulPointsNum < 2)
		{
			IVS_LOG(IVS_LOG_ERR, "Select Graphic By Point", "PointNum Invalid");
			return IVS_FAIL;
		}
		// �ҵ�����뵥��ͼ�θ����ߵ���С���루ulPointsNum > 2 ʱ���룩
		for (i = 0; i < pIAGraphic->iaGraphic.ulPointsNum - 2; i++)
		{
			LONG distToShape = DistanceToLine(Point, pIAGraphic->iaGraphic.IAPoints[i], 
										 pIAGraphic->iaGraphic.IAPoints[i + 1]);
			// ������Ϊ0��ֱ�ӱ�Ǹ�ͼ��Ϊѡ�У��˳�
			if (0 == distToShape) 
			{
				pIAGraphic->bEditing = TRUE;
                ulGraphicGroup = pIAGraphic->iaGraphic.ulGraphicGroup;
				bAllowDel = pIAGraphic->iaGraphic.bAllowDelete;
				return IVS_SUCCEED;
			}
			if (distToShape <= distMin)
			{
				distMin = distToShape;
			}
		}

		// ֱ�������ulPointsNum == 2��
		dist = DistanceToLine(Point, pIAGraphic->iaGraphic.IAPoints[i], 
							  pIAGraphic->iaGraphic.IAPoints[i + 1]);
		// ������Ϊ0��ֱ�ӱ�Ǹ�ͼ��Ϊѡ�У��˳�
		if (0 == dist)
		{
			pIAGraphic->bEditing = TRUE;
            ulGraphicGroup = pIAGraphic->iaGraphic.ulGraphicGroup;
			bAllowDel = pIAGraphic->iaGraphic.bAllowDelete;
			return IVS_SUCCEED;
		}
		if (dist <= distMin)
		{
			distMin = dist;
		}

		// ������ΪĿǰ��С�����¼��С����͵�ǰͼ��
		if (distMin < MatchDist)
		{
			MatchDist = distMin;
			pGraphMatch = pIAGraphic;
		}
	}

	// �����point������С��ͼ����Ϊ�༭״̬��ע����Ҫ�����ͼ���߸�����
	if (pGraphMatch && MatchDist < CLOSE_TO_LINE_DISTANCE)
	{
		pGraphMatch->bEditing = TRUE;
        ulGraphicGroup = pGraphMatch->iaGraphic.ulGraphicGroup;
		bAllowDel = pGraphMatch->iaGraphic.bAllowDelete;
		return IVS_SUCCEED;
	}

	// û��ѡ���κ�ͼ��
	return IVS_FAIL;
}

LONG CIADrawer::GetCursorType(POINT Point)
{
	CAutoLock Lock(m_Mutex);
	BOOL bFind = FALSE;
	LONG lCursorType = CURSOR_TYPE_ARROW;
	IA_GRAPHICS_LIST_ITER iter    = m_IADrawGraphicsList.begin();
	IA_GRAPHICS_LIST_ITER iterEnd = m_IADrawGraphicsList.end();
	while (iter != iterEnd)
	{
		IA_INNER_GRAPHICS *pIAGraphic = *iter;
		if (NULL == pIAGraphic)
		{
			iter = m_IADrawGraphicsList.erase(iter);
			continue;
		}
		if (pIAGraphic->bEditing) // ��ȡ���ڱ༭ͼ��
		{
			lCursorType = GetCursorTypeInShape(Point, pIAGraphic->iaGraphic);
			bFind = TRUE;
			break;
		}
		iter++;
	}
	if (!bFind && m_bActive)
	{
		lCursorType = GetCursorTypeInShape(Point, m_IAGraphic.iaGraphic);
	}
	return lCursorType;
}

LONG CIADrawer::GetCursorTypeInShape(POINT Point, const IA_GRAPHICS &IAGraphic)
{
	// ����ڵ���
	for(ULONG i = 0; i < IAGraphic.ulPointsNum; i++)
	{
		if (IsCloseToPoint(Point, IAGraphic.IAPoints[i]))
		{
			return GetCursorTypePoint(Point, IAGraphic, i);
		}
	}

	// ���������
	ULONG ulPointsNum = IAGraphic.ulPointsNum;
	if (IA_GRAPHICS_TYPE_RECTANGLE != IAGraphic.ulGraphicType 
		&& IA_GRAPHICS_TYPE_POLYGON != IAGraphic.ulGraphicType)
	{
		ulPointsNum--;
	}
	for(ULONG i = 0; i < ulPointsNum; i++)
	{
		if (CLOSE_TO_LINE_DISTANCE > 
			DistanceToLine(Point, IAGraphic.IAPoints[i], IAGraphic.IAPoints[i + 1]))
		{
			return CURSOR_TYPE_SIZEALL;
		}
	}

	return CURSOR_TYPE_ARROW;
}

LONG CIADrawer::GetCursorTypePoint(POINT /*Point*/, const IA_GRAPHICS &IAGraphic, ULONG ulIndex)
{
	ULONG ulPointsNum = IAGraphic.ulPointsNum;
	if (IA_GRAPHICS_TYPE_RECTANGLE == IAGraphic.ulGraphicType 
		|| IA_GRAPHICS_TYPE_POLYGON == IAGraphic.ulGraphicType)
	{
		ulPointsNum--;
	}
	if (ulPointsNum == 0)
	{
		return CURSOR_TYPE_ARROW;
	}
	ULONG ulIndexPre =  (ulIndex + ulPointsNum - 1) % ulPointsNum;
	ULONG ulIndexNext = (ulIndex + ulPointsNum + 1) % ulPointsNum;
	LONG lCursorType = GetCursorTypePointInner(IAGraphic.IAPoints[ulIndex], 
		IAGraphic.IAPoints[ulIndexPre], IAGraphic.IAPoints[ulIndexNext]);
	return lCursorType;
} 

LONG CIADrawer::GetCursorTypePointInner(POINT Point, POINT PointPre, POINT PointNext)
{
	LONG lCursorType = CURSOR_TYPE_ARROW;

	LONG lWidthPre   = Point.x - PointPre.x;
	LONG lHeightPre  = Point.y - PointPre.y;
	LONG lWidthNext  = Point.x - PointNext.x;
	LONG lHeightNext = Point.y - PointNext.y;
	LONG lPre        = lWidthPre  * lHeightPre;
	LONG lNext       = lWidthNext * lHeightNext;
	LONG lAll        = lPre * lNext;
	if (0 <= lAll)
	{
		if (0 < lPre)
		{
			lCursorType = CURSOR_TYPE_SIZENESW; 
		} 
		else if (0 > lPre)
		{
			lCursorType = CURSOR_TYPE_SIZENWSE;
		}
		else
		{
			if (0 < (lWidthPre ^ lHeightNext) || 0 < (lHeightPre ^ lWidthNext))
			{
				lCursorType = CURSOR_TYPE_SIZENWSE;
			}
			else
			{
				lCursorType = CURSOR_TYPE_SIZENESW; 
			}
		}
	} else {
		if (0 <= lHeightPre * lHeightNext)
		{
			lCursorType = CURSOR_TYPE_SIZENS; 
		} else {
			lCursorType = CURSOR_TYPE_SIZEWE;
		}
	}
	return lCursorType;
}


// ѡ��ָ��ͼ���������ͼ��
LONG CIADrawer::SelectGraphicByGroup(ULONG ulGraphicGroup, BOOL &bAllowDel)
{
	IVS_TRACE();

	CAutoLock Lock(m_Mutex);
	if (m_IADrawGraphicsList.empty())
	{
		IVS_LOG(IVS_LOG_INFO, "Select Graphic By Group", "No Graphics");
		return IVS_SUCCEED;
	}

	// ����ͼ���б�,�ҳ����и�GraphicGroup��ͼ��,��Ϊ�༭״̬
	IA_GRAPHICS_LIST_ITER iter    = m_IADrawGraphicsList.begin();
	IA_GRAPHICS_LIST_ITER iterEnd = m_IADrawGraphicsList.end();
	UINT uiCount = 0;
	while (iter != iterEnd)
	{
		IA_INNER_GRAPHICS *pIAGraphic = *iter;
		if (NULL == pIAGraphic)
		{
			iter = m_IADrawGraphicsList.erase(iter);
			continue;
		}

		if (pIAGraphic->iaGraphic.ulGraphicGroup == ulGraphicGroup)
		{
			if (!uiCount)
			{
				pIAGraphic->bEditing = TRUE;
				bAllowDel = pIAGraphic->iaGraphic.bAllowDelete;
			}
			uiCount++;
		}
		else
		{
			pIAGraphic->bEditing = FALSE;
		}
		iter++;
	}
	return IVS_SUCCEED;
}

// ���ܷ�����ʼ�޸�ͼ��
LONG CIADrawer::StartModifyIAGraphics(POINT point, MOUSE_EVENT_TYPE MouseEvent, ULONG ulGraphicGroup)
{
	IVS_TRACE();
	LONG lRet  = IVS_FAIL;	
	CAutoLock Lock(m_Mutex);	
	if (m_IADrawGraphicsList.empty())
	{
		IVS_LOG(IVS_LOG_ERR, "Start Modify IA Graphics", "No Graphics");
		return IVS_FAIL;
	}
	
	// ����ͼ���б�,�ҵ���ǰ�ڱ༭״̬��ͼ��	
	BOOL bFind = FALSE;
	IA_GRAPHICS_LIST_ITER iter    = m_IADrawGraphicsList.begin();
	IA_GRAPHICS_LIST_ITER iterEnd = m_IADrawGraphicsList.end();
	IA_INNER_GRAPHICS *pIAGraphic = NULL;
	while (iter != iterEnd)
	{
		pIAGraphic = *iter;	
		if (NULL == pIAGraphic)
		{
			iter = m_IADrawGraphicsList.erase(iter);
			continue;
		}

		if (pIAGraphic->bEditing)
		{
			bFind = TRUE;
			break;
		}
		iter++;
	}

	// ���ҵ����ڱ༭��ͼ��,���俽����m_IAGraphic��,�����б���ɾ��
	if ((bFind) && (NULL != pIAGraphic))
	{
		memset(&m_IAGraphic, 0, sizeof(IA_INNER_GRAPHICS));
		memcpy_s(&m_IAGraphic, sizeof(IA_INNER_GRAPHICS), pIAGraphic, sizeof(IA_INNER_GRAPHICS));
		m_IAGraphic.bFinish = FALSE;
		m_bActive = TRUE;

		//��List��ɾ����ͼ�εĴ洢
		IVS_DELETE(pIAGraphic);
		m_IADrawGraphicsList.erase(iter);
	
		// ��ʼ�޸�ͼ�Σ�ȷ���޸ĵ������Լ�������Ҫת�����꣩
		lRet = StartModifyIAShapes(point, MouseEvent, ulGraphicGroup);

		// �޸�ʧ�ܽ���ǰm_IAGraphic��ͼ�β����б���
		lRet = ModifyFinishedAndInsert(lRet);
	}
	return lRet;
}

// ���ܷ���������ƶ��޸�ͼ����Ӧ������
LONG CIADrawer::StartModifyIAShapes(POINT point, MOUSE_EVENT_TYPE MouseEvent, ULONG ulGraphicGroup) 
{
	LONG lRet = IVS_FAIL;
	// ���øú���֮ǰ�Ѿ������m_IAGraphic
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Start Modify IA Shapes", "m_IAGraphic is Inactive");
		return lRet;
	}

	switch (m_IAGraphic.iaGraphic.ulGraphicType)
	{
	case IA_GRAPHICS_TYPE_STRAIGHT_LINE:
		lRet = ModifyIAStraightLine(point, MouseEvent, ulGraphicGroup);
		break;
	case IA_GRAPHICS_TYPE_RECTANGLE:
		lRet = ModifyIARectangle(point, &m_lPointNo, MouseEvent, ulGraphicGroup);
		break;
	case IA_GRAPHICS_TYPE_POLYGON:
		lRet = ModifyIAPolygon(point, &m_lPointNo, MouseEvent, ulGraphicGroup);
		break;
	case IA_GRAPHICS_TYPE_UP_ULINE: 
	case IA_GRAPHICS_TYPE_DOWN_ULINE: 
		lRet = ModifyIAULine(point, MouseEvent, ulGraphicGroup);
		break;
	default:
		IVS_LOG(IVS_LOG_ERR, "Start Modify IA Shapes", "GraphicType Invalid");
		break;
	}	
	return lRet;
}


// ���ܷ�����ʼ�޸�ֱ��
LONG CIADrawer::ModifyIAStraightLine(POINT point, MOUSE_EVENT_TYPE MouseEvent, ULONG ulGraphicGroup)
{
	// ���øú���֮ǰ�Ѿ������m_IAGraphic
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Modify IA Straight Line", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}
	// ������
	if (LEFT_BUTTON_DOWN == MouseEvent)
	{
		// ����ֱ����㣬��Ҫת����ֹ�����꣬��ǵ�ǰ״̬Ϊ�޸�ֱ�ߵĵ�
		if (IsCloseToPoint(point, m_IAGraphic.iaGraphic.IAPoints[0]))
		{
			m_IAGraphic.iaGraphic.IAPoints[0] = m_IAGraphic.iaGraphic.IAPoints[1];
			m_IAGraphic.iaGraphic.IAPoints[1] = point;
			m_ulCurrentState = IA_MODIFING_STRAIGHT_LINE_POINT;
			if (ARROW_TYPE_LINE_TO_LEFT == m_IAGraphic.iaGraphic.ulArrowType)
			{
				m_IAGraphic.iaGraphic.ulArrowType = ARROW_TYPE_LINE_TO_RIGHT;
			}else if (ARROW_TYPE_LINE_TO_RIGHT == m_IAGraphic.iaGraphic.ulArrowType)
			{
				m_IAGraphic.iaGraphic.ulArrowType = ARROW_TYPE_LINE_TO_LEFT;
			}
			GetArrowPoints(m_IAGraphic.iaGraphic);
			m_bReversePoint = TRUE;
			return IVS_SUCCEED;
		}
		// ����ֱ���յ㣬��ǵ�ǰ״̬Ϊ�޸�ֱ�ߵĵ�
		if (IsCloseToPoint(point, m_IAGraphic.iaGraphic.IAPoints[1]))
		{
			m_IAGraphic.iaGraphic.IAPoints[1] = point;
			m_ulCurrentState = IA_MODIFING_STRAIGHT_LINE_POINT;
			return IVS_SUCCEED;
		}

		// ����ֱ���߶Σ���ǵ�ǰ״̬Ϊ�ƶ�ͼ��
		if (CLOSE_TO_LINE_DISTANCE > 
			DistanceToLine(point, 
			m_IAGraphic.iaGraphic.IAPoints[0], m_IAGraphic.iaGraphic.IAPoints[1]))
		{
			m_RecordPoint = point;
			m_OriginIAPoints[0] = m_IAGraphic.iaGraphic.IAPoints[0];
			m_OriginIAPoints[1] = m_IAGraphic.iaGraphic.IAPoints[1];
			m_ulCurrentState = IA_MODIFING_MOVING_SHAPE;
			m_ulCurrentGraphicType = IA_GRAPHICS_TYPE_STRAIGHT_LINE;
			return IVS_SUCCEED;
		}
	}
	
	// �Ҽ����,ɾ��ֱ��
	if (RIGHT_BUTTON_DOWN == MouseEvent) 
	{
		if (IVS_SUCCEED == InsertCurrentToList())
		{
			return DeleteIAGraphicsByPoint(ulGraphicGroup, point);
		}
	}
	return IVS_FAIL;
}

// ���ܷ�����ʼ�޸ľ���
LONG CIADrawer::ModifyIARectangle(POINT point, LONG *pPointNo, MOUSE_EVENT_TYPE MouseEvent, ULONG ulGraphicGroup)
{
	// ���øú���֮ǰ�Ѿ������m_IAGraphic
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Modify IA Rectangle", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}
	*pPointNo = POINTNO_ILLEGAL; // ����¶���������±�
	for (ULONG i = 0; i < POINT_NUM_OF_RECTANGLE; i++)
	{
		// ���ھ��ζ��㸽��
		if (IsCloseToPoint(point, m_IAGraphic.iaGraphic.IAPoints[i]))
		{
			if (!m_IAGraphic.iaGraphic.bAllowModifySize)
			{
				break;
			}
			return ModifyIARectanglePoint(MouseEvent, i, point);
		}
	}
	for (ULONG i = 0; i < POINT_NUM_OF_RECTANGLE; i++)
	{
		// ���ھ��α߸���
		if (CLOSE_TO_LINE_DISTANCE > 
			DistanceToLine(point, 
			m_IAGraphic.iaGraphic.IAPoints[i], m_IAGraphic.iaGraphic.IAPoints[i + 1]))
		{
			return ModifyIARectangleLine(pPointNo, MouseEvent, i, point);
		}
	}

	// �Ҽ���������ڲ�,ɾ��ͼ��
	if (RIGHT_BUTTON_DOWN == MouseEvent) 
	{
		if (IVS_SUCCEED == InsertCurrentToList())
		{
			return DeleteIAGraphicsByPoint(ulGraphicGroup, point);
		}
	}
	return IVS_FAIL;
}

// ���ܷ�����ʼ�޸ľ��Σ��޸ĵ㣺����϶����������ž��Ρ��Ҽ���������������϶���
LONG CIADrawer::ModifyIARectanglePoint(MOUSE_EVENT_TYPE MouseEvent, ULONG ulPointIndex, POINT /*point*/) 
{
	// ���øú���֮ǰ�Ѿ������m_IAGraphic
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Modify IA Rectangle Point", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}
	POINT tmp_Points[5];
	memset(tmp_Points, 0, sizeof(tmp_Points));

	// ��������ת���Զ�������Ϊ��ʼ���꣬��ǵ�ǰ״̬Ϊ����ı���δ�С
	if (LEFT_BUTTON_DOWN == MouseEvent && m_IAGraphic.iaGraphic.bAllowModifySize)
	{	
		tmp_Points[0] = m_IAGraphic.iaGraphic.IAPoints[(ulPointIndex + 2) % POINT_NUM_OF_RECTANGLE];
		tmp_Points[1] = m_IAGraphic.iaGraphic.IAPoints[(ulPointIndex + 3) % POINT_NUM_OF_RECTANGLE];
		tmp_Points[2] = m_IAGraphic.iaGraphic.IAPoints[(ulPointIndex + 4) % POINT_NUM_OF_RECTANGLE];
		tmp_Points[3] = m_IAGraphic.iaGraphic.IAPoints[(ulPointIndex + 1) % POINT_NUM_OF_RECTANGLE];
		tmp_Points[4] = tmp_Points[0];
		memcpy_s(m_IAGraphic.iaGraphic.IAPoints, sizeof(POINT) * POINTS_NUM_MAX, 
			tmp_Points, sizeof(POINT) * 5);

		m_ulCurrentState = IA_MODIFING_RECTANGLE_POINT;
		return IVS_SUCCEED;
	}
#if 0 // �Ҽ�����㣬�����ƶ����ɾ����
	// �Ҽ��������ǵ�ǰ״̬Ϊ�����϶�ͼ��
	if (RIGHT_BUTTON_DOWN == MouseEvent)
	{
		m_RecordPoint = point;
		for (ULONG j = 0; j < 5; j++)
		{
			m_OriginIAPoints[j] = m_IAGraphic.iaGraphic.IAPoints[j];
		}
		m_ulCurrentGraphicType = IA_GRAPHICS_TYPE_RECTANGLE;
		m_ulCurrentState = IA_MODIFING_MOVING_SHAPE;
		return IVS_SUCCEED;
	}
#endif
	// �Ҽ������ɾ�����ε�����
	if (RIGHT_BUTTON_DOWN == MouseEvent && m_IAGraphic.iaGraphic.bAllowAddNewPoint)
	{
		memcpy_s(tmp_Points, sizeof(tmp_Points), 
			m_IAGraphic.iaGraphic.IAPoints, sizeof(tmp_Points));
		// ���ǰ�ƺ�������,��¼����ʱ������
		for (ULONG j = ulPointIndex; j < 4; j++)
		{
			tmp_Points[j] = tmp_Points[j + 1];
		}

		// �պ����һ����
		tmp_Points[3] = tmp_Points[0];
		tmp_Points[4].x = 0;
		tmp_Points[4].y = 0;
		memcpy_s(m_IAGraphic.iaGraphic.IAPoints, sizeof(tmp_Points), 
			tmp_Points, sizeof(tmp_Points));
		m_IAGraphic.iaGraphic.ulPointsNum--;
		m_IAGraphic.iaGraphic.ulGraphicType = IA_GRAPHICS_TYPE_POLYGON;
		m_ulCurrentState = IA_MODIFING_POLYGON_DELETE_POINT;
		return IVS_SUCCEED;
	}

	m_IAGraphic.bFinish = TRUE;
	return IVS_FAIL;
}

// ���ܷ�����ʼ�޸ľ��Σ��޸ıߣ��϶����ߡ��ڱ������ӽڵ㣩
LONG CIADrawer::ModifyIARectangleLine(LONG *pPointNo, MOUSE_EVENT_TYPE MouseEvent, ULONG ulPointIndex, POINT point) 
{
	// ���øú���֮ǰ�Ѿ������m_IAGraphic
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Modify IA Rectangle Line", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}
	*pPointNo = POINTNO_ILLEGAL;

#if 0// �������㣬�޸ĵ��߱�������ƶ�
	// ��������ת���Զ�������Ϊ��ʼ���꣬��ǵ�ǰ״̬Ϊ�����޸ľ��δ�С
	if (LEFT_BUTTON_DOWN == MouseEvent)
	{
		POINT tmp_points[5];
		memset(tmp_points, 0, sizeof(tmp_points));

		// �޸ľ��ε�ˮƽ��:�����϶�
		if (m_IAGraphic.iaGraphic.IAPoints[ulPointIndex].y 
			== m_IAGraphic.iaGraphic.IAPoints[ulPointIndex+1].y)
		{
			tmp_points[0] = m_IAGraphic.iaGraphic.IAPoints[(ulPointIndex + 2) % POINT_NUM_OF_RECTANGLE];
			tmp_points[1] = m_IAGraphic.iaGraphic.IAPoints[(ulPointIndex + 3) % POINT_NUM_OF_RECTANGLE];
			tmp_points[2] = m_IAGraphic.iaGraphic.IAPoints[(ulPointIndex + 4) % POINT_NUM_OF_RECTANGLE];
			tmp_points[3] = m_IAGraphic.iaGraphic.IAPoints[(ulPointIndex + 1) % POINT_NUM_OF_RECTANGLE];
			tmp_points[4] = tmp_points[0];
			memcpy_s(m_IAGraphic.iaGraphic.IAPoints, sizeof(POINT) * POINTS_NUM_MAX, 
				tmp_points, sizeof(POINT) * 5);

			m_ulCurrentState = IA_MODIFING_RECTANGLE_HORIZON_LINE;
		}
		else //�޸ľ��εĴ�ֱ��:�����϶�
		{
			tmp_points[0] = m_IAGraphic.iaGraphic.IAPoints[(ulPointIndex + 3) % POINT_NUM_OF_RECTANGLE];
			tmp_points[1] = m_IAGraphic.iaGraphic.IAPoints[(ulPointIndex + 4) % POINT_NUM_OF_RECTANGLE];
			tmp_points[2] = m_IAGraphic.iaGraphic.IAPoints[(ulPointIndex + 1) % POINT_NUM_OF_RECTANGLE];
			tmp_points[3] = m_IAGraphic.iaGraphic.IAPoints[(ulPointIndex + 2) % POINT_NUM_OF_RECTANGLE];
			tmp_points[4] = tmp_points[0];
			memcpy_s(m_IAGraphic.iaGraphic.IAPoints, sizeof(POINT) * POINTS_NUM_MAX, 
				tmp_points, sizeof(POINT) * 5);

			m_ulCurrentState = IA_MODIFING_RECTANGLE_VERTICAL_LINE;
		}

		return IVS_SUCCEED;
	}
#endif
	// �Ҽ��������ǵ�ǰ״̬Ϊ�����϶�ͼ��
	if (LEFT_BUTTON_DOWN == MouseEvent)
	{
		m_RecordPoint = point;
		for (ULONG j = 0; j < 5; j++)
		{
			m_OriginIAPoints[j] = m_IAGraphic.iaGraphic.IAPoints[j];
		}
		m_ulCurrentGraphicType = IA_GRAPHICS_TYPE_RECTANGLE;
		m_ulCurrentState = IA_MODIFING_MOVING_SHAPE;
		return IVS_SUCCEED;
	}

	// �Ҽ����,���Ҹþ����ǿ������Ӷ����ɶ���ε�,������ת��Ϊ�����,��ǵ�ǰ״̬Ϊ���Ӷ���ζ���
	if ((RIGHT_BUTTON_DOWN == MouseEvent) && m_IAGraphic.iaGraphic.bAllowAddNewPoint)
	{
		m_IAGraphic.iaGraphic.ulGraphicType = IA_GRAPHICS_TYPE_POLYGON;
		for (ULONG j = 5; j > (ulPointIndex + 1); j--)
		{
			m_IAGraphic.iaGraphic.IAPoints[j] = m_IAGraphic.iaGraphic.IAPoints[j - 1];
		}
		m_IAGraphic.iaGraphic.IAPoints[ulPointIndex + 1] = point;
		*pPointNo = (LONG)(ulPointIndex + 1);
		m_IAGraphic.iaGraphic.ulPointsNum++;
		m_ulCurrentState = IA_MODIFING_POLYGON_ADD_POINT;
		return IVS_SUCCEED;
	}
	m_IAGraphic.bFinish = TRUE;
	return IVS_FAIL;
}

// ���ܷ�����ʼ�޸Ķ����
LONG CIADrawer::ModifyIAPolygon(POINT point, LONG *pPointNo, MOUSE_EVENT_TYPE MouseEvent, ULONG ulGraphicGroup)
{
	// ���øú���֮ǰ�Ѿ������m_IAGraphic
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Modify IA Polygon", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}
	*pPointNo = POINTNO_ILLEGAL;  // ����¶���������±�
	for (ULONG k = 0; k < m_IAGraphic.iaGraphic.ulPointsNum - 1; k++)
	{
		// ���ڶ���ζ��㸽��
		if (IsCloseToPoint(point, m_IAGraphic.iaGraphic.IAPoints[k]))
		{
			return ModifyIAPolygonPoint(pPointNo, MouseEvent, ulGraphicGroup, k);
		}
	}
	for (ULONG k = 0; k < m_IAGraphic.iaGraphic.ulPointsNum - 1; k++)
	{
		// ���ڶ���α߸���
		if (CLOSE_TO_LINE_DISTANCE > 
			DistanceToLine(point, m_IAGraphic.iaGraphic.IAPoints[k], 
						   m_IAGraphic.iaGraphic.IAPoints[k + 1]))
		{
			return ModifyIAPolygonLine(pPointNo, MouseEvent, point, k);
		}
	}

	// �Ҽ����������ڲ�,ɾ��ͼ��
	if (RIGHT_BUTTON_DOWN == MouseEvent) 
	{
		if (IVS_SUCCEED == InsertCurrentToList())
		{
			return DeleteIAGraphicsByPoint(ulGraphicGroup, point);
		}
	}
	return IVS_FAIL;
}

// ���ܷ�����ʼ�޸Ķ���Σ��޸ĵ㣺����޸ĵ�����λ�á��Ҽ�ɾ���õ㣩
LONG CIADrawer::ModifyIAPolygonPoint(LONG *pPointNo, MOUSE_EVENT_TYPE MouseEvent, ULONG ulGraphicGroup, ULONG ulPointIndex) 
{
	// ���øú���֮ǰ�Ѿ������m_IAGraphic
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Modify IA Polygon Point", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}

	*pPointNo = POINTNO_ILLEGAL;
	ULONG ulPointNum = m_IAGraphic.iaGraphic.ulPointsNum;
	POINT tmp_Points[POINTS_NUM_MAX];
	memset(tmp_Points, 0, sizeof(tmp_Points));
	memcpy_s(tmp_Points, sizeof(POINT) * POINTS_NUM_MAX, 
		m_IAGraphic.iaGraphic.IAPoints, 
		sizeof(POINT) * m_IAGraphic.iaGraphic.ulPointsNum);

	// ����������ǵ�ǰ״̬Ϊ�޸Ķ���ε�������
	if (LEFT_BUTTON_DOWN == MouseEvent)
	{
		*pPointNo = (LONG)ulPointIndex;
		m_ulCurrentState = IA_MODIFING_POLYGON_MOVING_POINT;
		return IVS_SUCCEED;
	}

	// ����������ǵ�ǰ״̬Ϊɾ������ε�������
	if (RIGHT_BUTTON_DOWN == MouseEvent)
	{
		// ������ε���С��5,��˵��Ϊ������,��������ɾ����
		if (ulPointNum < 5)
		{
			IVS_LOG(IVS_LOG_ERR, "Modify IA Polygon Point", "Reach MIN PointNum");
			return IVS_FAIL;
		}

		// ���ǰ�ƺ�������,��¼����ʱ������
		for (ULONG j = ulPointIndex; j < ulPointNum; j++)
		{
			tmp_Points[j] = tmp_Points[j + 1];
		}
		ulPointNum--;
		// ȷ�����鲻��Խ��
		if (ulPointNum >= POINTS_NUM_MAX)	
		{
			IVS_LOG(IVS_LOG_ERR, "Modify IA Polygon Point", "Reach MAX PointNum");
			return IVS_FAIL;
		}

		// �պ����һ����
		tmp_Points[ulPointNum - 1] = tmp_Points[0];

		// ����ʱ�����д洢���¶���ζ�����Ϣû�зǷ����,�򿽱���m_IAGraphic��
		if ((!IsForbiddenOverlaped(tmp_Points, ulPointNum, ulGraphicGroup, IA_GRAPHICS_TYPE_POLYGON)) 
			&& IsIAPolygonSelfLegal(tmp_Points, ulPointNum))
		{	
			memcpy_s(m_IAGraphic.iaGraphic.IAPoints, sizeof(POINT) * POINTS_NUM_MAX, 
				tmp_Points, sizeof(POINT) * m_IAGraphic.iaGraphic.ulPointsNum);
			m_IAGraphic.iaGraphic.ulPointsNum--;
			m_ulCurrentState = IA_MODIFING_POLYGON_DELETE_POINT;
			return IVS_SUCCEED;
		}
		IVS_LOG(IVS_LOG_ERR, "Modify IA Polygon Point", "New Polygon Illegal");
		return IVS_FAIL;
	}
	return IVS_FAIL;
}

// ���ܷ�����ʼ�޸Ķ���Σ��޸ıߣ���������϶����Ҽ������½ڵ㣩
LONG CIADrawer::ModifyIAPolygonLine(LONG *pPointNo, MOUSE_EVENT_TYPE MouseEvent, POINT point, ULONG ulPointIndex) 
{
	// ���øú���֮ǰ�Ѿ������m_IAGraphic
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Modify IA Polygon Line", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}
	*pPointNo = POINTNO_ILLEGAL;
	ULONG ulPointNum = m_IAGraphic.iaGraphic.ulPointsNum;
	POINT tmp_Points[POINTS_NUM_MAX];
	memset(tmp_Points, 0, sizeof(tmp_Points));
	memcpy_s(tmp_Points, sizeof(POINT) * POINTS_NUM_MAX, 
		m_IAGraphic.iaGraphic.IAPoints, 
		sizeof(POINT) * m_IAGraphic.iaGraphic.ulPointsNum);

	// ����������ǵ�ǰ״̬Ϊ�϶�����ͼ��
	if (LEFT_BUTTON_DOWN == MouseEvent)
	{
		m_RecordPoint = point;
		for (ULONG j = 0; j < ulPointNum; j++)
		{
			m_OriginIAPoints[j] = m_IAGraphic.iaGraphic.IAPoints[j];
		}
		m_ulCurrentGraphicType = IA_GRAPHICS_TYPE_POLYGON;
		m_ulCurrentState = IA_MODIFING_MOVING_SHAPE;
		return IVS_SUCCEED;
	}

	// ����������ǵ�ǰ״̬Ϊ���Ӷ���ζ���
	if (RIGHT_BUTTON_DOWN == MouseEvent)
	{
		// PointNum�ﵽ����,�����������Ӷ���
		if (ulPointNum >= POINTS_NUM_MAX)
		{
			IVS_LOG(IVS_LOG_ERR, "Modify IA Polygon Line", "Reach MAX PointNum");
			return IVS_FAIL;
		}

		// ������ƺ�������
		for (ULONG j = ulPointNum; j > (ulPointIndex + 1); j--)
		{
			m_IAGraphic.iaGraphic.IAPoints[j] = m_IAGraphic.iaGraphic.IAPoints[j - 1];
		}
		m_IAGraphic.iaGraphic.IAPoints[ulPointIndex + 1] = point;
		*pPointNo = (LONG)(ulPointIndex + 1);
		m_IAGraphic.iaGraphic.ulPointsNum++;
		m_ulCurrentState = IA_MODIFING_POLYGON_ADD_POINT;
		return IVS_SUCCEED;
	}
	return IVS_FAIL;
}

// ���ܷ�����ʼ�޸�U����
LONG CIADrawer::ModifyIAULine(POINT point, MOUSE_EVENT_TYPE MouseEvent, ULONG ulGraphicGroup)
{
	// ���øú���֮ǰ�Ѿ������m_IAGraphic
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Modify IA ULine", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}

	// ������
	if (LEFT_BUTTON_DOWN == MouseEvent)
	{
		// ����U������㣬��Ҫת����ֹ�����꣬��ǵ�ǰ״̬Ϊ�޸�U���ߵĶ˵�
		if (IsCloseToPoint(point, m_IAGraphic.iaGraphic.IAPoints[0]))
		{
			POINT tmp_point = m_IAGraphic.iaGraphic.IAPoints[1];
			m_IAGraphic.iaGraphic.IAPoints[0] = m_IAGraphic.iaGraphic.IAPoints[3];
			m_IAGraphic.iaGraphic.IAPoints[1] = m_IAGraphic.iaGraphic.IAPoints[2];
			m_IAGraphic.iaGraphic.IAPoints[2] = tmp_point;
			m_IAGraphic.iaGraphic.IAPoints[3] = point;
			m_IAGraphic.iaGraphic.ulPointsNum = POINT_NUM_OF_ULINE;
			m_ulCurrentState = IA_MODIFING_ULINE_POINT;
			return IVS_SUCCEED;
		}

		// ����U�����յ�,��ǵ�ǰ״̬Ϊ�޸�U���ߵĶ˵�
		if (IsCloseToPoint(point,m_IAGraphic.iaGraphic.IAPoints[3]))
		{
			m_IAGraphic.iaGraphic.ulPointsNum = POINT_NUM_OF_ULINE;
			m_ulCurrentState = IA_MODIFING_ULINE_POINT;
			return IVS_SUCCEED;
		}

		// ����U�����м�սǵ�,��ǵ�ǰ״̬Ϊ�޸�U���ߵĹսǵ�
		if (IsCloseToPoint(point,m_IAGraphic.iaGraphic.IAPoints[1]) 
			|| (IsCloseToPoint(point,m_IAGraphic.iaGraphic.IAPoints[2])))
		{
			if (m_IAGraphic.iaGraphic.IAPoints[0].x > m_IAGraphic.iaGraphic.IAPoints[3].x)
			{
				POINT tmp_point            = m_IAGraphic.iaGraphic.IAPoints[0];
				m_IAGraphic.iaGraphic.IAPoints[0] = m_IAGraphic.iaGraphic.IAPoints[3];
				m_IAGraphic.iaGraphic.IAPoints[3] = tmp_point;
				tmp_point                  = m_IAGraphic.iaGraphic.IAPoints[1];
				m_IAGraphic.iaGraphic.IAPoints[1] = m_IAGraphic.iaGraphic.IAPoints[2];
				m_IAGraphic.iaGraphic.IAPoints[2] = tmp_point;
			}
			m_IAGraphic.iaGraphic.ulPointsNum = POINT_NUM_OF_ULINE;
			m_ulCurrentState = IA_MODIFING_ULINE_ANGLE;

            // add by wanglei 00165153
            //  DTS2013051709238 �û���ǵ�ǰ�ĸ��������ڱ��޸ģ������������յ㣩
            m_ulUlineModifyIndex = 2;
			if (IsCloseToPoint(point,m_IAGraphic.iaGraphic.IAPoints[1]))
			{
				m_ulUlineModifyIndex = 1;
			}
            

			return IVS_SUCCEED;
		}

		// ����U���ߵ��߶Σ���ǵ�ǰ״̬Ϊ�ƶ�ͼ��
		for (ULONG k = 0; k < 3; k++)
		{
			if (CLOSE_TO_LINE_DISTANCE < 
				DistanceToLine(point, m_IAGraphic.iaGraphic.IAPoints[k], 
							   m_IAGraphic.iaGraphic.IAPoints[k + 1]))
			{
				continue;
			}
			m_RecordPoint = point;
			for (ULONG j = 0; j < POINT_NUM_OF_ULINE; j++)
			{
				m_OriginIAPoints[j] = m_IAGraphic.iaGraphic.IAPoints[j];
			}
			m_ulCurrentGraphicType = IA_GRAPHICS_TYPE_UP_ULINE;
			m_ulCurrentState = IA_MODIFING_MOVING_SHAPE;
			return IVS_SUCCEED;
		}
		return IVS_FAIL;
	}

	// �Ҽ����,ɾ��U����
	if (RIGHT_BUTTON_DOWN == MouseEvent) 
	{
		if (IVS_SUCCEED == InsertCurrentToList())
		{
			return DeleteIAGraphicsByPoint(ulGraphicGroup, point);
		}
	}
	return IVS_FAIL;
}


// ���ܷ���������ƶ��޸�ͼ����Ӧ������
LONG CIADrawer::ModifyIAShapeUpDate(POINT point, BOOL bFinish, ULONG ulGraphicGroup)
{
	// ���øú���֮ǰ�Ѿ������m_IAGraphic
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Modify IA Shape UpDate", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}
	LONG lRet = IVS_FAIL;

	// �����޸������޸ľ����ͼ��
	switch (m_ulCurrentState)
	{
	case IA_MODIFING_STRAIGHT_LINE_POINT:
		lRet = DrawIAStraightLine(point, bFinish, FALSE);
		break;
	case IA_MODIFING_MOVING_SHAPE:
		lRet = UpdateIAMove(point, ulGraphicGroup, bFinish);
		break;
	case IA_MODIFING_RECTANGLE_HORIZON_LINE:
		point.x = m_IAGraphic.iaGraphic.IAPoints[1].x;
		lRet = DrawIARectangle(point, bFinish, FALSE);
		break;
	case IA_MODIFING_RECTANGLE_VERTICAL_LINE:
		point.y = m_IAGraphic.iaGraphic.IAPoints[3].y;
		lRet = DrawIARectangle(point, bFinish, FALSE);
		break;
	case IA_MODIFING_RECTANGLE_POINT:
		lRet = DrawIARectangle(point, bFinish, FALSE);
		break;
	case IA_MODIFING_POLYGON_MOVING_POINT:	// ���������ͬ��
	case IA_MODIFING_POLYGON_DELETE_POINT:  // ���������ͬ��
	case IA_MODIFING_POLYGON_ADD_POINT:
		lRet = UpdateIAPolygon(point, bFinish, ulGraphicGroup);
		break;
	case IA_MODIFING_ULINE_POINT:
		lRet = DrawIAULine(point, bFinish, bFinish, FALSE);
		break;
	case IA_MODIFING_ULINE_ANGLE:
		lRet = UpdateIAULine(point, bFinish);
		break;
	default:
		IVS_LOG(IVS_LOG_ERR, "Modify IA Shape UpDate", "Modify Type Invalid");
		break;
	}
	
	// �޸Ľ�������ͼ���б�
	if (bFinish)
	{
		lRet = ModifyFinishedAndInsert(lRet);
	}
	return lRet;
}

// ���ܷ����޸�ͼ�ν���
LONG CIADrawer::ModifyFinishedAndInsert(LONG lRet) 
{
	// �޸ĳɹ�ʱ���Ѿ������б���
	if ((IVS_SUCCEED != lRet) && m_bActive)
	{
		m_IAGraphic.bFinish = TRUE;
		LONG lInnerRet = InsertCurrentToList();
		if (IVS_SUCCEED != lInnerRet)
		{
			return lInnerRet;
		}
	}
	return lRet;
}

// �޸��и��¶����ͼ��
LONG CIADrawer::UpdateIAPolygon(POINT point, BOOL bFinish, ULONG ulGraphicGroup)
{
	// ���øú���֮ǰ�Ѿ������m_IAGraphic
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Update IA Polygon", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}
	LONG lRet = IVS_FAIL;
	switch (m_ulCurrentState)
	{
	case IA_MODIFING_POLYGON_DELETE_POINT:
		lRet = IVS_SUCCEED;
		break;
	case IA_MODIFING_POLYGON_MOVING_POINT:	// ���������ͬ��
	case IA_MODIFING_POLYGON_ADD_POINT:
		lRet = UpdateIAPolygonPoint(point, ulGraphicGroup);
		break;
	default:
		IVS_LOG(IVS_LOG_ERR, "Update IA Polygon", "Modify Type Invalid");
		break;
	}

	if (bFinish && (IVS_SUCCEED == lRet))
	{
		m_ulCurrentState = IA_DONOTHING;
		LONG lInnerRet = InsertCurrentToList();
		if (IVS_SUCCEED != lInnerRet)
		{
			return lInnerRet;
		}
	}
	return lRet;
}

// �����϶�ͼ��ʱ����ͼ��
LONG CIADrawer::UpdateIAMove(POINT point, ULONG ulGraphicGroup, BOOL bFinish)
{
	// ���øú���֮ǰ�Ѿ������m_IAGraphic
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Update IA Move", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}
	POINT tmp_Points[POINTS_NUM_MAX];	// ��ʱ�����ݴ��޸Ľ��
	memset(tmp_Points, 0 , sizeof(POINT) * POINTS_NUM_MAX);
	LONG xDistance = point.x - m_RecordPoint.x;
	LONG yDistance = point.y - m_RecordPoint.y;
	CAutoLock Lock(m_Mutex);

	// �ж��ƶ��е�ǰͼ�ε�������Ƿ����Ƿ�����
	for (ULONG i = 0; i < m_IAGraphic.iaGraphic.ulPointsNum; i++)
	{
		tmp_Points[i].x = m_OriginIAPoints[i].x + xDistance;
		tmp_Points[i].y = m_OriginIAPoints[i].y + yDistance;
		if ((!IsPointInside(&tmp_Points[i])) 
			|| (IsInForbiddenArea(tmp_Points[i], ulGraphicGroup)))
		{
			IVS_LOG(IVS_LOG_ERR, "Update IA Move", "Point Illegal");
			return IVS_FAIL;
		}
	}

	// �ж��ƶ�����λ�õĵ�ǰͼ���Ƿ�������ͼ�ηǷ��ص�
	if (IsForbiddenOverlaped(tmp_Points, m_IAGraphic.iaGraphic.ulPointsNum, 
								ulGraphicGroup, m_ulCurrentGraphicType))		
	{
		IVS_LOG(IVS_LOG_ERR, "Update IA Move", "Point Illegal");
		return IVS_FAIL;
	}
	
	// �޸ĺϷ�,����ʱ�����еĵ㿽����m_IAGraphic��
	memcpy_s(m_IAGraphic.iaGraphic.IAPoints, sizeof(POINT) * POINTS_NUM_MAX, 
		tmp_Points, sizeof(POINT) * m_IAGraphic.iaGraphic.ulPointsNum);

	if (m_IAGraphic.iaGraphic.ulArrowType)
	{
		GetArrowPoints(m_IAGraphic.iaGraphic);
	}

	if (bFinish)
	{
		m_ulCurrentState = IA_DONOTHING;
		return InsertCurrentToList();
	}

	return IVS_SUCCEED;
	
}

// �϶�����ε����ڵ�ʱ����ͼ��
LONG CIADrawer::UpdateIAPolygonPoint(POINT point, ULONG ulGraphicGroup)
{
	// ���øú���֮ǰ�Ѿ������m_IAGraphic
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Update IA Polygon Point", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}
	POINT tmp_Points[POINTS_NUM_MAX];
	memset(tmp_Points, 0, sizeof(POINT) * POINTS_NUM_MAX);
	memcpy_s(tmp_Points, sizeof(POINT) * POINTS_NUM_MAX, 
		m_IAGraphic.iaGraphic.IAPoints, 
		sizeof(POINT) * m_IAGraphic.iaGraphic.ulPointsNum);

	// �жϼ�¼�¶���������±��Ƿ�Ϸ�
	if (POINTNO_ILLEGAL == m_lPointNo)
	{
		IVS_LOG(IVS_LOG_ERR, "Update IA Polygon Point", "PointNo Invalid");
		return IVS_FAIL;
	}

	tmp_Points[m_lPointNo] = point;
	// �޸ĵĵ�Ϊ��ʼ������ʱ��Ҫͬʱ�޸����һ���������
	if (0 == m_lPointNo)
	{
		ULONG ulPointNum = m_IAGraphic.iaGraphic.ulPointsNum - 1;
		tmp_Points[ulPointNum] = tmp_Points[0];
	}

	// �ж϶�����Ƿ�������ͼ�ηǷ��ص�������������Ƿ�Ϸ�
	if ((!IsForbiddenOverlaped(tmp_Points, m_IAGraphic.iaGraphic.ulPointsNum, 
								ulGraphicGroup, IA_GRAPHICS_TYPE_POLYGON)) 
		&& IsIAPolygonSelfLegal(tmp_Points, m_IAGraphic.iaGraphic.ulPointsNum))
	{
		memcpy_s(m_IAGraphic.iaGraphic.IAPoints, sizeof(POINT) * POINTS_NUM_MAX, 
			tmp_Points, sizeof(POINT) * m_IAGraphic.iaGraphic.ulPointsNum);
		return IVS_SUCCEED;
	}
	IVS_LOG(IVS_LOG_INFO, "Update IA Polygon Point", "Point Illegal");
	return IVS_FAIL;
}

// �϶�U���ߵ����ڵ�ʱ����ͼ��
LONG CIADrawer::UpdateIAULine(POINT point, BOOL bFinish)
{
	// ���øú���֮ǰ�Ѿ������m_IAGraphic
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Update IA ULine", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}
	
    // add by wanglei 00165153
    //if ((IsCloseToPoint(point,m_IAGraphic.iaGraphic.IAPoints[1])
    //{
    //    m_ulUlineModifyIndex = 1;
    //}
    //else
    //{
    //    m_ulUlineModifyIndex = 2;
    //}
    //if (IsCloseToPoint(point, m_IAGraphic.iaGraphic.IAPoints[1]))
    // add by wanglei 00165153
    //  DTS2013051709238 �û���ǵ�ǰ�ĸ��������ڱ��޸ģ������������յ㣩
    // ԭ�ȵ�IsCloseToPoint(...)������̫С�������������ٶ�ʧ
	if (m_ulUlineModifyIndex == 1)
	{
		LONG yRange = abs(m_IAGraphic.iaGraphic.IAPoints[1].y 
							- m_IAGraphic.iaGraphic.IAPoints[0].y);
		LONG xLeft  = m_IAGraphic.iaGraphic.IAPoints[0].x;
		LONG xRight = min(m_IAGraphic.iaGraphic.IAPoints[2].x - ULine_BOTTOM_WIDTH_MIN, 
							m_IAGraphic.iaGraphic.IAPoints[0].x + yRange);
		m_IAGraphic.iaGraphic.IAPoints[1].x = max(xLeft,point.x);
		m_IAGraphic.iaGraphic.IAPoints[1].x = min(xRight,m_IAGraphic.iaGraphic.IAPoints[1].x);
	}
    else
	//if (IsCloseToPoint(point, m_IAGraphic.iaGraphic.IAPoints[2]))
	{
		LONG yRange = abs(m_IAGraphic.iaGraphic.IAPoints[2].y 
							- m_IAGraphic.iaGraphic.IAPoints[3].y);
		LONG xLeft  = max(m_IAGraphic.iaGraphic.IAPoints[1].x 
							+ ULine_BOTTOM_WIDTH_MIN, m_IAGraphic.iaGraphic.IAPoints[3].x - yRange);
		LONG xRight = m_IAGraphic.iaGraphic.IAPoints[3].x;
		m_IAGraphic.iaGraphic.IAPoints[2].x = max(xLeft,point.x);
		m_IAGraphic.iaGraphic.IAPoints[2].x = min(xRight,m_IAGraphic.iaGraphic.IAPoints[2].x);
	}

	m_IAGraphic.bFinish = bFinish;
	return FinishAndInsertIntoGraphicList(FALSE);
}


// ȡ��ѡ�����ܷ���ͼ��
LONG CIADrawer::SelectCancel()
{
	IVS_TRACE();
	CAutoLock Lock(m_Mutex);

	if (m_IADrawGraphicsList.empty())
	{
		IVS_LOG(IVS_LOG_ERR, "Modify Cancel", "No Graphics");
		return IVS_SUCCEED;
	}

	IA_GRAPHICS_LIST_ITER iter    = m_IADrawGraphicsList.begin();
	IA_GRAPHICS_LIST_ITER iterEnd = m_IADrawGraphicsList.end();
	while(iter != iterEnd)
	{
		IA_INNER_GRAPHICS *pIAGraphic = *iter;
		if (NULL == pIAGraphic)
		{
			iter = m_IADrawGraphicsList.erase(iter);
			continue;
		}

		if (pIAGraphic->bEditing)
		{
			pIAGraphic->bEditing = FALSE;
		}
		iter++;
	}
	return IVS_SUCCEED;
}


VOID CIADrawer::InsertCurrentToGraphicList()
{
	if (m_bActive)
	{
		// modify limingjie lWX175483: 2013-5-24 
		// TQE��� InsertCurrentToList����ֵǿ��תvoid
		(void)InsertCurrentToList();
	}

}

// �ж��Ƿ�ﵽ��ͼ�������ͼ��
BOOL CIADrawer::IsFinishDrawGraphicGroup(ULONG ulGraphicGroup, ULONG ulMaxDrawNum) 
{
	ULONG ulCount = 0;
	CAutoLock Lock(m_Mutex);

	// ����ͼ���б�,��¼��ǰͼ�������ͼ������
	IA_GRAPHICS_LIST_ITER iter    = m_IADrawGraphicsList.begin();
	IA_GRAPHICS_LIST_ITER iterEnd = m_IADrawGraphicsList.end();
	while (iter != iterEnd)
	{
		IA_INNER_GRAPHICS *pIAGraphic = *iter;
		if (NULL == pIAGraphic)
		{
			iter = m_IADrawGraphicsList.erase(iter);
			continue;
		}

		if (ulGraphicGroup == pIAGraphic->iaGraphic.ulGraphicGroup)
		{
			ulCount++;
		}
		iter++;
	}
	return (ulCount >= ulMaxDrawNum) ? TRUE : FALSE;
}


//************************************************
//===================�ڲ�ʵ��======================
//************************************************

// ����ͼ���������뵽��ͼ�б���
LONG CIADrawer::FinishAndInsertIntoGraphicList(BOOL bIsDrawing)
{
	// ���øú���֮ǰ�Ѿ������m_IAGraphic
	if (!m_bActive)
	{
		return IVS_FAIL;
	}
	CAutoLock Lock(m_Mutex);

	if (m_IAGraphic.bFinish)
	{
		LONG lRet = IVS_FAIL;
		// ��ͼ״̬��,��ǰͼ�������ͼ�������ﵽ���ʱ
		if (bIsDrawing 
			&& IsFinishDrawGraphicGroup(m_IAGraphic.iaGraphic.ulGraphicGroup, m_ulMaxGroupShapeNum))
		{
			IVS_LOG(IVS_LOG_INFO, "Finish And Insert Into Graphic List", "Reach Max Draw Number");
		}

		m_ulCurrentState = IA_DONOTHING;
		lRet = InsertCurrentToList();
		return lRet;
	}
	return IVS_SUCCEED;
}

LONG CIADrawer::InsertCurrentToList() 
{
	// ������ͼ��ָ�����ڲ��뵽ͼ���б���
	m_IAGraphic.bFinish = TRUE;
	IA_INNER_GRAPHICS *pInsertGraphic = NULL;
	if (NULL == IVS_NEW(pInsertGraphic))
	{
		IVS_LOG(IVS_LOG_ERR, "Insert Current To List", "No Enough Memory");
		return IVS_ALLOC_MEMORY_ERROR;
	}

	if (m_bReversePoint)
	{
		if (ARROW_TYPE_LINE_TO_LEFT == m_IAGraphic.iaGraphic.ulArrowType)
		{
			m_IAGraphic.iaGraphic.ulArrowType = ARROW_TYPE_LINE_TO_RIGHT;
		}else if (ARROW_TYPE_LINE_TO_RIGHT == m_IAGraphic.iaGraphic.ulArrowType)
		{
			m_IAGraphic.iaGraphic.ulArrowType = ARROW_TYPE_LINE_TO_LEFT;
		}
		POINT tmpPoint = m_IAGraphic.iaGraphic.IAPoints[0];
		m_IAGraphic.iaGraphic.IAPoints[0] = m_IAGraphic.iaGraphic.IAPoints[1];
		m_IAGraphic.iaGraphic.IAPoints[1] = tmpPoint;
		GetArrowPoints(m_IAGraphic.iaGraphic);

		m_bReversePoint = FALSE;
	}

	memcpy_s(pInsertGraphic, sizeof(IA_INNER_GRAPHICS), 
		&m_IAGraphic, sizeof(IA_INNER_GRAPHICS));
	m_IADrawGraphicsList.push_back(pInsertGraphic);
	pInsertGraphic = NULL;

	m_bActive = FALSE;
	memset(&m_IAGraphic, 0, sizeof(IA_INNER_GRAPHICS));
	return IVS_SUCCEED;
}

// �����ܷ��������
LONG CIADrawer::DrawIAPolygonShape(POINT point, BOOL bNextPoint, BOOL bFinish, ULONG ulGraphicGroup)
{
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Draw IA Polygon Shape", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}

	// ����ζ���������������ʱ���ش���
	if (POINTS_NUM_MAX < m_IAGraphic.iaGraphic.ulPointsNum)
	{
		IVS_LOG(IVS_LOG_DEBUG, "Draw IA Polygon Shape", "Reach to MAX PointNum");
		return IVS_FAIL;
	}
	if (1 == m_IAGraphic.iaGraphic.ulPointsNum)
	{
		m_IAGraphic.iaGraphic.ulPointsNum++;
	}
	ULONG ulIndex = m_IAGraphic.iaGraphic.ulPointsNum;
	
	// ����ε����ڽ���ʱ���� >= 4
	if (bFinish && m_IAGraphic.iaGraphic.ulPointsNum < 4)
	{
		IVS_LOG(IVS_LOG_ERR, "Draw IA Polygon Shape", "No Enough Polyline");
		return IVS_FAIL;
	}
	// �жϻ�������µ�һ�����Ƿ�Ϸ�
	LONG lResult = IsDrawIAPolygonLegal(point, ulIndex, ulGraphicGroup);
	if (IVS_SUCCEED != lResult)
	{
		IVS_LOG(IVS_LOG_DEBUG, "Draw IA Polygon Shape", "Point Invalid");
		return lResult;
	}

	// ��������µ㿿����ʼ��,���Զ����ӵ���ʼ������
	if (IsCloseToPoint(point,m_IAGraphic.iaGraphic.IAPoints[0]))
	{
		point.x = m_IAGraphic.iaGraphic.IAPoints[0].x;
		point.y = m_IAGraphic.iaGraphic.IAPoints[0].y;
	}
	BOOL bReachMaxPoint = FALSE;
	// �Ƿ�ȷ���õ�Ϊ��������
	if (bNextPoint)
	{		
		if (POINTS_NUM_MAX > ulIndex)
		{
			// ��ʼ���¸���
			m_IAGraphic.iaGraphic.ulPointsNum++;
			m_IAGraphic.iaGraphic.IAPoints[ulIndex].x = point.x;
			m_IAGraphic.iaGraphic.IAPoints[ulIndex].y = point.y;
		}
		else
		{
			bReachMaxPoint = TRUE; //����ﵽ�������ʱ���⴦��
		}
	}
	// ��m_IAGraphic�м�¼��ǰ�����
	m_IAGraphic.iaGraphic.IAPoints[ulIndex - 1].x = point.x;
	m_IAGraphic.iaGraphic.IAPoints[ulIndex - 1].y = point.y;
	
	// ������ν���
	if (bFinish)
	{
		return IsPolygonFinishedLegal(ulGraphicGroup, bReachMaxPoint);
	}
	return IVS_SUCCEED;
}

// �ж϶���λ���������Ƿ�Ϸ�
LONG CIADrawer::IsPolygonFinishedLegal(ULONG ulGraphicGroup, BOOL bReachMaxPoint) 
{
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Is Polygon Finished Legal", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}
	m_IAGraphic.bFinish = TRUE;
	
	m_IAGraphic.iaGraphic.ulPointsNum--;
	ULONG ulIndex = m_IAGraphic.iaGraphic.ulPointsNum - 1;

	// ������û�дﵽ���ʱ
	if ((POINTS_NUM_MAX > m_IAGraphic.iaGraphic.ulPointsNum) && !bReachMaxPoint)
	{
		m_IAGraphic.iaGraphic.ulPointsNum--;
		ulIndex--;
		m_IAGraphic.iaGraphic.IAPoints[ulIndex + 1].x = 0;
		m_IAGraphic.iaGraphic.IAPoints[ulIndex + 1].y = 0;
	 	m_IAGraphic.iaGraphic.IAPoints[ulIndex + 2].x = 0;
	 	m_IAGraphic.iaGraphic.IAPoints[ulIndex + 2].y = 0;
	}

	// �պ����Ķ�������ʼ��
	if ((m_IAGraphic.iaGraphic.IAPoints[ulIndex].x != m_IAGraphic.iaGraphic.IAPoints[0].x) 
		|| (m_IAGraphic.iaGraphic.IAPoints[ulIndex].y != m_IAGraphic.iaGraphic.IAPoints[0].y))
	{
		if (POINTS_NUM_MAX <= m_IAGraphic.iaGraphic.ulPointsNum)
		{
			IVS_LOG(IVS_LOG_ERR, "Is Polygon Finished Legal", "Reach to MAX PointNum");
			return FALSE;
		}
		m_IAGraphic.iaGraphic.ulPointsNum++;
		m_IAGraphic.iaGraphic.IAPoints[ulIndex+1].x = m_IAGraphic.iaGraphic.IAPoints[0].x;
		m_IAGraphic.iaGraphic.IAPoints[ulIndex+1].y = m_IAGraphic.iaGraphic.IAPoints[0].y;
	}

	// �жϱպ�ͼ�κ������Ƿ�����Ϸ���λ�ù�ϵ�Ϸ�
	if (!IsIAPolygonSelfLegal(m_IAGraphic.iaGraphic.IAPoints, m_IAGraphic.iaGraphic.ulPointsNum)
		|| IsForbiddenOverlaped(m_IAGraphic.iaGraphic.IAPoints, 
		m_IAGraphic.iaGraphic.ulPointsNum, ulGraphicGroup, IA_GRAPHICS_TYPE_POLYGON))
	{
		IVS_LOG(IVS_LOG_ERR, "Is Polygon Finished Legal", "Polygon ItSelf Illegal");
		return IVS_FAIL;
	}
	return IVS_SUCCEED;
}

// �ж϶���������Ƿ�Ϸ�
BOOL CIADrawer::IsIAPolygonSelfLegal(const POINT *pPoint, ULONG ulPointsNum)
{
	if ((NULL == pPoint) || (4 > ulPointsNum))
	{
		return FALSE;
	}
	// �����������������߶��Ƿ��ཻ
	for (ULONG i = 0; i < ulPointsNum - 2; i++)
	{
		for (ULONG j = i + 2; j < ulPointsNum - 1; j++)
		{
			if ((0 == i) && ((ulPointsNum - 2) == j))
			{
				continue;
			}
			if (IsLineSegmentCross(pPoint[i], pPoint[i + 1], 
				pPoint[j], pPoint[j + 1]))
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}


// ���ܷ���������U����
LONG CIADrawer::DrawIAULineShape(POINT point, BOOL bNextPoint, BOOL bIsDrawing)
{   
	LONG lRet = IVS_FAIL;
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Draw IA ULine Shape", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}

	if (1 == m_IAGraphic.iaGraphic.ulPointsNum)
	{
		m_IAGraphic.iaGraphic.ulPointsNum++;
	}
	
	switch (m_IAGraphic.iaGraphic.ulPointsNum)
	{
	case 2:  // U���ߵ�ǰ����Ϊ2
		lRet = DrawIAUFirstLine(point, bNextPoint);
		break;
	case 3:  // U���ߵ�ǰ����Ϊ3
		lRet = DrawIAUSecondLine(point,bNextPoint);
		break;
	case 4:  // U���ߵ�ǰ����Ϊ4
		lRet = DrawIAUThirdLine(point,bNextPoint, bIsDrawing);
		break;
	default:
		IVS_LOG(IVS_LOG_ERR, "Draw IA ULine Shape", "PointNum Error");
		break;
	}
	return lRet;
}

// ���ܷ�����U���ߵ�һ����
LONG CIADrawer::DrawIAUFirstLine(POINT point, BOOL bNextPoint) 
{
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Draw IA U_FirstLine", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}

	// U����������ױߵļнǲ��ܴ���135��
	LONG xRange = abs(point.x - m_IAGraphic.iaGraphic.IAPoints[0].x);
	LONG yRange = abs(point.y - m_IAGraphic.iaGraphic.IAPoints[0].y);
	if (xRange > yRange)
	{
		point.x = (point.x > m_IAGraphic.iaGraphic.IAPoints[0].x) 
			? (m_IAGraphic.iaGraphic.IAPoints[0].x + yRange) 
			: (m_IAGraphic.iaGraphic.IAPoints[0].x - yRange);
	}

	// �Ƿ�ȷ���õ�ΪU���ߵڶ�����ͳ�ʼ����һ����
	if (bNextPoint)
	{
		m_IAGraphic.iaGraphic.IAPoints[2] = point;
		m_IAGraphic.iaGraphic.ulPointsNum++;
	}
	m_IAGraphic.iaGraphic.IAPoints[1] = point;
	m_IAGraphic.bFinish     = FALSE;
	return IVS_SUCCEED;
}

// ���ܷ�����U���ߵڶ�����
LONG CIADrawer::DrawIAUSecondLine(POINT point,BOOL bNextPoint)
{   
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Draw IA U_SecondLine", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}

	// U���ߵڶ�����Ϊ�ױ�,����ˮƽ
	m_IAGraphic.iaGraphic.IAPoints[2].x = point.x; 
	m_IAGraphic.iaGraphic.IAPoints[2].y = m_IAGraphic.iaGraphic.IAPoints[1].y;

	// U�������߱���Ϊ��ֱ������,���ݵ�һ�͵ڶ������λ��ȷ��ˮƽ�ߵ����ҷ���
	if (m_IAGraphic.iaGraphic.IAPoints[1].x > m_IAGraphic.iaGraphic.IAPoints[0].x)
	{
		m_IAGraphic.iaGraphic.IAPoints[2].x 
			= max(point.x, m_IAGraphic.iaGraphic.IAPoints[1].x + ULine_BOTTOM_WIDTH_MIN);
	}
	if (m_IAGraphic.iaGraphic.IAPoints[1].x < m_IAGraphic.iaGraphic.IAPoints[0].x)
	{
		m_IAGraphic.iaGraphic.IAPoints[2].x 
			= min(point.x, m_IAGraphic.iaGraphic.IAPoints[1].x - ULine_BOTTOM_WIDTH_MIN);
	}
	
	// �Ƿ�ȷ���õ�ΪU���ߵ�������ͳ�ʼ����һ����
	if (bNextPoint)
	{
		m_IAGraphic.iaGraphic.IAPoints[3] = m_IAGraphic.iaGraphic.IAPoints[2];
		m_IAGraphic.iaGraphic.ulPointsNum++;
	}
	
	m_IAGraphic.bFinish = FALSE;
	return IVS_SUCCEED;

}

// ���ܷ�����U���ߵ�������
LONG CIADrawer::DrawIAUThirdLine(POINT point,BOOL bNextPoint, BOOL bIsDrawing)
{   
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Draw IA U_ThirdLine", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}

	// ����ǰ����ȷ���ĵ�,�жϵ��ĸ���ĺϷ�����,��Խ��ĵ�����Ӧ����
	LONG xRange = abs(point.x - m_IAGraphic.iaGraphic.IAPoints[2].x);
	LONG yRange = abs(point.y - m_IAGraphic.iaGraphic.IAPoints[2].y);
	int xflag = (point.x - m_IAGraphic.iaGraphic.IAPoints[2].x) 
		* (m_IAGraphic.iaGraphic.IAPoints[2].x - m_IAGraphic.iaGraphic.IAPoints[0].x);
	int yflag = (point.y - m_IAGraphic.iaGraphic.IAPoints[2].y) 
		* (m_IAGraphic.iaGraphic.IAPoints[2].y - m_IAGraphic.iaGraphic.IAPoints[0].y);
	m_IAGraphic.iaGraphic.IAPoints[3] = point;

	// �жϵ��ĸ����x�����Ƿ��ڵ�һ�͵�������֮��
	// 0 >= xflag�����ڵ�һ�͵�������֮��,�������������ŵ�Ҫ��
	if (0 >= xflag) 
	{
		m_IAGraphic.iaGraphic.IAPoints[3].x = m_IAGraphic.iaGraphic.IAPoints[2].x;
	}
	else
	{
		// U����������ױߵļнǲ��ܴ���135��
		if (xRange > yRange)
		{
			if (m_IAGraphic.iaGraphic.IAPoints[2].x > m_IAGraphic.iaGraphic.IAPoints[0].x)
			{
				m_IAGraphic.iaGraphic.IAPoints[3].x = m_IAGraphic.iaGraphic.IAPoints[2].x + yRange;
			}
			else
			{
				m_IAGraphic.iaGraphic.IAPoints[3].x = m_IAGraphic.iaGraphic.IAPoints[2].x - yRange;
			}
		}
	}

	// U�������߱����ڵױ�ͬһ��
	if (0 < yflag)
	{
		m_IAGraphic.iaGraphic.IAPoints[3] = m_IAGraphic.iaGraphic.IAPoints[2];
		if (bIsDrawing)
		{
			return IVS_FAIL;
		}
	}
	if (bNextPoint)
	{
		m_IAGraphic.bFinish = TRUE;
	}
	
	return IVS_SUCCEED;
}





// ��ʾ���ܷ�������ͼ��
VOID CIADrawer::ShowIAShape(HDC hDC, IA_INNER_GRAPHICS &pGraphics)
{
	IA_GRAPHICS &rGraphic = pGraphics.iaGraphic;
	COLORREF color  = RGB(rGraphic.LineColor.Red,
						rGraphic.LineColor.Green, 
						rGraphic.LineColor.Blue);
	INT	iLineWidth = (INT)max(0.5 >= m_fDiv ? 1 : 2, pGraphics.iaGraphic.ulLineWidth * m_fDiv + 0.5);

// 	INT iLineWidth  = (INT)pGraphics.iaGraphic.ulLineWidth + (INT)m_lLineWidthAdjust;
// 	iLineWidth = (LONG)max(1, iLineWidth);

	HBRUSH brush    = (HBRUSH)GetStockObject(NULL_BRUSH);
	HPEN   pen      = CreatePen(PS_SOLID, iLineWidth, color);
	HPEN   oldPen   = (HPEN)SelectObject(hDC, (HGDIOBJ)pen);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, (HGDIOBJ)brush);

	
	if (IA_GRAPHICS_TYPE_RECTANGLE == pGraphics.iaGraphic.ulGraphicType)
	{
		Rectangle(hDC, pGraphics.iaGraphic.IAPoints[0].x, 
				pGraphics.iaGraphic.IAPoints[0].y, 
				pGraphics.iaGraphic.IAPoints[2].x, 
				pGraphics.iaGraphic.IAPoints[2].y);
		
	}
	else
	{
		POINT *points      = (POINT*)pGraphics.iaGraphic.IAPoints;
		ULONG  ulPointsNum = pGraphics.iaGraphic.ulPointsNum;
		ShowPolyline(hDC, points, ulPointsNum, color);
	}
	if (pGraphics.iaGraphic.ulArrowType)
	{
		ShowArrowShape(hDC, pGraphics.iaGraphic.IAArrow, pGraphics.iaGraphic.ulArrowType);
	}
	// �ͷ���Դ
	SelectObject(hDC,oldPen);
	SelectObject(hDC,oldBrush);
	DeleteObject(pen);

	// ��ʾͼ�����ڱ��༭�ı�־
	if (pGraphics.bEditing)
	{
		ShowGraphicChoosen(hDC, pGraphics);
	}

	// ��ʾͼ������
	ShowGraphicName(hDC, pGraphics);

	// ��ʾ��͸������
	BOOL bNeedAlphaBlend = (((IA_GRAPHICS_TYPE_RECTANGLE == pGraphics.iaGraphic.ulGraphicType) 
							|| (IA_GRAPHICS_TYPE_POLYGON == pGraphics.iaGraphic.ulGraphicType))
							&& pGraphics.bFinish && pGraphics.iaGraphic.BgColor.Alpha);
	if (bNeedAlphaBlend)
	{	
		ShowAlphaBlendArea(hDC, pGraphics);
	}

	return;
}

VOID CIADrawer::ShowArrowShape(HDC hDC, const IA_ARROW &iaArrow, ULONG ulArrowType)
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
	return;
}

// ��ʾ���ܷ��������
VOID CIADrawer::ShowPolyline(HDC hDC, const POINT *points, ULONG numberPoints, COLORREF /*color*/)
{
	if ((NULL == points) || (POINTS_NUM_MAX < numberPoints))
	{
		IVS_LOG(IVS_LOG_ERR, "Show Polyline", "Input invalid.");
		return;
	}
	MoveToEx(hDC,points[0].x,points[0].y,NULL);
	for (ULONG i=1; i<numberPoints; i++)
	{
		LineTo(hDC,points[i].x, points[i].y);
	}
	return;
}

// ѡ�����ܷ���ͼ��
VOID CIADrawer::ShowGraphicChoosen(HDC hDC, const IA_INNER_GRAPHICS &pIAGraphics)
{
	const IA_GRAPHICS &rGraphic = pIAGraphics.iaGraphic;
	COLORREF color  = RGB(rGraphic.LineColor.Red,
						  rGraphic.LineColor.Green, 
						  rGraphic.LineColor.Blue);
	INT iLineWidth = (INT)max(0.5 >= m_fDiv ? 1 : 2, pIAGraphics.iaGraphic.ulLineWidth * m_fDiv + 0.5);
// 	INT iLineWidth  = (INT)pIAGraphics.iaGraphic.ulLineWidth + (INT)m_lLineWidthAdjust;
// 	iLineWidth = (LONG)max(1, iLineWidth);

	HBRUSH   brush    =(HBRUSH)GetStockObject(WHITE_BRUSH);
	HPEN     pen      = CreatePen(PS_SOLID, iLineWidth, color);
	HPEN     oldPen   = (HPEN)SelectObject(hDC, (HGDIOBJ)pen);
	HBRUSH   oldBrush = (HBRUSH)SelectObject(hDC, (HGDIOBJ)brush);

	// ��ͼ�θ������㻭��С����
	for (ULONG i = 0; i < pIAGraphics.iaGraphic.ulPointsNum; i++)
	{
		// ����radiusΪ��ʾ�޸ı�ǵ�С�����ΰ뾶
		LONG radius = (LONG)iLineWidth + 1;
		Rectangle(hDC, pIAGraphics.iaGraphic.IAPoints[i].x - radius, 
					pIAGraphics.iaGraphic.IAPoints[i].y - radius,
					pIAGraphics.iaGraphic.IAPoints[i].x + radius, 
					pIAGraphics.iaGraphic.IAPoints[i].y + radius);
	}
	SelectObject(hDC,oldPen);
	SelectObject(hDC,oldBrush);
	DeleteObject(pen);	
		
	return;
}

// ��ʾͼ������
VOID CIADrawer::ShowGraphicName(HDC hDC, const IA_INNER_GRAPHICS &pIAGraphics)
{
	RECT rc;
	if (!FindEdgeRectangle(pIAGraphics.iaGraphic.IAPoints, pIAGraphics.iaGraphic.ulPointsNum, rc))
	{
		return;
	}
	
	if (NULL == m_hFont)
	{
		IVS_LOG(IVS_LOG_ERR, "Show Graphic Name", "m_hFont NULL");
		return;
	}
	const IA_GRAPHICS &rGraphic = pIAGraphics.iaGraphic;
	COLORREF color  = RGB(rGraphic.LineColor.Red,
						  rGraphic.LineColor.Green, 
						  rGraphic.LineColor.Blue);
	SetTextColor(hDC, color); //ǰ��   
	SetBkMode(hDC,TRANSPARENT);
	SelectObject(hDC,m_hFont);
	switch (pIAGraphics.iaGraphic.NamePosition)
	{
	case GRAPHIC_NAME_ABOVE:
		// ����ͼ���ϱ߽�ճ�5������
		TextOut(hDC, rc.left, rc.top + m_lf.lfHeight - 5, 
				pIAGraphics.iaGraphic.GraphicGroupName, 
				(int)strlen(pIAGraphics.iaGraphic.GraphicGroupName));
		break;
	case GRAPHIC_NAME_UNDER:
		// ����ͼ���±߽�ճ�5������
		TextOut(hDC, rc.left, rc.bottom + 5, 
				pIAGraphics.iaGraphic.GraphicGroupName, 
				(int)strlen(pIAGraphics.iaGraphic.GraphicGroupName));
		break;
	case GRAPHIC_NAME_INSIDE:
		TextOut(hDC, rc.left, rc.bottom + m_lf.lfHeight - 5, 
				pIAGraphics.iaGraphic.GraphicGroupName, 
				(int)strlen(pIAGraphics.iaGraphic.GraphicGroupName));
		break;
	default:
		IVS_LOG(IVS_LOG_ERR, "Show Graphic Name", "Name Position Invalid");
		break;
	}

}



// �жϵ��Ƿ����߶���
BOOL CIADrawer::IsOnLine(const POINT *pPointA, const POINT* pPointB, POINT point)
{  
	LONG CompWidth = (LONG)(m_ulDCWidth / 200); 

	BOOL bRet = IsPointOnLine(pPointA, pPointB, point);
	CHECK_RESULT(bRet, TRUE);

	LONG xflag = (point.x - pPointA->x) * (point.x - pPointB->x);
	LONG yflag = (point.y - pPointA->y) * (point.y - pPointB->y);
	if ((xflag > 0) || (yflag > 0))
	{ 
		return   FALSE; 
	} 

	if (pPointA->x != pPointB->x)
	{ 
		double k  = (double)(pPointB->y - pPointA->y) 
			/ (double)(pPointB->x - pPointA->x); 
		double kt = - 1.0 / k; 
		double xt = (double)((((point.y - pPointB->y) - kt * point.x) + k * pPointB->x) 
			/ (double)(k - kt)); 
		double yt = (double)(point.y - kt * (point.x - xt)); 
		double d  = sqrt((double)(yt - point.y) * (yt - point.y) 
			+ (xt - point.x) * (xt - point.x)); 
		if (d < (double)CompWidth)
		{ 
			return   TRUE; 
		} 
	} 
	return   FALSE; 
}

// �жϵ��Ƿ������ϣ��ڲ���
BOOL CIADrawer::IsPointOnLine(const POINT *pPointA, const POINT *pPointB, POINT point)
{
	LONG CompWidth = static_cast<LONG>(m_ulDCWidth) / 200; 

	if (pPointA->x == pPointB->x)
	{
		LONG yMin = min(pPointA->y, pPointB->y);
		LONG yMax = max(pPointA->y, pPointB->y);
		if ((point.y > yMin) && (point.y < yMax) 
			&& (point.x > pPointA->x - CompWidth) 
			&& (point.x < pPointA->x + CompWidth))
		{ 
			return   TRUE; 
		}
	} 
	if (pPointA->y == pPointB->y)
	{
		LONG xMin = min(pPointA->x, pPointB->x);
		LONG xMax = max(pPointA->x, pPointB->x);
		if ((point.x > xMin) && (point.x < xMax) 
			&& (point.y > pPointA->y - CompWidth) 
			&& (point.y < pPointA->y + CompWidth))
		{ 
			return   TRUE; 
		}
	}
	return FALSE;
}

// �жϵ��Ƿ���ͼ����
LONG CIADrawer::IsInShape(const IA_INNER_GRAPHICS *pIAGraphic, POINT point)
{
	if ((NULL == pIAGraphic) || (2 > pIAGraphic->iaGraphic.ulPointsNum))
	{
		IVS_LOG(IVS_LOG_ERR, "Is In Shape", "Input invalid.");
		return IVS_PARA_INVALID;
	}

	if ((IA_GRAPHICS_TYPE_STRAIGHT_LINE == pIAGraphic->iaGraphic.ulGraphicType)
		|| (IA_GRAPHICS_TYPE_UP_ULINE == pIAGraphic->iaGraphic.ulGraphicType) 
		|| (IA_GRAPHICS_TYPE_DOWN_ULINE == pIAGraphic->iaGraphic.ulGraphicType))
	{
		return IVS_FAIL;
	}
	
	if (IA_GRAPHICS_TYPE_RECTANGLE == pIAGraphic->iaGraphic.ulGraphicType)
	{
		return IsPointInRectangle(pIAGraphic->iaGraphic.IAPoints, 
			pIAGraphic->iaGraphic.ulPointsNum, point);
	}
	return IsPointInPolygon(pIAGraphic->iaGraphic.IAPoints, 
		pIAGraphic->iaGraphic.ulPointsNum, point);

}

// �жϵ��Ƿ��ھ�����
BOOL CIADrawer::IsPointInRectangle(const POINT *pPoint, ULONG /*ulPointNum*/, POINT point) 
{
	LONG lLeft   = min(pPoint[0].x, pPoint[2].x);
	LONG lRight  = max(pPoint[0].x, pPoint[2].x);
	LONG lTop    = min(pPoint[0].y, pPoint[2].y);
	LONG lButtom = max(pPoint[0].y, pPoint[2].y);
	return ((point.x >= lLeft) && (point.x <= lRight)
		&& (point.y >= lTop) && (point.y <= lButtom));
}


// �жϵ��Ƿ��ڶ������
// �жϵ�point�Ƿ��ڶ�����ڣ������߽磩
// ���Ƕ���Σ����жϸõ����󴩹����������εĽ������
BOOL CIADrawer::IsPointInPolygon(const POINT *pPoint, ULONG ulPointNum, POINT point) 
{
	INT nCross = 0;
	// �ⲿ���ж�ulPointNum��ȷ����С��2
	for (ULONG i = 0; i < ulPointNum - 1; i++) 
	{
		POINT p0 = pPoint[(i + ulPointNum - 1) % ulPointNum];
		POINT p1 = pPoint[i];  
		POINT p2 = pPoint[(i + 1) % ulPointNum];

		if (p1.y == p2.y)
		{
			BOOL bRet = (point.y == p1.y) 
						&& (point.x >=  min(p1.x, p2.x)) 
						&& (point.x <=  max(p1.x, p2.x));
			CHECK_RESULT(bRet, TRUE);
			continue;
		}

		if ((p1.y == point.y)&&(point.x >= p1.x))
		{
			LONG range = abs(p0.y - p2.y);
			LONG range0 = abs(p0.y - p1.y);
			LONG range2 = abs(p2.y - p1.y);
			if (range <= max(range0, range2))
			{
				nCross++; 
			}
			continue;
		}

		if ( point.y < min(p1.y, p2.y) )
			continue;
		if ( point.y > max(p1.y, p2.y) )
			continue;

		double x = (double)(point.y - p1.y) * (double)(p2.x - p1.x) 
					/ (double)(p2.y - p1.y) + p1.x;

		if (x == point.x)
			return TRUE;

		if (x < point.x) 
			nCross++;
	}
	return (nCross&1);
}




// ������κϷ�
LONG CIADrawer::IsDrawIAPolygonLegal(POINT newPoint, ULONG ulCurrentPointsNum, ULONG ulGraphicGroup)
{
	if (!m_bActive)
	{
		IVS_LOG(IVS_LOG_ERR, "Is Draw IA Polygon Legal", "m_IAGraphic is Inactive");
		return IVS_FAIL;
	}

	if (3 <= ulCurrentPointsNum)
	{
		for (ULONG i = 0; i < ulCurrentPointsNum - 3; i++)
		{
			if (IsLineSegmentCross(m_IAGraphic.iaGraphic.IAPoints[i], 
				m_IAGraphic.iaGraphic.IAPoints[i+1], 
				m_IAGraphic.iaGraphic.IAPoints[ulCurrentPointsNum-2], newPoint)
				&& ((m_IAGraphic.iaGraphic.IAPoints[i].x != newPoint.x) 
				|| (m_IAGraphic.iaGraphic.IAPoints[i].y != newPoint.y)))
			{
				return IVS_FAIL;
			}
		}
	}
	if (IsCrossForbiddenArea(newPoint, 
		m_IAGraphic.iaGraphic.IAPoints[ulCurrentPointsNum - 2], 
		ulGraphicGroup))
	{
		return IVS_FAIL;
	}
	return IVS_SUCCEED;
}



// �ж������߶��Ƿ��ཻ(�н���) 
BOOL CIADrawer::IsLineSegmentCross(POINT pFirst1, POINT pFirst2, POINT pSecond1, POINT pSecond2)  
{  
	//ÿ���߶ε����㶼����һ���߶ε����Ҳ�ͬ�࣬���ܶ϶��߶��ཻ  
	//��ʽ��������(x1,y1)->(x2,y2),�жϵ�(x3,y3)�����������,�ұ�,��������.  
	//p=x1(y3-y2)+x2(y1-y3)+x3(y2-y1).p<0 ���,    p=0 ����, p>0 �Ҳ�  
	LONG Linep1 = 0, Linep2 = 0;  
	//�ж�pSecond1��pSecond2�Ƿ���pFirst1->pFirst2����  
	Linep1 = pFirst1.x * (pSecond1.y - pFirst2.y) +  
		pFirst2.x * (pFirst1.y - pSecond1.y) +  
		pSecond1.x * (pFirst2.y - pFirst1.y); //lint !e838 
	Linep2 = pFirst1.x * (pSecond2.y - pFirst2.y) +  
		pFirst2.x * (pFirst1.y - pSecond2.y) +  
		pSecond2.x * (pFirst2.y - pFirst1.y);//lint !e838  
	// �������߶���ͬһ��ʱ����Ҫ�ж��߶��Ƿ�����
	if ((0 == Linep1) && (0 == Linep2))
	{
		if (!IsOnLine(&pFirst1, &pFirst2, pSecond1) 
			&& !(IsOnLine(&pFirst1, &pFirst2, pSecond2)))
		{
			return FALSE;
		}
	}
	//����λ���Ϊ0:pSecond1��pSecond2��pFirst1->pFirst2ͬ�� 
	if (((Linep1 ^ Linep2) >= 0 ) && !(Linep1 == 0 && Linep2 == 0)) 
	{  
		return FALSE;  
	}

	//�ж�pFirst1��pFirst2�Ƿ���pSecond1->pSecond2����  
	Linep1 = pSecond1.x * (pFirst1.y - pSecond2.y) +  
		pSecond2.x * (pSecond1.y - pFirst1.y) +  
		pFirst1.x * (pSecond2.y - pSecond1.y);  
	Linep2 = pSecond1.x * (pFirst2.y - pSecond2.y) +  
		pSecond2.x * (pSecond1.y - pFirst2.y) +  
		pFirst2.x * (pSecond2.y - pSecond1.y); 

	//����λ���Ϊ0:pFirst1��pFirst2��pSecond1->pSecond2ͬ��  
	if (((Linep1 ^ Linep2) >= 0 ) && !(Linep1 == 0 && Linep2 == 0))
	{  
		return FALSE;  
	}  
	//������Ϊ�ཻ  
	return TRUE;  
}






// ����㵽ֱ�������һ��ľ��� X*X + Y*Y
LONG CIADrawer::DistanceToLine(POINT Point, POINT PointStart, POINT PointEnd)
{
	if (PointStart.y == PointEnd.y)  // ˮƽ�ߴ���
	{
		LONG left  = min(PointStart.x, PointEnd.x);
		LONG right = max(PointStart.x, PointEnd.x);
		if (Point.x < left)
		{
			return (Point.x - left) * (Point.x - left) + 
				(Point.y - PointStart.y) * (Point.y - PointStart.y);
		}

		if (Point.x > right)
		{
			return (Point.x - right) * (Point.x - right) + 
				(Point.y - PointStart.y) * (Point.y - PointStart.y);
		}
		return (Point.y - PointStart.y) * (Point.y - PointStart.y);
	} 
	else if (PointStart.x == PointEnd.x)  // ��ֱ�ߴ���
	{
		LONG upper = max(PointStart.y, PointEnd.y);
		LONG lower = min(PointStart.y, PointEnd.y);
		if (Point.y < lower)
		{
			return (lower - Point.y) * (lower - Point.y) + 
				(PointStart.x - Point.x) * (PointStart.x - Point.x);
		}

		if (Point.y > upper)
		{
			return (upper - Point.y) * (upper - Point.y) + 
				(PointStart.x - Point.x) * (PointStart.x - Point.x);
		}
		return (PointStart.x - Point.x) * (PointStart.x - Point.x);
	} 
	else  // б�ߴ���
	{
		LONG X0 = 0, Y0 = 0, X1 = 0, Y1 = 0;
		if (PointStart.x <= PointEnd.x)
		{
			X0 = PointStart.x;
			Y0 = PointStart.y;
			X1 = PointEnd.x;
			Y1 = PointEnd.y;
		}
		else
		{
			X0 = PointEnd.x;
			Y0 = PointEnd.y;
			X1 = PointStart.x;
			Y1 = PointStart.y;
		}

		float K = (float)(Y1 - Y0)/(float)(X1 - X0);
		float X = (K * K * X0 + K * (Point.y - Y0) + Point.x)/(1 + K * K);
		float Y = K*(X - X0) + Y0;

		if (X <= X0)
		{
			return (LONG)((Point.x - X0) * (Point.x - X0) + (Point.y - Y0) * (Point.y - Y0));
		}	   

		if (X >= X1)
		{
			return (LONG)((Point.x - X1) * (Point.x - X1) + (Point.y - Y1) * (Point.y - Y1));
		}
		return (LONG)((Point.x - X) * (Point.x - X) + (Point.y - Y) * (Point.y - Y));
	}
}

// �жϵ�A�Ƿ񿿽���B
BOOL CIADrawer::IsCloseToPoint(POINT pointA, POINT pointB)
{
	LONG width  = pointA.x - pointB.x;
	LONG height = pointA.y - pointB.y;
	return ((width * width + height * height) < CLOSE_TO_LINE_DISTANCE * 2);
}

// �жϵ��Ƿ���ͼ�����
BOOL CIADrawer::IsPointInside(const POINT *point)
{
	return ((0 <= point->x) && (point->x <= ((LONG)m_ulDCWidth)) 
			&& (0 <= point->y) && (point->y <= ((LONG)m_ulDCHeight)));
}

// �жϵ��Ƿ��ڷǷ�����Ѱ����Ӧ��RelativeGraphicGroup�������RelationshipType
BOOL CIADrawer::IsInForbiddenArea(POINT point, ULONG ulGraphicGroup)
{
	BOOL bRet = FALSE;
	CAutoLock Lock(m_Mutex);
	if (m_IARelationShipList.empty())
	{
		return bRet;
	}
	IA_RELATIONS_LIST_ITER RelationIter    = m_IARelationShipList.begin();
	IA_RELATIONS_LIST_ITER RelationIterEnd = m_IARelationShipList.end();
	while (RelationIter != RelationIterEnd)
	{
		IA_RELATIONS *pRelation = *RelationIter;
		if (NULL == pRelation)
		{
			RelationIter = m_IARelationShipList.erase(RelationIter);
			continue;
		}
        RelationIter++;  // modify by xiongfeng ��ֹ��ѭ��
		if (pRelation->ulCurrentGraphicGroup != ulGraphicGroup)
		{
			continue;
		}
		switch (pRelation->ulRelationShipType)
		{
		case IA_GRAPHICS_RELATION_TYPE_BE_COVERED:
			bRet = !IsPointInForbiddenArea(point, pRelation->ulRelativeGraphicGroup, TRUE);
			break;
		case IA_GRAPHICS_RELATION_TYPE_COVER:
		case IA_GRAPHICS_RELATION_TYPE_NOT_CROSS:
			bRet = IsPointInForbiddenArea(point, pRelation->ulRelativeGraphicGroup, FALSE);
			break;
		default:  //�������Ͳ��账��
			break;
		}
	}
	return bRet;
}

// �жϵ��Ƿ��ڷǷ������ڲ������жϵ��Ƿ���RelativeGraphicGroupͼ����
BOOL CIADrawer::IsPointInForbiddenArea(POINT point, ULONG ulGraphicGroup, BOOL bIsIn)
{
	BOOL bFind = FALSE;
	if (m_IADrawGraphicsList.empty())
	{
		return bIsIn;
	}
	IA_GRAPHICS_LIST_ITER iter    = m_IADrawGraphicsList.begin();
	IA_GRAPHICS_LIST_ITER iterEnd = m_IADrawGraphicsList.end();
	while(iter != iterEnd)
	{
		IA_INNER_GRAPHICS *pIAGraphic = *iter;
		if (NULL == pIAGraphic)
		{
			iter = m_IADrawGraphicsList.erase(iter);
			continue;
		}
		iter++;

		if (ulGraphicGroup != pIAGraphic->iaGraphic.ulGraphicGroup) 
		{
			continue;
		}
		bFind = TRUE;
		BOOL bRet = IsInShape(pIAGraphic,point);
		CHECK_RESULT(bRet, TRUE);
	}
	return (bIsIn && !bFind);
}


// �ж��߶��Ƿ���Ƿ������ཻ��Ѱ����Ӧ��RelativeGraphicGroup�������RelationshipType
BOOL CIADrawer::IsCrossForbiddenArea(POINT newPoint, POINT lastPoint, ULONG ulGraphicGroup)
{
	CAutoLock Lock(m_Mutex);
	if (m_IARelationShipList.empty())
	{
		return FALSE;
	}
	IA_RELATIONS_LIST_ITER RelationIter    = m_IARelationShipList.begin();
	IA_RELATIONS_LIST_ITER RelationIterEnd = m_IARelationShipList.end();
	while (RelationIterEnd != RelationIter)
	{
		IA_RELATIONS *pRelation = *RelationIter;
		if (NULL == pRelation)
		{
			RelationIter = m_IARelationShipList.erase(RelationIter);
			continue;
		}
		RelationIter++;

		if (pRelation->ulCurrentGraphicGroup != ulGraphicGroup)
		{
			continue;
		}	
		BOOL bRet = IsLineCrossForbiddenArea(newPoint, lastPoint, 
					pRelation->ulRelativeGraphicGroup);
		CHECK_RESULT(bRet, TRUE);

	}
	return FALSE;

}

// �ж��߶��Ƿ���Ƿ������ཻ���ڲ������ж��µ�һ�����Ƿ���RelativeGraphicGroupͼ���ཻ
BOOL CIADrawer::IsLineCrossForbiddenArea(POINT newPoint, POINT lastPoint, ULONG ulGraphicGroup)
{
	if (m_IADrawGraphicsList.empty())
	{
		return FALSE;
	}
	IA_GRAPHICS_LIST_ITER iter    = m_IADrawGraphicsList.begin();
	IA_GRAPHICS_LIST_ITER iterEnd = m_IADrawGraphicsList.end();
	while(iter != iterEnd)
	{
		IA_INNER_GRAPHICS *pIAGraphic = *iter;
		if (NULL == pIAGraphic)
		{
			iter = m_IADrawGraphicsList.erase(iter);
			continue;
		}
		iter++;

		if (pIAGraphic->iaGraphic.ulGraphicGroup != ulGraphicGroup)
		{
			continue;
		}
		for(ULONG i = 0; i < pIAGraphic->iaGraphic.ulPointsNum - 1; i++)
		{
			BOOL bRet = IsLineSegmentCross(newPoint, lastPoint, 
						pIAGraphic->iaGraphic.IAPoints[i], 
						pIAGraphic->iaGraphic.IAPoints[i + 1]);
			CHECK_RESULT(bRet, TRUE);
		}
	}
	return FALSE;

}

// �ж��ض���ͼ���Ƿ�����������ͼ��λ�ù�ϵ��Ѱ����Ӧ��RelativeGraphicGroup�������RelationshipType
BOOL CIADrawer::IsForbiddenOverlaped(const POINT *pPoint, ULONG ulPointNum, ULONG ulGraphicGroup, ULONG ulGraphicType)
{
	CAutoLock Lock(m_Mutex);
	if ((NULL == pPoint) || (2 > ulPointNum))
	{
		return FALSE;
	}
	IA_RELATIONS_LIST_ITER RelationIter    = m_IARelationShipList.begin();
	IA_RELATIONS_LIST_ITER RelationIterEnd = m_IARelationShipList.end();
	
	while (RelationIter != RelationIterEnd)
	{
		IA_RELATIONS *pRelation = *RelationIter;
		if (NULL == pRelation)
		{
			RelationIter = m_IARelationShipList.erase(RelationIter);
			continue;
		}
        RelationIter++;
		if (ulGraphicGroup != pRelation->ulCurrentGraphicGroup)
        {
            continue;
        }

		switch (pRelation->ulRelationShipType)
		{
		case IA_GRAPHICS_RELATION_TYPE_NONE:
		case IA_GRAPHICS_RELATION_TYPE_SMALLER:
		case IA_GRAPHICS_RELATION_TYPE_BIGGER:
			continue;
		default: //����������Ҫ����
			break;
		}
		BOOL bRet = IsAreaForbiddenOverlaped(pPoint, ulPointNum, 
			pRelation->ulRelativeGraphicGroup, ulGraphicType, 
			pRelation->ulRelationShipType);
		CHECK_RESULT(bRet, TRUE);
	}
	return FALSE;

}

// �ж��ض���ͼ���Ƿ�����������ͼ��λ�ù�ϵ���ڲ���������IADrawGraphicsList���ҵ�RelativeGraphicGroup��ͼ��
BOOL CIADrawer::IsAreaForbiddenOverlaped(const POINT *pPoint, 
										 ULONG ulPointNum, 
										 ULONG ulGraphicGroup, 
										 ULONG ulGraphicType, 
										 RELATION_SHIP_TYPE ulRelationShipType)
{
	// ������ϲ��б�֤��Ϊ��
	IA_GRAPHICS_LIST_ITER iter    = m_IADrawGraphicsList.begin();
	IA_GRAPHICS_LIST_ITER iterEnd = m_IADrawGraphicsList.end();
	while (iter != iterEnd)
	{
		IA_INNER_GRAPHICS *pIAGraphic = *iter;
		if (NULL == pIAGraphic)
		{
			iter = m_IADrawGraphicsList.erase(iter);
			continue;
		}
        iter++;
		if ((ulGraphicGroup != pIAGraphic->iaGraphic.ulGraphicGroup) 
			|| ((IA_GRAPHICS_TYPE_RECTANGLE != pIAGraphic->iaGraphic.ulGraphicType) 
				&& (IA_GRAPHICS_TYPE_POLYGON != pIAGraphic->iaGraphic.ulGraphicType)))
			continue;

		BOOL bisInShape = IsShapeForbiddenOverlaped(pIAGraphic, pPoint, 
							ulPointNum, ulGraphicType, ulRelationShipType);
		CHECK_RESULT(bisInShape, TRUE);
		
	}
	return FALSE;
}

// �ж��ض���ͼ���Ƿ�����������ͼ��λ�ù�ϵ���ڲ������жϵ�ǰͼ���Ƿ���RelativeGraphicGroupͼ�ηǷ��ص�
BOOL CIADrawer::IsShapeForbiddenOverlaped(const IA_INNER_GRAPHICS *pIAGraphic, 
										  const POINT *pPoint, 
										  ULONG ulPointNum, 
										  ULONG ulGraphicType, 
										  RELATION_SHIP_TYPE ulRelationShipType)
{
	// ������ϲ��б�֤��Ϊ��
	for (ULONG i = 0; i < pIAGraphic->iaGraphic.ulPointsNum -1; i++)
	{
		// �ж�RelativeGraphicGroupͼ�εĵ��Ƿ��ڵ�ǰͼ����
		BOOL bisInShape = FALSE;
		switch (ulGraphicType)
		{
		case IA_GRAPHICS_TYPE_RECTANGLE:
			bisInShape = IsPointInRectangle(pPoint, ulPointNum, 
				pIAGraphic->iaGraphic.IAPoints[i]);
			break;
		case IA_GRAPHICS_TYPE_POLYGON:
			bisInShape = IsPointInPolygon(pPoint, ulPointNum, 
				pIAGraphic->iaGraphic.IAPoints[i]);
			break;
		default:  //�������Ͳ��账��
			break;
		}
		if (IA_GRAPHICS_RELATION_TYPE_COVER == ulRelationShipType)
		{
			bisInShape = !bisInShape;
		}
		CHECK_RESULT(bisInShape, TRUE);  // ����ͼ�εĵ��ڷǷ����򣬷���ͼ�ηǷ��ص�

		// �жϱ��������͵ĵ�ǰͼ�εĵ��Ƿ���RelativeGraphicGroupͼ����
		//�����ж�һ���㼴�ɣ��������ཻ�ж�
		BOOL bRet = (!IsInShape(pIAGraphic, pPoint[0]) 
					&& (IA_GRAPHICS_RELATION_TYPE_BE_COVERED == ulRelationShipType));
		CHECK_RESULT(bRet, TRUE);// ��������ͼ�δ��ڲ������ͼ���ڵĵ�

		// �ж�RelativeGraphicGroupͼ�εĸ������Ƿ��뵱ǰͼ���ཻ
		for (ULONG j = 0; j < ulPointNum - 1; j++)
		{
			bRet = IsLineSegmentCross(pPoint[j], pPoint[j + 1], 
					pIAGraphic->iaGraphic.IAPoints[i], 
					pIAGraphic->iaGraphic.IAPoints[i + 1]);
			CHECK_RESULT(bRet, TRUE);   // ����ͼ�ε��߶���Ƿ����򽻲棬����ͼ�ηǷ��ص�
		}
	}
	return FALSE; // δ�ҵ��Ƿ��ص������
}

// �жϾ��γ����Ƿ�Ϸ���Ѱ����Ӧ��RelativeGraphicGroup�������RelationshipType
BOOL CIADrawer::IsRectangleSizeIllegal(const POINT *pPoint, 
									   ULONG ulPointNum, 
									   ULONG ulGraphicGroup)
{
	CAutoLock Lock(m_Mutex);
	if (NULL == pPoint)
	{
		return FALSE;
	}
	IA_RELATIONS_LIST_ITER RelationIter    = m_IARelationShipList.begin();
	IA_RELATIONS_LIST_ITER RelationIterEnd = m_IARelationShipList.end();

	while (RelationIterEnd != RelationIter)
	{
		IA_RELATIONS *pRelation = *RelationIter;
		if (NULL == pRelation)
		{
			RelationIter = m_IARelationShipList.erase(RelationIter);
			continue;
		}
		RelationIter++;

		if (ulGraphicGroup != pRelation->ulCurrentGraphicGroup)
			continue;
		
		BOOL bRectangleSizeIllegal = FALSE;
		switch (pRelation->ulRelationShipType)
		{
		case IA_GRAPHICS_RELATION_TYPE_SMALLER:
		case IA_GRAPHICS_RELATION_TYPE_BIGGER:
			bRectangleSizeIllegal = IsRectangleEdgeIllegal(pPoint, ulPointNum, 
									pRelation->ulRelativeGraphicGroup, 
									pRelation->ulRelationShipType);
			break;
		default://����������Ҫ����
			continue;
		}
		CHECK_RESULT(bRectangleSizeIllegal, TRUE);
	}
	return FALSE;
}

// �жϾ��γ����Ƿ�Ϸ����ڲ���������IADrawGraphicsList���жϸ�ͼ���Ƿ���RelativeGraphicGroupͼ�δ�С��ϵ��ȷ
BOOL CIADrawer::IsRectangleEdgeIllegal(const POINT *pPoint, 
										ULONG ulPointNum, 
										ULONG ulGraphicGroup, 
										RELATION_SHIP_TYPE ulRelationShipType)
{
	// ������ϲ��б�֤��Ϊ��
	IA_GRAPHICS_LIST_ITER iter    = m_IADrawGraphicsList.begin();
	IA_GRAPHICS_LIST_ITER iterEnd = m_IADrawGraphicsList.end();
	while (iterEnd != iter)
	{
		IA_INNER_GRAPHICS *pIAGraphic = *iter;
		if (NULL == pIAGraphic)
		{
			iter = m_IADrawGraphicsList.erase(iter);
			continue;
		}
		iter++;

		if ((ulGraphicGroup != pIAGraphic->iaGraphic.ulGraphicGroup) 
			|| (IA_GRAPHICS_TYPE_RECTANGLE != pIAGraphic->iaGraphic.ulGraphicType))
			continue;
		
		RECT rcCurrent = {0};

		//modify by limingjie lWX175483:2013-5-24
		//���뷵��ֵ�ж�

		(void)FindEdgeRectangle(pPoint, ulPointNum, rcCurrent);

		RECT rcRelated = {0};

		(void)FindEdgeRectangle(pIAGraphic->iaGraphic.IAPoints, 
			pIAGraphic->iaGraphic.ulPointsNum, rcRelated);


		LONG lCurrentWeith  = rcCurrent.right  - rcCurrent.left;
		LONG lCurrentHeight = rcCurrent.bottom - rcCurrent.top;
		LONG lRelatedWeith  = rcRelated.right  - rcRelated.left;
		LONG lRelatedHeight = rcRelated.bottom - rcRelated.top;

		BOOL bRet = FALSE;
		bRet = (IA_GRAPHICS_RELATION_TYPE_BIGGER == ulRelationShipType) 
				&& ((lCurrentWeith < lRelatedWeith) 
					|| (lCurrentHeight < lRelatedHeight));
		CHECK_RESULT(bRet, TRUE);

		bRet = (IA_GRAPHICS_RELATION_TYPE_SMALLER == ulRelationShipType) 
				&& ((lCurrentWeith > lRelatedWeith) 
					|| (lCurrentHeight > lRelatedHeight));
		CHECK_RESULT(bRet, TRUE);
	}
	return FALSE;
}




// Ѱ�Ұ����ض�ͼ�ε���С���ο�
BOOL CIADrawer::FindEdgeRectangle(const POINT *pPoint, ULONG ulPointNum, RECT &rc)
{
	rc.left   = 0x7FFFFFFF;
	rc.right  = 0;
	rc.top    = 0x7FFFFFFF;
	rc.bottom = 0;
	for (ULONG i = 0; i < ulPointNum; i++)
	{
		rc.left   = min(rc.left, pPoint[i].x);
		rc.right  = max(rc.right, pPoint[i].x);
		rc.top    = min(rc.top, pPoint[i].y);
		rc.bottom = max(rc.bottom,pPoint[i].y);
	}
	CHECK_RESULT((rc.left <= rc.right) && (rc.top <= rc.bottom), TRUE);

	return FALSE;
}

// ��ʾͼ���ڰ�͸���������
VOID CIADrawer::ShowAlphaBlendArea(HDC hDC, const IA_INNER_GRAPHICS &pGraphics)
{
	RECT rc;
	if (!FindEdgeRectangle(pGraphics.iaGraphic.IAPoints, pGraphics.iaGraphic.ulPointsNum, rc))
	{
		return;
	}
	for (UINT32 y = 0; y < (UINT32)m_ulDCHeight; y++)
		for (UINT32 x = 0; x < (UINT32)m_ulDCWidth; x++)
			((UINT32 *)m_pvBits)[x + y * (UINT32)m_ulDCWidth] = 0x00000000; 
	const IA_GRAPHICS &rGraphic = pGraphics.iaGraphic;
	COLORREF color  = RGB(rGraphic.LineColor.Red,
						  rGraphic.LineColor.Green, 
						  rGraphic.LineColor.Blue);
	HBRUSH brush    = CreateSolidBrush(color);
	HPEN   pen      = CreatePen(PS_SOLID, 1, color);
	HPEN   oldPen   = (HPEN)SelectObject(m_memhdc, (HGDIOBJ)pen);
	HBRUSH oldBrush = (HBRUSH)SelectObject(m_memhdc, (HGDIOBJ)brush);
	HRGN hPolygonRgn = NULL;
	LONG lWidth = rc.right - rc.left;
	LONG lHeight = rc.bottom - rc.top;

	hPolygonRgn = CreatePolygonRgn(pGraphics.iaGraphic.IAPoints, 
		(int)pGraphics.iaGraphic.ulPointsNum - 1, ALTERNATE);
	if (NULL == hPolygonRgn)
	{
		SelectObject(m_memhdc,oldPen);
		SelectObject(m_memhdc,oldBrush);
		DeleteObject(pen);
		DeleteObject(brush);
		return;
	}
	if (!FillRgn(m_memhdc, hPolygonRgn, brush))
	{
		SelectObject(m_memhdc,oldPen);
		SelectObject(m_memhdc,oldBrush);
		DeleteObject(pen);
		DeleteObject(brush);
		DeleteObject(hPolygonRgn);
		return;
	}

	
	UINT32 uAlpha = (UINT32)pGraphics.iaGraphic.BgColor.Alpha << 24;
	UINT32 BitmapBottom = (UINT32)m_ulDCHeight - (UINT32)rc.top;

	for (UINT32 y = (UINT32)m_ulDCHeight - (UINT32)rc.bottom; y < BitmapBottom; y++)
	{
		UINT32 *pLineStart = (UINT32 *)m_pvBits + y * (UINT32)m_ulDCWidth;
		for (UINT32 x = (UINT32)rc.left; x < (UINT32)rc.right; x++)
		{
			register UINT32 *ptr = pLineStart + x;
			register UINT32  val = *ptr;
			if (val)
			{
				*ptr = val| uAlpha; 
			}
		}
	}
	m_bf.BlendOp = AC_SRC_OVER;
	m_bf.BlendFlags = 0;
	m_bf.SourceConstantAlpha = pGraphics.iaGraphic.BgColor.Alpha;  
	m_bf.AlphaFormat = AC_SRC_ALPHA ;  

	AlphaBlend(hDC, rc.left, rc.top, lWidth, lHeight, m_memhdc, rc.left, rc.top, lWidth, lHeight, m_bf);

	SelectObject(m_memhdc,oldPen);
	SelectObject(m_memhdc,oldBrush);
	DeleteObject(pen);	
	DeleteObject(brush);
	DeleteObject(hPolygonRgn);
	
}


// ���������ڴ�DC����ԭͼ����ͬ��С��λͼ
LONG CIADrawer::CreatBitmapDC(HDC hDC) 
{
	if (0 == m_ulWndHeight)
	{
		IVS_LOG(IVS_LOG_ERR, "Creat Bitmap DC", "MsgWnd Null");
		return IVS_FAIL;
	}

	if (NULL != m_memhdc)
	{
		DeleteDC(m_memhdc);
		m_memhdc = NULL;
	}
	// create a DC for our bitmap -- the source DC for AlphaBlend 
	m_memhdc = CreateCompatibleDC(hDC);
	if (NULL == m_memhdc)
	{
		IVS_LOG(IVS_LOG_ERR, "Creat Bitmap DC", "No Enough Memory");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	ZeroMemory(&m_bmi, sizeof(BITMAPINFO));

	// setup bitmap info 
	// set the bitmap width and height to 60% of the width and height 
	// of each of the three horizontal areas. Later on, the blending 
	// will occur in the center of each of the three areas.
	m_bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_bmi.bmiHeader.biWidth = (LONG)m_ulDCWidth;
	m_bmi.bmiHeader.biHeight = (LONG)m_ulDCHeight;
	m_bmi.bmiHeader.biPlanes = 1;
	m_bmi.bmiHeader.biBitCount = 32;         // four 8-bit components
	m_bmi.bmiHeader.biCompression = BI_RGB;
	m_bmi.bmiHeader.biSizeImage = (DWORD)(m_ulDCWidth * m_ulDCHeight * 4);

	// create our DIB section and select the bitmap into the dc
	m_hbitmap = CreateDIBSection(m_memhdc, &m_bmi, DIB_RGB_COLORS, &m_pvBits, NULL, 0x0);

	if ((NULL == m_hbitmap) || (NULL == m_pvBits))
	{
		IVS_LOG(IVS_LOG_ERR, "Creat Bitmap DC", "No Enough Memory");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	SelectObject(m_memhdc, m_hbitmap);
	for (UINT32 y = 0; y < (UINT32)m_ulDCHeight; y++)
		for (UINT32 x = 0; x < (UINT32)m_ulDCWidth; x++)
			((UINT32 *)m_pvBits)[x + y * (UINT32)m_ulDCWidth] = 0x00000000; 

	return IVS_SUCCEED;
}


LONG CIADrawer::CreateIAFont() 
{
	if (NULL != m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}

	/****************** Windows��������ṩ���㷨****************************************
	//�����ֵ���豸�ġ��߼��ֱ��ʡ����Ǻ�
	FLOAT cyDpi = (FLOAT) GetDeviceCaps (hDC, LOGPIXELSY) ;
	//�����ֵ���豸�ġ��߼��ֱ��ʡ����Ǻ�
	FLOAT cyDpi = (FLOAT) (25.4 * GetDeviceCaps (hDC, VERTRES) / GetDeviceCaps (hDC, VERTSIZE)) ;

	FLOAT y = 200 * cyDpi / 72;
	m_lf.lfHeight = - (int)(fabs(y) / 10.0 + 0.5) ;
	*********************************************************************************/


	/*************************MSDN�ṩ���㷨*******************************************
	m_lf.lfHeight = -MulDiv(20, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	*********************************************************************************/

	// 	INT iHeight = (INT)(15 * m_lLineWidthAdjust);
	// 	m_lf.lfHeight = -MulDiv(iHeight, GetDeviceCaps(hDC, LOGPIXELSY), 72);

	memset(&m_lf, 0, sizeof(LOGFONT));
	m_lf.lfHeight = - m_lFontSize;
	if (12 > m_lFontSize)
	{
		m_lf.lfWeight = FW_LIGHT;
	} else if (20 > m_lFontSize)
	{
		m_lf.lfWeight = FW_MEDIUM;
	} else if (30 > m_lFontSize){
		m_lf.lfWeight = FW_SEMIBOLD;
	} else {
		m_lf.lfWeight = FW_BOLD;
	}
	m_lf.lfCharSet = DEFAULT_CHARSET;
	memcpy_s(m_lf.lfFaceName, sizeof(m_lf.lfFaceName), "Arial\0", strlen("Arial\0"));

	m_hFont = CreateFontIndirect(&m_lf);  
	if (NULL == m_hFont)
	{
		IVS_LOG(IVS_LOG_ERR, "Creat Bitmap DC", "CreatFont Error");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	return IVS_SUCCEED;
}



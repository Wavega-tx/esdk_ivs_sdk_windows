#include "IVS_IADrawAPI.h"
#include "IVSCommon.h"
#include "IVS_IADraw.h"
#include "ivs_log.h"
#include "ivs_error.h"


// �������Ч��
#define CHECK_HANDLE(h) \
	do { IVS_ASSERT(h); if (!(h)) return IVS_FAIL;} while(0)


// ��ʼ�����ܷ�����
IVS_DRAW_API INT Draw_LibInit(const char* pLogPath)
{
	int iRet = 0;
    // ָ��Ŀ¼������̳�����
    const char* pLogPathTemp = "../log/";
    if (NULL != pLogPath && 0 != strlen(pLogPath))
    {
        pLogPathTemp = pLogPath;
    }
	iRet = IVS_LogInit(pLogPathTemp, "mp_log.conf" , "IVS_IA_Draw", "ia_draw", 102, NULL);
    return iRet;
}

// �������ܷ�����
IVS_DRAW_API VOID Draw_LibClean()
{
	IVS_LogClean();
    return;
}

// ��ȡIADraw���
IVS_DRAW_API DRAW_HANDLE Draw_GetHandle()
{
	CIADrawer *pIADrawer = IVS_NEW(pIADrawer);
	if (NULL == pIADrawer)
	{
		IVS_LOG(IVS_LOG_ERR, "Get Handle", "ALLOC MEM ERROR.");
		return (DRAW_HANDLE)NULL;
	}
	return (DRAW_HANDLE)pIADrawer;
}

// ��ʼ�����ܷ�����ͼ
IVS_DRAW_API LONG Draw_Init(DRAW_HANDLE handle, HWND hMsgWnd)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	return pIADrawer->InitIADrawer(hMsgWnd);
}

// �����Ƿ���������ʾ
IVS_DRAW_API LONG Draw_SetDisplayScaleMode(DRAW_HANDLE handle, BOOL bScaleModeFull)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	pIADrawer->SetDisplayScaleMode(bScaleModeFull);
	return IVS_SUCCEED;
}


// �����ȡ���ܷ���ͼ��
IVS_DRAW_API LONG Draw_GetGraphicsByGroup(DRAW_HANDLE handle, ULONG ulGraphicGroup, IA_GRAPHICS *pGraphics, ULONG *pulNum)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	return pIADrawer->GetIAGraphics(ulGraphicGroup, pGraphics, pulNum);
}

// ��ȡ���ܷ���ѡ�е�ͼ��
IVS_DRAW_API LONG Draw_GetGraphicSelected(DRAW_HANDLE handle, IA_GRAPHICS *pGraphic)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	return pIADrawer->GetIAGraphicSelected(pGraphic);
}

// ��ȡ���ܷ�������ͼ��
IVS_DRAW_API LONG Draw_GetGraphicsAll(DRAW_HANDLE handle, IA_GRAPHICS *pGraphics, ULONG *pulNum)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	return pIADrawer->GetIAGraphics(0, pGraphics, pulNum);
}

// �������ܷ���ͼ��
IVS_DRAW_API LONG Draw_SetGraphics(DRAW_HANDLE handle, const IA_GRAPHICS *pGraphics, ULONG ulNum, BOOL bPersent)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	return pIADrawer->SetIAGraphics(pGraphics, ulNum, bPersent);
}

// �������ܷ���ͼ����֮��Ĺ�ϵ
IVS_DRAW_API LONG Draw_SetRelationShip(DRAW_HANDLE handle, 
									   ULONG ulGraphicGroupA, 
									   ULONG ulGraphicGroupB, 
									   RELATION_SHIP_TYPE ulRelationShipType)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	return pIADrawer->SetRelationShip(ulGraphicGroupA, ulGraphicGroupB, ulRelationShipType);
}



// ���ܷ�����ʼ��ֱ��
IVS_DRAW_API LONG Draw_StraightLineStart(DRAW_HANDLE handle, ULONG ulNum, IA_GRAPHICS *pIAGraphics)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;	
	if (IVS_SUCCEED != pIADrawer->WindowPointToGraphicPoint(pIAGraphics->IAPoints))
		return IVS_FAIL;
	return pIADrawer->SetIAStartStatus(ulNum, pIAGraphics);
}

// ���ܷ���������ƶ�����ֱ�߶�Ӧ����
IVS_DRAW_API LONG Draw_StraightLineUpDate(DRAW_HANDLE handle, POINT point)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	if (IVS_SUCCEED != pIADrawer->WindowPointToGraphicPoint(&point))
		return IVS_FAIL;
	return pIADrawer->DrawIAStraightLine(point, FALSE, FALSE);
}

// ���ܷ���������ֱ�߲��趨�յ�
IVS_DRAW_API LONG Draw_StraightLineFinish(DRAW_HANDLE handle, POINT point, ULONG /*ulGraphicGroup*/)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	if (IVS_SUCCEED != pIADrawer->WindowPointToGraphicPoint(&point))
	{
		pIADrawer->InsertCurrentToGraphicList();
// 		pIADrawer->StopDrawGraphics(ulGraphicGroup);
		return IVS_SUCCEED;
	}
	return pIADrawer->DrawIAStraightLine(point, TRUE, TRUE);

}

// ���ܷ�����ʼ������
IVS_DRAW_API LONG Draw_RectangleStart(DRAW_HANDLE handle, ULONG ulNum, IA_GRAPHICS *pIAGraphics)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	if (IVS_SUCCEED != pIADrawer->WindowPointToGraphicPoint(pIAGraphics->IAPoints))
		return IVS_FAIL;
	return pIADrawer->SetIAStartStatus(ulNum, pIAGraphics);
}

// ���ܷ���������ƶ����¾��ζ�Ӧ����
IVS_DRAW_API LONG Draw_RectangleUpDate(DRAW_HANDLE handle, POINT point)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	if (IVS_SUCCEED != pIADrawer->WindowPointToGraphicPoint(&point))
		return IVS_FAIL;
	return pIADrawer->DrawIARectangle(point, FALSE, TRUE);
}

// ���ܷ������������β��趨���ո�����
IVS_DRAW_API LONG Draw_RectangleFinish(DRAW_HANDLE handle, POINT point, ULONG /*ulGraphicGroup*/)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	if (IVS_SUCCEED != pIADrawer->WindowPointToGraphicPoint(&point))
	{
		pIADrawer->InsertCurrentToGraphicList();
// 		pIADrawer->StopDrawGraphics(ulGraphicGroup);
		return IVS_SUCCEED;
	}
	return pIADrawer->DrawIARectangle(point, TRUE, TRUE);
}

IVS_DRAW_API LONG Draw_InsertCurrentToGraphicList(DRAW_HANDLE handle)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	pIADrawer->InsertCurrentToGraphicList();
	return IVS_SUCCEED;
}

// ���ܷ�����ʼ�������
IVS_DRAW_API LONG Draw_PolygonStart(DRAW_HANDLE handle, ULONG ulNum, IA_GRAPHICS *pIAGraphics)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	if (IVS_SUCCEED != pIADrawer->WindowPointToGraphicPoint(pIAGraphics->IAPoints))
		return IVS_FAIL;
	return pIADrawer->SetIAStartStatus(ulNum, pIAGraphics);
}

// ���ܷ���������ƶ����¶��������һ���ߵ��յ�����
IVS_DRAW_API LONG Draw_PolygonUpDate(DRAW_HANDLE handle, POINT point, ULONG ulGraphicGroup)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	if (IVS_SUCCEED != pIADrawer->WindowPointToGraphicPoint(&point))
		return IVS_FAIL;
	return pIADrawer->DrawIAPolygon(point, FALSE, FALSE, ulGraphicGroup);
}

// ���ܷ������ȷ��������м�ڵ�λ������
IVS_DRAW_API LONG Draw_PolygonSetPoint(DRAW_HANDLE handle, POINT point, ULONG ulGraphicGroup)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	if (IVS_SUCCEED != pIADrawer->WindowPointToGraphicPoint(&point))
	{
// 		pIADrawer->StopDrawGraphics(ulGraphicGroup);
		return IVS_FAIL;
	}
	return pIADrawer->DrawIAPolygon(point, TRUE, FALSE, ulGraphicGroup);
}

// ���ܷ������ȷ����������һ�߲��ұպ�ͼ��
IVS_DRAW_API LONG Draw_PolygonFinish(DRAW_HANDLE handle, POINT point, ULONG ulGraphicGroup)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	if (IVS_SUCCEED != pIADrawer->WindowPointToGraphicPoint(&point))
	{
// 		pIADrawer->InsertCurrentToGraphicList();
		pIADrawer->StopDrawGraphics(ulGraphicGroup);
		return IVS_SUCCEED;
	}
	return pIADrawer->DrawIAPolygon(point, TRUE, TRUE, ulGraphicGroup);
}

// ���ܷ�����ʼ��U����
IVS_DRAW_API LONG Draw_ULineStart(DRAW_HANDLE handle, ULONG ulNum, IA_GRAPHICS *pIAGraphics)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	if (IVS_SUCCEED != pIADrawer->WindowPointToGraphicPoint(pIAGraphics->IAPoints))
		return IVS_FAIL;
	return pIADrawer->SetIAStartStatus(ulNum, pIAGraphics);
}

// ���ܷ���������ƶ�����U��������һ���ߵ��յ�����
IVS_DRAW_API LONG Draw_ULineUpDate(DRAW_HANDLE handle, POINT point)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	if (IVS_SUCCEED != pIADrawer->WindowPointToGraphicPoint(&point))
		return IVS_FAIL;
	return pIADrawer->DrawIAULine(point, FALSE, FALSE, TRUE);
}

// ���ܷ������ȷ��U�Ϳ��м�ڵ�λ������
IVS_DRAW_API LONG Draw_ULineSetPoint(DRAW_HANDLE handle, POINT point, ULONG /*ulGraphicGroup*/)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	if (IVS_SUCCEED != pIADrawer->WindowPointToGraphicPoint(&point))
	{
// 		pIADrawer->StopDrawGraphics(ulGraphicGroup);
		return IVS_FAIL;
	}
	return pIADrawer->DrawIAULine(point, TRUE, FALSE, TRUE);
}

// ���ܷ������ȷ��U�������һ�߲��ҽ�����U����
// ������ȷ�������ߣ�ֱ�ӽ�����ͼ������������δȷ�������Ե�ǰ��ΪU���ߵ������յ㣩
IVS_DRAW_API LONG Draw_ULineFinish(DRAW_HANDLE handle, POINT point, ULONG /*ulGraphicGroup*/)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	if (IVS_SUCCEED != pIADrawer->WindowPointToGraphicPoint(&point))
	{
		pIADrawer->InsertCurrentToGraphicList();
// 		pIADrawer->StopDrawGraphics(ulGraphicGroup);
		return IVS_SUCCEED;
	}
	return pIADrawer->DrawIAULine(point, TRUE, TRUE, TRUE);
}



// ���ܷ�������ĳͼ����Ļ�ͼ�����������ڻ��ĸ���ͼ�Σ�ֱ�Ӷ�����
IVS_DRAW_API LONG Draw_StopDrawGraphics(DRAW_HANDLE handle, ULONG ulGraphicGroup)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	return pIADrawer->StopDrawGraphics(ulGraphicGroup);
}




// ��ʾ�������ܷ���ͼ��
IVS_DRAW_API LONG Draw_ShowGraphics(DRAW_HANDLE handle,HDC hDC, const RECT *rc)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	return pIADrawer->ShowIAGraphics(hDC, rc);
}


// ɾ������ѡ��ͼ��
IVS_DRAW_API LONG Draw_DeleteGraphicsSelected(DRAW_HANDLE handle)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	return pIADrawer->DeleteIAGraphicsSelected();
}

// ������������ɾ�����ܷ�������ͼ�Σ���������ָ��ͼ����ĵ���ͼ���ڲ���ɾ����ͼ�Σ����ڶ��ͼ���ص�������ɾ����
IVS_DRAW_API LONG Draw_DeleteGraphicsByPoint(DRAW_HANDLE handle, ULONG ulGraphicGroup, POINT point)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	return pIADrawer->DeleteIAGraphicsByPoint(ulGraphicGroup, point);
}

// ���ָ��ͼ���������ͼ��
IVS_DRAW_API LONG Draw_DeleteGraphicsByGroup(DRAW_HANDLE handle, ULONG ulGraphicGroup)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	return pIADrawer->DeleteGraphicsByGroup(ulGraphicGroup);
}

// ������ܷ�������ͼ��
IVS_DRAW_API LONG Draw_DeleteGraphics(DRAW_HANDLE handle)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	return pIADrawer->DeleteIAGraphicsAll();
}



// ������������ѡ�����ܷ�������ͼ�Σ�ѡ���ض������������������ĵ���ͼ�Σ�
IVS_DRAW_API LONG Draw_SelectGraphicByPoint(DRAW_HANDLE handle, POINT point, ULONG *ulGraphicGroup, BOOL *bAllowDel)
{
	CHECK_HANDLE(handle);
	if ((NULL == ulGraphicGroup) || (NULL == bAllowDel))
	{
		return IVS_PARA_INVALID;
	}
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	if (IVS_SUCCEED != pIADrawer->WindowPointToGraphicPoint(&point))
	{
		return IVS_FAIL;
	}
	return pIADrawer->SelectGraphicByPoint(point, *ulGraphicGroup, *bAllowDel);
}

IVS_DRAW_API LONG Draw_GetCursorType(DRAW_HANDLE handle, POINT Point)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	if (IVS_SUCCEED != pIADrawer->WindowPointToGraphicPoint(&Point))
	{
		return CURSOR_TYPE_ARROW;
	}
	return pIADrawer->GetCursorType(Point);
}


// ѡ��ָ��ͼ���������ͼ��
IVS_DRAW_API LONG Draw_SelectGraphicByGroup(DRAW_HANDLE handle, ULONG ulGraphicGroup, BOOL *bAllowDel)
{
	CHECK_HANDLE(handle);
	if (NULL == bAllowDel)
	{
		return IVS_PARA_INVALID;
	}

	CIADrawer *pIADrawer = (CIADrawer *)handle;
	return pIADrawer->SelectGraphicByGroup(ulGraphicGroup, *bAllowDel);
}

// ���ܷ�����ʼ�޸�ͼ��
IVS_DRAW_API LONG Draw_StartModifyGraphics(DRAW_HANDLE handle, POINT point, MOUSE_EVENT_TYPE MouseEvent, ULONG ulGraphicGroup)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	if (IVS_SUCCEED != pIADrawer->WindowPointToGraphicPoint(&point))
		return IVS_FAIL;
	return pIADrawer->StartModifyIAGraphics(point, MouseEvent, ulGraphicGroup);
}

// ���ܷ���������ƶ��޸�ͼ����Ӧ������
IVS_DRAW_API LONG Draw_ModifyShapeUpDate(DRAW_HANDLE handle, ULONG ulGraphicGroup, POINT point)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	if (IVS_SUCCEED != pIADrawer->WindowPointToGraphicPoint(&point))
		return IVS_FAIL;
	return pIADrawer->ModifyIAShapeUpDate(point, FALSE, ulGraphicGroup);
}

// ���ܷ��������޸�ͼ��
IVS_DRAW_API LONG Draw_ModifyFinish(DRAW_HANDLE handle, ULONG ulGraphicGroup, POINT point)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	if (IVS_SUCCEED != pIADrawer->WindowPointToGraphicPoint(&point))
	{
		pIADrawer->InsertCurrentToGraphicList();
		return IVS_SUCCEED;
	}
	return pIADrawer->ModifyIAShapeUpDate(point, TRUE, ulGraphicGroup);
}

// ȡ��ѡ�����ܷ���ͼ��
IVS_DRAW_API LONG Draw_SelectCancel(DRAW_HANDLE handle)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	return pIADrawer->SelectCancel();
}

// �ж��Ƿ�ﵽ��ͼ�������ͼ��
IVS_DRAW_API BOOL Draw_IsFinishDrawGraphicGroup(DRAW_HANDLE handle, ULONG ulGraphicGroup, ULONG ulMaxDrawNum)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	return pIADrawer->IsFinishDrawGraphicGroup(ulGraphicGroup, ulMaxDrawNum);
}



// �ͷ�IADraw���
IVS_DRAW_API LONG Draw_FreeHandle(DRAW_HANDLE handle)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	IVS_DELETE(pIADrawer);
	return IVS_SUCCEED;
}

// add by z00193167
// ������֪�����ܷ����ṹ��ͼ
IVS_DRAW_API LONG Draw_RectangleStartByGraphicPoint(DRAW_HANDLE handle, ULONG ulNum, const IA_GRAPHICS *pIAGraphics)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	return pIADrawer->SetIAStartStatus(ulNum, pIAGraphics);
}

// ���ܷ������������β��趨���ո�����
IVS_DRAW_API LONG Draw_RectangleFinishByGraphicPoint(DRAW_HANDLE handle, POINT point/*, ULONG ulGraphicGroup*/)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	return pIADrawer->DrawIARectangle(point, TRUE, TRUE);
}

IVS_DRAW_API LONG Draw_WindowPointToGraphicPoint(DRAW_HANDLE handle, POINT* pPoint)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	if (IVS_SUCCEED != pIADrawer->WindowPointToGraphicPoint(pPoint))
	{
		pIADrawer->InsertCurrentToGraphicList();
		// 		pIADrawer->StopDrawGraphics(ulGraphicGroup);
		return IVS_SUCCEED;
	}
	
	return IVS_SUCCEED;
}

// ���ܷ�����ʼ�޸�ͼ��
IVS_DRAW_API LONG Draw_StartModifyGraphicsByGraphicPoint(DRAW_HANDLE handle, POINT point, MOUSE_EVENT_TYPE MouseEvent, ULONG ulGraphicGroup)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	return pIADrawer->StartModifyIAGraphics(point, MouseEvent, ulGraphicGroup);
}

// ���ܷ��������޸�ͼ��
IVS_DRAW_API LONG Draw_ModifyFinishByGraphicPoint(DRAW_HANDLE handle, ULONG ulGraphicGroup, POINT point)
{
	CHECK_HANDLE(handle);
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	return pIADrawer->ModifyIAShapeUpDate(point, TRUE, ulGraphicGroup);
}

// ������������ѡ�����ܷ�������ͼ�Σ�ѡ���ض������������������ĵ���ͼ�Σ�
IVS_DRAW_API LONG Draw_SelectGraphicByGraphicPoint(DRAW_HANDLE handle, POINT point, ULONG *ulGraphicGroup, BOOL *bAllowDel)
{
	CHECK_HANDLE(handle);
	if ((NULL == ulGraphicGroup) || (NULL == bAllowDel))
	{
		return IVS_PARA_INVALID;
	}
	CIADrawer *pIADrawer = (CIADrawer *)handle;
	return pIADrawer->SelectGraphicByPoint(point, *ulGraphicGroup, *bAllowDel);
}


/********************************************************************
filename 	: IVS_IADrawAPI.h
author   	: z00227377
created 	: 2012/12/03
description : ���ܷ�����ͼ���ܣ�ʵ��ͼ�ε�����ɾ���ġ���ȡ����ʾ
copyright 	: Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history 	: 2012/12/03 ��ʼ�汾
*********************************************************************/


#ifndef __IADRAW_API_H__
#define __IADRAW_API_H__

#include "IVS_DrawDataType.h"


//����API�ӿ�ǰ׺
#define IVS_DRAW_API extern "C" __declspec(dllexport)

//IADraw���
typedef void * DRAW_HANDLE;



/*****************************************************************************
 �������ƣ�Draw_LibInit
 ������������ʼ�����ܷ�����
 ���������NA  
 ���������NA
 �� �� ֵ��INT
 *****************************************************************************/
IVS_DRAW_API INT Draw_LibInit(const char* pLogPath = NULL);


/*****************************************************************************
 �������ƣ�Draw_LibClean
 �����������������ܷ�����
 ���������NA  
 ���������NA
 �� �� ֵ��VOID
 *****************************************************************************/
IVS_DRAW_API VOID Draw_LibClean();


/*****************************************************************************
 �������ƣ�Draw_GetHandle
 ������������ȡIADraw���
 ���������
  	@handle��       IADraw���
 ���������NA
 �� �� ֵ��IADraw_HANDLE
*****************************************************************************/
IVS_DRAW_API DRAW_HANDLE Draw_GetHandle();


/*****************************************************************************
 �������ƣ�Draw_Init
 ������������ʼ�����ܷ�����ͼ
 ���������
  	@handle��       IADraw���
	@hMsgWnd��      ��ͼ���ھ��
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_Init(DRAW_HANDLE handle, HWND hMsgWnd);


/*****************************************************************************
 �������ƣ�Draw_SetDisplayScaleMode
 ���������������Ƿ���������ʾ
 ���������
  	@handle��			IADraw���
	@bScaleModeFull��	�����Ƿ���������ʾ
 ���������NA 
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_SetDisplayScaleMode(DRAW_HANDLE handle, BOOL bScaleModeFull);


/*****************************************************************************
 �������ƣ�Draw_GetGraphicsByGroup
 ���������������ȡ���ܷ���ͼ��
 ���������
  	@handle��			IADraw���
	@ulGraphicGroup��	ͼ�����ʶ
	@pulNum��			���������Ի�ȡ���ٸ�ͼ��
 ���������	 
	@pGraphics��			����ָ�����ͼ�������ָ��
	@pulNum��			��ȡ����ͼ�θ���
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_GetGraphicsByGroup(DRAW_HANDLE handle, 
										  ULONG ulGraphicGroup, 
										  IA_GRAPHICS *pGraphics, 
									 	  ULONG *pulNum);

/*****************************************************************************
 �������ƣ�Draw_GetIAGraphicsSelected
 ������������ȡ���ܷ���ѡ�е�ͼ��
 ���������
  	@handle��			IADraw���
 ���������	 
	@pGraphic��			����ָ�����ͼ�ε�ָ��
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_GetGraphicSelected(DRAW_HANDLE handle, IA_GRAPHICS *pGraphic);

/*****************************************************************************
 �������ƣ�Draw_GetGraphicsAll
 ������������ȡ���ܷ���ͼ��
 ���������
  	@handle��			IADraw���
 ���������	 
	@pGraphics��			����ָ�����ͼ�������ָ��
	@pulNum��			��ȡ����ͼ�θ���
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_GetGraphicsAll(DRAW_HANDLE handle, 
									  IA_GRAPHICS *pGraphics, 
								      ULONG *pulNum);


/*****************************************************************************
 �������ƣ�Draw_SetRelationShip
 �����������������ܷ���ͼ����֮��Ĺ�ϵ
 ���������
  	@handle��				IADraw���
	@ulGraphicGroupA��		ͼ����A��ʶ
	@ulGraphicGroupB��		ͼ����B��ʶ
	@ulRelationShipType��	ͼ����A�������B�Ĺ�ϵ��In��Out��NotCross��
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_SetRelationShip(DRAW_HANDLE handle, 
									   ULONG ulGraphicGroupA, 
									   ULONG ulGraphicGroupB, 
									   RELATION_SHIP_TYPE ulRelationShipType);


/*****************************************************************************
 �������ƣ�Draw_SetGraphics
 �����������������ܷ���ͼ��
 ���������
  	@handle��       IADraw���
	@pGraphics��    ָ������ͼ�������ָ��
	@ulNum��        ����ͼ�θ���
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_SetGraphics(DRAW_HANDLE handle, const IA_GRAPHICS *pGraphics, ULONG ulNum, BOOL bPersent);


/*****************************************************************************
 �������ƣ�Draw_StraightLineStart
 �������������ܷ�����ʼ��ֱ��
 ���������
  	@handle��					IADraw���
	@ulNum��						��ͼ��������ܻ�ͼ�εĸ���
	@pIAGraphics��				��ʼͼ�ε�������Լ���ʼ��
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_StraightLineStart(DRAW_HANDLE handle, 
										 ULONG ulNum, 
										 IA_GRAPHICS *pIAGraphics);

/*****************************************************************************
 �������ƣ�Draw_StraightLineUpDate
 �������������ܷ���������ƶ�����ֱ�߶�Ӧ����
 ���������
  	@handle��			IADraw���
	@point��				ֱ���յ�����
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_StraightLineUpDate(DRAW_HANDLE handle,POINT point);

/*****************************************************************************
 �������ƣ�Draw_StraightLineFinish
 �������������ܷ���������ֱ�߲��趨�յ�
 ���������
  	@handle��			IADraw���
	@point��				ֱ���յ�����
	@ulGraphicGroup��	ͼ�����ʶ
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_StraightLineFinish(DRAW_HANDLE handle,POINT point, ULONG /*ulGraphicGroup*/);

/*****************************************************************************
 �������ƣ�Draw_RectangleStart
 �������������ܷ�����ʼ������
 ���������
	 @handle��					IADraw���
	 @ulNum��					��ͼ��������ܻ�ͼ�εĸ���
	 @pIAGraphics��				��ʼͼ�ε�������Լ���ʼ��
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_RectangleStart(DRAW_HANDLE handle, 
									  ULONG ulNum,
									  IA_GRAPHICS *pIAGraphics);

/*****************************************************************************
 �������ƣ�Draw_RectangleUpDate
 �������������ܷ���������ƶ����¾��ζ�Ӧ����
 ���������
  	@handle��			IADraw���
	@point��				���ζԶ�������
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_RectangleUpDate(DRAW_HANDLE handle, POINT point);

/*****************************************************************************
 �������ƣ�Draw_RectangleFinish
 �������������ܷ������������β��趨���ո�����
 ���������
  	@handle��			IADraw���
	@point��				���ζԶ�������
	@ulGraphicGroup��	ͼ�����ʶ
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_RectangleFinish(DRAW_HANDLE handle, POINT point, ULONG /*ulGraphicGroup*/);

/*****************************************************************************
 �������ƣ�Draw_InsertCurrentToGraphicList
 ����������ֱ�Ӳ��뵱ǰͼ��
 ���������
  	@handle��			IADraw���
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_InsertCurrentToGraphicList(DRAW_HANDLE handle);

/*****************************************************************************
 �������ƣ�Draw_PolygonStart
 �������������ܷ�����ʼ�������
 ���������
	 @handle��					IADraw���
	 @ulNum��					��ͼ��������ܻ�ͼ�εĸ���
	 @pIAGraphics��				��ʼͼ�ε�������Լ���ʼ��
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_PolygonStart(DRAW_HANDLE handle, 
									ULONG ulNum, 
									IA_GRAPHICS *pIAGraphics);

/*****************************************************************************
 �������ƣ�Draw_PolygonUpDate
 �������������ܷ���������ƶ����¶��������һ���ߵ��յ�����
 ���������
  	@handle��			IADraw���
	@point��				���������һ���ߵ��յ�����
	@ulGraphicGroup��	ͼ�����ʶ
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_PolygonUpDate(DRAW_HANDLE handle, POINT point, ULONG ulGraphicGroup);

/*****************************************************************************
 �������ƣ�Draw_PolygonSetPoint
 �������������ܷ������ȷ��������м�ڵ�λ������
 ���������
  	@handle��			IADraw���
	@point��				������м�ڵ�λ������
	@ulGraphicGroup��	ͼ�����ʶ
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_PolygonSetPoint(DRAW_HANDLE handle, POINT point, ULONG ulGraphicGroup);

/*****************************************************************************
 �������ƣ�Draw_PolygonFinish
 �������������ܷ������ȷ����������һ�߲��ұպ�ͼ��
 ���������
  	@handle��			IADraw���
	@point��				��ǰ�����ͼ�������꣨�õ㲻���������ڣ�
	@ulGraphicGroup��	ͼ�����ʶ
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_PolygonFinish(DRAW_HANDLE handle, POINT point, ULONG ulGraphicGroup);

/*****************************************************************************
 �������ƣ�Draw_ULineStart
 �������������ܷ�����ʼ��U����
 ���������
	@handle��					IADraw���
	@ulNum��						��ͼ��������ܻ�ͼ�εĸ���
	@pIAGraphics��				��ʼͼ�ε�������Լ���ʼ��
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_ULineStart(DRAW_HANDLE handle, 
								  ULONG ulNum,
								  IA_GRAPHICS *pIAGraphics);

/*****************************************************************************
 �������ƣ�Draw_ULineUpDate
 �������������ܷ���������ƶ�����U��������һ���ߵ��յ�����
 ���������
  	@handle��			IADraw���
	@point��				U��������һ���ߵ��յ�����
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_ULineUpDate(DRAW_HANDLE handle, POINT point);

/*****************************************************************************
 �������ƣ�Draw_ULineSetPoint
 �������������ܷ������ȷ��U�Ϳ��м�ڵ�λ������
 ���������
  	@handle��			IADraw���
	@point��				U�����м�ڵ�λ������
	@ulGraphicGroup��	ͼ�����ʶ
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_ULineSetPoint(DRAW_HANDLE handle, POINT point, ULONG /*ulGraphicGroup*/);

/*****************************************************************************
 �������ƣ�Draw_ULineFinish
 �������������ܷ������ȷ��U�������һ�߲��ҽ�����U����
		������ȷ�������ߣ�ֱ�ӽ�����ͼ������������δȷ�������Ե�ǰ��ΪU���ߵ������յ㣩
 ���������
  	@handle��			IADraw���
	@point��				��ǰ�����ͼ��������
	@ulGraphicGroup��	ͼ�����ʶ
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_ULineFinish(DRAW_HANDLE handle, POINT point, ULONG /*ulGraphicGroup*/);


/*****************************************************************************
 �������ƣ�Draw_StopDrawGraphics
 �������������ܷ�������ĳͼ����Ļ�ͼ�����������ڻ��ĸ���ͼ�Σ�ֱ�Ӷ�����
 ���������
  	@handle��			IADraw���
	@ulGraphicGroup��	ͼ�����ʶ
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_StopDrawGraphics(DRAW_HANDLE handle, ULONG ulGraphicGroup);


/*****************************************************************************
 �������ƣ�Draw_ShowGraphics
 ������������ʾ�������ܷ���ͼ��
 ���������
  	@handle��       IADraw���
	@hDC��			��ͼ���
	@rc��			��ͼ���ڵĴ�С
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_ShowGraphics(DRAW_HANDLE handle, HDC hDC, const RECT *rc);

/*****************************************************************************
 �������ƣ�Draw_DeleteGraphicsSelected
 ����������ɾ������ѡ��ͼ��
 ���������
  	@handle��			IADraw���
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_DeleteGraphicsSelected(DRAW_HANDLE handle);


/*****************************************************************************
 �������ƣ�Draw_DeleteGraphicsByPoint
 ����������������������ɾ�����ܷ�������ͼ�Σ���������ָ��ͼ����ĵ���ͼ���ڲ���ɾ����ͼ�Σ����ڶ��ͼ���ص�������ɾ����
 ���������
  	@handle��			IADraw���
	@ulGraphicGroup��	ͼ�����ʶ
	@point��				��ǰ�����ͼ���е������
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_DeleteGraphicsByPoint(DRAW_HANDLE handle, ULONG ulGraphicGroup, POINT point);

/*****************************************************************************
 �������ƣ�Draw_DeleteGraphicsByGroup
 �������������ָ��ͼ���������ͼ��
 ���������
 	@handle��			IADraw���
	@ulGraphicGroup��	ͼ�����ʶ
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_DeleteGraphicsByGroup(DRAW_HANDLE handle, ULONG ulGraphicGroup);

/*****************************************************************************
 �������ƣ�Draw_DeleteGraphics
 ����������������ܷ�������ͼ��
 ���������
 	@handle��       IADraw���
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_DeleteGraphics(DRAW_HANDLE handle);


/*****************************************************************************
 �������ƣ�Draw_SelectGraphicByPoint
 ����������������������ѡ�����ܷ�������ͼ�Σ�ѡ���ض������������������ĵ���ͼ�Σ�
 ���������
  	@handle��			IADraw���
	@point��				��ǰ�����ͼ���е������
���������	
	@ulGraphicGroup��	ͼ�����ʶ
	@bAllowDel��			�Ƿ�����ɾ�����
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_SelectGraphicByPoint(DRAW_HANDLE handle, POINT point, ULONG *ulGraphicGroup, BOOL *bAllowDel);


/*****************************************************************************
 �������ƣ�Draw_GetCursorType
 ������������ȡ�������
 ���������
  	@handle��			IADraw���
	@point��				��ǰ�����ͼ���е������
���������NA	
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_GetCursorType(DRAW_HANDLE handle, POINT Point);

/*****************************************************************************
 �������ƣ�Draw_SelectGraphicByGroup
 ����������ѡ��ָ��ͼ���������ͼ��
 ���������
 	@handle��			IADraw���
	@ulGraphicGroup��	ͼ�����ʶ
 ���������
	@bAllowDel��			�Ƿ�����ɾ�����
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_SelectGraphicByGroup(DRAW_HANDLE handle, ULONG ulGraphicGroup, BOOL *bAllowDel);

/*****************************************************************************
 �������ƣ�Draw_StartModifyGraphics
 �������������ܷ�����ʼ�޸�ͼ��
 ���������
 	@handle��			IADraw���
	@point��				�����ʼ���λ������
	@MouseEvent��		��������ͣ�LEFT_BUTTON_DOWN ��RIGHT_BUTTON_DOWN��
	@ulGraphicGroup��	ͼ�����ʶ
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_StartModifyGraphics(DRAW_HANDLE handle, POINT point, MOUSE_EVENT_TYPE MouseEvent, ULONG ulGraphicGroup);

/*****************************************************************************
 �������ƣ�Draw_ModifyShapeUpDate
 �������������ܷ���������ƶ��޸�ͼ����Ӧ������
 ���������
 	@handle��			IADraw���
	@ulGraphicGroup��	ͼ�����ʶ
	@point��				��굱ǰλ��
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_ModifyShapeUpDate(DRAW_HANDLE handle, ULONG ulGraphicGroup, POINT point);

/*****************************************************************************
 �������ƣ�Draw_ModifyFinish
 �������������ܷ��������޸�ͼ��
 ���������
 	@handle��			IADraw���
	@ulGraphicGroup��	ͼ�����ʶ
	@point��				��굱ǰλ��
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_ModifyFinish(DRAW_HANDLE handle, ULONG ulGraphicGroup, POINT point);

/*****************************************************************************
 �������ƣ�Draw_SelectCancel
 ����������ȡ��ѡ�����ܷ���ͼ��
 ���������
 	@handle��       IADraw���
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_SelectCancel(DRAW_HANDLE handle);


/*****************************************************************************
 �������ƣ�Draw_IsFinishDrawGraphicGroup
 �����������ж��Ƿ�ﵽ��ͼ�������ͼ��
 ���������
 	@handle��			IADraw���
	@ulGraphicGroup:	ͼ�����ʶ
	@ulMaxDrawNum��		ͼ�������ͼ����
 ���������NA
 �� �� ֵ��BOOL
*****************************************************************************/
IVS_DRAW_API BOOL Draw_IsFinishDrawGraphicGroup(DRAW_HANDLE handle, ULONG ulGraphicGroup, ULONG ulMaxDrawNum);


/*****************************************************************************
 �������ƣ�Draw_FreeHandle
 �����������ͷ�IADraw���
 ���������
	@handle��       IADraw���
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_FreeHandle(DRAW_HANDLE handle);

/*****************************************************************************
 �������ƣ�Draw_RectangleStartByGraphicPoint
 �������������ܷ�����ʼ������
 ���������
	 @handle��					IADraw���
	 @ulNum��					��ͼ��������ܻ�ͼ�εĸ���
	 @pIAGraphics��				��ʼͼ�ε�������Լ���ʼ��
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_DRAW_API LONG Draw_RectangleStartByGraphicPoint(DRAW_HANDLE handle, ULONG ulNum, const IA_GRAPHICS *pIAGraphics);

/*****************************************************************************
 �������ƣ�Draw_RectangleFinishByGraphicPoint
 �������������ܷ������������β��趨���ո�����
 ���������
 @handle��			IADraw���
 @point��				���ζԶ�������
 @ulGraphicGroup��	ͼ�����ʶ
 ���������NA
 �� �� ֵ��LONG
 *****************************************************************************/
IVS_DRAW_API LONG Draw_RectangleFinishByGraphicPoint(DRAW_HANDLE handle, POINT point/*, ULONG ulGraphicGroup*/);

/*****************************************************************************
 �������ƣ�Draw_WindowPointToGraphicPoint
 ����������ת��WINDOW����㵽ͼ�������
 ���������
 @handle��			IADraw���
 @pPoint��			��Ҫת���ĵ�
 ���������NA
 �� �� ֵ��LONG
 *****************************************************************************/
IVS_DRAW_API LONG Draw_WindowPointToGraphicPoint(DRAW_HANDLE handle, POINT* pPoint);

/*****************************************************************************
 �������ƣ�Draw_StartModifyGraphicsByGraphicPoint
 �������������ܷ�����ʼ������
 ���������
 @handle��			IADraw���
 @pPoint��			��Ҫת���ĵ�
 @MouseEvent��
 @ulGraphicGroup��
 ���������NA
 �� �� ֵ��LONG
 *****************************************************************************/
IVS_DRAW_API LONG Draw_StartModifyGraphicsByGraphicPoint(DRAW_HANDLE handle, POINT point, MOUSE_EVENT_TYPE MouseEvent, ULONG ulGraphicGroup);

/*****************************************************************************
 �������ƣ�Draw_ModifyFinishByGraphicPoint
 �������������ܷ������������β��趨���ո�����
 ���������
 @handle��			IADraw���
 @ulGraphicGroup��  ��ID
 @pPoint��			��Ҫת���ĵ�
 ���������NA
 �� �� ֵ��LONG
 *****************************************************************************/
IVS_DRAW_API LONG Draw_ModifyFinishByGraphicPoint(DRAW_HANDLE handle, ULONG ulGraphicGroup, POINT point);

/*****************************************************************************
 �������ƣ�Draw_SelectGraphicByGraphicPoint
 �������������ܷ���ѡ��
 ���������
 @handle��			IADraw���
 @pPoint��			��Ҫת���ĵ�
 @ulGraphicGroup��
 @bAllowDel��
 ���������NA
 �� �� ֵ��LONG
 *****************************************************************************/
IVS_DRAW_API LONG Draw_SelectGraphicByGraphicPoint(DRAW_HANDLE handle, POINT point, ULONG *ulGraphicGroup, BOOL *bAllowDel);
#endif // __IADRAW_API_H__


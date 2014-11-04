/********************************************************************
filename 	: IVS_IADraw.h
author   	: z00227377
created 	: 2012/12/03
description : ��ͼ��ʾ��
copyright 	: Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history 	: 2012/12/03 ��ʼ�汾
*********************************************************************/

#ifndef  __CIADRAWER_H__
#define  __CIADRAWER_H__

#include <list>
#include "Lock.h"
#include "IVS_DrawDataType.h"

// ����ͼ�νṹ��
typedef struct
{
	ULONG		ulGraphicsID;	// �ڲ�ͼ��ID
	BOOL		bFinish;		// �Ƿ���ɻ�ͼ��ʶ
	BOOL		bEditing;		// �Ƿ���ѡ��״̬��ʶ
	BOOL		bPersent;		// �Ƿ�ٷֱ�����
	IA_GRAPHICS	iaGraphic;		// ����ͼ�νṹ��

}IA_INNER_GRAPHICS;

// ���ܷ���ͼ���б�
typedef std::list<IA_INNER_GRAPHICS*>    IA_GRAPHICS_LIST;
typedef IA_GRAPHICS_LIST::iterator  IA_GRAPHICS_LIST_ITER;

// ����ͼ����λ�ù�ϵ�ṹ��
typedef struct
{
	ULONG				ulCurrentGraphicGroup;	// ��ǰ��ͼ�����ʶ
	ULONG				ulRelativeGraphicGroup;	// ��ص�ͼ�����ʶ
	RELATION_SHIP_TYPE	ulRelationShipType;		// ��ǰͼ���������ͼ����֮���λ�ù�ϵ����

}IA_RELATIONS;

// ͼ����λ�ù�ϵ�б�
typedef std::list<IA_RELATIONS*>			 IA_RELATIONS_LIST;
typedef IA_RELATIONS_LIST::iterator  IA_RELATIONS_LIST_ITER;


class CIADrawer
{
public:
	CIADrawer();
	~ CIADrawer();
	/*****************************************************************************
	 �������ƣ�initIADrawer
	 ������������ʼ�����ܷ�����ͼ
	 ���������
		@hMsgWnd��      ��ͼ���ھ��
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG InitIADrawer(HWND hMsgWnd);

	/*****************************************************************************
	 �������ƣ�SetDisplayScaleMode
	 ���������������Ƿ���������ʾ
	 ���������
		@bScaleModeFull��			�Ƿ���������ʾ
	 ���������NA
	 �� �� ֵ��VOID
	 *****************************************************************************/	
	VOID SetDisplayScaleMode(BOOL bScaleModeFull);

	/*****************************************************************************
	 �������ƣ�WindowPointToGraphicPoint
	 ����������ת����������Ϊͼ������
	 ����/���������
		@point��			ԭ��������/ת����Ϊ��ͼ��������
	 �� �� ֵ��LONG
	 *****************************************************************************/	
	LONG WindowPointToGraphicPoint(POINT *point);

	/*****************************************************************************
	 �������ƣ�SetIAGraphics
	 �����������������ܷ���ͼ��
	 ���������
		@pGraphics��    ָ������ͼ�������ָ��
		@ulNum��        ����ͼ�θ���
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG SetIAGraphics(const IA_GRAPHICS *pGraphics, ULONG ulNum, BOOL bPersent);

	/*****************************************************************************
	 �������ƣ�SetRelationShip
	 �����������������ܷ���ͼ����֮��Ĺ�ϵ
	 ���������
		@ulGraphicGroupA��		ͼ����A��ʶ
		@ulGraphicGroupB��		ͼ����B��ʶ
		@ulRelationShipType��	ͼ����A�������B�Ĺ�ϵ
	 ���������NA
	 �� �� ֵ��LONG
	*****************************************************************************/
	LONG SetRelationShip(ULONG ulGraphicGroupA, ULONG ulGraphicGroupB, RELATION_SHIP_TYPE ulRelationShipType);	

	/*****************************************************************************
	 �������ƣ�GetIAGraphics
	 ������������ȡ���ܷ���ͼ��
	 ���������
		@ulGraphicGroup��	ͼ�����ʶ
		@pulNum��			���������Ի�ȡ���ٸ�ͼ��
	 ���������	 
		@pGraphics��			����ָ�����ͼ�������ָ��
		@pulNum��			��ȡ����ͼ�θ���
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG GetIAGraphics(ULONG ulGraphicGroup, IA_GRAPHICS *pGraphics, ULONG *pulNum);
	
	/*****************************************************************************
	 �������ƣ�GetIAGraphicsSelected
	 ������������ȡѡ�е����ܷ���ͼ��
	 ���������NA
	 ���������	 
		@pGraphics��			����ָ�����ͼ�ε�ָ��
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG GetIAGraphicSelected(IA_GRAPHICS *pGraphic);

	/*****************************************************************************
	 �������ƣ�SetIAStartStatus
	 �������������ÿ�ʼ��ͼ����ʼ״̬
	 ���������
		@ulNum��						��ͼ��������ܻ�ͼ�εĸ���
		@pIAGraphics��				��ʼͼ�ε�������Լ���ʼ��
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG SetIAStartStatus(ULONG ulNum, const IA_GRAPHICS *pIAGraphics);

	/*****************************************************************************
	 �������ƣ�DrawIAStraightLine
	 �������������ܷ�����ֱ��
	 ���������
		@point��				ֱ���յ�����
		@bFinish:			�Ƿ������ͼ
		@bIsDrawing:		�Ƿ��ǻ�ͼ״̬�����޸�״̬��
	 ���������NA
	 �� �� ֵ��LONG
	*****************************************************************************/
	LONG DrawIAStraightLine(POINT point, BOOL bFinish, BOOL bIsDrawing);

	/*****************************************************************************
	 �������ƣ�DrawIARectangle
	 �������������ܷ���������
	 ���������
		@point��				ֱ���յ�����
		@bFinish:			�Ƿ������ͼ
		@bIsDrawing��		�Ƿ��ǻ�ͼ״̬�����޸�״̬��
	 ���������NA
	 �� �� ֵ��LONG
	*****************************************************************************/
	LONG DrawIARectangle(POINT point, BOOL bFinish, BOOL bIsDrawing);

	/*****************************************************************************
	 �������ƣ�DrawIAPolygon
	 �������������ܷ����������
	 ���������
		@point��				ֱ���յ�����
		@bNextPoint��		�Ƿ�ȷ��һ������
		@bFinish:			�Ƿ������ͼ
		@ulGraphicGroup��	ͼ�����ʶ
	 ���������NA
	 �� �� ֵ��LONG
	*****************************************************************************/
	LONG DrawIAPolygon(POINT point, BOOL bNextPoint, BOOL bFinish, ULONG ulGraphicGroup);

	/*****************************************************************************
	 �������ƣ�DrawIAULine
	 �������������ܷ�����U����
	 ���������
		@point��				ֱ���յ�����
		@bNextPoint��		�Ƿ�ȷ��һ������
		@bFinish:			�Ƿ������ͼ
		@bIsDrawing��		�Ƿ��ǻ�ͼ״̬�����޸�״̬��
	 ���������NA
	 �� �� ֵ��LONG
	*****************************************************************************/
	LONG DrawIAULine(POINT point, BOOL bNextPoint, BOOL bFinish, BOOL bIsDrawing);

	/*****************************************************************************
	 �������ƣ�StopDrawGraphics
	 �������������ܷ�������ĳͼ����Ļ�ͼ�����������ڻ��ĸ���ͼ�Σ�ֱ�Ӷ�����
	 ���������
		@ulGraphicGroup��	ͼ�����ʶ
	 ���������NA
	 �� �� ֵ��LONG
	*****************************************************************************/
	LONG StopDrawGraphics(ULONG ulGraphicGroup);

	/*****************************************************************************
	 �������ƣ�ShowIAGraphics
	 ������������ʾ���ܷ���ͼ��
	 ���������
		@hDC��			��ͼ���
		@rc��			ͼ��Ĵ�С��Ϣ
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG ShowIAGraphics(HDC hDC, const RECT *rc);


	/*****************************************************************************
	 �������ƣ�DeleteIAGraphicsSelected
	 ����������ɾ������ѡ��ͼ��
	 ���������NA
	 ���������NA
	 �� �� ֵ��LONG
	*****************************************************************************/
	LONG DeleteIAGraphicsSelected();

	/*****************************************************************************
	 �������ƣ�DeleteIAGraphics
	 ����������������������ɾ�����ܷ�������ͼ��
			����������ָ��ͼ����ĵ���ͼ���ڲ���ɾ����ͼ�Σ����ڶ��ͼ���ص�������ɾ����
	 ���������
		@ulGraphicGroup��	ͼ�����ʶ
		@point��				��ǰ�����ͼ���е������
	 ���������NA
	 �� �� ֵ��LONG
	*****************************************************************************/
	LONG DeleteIAGraphicsByPoint(ULONG ulGraphicGroup, POINT point);
	
	/*****************************************************************************
	 �������ƣ�DeleteGraphicsByGroup
	 �������������ָ��ͼ���������ͼ��
	 ���������
		@ulGraphicGroup��	ͼ�����ʶ
	 ���������NA
	 �� �� ֵ��LONG
	*****************************************************************************/
	LONG DeleteGraphicsByGroup(ULONG ulGraphicGroup);

	/*****************************************************************************
	 �������ƣ�DeleteIAGraphicsAll
	 ����������������ܷ�������ͼ��
	 ���������NA
	 ���������NA
	 �� �� ֵ��LONG
	*****************************************************************************/
	LONG DeleteIAGraphicsAll();

	/*****************************************************************************
	 �������ƣ�SelectGraphicByPoint
	 ����������������������ѡ�����ܷ�������ͼ�Σ�ѡ���ض������������������ĵ���ͼ�Σ�
	 ���������
		@Point��				��ǰ�����ͼ���е������
	 ���������
		@ulGraphicGroup��	ͼ�����ʶ
		@bAllowDel��			�Ƿ�����ɾ�����
	 �� �� ֵ��LONG
	*****************************************************************************/
	LONG SelectGraphicByPoint(POINT Point, ULONG &ulGraphicGroup, BOOL &bAllowDel);

	/*****************************************************************************
	 �������ƣ�GetCursorType
	 ������������ȡ�������
	 ���������
		@Point��				��ǰ�����ͼ���е������
	 ���������NA
	 �� �� ֵ��LONG
	*****************************************************************************/
	LONG GetCursorType(POINT Point);

	/*****************************************************************************
	 �������ƣ�SelectGraphicByGroup
	 ����������ѡ��ָ��ͼ���������ͼ��
	 ���������
		@ulGraphicGroup��	ͼ�����ʶ
	 ���������
		@bAllowDel��			�Ƿ�����ɾ�����
	 �� �� ֵ��LONG
	*****************************************************************************/
	LONG SelectGraphicByGroup(ULONG ulGraphicGroup, BOOL &bAllowDel);

	/*****************************************************************************
	 �������ƣ�StartModifyIAGraphics
	 �������������ܷ�����ʼ�޸�ͼ��
	 ���������
		@point��				�����ʼ���λ������
		@MouseEvent��		��������ͣ�LEFT_BUTTON_DOWN ��RIGHT_BUTTON_DOWN��
		@ulGraphicGroup��	ͼ�����ʶ
	 ���������NA
	 �� �� ֵ��LONG
	*****************************************************************************/
	LONG StartModifyIAGraphics(POINT point, MOUSE_EVENT_TYPE MouseEvent, ULONG ulGraphicGroup);

	/*****************************************************************************
	 �������ƣ�ModifyIAShapeUpDate
	 �������������ܷ���������ƶ��޸�ͼ����Ӧ������
	 ���������
		@point��				��굱ǰλ��
		@bFinish��			�Ƿ��޸Ľ���
		@ulGraphicGroup��	ͼ�����ʶ
	 ���������NA
	 �� �� ֵ��LONG
	*****************************************************************************/
	LONG ModifyIAShapeUpDate(POINT point, BOOL bFinish, ULONG ulGraphicGroup);

	/*****************************************************************************
	 �������ƣ�ModifyFinishedAndInsert
	 �������������ܷ����޸�ͼ�ν���
	 ���������
		@lRet��				�޸�ͼ���з�����
	 ���������NA
	 �� �� ֵ��LONG
	*****************************************************************************/
	LONG ModifyFinishedAndInsert(LONG lRet);

	/*****************************************************************************
	 �������ƣ�ModifyCancel
	 ����������ȡ��ѡ�����ܷ���ͼ��
	 ���������NA
	 ���������NA
	 �� �� ֵ��LONG
	*****************************************************************************/
	LONG SelectCancel();

	/*****************************************************************************
	 �������ƣ�InsertCurrentToGraphicList
	 ��������������ǰ��ͼ�β��뵽��ͼ�б���
	 ���������NA
	 ���������NA
	 �� �� ֵ��LONG
	*****************************************************************************/
	VOID InsertCurrentToGraphicList();

	/*****************************************************************************
	 �������ƣ�IsFinishDrawGraphicGroup
	 �����������ж��Ƿ�ﵽ��ͼ�������ͼ��
	 ���������
		@ulGraphicGroup��	ͼ�����ʶ
	 ���������NA
	 �� �� ֵ��BOOL
	*****************************************************************************/
	BOOL IsFinishDrawGraphicGroup(ULONG ulGraphicGroup, ULONG ulMaxDrawNum);

private:
	
	LONG GetCursorTypeInShape(POINT Point, const IA_GRAPHICS &IAGraphic);
	LONG GetCursorTypePoint(POINT /*Point*/, const IA_GRAPHICS &IAGraphic, ULONG ulIndex);
	LONG GetCursorTypePointInner(POINT Point, POINT PointPre, POINT PointNext);

	VOID GetArrowPoints(IA_GRAPHICS &iaGraphic);
	VOID ShowArrowShape(HDC hDC, const IA_ARROW &iaArrow, ULONG ulArrowType);
	/*****************************************************************************
	 �������ƣ�GetOppositeType
	 ������������ȡ�෴��ͼ�����ϵ
	 ���������
		 @ulRelationShipType��	ԭͼ�����ϵ
	 ���������NA
	 �� �� ֵ��RelationShipType
	 *****************************************************************************/
	RELATION_SHIP_TYPE GetOppositeType(RELATION_SHIP_TYPE ulRelationShipType);

	/*****************************************************************************
	 �������ƣ�InsertIntoRelationShipList
	 ������������λ�ù�ϵ���뵽λ�ù�ϵ�б���
	 ���������
		@ulGraphicGroupA��		ͼ����A��ʶ
		@ulGraphicGroupB��		ͼ����B��ʶ
		@ulRelationShipType��	ͼ����A�������B�Ĺ�ϵ
	 ���������NA
	 �� �� ֵ��LONG
	*****************************************************************************/
	LONG InsertIntoRelationShipList(ULONG ulGraphicGroupA, ULONG ulGraphicGroupB, RELATION_SHIP_TYPE ulRelationShipType);

	/*****************************************************************************
	 �������ƣ�FinishAndInsertIntoGraphicList
	 ��������������ͼ���������뵽��ͼ�б���
	 ���������
	 	@bIsDrawing��		�Ƿ��ǻ�ͼ״̬�����޸�״̬��
	 ���������NA
	 �� �� ֵ��LONG
	*****************************************************************************/
	LONG FinishAndInsertIntoGraphicList(BOOL bIsDrawing);

	/*****************************************************************************
	 �������ƣ�InsertCurrentToList
	 ��������������ǰͼ�β��뵽��ͼ�б���
	 ���������NA
	 ���������NA
	 �� �� ֵ��LONG
	*****************************************************************************/
	LONG InsertCurrentToList();

	/*****************************************************************************
	 �������ƣ�IsSetGraphicsInvalid
	 �����������ж����õ����ܷ���ͼ���Ƿ�Ϸ�
	 ���������
		@pGraphics��    ָ������ͼ�������ָ��
		@ulNum��        ����ͼ�θ���
	 ���������NA
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	BOOL IsSetGraphicsInvalid(const IA_GRAPHICS *pGraphics, ULONG ulNum);

	/*****************************************************************************
	 �������ƣ�AdjustRectPoint
	 ��������������bKeepRectShapeֵ�Զ��������������
	 ���������
		@point��    �����������ֵ
	 ���������
		@point��    �����������ֵ
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG AdjustRectPoint(POINT &point);

	/*****************************************************************************
	 �������ƣ�DrawIAPolygonShape
	 ���������������ܷ��������
	 ���������
		@point��				�µ�����
		@bNextPoint��		�Ƿ�����һ����
		@bFinish��			��ͼ�Ƿ����
		@ulGraphicGroup��	ͼ�����ʶ
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG DrawIAPolygonShape(POINT point, BOOL bNextPoint, BOOL bFinish, ULONG ulGraphicGroup);

	/*****************************************************************************
	 �������ƣ�IsDrawIAPolygonLegal
	 �����������ж��Ƿ񻭶���κϷ�
	 ���������
		@newPoint��			    �µ�����
		@ulCurrentPointsNum��	��ǰ����
		@ulGraphicGroup��		ͼ�����ʶ
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG IsDrawIAPolygonLegal(POINT newPoint, ULONG ulCurrentPointsNum, ULONG ulGraphicGroup);

	/*****************************************************************************
	 �������ƣ�IsPolygonFinishedLegal
	 �����������ж϶���λ���������Ƿ�Ϸ�
	 ���������
		@ulGraphicGroup��	ͼ�����ʶ
		@bReachMaxPoint:	�Ƿ�ﵽ������
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG IsPolygonFinishedLegal(ULONG ulGraphicGroup, BOOL bReachMaxPoint);

	/*****************************************************************************
	 �������ƣ�IsIAPolygonSelfLegal
	 �����������ж϶���������Ƿ�Ϸ�
	 ���������
		@pPoint��		����ζ�������
		@ulPointNum��	����ζ�����
	 ���������NA
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	BOOL IsIAPolygonSelfLegal(const POINT *pPoint, ULONG ulPointsNum);

	/*****************************************************************************
	 �������ƣ�DrawIAULineShape
	 ���������������ܷ���U����
	 ���������
		@point��				�յ�����
		@bNextPoint��		�Ƿ�����һ����
		@bIsDrawing��		�Ƿ��ǻ�ͼ״̬
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG DrawIAULineShape(POINT point, BOOL bNextPoint, BOOL bIsDrawing);

	/*****************************************************************************
	 �������ƣ�DrawIAUFirstLine
	 ���������������ܷ���U���ߵ�һ����
	 ���������
		@point��				�յ�����
		@bNextPoint��		�Ƿ�����һ����
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG DrawIAUFirstLine(POINT point, BOOL bNextPoint);

	/*****************************************************************************
	 �������ƣ�DrawIAULineShape
	 ���������������ܷ���U���ߵڶ�����
	 ���������
		@point��				�յ�����
		@bNextPoint��		�Ƿ�����һ����
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG DrawIAUSecondLine(POINT point, BOOL bNextPoint);
	
	/*****************************************************************************
	 �������ƣ�DrawIAUThirdLine
	 ���������������ܷ���U���ߵ�������
	 ���������
		@point��				�յ�����
		@bNextPoint��		�Ƿ�����һ����
		@bIsDrawing��		�Ƿ��ǻ�ͼ״̬
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG DrawIAUThirdLine(POINT point,BOOL bNextPoint, BOOL bIsDrawing);

	/*****************************************************************************
	 �������ƣ�UpdateWnd
	 �������������»�ͼ����
	 ���������
		@hDC��			��ͼ���
		@rc��			ͼ��Ĵ�С��Ϣ
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG UpdateWnd(HDC hDC, const RECT *rc); 

	void GetOffset();

	/*****************************************************************************
	 �������ƣ�IsShowIAShapeInvalid
	 �����������ж���Ҫ��ʾ�ĵ���ͼ���Ƿ�Ϸ�
	 ���������
		@pGraphics��		ͼ������
	 ���������NA
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	BOOL IsShowIAShapeInvalid(const IA_INNER_GRAPHICS &pIAGraphic);


	/*****************************************************************************
	 �������ƣ�ShowIAShape
	 ������������ʾ���ܷ�������ͼ��
	 ���������
		@hDC��			��ͼ���
		@pGraphics��		ͼ������
	 ���������NA
	 �� �� ֵ��VOID
	 *****************************************************************************/
	VOID ShowIAShape(HDC hDC, IA_INNER_GRAPHICS &pGraphics);

	/*****************************************************************************
	 �������ƣ�ShowPolyline
	 ������������ʾ���ܷ��������
	 ���������
		@hDC��			��ͼ���
		@points��		����θ�������������
		@numberPoints��	����ζ������
		@color��			��ǰ��ɫ
	 ���������NA
	 �� �� ֵ��VOID
	 *****************************************************************************/
	VOID ShowPolyline(HDC hDC, const POINT *points, ULONG numberPoints, COLORREF /*color*/);

	/*****************************************************************************
	 �������ƣ�ShowGraphicChoosen
	 ������������ʾѡ�е����ܷ���ͼ��
	 ���������
		@hDC��			��ͼ���
		@pIAGraphics��	ѡ�е�ͼ��
	 ���������NA
	 �� �� ֵ��VOID
	 *****************************************************************************/
	VOID ShowGraphicChoosen(HDC hDC, const IA_INNER_GRAPHICS &pIAGraphics);

	/*****************************************************************************
	 �������ƣ�ShowGraphicName
	 ������������ʾͼ������
	 ���������
		@hDC��			��ͼ���
		@pIAGraphics��	ѡ�е�ͼ��
	 ���������NA
	 �� �� ֵ��VOID
	 *****************************************************************************/
	VOID ShowGraphicName(HDC hDC, const IA_INNER_GRAPHICS &pIAGraphics);

	/*****************************************************************************
	 �������ƣ�ShowAlphaBlendArea
	 ������������ʾͼ���ڰ�͸���������
	 ���������
		@hDC��			��ͼ���
		@pIAGraphics��	ѡ�е�ͼ��
	 ���������NA
	 �� �� ֵ��VOID
	 *****************************************************************************/
	VOID ShowAlphaBlendArea(HDC hDC, const IA_INNER_GRAPHICS &pGraphics);

	/*****************************************************************************
	 �������ƣ�CreatDCAndBitmap
	 �������������������ڴ�DC����ԭͼ����ͬ��С��λͼ
	 ���������
		@hDC��			��ͼ���
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG CreatBitmapDC(HDC hDC);	

	LONG CreateIAFont();


	/*****************************************************************************
	 �������ƣ�IsGraphicToDelete
	 �����������жϸ��ݸõ��Ƿ����ɾ����ͼ��
	 ���������
		@pIAGraphics��		ָ��ͼ�ε�ָ��
		@point��				��ǰ��������
	 ���������NA
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	BOOL IsGraphicToDelete(const IA_INNER_GRAPHICS *pIAGraphics, POINT point);

	/*****************************************************************************
	 �������ƣ�StartModifyIAShapes
	 �������������ܷ�����ʼ�޸�ͼ��
	 ���������
		@point��				�����ʼ���λ������
		@MouseEvent��		��������ͣ�LEFT_BUTTON_DOWN ��RIGHT_BUTTON_DOWN��
		@ulGraphicGroup��	ͼ�����ʶ
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG StartModifyIAShapes(POINT point, MOUSE_EVENT_TYPE MouseEvent, ULONG ulGraphicGroup);

	/*****************************************************************************
	 �������ƣ�ModifyIAStraightLine
	 �������������ܷ�����ʼ�޸�ֱ��
	 ���������
		@point��				�����ʼ���λ������
		@MouseEvent��		��������ͣ�LEFT_BUTTON_DOWN ��RIGHT_BUTTON_DOWN��
		@ulGraphicGroup��	ͼ�����ʶ
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG ModifyIAStraightLine(POINT point, MOUSE_EVENT_TYPE MouseEvent, ULONG ulGraphicGroup);

	/*****************************************************************************
	 �������ƣ�ModifyIARectangle
	 �������������ܷ�����ʼ�޸ľ���
	 ���������
		@point��				�����ʼ���λ������
		@pPointNo��			���ӽڵ�ʱָ���½ڵ������ָ��
		@MouseEvent��		��������ͣ�LEFT_BUTTON_DOWN ��RIGHT_BUTTON_DOWN��
		@ulGraphicGroup��	ͼ�����ʶ
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG ModifyIARectangle(POINT point, LONG *pPointNo, MOUSE_EVENT_TYPE MouseEvent, ULONG ulGraphicGroup);

	/*****************************************************************************
	 �������ƣ�ModifyIARectangleLine
	 �������������ܷ�����ʼ�޸ľ��Σ��޸ıߣ��϶����ߡ��ڱ������ӽڵ㣩
	 ���������
		@pPointNo��			���ӽڵ�ʱָ���½ڵ������ָ��
		@MouseEvent��		��������ͣ�LEFT_BUTTON_DOWN ��RIGHT_BUTTON_DOWN��
		@ulPointIndex��		�޸ĵıߵ�ǰһ���������������±�
		@point��				��굱ǰ����
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG ModifyIARectangleLine(LONG *pPointNo, MOUSE_EVENT_TYPE MouseEvent, ULONG ulPointIndex, POINT point);

	/*****************************************************************************
	 �������ƣ�ModifyIARectanglePoint
	 �������������ܷ�����ʼ�޸ľ��Σ��޸ĵ㣺����϶����������ž��Ρ��Ҽ���������������϶���
	 ���������
		@MouseEvent��		��������ͣ�LEFT_BUTTON_DOWN ��RIGHT_BUTTON_DOWN��
		@ulPointIndex��		�޸ĵĶ������������±�
		@point��				��굱ǰ����
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG ModifyIARectanglePoint(MOUSE_EVENT_TYPE MouseEvent, ULONG ulPointIndex, POINT /*point*/);

	/*****************************************************************************
	 �������ƣ�ModifyIAPolygon
	 �������������ܷ�����ʼ�޸Ķ����
	 ���������
		 @point��			�����ʼ���λ������
		 @pPointNo��			ָ���½ڵ������ָ��
		 @MouseEvent��		��������ͣ�LEFT_BUTTON_DOWN ��RIGHT_BUTTON_DOWN��
		 @ulGraphicGroup��	ͼ�����ʶ
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG ModifyIAPolygon(POINT point, LONG *pPointNo, MOUSE_EVENT_TYPE MouseEvent, ULONG ulGraphicGroup);

	/*****************************************************************************
	 �������ƣ�ModifyIAPolygonPoint
	 �������������ܷ�����ʼ�޸Ķ���Σ��޸ĵ㣺����޸ĵ�����λ�á��Ҽ�ɾ���õ㣩
	 ���������
		@pPointNo��			ָ���½ڵ������ָ��
		@MouseEvent��		��������ͣ�LEFT_BUTTON_DOWN ��RIGHT_BUTTON_DOWN��
		@ulGraphicGroup��	ͼ�����ʶ
		@ulPointIndex��		�޸ĵĶ������������±�
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG ModifyIAPolygonPoint(LONG *pPointNo, MOUSE_EVENT_TYPE MouseEvent, ULONG ulGraphicGroup, ULONG ulPointIndex);
	
	/*****************************************************************************
	 �������ƣ�ModifyIAPolygonLine
	 �������������ܷ�����ʼ�޸Ķ���Σ��޸ıߣ���������϶����Ҽ������½ڵ㣩
	 ���������
		@pPointNo��			���ӽڵ�ʱָ���½ڵ������ָ��
		@MouseEvent��		��������ͣ�LEFT_BUTTON_DOWN ��RIGHT_BUTTON_DOWN��
		@point��				��굱ǰ����
		@ulPointIndex��		�޸ĵĶ������������±�
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG ModifyIAPolygonLine(LONG *pPointNo, MOUSE_EVENT_TYPE MouseEvent, POINT point, ULONG ulPointIndex);

	/*****************************************************************************
	 �������ƣ�ModifyIAULine
	 �������������ܷ�����ʼ�޸�U����
	 ���������
		 @point��			�����ʼ���λ������
		 @MouseEvent��		��������ͣ�LEFT_BUTTON_DOWN ��RIGHT_BUTTON_DOWN��
		 @ulGraphicGroup��	ͼ�����ʶ
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG ModifyIAULine(POINT point, MOUSE_EVENT_TYPE MouseEvent, ULONG ulGraphicGroup);

	/*****************************************************************************
	 �������ƣ�UpdateIAPolygon
	 �����������޸��и��¶����ͼ��
	 ���������
		 @point��			�����ʼ���λ������
		 @bFinish��			�޸��Ƿ����
		 @ulGraphicGroup��	ͼ�����ʶ
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG UpdateIAPolygon(POINT point, BOOL bFinish, ULONG ulGraphicGroup);

	/*****************************************************************************
	 �������ƣ�UpdateIAMove
	 ���������������϶�ͼ��ʱ����ͼ��
	 ���������
		 @point��			��굱ǰ����
		 @ulGraphicGroup��	ͼ�����ʶ
		 @bFinish:			�Ƿ�༭����
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG UpdateIAMove(POINT point, ULONG ulGraphicGroup, BOOL bFinish);

	/*****************************************************************************
	 �������ƣ�UpdateIAPolygonPoint
	 �����������϶�����ε����ڵ�ʱ����ͼ��
	 ���������
		 @point��			�����ʼ���λ������
		 @ulGraphicGroup��	ͼ�����ʶ
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG UpdateIAPolygonPoint(POINT point, ULONG ulGraphicGroup);	
	
	/*****************************************************************************
	 �������ƣ�UpdateIAULine
	 �����������϶�U���ߵ����ڵ�ʱ����ͼ��
	 ���������
		 @point��			�����ʼ���λ������
		 @bFinish��			�޸��Ƿ����
	 ���������NA
	 �� �� ֵ��VOID
	 *****************************************************************************/
	LONG UpdateIAULine(POINT point, BOOL bFinish);

	/*****************************************************************************
	 �������ƣ�IsOnLine
	 �����������жϵ��Ƿ�������
	 ���������
		@pPointA��		�߶��������
		@pPointB��		�߶��յ�����
		@point��			���жϵĵ�����
	 ���������NA
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	BOOL IsOnLine(const POINT *pPointA, const POINT *pPointB, POINT point);

	/*****************************************************************************
	 �������ƣ�IsPointOnLine
	 �����������жϵ��Ƿ������ϣ��ڲ���
	 ���������
		 @pPointA��		�߶��������
		 @pPointB��		�߶��յ�����
		 @point��		���жϵĵ�����
	 ���������NA
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	BOOL IsPointOnLine(const POINT *pPointA, const POINT *pPointB, POINT point);

	/*****************************************************************************
	 �������ƣ�IsInShape
	 �����������жϵ��Ƿ���ͼ����
	 ���������
		@pIAGraphic��	ָ��ͼ�ε�ָ��
		@point��			���жϵĵ�����
	 ���������NA
	 �� �� ֵ��LONG
	 *****************************************************************************/
	LONG IsInShape(const IA_INNER_GRAPHICS *pIAGraphic, POINT point);

	/*****************************************************************************
	 �������ƣ�IsPointInPolygon
	 �����������жϵ��Ƿ��ڶ������
	 ���������
		@pPoint��		����ζ�������
		@ulPointNum��	����ζ�����
		@point��			���жϵĵ�����
	 ���������NA
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	BOOL IsPointInPolygon(const POINT *pPoint, ULONG ulPointNum, POINT point );

	/*****************************************************************************
	 �������ƣ�IsPointInRectangle
	 �����������жϵ��Ƿ��ھ�����
	 ���������
		@pPoint��		����ζ�������
		@ulPointNum��	����ζ�����
		@point��			���жϵĵ�����
	 ���������NA
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	BOOL IsPointInRectangle(const POINT *pPoint, ULONG /*ulPointNum*/, POINT point);

	/*****************************************************************************
	 �������ƣ�IsLineSegmentCross
	 �����������ж������߶��Ƿ��ཻ(�н���)
	 ���������
		@pFirst1��		��1����ʼ����
		@pFirst2��		��2����ʼ����
		@pSecond1��		��1���յ�����
		@pSecond2��		��2���յ�����
	 ���������NA
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	BOOL IsLineSegmentCross(POINT pFirst1, POINT pFirst2, POINT pSecond1, POINT pSecond2);

	/*****************************************************************************
	 �������ƣ�DistanceToLine
	 ��������������㵽ֱ�������һ��ľ��� X*X + Y*Y
	 ���������
		@Point��				������
		@PointStart��		�ߵ���ʼ����
		@PointEnd��			�ߵ��յ�����
	 ���������NA
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	LONG DistanceToLine(POINT Point, POINT PointStart, POINT PointEnd);

	/*****************************************************************************
	 �������ƣ�IsCloseToPoint
	 �����������жϵ�A�Ƿ����B�ӽ�
	 ���������
		@pointA��		��A����
		@pointB��		��B����
	 ���������NA
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	BOOL IsCloseToPoint(POINT pointA, POINT pointB);

	/*****************************************************************************
	 �������ƣ�IsPointInside
	 �����������жϵ��Ƿ���ͼ�����
	 ���������
		@point��			������
	 ���������NA
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	BOOL IsPointInside(const POINT *point);

	/*****************************************************************************
	 �������ƣ�IsInForbiddenArea
	 �����������жϵ��Ƿ��ڷǷ�����Ѱ����Ӧ��RelativeGraphicGroup�������RelationshipType
	 ���������
		@point��				������
		@ulGraphicGroup��	��������ͼ�����ʶ
	 ���������NA
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	BOOL IsInForbiddenArea(POINT point, ULONG ulGraphicGroup);

	/*****************************************************************************
	 �������ƣ�isPointInForbiddenArea
	 �����������жϵ��Ƿ��ڷǷ������ڲ������жϵ��Ƿ���RelativeGraphicGroupͼ����
	 ���������
		@point��				������
		@ulGraphicGroup��	��������ͼ�����ʶ
		@bIsIn��				ͼ�����ϵ�Ƿ�Ϊ�����ı�־
	 ���������NA
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	BOOL IsPointInForbiddenArea(POINT point, ULONG ulGraphicGroup, BOOL bIsIn);

	/*****************************************************************************
	 �������ƣ�IsCrossForbiddenArea
	 �����������ж��߶��Ƿ���Ƿ������ཻ��Ѱ����Ӧ��RelativeGraphicGroup�������RelationshipType
	 ���������
		@newPoint��			�߶�ͷ�ڵ�����
		@lastPoint��			�߶�β�ڵ�����
		@ulGraphicGroup��	�߶�������ͼ�����ʶ
	 ���������NA
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	BOOL IsCrossForbiddenArea(POINT newPoint, POINT lastPoint, ULONG ulGraphicGroup);	
	
	/*****************************************************************************
	 �������ƣ�IsLineCrossForbiddenArea
	 �����������ж��߶��Ƿ���Ƿ������ཻ���ڲ������ж��µ�һ�����Ƿ���RelativeGraphicGroupͼ���ཻ
	 ���������
		@newPoint��			�߶�ͷ�ڵ�����
		@lastPoint��			�߶�β�ڵ�����
		@ulGraphicGroup��	���߶�������ͼ�����ʶ
	 ���������NA
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	BOOL IsLineCrossForbiddenArea(POINT newPoint, POINT lastPoint, ULONG ulGraphicGroup);

	/*****************************************************************************
	 �������ƣ�IsForbiddenOverlaped
	 �����������ж��ض���ͼ���Ƿ�����������ͼ��λ�ù�ϵ��Ѱ����Ӧ��RelativeGraphicGroup�������RelationshipType
	 ���������
		@pPoint��			��ǰͼ�θ����ڵ����������
		@ulPointNum��		��ǰͼ�νڵ����
		@ulGraphicGroup��	��ǰͼ�ε�ͼ�����ʶ
	 ���������NA
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	BOOL IsForbiddenOverlaped(const POINT *pPoint, ULONG ulPointNum, ULONG ulGraphicGroup, ULONG ulGraphicType);

	/*****************************************************************************
	 �������ƣ�IsAreaForbiddenOverlaped
	 �����������ж��ض���ͼ���Ƿ�����������ͼ��λ�ù�ϵ���ڲ���������IADrawGraphicsList���ҵ�RelativeGraphicGroup��ͼ��
	 ���������
		@pPoint��				��ǰͼ�θ����ڵ����������
		@ulPointNum��			��ǰͼ�νڵ����
		@ulGraphicGroup��		��ǰͼ�ε�ͼ�����ʶ
		@ulGraphicType��			��ǰͼ�ε�����
		@ulRelationShipType��	ͼ�����ϵ
	 ���������NA
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	BOOL IsAreaForbiddenOverlaped(const POINT *point, ULONG ulPointNum, ULONG ulGraphicGroup, ULONG ulGraphicType, RELATION_SHIP_TYPE ulRelationShipType);

	/*****************************************************************************
	 �������ƣ�IsShapeForbiddenOverlaped
	 �����������ж��ض���ͼ���Ƿ�����������ͼ��λ�ù�ϵ���ڲ������жϸ�ͼ���Ƿ���RelativeGraphicGroupͼ�ηǷ��ص�
	 ���������
		@pIAGraphics��			ָ��λ�����ͼ�ε�ָ��
		@pPoint��				��ǰͼ�θ����ڵ����������
		@ulPointNum��			��ǰͼ�νڵ����
		@ulGraphicType��			��ǰͼ�ε�����
		@ulRelationShipType��	ͼ�����ϵ
	 ���������NA
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	BOOL IsShapeForbiddenOverlaped(const IA_INNER_GRAPHICS *pIAGraphic, const POINT *pPoint, ULONG ulPointNum, ULONG ulGraphicType, RELATION_SHIP_TYPE ulRelationShipType); 
	
	/*****************************************************************************
	 �������ƣ�IsRectangleSizeIllegal
	 �����������жϾ��γ����Ƿ�Ϸ���Ѱ����Ӧ��RelativeGraphicGroup�������RelationshipType
	 ���������
		@pPoint��			ͼ�ζ�������
		@ulPointNum:		�������
		@ulGraphicGroup��	ͼ�����ʶ
	 ���������NA
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	BOOL IsRectangleSizeIllegal(const POINT *pPoint, ULONG ulPointNum, ULONG ulGraphicGroup);
		
	/*****************************************************************************
	 �������ƣ�IsRectangleEdgeIllegal
	 �����������жϾ��γ����Ƿ�Ϸ����ڲ���������IADrawGraphicsList���жϸ�ͼ���Ƿ���RelativeGraphicGroupͼ�δ�С��ϵ��ȷ
	 ���������
		@pPoint��				ͼ�ζ�������
		@ulPointNum:			�������
		@ulGraphicGroup��		ͼ�����ʶ
		@ulRelationShipType��	λ�ù�ϵ����
	 ���������NA
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	BOOL IsRectangleEdgeIllegal(const POINT *pPoint, ULONG ulPointNum, ULONG ulGraphicGroup, RELATION_SHIP_TYPE ulRelationShipType);

	/*****************************************************************************
	 �������ƣ�FindEdgeRectangle
	 ����������Ѱ�Ұ����ض�ͼ�ε���С���ο�
	 ���������
		@pPoint��			ͼ�ζ�������
		@ulPointNum:		�������
	 ���������
		@rc��				����С���ο�ĸ�����Ϣ
	 �� �� ֵ��BOOL
	 *****************************************************************************/
	BOOL FindEdgeRectangle(const POINT *pPoint, ULONG ulPointNum, RECT &rc);


private:

	ULONG					m_ulGraphicID;				// �ڲ�ͼ��ID
	ULONG					m_ulMaxDrawNum;				// ���ܷ������ͼ����
	ULONG					m_ulCurrentState;			// ��ǰ�޸ĵ�״̬
	ULONG					m_ulCurrentCursor;			// ��ǰ���״̬
	ULONG					m_ulCurrentGraphicType;		// ��ǰ���༭ͼ�ε�����
	ULONG					m_ulMaxGroupShapeNum;		// ��ǰͼ�������ͼ�θ���
	HWND					m_hMsgWnd;					// ���ھ��
	ULONG					m_ulWndWidth;				// ���ڿ��
	ULONG					m_ulWndHeight;				// ���ڸ߶�
	ULONG					m_ulDCWidth;				// ��ʾͼ����
	ULONG					m_ulDCHeight;				// ��ʾͼ��߶�
// 	ULONG					m_ulScreenWidth;			// ��Ļ���
// 	ULONG					m_ulScreenHeight;			// ��Ļ�߶�

	FLOAT					m_fDiv;						// DC/Wnd ���
	LONG					m_lLineWidthAdjust;			// ͼ���߿�ϵ�������ڷֱ��ʱ仯��
	LONG					m_lDistanceCloseToLine;
	LONG					m_lArrowLenth;
	LONG					m_lFontSize;

	BOOL					m_bScaleModeFull;			// �Ƿ���������ʾͼ��
	LONG					m_lOffsetX;					// ͼ����ʾ�ڴ�����X������ƫ��
	LONG					m_lOffsetY;					// ͼ����ʾ�ڴ�����Y������ƫ��

	IA_INNER_GRAPHICS		m_IAGraphic;				// ���ڻ������ܷ���ͼ�νṹ��
	BOOL					m_bActive;					// ��ǰͼ���Ƿ���Ч
	BOOL					m_bWindowPoint;				// ��ǰͼ����������Ƿ�Ϊ��������
	BOOL					m_bReversePoint;			// �Ƿ�ֱ�߱༭���������귴ת

	CIVSMutex				m_Mutex;					// ���ܷ���ͼ���б���

	LONG					m_lPointNo;					// ��¼ͼ�����޸ĵĶ����±�
	POINT					m_RecordPoint;				// ��¼�����ƶ�ͼ��ʱԭ������㣬���ڼ���λ����
	POINT					m_OriginIAPoints[POINTS_NUM_MAX];	//��¼�����ƶ�ͼ��ʱԭͼ�������
	IA_GRAPHICS_LIST		m_IADrawGraphicsList;		// ���ܷ�����ͼͼ���б�
	IA_RELATIONS_LIST		m_IARelationShipList;		// ���ܷ���ͼ�����ϵ�б�

	HDC						m_memhdc;					// �����ڴ�DC��� 
	BLENDFUNCTION			m_bf;						// AlphaBlending�еĽṹ�����
	HBITMAP					m_hbitmap;					// λͼ���
	BITMAPINFO				m_bmi;						// λͼͷ����Ϣ
	VOID					*m_pvBits;					// ָ��DIB section��ָ��
	LOGFONT					m_lf;						// ������������ṹ��
	HFONT					m_hFont;					// ������

    ULONG         m_ulUlineModifyIndex;    // U���ߵ�ǰ���޸ĵĶ���(ֻ����1��2��)��� DTS2013051709238

};

#endif //__CIADRAWER_H__



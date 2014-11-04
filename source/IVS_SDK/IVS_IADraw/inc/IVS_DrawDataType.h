/********************************************************************
filename 	: IVS_DrawDataType.h
author   	: z00227377
created 	: 2012/12/28
description : ��ͼ�����������ṹ��
copyright 	: Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history 	: 2012/12/28 ��ʼ�汾
*********************************************************************/
#ifndef __IVS_DRAW_DATATYPE_H__
#define __IVS_DRAW_DATATYPE_H__


#include <Windows.h>

#pragma pack(push, 1)


#define GRAPHICS_STATE_NONE		0	
#define GRAPHICS_STATE_DRAWING	1
#define GRAPHICS_STATE_EDITING	2


#define POINTS_NUM_MAX      100		// ����ͼ��������
#define CHARACTER_NUM_MAX   99		// ͼ������������ַ���
#define GRAPHICS_NUM_MAX	60		// ���ͼ��
#define FLOAT_TO_LONG_COEFF 1000000 // ������ת�ɳ�����ϵ��

#define ARROW_LENTH						40

// ���ܷ��������ʾ����
enum CURSOR_DISPLAY_TYPE{
	CURSOR_TYPE_ARROW = 0,	// ��׼��ͷ
	CURSOR_TYPE_CROSS,		// ʮ����׼��(��ʼ��ͼ)
	CURSOR_TYPE_SIZEALL,	// Four-pointed arrow pointing north, south, east, and west
	CURSOR_TYPE_SIZENESW,	// Double-pointed arrow pointing northeast and southwest 
	CURSOR_TYPE_SIZENS,		// Double-pointed arrow pointing north and south 
	CURSOR_TYPE_SIZENWSE,	// Double-pointed arrow pointing northwest and southeast 
	CURSOR_TYPE_SIZEWE,		// Double-pointed arrow pointing west and east 
};


// ���ܷ�������ͼ�λ�������
enum  GRAPHIC_TYPE {
   IA_GRAPHICS_TYPE_ILLEGAL       = 0,	//�Ƿ�����
   IA_GRAPHICS_TYPE_RECTANGLE     = 1,	//����
   IA_GRAPHICS_TYPE_POLYGON       = 2,	//�����
   IA_GRAPHICS_TYPE_STRAIGHT_LINE = 3,	//ֱ��
   IA_GRAPHICS_TYPE_UP_ULINE      = 4,	//U����
   IA_GRAPHICS_TYPE_DOWN_ULINE    = 5,	//N����

   // add new types here
   IA_GRAPHICS_TYPE_COUNT         = 6,
};

enum  ARROW_TYPE {
	ARROW_TYPE_LINE_TO_LEFT  = 1,	// ָ�����
	ARROW_TYPE_LINE_TO_RIGHT = 2,	// ָ���ұ�
	ARROW_TYPE_LINE_TO_BOTH  = 3,	// ָ��˫��

	// add new types here
	ARROW_TYPE_OTHER		 = 4,
};

#if 0
#define IA_GRAPHICS_TYPE_ILLEGAL 0		 //�Ƿ�����
#define IA_GRAPHICS_TYPE_RECTANGLE 1     //����
#define IA_GRAPHICS_TYPE_POLYGON 2       //�����
#define IA_GRAPHICS_TYPE_STRAIGHT_LINE 3 //ֱ��
#define IA_GRAPHICS_TYPE_UP_ULINE 4      //U����
#define IA_GRAPHICS_TYPE_DOWN_ULINE 5    //N����
#endif


// ͼƬ������ʾλ��
enum  GRAPHIC_NAME_POSITION {
	GRAPHIC_NAME_ABOVE     = 0,	//ͼ���Ϸ�
	GRAPHIC_NAME_UNDER     = 1,	//ͼ���·�
	GRAPHIC_NAME_INSIDE    = 2,	//ͼ���ڲ��·�

	// add new types here
	GRAPHIC_NAME_OTHER     = 3,
};



// ͼ�����λ�ù�ϵ
typedef enum 
{
	IA_GRAPHICS_RELATION_TYPE_NONE,			// ��λ�ù�ϵ
	IA_GRAPHICS_RELATION_TYPE_BE_COVERED,	// ������
	IA_GRAPHICS_RELATION_TYPE_COVER,		// ����
	IA_GRAPHICS_RELATION_TYPE_NOT_CROSS,	// ����������ཻ
	IA_GRAPHICS_RELATION_TYPE_SMALLER,		// �����Σ�����С��
	IA_GRAPHICS_RELATION_TYPE_BIGGER		// �����Σ���������
}RELATION_SHIP_TYPE;

// ����¼�����
typedef enum 
{
	LEFT_BUTTON_DOWN = 0,	//������
	RIGHT_BUTTON_DOWN		//�Ҽ����
} MOUSE_EVENT_TYPE;

// ��ɫ�ṹ��
typedef struct
{
	UCHAR	Alpha;		// ͸���ȣ�0~255��
	UCHAR	Red;		// Rֵ��0~255��
	UCHAR	Green;		// Gֵ��0~255��
	UCHAR	Blue;		// Bֵ��0~255��
}IA_COLOR;

typedef struct
{
	POINT ArrowHead[2];
	POINT ArrowTail[2];
	POINT ArrowTrangle1[2];
	POINT ArrowTrangle2[2];
}IA_ARROW;

// ����ͼ�νṹ��
typedef struct
{
	ULONG		ulGraphicGroup;					// ͼ�����ʶ
	IA_COLOR	BgColor;						// ͼ�α���ɫ
	IA_COLOR	LineColor;						// ͼ�α���ɫ
	ULONG		ulLineWidth;					// ���߿��
	ULONG		ulGraphicType;					// ����ͼ������(���Ρ�����Ρ�ֱ�ߣ�U��)
	ULONG		ulPointsNum;					// ͼ�ε���
	POINT		IAPoints[POINTS_NUM_MAX];		// ������
	CHAR		GraphicGroupName[CHARACTER_NUM_MAX];// ͼ�������� ����󳤶�99�ֽڣ�
	ULONG 		NamePosition;					// ͼ����������ʾ��λ��0: ͼ���Ϸ���1: ͼ���·���2:�ڲ��·���
	BOOL		bAllowDelete;					// �Ƿ���������Ҽ�ɾ��
	BOOL		bAllowAddNewPoint;				// �Ƿ����������¶���
	BOOL		bAllowModifySize;				// �Ƿ������޸ľ��δ�С
	BOOL		bKeepRectShape;					// �Ƿ���ԭͼ���߱�һ��
	BOOL		bKeepHorizon;					// �Ƿ�ֱ�߱���ˮƽ
	ULONG		ulArrowType;
	IA_ARROW    IAArrow;
}IA_GRAPHICS;

#pragma pack(pop)

#endif // __IVS_DRAW_DATATYPE_H__

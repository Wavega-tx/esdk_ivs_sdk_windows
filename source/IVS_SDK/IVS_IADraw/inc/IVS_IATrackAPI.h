/********************************************************************
filename 	: IVS_IATrackAPI.h
author   	: z00227377
created 	: 2012/12/19
description : ���ܷ����켣��ʾ���ܣ�ʵ�ֹ켣����ɾ����ʾ
copyright 	: Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history 	: 2012/12/19 ��ʼ�汾
*********************************************************************/


#ifndef __IATRACK_API_H__
#define __IATRACK_API_H__

#include <Windows.h>

#pragma pack(push, 1)

#define IA_TEXT_LENGTH_MAX		100		// �ı���󳤶�
#define IA_POINTS_NUM_MAX		100		// ����ͼ��������
#define IA_ELEMENT_NUM_MAX		60		// �켣��Ϣ�����
#define IA_MESSAGE_NUM_MAX		600		// �켣��Ϣ�б���󳤶�

// �켣��ϢLayer������
typedef enum 
{
	IA_LAYER_TYPE_NONE,
	IA_LAYER_TYPE_ALARM,	// �켣��ϢΪAlarmed
	IA_LAYER_TYPE_ROI		// �켣��ϢΪROI

}IA_LAYER_TYPE;

// �����켣����
typedef enum 
{
	IA_ELEMENT_TYPE_NONE,
	IA_ELEMENT_TYPE_TEXT,		// �켣����Ϊ�ı�
	IA_ELEMENT_TYPE_POLYLINE	// �켣����Ϊ����ͼ

}IA_ELEMENT_TYPE;


// ���ܷ����켣������Ϣ
typedef struct
{
	ULONG	ulTextScale;					// �ı����ű���
	POINT	TextPoint;						// �ı���ʾλ��
	ULONG	ulTextLenth;					// �ı�����
	CHAR	TextContent[IA_TEXT_LENGTH_MAX];// �ı�����
	ULONG   ulIsPercent;					// �Ƿ�ٷֱ�����

}IA_ALARM_TEXT;

// ���ܷ����켣ͼ����Ϣ
typedef struct
{
	ULONG	ulPointsNum;				// �켣ͼ�ε���
	POINT	Points[IA_POINTS_NUM_MAX];	// �켣������
	ULONG   ulIsPercent;				// �Ƿ�ٷֱ�����
    ULONG   ulDirection;                // ����0����          1��������       2��������           3��˫�� 
}IA_ALARM_POLYLINE;

// ���ܷ��������켣��Ϣ
typedef struct  
{
	IA_LAYER_TYPE		ulLayer;			// �����켣Layer��Ϣ
	COLORREF			ulColor;			// �켣��Ϣ��ɫ
	IA_ELEMENT_TYPE		ulElementType;		// �켣��Ϣ����
	IA_ALARM_TEXT		AlarmText;			// �켣����Ҫ��ʾ��������Ϣ	
	IA_ALARM_POLYLINE	AlarmPolyline;		// �켣����Ҫ��ʾ��ͼ��

}IA_LAYOUT_ELEMENT;

// ���ܷ����켣��Ϣ
typedef struct  
{
	ULONG64			ulFrameID;					// ʱ���
	ULONG			ulFrameWidth;				// �ֱ���(��)
	ULONG			ulFrameHeight;				// �ֱ���(��)
	ULONG			ulLayoutElementNum;			// �켣��Ϣ��
	IA_LAYOUT_ELEMENT LayoutElement[IA_ELEMENT_NUM_MAX];// �澯�켣����

}IA_LAYOUT_MESSAGE;

#pragma pack(pop)

//����API�ӿ�ǰ׺
#define IVS_IATRACK_API extern "C" __declspec(dllexport)

//#define IVS_IATRACK_API extern "C" __declspec(dllimport)
 
//IATrack���
typedef void * IATRACK_HANDLE;



/*****************************************************************************
 �������ƣ�IATrack_LibInit
 ������������ʼ�����ܷ�����
 ���������NA  
 ���������NA
 �� �� ֵ��INT
 *****************************************************************************/
//IVS_IATRACK_API INT IATrack_LibInit();


/*****************************************************************************
 �������ƣ�IATrack_LibClean
 �����������������ܷ�����
 ���������NA  
 ���������NA
 �� �� ֵ��VOID
 *****************************************************************************/
// IVS_IATRACK_API VOID IATrack_LibClean();


/*****************************************************************************
 �������ƣ�IATrack_GetHandle
 ������������ȡIATrack���
 ���������
  	@handle��       IATrack���
 ���������NA
 �� �� ֵ��IATrack_HANDLE
*****************************************************************************/
IVS_IATRACK_API IATRACK_HANDLE IATrack_GetHandle();


/*****************************************************************************
 �������ƣ�IATrack_InitIATrack
 ������������ʼ�����ܷ����켣��Ϣ
 ���������
  	@handle��       IATrack���
	@hMsgWnd��      ���ھ��
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_IATRACK_API LONG IATrack_Init(IATRACK_HANDLE handle, HWND hMsgWnd, ULONG ulVideoType);


/*****************************************************************************
 �������ƣ�IATrack_InsertIALayoutMessage
 ��������������켣��Ϣ
 ���������
  	@handle��			IATrack���
	@LayoutMessage��		�켣��Ϣ
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_IATRACK_API LONG IATrack_InsertIALayoutMessage(IATRACK_HANDLE handle, const IA_LAYOUT_MESSAGE *LayoutMessage);


/*****************************************************************************
 �������ƣ�IATrack_ShowIATrack
 ������������ʾ�켣��Ϣ
 ���������
  	@handle��			IATrack���
	@hDC��				��ʾDC
	@rc��				DC��С
	@ulFrameID��			��ǰʱ���
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_IATRACK_API LONG IATrack_ShowIATrack(IATRACK_HANDLE handle, HDC hDC, const RECT *rc, ULONG64 ulFrameID);


/*****************************************************************************
 �������ƣ�IATrack_GetCurrentElementNum
 ������������ȡ��ǰ�켣��Ϣ����
 ���������  	
	@handle��			IATrack���
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_IATRACK_API LONG IATrack_GetCurrentElementNum(IATRACK_HANDLE handle);


/*****************************************************************************
 �������ƣ�IATrack_FreeHandle
 �����������ͷ�IATrack���
 ���������
	@handle��       IATrack���
 ���������NA
 �� �� ֵ��LONG
*****************************************************************************/
IVS_IATRACK_API LONG IATrack_FreeHandle(IATRACK_HANDLE handle);



#endif // __IATrack_API_H__


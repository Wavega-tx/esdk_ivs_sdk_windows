/********************************************************************
filename 	: IVS_IATrack.h
author   	: z00227377
created 	: 2012/12/20
description : �켣������ʾ��
copyright 	: Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history 	: 2012/12/20 ��ʼ�汾
*********************************************************************/


#ifndef  __CIATRACK_H__
#define  __CIATRACK_H__

#include <list>
#include "Lock.h"
#include "IVS_IATrackAPI.h"
#include "IVS_DrawDataType.h"

// �켣��Ϣ���б�
typedef std::list<IA_LAYOUT_MESSAGE*>					IA_LAYOUT_MESSAGE_LIST;
typedef IA_LAYOUT_MESSAGE_LIST::iterator			IA_LAYOUT_MESSAGE_LIST_ITER;
typedef IA_LAYOUT_MESSAGE_LIST::reverse_iterator	IA_LAYOUT_MESSAGE_LIST_REVERSE_ITER;


class CIATrack
{
public:
	CIATrack();
	~CIATrack();

	// ��ʼ�����ܷ����켣
	LONG InitIATrack(HWND hMsgWnd, ULONG ulVideoType);

	// �������ܷ����켣��Ϣ
	LONG InsertIALayoutMessage(const IA_LAYOUT_MESSAGE *LayoutMessage);

	// ��ʾ���ܷ����켣��Ϣ
	LONG ShowIATrack(HDC hDC, const RECT *rc, ULONG64 ulFrameID);

	// ��ȡ��ǰ�켣��Ϣ����
	LONG GetCurrentElementNum();

protected:

	// ΪMessage�����ڴ�
	IA_LAYOUT_MESSAGE* AllocateMessageMemory();

	// ΪMessage�ͷ��ڴ�
	VOID ReleaseMessageMemory(IA_LAYOUT_MESSAGE* m);

	// ����ת��
	LONG CoordinateTransform(IA_LAYOUT_ELEMENT &LayoutElement, ULONG ulFrameWidth, ULONG ulFrameHeight);

	// ɾ���б��й�����Ϣ
	VOID DeleteExpiredMessage(IA_LAYOUT_MESSAGE_LIST &DestList, ULONG &refListLength, BOOL bIsAlarmed);

	// ����Layer��ֺ����Ϣ���뵽��Ӧ���б���
	LONG InsertMessageIntoList(const IA_LAYOUT_MESSAGE &refNewMessage, IA_LAYOUT_MESSAGE_LIST &DestList, ULONG &refListLength);

	// ��ʾ�б������ڵ�Ĺ켣��Ϣ
	LONG ShowIATrackMessage(HDC hDC, IA_LAYOUT_MESSAGE_LIST &DestList, BOOL bIsAlarmed);

	// ��ʾ�����켣Element
	LONG ShowIATrackElement(HDC hDC, IA_LAYOUT_ELEMENT &LayoutElememt);

	// ��ʾ�켣������Ϣ
	LONG ShowIATrackText(HDC hDC, IA_ALARM_TEXT &AlarmText, COLORREF ulColor);

	// ��ʾ�켣ͼ����Ϣ
	LONG ShowIATrackPolyline(HDC hDC, IA_ALARM_POLYLINE &AlarmPolyline, COLORREF ulColor);
	
    // �����ͷ����
    LONG CalcuArrowPoints(const IA_ALARM_POLYLINE& PolyLineInfo, IA_ARROW& ArrowPointSet);

    // ���Ƽ�ͷ
    LONG ShowArrowShape(HDC hDC, const IA_ARROW &iaArrow, ULONG ulArrowType);

	// ��������
	LONG CreatFont(/*ULONG ulFontSize*/);

private:

	BOOL					m_bInitTag;					// ����Ƿ��Ѿ���ʼ��

	ULONG64					m_ulCurrentFrameID;			// ��ǰʱ���
	ULONG					m_IAROIListLength;			// ROI�켣��Ϣ�б���
	ULONG					m_IAAlarmedListLength;		// Alarmed�켣��Ϣ�б���
	IA_LAYOUT_MESSAGE_LIST	m_IAROIList;				// ROI�켣��Ϣ�б�
	IA_LAYOUT_MESSAGE_LIST	m_IAAlarmedList;			// Alarmed�켣��Ϣ�б�

	HWND					m_hMsgWnd;					// ���ھ��
	ULONG					m_ulWndWidth;				// ���ڿ��
	ULONG					m_ulWndHeight;				// ���ڸ߶�
	ULONG					m_ulDCWidth;				// ��ʾͼ����
	ULONG					m_ulDCHeight;				// ��ʾͼ��߶�
	LONG					m_lLineWidthAdjust;			// �߿�ϵ�������ڷֱ��ʵ���
	ULONG					m_ulLineWidth;				// ͼ���߿�
	ULONG					m_ulVideoType;				// ��Ƶ���ͣ�ʵ����¼��
	LOGFONT					m_lf;						// ������������ṹ��
	HFONT					m_hFont;					// ������
	ULONG					m_ulCurrentFontSize;		// ��ǰ�����С
	CIVSMutex				m_Mutex;					// �켣��Ϣ�б���

};







#endif //__CIATRACK_H__
/*****************************************************************
 filename    :    VideoSoundCtrl.h
 author      :    ykf75928
 created     :    2013/01/15
 description :    �����ؼ�
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013/01/15 ��ʼ�汾
*****************************************************************/
#pragma once
#ifndef __VIDEO_SOUND_CTRL_H_
#define __VIDEO_SOUND_CTRL_H_

#include "BitmapSlider.h"

typedef enum ST_SOUNDCTRL_WND_TYPE
{
    SOUNDCTRL_WND_TYPE_SOUND    = 0,
    SOUNDCTRL_WND_TYPE_TALKBACK = 1,
    SOUNDCTRL_WND_TYPE_RECORD   = 2,
    SOUNDCTRL_WND_TYPE_MAX
}SOUNDCTRL_WND_TYPE;


// CVideoSoundCtrl dialog

class CVideoSoundCtrl : public CDialog
{
	DECLARE_DYNAMIC(CVideoSoundCtrl)

public:
	CVideoSoundCtrl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVideoSoundCtrl();

// Dialog Data
	enum { IDD = IDD_DLG_SOUNDCTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	ULONG         m_ulWndType;         // ��¼����   ��·���� �����Խ� �ط�����
	CWnd *        m_pRealTimeToolBar;         
	CBitmapSlider m_sliderSoundCtrl;
	CBitmapButton m_buttonSilence;

	ULONG         m_ulSliderPosition;  // ������λ��	
	BOOL          m_ubCheckSoundCtrlStatus;  // ������״̬
public:
	virtual BOOL    OnInitDialog();
	afx_msg void    OnPaint();
	afx_msg int     OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH  OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void    OnBnClickedButtonSoundCtrl();

	LRESULT        OnBitmapSliderMoved(WPARAM wParam, LPARAM lParam);   // �������ƶ����¼�
	void           ProcessSliderMoved();  // ���Ž��Ȼ����������¼�

	// �������ڱ������ĸ������Ѿ����Ǵ�����Ҫ��¼�¶�Ӧ�Ĵ���
	void           SetVideopBar(CWnd * pVideoBar);

	// ���þ���ͼ��
	void           SetSoundSilencePic(BOOL bState);
	void           SetSliderPosition(ULONG ulPos);
	void           SetTalkbackSilencePic(BOOL bState);
	void           SetRecordSilencePic(BOOL bState);
	ULONG          GetSliderPosition() const {return m_ulSliderPosition;} 

	// ���Ϳ���
	ULONG          GetSoundCtrlType();
	void           SetSoundCtrlType(ULONG ulType);
	int            GetSkinType();
};
#endif

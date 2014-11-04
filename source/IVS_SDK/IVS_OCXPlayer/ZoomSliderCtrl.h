#ifndef __ZOOM_SLIDER_CTRL_H_
#define __ZOOM_SLIDER_CTRL_H_

#include "BitmapSlider.h"

const ULONG MAGBIFYCTRL_SLIDER_INTERVAL = 1; // ���������
const ULONG MAGBIFYCTRL_SLIDER_MAX = 800;     // ����������
const ULONG MAGBIFYCTRL_SLIDER_MIN = 100;     // ���
const ULONG MAGBIFYCTRL_CLICK_INTERVAL = 100;     // ������Ӽ��
const ULONG CHANNEL_HEIGHT = 10;            // �������߶�

// CVideoSoundCtrl dialog

class CZoomSliderCtrl : public CDialog
{
	DECLARE_DYNAMIC(CZoomSliderCtrl)

public:
	CZoomSliderCtrl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CZoomSliderCtrl();

// Dialog Data
	enum { IDD = IDD_DLG_MAGNIFYCTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	//CWnd *        m_pRealTimeToolBar;         
	CBitmapSlider m_sliderMagbifyCtrl;
	ULONG         m_ulSliderPosition;  // ������λ��	
public:
	virtual BOOL    OnInitDialog();
	afx_msg void    OnPaint();
	afx_msg BOOL    OnEraseBkgnd(CDC* pDC);
	afx_msg int     OnCreate(LPCREATESTRUCT lpCreateStruct);

	LRESULT        OnBitmapSliderMoved(WPARAM wParam, LPARAM lParam);   // �������ƶ����¼�
	void           ProcessSliderMoved();  // ���Ž��Ȼ����������¼�

	// ����λ��
	void           SetSliderPosition(ULONG ulPos);
	ULONG          GetSliderPosition()const;
	void           IncSliderPosition();
	void           DecSliderPosition();
public:
	CBitmapSlider& GetMagbifyCtrl(){ return m_sliderMagbifyCtrl;} //lint !e1536
};
#endif // __ZOOM_SLIDER_CTRL_H_

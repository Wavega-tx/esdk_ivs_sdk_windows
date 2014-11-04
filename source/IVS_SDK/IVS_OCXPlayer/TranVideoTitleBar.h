/*****************************************************************
 filename    :    TranVideoTitleBar.h
 author      :    ykf75928
 created     :    2013/01/15
 description :    ͸��������
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013/01/15 ��ʼ�汾
*****************************************************************/
#pragma once

#include "TranslucentButton.h"
#include "TranslucentDialog.h"
#include "TranslucentStatic.h"
#include "afxwin.h"

// CTranVideoTitleBar dialog

class CTranVideoTitleBar : public CTranslucentDialog
{
	DECLARE_DYNAMIC(CTranVideoTitleBar)

public:
	CTranVideoTitleBar(Gdiplus::Image* pImage, CWnd* pParent = NULL);   // standard constructor
	virtual ~CTranVideoTitleBar();
// Dialog Data
	enum { IDD = IDD_DLG_TVIDEOTITLEBAR };

    virtual void OnInitChildrenWnds();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTranslucentStatic m_StaticDevName;
    // �رհ�ť
    CTranslucentButton m_btnOFF;
    // ��Ƶ��Ϣ��ť
    CTranslucentButton m_btnInf;
	// ����ǽ������ǽ
	CTranslucentButton m_btnTVWallHandling;
	// ����ǽ����ǽ
	CTranslucentButton m_btnTVWallOK;
	// ����ǽ��Ƶ����
	CTranslucentButton m_btnTVWallVideoBroken;
	//ˮӡ״̬
	CTranslucentButton m_btnWaterMakr;
	// ��ѵ״̬
	CTranslucentButton m_btnTurnCruiseStatus;
	// ��֡״̬
	CTranslucentButton m_btnSingleFrameStatus;
	// �澯״̬
	CTranslucentButton m_btnAlarmStatus;
	// �ֶ�¼��״̬
	CTranslucentButton m_btnRecordStatus;
	// Ѳ��
	CTranslucentButton m_btnCruiseTrackStatus;
    //����ǽ
    CTranslucentButton m_btnTvWallReconnect;

    afx_msg void OnBnClickedBtnTitleOff();
    afx_msg void OnBnClickedBtnTitleInf();

    void SetCurrentPaneToActivePane();

    afx_msg void OnPaint();

    //�ѹ�������ť��С����Ϊ0
    void ZeroSize();

    //�����豸����
    std::string GetDevName() const;//lint !e1036 !e1536
    void SetDevName(const std::string &strDevName);

    // ��ô�������
    WND_TYPE GetWndType() const;
    // ���Client����
    IVS_CLIENT_TYPE GetClientMode() const;
    virtual BOOL OnInitDialog();
	CPoint GetInfoButtonRightBottomPosition()const;

	void SetButtonPicture(CTranslucentButton &tButton,const std::string &strButtonName);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

    void EnableALLButton(BOOL isEnable);
private:
	// ����ǽ״̬
	ULONG m_ulTVWallStatus;
	// ˮӡ״̬
	LONG m_ulWatermarkStutus;
	// ��ѵ״̬
	ULONG m_ulTurnCruiseStatus;
	// ��֡״̬
	ULONG m_ulSingleFrameStatus;
	// �澯״̬
	ULONG m_ulAlarmStatus;
	// �ֶ�¼��״̬
	ULONG m_ulRecordStatus;
	// Ѳ��
	ULONG m_ulCruiseTrackStatus;
public:
	void SetTVWallStatus(ULONG ulStatus){m_ulTVWallStatus = ulStatus;}
    ULONG GetTVWallStatus()const{return m_ulTVWallStatus;}
	void SetWatermarkStatus(ULONG ulStatus);
	void SetTurnCruiseStatus(ULONG ulStatus){m_ulTurnCruiseStatus = ulStatus;}
    ULONG GetTurnCruiseStatus()const { return m_ulTurnCruiseStatus; }
	void SetSingleFrameStatus(ULONG ulStatus){m_ulSingleFrameStatus = ulStatus;}
	void SetAlarmStatus(ULONG ulStatus){m_ulAlarmStatus = ulStatus;}
	void SetRecordStatus(ULONG ulStatus){m_ulRecordStatus = ulStatus;}
	void SetCruiseTrackStatus(ULONG ulStatus){m_ulCruiseTrackStatus = ulStatus;}

	HWND GetOffBtnHwnd()const {return m_btnOFF.GetSafeHwnd(); }
	void UpdateAllButtonSkin();
    
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
}; //lint !e1712 MFC ������

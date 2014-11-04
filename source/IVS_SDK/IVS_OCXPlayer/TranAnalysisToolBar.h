/*****************************************************************
 filename    :    TranAnalysisToolBar.h
 author      :    ykf75928
 created     :    2013/01/15
 description :    ���ܷ���������
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013/01/15 ��ʼ�汾
*****************************************************************/
#pragma once

#include "translucentbutton.h"

// CTranAnalysisToolBar dialog

class CTranAnalysisToolBar : public CTranslucentDialog
{
	DECLARE_DYNAMIC(CTranAnalysisToolBar)

public:
	CTranAnalysisToolBar(LPCTSTR lpszFile, CWnd* pParent = NULL);   // standard constructor
	CTranAnalysisToolBar(Gdiplus::Image* pImage, CWnd* pParent = NULL);   // standard constructor
	virtual ~CTranAnalysisToolBar();
    virtual void OnInitChildrenWnds();
    virtual BOOL OnInitDialog();
    void SetButtonPicture( CTranslucentButton &tButton, const std::string &strButtonName);
    void SetAnalysisButtonPicture( CTranslucentButton &tButton, const std::string &strButtonName);

    BOOL m_IsStartPay;
    void SetIsStartPay(BOOL val);
    // Dialog Data
	enum { IDD = IDD_DLG_ANALYSISTOOLBAR };
    //CWnd* m_pVideoPane;
    //void SetVideoPane(CWnd * pVideoPane){m_pVideoPane = pVideoPane;}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnRect();
    afx_msg void OnBnClickedBtnPolygon();
	afx_msg void OnBnClickedBtnDelete();
    afx_msg void OnBnClickedBtnDisplayMode();
    afx_msg void OnBnClickedBtnPlay();    
    afx_msg void OnPaint();

    void DrawButton();

    afx_msg void OnMouseMove(UINT nFlags, CPoint point);

    void SetCurrentPaneToActivePane();

private:
    std::vector<CTranslucentButton *> m_btnVector;
#pragma region BTN
    //����
    CTranslucentButton m_BtnRect;
    //�޹���ͼ��
    CTranslucentButton m_BtnPolygon;
    //ɾ��
    CTranslucentButton m_BtnDelete;
    //��ʾģʽ
    CTranslucentButton m_BtnDisplayMode;
    //����
    CTranslucentButton m_BtnPlay;
#pragma endregion BTN
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
private:
    ULONGLONG  m_ullToolBarBtnVisableStatus;
public:
    ULONGLONG GetToolBarVisableStatus() const { return m_ullToolBarBtnVisableStatus; }
    void SetToolBarVisableStatus(ULONGLONG val) { m_ullToolBarBtnVisableStatus = val; }
	void SetToolbarEnableStatus( ULONGLONG dwToolbarBtn, ULONGLONG lButtonStatus );
	afx_msg void OnClickedBtnDelete();
	void ResetBottomButton();
	void EnableALLButton( BOOL isEnable );
}; //lint !e1712 MFC������

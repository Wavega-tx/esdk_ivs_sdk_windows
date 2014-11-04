// TranslucentDialog.h : ͷ�ļ�
//

#pragma once

#include "TranslucentWnd.h"
#include "TranslucentButton.h"
#include "SDKDef.h"
#include  "vos.h"
#include "TranslucentUtility.h"

typedef std::list<CTranslucentWnd*> TranslucentWndList;
typedef BOOL (WINAPI *MYFUNC)(HWND, HDC, POINT*, SIZE*, HDC, POINT*, COLORREF, BLENDFUNCTION*, DWORD); 
// CTranslucentDialog �Ի���
class CTranslucentDialog : public CDialog, public IRenderListener
{
	DECLARE_DYNAMIC(CTranslucentDialog)
	// ����
public:
	CTranslucentDialog(UINT nIDTemplate, LPCTSTR lpszFile, CWnd* pParent = NULL);
	CTranslucentDialog(UINT nIDTemplate, UINT nImgID, LPCTSTR lpszType = _T("PNG"), HINSTANCE hResourceModule = NULL, CWnd* pParent = NULL);
	//����һ��ֱ�Ӽ���Image�Ĺ��캯��
	CTranslucentDialog(UINT nIDTemplate, Gdiplus::Image* pImage, CWnd* pParent = NULL);

	virtual ~CTranslucentDialog();

	void SetEnableDrag(bool bEnableDrag);
	void SetCenterAligned(bool bCenterAligned);

	virtual void OnInitChildrenWnds() = 0;

	void RegisterTranslucentWnd(CTranslucentWnd* translucentWnd);
	void UnregisterTranslucentWnd(CTranslucentWnd* translucentWnd);

	void StartBuffered();
	void EndBuffered();

	virtual void OnRenderEvent(CTranslucentWnd* translucentWnd);

	void UpdateView();

	// ʵ��
protected:
	BLENDFUNCTION m_blend;
 private:
    Gdiplus::Image* m_pImage;
	//RECT m_rcWindow;
	bool m_bEnableDrag;
	bool m_bCenterAligned;
	bool m_bBuffered;

	Gdiplus::Size m_WindowSize;
	TranslucentWndList m_translucentWndList;
	VOS_Mutex* m_pMutexWndLock;
	MYFUNC mUpdateLayeredWindow;
protected:
    CWnd* m_pVideoPane;
public:
    CWnd* GetVideoPane() const { return m_pVideoPane; } //lint !e1763  MFC�Զ����ɵĵĺ���
    void SetVideoPane(CWnd * pVideoPane){m_pVideoPane = pVideoPane;}
public:

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	bool m_IsCreate;
public:
	void SetBackGroudPictrue(Gdiplus::Image* pImage);

protected:
	typedef std::map<std::string,CWnd*> MAP_TIP;
	typedef MAP_TIP::iterator MAP_TIP_ITER;
	MAP_TIP m_TipMap;
public:
	void UpdateAllTipText();
};//lint !e1712  MFC�Զ����ɵĵ���

#if !defined(AFX_BCGPPOPUPWINDOW_H__4DE5ED2B_6089_4A54_BF22_113C2C5FA4C0__INCLUDED_)
#define AFX_BCGPPOPUPWINDOW_H__4DE5ED2B_6089_4A54_BF22_113C2C5FA4C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BCGPPopupWindow.h : header file
//

#include "BCGPPopupDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CBCGPPopupWndButton window

class CBCGPPopupWndButton : public CBCGPButton
{
	virtual void OnFillBackground (CDC* pDC, const CRect& rectClient);
	virtual void OnDrawBorder (CDC* pDC, CRect& rectClient, UINT uiState);
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPPopupWindow window

class CBCGPPopupWindow : public CWnd
{
	DECLARE_DYNAMIC(CBCGPPopupWindow)

// Construction
public:
	CBCGPPopupWindow();

// Attributes
public:

protected:
	CBCGPPopupDlg		m_wndDlg;
	BOOL				m_bHasCloseButton;
	HMENU				m_hMenu;
	int					m_nAutoCloseTime;	// ms
	int					m_nCaptionHeight;

	CBCGPPopupWndButton	m_btnClose;
	CBCGPPopupWndButton	m_btnMenu;

	int					m_nBtnMarginVert;
	int					m_nBtnMarginHorz;

// Operations
public:
	BOOL ProcessCommand (HWND hwnd);

protected:
	CRect GetCaptionRect ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPPopupWindow)
	public:
	virtual BOOL Create(UINT uiDlgResID, CPoint ptPos = CPoint (-1, -1));
	//}}AFX_VIRTUAL
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

// Implementation
public:
	virtual ~CBCGPPopupWindow();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPPopupWindow)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNcDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg UINT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPPOPUPWINDOW_H__4DE5ED2B_6089_4A54_BF22_113C2C5FA4C0__INCLUDED_)

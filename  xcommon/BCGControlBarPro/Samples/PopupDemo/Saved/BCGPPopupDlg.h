#if !defined(AFX_BCGPPOPUPDLG_H__9EC5BC9D_ED2B_4255_A14E_E130CF5E49CA__INCLUDED_)
#define AFX_BCGPPOPUPDLG_H__9EC5BC9D_ED2B_4255_A14E_E130CF5E49CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BCGPPopupDlg.h : header file
//
#include "PopupDemo.h"

/////////////////////////////////////////////////////////////////////////////
// CBCGPPopupDlg window

class CBCGPPopupWindow;

class CBCGPPopupDlg : public CBCGPDialogBar
{
	DECLARE_DYNAMIC (CBCGPPopupDlg)

	friend class CBCGPPopupWindow;

// Construction
public:
	CBCGPPopupDlg();

// Attributes
protected:
	BOOL				m_bInitizlized;
	CBCGPPopupWindow*	m_pParentPopup;

// Operations
public:
	BOOL HasFocus () const;

// Dialog Data
	//{{AFX_DATA(CBCGPPopupDlg)
	enum { IDD = IDD_DIALOG1 };
	CBCGPURLLinkButton	m_Button;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPPopupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBCGPPopupDlg();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPPopupDlg)
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void Init ();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPPOPUPDLG_H__9EC5BC9D_ED2B_4255_A14E_E130CF5E49CA__INCLUDED_)

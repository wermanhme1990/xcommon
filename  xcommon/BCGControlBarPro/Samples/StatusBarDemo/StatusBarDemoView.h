// StatusBarDemoView.h : interface of the CStatusBarDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUSBARDEMOVIEW_H__78786632_D7C9_4EC9_B498_A5FEA916911B__INCLUDED_)
#define AFX_STATUSBARDEMOVIEW_H__78786632_D7C9_4EC9_B498_A5FEA916911B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MainFrm.h"

class CStatusBarDemoView : public CFormView
{
protected: // create from serialization only
	CStatusBarDemoView();
	DECLARE_DYNCREATE(CStatusBarDemoView)

public:
	//{{AFX_DATA(CStatusBarDemoView)
	enum { IDD = IDD_STATUSBARDEMO_FORM };
	CBCGPColorButton	m_wndTextColor;
	CBCGPColorButton	m_wndBackColor;
	CButton	m_wndStartAnimation;
	CButton	m_wndStartProgress;
	int		m_nIcon;
	//}}AFX_DATA

// Attributes
public:
	CStatusBarDemoDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatusBarDemoView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStatusBarDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CStatusBarDemoView)
	afx_msg void OnStartProgress();
	afx_msg void OnStartAnimation();
	afx_msg void OnIcon();
	afx_msg void OnBackColor();
	afx_msg void OnTextColor();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	afx_msg void OnIndicatorLabel();
	DECLARE_MESSAGE_MAP()

	CBCGPStatusBar& GetStatusBar () const
	{
		return ((CMainFrame*) AfxGetMainWnd ())->GetStatusBar ();
	}

	int			m_nProgressCurr;
	BOOL		m_bInProgress;

	CImageList	m_imlStatusAnimation;
	BOOL		m_bInAnimation;

	CBitmap		m_bmpIcon1;
	CBitmap		m_bmpIcon2;
};

#ifndef _DEBUG  // debug version in StatusBarDemoView.cpp
inline CStatusBarDemoDoc* CStatusBarDemoView::GetDocument()
   { return (CStatusBarDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATUSBARDEMOVIEW_H__78786632_D7C9_4EC9_B498_A5FEA916911B__INCLUDED_)

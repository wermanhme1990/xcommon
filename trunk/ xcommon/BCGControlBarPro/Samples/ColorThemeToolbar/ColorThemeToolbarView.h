// ColorThemeToolbarView.h : interface of the CColorThemeToolbarView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLORTHEMETOOLBARVIEW_H__A363C66E_02C1_467D_8E40_C740630A390E__INCLUDED_)
#define AFX_COLORTHEMETOOLBARVIEW_H__A363C66E_02C1_467D_8E40_C740630A390E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CColorThemeToolbarView : public CFormView
{
protected: // create from serialization only
	CColorThemeToolbarView();
	DECLARE_DYNCREATE(CColorThemeToolbarView)

public:
	//{{AFX_DATA(CColorThemeToolbarView)
	enum { IDD = IDD_COLORTHEMETOOLBAR_FORM };
	int		m_nColorTheme;
	//}}AFX_DATA

// Attributes
public:
	CColorThemeToolbarDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorThemeToolbarView)
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
	virtual ~CColorThemeToolbarView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CColorThemeToolbarView)
	afx_msg void OnColorTheme();
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ColorThemeToolbarView.cpp
inline CColorThemeToolbarDoc* CColorThemeToolbarView::GetDocument()
   { return (CColorThemeToolbarDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORTHEMETOOLBARVIEW_H__A363C66E_02C1_467D_8E40_C740630A390E__INCLUDED_)

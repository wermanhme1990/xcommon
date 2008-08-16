// ToolBoxDemoView.h : interface of the CToolBoxDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBOXDEMOVIEW_H__6FA58C24_40D7_4361_99EC_6A01673F0049__INCLUDED_)
#define AFX_TOOLBOXDEMOVIEW_H__6FA58C24_40D7_4361_99EC_6A01673F0049__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBListCtrl.h"

class CToolBoxDemoView : public CFormView
{
protected: // create from serialization only
	CToolBoxDemoView();
	DECLARE_DYNCREATE(CToolBoxDemoView)

public:
	//{{AFX_DATA(CToolBoxDemoView)
	enum { IDD = IDD_TOOLBOXDEMO_FORM };
	CTBListCtrl	m_TBListCtrl;
	int		m_nPageLayout;
	int		m_nToolBoxMode;
	int		m_nTheme;
	//}}AFX_DATA

// Attributes
public:
	CToolBoxDemoDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBoxDemoView)
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
	virtual ~CToolBoxDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CToolBoxDemoView)
	afx_msg void OnAddButton();
	afx_msg void OnSelchangePageLayout();
	afx_msg void OnSelchangeToolboxMode();
	afx_msg void OnSelchangeToolboxTheme();
	afx_msg void OnClearList();
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ToolBoxDemoView.cpp
inline CToolBoxDemoDoc* CToolBoxDemoView::GetDocument()
   { return (CToolBoxDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBOXDEMOVIEW_H__6FA58C24_40D7_4361_99EC_6A01673F0049__INCLUDED_)

// TasksPaneView.h : interface of the CTasksPaneView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKSPANEVIEW_H__1615D84C_3885_4A8C_AF62_DD181435E028__INCLUDED_)
#define AFX_TASKSPANEVIEW_H__1615D84C_3885_4A8C_AF62_DD181435E028__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TaskPane.h"

class CTasksPaneView : public CFormView
{
protected: // create from serialization only
	CTasksPaneView();
	DECLARE_DYNCREATE(CTasksPaneView)

public:
	//{{AFX_DATA(CTasksPaneView)
	enum { IDD = IDD_TASKSPANE_FORM };
	CBCGPColorButton	m_ColorTaskTextHot;
	CBCGPColorButton	m_ColorTaskText;
	CBCGPColorButton	m_ColorGroupTextHot;
	CBCGPColorButton	m_ColorGroupText;
	int		m_nVisualManager;
	BOOL	m_bWrapTasks;
	BOOL	m_bWrapLabels;
	BOOL	m_bNavToolbar;
	BOOL	m_bMenuButtons;
	BOOL	m_bHicolorImages;
	BOOL	m_bCollapseGroups;
	BOOL	m_bCustomizeMargins;
	BOOL	m_bScrollButtons;
	BOOL	m_bAnimation;
	int		m_nCaptionHeight;
	int		m_nGroupSpacing;
	int		m_nHMargin;
	int		m_nIconOffset;
	int		m_nTaskOffset;
	int		m_nTaskSpacing;
	int		m_nVMargin;
	BOOL	m_bCaptionIcon;
	//}}AFX_DATA

// Attributes
public:
	CTasksPaneDoc* GetDocument();
	CTaskPane* GetTasksPane();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTasksPaneView)
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
	virtual ~CTasksPaneView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTasksPaneView)
	afx_msg void OnVisualManager();
	afx_msg void OnWraptasks();
	afx_msg void OnWraplabels();
	afx_msg void OnNavtoolbar();
	afx_msg void OnHistbuttons();
	afx_msg void OnHicolorimages();
	afx_msg void OnDestroy();
	afx_msg void OnScrollbuttons();
	afx_msg void OnAnimation();
	afx_msg void OnCollapsegroups();
	afx_msg void OnMargins();
	afx_msg void OnColorGrouptext();
	afx_msg void OnColorGrouptexthot();
	afx_msg void OnColorTasktext();
	afx_msg void OnColorTasktexthot();
	afx_msg void OnChangeMargins();
	afx_msg void OnCaptionicon();
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TasksPaneView.cpp
inline CTasksPaneDoc* CTasksPaneView::GetDocument()
   { return (CTasksPaneDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKSPANEVIEW_H__1615D84C_3885_4A8C_AF62_DD181435E028__INCLUDED_)

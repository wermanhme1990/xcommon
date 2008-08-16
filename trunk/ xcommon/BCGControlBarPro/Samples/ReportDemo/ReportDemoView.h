// ReportDemoView.h : interface of the CReportDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_REPORTDEMOVIEW_H__AC23BEA5_8CE3_4B70_B0C3_2D73D6683483__INCLUDED_)
#define AFX_REPORTDEMOVIEW_H__AC23BEA5_8CE3_4B70_B0C3_2D73D6683483__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MailReportCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CFlagItem class

class CFlagItem: public CBCGPGridItem
{
public:
	CFlagItem(
		CBCGPToolBarImages& icons,
		int nSelectedIcon,
		DWORD dwData = 0
	);

public:
	virtual BOOL OnClickValue (UINT uiMsg, CPoint point);
	
protected:
	virtual void OnDrawValue (CDC* pDC, CRect rect);
	virtual void OnPrintValue (CDC* /*pDC*/, CRect /*rect*/) {}
	CBCGPToolBarImages&	m_Icons;
};

/////////////////////////////////////////////////////////////////////////////
// CReportDemoView

class CReportDemoView : public CBCGPReportView
{
protected: // create from serialization only
	CReportDemoView();
	DECLARE_DYNCREATE(CReportDemoView)

// Attributes
public:
	CReportDemoDoc* GetDocument();

protected:
	CImageList			m_ImagesHeader;
	CImageList			m_Images;
	CBCGPToolBarImages	m_Flags;

	enum IMPORTANCE
	{
		IMPORTANCE_NORMAL,
		IMPORTANCE_HIGH,
		IMPORTANCE_LOW
	};
	
// Operations
public:

// Overrides
	CBCGPGridCtrl* CreateGrid ()
	{
		return new CMailReportCtrl;
	}

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportDemoView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CReportDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// Generated message map functions
protected:
	//{{AFX_MSG(CReportDemoView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewColumnsselector();
	afx_msg void OnUpdateViewColumnsselector(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnViewGroupbox();
	afx_msg void OnUpdateViewGroupbox(CCmdUI* pCmdUI);
	afx_msg void OnViewColumnAutoResize();
	afx_msg void OnUpdateViewColumnAutoResize(CCmdUI* pCmdUI);
	afx_msg void OnReportExpandAll();
	afx_msg void OnReportCollapseAll();
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()

	BOOL AddMail (int nIcon, LPCTSTR strFrom, LPCTSTR strTo,
		 LPCTSTR strSubject, COleDateTime dateCreated, 
		 COleDateTime dateReceived, long lSize, 
		 IMPORTANCE importance = IMPORTANCE_NORMAL,
		 BOOL bHasAttachment = FALSE,
		 int nFlag = 0);
};

#ifndef _DEBUG  // debug version in ReportDemoView.cpp
inline CReportDemoDoc* CReportDemoView::GetDocument()
   { return (CReportDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTDEMOVIEW_H__AC23BEA5_8CE3_4B70_B0C3_2D73D6683483__INCLUDED_)

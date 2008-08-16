// RibbonMDIDemoView.h : interface of the CRibbonMDIDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIBBONMDIDEMOVIEW_H__C30832B5_7630_4AED_9298_1E72761B6208__INCLUDED_)
#define AFX_RIBBONMDIDEMOVIEW_H__C30832B5_7630_4AED_9298_1E72761B6208__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRibbonMDIDemoView : public CView
{
protected: // create from serialization only
	CRibbonMDIDemoView();
	DECLARE_DYNCREATE(CRibbonMDIDemoView)

// Attributes
public:
	CRibbonMDIDemoDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRibbonMDIDemoView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRibbonMDIDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRibbonMDIDemoView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RibbonMDIDemoView.cpp
inline CRibbonMDIDemoDoc* CRibbonMDIDemoView::GetDocument()
   { return (CRibbonMDIDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RIBBONMDIDEMOVIEW_H__C30832B5_7630_4AED_9298_1E72761B6208__INCLUDED_)

// BCGPVizioStyleExampleView.h : interface of the CBCGPVizioStyleExampleView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPVIZIOSTYLEEXAMPLEVIEW_H__5A626CD1_E3BB_444D_9A68_F157495E28D6__INCLUDED_)
#define AFX_BCGPVIZIOSTYLEEXAMPLEVIEW_H__5A626CD1_E3BB_444D_9A68_F157495E28D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBCGPVizioStyleExampleView : public CView
{
protected: // create from serialization only
	CBCGPVizioStyleExampleView();
	DECLARE_DYNCREATE(CBCGPVizioStyleExampleView)

// Attributes
public:
	CBCGPVizioStyleExampleDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPVizioStyleExampleView)
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
	virtual ~CBCGPVizioStyleExampleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPVizioStyleExampleView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BCGPVizioStyleExampleView.cpp
inline CBCGPVizioStyleExampleDoc* CBCGPVizioStyleExampleView::GetDocument()
   { return (CBCGPVizioStyleExampleDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPVIZIOSTYLEEXAMPLEVIEW_H__5A626CD1_E3BB_444D_9A68_F157495E28D6__INCLUDED_)

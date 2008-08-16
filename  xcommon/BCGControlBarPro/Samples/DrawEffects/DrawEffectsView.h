// DrawEffectsView.h : interface of the CDrawEffectsView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWEFFECTSVIEW_H__A0F04E77_9A79_427D_A9FC_CA4891EB331C__INCLUDED_)
#define AFX_DRAWEFFECTSVIEW_H__A0F04E77_9A79_427D_A9FC_CA4891EB331C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDrawEffectsView : public CScrollView
{
protected: // create from serialization only
	CDrawEffectsView();
	DECLARE_DYNCREATE(CDrawEffectsView)

// Attributes
public:
	CDrawEffectsDoc* GetDocument();

protected:
	CBCGPToolBarImages	m_Bitmap;

// Operations
public:

	void Shadow (CDC* pDC, CRect rect);
	void Gradient (CDC* pDC, CRect rect);
	void Highlight (CDC* pDC, CRect rect);
	void GradientRings (CDC* pDC, CRect rect);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawEffectsView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDrawEffectsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDrawEffectsView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DrawEffectsView.cpp
inline CDrawEffectsDoc* CDrawEffectsView::GetDocument()
   { return (CDrawEffectsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWEFFECTSVIEW_H__A0F04E77_9A79_427D_A9FC_CA4891EB331C__INCLUDED_)

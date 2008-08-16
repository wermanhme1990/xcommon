// SliderView.h : interface of the CSliderView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLIDERVIEW_H__44039040_2337_4853_97E3_6AA185688DE5__INCLUDED_)
#define AFX_SLIDERVIEW_H__44039040_2337_4853_97E3_6AA185688DE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSliderView : public CView
{
protected: // create from serialization only
	CSliderView();
	DECLARE_DYNCREATE(CSliderView)

// Attributes
public:
	CSliderDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSliderView)
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
	virtual ~CSliderView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSliderView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SliderView.cpp
inline CSliderDoc* CSliderView::GetDocument()
   { return (CSliderDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIDERVIEW_H__44039040_2337_4853_97E3_6AA185688DE5__INCLUDED_)

// RTLSampleView.h : interface of the CRTLSampleView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTLSAMPLEVIEW_H__B9B24401_443C_420C_BD5E_CF661EE8F03D__INCLUDED_)
#define AFX_RTLSAMPLEVIEW_H__B9B24401_443C_420C_BD5E_CF661EE8F03D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRTLSampleView : public CView
{
protected: // create from serialization only
	CRTLSampleView();
	DECLARE_DYNCREATE(CRTLSampleView)

// Attributes
public:
	CRTLSampleDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRTLSampleView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRTLSampleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRTLSampleView)
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RTLSampleView.cpp
inline CRTLSampleDoc* CRTLSampleView::GetDocument()
   { return (CRTLSampleDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTLSAMPLEVIEW_H__B9B24401_443C_420C_BD5E_CF661EE8F03D__INCLUDED_)

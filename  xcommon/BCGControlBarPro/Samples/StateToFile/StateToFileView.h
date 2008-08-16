// StateToFileView.h : interface of the CStateToFileView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATETOFILEVIEW_H__2FA215DF_AEF4_4E07_AAF9_A79CB0D15806__INCLUDED_)
#define AFX_STATETOFILEVIEW_H__2FA215DF_AEF4_4E07_AAF9_A79CB0D15806__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CStateToFileView : public CView
{
protected: // create from serialization only
	CStateToFileView();
	DECLARE_DYNCREATE(CStateToFileView)

// Attributes
public:
	CStateToFileDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStateToFileView)
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
	virtual ~CStateToFileView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CStateToFileView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in StateToFileView.cpp
inline CStateToFileDoc* CStateToFileView::GetDocument()
   { return (CStateToFileDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATETOFILEVIEW_H__2FA215DF_AEF4_4E07_AAF9_A79CB0D15806__INCLUDED_)

// MDIBarsView.h : interface of the CMDIBarsView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDIBARSVIEW_H__5FB564F0_0998_11D5_95C8_00A0C9289F1B__INCLUDED_)
#define AFX_MDIBARSVIEW_H__5FB564F0_0998_11D5_95C8_00A0C9289F1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMDIBarsView : public CView
{
protected: // create from serialization only
	CMDIBarsView();
	DECLARE_DYNCREATE(CMDIBarsView)

// Attributes
public:
	CMDIBarsDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDIBarsView)
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
	virtual ~CMDIBarsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMDIBarsView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MDIBarsView.cpp
inline CMDIBarsDoc* CMDIBarsView::GetDocument()
   { return (CMDIBarsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDIBARSVIEW_H__5FB564F0_0998_11D5_95C8_00A0C9289F1B__INCLUDED_)

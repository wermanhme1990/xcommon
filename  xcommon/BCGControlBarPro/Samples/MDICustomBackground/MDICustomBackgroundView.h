// MDICustomBackgroundView.h : interface of the CMDICustomBackgroundView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDICUSTOMBACKGROUNDVIEW_H__A74B6F55_D2DF_4401_9545_21A5C0B67B2C__INCLUDED_)
#define AFX_MDICUSTOMBACKGROUNDVIEW_H__A74B6F55_D2DF_4401_9545_21A5C0B67B2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMDICustomBackgroundView : public CView
{
protected: // create from serialization only
	CMDICustomBackgroundView();
	DECLARE_DYNCREATE(CMDICustomBackgroundView)

// Attributes
public:
	CMDICustomBackgroundDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDICustomBackgroundView)
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
	virtual ~CMDICustomBackgroundView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMDICustomBackgroundView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MDICustomBackgroundView.cpp
inline CMDICustomBackgroundDoc* CMDICustomBackgroundView::GetDocument()
   { return (CMDICustomBackgroundDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDICUSTOMBACKGROUNDVIEW_H__A74B6F55_D2DF_4401_9545_21A5C0B67B2C__INCLUDED_)

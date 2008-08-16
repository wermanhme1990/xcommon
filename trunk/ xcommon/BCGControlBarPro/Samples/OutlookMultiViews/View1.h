#if !defined(AFX_VIEW1_H__8B9B33F1_14FC_435C_9FE4_70C765E8A043__INCLUDED_)
#define AFX_VIEW1_H__8B9B33F1_14FC_435C_9FE4_70C765E8A043__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// View1.h : header file
//
//
// This sample is based on Vswap32 sample:
// http://support.microsoft.com/default.aspx?scid=kb;en-us;141334

/////////////////////////////////////////////////////////////////////////////
// CView1 view

class CView1 : public CView
{
public:
	CView1();           // protected constructor used by dynamic creation
	virtual ~CView1();

	DECLARE_DYNCREATE(CView1)

// Attributes

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CView1)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CView1)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEW1_H__8B9B33F1_14FC_435C_9FE4_70C765E8A043__INCLUDED_)

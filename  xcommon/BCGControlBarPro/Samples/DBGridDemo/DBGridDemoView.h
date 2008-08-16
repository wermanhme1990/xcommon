// DBGridDemoView.h : interface of the CDBGridDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBGRIDDEMOVIEW_H__F4EEA896_FBC5_49D1_9DDB_84BE280D33E5__INCLUDED_)
#define AFX_DBGRIDDEMOVIEW_H__F4EEA896_FBC5_49D1_9DDB_84BE280D33E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDBGridDemoView : public CBCGPGridView
{
protected: // create from serialization only
	CDBGridDemoView();
	DECLARE_DYNCREATE(CDBGridDemoView)

// Attributes
public:
	CDBGridDemoDoc* GetDocument();

// Operations
public:
	void DeleteAll ();

// Overrides
	
	virtual CBCGPGridCtrl* CreateGrid ()
	{
		return new CBCGPDAOGridCtrl;
//		return new CBCGPODBCGridCtrl;
//		return new CBCGPADOGridCtrl;
	}

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDBGridDemoView)
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
	virtual ~CDBGridDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDBGridDemoView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DBGridDemoView.cpp
inline CDBGridDemoDoc* CDBGridDemoView::GetDocument()
   { return (CDBGridDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBGRIDDEMOVIEW_H__F4EEA896_FBC5_49D1_9DDB_84BE280D33E5__INCLUDED_)

// FileDialogDemoView.h : interface of the CFileDialogDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEDIALOGDEMOVIEW_H__2C75A426_859B_43A3_A703_FDA01CAD1D4F__INCLUDED_)
#define AFX_FILEDIALOGDEMOVIEW_H__2C75A426_859B_43A3_A703_FDA01CAD1D4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFileDialogDemoView : public CView
{
protected: // create from serialization only
	CFileDialogDemoView();
	DECLARE_DYNCREATE(CFileDialogDemoView)

// Attributes
public:
	CFileDialogDemoDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileDialogDemoView)
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
	virtual ~CFileDialogDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFileDialogDemoView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FileDialogDemoView.cpp
inline CFileDialogDemoDoc* CFileDialogDemoView::GetDocument()
   { return (CFileDialogDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEDIALOGDEMOVIEW_H__2C75A426_859B_43A3_A703_FDA01CAD1D4F__INCLUDED_)

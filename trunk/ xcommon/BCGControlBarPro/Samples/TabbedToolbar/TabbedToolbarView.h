// TabbedToolbarView.h : interface of the CTabbedToolbarView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABBEDTOOLBARVIEW_H__E477C16F_2BCA_4EDA_928B_F3FA4B1DB477__INCLUDED_)
#define AFX_TABBEDTOOLBARVIEW_H__E477C16F_2BCA_4EDA_928B_F3FA4B1DB477__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTabbedToolbarView : public CFormView
{
protected: // create from serialization only
	CTabbedToolbarView();
	DECLARE_DYNCREATE(CTabbedToolbarView)

public:
	//{{AFX_DATA(CTabbedToolbarView)
	enum{ IDD = IDD_TABBEDTOOLBAR_FORM };
	int		m_nVisualManager;
	//}}AFX_DATA

// Attributes
public:
	CTabbedToolbarDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabbedToolbarView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTabbedToolbarView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTabbedToolbarView)
	afx_msg void OnVisualManager();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TabbedToolbarView.cpp
inline CTabbedToolbarDoc* CTabbedToolbarView::GetDocument()
   { return (CTabbedToolbarDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABBEDTOOLBARVIEW_H__E477C16F_2BCA_4EDA_928B_F3FA4B1DB477__INCLUDED_)

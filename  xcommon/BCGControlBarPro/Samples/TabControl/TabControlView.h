// TabControlView.h : interface of the CTabControlView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABCONTROLVIEW_H__F43D69E7_A814_41CF_A602_940A78DBBF12__INCLUDED_)
#define AFX_TABCONTROLVIEW_H__F43D69E7_A814_41CF_A602_940A78DBBF12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTabControlView : public CFormView
{
protected: // create from serialization only
	CTabControlView();
	DECLARE_DYNCREATE(CTabControlView)

public:
	//{{AFX_DATA(CTabControlView)
	enum { IDD = IDD_TABCONTROL_FORM };
	CButton	m_wndTabIcons;
	CListBox	m_wndNotifications;
	CStatic	m_wndTabLoc;
	int		m_nTabStyle;
	int		m_nTabLocation;
	int		m_nColor;
	BOOL	m_bTabIcons;
	BOOL	m_bTabsSwap;
	//}}AFX_DATA

// Attributes
public:
	CTabControlDoc* GetDocument();

protected:
	CBCGPTabWnd	m_wndTab;

	CEdit		m_wnd1;
	CEdit		m_wnd2;
	CEdit		m_wnd3;
	CEdit		m_wnd4;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabControlView)
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
	virtual ~CTabControlView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void AddNotification (LPCTSTR lpszText);

// Generated message map functions
protected:
	//{{AFX_MSG(CTabControlView)
	afx_msg void OnSelchangeTabStyle();
	afx_msg void OnLocation();
	afx_msg void OnColor();
	afx_msg void OnTabIcons();
	afx_msg void OnTabSwap();
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	afx_msg LRESULT OnMoveTab(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnChangeActiveTab(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnChangingActiveTab(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TabControlView.cpp
inline CTabControlDoc* CTabControlView::GetDocument()
   { return (CTabControlDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCONTROLVIEW_H__F43D69E7_A814_41CF_A602_940A78DBBF12__INCLUDED_)

#if !defined(AFX_OUTPUTBAR_H__708257AC_7B83_11D3_A74B_0090274409AC__INCLUDED_)
#define AFX_OUTPUTBAR_H__708257AC_7B83_11D3_A74B_0090274409AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutputBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COutputList1 window

class COutputList1 : public CListBox
{
// Construction
public:
	COutputList1();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputList1)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COutputList1();

	// Generated message map functions
protected:
	//{{AFX_MSG(COutputList1)
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnOutputGotoError();
	afx_msg void OnViewOutput();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// COutputBar window

class COutputBar : public CBCGPDockingControlBar
{
// Construction
public:
	COutputBar();

// Attributes
protected:
	CBCGPTabWnd	m_wndTabs;
	CFont		m_Font;

	COutputList1	m_wndOutputBuild;
	COutputList1	m_wndOutputDebug;
	COutputList1	m_wndOutputFind1;
	COutputList1	m_wndOutputFind2;
	COutputList1	m_wndOutputSQL;

// Operations
public:
	void SetFlatTabs (BOOL bFlat = TRUE, BOOL bRepaint = TRUE)
	{
		m_wndTabs.SetFlatFrame (bFlat, bRepaint);
	}

protected:
	void FillBuildWindow ();
	void FillDebugWindow ();
	void AdjusrHorzScroll (CListBox& wndListBox);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COutputBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(COutputBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTBAR_H__708257AC_7B83_11D3_A74B_0090274409AC__INCLUDED_)

#if !defined(AFX_EXPLORERBAR_H__E15D7C82_B803_11D4_95C8_00A0C9289F1B__INCLUDED_)
#define AFX_EXPLORERBAR_H__E15D7C82_B803_11D4_95C8_00A0C9289F1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExplorerBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExplorerBar window

class CExplorerBar : public CBCGPTasksPane
{
// Construction
public:
	CExplorerBar();

// Attributes
protected:
	CTreeCtrl		m_wndFavorites;

// Operations
protected:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExplorerBar)
	//}}AFX_VIRTUAL

protected:

// Implementation
public:
	virtual ~CExplorerBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExplorerBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnSelectTree(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPLORERBAR_H__E15D7C82_B803_11D4_95C8_00A0C9289F1B__INCLUDED_)

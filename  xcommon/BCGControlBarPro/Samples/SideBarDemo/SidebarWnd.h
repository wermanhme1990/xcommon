#if !defined(AFX_SIDEBARWND_H__B2718A0A_D7B5_41A6_99A3_4A56D1202035__INCLUDED_)
#define AFX_SIDEBARWND_H__B2718A0A_D7B5_41A6_99A3_4A56D1202035__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "BCGPAppBarWnd.h"
#include "ChildBar.h"

// SidebarWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSidebarWnd window

class CSidebarWnd : public CBCGPAppBarWnd
{
// Construction
public:
	CSidebarWnd();

// Attributes
protected:
	CChildBar		m_wndChildBar;
	CTreeCtrl		m_wndTree;
	NOTIFYICONDATA	m_nid;			// struct for Shell_NotifyIcon args
	HICON			m_hIcon;

// Operations
public:
	void OnChangeLook ();
	void OnTrayContextMenu ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSidebarWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSidebarWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSidebarWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnAppExit();
	afx_msg void OnAppOptions();
	//}}AFX_MSG
	afx_msg LRESULT OnTrayNotify(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIDEBARWND_H__B2718A0A_D7B5_41A6_99A3_4A56D1202035__INCLUDED_)

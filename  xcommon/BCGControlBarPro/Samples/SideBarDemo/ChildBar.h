// ChildBar.h : interface of the CChildBar class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDBAR_H__D9676E2D_0121_4AEE_965F_E0FB7E1692A1__INCLUDED_)
#define AFX_CHILDBAR_H__D9676E2D_0121_4AEE_965F_E0FB7E1692A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ClockWnd.h"

class CChildBar : public CBCGPTasksPane
{
// Construction
public:
	CChildBar();
	
// Attributes
protected:
	CBCGPCalendar	m_wndCalendar;
	CBCGPCalculator	m_wndCalculator;
	CClockWnd		m_wndClock;

// Operations
public:

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildBar)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

protected:
	BOOL CreateCalendar();
	BOOL CreateCalculator();
	BOOL CreateClockControl();

// Implementation
public:
	virtual ~CChildBar();

	virtual CBCGPMiniFrameWnd* CreateDefaultMiniframe (CRect rectInitial);
	virtual BOOL OnShowControlBarMenu (CPoint point);

	// Generated message map functions
protected:
	//{{AFX_MSG(CChildBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDBAR_H__D9676E2D_0121_4AEE_965F_E0FB7E1692A1__INCLUDED_)

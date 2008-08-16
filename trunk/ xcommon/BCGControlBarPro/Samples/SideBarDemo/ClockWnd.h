#if !defined(AFX_CLOCKWND_H__BA4560A0_460C_4CE6_B31D_4F133C97F86E__INCLUDED_)
#define AFX_CLOCKWND_H__BA4560A0_460C_4CE6_B31D_4F133C97F86E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClockWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClockWnd window

class CClockWnd : public CWnd
{
// Construction
public:
	CClockWnd();

// Attributes
protected:
	CBCGPToolBarImages	m_BackImage;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClockWnd)
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CClockWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CClockWnd)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLOCKWND_H__BA4560A0_460C_4CE6_B31D_4F133C97F86E__INCLUDED_)

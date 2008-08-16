#if !defined(AFX_INFOBAR_H__B75B07FA_89FC_4888_8C0B_293487F49F67__INCLUDED_)
#define AFX_INFOBAR_H__B75B07FA_89FC_4888_8C0B_293487F49F67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInfoBar window

class CInfoBar : public CBCGPDockingControlBar
{
// Construction
public:
	CInfoBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInfoBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInfoBar)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOBAR_H__B75B07FA_89FC_4888_8C0B_293487F49F67__INCLUDED_)

#if !defined(AFX_BCGPMSMTASKSPANE_H__40372820_EB1E_43BF_A81A_AC918194A9B5__INCLUDED_)
#define AFX_BCGPMSMTASKSPANE_H__40372820_EB1E_43BF_A81A_AC918194A9B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BCGPMSMTasksPane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBCGPMSMTasksPane window

class CBCGPMSMTasksPane : public CBCGPTasksPane
{
	DECLARE_DYNAMIC(CBCGPMSMTasksPane)

// Construction
public:
	CBCGPMSMTasksPane();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPMSMTasksPane)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBCGPMSMTasksPane();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPMSMTasksPane)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPMSMTASKSPANE_H__40372820_EB1E_43BF_A81A_AC918194A9B5__INCLUDED_)

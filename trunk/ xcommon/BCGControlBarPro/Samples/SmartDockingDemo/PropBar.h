#if !defined(AFX_PROPBAR_H__45B57F59_E16C_4912_BC34_03BDD465E173__INCLUDED_)
#define AFX_PROPBAR_H__45B57F59_E16C_4912_BC34_03BDD465E173__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropBar window

class CPropBar : public CBCGPDockingControlBar
{
// Construction
public:
	CPropBar();

// Attributes
protected:
	CBCGPPropList m_wndProp;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPBAR_H__45B57F59_E16C_4912_BC34_03BDD465E173__INCLUDED_)

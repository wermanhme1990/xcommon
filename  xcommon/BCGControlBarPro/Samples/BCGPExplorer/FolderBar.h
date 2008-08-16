#if !defined(AFX_FOLDERBAR_H__CBF22A6B_F6A3_43B1_B915_A38F2108C757__INCLUDED_)
#define AFX_FOLDERBAR_H__CBF22A6B_F6A3_43B1_B915_A38F2108C757__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FolderBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFolderBar window

class CFolderBar : public CBCGPDockingControlBar
{
// Construction
public:
	CFolderBar();

// Attributes
public:
	CBCGPShellTree	m_wndShellTree;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFolderBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFolderBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFolderBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FOLDERBAR_H__CBF22A6B_F6A3_43B1_B915_A38F2108C757__INCLUDED_)

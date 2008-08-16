// WorkspaceBar.h : interface of the CWorkspaceBar class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKSPACEBAR_H__64AD42E5_DF64_4528_83F6_D532A007F118__INCLUDED_)
#define AFX_WORKSPACEBAR_H__64AD42E5_DF64_4528_83F6_D532A007F118__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWorkspaceBar : public CBCGPDockingControlBar
{
public:
	CWorkspaceBar();

// Attributes
protected:
	CBCGPPropList	m_wndPropList;

	enum
	{
		idShowMDITabs,
		idMaximizeMDIChild,
		idTabsOnTop,
		idTabsStyle,
		idTabsAutoColor,
		idMDITabsIcons,
		idMDITabsDocMenu,
		idDragMDITabs,
		idMDITabsContextMenu,
		idMDITabsBorderSize,
		idDisableMDIChildRedraw,
		idFlatFrame,
		idCustomTooltips
	};

// Operations
public:
	void SetPropState ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkspaceBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWorkspaceBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CWorkspaceBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LRESULT OnPropertyChanged (WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKSPACEBAR_H__64AD42E5_DF64_4528_83F6_D532A007F118__INCLUDED_)

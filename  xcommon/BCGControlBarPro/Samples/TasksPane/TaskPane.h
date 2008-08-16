// TaskPane.h : interface of the CTaskPane class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKPANE_H__7FF79EF9_D239_46F7_80B4_FC516CF0341A__INCLUDED_)
#define AFX_TASKPANE_H__7FF79EF9_D239_46F7_80B4_FC516CF0341A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTaskPane : public CBCGPTasksPane
{
	friend class CTasksPaneView;

// Construction
public:
	CTaskPane();
	
// Attributes
	int	m_nUserColorGroup;
	int	m_nUserColorTask;

protected:
	int	m_nDocumentsGroup;

	CFont		m_Font;
	CTreeCtrl	m_wndTree;
	CEdit		m_wndEdit;

// Operations
public:
	void UpdateMRUFilesList ();
	void UpdateToolbar ();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskPane)
	//}}AFX_VIRTUAL

protected:
	BOOL CreateTreeControl();
	BOOL CreateEditControl();

// Implementation
public:
	virtual ~CTaskPane();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTaskPane)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKPANE_H__7FF79EF9_D239_46F7_80B4_FC516CF0341A__INCLUDED_)

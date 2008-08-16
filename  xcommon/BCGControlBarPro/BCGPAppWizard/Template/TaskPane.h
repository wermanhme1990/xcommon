// $$taskpane_hfile$$.h : interface of the $$TASKPANE_CLASS$$ class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)
#define $$FILE_NAME_SYMBOL$$_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class $$TASKPANE_CLASS$$ : public CBCGPTasksPane
{
// Construction
public:
	$$TASKPANE_CLASS$$();
	
// Attributes
protected:
	int	m_nDocumentsGroup;

$$IF(TASKPANE_PAGES)
	CFont		m_Font;
	CTreeCtrl	m_wndTree;
	CEdit		m_wndEdit;
$$ENDIF

// Operations
public:
	void UpdateMRUFilesList ();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL($$TASKPANE_CLASS$$)
	//}}AFX_VIRTUAL

protected:
$$IF(TASKPANE_PAGES)
	BOOL CreateTreeControl();
	BOOL CreateEditControl();
$$ENDIF

// Implementation
public:
	virtual ~$$TASKPANE_CLASS$$();

	// Generated message map functions
protected:
	//{{AFX_MSG($$TASKPANE_CLASS$$)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// $$INSERT_LOCATION_COMMENT$$

#endif // !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)

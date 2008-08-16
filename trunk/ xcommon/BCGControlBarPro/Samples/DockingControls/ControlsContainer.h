#if !defined(AFX_CONTROLSCONTAINER_H__17FEBF39_7F5C_4F07_A9D7_A30B0A463E58__INCLUDED_)
#define AFX_CONTROLSCONTAINER_H__17FEBF39_7F5C_4F07_A9D7_A30B0A463E58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ControlsContainer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CControlsContainer window

class CControlsContainer : public CWnd
{
// Construction
public:
	CControlsContainer();

// Attributes
public:
	CStatic		m_wndStatic;
	CListBox	m_wndList;
	CEdit		m_wndEdit;
	CButton		m_wndButton1;
	CButton		m_wndButton2;

	CFont		m_Font;
	int			m_nTextHeight;

// Operations
public:
	void Layout ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlsContainer)
	public:
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CControlsContainer();

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlsContainer)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROLSCONTAINER_H__17FEBF39_7F5C_4F07_A9D7_A30B0A463E58__INCLUDED_)

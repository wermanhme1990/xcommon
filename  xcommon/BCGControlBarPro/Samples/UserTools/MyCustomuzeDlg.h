#if !defined(AFX_MYCUSTOMUZEDLG_H__5788BA1E_F805_4EE7_B5D9_B2F4F1405F20__INCLUDED_)
#define AFX_MYCUSTOMUZEDLG_H__5788BA1E_F805_4EE7_B5D9_B2F4F1405F20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyCustomuzeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropsBtn window

class CSampleUserTool;

class CPropsBtn : public CButton
{
// Construction
public:
	CPropsBtn();

// Attributes
public:
	CSampleUserTool* m_pTool;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropsBtn)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropsBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropsBtn)
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMyCustomuzeDlg

class CMyCustomuzeDlg : public CBCGPToolbarCustomize
{
	DECLARE_DYNAMIC(CMyCustomuzeDlg)

// Construction
public:
	CMyCustomuzeDlg (CFrameWnd* pWndParentFrame);

// Attributes
protected:
	CStatic		m_wndParam1Label;
	CEdit		m_wndParam1;

	CPropsBtn	m_wndParam2;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCustomuzeDlg)
	//}}AFX_VIRTUAL

	virtual void OnInitToolsPage ();
	virtual void OnBeforeChangeTool (CBCGPUserTool* pSelTool);
	virtual void OnAfterChangeTool (CBCGPUserTool* pSelTool);
	virtual BOOL CheckToolsValidity (const CObList& lstTools);

// Implementation
public:
	virtual ~CMyCustomuzeDlg();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyCustomuzeDlg)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCUSTOMUZEDLG_H__5788BA1E_F805_4EE7_B5D9_B2F4F1405F20__INCLUDED_)

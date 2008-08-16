#if !defined(AFX_MSNDLG_H__99FA0F3E_B137_4F83_AF3B_04F1AFBAC34F__INCLUDED_)
#define AFX_MSNDLG_H__99FA0F3E_B137_4F83_AF3B_04F1AFBAC34F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MSNDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMSNDlg dialog

class CMSNDlg : public CBCGPPopupDlg
{
	DECLARE_DYNCREATE(CMSNDlg)

// Construction
public:
	CMSNDlg();

// Dialog Data
	//{{AFX_DATA(CMSNDlg)
	enum { IDD = IDD_DIALOG2 };
	CBCGPURLLinkButton	m_Options;
	CBCGPURLLinkButton	m_btnRL;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMSNDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	virtual void OnDraw (CDC* pDC);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMSNDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnOptions();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CBCGPToolBarImages	m_imgLogo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSNDLG_H__99FA0F3E_B137_4F83_AF3B_04F1AFBAC34F__INCLUDED_)

#if !defined(AFX_OPENPROFILEDLG_H__CC53E4BB_E3C6_4ABD_B6C3_B5D4D9E81E1C__INCLUDED_)
#define AFX_OPENPROFILEDLG_H__CC53E4BB_E3C6_4ABD_B6C3_B5D4D9E81E1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenProfileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpenProfileDlg dialog

class COpenProfileDlg : public CDialog
{
// Construction
public:
	COpenProfileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COpenProfileDlg)
	enum { IDD = IDD_OPEN_PROFILE_DLG };
	CListBox	m_lstListProfiles;
	//}}AFX_DATA

	CString GetProfileName () const {return m_strProfileName;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenProfileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COpenProfileDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListProfiles();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CString	m_strProfileName;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENPROFILEDLG_H__CC53E4BB_E3C6_4ABD_B6C3_B5D4D9E81E1C__INCLUDED_)

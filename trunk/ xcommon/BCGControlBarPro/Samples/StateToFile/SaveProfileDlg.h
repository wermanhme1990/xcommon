#if !defined(AFX_SAVEPROFILEDLG_H__2DBA5AD7_5277_4E04_A4B2_2F8AA7082CDA__INCLUDED_)
#define AFX_SAVEPROFILEDLG_H__2DBA5AD7_5277_4E04_A4B2_2F8AA7082CDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SaveProfileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSaveProfileDlg dialog

class CSaveProfileDlg : public CDialog
{
// Construction
public:
	CSaveProfileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSaveProfileDlg)
	enum { IDD = IDD_SAVE_PROFILE_DLG };
	CButton	m_wndOK;
	CListBox	m_lstProfiles;
	CString	m_strProfileName;
	CString	m_strCurSelProfileName;
	//}}AFX_DATA

	CString GetProfileName () const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaveProfileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSaveProfileDlg)
	afx_msg void OnSelchangeListProfiles();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateEditProfileName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVEPROFILEDLG_H__2DBA5AD7_5277_4E04_A4B2_2F8AA7082CDA__INCLUDED_)

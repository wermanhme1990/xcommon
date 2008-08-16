#if !defined(AFX_RECURRENCECONFIRMDLG_H__AB16AC34_774C_4CF0_89B0_00D1A4A43363__INCLUDED_)
#define AFX_RECURRENCECONFIRMDLG_H__AB16AC34_774C_4CF0_89B0_00D1A4A43363__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecurrenceConfirmDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRecurrenceConfirmDlg dialog

class CRecurrenceConfirmDlg : public CDialog
{
// Construction
public:
	CRecurrenceConfirmDlg(const CString& strDescription, BOOL bOpen,
		CWnd* pParent = NULL);   // standard constructor

	BOOL IsOnlyOccurrence () const
	{
		return m_bOccurrence;
	}
// Dialog Data
	//{{AFX_DATA(CRecurrenceConfirmDlg)
	CStatic	m_stIcon;
	CStatic	m_stQuestion;
	CButton m_btnOccurrence;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecurrenceConfirmDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	BOOL	m_bOccurrence;
	CString	m_strDescription;

	// Generated message map functions
	//{{AFX_MSG(CRecurrenceConfirmDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECURRENCECONFIRMDLG_H__AB16AC34_774C_4CF0_89B0_00D1A4A43363__INCLUDED_)

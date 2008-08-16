#if !defined(AFX_TABLEDLG_H__2DBD8CE2_098B_4EF4_9B64_73FE6C7B739F__INCLUDED_)
#define AFX_TABLEDLG_H__2DBD8CE2_098B_4EF4_9B64_73FE6C7B739F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TableDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTableDlg dialog

class CTableDlg : public CDialog
{
// Construction
public:
	CTableDlg(CStringList& lstTable, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTableDlg)
	enum { IDD = IDD_TABLE };
	CButton	m_btnOk;
	CListBox	m_wndTables;
	CString	m_strTable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTableDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTableDlg)
	afx_msg void OnDblclkTables();
	afx_msg void OnSelchangeTables();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CStringList& m_lstTable;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABLEDLG_H__2DBD8CE2_098B_4EF4_9B64_73FE6C7B739F__INCLUDED_)

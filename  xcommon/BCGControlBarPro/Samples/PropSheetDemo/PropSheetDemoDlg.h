// PropSheetDemoDlg.h : header file
//

#if !defined(AFX_PROPSHEETDEMODLG_H__F5A6E996_9BE9_4CD5_83BE_526B00914DCC__INCLUDED_)
#define AFX_PROPSHEETDEMODLG_H__F5A6E996_9BE9_4CD5_83BE_526B00914DCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPropSheetDemoDlg dialog

class CPropSheetDemoDlg : public CBCGPDialog
{
// Construction
public:
	CPropSheetDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPropSheetDemoDlg)
	enum { IDD = IDD_PROPSHEETDEMO_DIALOG };
	BOOL	m_bHeader;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropSheetDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPropSheetDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPropSheet1();
	afx_msg void OnPropSheet2();
	afx_msg void OnPropSheet3();
	afx_msg void OnPropSheet4();
	afx_msg void OnPropSheet5();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPSHEETDEMODLG_H__F5A6E996_9BE9_4CD5_83BE_526B00914DCC__INCLUDED_)

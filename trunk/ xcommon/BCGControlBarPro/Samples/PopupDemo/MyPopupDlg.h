#if !defined(AFX_MYPOPUPDLG_H__7AD08F23_CC5F_482D_B04F_7E62F3ABC1C9__INCLUDED_)
#define AFX_MYPOPUPDLG_H__7AD08F23_CC5F_482D_B04F_7E62F3ABC1C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyPopupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyPopupDlg dialog

class CMyPopupDlg : public CBCGPPopupDlg
{
	DECLARE_DYNCREATE(CMyPopupDlg)

// Construction
public:
	CMyPopupDlg();

// Dialog Data
	//{{AFX_DATA(CMyPopupDlg)
	enum { IDD = IDD_DIALOG1 };
	CStatic	m_wndFrom;
	CBCGPPopupWndButton	m_btnFlag;
	CBCGPPopupWndButton	m_btnDelete;
	CBCGPURLLinkButton	m_btnRL;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyPopupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyPopupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDelete();
	afx_msg void OnFlag();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPOPUPDLG_H__7AD08F23_CC5F_482D_B04F_7E62F3ABC1C9__INCLUDED_)

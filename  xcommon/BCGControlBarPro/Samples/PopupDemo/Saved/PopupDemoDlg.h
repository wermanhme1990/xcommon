// PopupDemoDlg.h : header file
//

#if !defined(AFX_POPUPDEMODLG_H__A4E66BB5_21ED_42BE_A61A_07DE7CB820BC__INCLUDED_)
#define AFX_POPUPDEMODLG_H__A4E66BB5_21ED_42BE_A61A_07DE7CB820BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPopupDemoDlg dialog

class CPopupDemoDlg : public CBCGPDialog
{
// Construction
public:
	CPopupDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPopupDemoDlg)
	enum { IDD = IDD_POPUPDEMO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPopupDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnShow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPUPDEMODLG_H__A4E66BB5_21ED_42BE_A61A_07DE7CB820BC__INCLUDED_)

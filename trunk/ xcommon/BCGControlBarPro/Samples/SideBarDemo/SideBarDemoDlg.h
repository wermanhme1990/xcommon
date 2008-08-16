// SideBarDemoDlg.h : header file
//

#if !defined(AFX_SIDEBARDEMODLG_H__70AB1E67_DF2C_4689_BE83_86ED75AD6DB7__INCLUDED_)
#define AFX_SIDEBARDEMODLG_H__70AB1E67_DF2C_4689_BE83_86ED75AD6DB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSideBarDemoDlg dialog

class CSideBarDemoDlg : public CBCGPDialog
{
// Construction
public:
	CSideBarDemoDlg(CWnd* pParent = NULL);	// standard constructor

	static CSideBarDemoDlg* GetActiveDlg ()
	{
		return m_pActiveDlg;
	}

// Dialog Data
	//{{AFX_DATA(CSideBarDemoDlg)
	enum { IDD = IDD_SIDEBARDEMO_DIALOG };
	int		m_nLocation;
	int		m_nLook;
	BOOL	m_bIsAlwaysOnTop;
	BOOL	m_bIsAutoHide;
	BOOL	m_bShowOnStartup;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSideBarDemoDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSideBarDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnApply();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	static CSideBarDemoDlg*	m_pActiveDlg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIDEBARDEMODLG_H__70AB1E67_DF2C_4689_BE83_86ED75AD6DB7__INCLUDED_)

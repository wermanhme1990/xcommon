#if !defined(AFX_CSTM1DLG_H__FDAF082B_F004_11D2_A712_0090274409AC__INCLUDED_)
#define AFX_CSTM1DLG_H__FDAF082B_F004_11D2_A712_0090274409AC__INCLUDED_

// cstm1dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustom1Dlg dialog

class CCustom1Dlg : public CAppWizStepDlg
{
// Construction
public:
	CCustom1Dlg();
	virtual BOOL OnDismiss();

// Dialog Data
	//{{AFX_DATA(CCustom1Dlg)
	enum { IDD = IDD_CUSTOM1 };
	CButton	m_btnWindowsDlg;
	CStatic	m_wndCaption;
	CStatic	m_wndImage;
	BOOL	m_bMenuBar;
	int		m_nBarsLook;
	BOOL	m_bStaticLink;
	BOOL	m_b256Colors;
	BOOL	m_bHotTrack;
	BOOL	m_bMDITabs;
	BOOL	m_bWindowsDlg;
	BOOL	m_bMDITabGroups;
	//}}AFX_DATA

	CBitmap	m_Bmp;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustom1Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCustom1Dlg)
	afx_msg void OnColor256();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnMdiTabs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void SetBoolVar (LPCTSTR lpszName, BOOL bVal);
	void EnableControls ();

	CRect m_rectCaption;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSTM1DLG_H__FDAF082B_F004_11D2_A712_0090274409AC__INCLUDED_)

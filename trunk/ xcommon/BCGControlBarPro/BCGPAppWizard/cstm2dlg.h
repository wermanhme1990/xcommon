#if !defined(AFX_CSTM2DLG_H__FDAF082B_F004_11D2_A712_0090274409AC__INCLUDED_)
#define AFX_CSTM2DLG_H__FDAF082B_F004_11D2_A712_0090274409AC__INCLUDED_

// cstm1dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustom2Dlg dialog

class CCustom2Dlg : public CAppWizStepDlg
{
// Construction
public:
	CCustom2Dlg();
	virtual BOOL OnDismiss();

// Dialog Data
	//{{AFX_DATA(CCustom2Dlg)
	enum { IDD = IDD_CUSTOM2 };
	CStatic	m_wndCaption;
	CStatic	m_wndImage;
	BOOL	m_bOutputBar;
	BOOL	m_bWorkspaceBar;
	BOOL	m_bCaptionBar;
	BOOL	m_bOutlookBar;
	BOOL	m_bOutlookEmbeddedControl;
	BOOL	m_bOutlookBackground;
	BOOL	m_bAutoHide;
	BOOL	m_bTaskPane;
	BOOL	m_bTaskPanePages;
	int		m_nDockingStyle;
	//}}AFX_DATA

	CBitmap	m_Bmp;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustom2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCustom2Dlg)
	afx_msg void OnCustomization();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnOutlookBar();
	afx_msg void OnPaint();
	afx_msg void OnRightBar();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void SetBoolVar (LPCTSTR lpszName, BOOL bVal);
	void EnableControls ();

	CRect	m_rectCaption;
	BOOL	m_bRebar;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSTM2DLG_H__FDAF082B_F004_11D2_A712_0090274409AC__INCLUDED_)

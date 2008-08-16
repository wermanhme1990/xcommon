#if !defined(AFX_CSTM3DLG_H__FDAF082B_F004_11D2_A712_0090274409AC__INCLUDED_)
#define AFX_CSTM3DLG_H__FDAF082B_F004_11D2_A712_0090274409AC__INCLUDED_

// cstm3dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustom3Dlg dialog

class CCustom3Dlg : public CAppWizStepDlg
{
// Construction
public:
	CCustom3Dlg();
	virtual BOOL OnDismiss();

// Dialog Data
	//{{AFX_DATA(CCustom3Dlg)
	enum { IDD = IDD_CUSTOM3 };
	CStatic	m_wndCaption;
	CStatic	m_wndImage;
	BOOL	m_bCustomization;
	BOOL	m_bKeyboard;
	BOOL	m_bMouse;
	BOOL	m_bContextMenuCustomization;
	BOOL	m_bUserToolbars;
	BOOL	m_bUserImages;
	BOOL	m_bAltCustomization;
	BOOL	m_bRUMenus;
	BOOL	m_bUserTools;
	//}}AFX_DATA

	CBitmap	m_Bmp;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustom3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCustom3Dlg)
	afx_msg void OnCustomization();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void SetBoolVar (LPCTSTR lpszName, BOOL bVal);
	void EnableControls ();

	BOOL	m_bRebar;
	BOOL	m_bMenuBar;

	CRect	m_rectCaption;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSTM3DLG_H__FDAF082B_F004_11D2_A712_0090274409AC__INCLUDED_)

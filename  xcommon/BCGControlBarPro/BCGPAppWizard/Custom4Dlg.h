#if !defined(AFX_CUSTOM4DLG_H__CC8313E6_DACD_4874_806B_554B97CE3025__INCLUDED_)
#define AFX_CUSTOM4DLG_H__CC8313E6_DACD_4874_806B_554B97CE3025__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Custom4Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustom4Dlg dialog

class CCustom4Dlg : public CAppWizStepDlg
{
// Construction
public:
	CCustom4Dlg();

// Dialog Data
	//{{AFX_DATA(CCustom4Dlg)
	enum { IDD = IDD_CUSTOM4 };
	CButton	m_wndSkinManager;
	CEdit	m_wndSkinURL;
	CStatic	m_wndImage;
	CEdit	m_wndSkinsFolder;
	CButton	m_wndDownloadSkins;
	CStatic	m_wndCaption;
	CButton	m_btnBrowseSkinsFolder;
	BOOL	m_bDownloadSkins;
	BOOL	m_bSkinsManager;
	int		m_nSkinsDir;
	CString	m_strSkinsFolder;
	CString	m_strSkinURL;
	BOOL	m_bVisualManager;
	//}}AFX_DATA

	void SetBoolVar (LPCTSTR lpszName, BOOL bVal);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustom4Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	virtual BOOL OnDismiss();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCustom4Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnUpdateControls();
	afx_msg void OnBrowseSkinsFolder();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CBitmap	m_Bmp;
	CRect	m_rectCaption;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOM4DLG_H__CC8313E6_DACD_4874_806B_554B97CE3025__INCLUDED_)

// PopupDemoDlg.h : header file
//

#if !defined(AFX_POPUPDEMODLG_H__A4E66BB5_21ED_42BE_A61A_07DE7CB820BC__INCLUDED_)
#define AFX_POPUPDEMODLG_H__A4E66BB5_21ED_42BE_A61A_07DE7CB820BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IconComboBox.h"

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
	CSliderCtrl	m_wndAutoCloseTime;
	CEdit	m_wndText;
	CEdit	m_wndLink;
	CStatic	m_wndLabel3;
	CStatic	m_wndLabel2;
	CStatic	m_wndLabel1;
	CIconComboBox	m_wndIcons;
	CSliderCtrl	m_wndTransparency;
	CSliderCtrl	m_wndANimationSpeed;
	int		m_nVisualMngr;
	int		m_nAnimation;
	int		m_nAnimationSpeed;
	CString	m_strAnimSpeed;
	int		m_nTransparency;
	CString	m_strTransparency;
	BOOL	m_bSmallCaption;
	int		m_nPopupSource;
	int		m_nIcon;
	CString	m_strLink;
	CString	m_strText;
	int		m_nAutoCloseTime;
	CString	m_strAutoClose;
	BOOL	m_bAutoClose;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON				m_hIcon;
	CMenu				m_menuPopup;
	CPoint				m_ptPopup;
	CBCGPToolBarImages	m_Icons;
	CBCGPToolBarImages	m_IconsSmall;

	// Generated message map functions
	//{{AFX_MSG(CPopupDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnShow();
	afx_msg void OnSelchangeVisualMngr();
	afx_msg void OnSelchangeAnimation();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSmallCaption();
	afx_msg void OnPopupSource();
	afx_msg void OnAutoClose();
	//}}AFX_MSG
	afx_msg LRESULT OnClosePopup(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()

	void EnableControls (BOOL bEnable = TRUE);
	void EnableSourceControls ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPUPDEMODLG_H__A4E66BB5_21ED_42BE_A61A_07DE7CB820BC__INCLUDED_)

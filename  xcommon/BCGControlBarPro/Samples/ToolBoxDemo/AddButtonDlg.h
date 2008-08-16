#if !defined(AFX_ADDBUTTONDLG_H__79C713FC_3BD2_45EB_876A_60F8AD5C542D__INCLUDED_)
#define AFX_ADDBUTTONDLG_H__79C713FC_3BD2_45EB_876A_60F8AD5C542D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddButtonDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIconCombo window

class CIconCombo : public CComboBox
{
// Construction
public:
	CIconCombo (CBCGPToolBarImages& imageListIcons);
	
// Attributes
protected:
	CBCGPToolBarImages&	m_imageListIcons;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIconCombo)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIconCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIconCombo)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CAddButtonDlg dialog

class CAddButtonDlg : public CDialog
{
// Construction
public:
	CAddButtonDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddButtonDlg)
	enum { IDD = IDD_ADD_BUTTON };
	CEdit	m_wndLabel;
	CIconCombo	m_wndIconList;
	CSpinButtonCtrl	m_wndSpin;
	int		m_nPage;
	CString	m_strLabel;
	//}}AFX_DATA

	HICON GetSelIcon ()
	{
		return m_imageList.ExtractIcon (m_nSelImage);
	}


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddButtonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddButtonDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CBCGPToolBarImages  m_imageList;
	int					m_nSelImage;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDBUTTONDLG_H__79C713FC_3BD2_45EB_876A_60F8AD5C542D__INCLUDED_)

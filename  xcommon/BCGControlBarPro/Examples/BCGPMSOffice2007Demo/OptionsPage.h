#if !defined(AFX_OPTIONSPAGE_H__31F01121_D253_4175_9CD9_69615717107E__INCLUDED_)
#define AFX_OPTIONSPAGE_H__31F01121_D253_4175_9CD9_69615717107E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsPage dialog

class COptionsPage : public CBCGPPropertyPage
{
	DECLARE_DYNCREATE(COptionsPage)

// Construction
public:
	COptionsPage();
	~COptionsPage();

// Dialog Data
	//{{AFX_DATA(COptionsPage)
	enum { IDD = IDD_POPULAR_PAGE };
	CComboBox	m_wndColorScheme;
	int		m_nColorScheme;
	int		m_nTooltipStyle;
	BOOL	m_bShowDevTab;
	BOOL	m_bShowFloaty;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

class COptionsPropSheet : public CBCGPPropertySheet
{
public:
	COptionsPropSheet (CWnd* pWndParent, UINT nSelectedPage = 0);

protected:
	virtual void OnDrawPageHeader (CDC* pDC, int nPage, CRect rectHeader);

	CBCGPToolBarImages	m_Icons;
	CBCGPControlRenderer m_Pat[4];
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSPAGE_H__31F01121_D253_4175_9CD9_69615717107E__INCLUDED_)

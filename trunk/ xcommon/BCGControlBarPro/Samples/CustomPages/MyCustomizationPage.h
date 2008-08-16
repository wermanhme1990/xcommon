#if !defined(AFX_MYCUSTOMIZATIONPAGE_H__9127CCC9_5371_4618_BB27_6FC8ACC07A82__INCLUDED_)
#define AFX_MYCUSTOMIZATIONPAGE_H__9127CCC9_5371_4618_BB27_6FC8ACC07A82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyCustomizationPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyCustomizationPage dialog

class CMyCustomizationPage : public CPropertyPage
{
	DECLARE_DYNCREATE (CMyCustomizationPage)
// Construction
public:
	CMyCustomizationPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMyCustomizationPage)
	enum { IDD = IDD_CUST_PAGE_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCustomizationPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyCustomizationPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCUSTOMIZATIONPAGE_H__9127CCC9_5371_4618_BB27_6FC8ACC07A82__INCLUDED_)

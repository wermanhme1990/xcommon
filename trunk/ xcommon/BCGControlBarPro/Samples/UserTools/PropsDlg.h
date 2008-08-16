#if !defined(AFX_PROPSDLG_H__FCF266BC_8BD2_4810_B760_F91675CFCF89__INCLUDED_)
#define AFX_PROPSDLG_H__FCF266BC_8BD2_4810_B760_F91675CFCF89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropsDlg dialog

class CPropsDlg : public CDialog
{
// Construction
public:
	CPropsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropsDlg)
	enum { IDD = IDD_PROPS };
	CString	m_strProp2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropsDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPSDLG_H__FCF266BC_8BD2_4810_B760_F91675CFCF89__INCLUDED_)

#if !defined(AFX_PRINTTYPEDLG_H__716A17A5_FF51_467C_8C8E_B5BDF97ECD87__INCLUDED_)
#define AFX_PRINTTYPEDLG_H__716A17A5_FF51_467C_8C8E_B5BDF97ECD87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrintTypeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrintTypeDlg dialog

class CPrintTypeDlg : public CDialog
{
// Construction
public:
	CPrintTypeDlg(CWnd* pParent = NULL);   // standard constructor

	void SetType (int type);
	int GetType () const
	{
		return m_Type;
	}

private:
// Dialog Data
	//{{AFX_DATA(CPrintTypeDlg)
	enum { IDD = IDD_PRNTYPE };
	CButton m_Radio1;
	CButton m_Radio2;
	CButton m_Radio3;
	CButton m_Radio4;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrintTypeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_Type;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTTYPEDLG_H__716A17A5_FF51_467C_8C8E_B5BDF97ECD87__INCLUDED_)

#if !defined(AFX_RECURRENCERULEMONTHLYDLG_H__7DF2382B_9A0E_4AD6_AC24_E077A4FBBC12__INCLUDED_)
#define AFX_RECURRENCERULEMONTHLYDLG_H__7DF2382B_9A0E_4AD6_AC24_E077A4FBBC12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecurrenceRuleMonthlyDlg.h : header file
//

#include "RecurrenceRuleDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CRecurrenceRuleMonthlyDlg dialog

class CRecurrenceRuleMonthlyDlg : public CRecurrenceRuleDlg
{
// Construction
public:
	CRecurrenceRuleMonthlyDlg();   // standard constructor

	virtual void SetRule (CBCGPRecurrenceBaseRule* pRule);

	virtual BOOL UpdateRule (const COleDateTime& dtStart);

// Dialog Data
	//{{AFX_DATA(CRecurrenceRuleMonthlyDlg)
	enum { IDD = IDD_RECURRENCE_RULE_MONTHLY };
	CComboBox	m_cbDayTypeWeekDay;
	CButton m_btnTypeDay;
	CButton m_btnTypeDayType;
	DWORD	m_dwDay;
	DWORD	m_dwDaySpan;
	DWORD	m_dwDayTypeSpan;
	int		m_nDayTypeWeekDay;
	int		m_nDayTypeDay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecurrenceRuleMonthlyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	virtual UINT GetID () const
	{
		return IDD;
	}

	// Generated message map functions
	//{{AFX_MSG(CRecurrenceRuleMonthlyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTypeDay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECURRENCERULEMONTHLYDLG_H__7DF2382B_9A0E_4AD6_AC24_E077A4FBBC12__INCLUDED_)

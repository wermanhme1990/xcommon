#if !defined(AFX_APPOINTMENTDLG_H__57DADDA2_DC8A_4BF0_AB01_2A475BD9F962__INCLUDED_)
#define AFX_APPOINTMENTDLG_H__57DADDA2_DC8A_4BF0_AB01_2A475BD9F962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AppointmentDlg.h : header file
//

#include "ColorComboBox.h"
#include "BCGPRecurrenceRule.h"

/////////////////////////////////////////////////////////////////////////////
// CAppointmentDlg dialog

class CAppointmentDlg : public CDialog
{
// Construction
public:
	CAppointmentDlg(CBCGPAppointment& app, 
					const CBCGPRecurrenceBaseRule* pRule = NULL,
					BOOL bEcp = FALSE,
					BOOL bNewRecur = FALSE,
					BOOL bReadOnly = FALSE,
					CWnd* pParent = NULL);   // standard constructor
	
	virtual ~CAppointmentDlg ();

// Dialog Data
	//{{AFX_DATA(CAppointmentDlg)
	enum { IDD = IDD_APP_EDIT_DLG };
	CEdit	m_EditMemo;
	CEdit	m_EditLocation;
	CButton	m_Recurrence;
	CButton m_AllDay;
	CStatic	m_stDateTimeStart;
	CStatic	m_stDateTimeEnd;
	CColorComboBox	m_wndShowTime;
	CColorComboBox	m_wndLabel;
	CBCGPDateTimeCtrl	m_DateStart;
	CBCGPDateTimeCtrl	m_TimeStart;
	CBCGPDateTimeCtrl	m_DateEnd;
	CBCGPDateTimeCtrl	m_TimeEnd;
	CString	m_strSubject;
	CString	m_strLocation;
	BOOL	m_bAllDay;
	BOOL	m_bMeeting;
	BOOL	m_bPrivate;
	BOOL	m_bPhoneCall;
	int		m_nLabel;
	int		m_nShowTime;
	CString	m_strMemo;
	//}}AFX_DATA

	const CBCGPRecurrenceBaseRule* GetRule () const
	{
		return m_Rule;
	}

	BOOL IsDeleted () const
	{
		return m_bDeleted;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppointmentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAppointmentDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBtnRecurrence();
	afx_msg void OnBtnAllDay();
	afx_msg void OnBtnDelete();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CBCGPAppointment& m_Appointment;
	CBCGPRecurrenceBaseRule* m_Rule;
	BOOL	m_bEcp;
	BOOL	m_bNewRecur;
	BOOL	m_bReadOnly;
	BOOL	m_bDeleted;

	void EnableDateControls (BOOL bEnable);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPOINTMENTDLG_H__57DADDA2_DC8A_4BF0_AB01_2A475BD9F962__INCLUDED_)

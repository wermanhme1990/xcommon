// ToolbarDateTimePicker.h : main header file for the TOOLBARDATETIMEPICKER application
//

#if !defined(AFX_TOOLBARDATETIMEPICKER_H__4B2F9451_37DA_46B8_B199_0D4496B694E4__INCLUDED_)
#define AFX_TOOLBARDATETIMEPICKER_H__4B2F9451_37DA_46B8_B199_0D4496B694E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CToolbarDateTimePickerApp:
// See ToolbarDateTimePicker.cpp for the implementation of this class
//

class CToolbarDateTimePickerApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CToolbarDateTimePickerApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolbarDateTimePickerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CToolbarDateTimePickerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CToolbarDateTimePickerApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARDATETIMEPICKER_H__4B2F9451_37DA_46B8_B199_0D4496B694E4__INCLUDED_)

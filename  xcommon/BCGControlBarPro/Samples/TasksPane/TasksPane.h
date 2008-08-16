// TasksPane.h : main header file for the TASKSPANE application
//

#if !defined(AFX_TASKSPANE_H__FF6D5B95_3603_4568_9308_2DA38B88ABFE__INCLUDED_)
#define AFX_TASKSPANE_H__FF6D5B95_3603_4568_9308_2DA38B88ABFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTasksPaneApp:
// See TasksPane.cpp for the implementation of this class
//

class CTasksPaneApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CTasksPaneApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTasksPaneApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTasksPaneApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CTasksPaneApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKSPANE_H__FF6D5B95_3603_4568_9308_2DA38B88ABFE__INCLUDED_)

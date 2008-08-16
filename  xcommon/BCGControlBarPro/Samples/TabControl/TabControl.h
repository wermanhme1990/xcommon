// TabControl.h : main header file for the TABCONTROL application
//

#if !defined(AFX_TABCONTROL_H__35299760_5D7D_4C0D_9457_17A0F9CCCDD4__INCLUDED_)
#define AFX_TABCONTROL_H__35299760_5D7D_4C0D_9457_17A0F9CCCDD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTabControlApp:
// See TabControl.cpp for the implementation of this class
//

class CTabControlApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CTabControlApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabControlApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTabControlApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CTabControlApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCONTROL_H__35299760_5D7D_4C0D_9457_17A0F9CCCDD4__INCLUDED_)

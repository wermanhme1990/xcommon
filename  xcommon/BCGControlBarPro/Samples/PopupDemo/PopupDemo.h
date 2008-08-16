// PopupDemo.h : main header file for the POPUPDEMO application
//

#if !defined(AFX_POPUPDEMO_H__496BDD98_FD21_4BB4_BA13_C9D09B17FAF3__INCLUDED_)
#define AFX_POPUPDEMO_H__496BDD98_FD21_4BB4_BA13_C9D09B17FAF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPopupDemoApp:
// See PopupDemo.cpp for the implementation of this class
//

class CPopupDemoApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CPopupDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPopupDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CPopupDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPUPDEMO_H__496BDD98_FD21_4BB4_BA13_C9D09B17FAF3__INCLUDED_)

// WorkSpaceToolBar.h : main header file for the WORKSPACETOOLBAR application
//

#if !defined(AFX_WORKSPACETOOLBAR_H__FC00CAB6_FE24_4131_B711_40CFCCF240AA__INCLUDED_)
#define AFX_WORKSPACETOOLBAR_H__FC00CAB6_FE24_4131_B711_40CFCCF240AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceToolBarApp:
// See WorkSpaceToolBar.cpp for the implementation of this class
//

class CWorkSpaceToolBarApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CWorkSpaceToolBarApp();

	// Override from CBCGWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkSpaceToolBarApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CWorkSpaceToolBarApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CWorkSpaceToolBarApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKSPACETOOLBAR_H__FC00CAB6_FE24_4131_B711_40CFCCF240AA__INCLUDED_)

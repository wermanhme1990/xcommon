// StatusBarDemo.h : main header file for the STATUSBARDEMO application
//

#if !defined(AFX_STATUSBARDEMO_H__5B46A59B_B676_4DF5_86FB_172219C9D631__INCLUDED_)
#define AFX_STATUSBARDEMO_H__5B46A59B_B676_4DF5_86FB_172219C9D631__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CStatusBarDemoApp:
// See StatusBarDemo.cpp for the implementation of this class
//

class CStatusBarDemoApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CStatusBarDemoApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatusBarDemoApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CStatusBarDemoApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CStatusBarDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATUSBARDEMO_H__5B46A59B_B676_4DF5_86FB_172219C9D631__INCLUDED_)

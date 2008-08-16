// SmartDockingDemo.h : main header file for the SMARTDOCKINGDEMO application
//

#if !defined(AFX_SMARTDOCKINGDEMO_H__3CC35B61_9C34_4071_9BFD_DDD7BFF4DDEE__INCLUDED_)
#define AFX_SMARTDOCKINGDEMO_H__3CC35B61_9C34_4071_9BFD_DDD7BFF4DDEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSmartDockingDemoApp:
// See SmartDockingDemo.cpp for the implementation of this class
//

class CSmartDockingDemoApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CSmartDockingDemoApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmartDockingDemoApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSmartDockingDemoApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CSmartDockingDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMARTDOCKINGDEMO_H__3CC35B61_9C34_4071_9BFD_DDD7BFF4DDEE__INCLUDED_)

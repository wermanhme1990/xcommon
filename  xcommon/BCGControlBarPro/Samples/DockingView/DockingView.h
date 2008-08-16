// DockingView.h : main header file for the DOCKINGVIEW application
//

#if !defined(AFX_DOCKINGVIEW_H__19FFFB40_C67B_4B7C_A108_2DCEFB81C416__INCLUDED_)
#define AFX_DOCKINGVIEW_H__19FFFB40_C67B_4B7C_A108_2DCEFB81C416__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDockingViewApp:
// See DockingView.cpp for the implementation of this class
//

class CDockingViewApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CDockingViewApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDockingViewApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDockingViewApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CDockingViewApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOCKINGVIEW_H__19FFFB40_C67B_4B7C_A108_2DCEFB81C416__INCLUDED_)

// MDICustomBackground.h : main header file for the MDICUSTOMBACKGROUND application
//

#if !defined(AFX_MDICUSTOMBACKGROUND_H__F79B43FE_4547_4947_919A_CF2A2698B4B7__INCLUDED_)
#define AFX_MDICUSTOMBACKGROUND_H__F79B43FE_4547_4947_919A_CF2A2698B4B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMDICustomBackgroundApp:
// See MDICustomBackground.cpp for the implementation of this class
//

class CMDICustomBackgroundApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CMDICustomBackgroundApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDICustomBackgroundApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL SaveAllModified();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMDICustomBackgroundApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CMDICustomBackgroundApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDICUSTOMBACKGROUND_H__F79B43FE_4547_4947_919A_CF2A2698B4B7__INCLUDED_)

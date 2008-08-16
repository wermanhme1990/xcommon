// ColorThemeToolbar.h : main header file for the COLORTHEMETOOLBAR application
//

#if !defined(AFX_COLORTHEMETOOLBAR_H__DB7C0C6F_A919_4D4B_8B9C_630C8A13688B__INCLUDED_)
#define AFX_COLORTHEMETOOLBAR_H__DB7C0C6F_A919_4D4B_8B9C_630C8A13688B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CColorThemeToolbarApp:
// See ColorThemeToolbar.cpp for the implementation of this class
//

class CColorThemeToolbarApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CColorThemeToolbarApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorThemeToolbarApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CColorThemeToolbarApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CColorThemeToolbarApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORTHEMETOOLBAR_H__DB7C0C6F_A919_4D4B_8B9C_630C8A13688B__INCLUDED_)

// UserTools.h : main header file for the USERTOOLS application
//

#if !defined(AFX_USERTOOLS_H__0E423F58_77E5_4524_8074_26C89362994F__INCLUDED_)
#define AFX_USERTOOLS_H__0E423F58_77E5_4524_8074_26C89362994F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CUserToolsApp:
// See UserTools.cpp for the implementation of this class
//

class CUserToolsApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CUserToolsApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserToolsApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CUserToolsApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CUserToolsApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERTOOLS_H__0E423F58_77E5_4524_8074_26C89362994F__INCLUDED_)

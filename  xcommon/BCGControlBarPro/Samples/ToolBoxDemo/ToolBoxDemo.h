// ToolBoxDemo.h : main header file for the TOOLBOXDEMO application
//

#if !defined(AFX_TOOLBOXDEMO_H__774039D4_C028_41A0_A011_F6A878D53CD1__INCLUDED_)
#define AFX_TOOLBOXDEMO_H__774039D4_C028_41A0_A011_F6A878D53CD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CToolBoxDemoApp:
// See ToolBoxDemo.cpp for the implementation of this class
//

class CToolBoxDemoApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CToolBoxDemoApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBoxDemoApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CToolBoxDemoApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CToolBoxDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBOXDEMO_H__774039D4_C028_41A0_A011_F6A878D53CD1__INCLUDED_)

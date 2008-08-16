// DBGridDemo.h : main header file for the DBGRIDDEMO application
//

#if !defined(AFX_DBGRIDDEMO_H__BFDC1005_50D5_42CF_904B_AACA651A2722__INCLUDED_)
#define AFX_DBGRIDDEMO_H__BFDC1005_50D5_42CF_904B_AACA651A2722__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDBGridDemoApp:
// See DBGridDemo.cpp for the implementation of this class
//

class CDBGridDemoApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CDBGridDemoApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDBGridDemoApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDBGridDemoApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CDBGridDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBGRIDDEMO_H__BFDC1005_50D5_42CF_904B_AACA651A2722__INCLUDED_)

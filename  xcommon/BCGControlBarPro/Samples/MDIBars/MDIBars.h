// MDIBars.h : main header file for the MDIBARS application
//

#if !defined(AFX_MDIBARS_H__5FB564E6_0998_11D5_95C8_00A0C9289F1B__INCLUDED_)
#define AFX_MDIBARS_H__5FB564E6_0998_11D5_95C8_00A0C9289F1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMDIBarsApp:
// See MDIBars.cpp for the implementation of this class
//

class CMDIBarsApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CMDIBarsApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDIBarsApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMDIBarsApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CMDIBarsApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDIBARS_H__5FB564E6_0998_11D5_95C8_00A0C9289F1B__INCLUDED_)
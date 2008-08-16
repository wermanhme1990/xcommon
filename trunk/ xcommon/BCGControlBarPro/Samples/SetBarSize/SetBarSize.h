// SetBarSize.h : main header file for the SETBARSIZE application
//

#if !defined(AFX_SETBARSIZE_H__6B66C42D_1913_4C06_B082_8827FBD725B9__INCLUDED_)
#define AFX_SETBARSIZE_H__6B66C42D_1913_4C06_B082_8827FBD725B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSetBarSizeApp:
// See SetBarSize.cpp for the implementation of this class
//

class CSetBarSizeApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CSetBarSizeApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetBarSizeApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSetBarSizeApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CSetBarSizeApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETBARSIZE_H__6B66C42D_1913_4C06_B082_8827FBD725B9__INCLUDED_)

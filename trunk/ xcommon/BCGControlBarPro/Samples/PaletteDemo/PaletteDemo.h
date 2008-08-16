// PaletteDemo.h : main header file for the PALETTEDEMO application
//

#if !defined(AFX_PALETTEDEMO_H__3F9B3866_0A56_11D5_95C8_00A0C9289F1B__INCLUDED_)
#define AFX_PALETTEDEMO_H__3F9B3866_0A56_11D5_95C8_00A0C9289F1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPaletteDemoApp:
// See PaletteDemo.cpp for the implementation of this class
//

class CPaletteDemoApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CPaletteDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaletteDemoApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CPaletteDemoApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CPaletteDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PALETTEDEMO_H__3F9B3866_0A56_11D5_95C8_00A0C9289F1B__INCLUDED_)

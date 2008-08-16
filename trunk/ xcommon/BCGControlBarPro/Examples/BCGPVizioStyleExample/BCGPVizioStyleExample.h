// BCGPVizioStyleExample.h : main header file for the BCGPVIZIOSTYLEEXAMPLE application
//

#if !defined(AFX_BCGPVIZIOSTYLEEXAMPLE_H__B25C27B4_961F_4FA4_9197_CFC0B00A57CF__INCLUDED_)
#define AFX_BCGPVIZIOSTYLEEXAMPLE_H__B25C27B4_961F_4FA4_9197_CFC0B00A57CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CBCGPVizioStyleExampleApp:
// See BCGPVizioStyleExample.cpp for the implementation of this class
//

class CBCGPVizioStyleExampleApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CBCGPVizioStyleExampleApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPVizioStyleExampleApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CBCGPVizioStyleExampleApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CBCGPVizioStyleExampleApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPVIZIOSTYLEEXAMPLE_H__B25C27B4_961F_4FA4_9197_CFC0B00A57CF__INCLUDED_)

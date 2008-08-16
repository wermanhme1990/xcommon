// DrawEffects.h : main header file for the DRAWEFFECTS application
//

#if !defined(AFX_DRAWEFFECTS_H__1A73F505_C1F5_46DD_9E96_D4B01D954E8B__INCLUDED_)
#define AFX_DRAWEFFECTS_H__1A73F505_C1F5_46DD_9E96_D4B01D954E8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDrawEffectsApp:
// See DrawEffects.cpp for the implementation of this class
//

class CDrawEffectsApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CDrawEffectsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawEffectsApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDrawEffectsApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CDrawEffectsApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWEFFECTS_H__1A73F505_C1F5_46DD_9E96_D4B01D954E8B__INCLUDED_)

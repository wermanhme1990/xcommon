// BCGPIEDemo.h : main header file for the BCGPIEDemo application
//

#if !defined(AFX_BCGPIEDEMO_H__F83C8E0B_F33E_11D2_A713_0090274409AC__INCLUDED_)
#define AFX_BCGPIEDEMO_H__F83C8E0B_F33E_11D2_A713_0090274409AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "FavoritesManager.h"

/////////////////////////////////////////////////////////////////////////////
// CBCGPIEDemoApp:
// See BCGPIEDemo.cpp for the implementation of this class
//

class CBCGPIEDemoApp :	public CWinApp,
						public CBCGPWorkspace
{
public:
	CBCGPIEDemoApp();

	BOOL				m_bBackgroundImage;
	CFavoritesManager	m_Favorites;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPIEDemoApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	virtual void LoadCustomState ();
	virtual void SaveCustomState ();

// Implementation
	//{{AFX_MSG(CBCGPIEDemoApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CBCGPIEDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPIEDEMO_H__F83C8E0B_F33E_11D2_A713_0090274409AC__INCLUDED_)

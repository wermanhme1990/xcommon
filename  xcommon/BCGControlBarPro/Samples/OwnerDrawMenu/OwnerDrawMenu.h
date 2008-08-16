// OwnerDrawMenu.h : main header file for the OWNERDRAWMENU application
//

#if !defined(AFX_OWNERDRAWMENU_H__F044FDAB_BD99_4A87_B787_E8D61129D56F__INCLUDED_)
#define AFX_OWNERDRAWMENU_H__F044FDAB_BD99_4A87_B787_E8D61129D56F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// COwnerDrawMenuApp:
// See OwnerDrawMenu.cpp for the implementation of this class
//

class COwnerDrawMenuApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	COwnerDrawMenuApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COwnerDrawMenuApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(COwnerDrawMenuApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern COwnerDrawMenuApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OWNERDRAWMENU_H__F044FDAB_BD99_4A87_B787_E8D61129D56F__INCLUDED_)

// DynamicMenu.h : main header file for the DYNAMICMENU application
//

#if !defined(AFX_DYNAMICMENU_H__63400581_7E6A_4673_9DBB_B56121712F68__INCLUDED_)
#define AFX_DYNAMICMENU_H__63400581_7E6A_4673_9DBB_B56121712F68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDynamicMenuApp:
// See DynamicMenu.cpp for the implementation of this class
//

class CDynamicMenuApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CDynamicMenuApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynamicMenuApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDynamicMenuApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CDynamicMenuApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DYNAMICMENU_H__63400581_7E6A_4673_9DBB_B56121712F68__INCLUDED_)

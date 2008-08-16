// StateCollection.h : main header file for the STATECOLLECTION application
//

#if !defined(AFX_STATECOLLECTION_H__0CCA1C1B_EFC8_4A09_89E0_375046B3D6E1__INCLUDED_)
#define AFX_STATECOLLECTION_H__0CCA1C1B_EFC8_4A09_89E0_375046B3D6E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CStateCollectionApp:
// See StateCollection.cpp for the implementation of this class
//

class CStateCollectionApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CStateCollectionApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStateCollectionApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CStateCollectionApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CStateCollectionApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATECOLLECTION_H__0CCA1C1B_EFC8_4A09_89E0_375046B3D6E1__INCLUDED_)

// CustomPages.h : main header file for the CUSTOMPAGES application
//

#if !defined(AFX_CUSTOMPAGES_H__EAD48310_3498_4ACA_855E_C52A1AA06AAB__INCLUDED_)
#define AFX_CUSTOMPAGES_H__EAD48310_3498_4ACA_855E_C52A1AA06AAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCustomPagesApp:
// See CustomPages.cpp for the implementation of this class
//

class CCustomPagesApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CCustomPagesApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomPagesApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCustomPagesApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CCustomPagesApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMPAGES_H__EAD48310_3498_4ACA_855E_C52A1AA06AAB__INCLUDED_)

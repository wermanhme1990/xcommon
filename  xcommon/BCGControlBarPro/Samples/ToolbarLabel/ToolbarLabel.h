// ToolbarLabel.h : main header file for the TOOLBARLABEL application
//

#if !defined(AFX_TOOLBARLABEL_H__BF50CA37_EAE0_497D_ABE2_6EEE547653C0__INCLUDED_)
#define AFX_TOOLBARLABEL_H__BF50CA37_EAE0_497D_ABE2_6EEE547653C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CToolbarLabelApp:
// See ToolbarLabel.cpp for the implementation of this class
//

class CToolbarLabelApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CToolbarLabelApp();

	// Override from CBCGWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolbarLabelApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CToolbarLabelApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CToolbarLabelApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARLABEL_H__BF50CA37_EAE0_497D_ABE2_6EEE547653C0__INCLUDED_)

// TooltipDemo.h : main header file for the TOOLTIPDEMO application
//

#if !defined(AFX_TOOLTIPDEMO_H__F55DF02E_A8D1_4C71_A4D6_03DA7B2F6620__INCLUDED_)
#define AFX_TOOLTIPDEMO_H__F55DF02E_A8D1_4C71_A4D6_03DA7B2F6620__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTooltipDemoApp:
// See TooltipDemo.cpp for the implementation of this class
//

class CTooltipDemoApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CTooltipDemoApp();

	BOOL m_bTTInPopupMenus;

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTooltipDemoApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTooltipDemoApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CTooltipDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLTIPDEMO_H__F55DF02E_A8D1_4C71_A4D6_03DA7B2F6620__INCLUDED_)

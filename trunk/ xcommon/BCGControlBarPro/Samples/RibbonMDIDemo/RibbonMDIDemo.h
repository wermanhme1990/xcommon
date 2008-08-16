// RibbonMDIDemo.h : main header file for the RIBBONMDIDEMO application
//

#if !defined(AFX_RIBBONMDIDEMO_H__EE210392_A6CF_47E4_A90F_5742F5103E32__INCLUDED_)
#define AFX_RIBBONMDIDEMO_H__EE210392_A6CF_47E4_A90F_5742F5103E32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRibbonMDIDemoApp:
// See RibbonMDIDemo.cpp for the implementation of this class
//

class CRibbonMDIDemoApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CRibbonMDIDemoApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRibbonMDIDemoApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL SaveAllModified();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CRibbonMDIDemoApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CRibbonMDIDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RIBBONMDIDEMO_H__EE210392_A6CF_47E4_A90F_5742F5103E32__INCLUDED_)

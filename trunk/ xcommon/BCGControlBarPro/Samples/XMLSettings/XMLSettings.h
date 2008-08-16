// XMLSettings.h : main header file for the XMLSETTINGS application
//

#if !defined(AFX_XMLSETTINGS_H__81D60EE7_A349_4E3F_8571_2372EB34E185__INCLUDED_)
#define AFX_XMLSETTINGS_H__81D60EE7_A349_4E3F_8571_2372EB34E185__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CXMLSettingsApp:
// See XMLSettings.cpp for the implementation of this class
//

class CXMLSettingsApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CXMLSettingsApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMLSettingsApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CXMLSettingsApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CXMLSettingsApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMLSETTINGS_H__81D60EE7_A349_4E3F_8571_2372EB34E185__INCLUDED_)

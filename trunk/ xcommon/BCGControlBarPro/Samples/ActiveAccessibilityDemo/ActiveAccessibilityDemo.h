// ActiveAccessibilityDemo.h : main header file for the ACTIVEACCESSIBILITYDEMO application
//

#if !defined(AFX_ACTIVEACCESSIBILITYDEMO_H__62A86698_88F8_4F56_B326_2ECFA576C9C8__INCLUDED_)
#define AFX_ACTIVEACCESSIBILITYDEMO_H__62A86698_88F8_4F56_B326_2ECFA576C9C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CActiveAccessibilityDemoApp:
// See ActiveAccessibilityDemo.cpp for the implementation of this class
//

class CActiveAccessibilityDemoApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CActiveAccessibilityDemoApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActiveAccessibilityDemoApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CActiveAccessibilityDemoApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CActiveAccessibilityDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIVEACCESSIBILITYDEMO_H__62A86698_88F8_4F56_B326_2ECFA576C9C8__INCLUDED_)

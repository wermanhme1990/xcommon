// ReportDemo.h : main header file for the REPORTDEMO application
//

#if !defined(AFX_REPORTDEMO_H__9F90BE64_C57D_40CD_A0B2_7112A6AA693C__INCLUDED_)
#define AFX_REPORTDEMO_H__9F90BE64_C57D_40CD_A0B2_7112A6AA693C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CReportDemoApp:
// See ReportDemo.cpp for the implementation of this class
//

class CReportDemoApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CReportDemoApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportDemoApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CReportDemoApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CReportDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTDEMO_H__9F90BE64_C57D_40CD_A0B2_7112A6AA693C__INCLUDED_)

// StateToFile.h : main header file for the STATETOFILE application
//

#if !defined(AFX_STATETOFILE_H__FE9DB39B_95B4_4765_9B58_4A1B543202F2__INCLUDED_)
#define AFX_STATETOFILE_H__FE9DB39B_95B4_4765_9B58_4A1B543202F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CStateToFileApp:
// See StateToFile.cpp for the implementation of this class
//

class CStateToFileApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CStateToFileApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();

	static CString g_strRegistryBase;
	void ImportExportRegFile (LPCTSTR lpszCommandLine);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStateToFileApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CStateToFileApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CStateToFileApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATETOFILE_H__FE9DB39B_95B4_4765_9B58_4A1B543202F2__INCLUDED_)

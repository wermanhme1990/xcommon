// FileDialogDemo.h : main header file for the FILEDIALOGDEMO application
//

#if !defined(AFX_FILEDIALOGDEMO_H__C4ED70E8_B222_48F5_B532_22FAC8C55AB2__INCLUDED_)
#define AFX_FILEDIALOGDEMO_H__C4ED70E8_B222_48F5_B532_22FAC8C55AB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CFileDialogDemoApp:
// See FileDialogDemo.cpp for the implementation of this class
//

class CFileDialogDemoApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CFileDialogDemoApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileDialogDemoApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CFileDialogDemoApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL PromptForFileName(CString& fileName, UINT nIDSTitle, DWORD dwFlags, BOOL bOpenFileDialog);
};


extern CFileDialogDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEDIALOGDEMO_H__C4ED70E8_B222_48F5_B532_22FAC8C55AB2__INCLUDED_)

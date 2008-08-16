// ToolbarEditBox.h : main header file for the TOOLBAREDITBOX application
//

#if !defined(AFX_TOOLBAREDITBOX_H__6B01A52C_AF8B_45C2_A8B2_6D4263E28591__INCLUDED_)
#define AFX_TOOLBAREDITBOX_H__6B01A52C_AF8B_45C2_A8B2_6D4263E28591__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CToolbarEditBoxApp:
// See ToolbarEditBox.cpp for the implementation of this class
//

class CToolbarEditBoxApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CToolbarEditBoxApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolbarEditBoxApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CToolbarEditBoxApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CToolbarEditBoxApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBAREDITBOX_H__6B01A52C_AF8B_45C2_A8B2_6D4263E28591__INCLUDED_)

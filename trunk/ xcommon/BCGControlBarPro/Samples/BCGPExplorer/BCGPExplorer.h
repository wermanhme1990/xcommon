// BCGPExplorer.h : main header file for the BCGPEXPLORER application
//

#if !defined(AFX_BCGPEXPLORER_H__EA1ED6B0_6DE3_42D1_B877_AFBC40423A54__INCLUDED_)
#define AFX_BCGPEXPLORER_H__EA1ED6B0_6DE3_42D1_B877_AFBC40423A54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CBCGPExplorerApp:
// See BCGPExplorer.cpp for the implementation of this class
//

class CBCGPExplorerApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CBCGPExplorerApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPExplorerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CBCGPExplorerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CBCGPExplorerApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPEXPLORER_H__EA1ED6B0_6DE3_42D1_B877_AFBC40423A54__INCLUDED_)

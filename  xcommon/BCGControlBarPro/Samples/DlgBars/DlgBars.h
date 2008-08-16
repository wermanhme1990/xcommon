// DlgBars.h : main header file for the DLGBARS application
//

#if !defined(AFX_DLGBARS_H__3B767534_A807_486E_89E8_138BF7F8D082__INCLUDED_)
#define AFX_DLGBARS_H__3B767534_A807_486E_89E8_138BF7F8D082__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDlgBarsApp:
// See DlgBars.cpp for the implementation of this class
//

class CDlgBarsApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CDlgBarsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBarsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDlgBarsApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBARS_H__3B767534_A807_486E_89E8_138BF7F8D082__INCLUDED_)

// PropSheetDemo.h : main header file for the PROPSHEETDEMO application
//

#if !defined(AFX_PROPSHEETDEMO_H__89A250B6_BAD7_4E9C_8E1A_FC2DEFF13229__INCLUDED_)
#define AFX_PROPSHEETDEMO_H__89A250B6_BAD7_4E9C_8E1A_FC2DEFF13229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPropSheetDemoApp:
// See PropSheetDemo.cpp for the implementation of this class
//

class CPropSheetDemoApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CPropSheetDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropSheetDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPropSheetDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPSHEETDEMO_H__89A250B6_BAD7_4E9C_8E1A_FC2DEFF13229__INCLUDED_)

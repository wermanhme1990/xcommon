// TrayMenu.h : main header file for the TRAYMENU application
//

#if !defined(AFX_TRAYMENU_H__F3EB4CF2_7F49_49BE_8505_83950537090B__INCLUDED_)
#define AFX_TRAYMENU_H__F3EB4CF2_7F49_49BE_8505_83950537090B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTrayMenuApp:
// See TrayMenu.cpp for the implementation of this class
//

class CTrayMenuApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CTrayMenuApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrayMenuApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTrayMenuApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAYMENU_H__F3EB4CF2_7F49_49BE_8505_83950537090B__INCLUDED_)

// UndoBox.h : main header file for the UNDOBOX application
//

#if !defined(AFX_UNDOBOX_H__9971B441_3D74_4444_BD94_34988AF4CD84__INCLUDED_)
#define AFX_UNDOBOX_H__9971B441_3D74_4444_BD94_34988AF4CD84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CUndoBoxApp:
// See UndoBox.cpp for the implementation of this class
//

class CUndoBoxApp :		public CWinApp,
						public CBCGPWorkspace
{
public:
	CUndoBoxApp();

	// Override from CBCGWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUndoBoxApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CUndoBoxApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CUndoBoxApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNDOBOX_H__9971B441_3D74_4444_BD94_34988AF4CD84__INCLUDED_)

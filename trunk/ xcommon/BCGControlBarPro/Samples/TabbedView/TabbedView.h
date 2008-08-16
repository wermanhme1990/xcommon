// TabbedView.h : main header file for the TABBEDVIEW application
//

#if !defined(AFX_TABBEDVIEW_H__3AAC59C0_1A9F_41E4_BD97_36AE66EB2A59__INCLUDED_)
#define AFX_TABBEDVIEW_H__3AAC59C0_1A9F_41E4_BD97_36AE66EB2A59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTabbedViewApp:
// See TabbedView.cpp for the implementation of this class
//

class CTabbedViewApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CTabbedViewApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabbedViewApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTabbedViewApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CTabbedViewApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABBEDVIEW_H__3AAC59C0_1A9F_41E4_BD97_36AE66EB2A59__INCLUDED_)

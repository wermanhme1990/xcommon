// OutlookMultiViews.h : main header file for the OUTLOOKMULTIVIEWS application
//
//
// This sample is based on Vswap32 sample:
// http://support.microsoft.com/default.aspx?scid=kb;en-us;141334

#if !defined(AFX_OUTLOOKMULTIVIEWS_H__9FA02FC1_523A_4457_A630_F91AFD71A2C6__INCLUDED_)
#define AFX_OUTLOOKMULTIVIEWS_H__9FA02FC1_523A_4457_A630_F91AFD71A2C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// COutlookMultiViewsApp:
// See OutlookMultiViews.cpp for the implementation of this class
//

class COutlookMultiViewsApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	COutlookMultiViewsApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutlookMultiViewsApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(COutlookMultiViewsApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern COutlookMultiViewsApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTLOOKMULTIVIEWS_H__9FA02FC1_523A_4457_A630_F91AFD71A2C6__INCLUDED_)

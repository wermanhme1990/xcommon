// Slider.h : main header file for the SLIDER application
//

#if !defined(AFX_SLIDER_H__A0EC79A1_F155_4654_9A5A_F491451521F9__INCLUDED_)
#define AFX_SLIDER_H__A0EC79A1_F155_4654_9A5A_F491451521F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSliderApp:
// See Slider.cpp for the implementation of this class
//

class CSliderApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CSliderApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSliderApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSliderApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CSliderApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIDER_H__A0EC79A1_F155_4654_9A5A_F491451521F9__INCLUDED_)

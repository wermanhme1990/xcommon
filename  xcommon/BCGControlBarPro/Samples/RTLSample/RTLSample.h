// RTLSample.h : main header file for the RTLSAMPLE application
//

#if !defined(AFX_RTLSAMPLE_H__3547CFD3_08B1_4B6D_AE81_4FDE00A119DC__INCLUDED_)
#define AFX_RTLSAMPLE_H__3547CFD3_08B1_4B6D_AE81_4FDE00A119DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRTLSampleApp:
// See RTLSample.cpp for the implementation of this class
//

class CRTLSampleApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CRTLSampleApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();

	int	m_nLayout;	// 0 - RTL, 1 - LTR

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRTLSampleApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CRTLSampleApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CRTLSampleApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTLSAMPLE_H__3547CFD3_08B1_4B6D_AE81_4FDE00A119DC__INCLUDED_)

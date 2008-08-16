// BCGCBDotNetExample.h : main header file for the BCGCBDOTNETEXAMPLE application
//

#if !defined(AFX_BCGCBDOTNETEXAMPLE_H__AEAC1B76_E65E_4075_9A38_537CC10CEC1F__INCLUDED_)
#define AFX_BCGCBDOTNETEXAMPLE_H__AEAC1B76_E65E_4075_9A38_537CC10CEC1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CBCGCBDotNetExampleApp:
// See BCGCBDotNetExample.cpp for the implementation of this class
//

class CBCGCBDotNetExampleApp : public CWinApp , public CBCGPWorkspace
{
public:
	CBCGCBDotNetExampleApp();
	virtual ~CBCGCBDotNetExampleApp();

	BOOL	m_bHiColorIcons;
	
	// Override from CBCGPWorkspace
	virtual void PreLoadState();
	virtual void LoadCustomState ();
	virtual void SaveCustomState ();

	CMultiDocTemplate*		m_pDocTemplateCpp;
	CMultiDocTemplate*		m_pDocTemplateWeb;
	CMultiDocTemplate*		m_pStartDocTemplate;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGCBDotNetExampleApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL SaveAllModified();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CBCGCBDotNetExampleApp)
	afx_msg void OnAppAbout();
	afx_msg void OnHelpShowStart();
	afx_msg void OnViewAppLook();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CBCGCBDotNetExampleApp theApp;


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGCBDOTNETEXAMPLE_H__AEAC1B76_E65E_4075_9A38_537CC10CEC1F__INCLUDED_)

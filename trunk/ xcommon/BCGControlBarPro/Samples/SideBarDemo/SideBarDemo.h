// SideBarDemo.h : main header file for the SIDEBARDEMO application
//

#if !defined(AFX_SIDEBARDEMO_H__D5D8F230_19C0_408F_B499_D41A010D55F1__INCLUDED_)
#define AFX_SIDEBARDEMO_H__D5D8F230_19C0_408F_B499_D41A010D55F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

class CSidebarWnd;

/////////////////////////////////////////////////////////////////////////////
// CSideBarDemoApp:
// See SideBarDemo.cpp for the implementation of this class
//

class CSideBarDemoApp : public CWinApp,
						public CBCGPWorkspace
{
public:
	CSideBarDemoApp();

	CSidebarWnd*	m_pwndAppBar;
	BOOL			m_bShowOptionsOnStartup;

	int GetLook () const
	{
		return m_nLook;
	}

	void SetLook (int nLook);

protected:
	int	m_nLook;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSideBarDemoApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSideBarDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CSideBarDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIDEBARDEMO_H__D5D8F230_19C0_408F_B499_D41A010D55F1__INCLUDED_)

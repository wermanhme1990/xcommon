// MDITabsDemo.h : main header file for the MDITABSDEMO application
//

#if !defined(AFX_MDITABSDEMO_H__F18CE417_028D_43E7_99E3_B13C36CB1D72__INCLUDED_)
#define AFX_MDITABSDEMO_H__F18CE417_028D_43E7_99E3_B13C36CB1D72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

class CMDITabOptions
{
public:
	CMDITabOptions();

	enum MDITabsType
	{
		None,
		MDITabsStandard,
		MDITabbedGroups
	};


	MDITabsType			m_nMDITabsType;
	BOOL				m_bMaximizeMDIChild;
	BOOL				m_bTabsOnTop;
	CBCGPTabWnd::Style	m_nTabsStyle;
	BOOL				m_bTabsAutoColor;
	BOOL				m_bMDITabsIcons;
	BOOL				m_bMDITabsDocMenu;
	BOOL				m_bDragMDITabs;
	BOOL				m_bMDITabsContextMenu;
	int					m_nMDITabsBorderSize;
	BOOL				m_bDisableMDIChildRedraw;
	BOOL				m_bFlatFrame;
	BOOL				m_bCustomTooltips;

	void Load ();
	void Save ();

	BOOL IsMDITabsDisabled () const {return m_nMDITabsType == CMDITabOptions::None;}
};

/////////////////////////////////////////////////////////////////////////////
// CMDITabsDemoApp:
// See MDITabsDemo.cpp for the implementation of this class
//

class CMDITabsDemoApp : public CWinApp,
						public CBCGPWorkspace
{
public:
	CMDITabsDemoApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();
	
	void UpdateMDITabs (BOOL bResetMDIChild);

	CMDITabOptions	m_Options;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDITabsDemoApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMDITabsDemoApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CMDITabsDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDITABSDEMO_H__F18CE417_028D_43E7_99E3_B13C36CB1D72__INCLUDED_)

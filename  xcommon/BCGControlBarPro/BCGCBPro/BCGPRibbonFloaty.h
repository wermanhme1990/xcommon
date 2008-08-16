//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGPro Library
// Copyright (C) 1998-2006 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPRibbonFloaty.h : header file
//

#if !defined(AFX_BCGPRIBBONFLOATY_H__629F5B11_333C_45E6_BD8F_748650A59139__INCLUDED_)
#define AFX_BCGPRIBBONFLOATY_H__629F5B11_333C_45E6_BD8F_748650A59139__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef BCGP_EXCLUDE_RIBBON

#include "BCGPRibbonPanelMenu.h"
#include "BCGPRibbonPanel.h"

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonFloaty window

class BCGCBPRODLLEXPORT CBCGPRibbonFloaty : public CBCGPRibbonPanelMenu
{
// Construction
public:
	CBCGPRibbonFloaty();

// Attributes
protected:
	static CBCGPRibbonFloaty*	m_pCurrent;

// Operations
public:
	void SetCommands (	CBCGPRibbonBar* pRibbonBar,
						const CList<UINT,UINT>& lstCommands);
	BOOL Show (int x, int y);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPRibbonFloaty)
	//}}AFX_VIRTUAL

	virtual void SetActive (BOOL bIsActive);
	virtual BOOL IsFloaty () const
	{
		return TRUE;
	}

// Implementation
public:
	virtual ~CBCGPRibbonFloaty();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPRibbonFloaty)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // BCGP_EXCLUDE_RIBBON

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPRIBBONFLOATY_H__629F5B11_333C_45E6_BD8F_748650A59139__INCLUDED_)

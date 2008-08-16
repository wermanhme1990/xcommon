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
// BCGPRibbonCheckBox.h: interface for the CBCGPRibbonCheckBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPRIBBONCHECKBOX_H__25B7B9D5_DE79_4F40_9EB1_1031AF523290__INCLUDED_)
#define AFX_BCGPRIBBONCHECKBOX_H__25B7B9D5_DE79_4F40_9EB1_1031AF523290__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGPRibbonButton.h"

#ifndef BCGP_EXCLUDE_RIBBON

class BCGCBPRODLLEXPORT CBCGPRibbonCheckBox : public CBCGPRibbonButton  
{
	DECLARE_DYNCREATE(CBCGPRibbonCheckBox)

// Construction
public:
	CBCGPRibbonCheckBox ();

	CBCGPRibbonCheckBox (
		UINT	nID, 
		LPCTSTR lpszText);

	virtual ~CBCGPRibbonCheckBox();

// Overrides
public:
	virtual CSize GetRegularSize (CDC* pDC);
	virtual CSize GetCompactSize (CDC* pDC);
	virtual void OnDraw (CDC* pDC);
	virtual void OnDrawOnList (CDC* pDC, CString strText, int nTextOffset, CRect rect, BOOL bIsSelected, BOOL bHighlighted);
	virtual CSize GetImageSize (RibbonImageType type) const;
	virtual BOOL OnDrawMenuImage (CDC* /*pDC*/, CRect /*rect*/)	{	return FALSE;	}

// Attributes:
protected:
	CSize	m_sizeCheckBox;
};

#endif // BCGP_EXCLUDE_RIBBON

#endif // !defined(AFX_BCGPRIBBONCHECKBOX_H__25B7B9D5_DE79_4F40_9EB1_1031AF523290__INCLUDED_)

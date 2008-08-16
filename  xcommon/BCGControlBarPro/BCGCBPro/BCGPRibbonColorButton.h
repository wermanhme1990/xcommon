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
// BCGPRibbonColorButton.h: interface for the CBCGPRibbonColorButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPRIBBONCOLORBUTTON_H__C89F027E_3034_4699_BA86_BC4B601F1681__INCLUDED_)
#define AFX_BCGPRIBBONCOLORBUTTON_H__C89F027E_3034_4699_BA86_BC4B601F1681__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "bcgcbpro.h"

#ifndef BCGP_EXCLUDE_RIBBON

#include "BCGPColorBar.h"
#include "BCGPRibbonButton.h"

class BCGCBPRODLLEXPORT CBCGPRibbonColorButton : public CBCGPRibbonButton  
{
	DECLARE_DYNCREATE(CBCGPRibbonColorButton)

// Construction:
public:
	CBCGPRibbonColorButton ();
	CBCGPRibbonColorButton (
		UINT		nID,
		LPCTSTR		lpszText,
		int			nSmallImageIndex,
		COLORREF	color = RGB (0, 0, 0));

	virtual ~CBCGPRibbonColorButton();

protected:
	void CommonInit ();

// Attributes:
public:
	void EnableAutomaticButton (LPCTSTR lpszLabel, COLORREF colorAutomatic, BOOL bEnable = TRUE);
	void EnableOtherButton (LPCTSTR lpszLabel);
	void EnableDocumentColors (LPCTSTR lpszLabel);
	
	void SetDocumentColors (LPCTSTR lpszLabel, CList<COLORREF,COLORREF>& lstColors);

	static void SetColorName (COLORREF color, const CString& strName)
	{
		CBCGPColorBar::SetColorName (color, strName);
	}

	COLORREF GetAutomaticColor () const
	{
		return m_ColorAutomatic;
	}

	void SetColor (COLORREF color)
	{
		m_Color = color;
	}

	COLORREF GetColor () const
	{
		return m_Color;
	}

	void SetPalette (CPalette* pPalette);

	int	GetColumns () const
	{
		return m_nColumns;
	}

	void SetColumns (int nColumns)
	{
		m_nColumns = nColumns;
	}

protected:
	virtual BOOL HasMenu () const
	{
		return TRUE;
	}

// Opeartions:
public:
	void UpdateColor (COLORREF color);

// Overrides
protected:
	virtual void DrawImage (CDC* pDC, RibbonImageType type, CRect rectImage);
	virtual void CopyFrom (const CBCGPBaseRibbonElement& src);
	virtual void OnShowPopupMenu ();

	COLORREF					m_Color;	// Currently selected color
	COLORREF					m_ColorAutomatic;
	CArray<COLORREF, COLORREF>	m_Colors;
	CPalette					m_Palette;
	int							m_nColumns;

	BOOL						m_bIsAutomaticButton;
	BOOL						m_bIsOtherButton;
	BOOL						m_bIsDocumentColors;

	CString						m_strAutomaticButtonLabel;
	CString						m_strOtherButtonLabel;
	CString						m_strDocumentColorsLabel;
};

#endif // BCGP_EXCLUDE_RIBBON

#endif // !defined(AFX_BCGPRIBBONCOLORBUTTON_H__C89F027E_3034_4699_BA86_BC4B601F1681__INCLUDED_)

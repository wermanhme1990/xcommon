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
// BCGPRibbonButton.h: interface for the CBCGPRibbonButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPRIBBONBUTTON_H__3698E9F7_DEB7_43C4_90C0_7973B7FF0A8F__INCLUDED_)
#define AFX_BCGPRIBBONBUTTON_H__3698E9F7_DEB7_43C4_90C0_7973B7FF0A8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGCBPro.h"

#ifndef BCGP_EXCLUDE_RIBBON

#include "BCGPBaseRibbonElement.h"

#define BCGPRIBBON_IMAGE_AUTO	9999

class BCGCBPRODLLEXPORT CBCGPRibbonButton : public CBCGPBaseRibbonElement  
{
	friend class CBCGPRibbonPanel;
	friend class CBCGPRibbonBar;
	friend class CBCGPRibbonPanelMenuBar;

	DECLARE_DYNCREATE(CBCGPRibbonButton)

// Construction
public:
	CBCGPRibbonButton ();

	CBCGPRibbonButton (
		UINT	nID, 
		LPCTSTR lpszText, 
		int		nSmallImageIndex = BCGPRIBBON_IMAGE_AUTO,
		int		nLargeImageIndex = -1,
		BOOL	bAlwaysShowDescription = FALSE);

	CBCGPRibbonButton (
		UINT	nID, 
		LPCTSTR lpszText, 
		HICON	hIcon,
		BOOL	bAlwaysShowDescription = FALSE);

	virtual ~CBCGPRibbonButton();

// Attributes
public:
	virtual void SetText (LPCTSTR lpszText);
	virtual void SetDescription (LPCTSTR lpszText);

	virtual BOOL IsDefaultPanelButton () const
	{
		return FALSE;
	}

	virtual BOOL IsMainRibbonButton () const
	{
		return FALSE;
	}

	void SetMenu (	HMENU hMenu, 
					BOOL bIsDefaultCommand = FALSE, 
					BOOL bRightAlign = FALSE);

	void SetMenu (	UINT uiMenuResID, 
					BOOL bIsDefaultCommand = FALSE,
					BOOL bRightAlign = FALSE);

	HMENU GetMenu () const
	{
		return m_hMenu;
	}

	void SetDefaultCommand (BOOL bSet = TRUE)
	{
		m_bIsDefaultCommand = bSet;
	}

	BOOL IsDefaultCommand () const
	{
		return m_bIsDefaultCommand;
	}

	void SetRightAlignMenu (BOOL bSet = TRUE)
	{
		m_bRightAlignMenu = bSet;
	}

	BOOL IsRightAlignMenu () const
	{
		return m_bRightAlignMenu;
	}

	virtual BOOL HasMenu () const
	{
		return m_hMenu != NULL || m_arSubItems.GetSize () > 0;
	}

	virtual BOOL IsCommandAreaHighlighted () const
	{
		return m_bIsCommandHighlighted && IsHighlighted ();
	}

	virtual BOOL IsMenuAreaHighlighted () const
	{
		return m_bIsMenuHighlighted && IsHighlighted ();
	}

	CRect GetMenuRect () const
	{
		return m_rectMenu;
	}

	CRect GetCommandRect () const
	{
		return m_rectCommand;
	}

	BOOL IsMenuOnBottom () const
	{
		return m_bMenuOnBottom;
	}

	virtual BOOL IsAlwaysDrawBorder () const
	{
		return m_bForceDrawBorder;
	}

	virtual BOOL CanBeStretched ();

	BOOL IsSingleLineText () const
	{
		return m_sizeTextBottom.cy == m_sizeTextRight.cy;
	}

	int GetTextRowHeight () const
	{
		return m_sizeTextRight.cy;
	}

	const CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>&
		GetSubItems () const
	{
		return m_arSubItems;
	}

	int	GetImageIndex (BOOL bLargeImage) const
	{
		return bLargeImage ? m_nLargeImageIndex : m_nSmallImageIndex;
	}

	void SetImageIndex (int nIndex, BOOL bLargeImage)
	{
		if (bLargeImage)
		{
			m_nLargeImageIndex = nIndex;
		}
		else
		{
			m_nSmallImageIndex = nIndex;
		}
	}

// Operations
public:
	void AddSubItem (CBCGPBaseRibbonElement* pSubItem);
	void RemoveAllSubItems ();

protected:
	void CommonInit ();

// Overrides
public:
	virtual void OnDraw (CDC* pDC);

	virtual CSize GetRegularSize (CDC* pDC);
	virtual CSize GetCompactSize (CDC* pDC);

	virtual void DrawImage (CDC* pDC, RibbonImageType type, CRect rectImage);
	virtual CSize GetImageSize (RibbonImageType type) const;

	virtual void OnCalcTextSize (CDC* pDC);
	virtual void CleanUpSizes ()
	{
		m_sizeTextRight = m_sizeTextBottom = CSize (0, 0);
	}

	virtual void SetParentCategory (CBCGPRibbonCategory* pParent);

	virtual void ClosePopupMenu ();
	virtual CString GetToolTipText () const;

	virtual int DrawText (CDC* pDC, const CString& strText, CRect rectText, UINT uiDTFlags,
							COLORREF clrText = (COLORREF)-1);

protected:
	virtual void OnDrawOnList (CDC* pDC, CString strText, int nTextOffset, CRect rect, BOOL bIsSelected, BOOL bHighlighted);
	virtual int AddToListBox (CBCGPRibbonCommandsListBox* pWndListBox, BOOL bDeep);

	virtual void OnLButtonDown (CPoint point);
	virtual void OnLButtonUp (CPoint point);
	virtual void OnMouseMove (CPoint point);

	virtual void OnClick (CPoint point);
	virtual void OnShowPopupMenu ();

	virtual void CopyFrom (const CBCGPBaseRibbonElement& src);
	virtual void SetOriginal (CBCGPBaseRibbonElement* pOriginal);

	virtual COLORREF OnFillBackground (CDC* pDC);
	virtual void OnDrawBorder (CDC* pDC);

	virtual CBCGPBaseRibbonElement* FindByID (UINT uiCmdID);
	virtual CBCGPBaseRibbonElement* FindByData (DWORD_PTR dwData);

	virtual int GetTextOffset () const	{	return m_szMargin.cx;	}

	virtual void SetParentRibbonBar (CBCGPRibbonBar* pRibbonBar);

// Attributes
protected:
	CSize	m_sizeTextRight;
	CSize	m_sizeTextBottom;

	int		m_nSmallImageIndex;
	int		m_nLargeImageIndex;

	HMENU	m_hMenu;
	BOOL	m_bRightAlignMenu;
	BOOL	m_bIsDefaultCommand;
	int		m_nMenuArrowMargin;
	BOOL	m_bAutodestroyMenu;

	CSize	m_szMargin;

	CRect	m_rectMenu;
	CRect	m_rectCommand;

	BOOL	m_bMenuOnBottom;
	BOOL	m_bIsMenuHighlighted;
	BOOL	m_bIsCommandHighlighted;

	HICON	m_hIcon;
	BOOL	m_bForceDrawBorder;

	BOOL	m_bToBeClosed;
	BOOL	m_bAlwaysShowDescription;

	CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>	m_arSubItems;
};

#endif // BCGP_EXCLUDE_RIBBON

#endif // !defined(AFX_BCGPRIBBONBUTTON_H__3698E9F7_DEB7_43C4_90C0_7973B7FF0A8F__INCLUDED_)

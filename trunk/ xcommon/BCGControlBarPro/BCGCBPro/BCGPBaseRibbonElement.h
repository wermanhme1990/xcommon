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
// BCGPBaseRibbonElement.h: interface for the CBCGPBaseRibbonElement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPBASERIBBONELEMENT_H__920C1FDC_61E3_4AC0_995B_AF8F333276E4__INCLUDED_)
#define AFX_BCGPBASERIBBONELEMENT_H__920C1FDC_61E3_4AC0_995B_AF8F333276E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGCBPro.h"

#ifndef BCGP_EXCLUDE_RIBBON

class CBCGPRibbonCategory;
class CBCGPRibbonPanel;
class CBCGPRibbonCmdUI;
class CBCGPPopupMenu;
class CBCGPRibbonButtonsGroup;
class CBCGPRibbonPanelMenuBar;
class CBCGPRibbonCommandsListBox;
class CBCGPPopupMenu;
class CBCGPRibbonQuickAccessToolbar;

class BCGCBPRODLLEXPORT CBCGPBaseRibbonElement : public CObject
{
	friend class CBCGPRibbonPanel;
	friend class CBCGPRibbonBar;
	friend class CBCGPRibbonCmdUI;
	friend class CBCGPRibbonButtonsGroup;
	friend class CBCGPPopupMenu;
	friend class CBCGPRibbonPanelMenuBar;
	friend class CBCGPRibbonPanelMenu;
	friend class CBCGPRibbonFloaty;
	friend class CBCGPRibbonCategory;
	friend class CBCGPPopupMenuBar;
	friend class CBCGPRibbonQuickAccessToolbar;
	friend class CBCGPRibbonPaletteButton;
	friend class CBCGPRibbonStatusBar;

	DECLARE_DYNAMIC(CBCGPBaseRibbonElement)

// Construction
public:
	CBCGPBaseRibbonElement ();
	virtual ~CBCGPBaseRibbonElement ();

// Attributes
public:
	enum RibbonElementLocation
	{
		RibbonElementNotInGroup,
		RibbonElementSingleInGroup,
		RibbonElementFirstInGroup,
		RibbonElementLastInGroup,
		RibbonElementMiddleInGroup,
	};

	enum RibbonImageType
	{
		RibbonImageLarge,
		RibbonImageSmall
	};

	RibbonElementLocation GetLocationInGroup () const
	{
		return m_Location;
	}

	virtual void SetID (UINT nID);
	UINT GetID () const
	{
		return m_nID;
	}

	virtual void SetText (LPCTSTR lpszText);
	LPCTSTR GetText () const
	{
		return m_strText;
	}

	virtual void SetTextAlwaysOnRight (BOOL bSet = TRUE);
	BOOL IsTextAlwaysOnRight () const
	{
		return m_bTextAlwaysOnRight;
	}

	void SetRect (CRect rect)
	{
		m_rect = rect;
	}

	CRect GetRect () const
	{
		return m_rect;
	}

	DWORD_PTR GetData () const
	{
		return m_dwData;
	}

	void SetData (DWORD_PTR dwData)
	{
		m_dwData = dwData;
	}

	virtual CString GetToolTipText () const;
	virtual CString GetDescription () const;

	virtual void SetToolTipText (LPCTSTR lpszText);
	virtual void SetDescription (LPCTSTR lpszText);

	virtual BOOL IsHighlighted () const
	{
		return m_bIsHighlighted;
	}

	virtual BOOL IsFocused () const
	{
		return m_bIsFocused;
	}

	virtual BOOL IsPressed () const
	{
		return m_bIsPressed;
	}

	virtual BOOL IsDisabled () const
	{
		return m_bIsDisabled;
	}

	virtual BOOL IsChecked () const
	{
		return m_bIsChecked;
	}

	virtual BOOL IsDroppedDown () const
	{
		return 	m_bIsDroppedDown;
	}

	BOOL IsCompactMode () const
	{
		return m_bCompactMode;
	}

	BOOL IsVisible () const
	{
		return m_bIsVisible;
	}

	void SetVisible (BOOL bIsVisible)
	{
		m_bIsVisible = bIsVisible;
	}

	void SetCompactMode (BOOL bCompactMode = TRUE)
	{
		m_bCompactMode = bCompactMode;
	}

	virtual CSize GetSize (CDC* pDC)
	{
		return m_bCompactMode ? GetCompactSize (pDC) : GetRegularSize (pDC);
	}

	virtual CSize GetRegularSize (CDC* pDC) = 0;
	virtual CSize GetCompactSize (CDC* pDC)
	{
		return GetRegularSize (pDC);
	}

	virtual CSize GetImageSize (RibbonImageType /*type*/) const
	{
		return CSize (0, 0);
	}

	virtual BOOL IsSeparator () const
	{
		return FALSE;
	}

	virtual BOOL IsTabStop () const
	{
		return TRUE;
	}

	BOOL IsShowGroupBorder () const
	{
		return m_bShowGroupBorder;
	}

	virtual void DrawImage (CDC* /*pDC*/, RibbonImageType /*type*/, CRect /*rectImage*/) {}

	virtual void OnAfterChangeRect (CDC* pDC);
	virtual void OnShow (BOOL /*bShow*/) {}

	virtual CBCGPBaseRibbonElement* HitTest (CPoint point);

	CBCGPRibbonCategory* GetParentCategory () const
	{
		return m_pParent;
	}

	CBCGPRibbonPanel* GetParentPanel () const;

	CWnd* GetParentWnd () const;

	CBCGPRibbonBar*	GetParentRibbonBar () const
	{
		return m_pRibbonBar;
	}

	virtual void SetParentRibbonBar (CBCGPRibbonBar* pRibbonBar)
	{
		m_pRibbonBar = pRibbonBar;
	}

	CBCGPRibbonBar* GetTopLevelRibbonBar () const;

	BOOL IsMenuMode () const;
	
	virtual BOOL HasMenu () const
	{
		return FALSE;
	}

	virtual void OnShowPopupMenu ()	{}

	virtual BOOL CanBeAddedToQAT () const;
	
	BOOL IsQATMode () const
	{
		return m_bQuickAccessMode;
	}

	virtual BOOL IsWholeRowHeight () const
	{
		return FALSE;
	}

	virtual UINT GetNotifyID ()
	{
		return m_nID;
	}

	virtual BOOL IsAutoRepeatMode (int& /*nDelay*/ /* ms */) const
	{
		return FALSE;
	}

	virtual BOOL OnAutoRepeat ()
	{
		return FALSE;
	}

	virtual BOOL StretchToWholeRow (CDC* pDC, int nHeight);

	virtual BOOL OnDrawMenuImage (CDC* pDC, CRect rect)
	{
		DrawImage (pDC, RibbonImageSmall, rect);
		return TRUE;
	}

// Overrides
public:
	virtual void SetParentCategory (CBCGPRibbonCategory* pParent);
	virtual void CopyFrom (const CBCGPBaseRibbonElement& src);
	virtual void SetParentMenu (CBCGPRibbonPanelMenuBar* pMenuBar);
	virtual void SetOriginal (CBCGPBaseRibbonElement* pOriginal);

	virtual int AddToListBox (CBCGPRibbonCommandsListBox* pWndListBox, BOOL bDeep);
	virtual void OnDrawOnList (CDC* /*pDC*/, CString /*strText*/, int /*nTextOffset*/, CRect /*rect*/, BOOL /*bIsSelected*/, BOOL /*bHighlighted*/) {}

	virtual void OnDraw (CDC* pDC) = 0;
	virtual void OnCalcTextSize (CDC* /*pDC*/)	{}
	virtual void CleanUpSizes ()	{}

	virtual BOOL CanBeStretched ()
	{
		return TRUE;
	}

	virtual BOOL IsAlignByColumn () const
	{
		return TRUE;
	}

	virtual void ClosePopupMenu ();
	virtual CBCGPBaseRibbonElement* FindByID (UINT uiCmdID);
	virtual CBCGPBaseRibbonElement* FindByData (DWORD_PTR dwData);
	virtual void UpdateTooltipInfo ();

	virtual BOOL OnAddToQAToolbar (CBCGPRibbonQuickAccessToolbar& qat);

	virtual void Redraw ();

	virtual CBCGPBaseRibbonElement* GetPressed ();
	virtual CBCGPBaseRibbonElement* GetDroppedDown ();
	virtual CBCGPBaseRibbonElement* GetHighlighted ();

protected:
	virtual void OnLButtonDown (CPoint point);
	virtual void OnLButtonUp (CPoint /*point*/)		{}
	virtual void OnMouseMove (CPoint /*point*/)		{}
	virtual void OnLButtonDblClk (CPoint /*point*/)	{}
	virtual void OnHighlight (BOOL /*bHighlight*/)	{}

	virtual void OnUpdateCmdUI (CBCGPRibbonCmdUI* pCmdUI, CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual BOOL NotifyControlCommand (BOOL bAccelerator, int nNotifyCode, WPARAM wParam, LPARAM lParam);

	virtual BOOL ReplaceByID (UINT uiCmdID, CBCGPBaseRibbonElement* pElem);

	virtual void GetElementsByID (UINT uiCmdID, 
		CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& arElements);

	virtual void GetItemIDsList (CList<UINT,UINT>& lstItems) const;
	
	virtual void OnEnable (BOOL /*bEnable*/)	{}
	virtual void OnCheck (BOOL /*bCheck*/)	{}

// Operations
public:
	BOOL NotifyCommand (BOOL bWithDelay = FALSE);
	void PostMenuCommand (UINT uiCmdId);

protected:
	void SetDroppedDown (CBCGPPopupMenu* pPopupMenu);

// Attributes
protected:
	RibbonElementLocation		m_Location;
	UINT						m_nID;
	DWORD_PTR					m_dwData;
	CString						m_strText;
	BOOL						m_bTextAlwaysOnRight;
	CRect						m_rect;
	CBCGPRibbonCategory*		m_pParent;
	CBCGPRibbonButtonsGroup*	m_pParentGroup;
	CBCGPRibbonPanelMenuBar*	m_pParentMenu;
	CBCGPRibbonBar*				m_pRibbonBar;
	BOOL						m_bCompactMode;
	BOOL						m_bQuickAccessMode;
	BOOL						m_bIsHighlighted;
	BOOL						m_bIsFocused;
	BOOL						m_bIsPressed;
	BOOL						m_bIsDisabled;
	BOOL						m_bIsChecked;
	BOOL						m_bIsDroppedDown;
	CBCGPBaseRibbonElement*		m_pOriginal;
	CString						m_strToolTip;
	CString						m_strDescription;
	int							m_nRow;
	BOOL						m_bDontNotify;
	CBCGPPopupMenu*				m_pPopupMenu;
	int							m_nImageOffset;
	BOOL						m_bShowGroupBorder;
	BOOL						m_bIsVisible;
};

class BCGCBPRODLLEXPORT CBCGPRibbonSeparator : public CBCGPBaseRibbonElement
{
	DECLARE_DYNCREATE(CBCGPRibbonSeparator)

public:
	CBCGPRibbonSeparator (BOOL bIsHoriz = FALSE);

protected:

	virtual BOOL IsSeparator () const
	{
		return TRUE;
	}

	virtual BOOL IsTabStop () const
	{
		return FALSE;
	}

public:
	virtual int AddToListBox (CBCGPRibbonCommandsListBox* pWndListBox, BOOL bDeep);

protected:
	virtual void OnDraw (CDC* pDC);
	virtual CSize GetRegularSize (CDC* pDC);
	virtual void CopyFrom (const CBCGPBaseRibbonElement& src);
	virtual void OnDrawOnList (CDC* pDC, CString strText, int nTextOffset, CRect rect, BOOL bIsSelected, BOOL bHighlighted);

	BOOL	m_bIsHoriz;
};

#endif // BCGP_EXCLUDE_RIBBON

#endif // !defined(AFX_BCGPBASERIBBONELEMENT_H__920C1FDC_61E3_4AC0_995B_AF8F333276E4__INCLUDED_)

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

#if !defined(AFX_BCGPRIBBONCATEGORY_H__DADE0EF9_F888_4CDC_A1B7_8F972E57A7C9__INCLUDED_)
#define AFX_BCGPRIBBONCATEGORY_H__DADE0EF9_F888_4CDC_A1B7_8F972E57A7C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BCGPRibbonCategory.h : header file
//

#include "BCGCBPro.h"

#ifndef BCGP_EXCLUDE_RIBBON

#include "BCGPBaseRibbonElement.h"
#include "BCGPToolBarImages.h"
#include "BCGPRibbonBar.h"

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonTab

class CBCGPRibbonCategory;

class BCGCBPRODLLEXPORT CBCGPRibbonTab : public CBCGPBaseRibbonElement
{
	friend class CBCGPRibbonCategory;
	friend class CBCGPRibbonBar;

	CBCGPRibbonTab ();
	virtual void OnDraw (CDC* pDC);
	virtual CSize GetRegularSize (CDC* pDC);
	virtual void OnLButtonDown (CPoint point);
	virtual void OnLButtonDblClk (CPoint point);
	virtual CString GetToolTipText () const;
	virtual void CopyFrom (const CBCGPBaseRibbonElement& src);

	BOOL					m_bIsTrancated;
	BCGPRibbonCategoryColor m_Color;
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonCategory

class CBCGPRibbonPanel;
class CBCGPRibbonBar;

class BCGCBPRODLLEXPORT CBCGPRibbonCategory : public CObject
{
	friend class CBCGPRibbonBar;
	friend class CBCGPRibbonTab;
	friend class CBCGPRibbonPanel;
	friend class CBCGPRibbonPanelMenuBar;

	DECLARE_DYNCREATE(CBCGPRibbonCategory)

// Construction
protected:
	CBCGPRibbonCategory();

	CBCGPRibbonCategory (
		CBCGPRibbonBar*	pParenrRibbonBar, 
		LPCTSTR			lpszName,
		UINT			uiSmallImagesResID,
		UINT			uiLargeImagesResID,
		CSize			sizeSmallImage = CSize (16, 16),
		CSize			sizeLargeImage = CSize (32, 32));

	void CommonInit ();

// Attributes
public:
	CBCGPRibbonBar* GetParentRibbonBar () const
	{
		return m_pParenrRibbonBar;
	}

	CBCGPRibbonPanelMenuBar* GetParentMenuBar () const
	{
		return m_pParentMenuBar;
	}

	CBCGPBaseRibbonElement* HitTest (CPoint point, BOOL bCheckPanelCaption = FALSE) const;
	CBCGPRibbonPanel* GetPanelFromPoint (CPoint point) const;

	CRect GetTabRect () const
	{
		return m_Tab.m_rect;
	}

	CRect GetRect () const
	{
		return m_rect;
	}

	BOOL IsActive () const
	{
		return m_bIsActive;
	}

	CSize GetImageSize (BOOL bIsLargeImage) const;

	int GetImageCount (BOOL bIsLargeImage) const
	{
		return bIsLargeImage ? m_LargeImages.GetCount () : m_SmallImages.GetCount ();
	}

	LPCTSTR GetName () const
	{
		return m_strName;
	}

	void SetName (LPCTSTR lpszName)
	{
		m_strName = lpszName == NULL ? _T("") : lpszName;
	}

	DWORD_PTR GetData () const
	{
		return m_dwData;
	}

	void SetData (DWORD_PTR dwData)
	{
		m_dwData = dwData;
	}

	UINT GetContextID () const
	{
		return m_uiContextID;
	}

	void SetTabColor (BCGPRibbonCategoryColor color)
	{
		m_Tab.m_Color = color;
	}

	BCGPRibbonCategoryColor GetTabColor () const
	{
		return m_Tab.m_Color;
	}

	BOOL IsVisible () const
	{
		return m_bIsVisible;
	}

	CBCGPBaseRibbonElement* GetDroppedDown ();

	CBCGPBaseRibbonElement* GetParentButton () const;

	CBCGPToolBarImages& GetSmallImages ()	{	return m_SmallImages;	}
	CBCGPToolBarImages& GetLargeImages ()	{	return m_LargeImages;	}

// Operations
public:
	CBCGPRibbonPanel* AddPanel (LPCTSTR lpszPanelName, HICON hIcon = 0,
		CRuntimeClass* pRTI = NULL);

	int GetPanelCount () const;
	CBCGPRibbonPanel* GetPanel (int nIndex);

	int GetMaxHeight (CDC* pDC);

	CBCGPBaseRibbonElement* FindByID (UINT uiCmdID, BOOL bVisibleOnly = TRUE) const;
	CBCGPBaseRibbonElement* FindByData (DWORD_PTR dwData, BOOL bVisibleOnly = TRUE) const;

	CBCGPRibbonPanel* HighlightPanel (CBCGPRibbonPanel* pHLPanel, CPoint point);
	CBCGPRibbonPanel* FindPanelWithElem (const CBCGPBaseRibbonElement* pElement);

	void AddHidden (CBCGPBaseRibbonElement* pElem);

	void GetElements (
		CArray <CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& arElements);

	void GetElementsByID (UINT uiCmdID, 
		CArray <CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& arElements);

	void GetItemIDsList (CList<UINT,UINT>& lstItems, BOOL bHiddenOnly = FALSE) const;

// Overrides
	virtual void RecalcLayout (CDC* pDC);
	virtual void OnDraw (CDC* pDC);
	virtual void OnCancelMode();
	virtual CBCGPBaseRibbonElement* OnLButtonDown (CPoint point);
	virtual void OnLButtonUp (CPoint point);
	virtual void OnMouseMove (CPoint point);
	virtual void OnUpdateCmdUI (CBCGPRibbonCmdUI* pCmdUI, CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual BOOL NotifyControlCommand (BOOL bAccelerator, int nNotifyCode, WPARAM wParam, LPARAM lParam);

	virtual BOOL OnDrawImage (CDC* pDC, CRect rect, 
		CBCGPBaseRibbonElement* pElement, BOOL bIsLargeImage, 
		BOOL nImageIndex, BOOL bCenter);

	virtual void CopyFrom (CBCGPRibbonCategory& src);

	virtual void OnDrawMenuBorder (CDC* /*pDC*/, CBCGPRibbonPanelMenuBar* /*pMenuBar*/)	{}

// Implementation
public:
	virtual ~CBCGPRibbonCategory();

	// Generated message map functions
protected:

	void SetActive (BOOL bIsActive = TRUE);
	void ShowElements (BOOL bShow = TRUE);

	void ShowFloating (CRect rectFloating);

	void RecalcPanelWidths (CDC* pDC);
	void CleanUpSizes ();
	int GetMinWidth (CDC* pDC);

	CString										m_strName;
	BOOL										m_bIsActive;
	BOOL										m_bIsVisible;
	CArray<CBCGPRibbonPanel*,CBCGPRibbonPanel*>	m_arPanels;
	CBCGPRibbonBar*								m_pParenrRibbonBar;
	CBCGPRibbonPanelMenuBar*					m_pParentMenuBar;
	BOOL										m_bMouseIsPressed;
	CRect										m_rect;
	CBCGPRibbonTab								m_Tab;
	DWORD_PTR									m_dwData;
	UINT										m_uiContextID;
	int											m_nLastCategoryWidth;

	//----------------------
	// Category image lists:
	//----------------------
	CBCGPToolBarImages		m_SmallImages;
	CBCGPToolBarImages		m_LargeImages;

	//---------------------------------
	// Category elements (non-visible):
	//---------------------------------
	CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>	m_arElements;
};

#endif // BCGP_EXCLUDE_RIBBON

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPRIBBONCATEGORY_H__DADE0EF9_F888_4CDC_A1B7_8F972E57A7C9__INCLUDED_)

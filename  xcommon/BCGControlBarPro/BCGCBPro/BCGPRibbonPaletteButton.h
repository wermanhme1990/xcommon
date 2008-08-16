// BCGPRibbonPaletteButton.h: interface for the CBCGPRibbonPaletteButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPRIBBONPALETTEBUTTON_H__EE6BE1A6_CA76_4764_9A27_C5128583BEB6__INCLUDED_)
#define AFX_BCGPRIBBONPALETTEBUTTON_H__EE6BE1A6_CA76_4764_9A27_C5128583BEB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGCBPro.h"

#ifndef BCGP_EXCLUDE_RIBBON

#include "BCGPRibbonButton.h"
#include "BCGPToolBarImages.h"
#include "BCGPToolbarMenuButton.h"

class CBCGPRibbonPaletteIcon;
class CBCGPPopupMenu;

class BCGCBPRODLLEXPORT CBCGPRibbonPaletteButton : public CBCGPRibbonButton
{
	friend class CBCGPRibbonPaletteIcon;
	friend class CBCGPRibbonPanelMenuBar;
	friend class CBCGPRibbonPanelMenu;
	friend class CBCGPRibbonPanel;
	friend class CBCGPRibbonPaletteMenuButton;

	DECLARE_DYNCREATE(CBCGPRibbonPaletteButton)

// Construction
public:
	CBCGPRibbonPaletteButton();

	CBCGPRibbonPaletteButton (
		UINT				nID,
		LPCTSTR				lpszText, 
		int					nSmallImageIndex,
		int					nLargeImageIndex,
		CBCGPToolBarImages& imagesPalette);

	CBCGPRibbonPaletteButton (
		UINT				nID,
		LPCTSTR				lpszText, 
		int					nSmallImageIndex,
		int					nLargeImageIndex,
		UINT				uiImagesPaletteResID = 0,
		int					cxPaletteImage = 0);

	virtual ~CBCGPRibbonPaletteButton();

// Attributes
public:
	void SetPaletteID (UINT nID)
	{
		m_nPaletteID = nID;
	}

	int GetPaletteID () const
	{
		return m_nPaletteID;
	}

	void SelectItem (int nItemIndex);
	int GetSelectedItem () const
	{
		return m_nSelected;
	}

	void EnableMenuResize (BOOL bEnable = TRUE)
	{
		m_bEnableMenuResize = bEnable;
	}

	BOOL IsMenuResizeEnabled () const
	{
		return m_bEnableMenuResize;
	}

	void SetIconsInRow (int nIconsInRow)
	{
		m_nIconsInRow = nIconsInRow;
	}

	int GetIconsInRow () const
	{
		return m_nIconsInRow;
	}

	static int GetLastSelectedItem (UINT uiCmdID);

// Operations:
public:
	void AddGroup (
		LPCTSTR lpszGroupName,
		UINT	uiImagesPaletteResID,
		int		cxPaletteImage);

	void AddGroup (
		LPCTSTR lpszGroupName,
		CBCGPToolBarImages& imagesGroup);

protected:
	void CommonInit ();
	void CreateIcons ();
	void RemoveAll ();
	void RebuildIconLocations ();
	void DrawPaletteIcon (CDC* pDC, CBCGPRibbonPaletteIcon* pIcon);
	void OnClickPaletteIcon (CBCGPRibbonPaletteIcon* pIcon);
	void GetMenuItems (CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& arButtons);

	virtual CBCGPBaseRibbonElement* HitTest (CPoint point);
	virtual CBCGPBaseRibbonElement* GetPressed ();
	virtual CBCGPBaseRibbonElement* GetHighlighted ();

	virtual void CopyFrom (const CBCGPBaseRibbonElement& src);

	virtual void OnShowPopupMenu ();

	int GetMenuRowHeight () const;

	virtual UINT GetNotifyID ()
	{
		if (m_bNotifyPaletteID)
		{
			SetNotifyParentID (FALSE);
			return m_nPaletteID;
		}
		else
		{
			return m_nID;
		}
	}

// Overrides
public:
	virtual void OnDraw (CDC* pDC);

	virtual CSize GetRegularSize (CDC* pDC);
	virtual CSize GetCompactSize (CDC* pDC);

	virtual void OnAfterChangeRect (CDC* pDC);

	virtual BOOL HasMenu () const
	{
		return m_imagesPalette.GetCount () > 0;
	}

	virtual BOOL IsWholeRowHeight () const
	{
		return TRUE;
	}

	virtual CBCGPBaseRibbonElement* GetDroppedDown ();

// Attributes
protected:
	CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>	m_arIcons;
	CStringArray		m_arGroupNames;
	CArray<int,int>		m_arGroupLen;

	UINT				m_nPaletteID;
	BOOL				m_bNotifyPaletteID;
	CBCGPToolBarImages	m_imagesPalette;
	int					m_nImagesInRow;
	int					m_nImagesInColumn;
	BOOL				m_bSmallIcons;
	int					m_nScrollOffset;
	int					m_nScrollTotal;
	int					m_nSelected;
	BOOL				m_bEnableMenuResize;
	int					m_nIconsInRow;

	static CMap<UINT,UINT,int,int>	m_mapSelectedItems;

	void SetNotifyParentID (BOOL bSet);
};

////////////////////////////////////////////
// CBCGPRibbonPaletteIcon

class BCGCBPRODLLEXPORT CBCGPRibbonPaletteIcon : public CBCGPRibbonButton
{
	friend class CBCGPRibbonPaletteButton;

	DECLARE_DYNCREATE(CBCGPRibbonPaletteIcon)

protected:
	CBCGPRibbonPaletteIcon (CBCGPRibbonPaletteButton* pOwner = NULL,
		int nIndex = 0);

public:
	BOOL IsFirst () const;
	BOOL IsLast () const;

protected:
	virtual void OnDraw (CDC* pDC);
	virtual void OnClick (CPoint point);
	virtual void CopyFrom (const CBCGPBaseRibbonElement& src);
	virtual CSize GetRegularSize (CDC* pDC);
	virtual CSize GetCompactSize (CDC* pDC);
	virtual BOOL IsAutoRepeatMode (int& nDelay /* ms */) const;
	virtual BOOL OnAutoRepeat ();
	virtual BOOL CanBeAddedToQAT () const
	{
		return TRUE;
	}
	virtual BOOL OnAddToQAToolbar (CBCGPRibbonQuickAccessToolbar& qat);

	int							m_nIndex;
	CBCGPRibbonPaletteButton*	m_pOwner;
};

////////////////////////////////////////////////
// CBCGPRibbonPaletteMenuButton

class BCGCBPRODLLEXPORT CBCGPRibbonPaletteMenuButton : public CBCGPToolbarMenuButton
{
	DECLARE_DYNCREATE(CBCGPRibbonPaletteMenuButton)

// Construction
protected:
	CBCGPRibbonPaletteMenuButton();

public:
	CBCGPRibbonPaletteMenuButton (UINT uiID, int iImage, LPCTSTR lpszText,
		CBCGPToolBarImages& imagesPalette);

	CBCGPRibbonPaletteMenuButton (UINT uiID, int iImage, LPCTSTR lpszText,
		UINT uiImagesPaletteResID = 0, int cxPaletteImage = 0);

	virtual ~CBCGPRibbonPaletteMenuButton ();

protected:
	void CommonInit ();

// Attributes:
public:
	CBCGPRibbonPaletteButton& GetPalette ()
	{
		return m_paletteButton;
	}

protected:
	CBCGPRibbonPaletteButton m_paletteButton;

// Overrides:
public:
	virtual void CopyFrom (const CBCGPToolbarButton& src);
	virtual CBCGPPopupMenu* CreatePopupMenu ();
	virtual BOOL IsEmptyMenuAllowed () const
	{
		return TRUE;
	}
	virtual BOOL HasButton () const
	{
		return TRUE;
	}

// Operations:
public:
};

#endif // BCGP_EXCLUDE_RIBBON

#endif // !defined(AFX_BCGPRIBBONPALETTEBUTTON_H__EE6BE1A6_CA76_4764_9A27_C5128583BEB6__INCLUDED_)

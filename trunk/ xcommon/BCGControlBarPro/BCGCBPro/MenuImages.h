//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2000 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************

// MenuImages.h: interface for the CMenuImages class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUIMAGES_H__DE028D41_36EB_11D3_95C5_00A0C9289F1B__INCLUDED_)
#define AFX_MENUIMAGES_H__DE028D41_36EB_11D3_95C5_00A0C9289F1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGCBPro.h"
#include "BCGPToolBarImages.h"

class BCGCBPRODLLEXPORT CMenuImages  
{
public:
	enum IMAGES_IDS
	{
		IdArowDown,
		IdArowRight,
		IdCheck,
		IdMinimize,
		IdRestore,
		IdClose,
		IdMaximize,
		IdArowUp,
		IdArowShowAll,
		IdArowLeft,
		IdCloseSmall,
		IdMoreButtons,
		IdRadio,
		IdArowDownLarge,
		IdArowRightLarge,
		IdPinHorz,
		IdPinVert,
		IdArowLeftLarge,
		IdArowFirst,
		IdArowLast,
		IdArowRightTab3d,
		IdArowLeftTab3d,
		IdArowRightDsbldTab3d,
		IdArowLeftDsbldTab3d,
		IdArowUpLarge,
		IdArowPageLeft,
		IdArowPageRight,
		IdArowBack,
		IdArowForward,
		IdCustomizeArowDown,
		IdCustomizeArowLeft,
		IdCustomizeMoreButtonsHorz,
		IdCustomizeMoreButtonsVert,
		IdCustomizeArowDownBold,
		IdCloseBold,
		IdLaunchArrow,
	};

	enum IMAGE_STATE
	{
		ImageBlack,
		ImageGray,
		ImageLtGray,
		ImageWhite,
		ImageDkGray,
	};

	static void Draw (	CDC* pDC, CMenuImages::IMAGES_IDS id, const CPoint& ptImage,
		CMenuImages::IMAGE_STATE state = CMenuImages::ImageBlack,
						const CSize& sizeImage = CSize (0, 0));
	static void Draw (	CDC* pDC, CMenuImages::IMAGES_IDS id, const CRect& rectImage,
		CMenuImages::IMAGE_STATE state = CMenuImages::ImageBlack,
						const CSize& sizeImage = CSize (0, 0));
	static CSize Size()
	{
		Initialize ();
		return m_ImagesBlack.GetImageSize ();
	}

	static void SetColor (	CMenuImages::IMAGE_STATE state,
							COLORREF color);	// color == -1: default

	static void CleanUp ();

protected:
	static BOOL Initialize ();
	static void CreateCopy (CBCGPToolBarImages& images, COLORREF clr);

	static CBCGPToolBarImages	m_ImagesBlack;
	static CBCGPToolBarImages	m_ImagesDkGray;
	static CBCGPToolBarImages	m_ImagesGray;
	static CBCGPToolBarImages	m_ImagesLtGray;
	static CBCGPToolBarImages	m_ImagesWhite;
};

#endif // !defined(AFX_MENUIMAGES_H__DE028D41_36EB_11D3_95C5_00A0C9289F1B__INCLUDED_)

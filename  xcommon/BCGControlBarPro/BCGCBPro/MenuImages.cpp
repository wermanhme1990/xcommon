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

// MenuImages.cpp: implementation of the CMenuImages class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MenuImages.h"
#include "BCGPLocalResource.h"
#include "bcgprores.h"
#include "bcgglobals.h"

static const COLORREF clrTransparent = RGB (255, 0, 255);
static const int iImageWidth = 9;
static const int iImageHeight = 9;

CBCGPToolBarImages CMenuImages::m_ImagesBlack;
CBCGPToolBarImages CMenuImages::m_ImagesDkGray;
CBCGPToolBarImages CMenuImages::m_ImagesGray;
CBCGPToolBarImages CMenuImages::m_ImagesLtGray;
CBCGPToolBarImages CMenuImages::m_ImagesWhite;

BOOL CMenuImages::Initialize ()
{
	if (m_ImagesBlack.IsValid ())
	{
		return TRUE;
	}

	CBCGPLocalResource locaRes;
	m_ImagesBlack.SetImageSize (CSize (iImageWidth, iImageHeight));
	if (!m_ImagesBlack.Load (globalData.Is32BitIcons () ? IDB_BCGBARRES_MENU_IMAGES24 : IDB_BCGBARRES_MENU_IMAGES))
	{
		TRACE(_T("CMenuImages. Can't load menu images %x\n"), IDB_BCGBARRES_MENU_IMAGES);
		return FALSE;
	}
	
	if (m_ImagesBlack.IsRTL ())
	{
		m_ImagesBlack.Mirror ();
	}

	m_ImagesBlack.SetTransparentColor (clrTransparent);

	CreateCopy (m_ImagesGray, RGB (128, 128, 128));
	CreateCopy (m_ImagesDkGray, RGB (72, 72, 72));
	CreateCopy (m_ImagesLtGray, RGB (192, 192, 192));
	CreateCopy (m_ImagesWhite, RGB (255, 255, 255));
	
	return TRUE;
}
//****************************************************************************************
void CMenuImages::Draw (CDC* pDC, IMAGES_IDS id, const CPoint& ptImage,
						CMenuImages::IMAGE_STATE state,
						const CSize& sizeImage/* = CSize (0, 0)*/)
{
	if (!Initialize ())
	{
		return;
	}

	CBCGPDrawState ds;

	CBCGPToolBarImages& images = (state == ImageBlack) ? m_ImagesBlack :
					(state == ImageGray) ? m_ImagesGray : 
					(state == ImageDkGray) ? m_ImagesDkGray : 
					(state == ImageLtGray) ? m_ImagesLtGray : m_ImagesWhite;

	images.PrepareDrawImage (ds, sizeImage);
	images.Draw (pDC, ptImage.x, ptImage.y, id);
	images.EndDrawImage (ds);
}
//****************************************************************************************
void CMenuImages::Draw (CDC* pDC, IMAGES_IDS id, const CRect& rectImage,
						CMenuImages::IMAGE_STATE state,
						const CSize& sizeImageDest/* = CSize (0, 0)*/)
{
	const CSize sizeImage = 
		(sizeImageDest == CSize (0, 0)) ? Size () : sizeImageDest;

	CPoint ptImage (
		rectImage.left + (rectImage.Width () - sizeImage.cx) / 2 + ((rectImage.Width () - sizeImage.cx) % 2), 
		rectImage.top + (rectImage.Height () - sizeImage.cy) / 2 + ((rectImage.Height () - sizeImage.cy) % 2));

	Draw (pDC, id, ptImage, state, sizeImageDest);
}
//*************************************************************************************
void CMenuImages::CleanUp ()
{
	if (m_ImagesBlack.GetCount () > 0)
	{
		m_ImagesBlack.Clear ();
		m_ImagesGray.Clear ();
		m_ImagesDkGray.Clear ();
		m_ImagesLtGray.Clear ();
		m_ImagesWhite.Clear ();
	}
}
//***********************************************************************************
void CMenuImages::CreateCopy (CBCGPToolBarImages& images, COLORREF clr)
{
	m_ImagesBlack.CopyTo (images);
	images.MapTo3dColors (TRUE, RGB (0, 0, 0), clr);
}
//***********************************************************************************
void CMenuImages::SetColor (CMenuImages::IMAGE_STATE state,
							COLORREF color)
{
	Initialize ();

	CBCGPLocalResource locaRes;

	CBCGPToolBarImages imagesTmp;

	imagesTmp.SetImageSize (m_ImagesBlack.GetImageSize ());
	imagesTmp.Load (globalData.Is32BitIcons () ? IDB_BCGBARRES_MENU_IMAGES24 : IDB_BCGBARRES_MENU_IMAGES);
	imagesTmp.SetTransparentColor (clrTransparent);
	
	if (imagesTmp.IsRTL ())
	{
		CBCGPToolBarImages::MirrorBitmap (imagesTmp.m_hbmImageWell, imagesTmp.GetImageSize ().cx);
	}

	CBCGPToolBarImages& images = (state == ImageBlack) ? m_ImagesBlack :
					(state == ImageGray) ? m_ImagesGray : 
					(state == ImageDkGray) ? m_ImagesDkGray : 
					(state == ImageLtGray) ? m_ImagesLtGray : m_ImagesWhite;

	if (color != (COLORREF)-1)
	{
		imagesTmp.MapTo3dColors (TRUE, RGB (0, 0, 0), color);
	}

	images.Clear ();
	imagesTmp.CopyTo (images);
}

//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2006 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPControlRenderer.h: interface for the CBCGPControlRenderer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPCONTROLRENDERER_H__CE281511_C403_4979_87C2_0E9F36FDF67D__INCLUDED_)
#define AFX_BCGPCONTROLRENDERER_H__CE281511_C403_4979_87C2_0E9F36FDF67D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGCBPro.h"
#include "BCGPToolBarImages.h"

class BCGCBPRODLLEXPORT CBCGPControlRendererParams
{
public:
	UINT		m_uiBmpResID;
	CRect		m_rectImage;
	CRect		m_rectCorners;
	CRect		m_rectSides;
	CRect		m_rectInter;
	COLORREF	m_clrTransparent;
	BOOL		m_bPreMultiplyCheck;

	CBCGPControlRendererParams ()
	{
		CommonInit ();
	}

	CBCGPControlRendererParams (UINT uiBmpResID,
					const CRect& rtImage, 
					const CRect& rtCorners,
					const CRect& rtSides = CRect (0, 0, 0, 0),
                    const CRect& rtInner = CRect (0, 0, 0, 0))
	{
		CommonInit ();

		m_uiBmpResID	 = uiBmpResID;
		m_rectImage      = rtImage;
		m_rectCorners	 = rtCorners;
		m_rectSides		 = rtSides;
		m_rectInter      = rtInner;
	}

	CBCGPControlRendererParams (UINT uiBmpResID,
					COLORREF clrTransparent,
					const CRect& rtImage, 
					const CRect& rtCorners,
					const CRect& rtSides = CRect (0, 0, 0, 0),
                    const CRect& rtInner = CRect (0, 0, 0, 0),
					BOOL bPreMultiplyCheck = TRUE)
	{
		CommonInit ();

		m_uiBmpResID	 = uiBmpResID;
		m_rectImage      = rtImage;
		m_rectCorners	 = rtCorners;
		m_rectSides		 = rtSides;
		m_rectInter      = rtInner;
		m_clrTransparent = clrTransparent;
		m_bPreMultiplyCheck = bPreMultiplyCheck;
	}

	void CommonInit ()
	{
		m_uiBmpResID = 0;
		m_rectImage.SetRectEmpty ();
		m_rectCorners.SetRectEmpty ();
		m_rectSides.SetRectEmpty ();
		m_rectInter.SetRectEmpty ();
		m_clrTransparent = CLR_DEFAULT;
		m_bPreMultiplyCheck = TRUE;
	}

	CBCGPControlRendererParams& operator = (const CBCGPControlRendererParams& src)
	{
		m_uiBmpResID	 = src.m_uiBmpResID;
		m_rectImage      = src.m_rectImage;
		m_rectCorners	 = src.m_rectCorners;
		m_rectSides		 = src.m_rectSides;
		m_rectInter      = src.m_rectInter;
		m_clrTransparent = src.m_clrTransparent;
		m_bPreMultiplyCheck = src.m_bPreMultiplyCheck;

		return *this;
	}
};

class BCGCBPRODLLEXPORT CBCGPControlRenderer : public CObject  
{
	DECLARE_DYNCREATE(CBCGPControlRenderer)

public:
	CBCGPControlRenderer();
	virtual ~CBCGPControlRenderer();

// Operations:
public:
	virtual BOOL Create (const CBCGPControlRendererParams& params, BOOL bFlipvert = FALSE);

	virtual void Draw (CDC* pDC, CRect rect, UINT index = 0, BYTE alphaSrc = 255);
	virtual void DrawFrame (CDC* pDC, CRect rect, UINT index = 0, BYTE alphaSrc = 255);
	virtual void FillInterior (CDC* pDC, CRect rect, UINT index = 0, BYTE alphaSrc = 255);
	virtual void FillInterior (CDC* pDC, CRect rect, CBCGPToolBarImages::ImageAlignHorz horz,
		CBCGPToolBarImages::ImageAlignVert vert, UINT index = 0, BYTE alphaSrc = 255);

	virtual void OnSysColorChange ();
	const CBCGPControlRendererParams& GetParams () const
	{
		return m_Params;
	}

	BOOL IsValid () const
	{
		return m_Bitmap.IsValid ();
	}

	virtual void CleanUp ();
	
	int GetImageCount () const
	{
		CRect rectImage (m_Params.m_rectImage);

		if (m_Bitmap.GetCount () == 1)
		{
			HBITMAP hBmp = m_Bitmap.GetImageWell ();
			BITMAP bmp;

			if (::GetObject (hBmp, sizeof (BITMAP), &bmp) == sizeof(BITMAP))
			{
				return bmp.bmHeight / m_Params.m_rectImage.Height ();
			}

			return 0;
		}

		return m_Bitmap.GetCount ();
	}

// Attributes:
protected:
	CBCGPToolBarImages			m_Bitmap;
	CBCGPControlRendererParams	m_Params;
    BOOL						m_bMirror;
};

#endif // !defined(AFX_BCGPCONTROLRENDERER_H__CE281511_C403_4979_87C2_0E9F36FDF67D__INCLUDED_)

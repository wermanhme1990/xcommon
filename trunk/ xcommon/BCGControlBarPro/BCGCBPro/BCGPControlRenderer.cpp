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
// BCGPControlRenderer.cpp: implementation of the CBCGPControlRenderer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPControlRenderer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CBCGPControlRenderer, CObject)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPControlRenderer::CBCGPControlRenderer()
{
	m_bMirror = FALSE;
}
//*********************************************************************************
CBCGPControlRenderer::~CBCGPControlRenderer()
{
	CleanUp ();
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

BOOL CBCGPControlRenderer::Create (const CBCGPControlRendererParams& params, BOOL bFlipvert /*= FALSE*/)
{
	CleanUp ();

	m_Params = params;

	if (m_Params.m_uiBmpResID != 0)
	{
		m_Bitmap.SetImageSize (m_Params.m_rectImage.Size ());
		m_Bitmap.SetPreMultiplyAutoCheck (m_Params.m_bPreMultiplyCheck);
		m_Bitmap.SetMapTo3DColors (FALSE);
		m_Bitmap.Load (m_Params.m_uiBmpResID);

		if (bFlipvert)
		{
			m_Bitmap.MirrorVert ();
		}

		if (m_Params.m_clrTransparent != CLR_DEFAULT)
		{
			m_Bitmap.SetTransparentColor (m_Params.m_clrTransparent);
		}

        if (CBCGPToolBarImages::IsRTL () && m_Bitmap.GetImageWell () != NULL)
        {
            BITMAP bmp;
            if (::GetObject (m_Bitmap.GetImageWell (), sizeof (BITMAP), &bmp) != 0)
            {
                if (bmp.bmBitsPixel == 32)
                {
                    m_bMirror = m_Bitmap.Mirror ();
                }
            }
        }

		if (m_Params.m_rectSides.IsRectNull ())
		{
			m_Params.m_rectSides = m_Params.m_rectCorners;
		}

		if (m_Params.m_rectInter.IsRectNull ())
		{
			m_Params.m_rectInter = CRect (CPoint (0, 0), m_Params.m_rectImage.Size ());
			m_Params.m_rectInter.left   += m_Params.m_rectCorners.left;
			m_Params.m_rectInter.top    += m_Params.m_rectCorners.top;
			m_Params.m_rectInter.right  -= m_Params.m_rectCorners.right;
			m_Params.m_rectInter.bottom -= m_Params.m_rectCorners.bottom;
		}

		if (bFlipvert)
		{
			long temp;
			temp = m_Params.m_rectCorners.top;
			m_Params.m_rectCorners.top = m_Params.m_rectCorners.bottom;
			m_Params.m_rectCorners.bottom = temp;

			temp = m_Params.m_rectSides.top;
			m_Params.m_rectSides.top = m_Params.m_rectSides.bottom;
			m_Params.m_rectSides.bottom = temp;

			long height = m_Params.m_rectImage.Height ();
			temp = m_Params.m_rectInter.top;
			m_Params.m_rectInter.top = height - m_Params.m_rectInter.bottom;
			m_Params.m_rectInter.bottom = height - temp;
		}
	}

	return TRUE;
}
//*********************************************************************************
void CBCGPControlRenderer::CleanUp ()
{
	m_Bitmap.Clear ();
	m_Bitmap.SetTransparentColor ((COLORREF)(-1));

	CBCGPControlRendererParams emptyParams;
	m_Params = emptyParams;
    m_bMirror = FALSE;
}
//*********************************************************************************
void CBCGPControlRenderer::Draw (CDC* pDC, CRect rect, UINT index, BYTE alphaSrc/* = 255*/)
{
	CRect rectInter (rect);
	rectInter.left   += m_Params.m_rectSides.left;
	rectInter.top    += m_Params.m_rectSides.top;
	rectInter.right  -= m_Params.m_rectSides.right;
	rectInter.bottom -= m_Params.m_rectSides.bottom;

	FillInterior (pDC, rectInter, index, alphaSrc);

	DrawFrame (pDC, rect, index, alphaSrc);
}
//*********************************************************************************
void CBCGPControlRenderer::DrawFrame (CDC* pDC, CRect rect, UINT index, BYTE alphaSrc/* = 255*/)
{
	struct XHVTypes
	{
		CBCGPToolBarImages::ImageAlignHorz horz;
		CBCGPToolBarImages::ImageAlignVert vert;
	};

	XHVTypes corners[4] = 
	{
		{CBCGPToolBarImages::ImageAlignHorzLeft , CBCGPToolBarImages::ImageAlignVertTop},
		{CBCGPToolBarImages::ImageAlignHorzRight, CBCGPToolBarImages::ImageAlignVertTop},
		{CBCGPToolBarImages::ImageAlignHorzLeft , CBCGPToolBarImages::ImageAlignVertBottom},
		{CBCGPToolBarImages::ImageAlignHorzRight, CBCGPToolBarImages::ImageAlignVertBottom}
	};

	XHVTypes sides[4] = 
	{
		{CBCGPToolBarImages::ImageAlignHorzLeft   , CBCGPToolBarImages::ImageAlignVertStretch},
		{CBCGPToolBarImages::ImageAlignHorzRight  , CBCGPToolBarImages::ImageAlignVertStretch},
		{CBCGPToolBarImages::ImageAlignHorzStretch, CBCGPToolBarImages::ImageAlignVertTop},
		{CBCGPToolBarImages::ImageAlignHorzStretch, CBCGPToolBarImages::ImageAlignVertBottom}
	};

	CRect rectImage (m_Params.m_rectImage);
	int ind = index;
	if (m_Bitmap.GetCount () == 1)
	{
		rectImage.OffsetRect (0, m_Params.m_rectImage.Size ().cy * ind);
		ind = 0;
	}

	CRect rt  (rect);
	CRect rectCorners (m_Params.m_rectCorners);
	CRect rectSides   (m_Params.m_rectSides);

	rt.left   += rectCorners.left;
	rt.top    += rectCorners.top;
	rt.right  -= rectCorners.right;
	rt.bottom -= rectCorners.bottom;

	if (rt.Width () > 0 || rt.Height () > 0)
	{
		if (rt.Height () > 0)
		{
			if (rectSides.left > 0)
			{
				CRect r (rt);
				r.left  = rect.left;
				r.right = r.left + rectSides.left;

                CRect rectPart;
                if (m_bMirror)
                {
				    rectPart = CRect (rectImage.right - rectSides.left, 
					    rectImage.top + rectCorners.top, rectImage.right, rectImage.bottom - rectCorners.bottom);
                }
                else
                {
				    rectPart = CRect (rectImage.left, 
					    rectImage.top + rectCorners.top, rectImage.left + rectSides.left, rectImage.bottom - rectCorners.bottom);
                }

				m_Bitmap.DrawEx (pDC, r, ind, sides[0].horz, sides[0].vert, rectPart, alphaSrc);
			}

			if (rectSides.right > 0)
			{
				CRect r (rt);
				r.right = rect.right;
				r.left  = r.right - rectSides.right;

				CRect rectPart;
                if (m_bMirror)
                {
                    rectPart = CRect (rectImage.left, 
					    rectImage.top + rectCorners.top, rectImage.left + rectSides.right, rectImage.bottom - rectCorners.bottom);
                }
                else
                {
                    rectPart = CRect (rectImage.right - rectSides.right, 
    					rectImage.top + rectCorners.top, rectImage.right, rectImage.bottom - rectCorners.bottom);
                }

				m_Bitmap.DrawEx (pDC, r, ind, sides[1].horz, sides[1].vert, rectPart, alphaSrc);
			}
		}

		if (rt.Width () > 0)
		{
			if (rectSides.top > 0)
			{
				CRect r (rt);
				r.top    = rect.top;
				r.bottom = r.top + rectSides.top;

				CRect rectPart;
                if (m_bMirror)
                {
                    rectPart = CRect (rectImage.left + rectCorners.left, 
					    rectImage.top, rectImage.right - rectCorners.right, rectImage.top + rectSides.top);
                }
                else
                {
                    rectPart = CRect (rectImage.left + rectCorners.left, 
					    rectImage.top, rectImage.right - rectCorners.right, rectImage.top + rectSides.top);
                }

				m_Bitmap.DrawEx (pDC, r, ind, sides[2].horz, sides[2].vert, rectPart, alphaSrc);
			}

			if (rectSides.bottom > 0)
			{
				CRect r (rt);
				r.bottom = rect.bottom;
				r.top    = r.bottom - rectSides.bottom;
				
				CRect rectPart;
                if (m_bMirror)
                {
                    rectPart = CRect (rectImage.left + rectCorners.right, 
    					rectImage.bottom - rectSides.bottom, rectImage.right - rectCorners.left, rectImage.bottom);
                }
                else
                {
                    rectPart = CRect (rectImage.left + rectCorners.left, 
    					rectImage.bottom - rectSides.bottom, rectImage.right - rectCorners.right, rectImage.bottom);
                }

				m_Bitmap.DrawEx (pDC, r, ind, sides[3].horz, sides[3].vert, rectPart, alphaSrc);
			}
		}

		if (rectCorners.left > 0 && rectCorners.top > 0)
		{
			CRect rectPart;
            if (m_bMirror)
            {
                rectPart = CRect (CPoint (rectImage.right - rectCorners.left, rectImage.top), 
					CSize (rectCorners.left, rectCorners.top));
            }
            else
            {
                rectPart = CRect (CPoint (rectImage.left, rectImage.top), 
					CSize (rectCorners.left, rectCorners.top));
            }

			m_Bitmap.DrawEx (pDC, rect, ind, corners[0].horz, corners[0].vert, rectPart, alphaSrc);
		}

		if (rectCorners.right > 0 && rectCorners.top > 0)
		{
			CRect rectPart;
            if (m_bMirror)
            {
                rectPart = CRect (CPoint (rectImage.left, rectImage.top), 
					CSize (rectCorners.right, rectCorners.top));
            }
            else
            {
                rectPart = CRect (CPoint (rectImage.right - rectCorners.right, rectImage.top), 
					CSize (rectCorners.right, rectCorners.top));
            }

			m_Bitmap.DrawEx (pDC, rect, ind, corners[1].horz, corners[1].vert, rectPart, alphaSrc);
		}

		if (rectCorners.left > 0 && rectCorners.bottom > 0)
		{
			CRect rectPart;
            if (m_bMirror)
            {
                rectPart = CRect (CPoint (rectImage.right - rectCorners.left, rectImage.bottom - rectCorners.bottom), 
					CSize (rectCorners.left, rectCorners.bottom));
            }
            else
            {
                rectPart = CRect (CPoint (rectImage.left, rectImage.bottom - rectCorners.bottom), 
					CSize (rectCorners.left, rectCorners.bottom));
            }

			m_Bitmap.DrawEx (pDC, rect, ind, corners[2].horz, corners[2].vert, rectPart, alphaSrc);
		}

		if (rectCorners.right > 0 && rectCorners.bottom > 0)
		{
			CRect rectPart;
            if (m_bMirror)
            {
                rectPart = CRect (CPoint (rectImage.left, rectImage.bottom - rectCorners.bottom), 
					CSize (rectCorners.right, rectCorners.bottom));
            }
            else
            {
                rectPart = CRect (CPoint (rectImage.right - rectCorners.right, rectImage.bottom - rectCorners.bottom), 
					CSize (rectCorners.right, rectCorners.bottom));
            }

			m_Bitmap.DrawEx (pDC, rect, ind, corners[3].horz, corners[3].vert, rectPart, alphaSrc);
		}
	}
}
//*********************************************************************************
void CBCGPControlRenderer::FillInterior (CDC* pDC, CRect rect, CBCGPToolBarImages::ImageAlignHorz horz,
		CBCGPToolBarImages::ImageAlignVert vert, UINT index, BYTE alphaSrc/* = 255*/)
{
	if (m_Params.m_rectInter.IsRectEmpty ())
	{
		return;
	}

	CRect rectImage (m_Params.m_rectInter);
	rectImage.OffsetRect (m_Params.m_rectImage.TopLeft ());

	int ind = index;
	if (m_Bitmap.GetCount () == 1)
	{
		rectImage.OffsetRect (0, m_Params.m_rectImage.Size ().cy * ind);
		ind = 0;
	}

	m_Bitmap.DrawEx (pDC, rect, ind, horz, vert, rectImage, alphaSrc);
}
//*********************************************************************************
void CBCGPControlRenderer::FillInterior (CDC* pDC, CRect rect, UINT index, BYTE alphaSrc/* = 255*/)
{
	FillInterior (pDC, rect, CBCGPToolBarImages::ImageAlignHorzStretch,
		CBCGPToolBarImages::ImageAlignVertStretch, index, alphaSrc);
}
//*********************************************************************************
void CBCGPControlRenderer::OnSysColorChange ()
{
	if (m_Bitmap.GetImageWell () != NULL)
	{
		m_Bitmap.OnSysColorChange ();
	}
}

/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XDC.cpp					
文件内容:	
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/08/19
*********************************************************************/

#include "stdafx.h"
#include "XDC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
#define COLORREF_NULL   (COLORREF)-1

CXTransparentBitmap::CXTransparentBitmap(HBITMAP hBitmap)
: m_hBitmap(hBitmap)
{
}

COLORREF CXTransparentBitmap::GetTransparentColor() const
{
	CBitmap* pBitmap = CBitmap::FromHandle(m_hBitmap);
	if (pBitmap != NULL)
	{
		CXCompatibleDC dc(NULL, pBitmap);
		return dc.GetPixel(0, 0);
	}
	return (COLORREF)-1;
}


HICON CXTransparentBitmap::ConvertToIcon() const
{
	if (m_hBitmap == NULL)
		return NULL;

	COLORREF crTransparent = GetTransparentColor();

	BITMAP bmp;
	if (!::GetObject(m_hBitmap, sizeof(BITMAP), &bmp))
		return NULL;

	if (bmp.bmHeight == 0 || bmp.bmWidth == 0)
		return NULL;

	CImageList il;
	il.Create(bmp.bmWidth, bmp.bmHeight, ILC_COLOR24 | ILC_MASK, 0, 1);
	il.Add(CBitmap::FromHandle(m_hBitmap), crTransparent);

	ASSERT(il.GetImageCount() == 1);

	return il.ExtractIcon(0);
}

//===========================================================================
// CXBufferDC class
//===========================================================================

CXBufferDC::CXBufferDC(HDC hDestDC, const CRect& rcPaint) : m_hDestDC (hDestDC)
{
	m_rect = rcPaint;
	Attach (::CreateCompatibleDC (m_hDestDC));
	if (!m_hDC)
		return;

	m_bitmap.Attach (::CreateCompatibleBitmap(
		m_hDestDC, m_rect.right, m_rect.bottom));
	m_hOldBitmap = ::SelectObject (m_hDC, m_bitmap);
}

CXBufferDC::CXBufferDC(CPaintDC& paintDC)
{
	m_hDestDC = paintDC.GetSafeHdc();
	m_rect = paintDC.m_ps.rcPaint;

	Attach (::CreateCompatibleDC (m_hDestDC));
	if (!m_hDC)
		return;

	m_bitmap.Attach (::CreateCompatibleBitmap(
		m_hDestDC, max(1, m_rect.right), max(1, m_rect.bottom)));
	m_hOldBitmap = ::SelectObject (m_hDC, m_bitmap);

	CRgn rgn;
	rgn.CreateRectRgnIndirect(&m_rect);

	SelectClipRgn(&rgn);
}

CXBufferDC::~CXBufferDC()
{
	if (!m_hDC)
		return;

	if (m_hDestDC)
	{
		::BitBlt (m_hDestDC, m_rect.left, m_rect.top, m_rect.Width(),
			m_rect.Height(), m_hDC, m_rect.left, m_rect.top, SRCCOPY);
	}
	::SelectObject (m_hDC, m_hOldBitmap);
}
void CXBufferDC::Discard()
{
	m_hDestDC = 0;
}

CDC* CXBufferDC::GetDestDC()
{
	return CDC::FromHandle(m_hDestDC);
}

//===========================================================================
// CXBufferDC class
//===========================================================================

CXBufferDCEx::CXBufferDCEx(HDC hDestDC, const CRect rcPaint) : m_hDestDC (hDestDC)
{
	m_rect = rcPaint;
	Attach (::CreateCompatibleDC (m_hDestDC));
	m_bitmap = ::CreateCompatibleBitmap(
		m_hDestDC, m_rect.Width(), m_rect.Height());

	m_hOldBitmap = ::SelectObject (m_hDC, m_bitmap);

	SetViewportOrg(-rcPaint.left, -rcPaint.top);
}

CXBufferDCEx::~CXBufferDCEx()
{
	SetViewportOrg(0, 0);

	::BitBlt (m_hDestDC, m_rect.left, m_rect.top, m_rect.Width(),
		m_rect.Height(), m_hDC, 0, 0, SRCCOPY);
	::SelectObject (m_hDC, m_hOldBitmap);
	::DeleteObject(m_bitmap);
}

//===========================================================================
// CXBitmapDC class
//===========================================================================

CXBitmapDC::CXBitmapDC(CDC* pDC, CBitmap* pBitmap)
: m_hDC(pDC->GetSafeHdc())
{
	m_hOldBitmap = SelectObject(m_hDC, pBitmap->GetSafeHandle());
}


CXBitmapDC::CXBitmapDC(CDC* pDC, HBITMAP hBitmap)
: m_hDC(pDC->GetSafeHdc())
{
	m_hOldBitmap = SelectObject(m_hDC, hBitmap);
}

CXBitmapDC::~CXBitmapDC()
{
	::SelectObject(m_hDC, m_hOldBitmap);
}

void CXBitmapDC::SetBitmap(CBitmap* pBitmap)
{
	::SelectObject(m_hDC, m_hOldBitmap);
	m_hOldBitmap = SelectObject(m_hDC, pBitmap->GetSafeHandle());
}

//===========================================================================
// CXFontDC class
//===========================================================================

CXFontDC::CXFontDC(CDC* pDC, CFont* pFont)
{
	ASSERT(pDC);

	m_pDC = pDC;
	m_pOldFont = NULL;
	m_clrOldTextColor = COLORREF_NULL;

	if (pFont)
	{
		SetFont(pFont);
	}
}

CXFontDC::CXFontDC(CDC* pDC, CFont* pFont, COLORREF clrTextColor)
{
	ASSERT(pDC);
	ASSERT(clrTextColor != COLORREF_NULL);

	m_pDC = pDC;
	m_pOldFont = NULL;
	m_clrOldTextColor = COLORREF_NULL;


	if (pFont)
	{
		SetFont(pFont);
	}

	SetColor(clrTextColor);
}

CXFontDC::~CXFontDC()
{
	ReleaseFont();
	ReleaseColor();
}

void CXFontDC::SetFont(CFont* pFont)
{
	if (m_pDC && pFont)
	{
		CFont* pFontPrev = m_pDC->SelectObject(pFont);

		if (!m_pOldFont && pFontPrev)
		{
			m_pOldFont = pFontPrev;
		}
	}
}

void CXFontDC::SetColor(COLORREF clrTextColor)
{
	ASSERT(clrTextColor != COLORREF_NULL);
	ASSERT(m_pDC);

	if (m_pDC && clrTextColor != COLORREF_NULL)
	{
		COLORREF clrTextColorPrev= m_pDC->SetTextColor(clrTextColor);

		if (m_clrOldTextColor == COLORREF_NULL)
		{
			m_clrOldTextColor = clrTextColorPrev;
		}
	}
}

void CXFontDC::SetFontColor(CFont* pFont, COLORREF clrTextColor)
{
	SetFont(pFont);
	SetColor(clrTextColor);
}

void CXFontDC::ReleaseFont()
{
	ASSERT(m_pDC);
	if (m_pDC && m_pOldFont)
	{
		m_pDC->SelectObject(m_pOldFont);
		m_pOldFont = NULL;
	}
}

void CXFontDC::ReleaseColor()
{
	ASSERT(m_pDC);
	if (m_pDC && m_clrOldTextColor != COLORREF_NULL)
	{
		m_pDC->SetTextColor(m_clrOldTextColor);
		m_clrOldTextColor = COLORREF_NULL;
	}
}

//===========================================================================
// CXPenDC class
//===========================================================================

CXPenDC::CXPenDC(CDC* pDC, CPen* pPen)
: m_hDC(pDC->GetSafeHdc())
{
	m_hOldPen = (HPEN)::SelectObject(m_hDC, pPen->GetSafeHandle());
}

CXPenDC::CXPenDC(HDC hDC, COLORREF crColor)
: m_hDC (hDC)
{
	VERIFY(m_pen.CreatePen (PS_SOLID, 1, crColor));
	m_hOldPen = (HPEN)::SelectObject (m_hDC, m_pen);
}

CXPenDC::~CXPenDC ()
{
	::SelectObject (m_hDC, m_hOldPen);
}

void CXPenDC::Color(COLORREF crColor)
{
	::SelectObject (m_hDC, m_hOldPen);
	VERIFY(m_pen.DeleteObject());
	VERIFY(m_pen.CreatePen (PS_SOLID, 1, crColor));
	m_hOldPen = (HPEN)::SelectObject (m_hDC, m_pen);
}

COLORREF CXPenDC::Color()
{
	LOGPEN logPen;
	m_pen.GetLogPen(&logPen);
	return logPen.lopnColor;
}

//===========================================================================
// CXBrushDC class
//===========================================================================

CXBrushDC::CXBrushDC(HDC hDC, COLORREF crColor)
: m_hDC (hDC)
{
	VERIFY(m_brush.CreateSolidBrush (crColor));
	m_hOldBrush = (HBRUSH)::SelectObject (m_hDC, m_brush);
}

CXBrushDC::~CXBrushDC()
{
	::SelectObject(m_hDC, m_hOldBrush);
}

void CXBrushDC::Color(COLORREF crColor)
{
	::SelectObject(m_hDC, m_hOldBrush);
	VERIFY(m_brush.DeleteObject());
	VERIFY(m_brush.CreateSolidBrush(crColor));
	m_hOldBrush = (HBRUSH)::SelectObject (m_hDC, m_brush);
}

//===========================================================================
// CXCompatibleDC class
//===========================================================================

CXCompatibleDC::CXCompatibleDC(CDC* pDC, CBitmap* pBitmap)
{
	CreateCompatibleDC(pDC);
	m_hOldBitmap = (HBITMAP)::SelectObject(GetSafeHdc(), pBitmap->GetSafeHandle());
}

CXCompatibleDC::CXCompatibleDC(CDC* pDC, HBITMAP hBitmap)
{
	CreateCompatibleDC(pDC);
	m_hOldBitmap = (HBITMAP)::SelectObject(GetSafeHdc(), hBitmap);
}

CXCompatibleDC::~CXCompatibleDC()
{
	::SelectObject(GetSafeHdc(), m_hOldBitmap);
	DeleteDC();
}

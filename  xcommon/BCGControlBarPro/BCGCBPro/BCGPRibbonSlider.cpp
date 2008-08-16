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
// BCGPRibbonSlider.cpp: implementation of the CBCGPRibbonSlider class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPRibbonSlider.h"
#include "BCGPVisualManager.h"
#include "BCGPRibbonBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef BCGP_EXCLUDE_RIBBON

IMPLEMENT_DYNCREATE(CBCGPRibbonSlider, CBCGPBaseRibbonElement)

static const int cySliderHeight = 18;
static const int cxThumbWidth = 10; 

static const int nThumbIndex = 0;
static const int nSliderIndex = 1;
static const int nZoomInIndex = 2;
static const int nZoomOutIndex = 3;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonSlider::CBCGPRibbonSlider()
{
	CommonInit ();
}
//*****************************************************************************************************
CBCGPRibbonSlider::CBCGPRibbonSlider(
		UINT	nID, 
		int		nWidth)
{
	CommonInit ();

	m_nID = nID;
	m_nWidth = nWidth;
}
//*****************************************************************************************************
CBCGPRibbonSlider::~CBCGPRibbonSlider()
{
}
//*****************************************************************************************************
void CBCGPRibbonSlider::CommonInit ()
{
	m_nMin = 0;
	m_nMax = 100;
	m_nPos = 0;
	m_nPageSize = 10;
	m_bZoomButtons = FALSE;
	m_nWidth = 100;

	m_rectZoomOut.SetRectEmpty ();
	m_rectZoomIn.SetRectEmpty ();
	m_rectSlider.SetRectEmpty ();
	m_rectThumb.SetRectEmpty ();

	m_nHighlighted = -1;
	m_nPressed = -1;
}
//********************************************************************************
void CBCGPRibbonSlider::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (m_rect.IsRectEmpty ())
	{
		return;
	}

	if (m_bZoomButtons)
	{
		// Draw zoom buttons:
		CBCGPVisualManager::GetInstance ()->OnDrawRibbonSliderZoomButton (
			pDC, this, m_rectZoomOut, TRUE, 
			m_bIsHighlighted && m_nHighlighted == nZoomOutIndex,
			m_bIsPressed && m_nPressed == nZoomOutIndex, IsDisabled ());

		CBCGPVisualManager::GetInstance ()->OnDrawRibbonSliderZoomButton (
			pDC, this, m_rectZoomIn, FALSE, 
			m_bIsHighlighted && m_nHighlighted == nZoomInIndex,
			m_bIsPressed && m_nPressed == nZoomInIndex, IsDisabled ());
	}

	// Draw channel:
	CRect rectChannel = m_rectSlider;
	rectChannel.top = rectChannel.CenterPoint ().y - 1;
	rectChannel.bottom = rectChannel.top + 2;

	CBCGPVisualManager::GetInstance ()->OnDrawRibbonSliderChannel (
		pDC, this, rectChannel);

	// Draw thumb:
	CBCGPVisualManager::GetInstance ()->OnDrawRibbonSliderThumb (
		pDC, this, m_rectThumb,
		m_bIsHighlighted && (m_nHighlighted == nThumbIndex || m_nHighlighted == nSliderIndex),
		m_bIsPressed && m_nPressed == nThumbIndex, IsDisabled ());
}
//*****************************************************************************
void CBCGPRibbonSlider::CopyFrom (const CBCGPBaseRibbonElement& s)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement::CopyFrom (s);

	CBCGPRibbonSlider& src = (CBCGPRibbonSlider&) s;

	m_nMin = src.m_nMin;
	m_nMax = src.m_nMax;
	m_nPos = src.m_nPos;
	m_bZoomButtons = src.m_bZoomButtons;
	m_nWidth = src.m_nWidth;
}
//*****************************************************************************
CSize CBCGPRibbonSlider::GetRegularSize (CDC* /*pDC*/)
{
	ASSERT_VALID (this);

	CSize size (m_nWidth, cySliderHeight);

	if (m_bZoomButtons)
	{
		size.cx += 2 * size.cy;
	}

	return size;
}
//*****************************************************************************
void CBCGPRibbonSlider::SetRange (int nMin, int nMax)
{
	ASSERT_VALID (this);

	m_nMin = nMin;
	m_nMax = nMax;
}
//*****************************************************************************
void CBCGPRibbonSlider::SetPos (int nPos, BOOL bRedraw)
{
	ASSERT_VALID (this);

	m_nPos = min (max (m_nMin, nPos), m_nMax);
	SetThumbRect ();

	if (bRedraw)
	{
		Redraw ();
	}
}
//*****************************************************************************
void CBCGPRibbonSlider::SetPageSize (int nPageSize)
{
	ASSERT_VALID (this);
	m_nPageSize = nPageSize;
}
//*****************************************************************************
void CBCGPRibbonSlider::SetZoomButtons (BOOL bSet)
{
	ASSERT_VALID (this);
	m_bZoomButtons = bSet;
}
//*****************************************************************************
void CBCGPRibbonSlider::OnAfterChangeRect (CDC* pDC)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement::OnAfterChangeRect (pDC);

	m_rectZoomOut.SetRectEmpty ();
	m_rectZoomIn.SetRectEmpty ();
	m_rectThumb.SetRectEmpty ();

	m_rectSlider = m_rect;

	if (m_rect.IsRectEmpty ())
	{
		return;
	}

	if (m_bZoomButtons)
	{
		m_rectZoomOut = m_rectSlider;
		m_rectZoomOut.right = m_rectZoomOut.left + m_rectZoomOut.Height ();

		m_rectZoomIn = m_rectSlider;
		m_rectZoomIn.left = m_rectZoomIn.right - m_rectZoomIn.Height ();

		m_rectSlider.left = m_rectZoomOut.right;
		m_rectSlider.right = m_rectZoomIn.left;
	}

	m_rectSlider.DeflateRect (cxThumbWidth / 2, 0);

	SetThumbRect ();
}
//*****************************************************************************
void CBCGPRibbonSlider::SetThumbRect ()
{
	ASSERT_VALID (this);

	if (m_nMax <= m_nMin || m_rect.IsRectEmpty ())
	{
		m_rectThumb.SetRectEmpty ();
		return;
	}

	m_rectThumb = m_rectSlider;

	double dx = ((double) m_rectSlider.Width ()) / (m_nMax - m_nMin);
	int xOffset = (int) (.5 + dx * (m_nPos - m_nMin));

	m_rectThumb.left += xOffset - cxThumbWidth / 2;
	m_rectThumb.right = m_rectThumb.left + cxThumbWidth;
}
//*****************************************************************************
void CBCGPRibbonSlider::OnLButtonDown (CPoint point)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement::OnLButtonDown (point);

	if (!IsDisabled ())
	{
		m_nPressed = GetHitTest (point);
		Redraw ();
	}
}
//*****************************************************************************
void CBCGPRibbonSlider::OnLButtonUp (CPoint point)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement::OnLButtonUp (point);

	if (m_nPressed == m_nHighlighted && !IsDisabled ())
	{
		switch (m_nPressed)
		{
		case nZoomInIndex:
			SetPos (m_nPos + m_nPageSize);
			break;

		case nZoomOutIndex:
			SetPos (m_nPos - m_nPageSize);
			break;

		default:
			SetPos (GetPosFromPoint (point.x));
			break;
		}

		NotifyCommand ();
	}
}
//*****************************************************************************
void CBCGPRibbonSlider::OnMouseMove (CPoint point)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement::OnMouseMove (point);

	if (IsDisabled ())
	{
		return;
	}

	int nHighlightedOld = m_nHighlighted;

	m_nHighlighted = GetHitTest (point);

	if (nHighlightedOld != m_nHighlighted)
	{
		Redraw ();
	}

	if (m_bIsPressed)
	{
		if (m_nPressed == nThumbIndex)
		{
			int nPos = GetPosFromPoint (point.x);
			if (nPos != m_nPos)
			{
				SetPos (nPos);
				NotifyCommand ();
			}
		}
	}
}
//*****************************************************************************
int CBCGPRibbonSlider::GetHitTest (CPoint point) const
{
	ASSERT_VALID (this);

	if (m_rectThumb.PtInRect (point))
	{
		return nThumbIndex;
	}

	if (m_rectSlider.PtInRect (point))
	{
		return nSliderIndex;
	}

	if (m_rectZoomOut.PtInRect (point))
	{
		return nZoomOutIndex;
	}

	if (m_rectZoomIn.PtInRect (point))
	{
		return nZoomInIndex;
	}

	return -1;
}
//*****************************************************************************
int CBCGPRibbonSlider::GetPosFromPoint (int x)
{
	ASSERT_VALID (this);

	if (m_nMax <= m_nMin || m_rect.IsRectEmpty ())
	{
		return m_nMin;
	}

	double dx = ((double) m_rectSlider.Width ()) / (m_nMax - m_nMin);
	int xOffset = x - m_rectSlider.left;

	return m_nMin + (int) ((double) xOffset / dx);
}
//***************************************************************************
BOOL CBCGPRibbonSlider::IsAutoRepeatMode (int& /*nDelay*/) const
{
	ASSERT_VALID (this);
	return m_nPressed == nZoomInIndex || m_nPressed == nZoomOutIndex;
}
//***************************************************************************
BOOL CBCGPRibbonSlider::OnAutoRepeat ()
{
	ASSERT_VALID (this);

	if (m_bIsDisabled)
	{
		return FALSE;
	}

	if (m_nPressed == nZoomInIndex)
	{
		SetPos (m_nPos + m_nPageSize);
		NotifyCommand ();
	}
	else if (m_nPressed == nZoomOutIndex)
	{
		SetPos (m_nPos - m_nPageSize);
		NotifyCommand ();
	}

	return TRUE;
}
//******************************************************************************
void CBCGPRibbonSlider::NotifyCommand ()
{
	ASSERT_VALID (this);

	if (m_nID == 0 || m_nID == (UINT)-1)
	{
		return;
	}

	CBCGPRibbonBar* pRibbonBar = GetTopLevelRibbonBar ();
	if (pRibbonBar == NULL)
	{
		return;
	}

	ASSERT_VALID (pRibbonBar);

	CWnd* pWndParent = pRibbonBar->GetParent ();
	if (pWndParent == NULL)
	{
		return;
	}

	pWndParent->SendMessage (WM_COMMAND, m_nID);
}

#endif // BCGP_EXCLUDE_RIBBON


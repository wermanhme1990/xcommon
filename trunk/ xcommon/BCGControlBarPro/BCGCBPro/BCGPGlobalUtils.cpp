// BCGPGlobalUtils.cpp: implementation of the CBCGPGlobalUtils class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPGlobalUtils.h"
#include "BCGPDockManager.h"
#include "BCGPBarContainerManager.h"
#include "BCGPDockingControlBar.h"
#include "BCGPMiniFrameWnd.h"
#include "BCGPMultiMiniFrameWnd.h"
#include "BCGPBaseTabbedBar.h"

#include "BCGPFrameWnd.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPOleIPFrameWnd.h"
#include "BCGPOleDocIPFrameWnd.h"
#include "BCGPMDIChildWnd.h"
#include "BCGPOleCntrFrameWnd.h"
#include "BCGPURLLinkButton.h"
#include "BCGCBProVer.h"
#include "BCGPLocalResource.h"
#include "bcgprores.h"

#pragma warning (disable : 4706)

#include "multimon.h"

#pragma warning (default : 4706)


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CBCGPGlobalUtils globalUtils;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPGlobalUtils::CBCGPGlobalUtils()
{
	m_bDialogApp = FALSE;
	m_bIsDragging = FALSE;
}

CBCGPGlobalUtils::~CBCGPGlobalUtils()
{

}
//------------------------------------------------------------------------//
BOOL CBCGPGlobalUtils::CheckAlignment (CPoint point, CBCGPBaseControlBar* pBar, int nSencitivity, 
                                       const CBCGPDockManager* pDockManager,
									   BOOL bOuterEdge, DWORD& dwAlignment, 
									   DWORD dwEnabledDockBars, LPCRECT lpRectBounds) const
{
    BOOL bSmartDocking = FALSE;
    CBCGPSmartDockingMarker::SDMarkerPlace nHilitedSide = CBCGPSmartDockingMarker::sdNONE;
	
	if (pDockManager == NULL && pBar != NULL)
    {
        pDockManager = globalUtils.GetDockManager (pBar->GetParent());
    }
	
	if (pDockManager != NULL)
	{
        CBCGPSmartDockingManager* pSDManager = pDockManager->GetSDManagerPermanent ();
        if (pSDManager != NULL && pSDManager->IsStarted ())
        {
            bSmartDocking = TRUE;
            nHilitedSide = pSDManager->GetHilitedMarkerNo ();
        }
	}
	
	CRect rectBounds;
	if (pBar != NULL)
	{
		pBar->GetWindowRect (rectBounds);
		
	}
	else if (lpRectBounds != NULL)
	{
		rectBounds = *lpRectBounds;
	}
	else
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	int nCaptionHeight = 0;
	int nTabAreaTopHeight = 0; 
	int nTabAreaBottomHeight = 0;
	
	CBCGPDockingControlBar* pDockingBar = 
		DYNAMIC_DOWNCAST (CBCGPDockingControlBar, pBar);
	
	if (pDockingBar != NULL)
	{
		nCaptionHeight = pDockingBar->GetCaptionHeight ();
		
		CRect rectTabAreaTop;
		CRect rectTabAreaBottom;
		pDockingBar->GetTabArea (rectTabAreaTop, rectTabAreaBottom);
		nTabAreaTopHeight = rectTabAreaTop.Height ();
		nTabAreaBottomHeight = rectTabAreaBottom.Height ();
	}
	
	// build rect for top area
	if (bOuterEdge)
	{
        if (bSmartDocking)
        {
            switch (nHilitedSide)
            {
            case CBCGPSmartDockingMarker::sdLEFT:
				dwAlignment = CBRS_ALIGN_LEFT;
				return TRUE;
            case CBCGPSmartDockingMarker::sdRIGHT:
				dwAlignment = CBRS_ALIGN_RIGHT;
				return TRUE;
            case CBCGPSmartDockingMarker::sdTOP:
				dwAlignment = CBRS_ALIGN_TOP;
				return TRUE;
            case CBCGPSmartDockingMarker::sdBOTTOM:
				dwAlignment = CBRS_ALIGN_BOTTOM;
				return TRUE;
            }
        }
		else
        {
			CRect rectToCheck (rectBounds.left - nSencitivity, rectBounds.top - nSencitivity, 
				rectBounds.right + nSencitivity, rectBounds.top);
			if (rectToCheck.PtInRect (point) && dwEnabledDockBars & CBRS_ALIGN_TOP)
			{
				dwAlignment = CBRS_ALIGN_TOP;
				return TRUE;
			}
			
			// build rect for left area
			rectToCheck.right = rectBounds.left;
			rectToCheck.bottom = rectBounds.bottom + nSencitivity;
			
			if (rectToCheck.PtInRect (point) && dwEnabledDockBars & CBRS_ALIGN_LEFT)
			{
				dwAlignment = CBRS_ALIGN_LEFT;
				return TRUE;
			}
			
			// build rect for bottom area
			rectToCheck.left = rectBounds.left - nSencitivity;
			rectToCheck.top = rectBounds.bottom;
			rectToCheck.right = rectBounds.right + nSencitivity;
			rectToCheck.bottom = rectBounds.bottom + nSencitivity;
			
			if (rectToCheck.PtInRect (point) && dwEnabledDockBars & CBRS_ALIGN_BOTTOM)
			{
				dwAlignment = CBRS_ALIGN_BOTTOM;
				return TRUE;
			}
			
			// build rect for right area
			rectToCheck.left = rectBounds.right;
			rectToCheck.top = rectBounds.top - nSencitivity;
			
			if (rectToCheck.PtInRect (point) && dwEnabledDockBars & CBRS_ALIGN_RIGHT)
			{
				dwAlignment = CBRS_ALIGN_RIGHT;
				return TRUE;
			}
        }
	}
	else
	{
        if (bSmartDocking)
        {
            switch (nHilitedSide)
            {
            case CBCGPSmartDockingMarker::sdCLEFT:
				dwAlignment = CBRS_ALIGN_LEFT;
				return TRUE;
            case CBCGPSmartDockingMarker::sdCRIGHT:
				dwAlignment = CBRS_ALIGN_RIGHT;
				return TRUE;
            case CBCGPSmartDockingMarker::sdCTOP:
				dwAlignment = CBRS_ALIGN_TOP;
				return TRUE;
            case CBCGPSmartDockingMarker::sdCBOTTOM:
				dwAlignment = CBRS_ALIGN_BOTTOM;
				return TRUE;
            }
        }
		else
        {
#ifdef __BOUNDS_FIX__
			CRect rectToCheck (rectBounds.left, rectBounds.top, 
				rectBounds.right, 
				rectBounds.top + nSencitivity + nCaptionHeight);
			if (rectToCheck.PtInRect (point) && dwEnabledDockBars & CBRS_ALIGN_TOP)
			{
				dwAlignment = CBRS_ALIGN_TOP;
				return TRUE;
			}
			
			
			// build rect for left area
			rectToCheck.right = rectBounds.left + nSencitivity;
			rectToCheck.bottom = rectBounds.bottom + nSencitivity;
			
			if (rectToCheck.PtInRect (point) && dwEnabledDockBars & CBRS_ALIGN_LEFT)
			{
				dwAlignment = CBRS_ALIGN_LEFT;
				return TRUE;
			}
			
			// build rect for bottom area
			rectToCheck.left = rectBounds.left;
			rectToCheck.top = rectBounds.bottom - nSencitivity - nTabAreaBottomHeight;
			rectToCheck.right = rectBounds.right;
			rectToCheck.bottom = rectBounds.bottom;
			
			if (rectToCheck.PtInRect (point) && dwEnabledDockBars & CBRS_ALIGN_BOTTOM)
			{
				dwAlignment = CBRS_ALIGN_BOTTOM;
				return TRUE;
			}
			
			// build rect for right area
			rectToCheck.left = rectBounds.right - nSencitivity;
			rectToCheck.top = rectBounds.top - nSencitivity;
			
			if (rectToCheck.PtInRect (point) && dwEnabledDockBars & CBRS_ALIGN_RIGHT)
			{
				dwAlignment = CBRS_ALIGN_RIGHT;
				return TRUE;
			}
#else
			
			// build rect for top area
			CRect rectToCheck (rectBounds.left - nSencitivity, rectBounds.top - nSencitivity, 
				rectBounds.right + nSencitivity, 
				rectBounds.top + nSencitivity + nCaptionHeight);
			if (rectToCheck.PtInRect (point) && dwEnabledDockBars & CBRS_ALIGN_TOP)
			{
				dwAlignment = CBRS_ALIGN_TOP;
				return TRUE;
			}
			
			
			// build rect for left area
			rectToCheck.right = rectBounds.left + nSencitivity;
			rectToCheck.bottom = rectBounds.bottom + nSencitivity;
			
			if (rectToCheck.PtInRect (point) && dwEnabledDockBars & CBRS_ALIGN_LEFT)
			{
				dwAlignment = CBRS_ALIGN_LEFT;
				return TRUE;
			}
			
			// build rect for bottom area
			rectToCheck.left = rectBounds.left - nSencitivity;
			rectToCheck.top = rectBounds.bottom - nSencitivity - nTabAreaBottomHeight;
			rectToCheck.right = rectBounds.right + nSencitivity;
			rectToCheck.bottom = rectBounds.bottom + nSencitivity;
			
			if (rectToCheck.PtInRect (point) && dwEnabledDockBars & CBRS_ALIGN_BOTTOM)
			{
				dwAlignment = CBRS_ALIGN_BOTTOM;
				return TRUE;
			}
			
			// build rect for right area
			rectToCheck.left = rectBounds.right - nSencitivity;
			rectToCheck.top = rectBounds.top - nSencitivity;
			
			if (rectToCheck.PtInRect (point) && dwEnabledDockBars & CBRS_ALIGN_RIGHT)
			{
				dwAlignment = CBRS_ALIGN_RIGHT;
				return TRUE;
			}
#endif		
        }
	}
	
	return FALSE;
}
//------------------------------------------------------------------------//
CBCGPDockManager* CBCGPGlobalUtils::GetDockManager (CWnd* pWnd)
{
	if (pWnd == NULL)
	{
		return NULL;
	}

	ASSERT_VALID (pWnd);

	if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPFrameWnd)))
	{
		return ((CBCGPFrameWnd*) pWnd)->GetDockManager ();
	}
	else if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPMDIFrameWnd)))
	{
		return ((CBCGPMDIFrameWnd*) pWnd)->GetDockManager ();
	}
	else if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPOleIPFrameWnd)))
	{
		return ((CBCGPOleIPFrameWnd*) pWnd)->GetDockManager ();
	}
	else if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPOleDocIPFrameWnd)))
	{
		return ((CBCGPOleDocIPFrameWnd*) pWnd)->GetDockManager ();
	}
	else if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPMDIChildWnd)))
	{
		return ((CBCGPMDIChildWnd*) pWnd)->GetDockManager ();
	}
	else if (pWnd->IsKindOf (RUNTIME_CLASS (CDialog)) ||
		pWnd->IsKindOf (RUNTIME_CLASS (CPropertySheet)))
	{
		m_bDialogApp = TRUE;
	}
	else if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPOleCntrFrameWnd)))
	{
		return ((CBCGPOleCntrFrameWnd*) pWnd)->GetDockManager ();
	}
    else if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPMiniFrameWnd)))
    {
		CBCGPMiniFrameWnd* pMiniFrameWnd = DYNAMIC_DOWNCAST (CBCGPMiniFrameWnd, pWnd);
		ASSERT_VALID (pMiniFrameWnd);

		CBCGPDockManager* pManager = pMiniFrameWnd->GetDockManager ();
        return pManager != NULL ? pManager : GetDockManager (pWnd->GetParent ());
    }

	return NULL;
}
//------------------------------------------------------------------------//
void CBCGPGlobalUtils::FlipRect (CRect& rect, int nDegrees)
{
	CRect rectTmp = rect;
	switch (nDegrees)
	{
	case 90:
		rect.top = rectTmp.left;
		rect.right = rectTmp.top;
		rect.bottom = rectTmp.right;
		rect.left = rectTmp.bottom;
		break;
	case 180:
		rect.top = rectTmp.bottom;
		rect.bottom = rectTmp.top;
		break;
	case 275:
	case -90:
		rect.left = rectTmp.top;
		rect.top = rectTmp.right;
		rect.right = rectTmp.bottom;
		rect.bottom = rectTmp.left;
		break;
	}
}
//------------------------------------------------------------------------//
DWORD CBCGPGlobalUtils::GetOppositeAlignment (DWORD dwAlign)
{
	switch (dwAlign & CBRS_ALIGN_ANY)
	{
	case CBRS_ALIGN_LEFT:
		return CBRS_ALIGN_RIGHT;
	case CBRS_ALIGN_RIGHT:
		return CBRS_ALIGN_LEFT;
	case CBRS_ALIGN_TOP:
		return CBRS_ALIGN_BOTTOM;
	case CBRS_ALIGN_BOTTOM:
		return CBRS_ALIGN_TOP;
	}
	return 0;
}
//------------------------------------------------------------------------//
void CBCGPGlobalUtils::SetNewParent (CObList& lstControlBars, CWnd* pNewParent,
									 BOOL bCheckVisibility)
{
	ASSERT_VALID (pNewParent);
	for (POSITION pos = lstControlBars.GetHeadPosition (); pos != NULL;)
	{
		CBCGPBaseControlBar* pBar = (CBCGPBaseControlBar*) lstControlBars.GetNext (pos);

		if (bCheckVisibility && !pBar->IsBarVisible ())
		{
			continue;
		}
		if (!pBar->IsKindOf (RUNTIME_CLASS (CBCGPSlider)))
		{
			pBar->ShowWindow (SW_HIDE);
			pBar->SetParent (pNewParent);
			CRect rectWnd;
			pBar->GetWindowRect (rectWnd);
			pNewParent->ScreenToClient (rectWnd);

			pBar->SetWindowPos (NULL, -rectWnd.Width (), -rectWnd.Height (), 
									  100, 100, SWP_NOZORDER | SWP_NOSIZE  | SWP_NOACTIVATE);
			pBar->ShowWindow (SW_SHOW);
		}
		else
		{
			pBar->SetParent (pNewParent);
		}
	}
}
//------------------------------------------------------------------------//
void CBCGPGlobalUtils::CalcExpectedDockedRect (CBCGPBarContainerManager& barContainerManager, 
												CWnd* pWndToDock, CPoint ptMouse, 
												CRect& rectResult, BOOL& bDrawTab, 
												CBCGPDockingControlBar** ppTargetBar)
{
	ASSERT (ppTargetBar != NULL);

	DWORD dwAlignment = CBRS_ALIGN_LEFT;
	BOOL bTabArea = FALSE;
	BOOL bCaption = FALSE;
	bDrawTab = FALSE;
	*ppTargetBar = NULL;

	rectResult.SetRectEmpty ();

	if (GetKeyState (VK_CONTROL) < 0)
	{
		return;
	}

	if (!GetCBAndAlignFromPoint (barContainerManager, ptMouse, 
								 ppTargetBar, dwAlignment, bTabArea, bCaption) || 
		*ppTargetBar == NULL)
	{
		return;
	}

	CBCGPControlBar* pBar = NULL;
	
	if (pWndToDock->IsKindOf (RUNTIME_CLASS (CBCGPMiniFrameWnd)))
	{
		CBCGPMiniFrameWnd* pMiniFrame = 
			DYNAMIC_DOWNCAST (CBCGPMiniFrameWnd, pWndToDock);
		ASSERT_VALID (pMiniFrame);
		pBar = DYNAMIC_DOWNCAST (CBCGPControlBar, pMiniFrame->GetFirstVisibleBar ());
	}
	else
	{
		pBar = DYNAMIC_DOWNCAST (CBCGPControlBar, pWndToDock);
	}

	if (*ppTargetBar != NULL)
	{
		DWORD dwTargetEnabledAlign = (*ppTargetBar)->GetEnabledAlignment ();
		DWORD dwTargetCurrentAlign = (*ppTargetBar)->GetCurrentAlignment ();
		BOOL bTargetBarIsFloating = ((*ppTargetBar)->GetParentMiniFrame () != NULL);

		if (pBar != NULL)
		{
			if (pBar->GetEnabledAlignment () != dwTargetEnabledAlign && bTargetBarIsFloating ||
				(pBar->GetEnabledAlignment () & dwTargetCurrentAlign) == 0 && !bTargetBarIsFloating)
			{
				return;
			}
		}
	}

	if (bTabArea || bCaption)
	{
		// can't make tab on miniframe
		bDrawTab = ((*ppTargetBar) != NULL);

		if (bDrawTab)
		{
			bDrawTab = (*ppTargetBar)->CanBeAttached () && CanBeAttached (pWndToDock) && 
				pBar != NULL && ((*ppTargetBar)->GetEnabledAlignment () == pBar->GetEnabledAlignment ());
		}

		if (!bDrawTab)
		{
			return;
		}

	}

	if ((*ppTargetBar) != NULL && (*ppTargetBar)->GetParentMiniFrame () != NULL && 
		!IsWndCanFloatMulti (pWndToDock))
	{
		bDrawTab = FALSE;
		return;
	}

	if ((*ppTargetBar) != NULL && 
		pWndToDock->IsKindOf (RUNTIME_CLASS (CBCGPBaseControlBar)) && 
		!(*ppTargetBar)->CanAcceptBar ((CBCGPBaseControlBar*) pWndToDock))
	{
		bDrawTab = FALSE;
		return;
	}

	CRect rectOriginal; 
	(*ppTargetBar)->GetWindowRect (rectOriginal);

	if ((*ppTargetBar) == pWndToDock ||
		pWndToDock->IsKindOf (RUNTIME_CLASS (CBCGPMiniFrameWnd)) && 
		(*ppTargetBar)->GetParentMiniFrame () == pWndToDock)
	{
		bDrawTab = FALSE;
		return;
	}
	
	CRect rectInserted;
	CRect rectSlider;
	DWORD dwSliderStyle;
	CSize sizeMinOriginal (0, 0);
	CSize sizeMinInserted (0, 0);

	
	pWndToDock->GetWindowRect (rectInserted);

	if ((dwAlignment & pBar->GetEnabledAlignment ()) != 0 ||
		CBCGPDockManager::m_bIgnoreEnabledAlignment)
	{
		barContainerManager.CalcRects (rectOriginal, rectInserted, rectSlider, dwSliderStyle, 
										 dwAlignment, sizeMinOriginal, sizeMinInserted);
		rectResult = rectInserted;
	}
	
}
//------------------------------------------------------------------------//
BOOL CBCGPGlobalUtils::CanBeAttached (CWnd* pWnd) const
{
	ASSERT_VALID (pWnd);

	if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPMiniFrameWnd)))
	{
		return ((CBCGPMiniFrameWnd*) pWnd)->CanBeAttached ();
	}

	if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPControlBar)))
	{
		return ((CBCGPControlBar*) pWnd)->CanBeAttached ();
	}

	return FALSE;

}
//------------------------------------------------------------------------//
BOOL CBCGPGlobalUtils::IsWndCanFloatMulti (CWnd* pWnd) const
{
	CBCGPControlBar* pBar = NULL;

	CBCGPMiniFrameWnd* pMiniFrame = 
		DYNAMIC_DOWNCAST (CBCGPMiniFrameWnd, pWnd);
	
	if (pMiniFrame != NULL)
	{
		pBar = DYNAMIC_DOWNCAST (CBCGPControlBar, pMiniFrame->GetControlBar ());
	}
	else
	{
		pBar = DYNAMIC_DOWNCAST (CBCGPControlBar, pWnd);

	}

	if (pBar == NULL)
	{
		return FALSE;
	}

	if (pBar->IsTabbed ())
	{
		CWnd* pParentMiniFrame = pBar->GetParentMiniFrame ();
		// tabbed bar that is floating in multi miniframe
		if (pParentMiniFrame != NULL && pParentMiniFrame->IsKindOf (RUNTIME_CLASS (CBCGPMultiMiniFrameWnd)))
		{
			return TRUE;
		}
	}

	
	return ((pBar->GetBarStyle () & CBRS_FLOAT_MULTI) != 0);
}
//------------------------------------------------------------------------//
BOOL CBCGPGlobalUtils::GetCBAndAlignFromPoint (CBCGPBarContainerManager& barContainerManager, 
													 CPoint pt, 
												     CBCGPDockingControlBar** ppTargetControlBar, 
												     DWORD& dwAlignment, 
													 BOOL& bTabArea, 
													 BOOL& bCaption)
{
	ASSERT (ppTargetControlBar != NULL);
	*ppTargetControlBar = NULL;

	BOOL bOuterEdge = FALSE;

	// if the mouse is over a miniframe's caption and this miniframe has only one
	// visible docking control bar, we need to draw a tab
	bCaption = barContainerManager.CheckForMiniFrameAndCaption (pt, ppTargetControlBar);
	if (bCaption)
	{
		return TRUE;
	}


	*ppTargetControlBar = 
		barContainerManager.ControlBarFromPoint (pt, CBCGPDockManager::m_nDockSencitivity, 
													TRUE, bTabArea, bCaption);

	if ((bCaption || bTabArea) && *ppTargetControlBar != NULL) 
	{
		return TRUE;
	}

	if (*ppTargetControlBar == NULL)
	{
		barContainerManager.ControlBarFromPoint (pt, CBCGPDockManager::m_nDockSencitivity, 
														FALSE, bTabArea, bCaption);
		// the exact bar was not found - it means the docked frame at the outer edge
		bOuterEdge = TRUE;
		return TRUE;
	}

	if (*ppTargetControlBar != NULL)
	{
		if (!globalUtils.CheckAlignment (pt, *ppTargetControlBar,
										CBCGPDockManager::m_nDockSencitivity, NULL,
										bOuterEdge, dwAlignment))
		{
			// unable for some reason to determine alignment
			*ppTargetControlBar = NULL;
		}
	}

	return TRUE;
}
//------------------------------------------------------------------------//
void CBCGPGlobalUtils::AdjustRectToWorkArea (CRect& rect, CRect* pRectDelta)
{
	CPoint ptStart;

	if (m_bIsDragging)
	{
		::GetCursorPos (&ptStart);
	}
	else
	{
		ptStart = rect.TopLeft ();
	}

	CRect rectScreen;
	MONITORINFO mi;
	mi.cbSize = sizeof (MONITORINFO);
	if (GetMonitorInfo (
		MonitorFromPoint (ptStart, MONITOR_DEFAULTTONEAREST),
		&mi))
	{
		rectScreen = mi.rcWork;
	}
	else
	{
		::SystemParametersInfo (SPI_GETWORKAREA, 0, &rectScreen, 0);
	}

	int nDelta = pRectDelta != NULL ? pRectDelta->left : 0;

	if (rect.right <= rectScreen.left + nDelta)
	{
		rect.OffsetRect (rectScreen.left - rect.right + nDelta, 0);
	}

	nDelta = pRectDelta != NULL ? pRectDelta->right : 0;
	if (rect.left >= rectScreen.right - nDelta)
	{
		rect.OffsetRect (rectScreen.right - rect.left - nDelta, 0);
	}

	nDelta = pRectDelta != NULL ? pRectDelta->bottom : 0;
	if (rect.top >= rectScreen.bottom - nDelta)
	{
		rect.OffsetRect (0, rectScreen.bottom - rect.top - nDelta);
	}

	nDelta = pRectDelta != NULL ? pRectDelta->top : 0;
	if (rect.bottom < rectScreen.top + nDelta)
	{
		rect.OffsetRect (0, rectScreen.top - rect.bottom + nDelta);
	}
}
//------------------------------------------------------------------------//
void CBCGPGlobalUtils::ForceAdjustLayout (CBCGPDockManager* pDockManager, BOOL bForce)
{
	if (pDockManager != NULL && 
		(CBCGPControlBar::m_bHandleMinSize || bForce))
	{
		CWnd* pDockSite = pDockManager->GetDockSite ();

		if (pDockSite == NULL || !pDockSite->IsWindowVisible ())
		{
			return;
		}

		CRect rectWnd;
		pDockManager->GetDockSite ()->SetRedraw (FALSE);
		pDockManager->GetDockSite ()->GetWindowRect (rectWnd);
		pDockManager->GetDockSite ()->SetWindowPos (NULL, -1, -1, 
			rectWnd.Width () + 1, rectWnd.Height () + 1, 
			SWP_NOZORDER |  SWP_NOMOVE | SWP_NOACTIVATE);
		pDockManager->GetDockSite ()->SetWindowPos (NULL, -1, -1, 
			rectWnd.Width (), rectWnd.Height (), 
			SWP_NOZORDER |  SWP_NOMOVE  | SWP_NOACTIVATE);
		pDockManager->GetDockSite ()->SetRedraw (TRUE);
		pDockManager->GetDockSite ()->RedrawWindow (NULL, NULL, 
			RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
	}
}
//------------------------------------------------------------------------//
BOOL CBCGPGlobalUtils::StringFromCy(CString& str, CY& cy)
{
	VARIANTARG varCy;
	VARIANTARG varBstr;
	AfxVariantInit(&varCy);
	AfxVariantInit(&varBstr);
	V_VT(&varCy) = VT_CY;
	V_CY(&varCy) = cy;
	if (FAILED(VariantChangeType(&varBstr, &varCy, 0, VT_BSTR)))
	{
		VariantClear(&varCy);
		VariantClear(&varBstr);
		return FALSE;
	}
	str = V_BSTR(&varBstr);
	VariantClear(&varCy);
	VariantClear(&varBstr);
	return TRUE;
}
//------------------------------------------------------------------------//
BOOL CBCGPGlobalUtils::CyFromString(CY& cy, LPCTSTR psz)
{
	USES_CONVERSION;

	if (psz == NULL || _tcslen (psz) == 0)
	{
		psz = _T("0");
	}

	VARIANTARG varBstr;
	VARIANTARG varCy;
	AfxVariantInit(&varBstr);
	AfxVariantInit(&varCy);
	V_VT(&varBstr) = VT_BSTR;
	V_BSTR(&varBstr) = SysAllocString(T2COLE(psz));
	if (FAILED(VariantChangeType(&varCy, &varBstr, 0, VT_CY)))
	{
		VariantClear(&varBstr);
		VariantClear(&varCy);
		return FALSE;
	}
	cy = V_CY(&varCy);
	VariantClear(&varBstr);
	VariantClear(&varCy);
	return TRUE;
}
//------------------------------------------------------------------------//
BOOL CBCGPGlobalUtils::StringFromDecimal(CString& str, DECIMAL& decimal)
{
	VARIANTARG varDecimal;
	VARIANTARG varBstr;
	AfxVariantInit(&varDecimal);
	AfxVariantInit(&varBstr);
	V_VT(&varDecimal) = VT_DECIMAL;
	V_DECIMAL(&varDecimal) = decimal;
	if (FAILED(VariantChangeType(&varBstr, &varDecimal, 0, VT_BSTR)))
	{
		VariantClear(&varDecimal);
		VariantClear(&varBstr);
		return FALSE;
	}
	str = V_BSTR(&varBstr);
	VariantClear(&varDecimal);
	VariantClear(&varBstr);
	return TRUE;
}
//------------------------------------------------------------------------//
BOOL CBCGPGlobalUtils::DecimalFromString(DECIMAL& decimal, LPCTSTR psz)
{
	USES_CONVERSION;

	if (psz == NULL || _tcslen (psz) == 0)
	{
		psz = _T("0");
	}

	VARIANTARG varBstr;
	VARIANTARG varDecimal;
	AfxVariantInit(&varBstr);
	AfxVariantInit(&varDecimal);
	V_VT(&varBstr) = VT_BSTR;
	V_BSTR(&varBstr) = SysAllocString(T2COLE(psz));
	if (FAILED(VariantChangeType(&varDecimal, &varBstr, 0, VT_DECIMAL)))
	{
		VariantClear(&varBstr);
		VariantClear(&varDecimal);
		return FALSE;
	}
	decimal = V_DECIMAL(&varDecimal);
	VariantClear(&varBstr);
	VariantClear(&varDecimal);
	return TRUE;
}
//------------------------------------------------------------------------//
HICON CBCGPGlobalUtils::GetWndIcon (CWnd* pWnd)
{
	ASSERT_VALID (pWnd);
	
	if (pWnd->GetSafeHwnd () == NULL)
	{
		return NULL;
	}

	HICON hIcon = pWnd->GetIcon (FALSE);

	if (hIcon == NULL)
	{
		hIcon = pWnd->GetIcon (TRUE);

		if (hIcon != NULL)
		{
			CImageList il;
			il.Create (16, 16, ILC_COLOR32 | ILC_MASK, 0, 1);
			il.Add (hIcon);

			if (il.GetImageCount () == 1)
			{
				hIcon = il.ExtractIcon (0);
			}
		}
	}

	if (hIcon == NULL)
	{
		hIcon = (HICON)(LONG_PTR)::GetClassLongPtr(pWnd->GetSafeHwnd (), GCLP_HICONSM);
	}

	if (hIcon == NULL)
	{
		hIcon = (HICON)(LONG_PTR)::GetClassLongPtr(pWnd->GetSafeHwnd (), GCLP_HICON);
	}

	return hIcon;
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPAboutDlg dialog

class CBCGPAboutDlg : public CDialog
{
// Construction
public:
	CBCGPAboutDlg(LPCTSTR lpszAppName, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBCGPAboutDlg)
	enum { IDD = IDD_BCGBARRES_ABOUT_DLG };
	CBCGPURLLinkButton	m_wndURL;
	CButton	m_wndPurchaseBtn;
	CStatic	m_wndAppName;
	CBCGPURLLinkButton	m_wndEMail;
	CString	m_strAppName;
	CString	m_strVersion;
	CString	m_strYear;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBCGPAboutDlg)
	afx_msg void OnBcgbarresPurchase();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CFont	m_fontBold;
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPAboutDlg dialog

CBCGPAboutDlg::CBCGPAboutDlg(LPCTSTR lpszAppName, CWnd* pParent /*=NULL*/)
	: CDialog(CBCGPAboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBCGPAboutDlg)
	m_strAppName = _T("");
	m_strVersion = _T("");
	m_strYear = _T("");
	//}}AFX_DATA_INIT

	m_strVersion.Format (_T("%d.%d"), _BCGCBPRO_VERSION_MAJOR, _BCGCBPRO_VERSION_MINOR);

	CString strCurrDate = __DATE__;
	m_strYear.Format (_T("1998-%s"), strCurrDate.Right (4));

	m_strAppName = lpszAppName;
}

void CBCGPAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBCGPAboutDlg)
	DDX_Control(pDX, IDC_BCGBARRES_URL, m_wndURL);
	DDX_Control(pDX, IDC_BCGBARRES_PURCHASE, m_wndPurchaseBtn);
	DDX_Control(pDX, IDC_BCGBARRES_NAME, m_wndAppName);
	DDX_Control(pDX, IDC_BCGBARRES_MAIL, m_wndEMail);
	DDX_Text(pDX, IDC_BCGBARRES_NAME, m_strAppName);
	DDX_Text(pDX, IDC_BCGBARRES_VERSION, m_strVersion);
	DDX_Text(pDX, IDC_BCGBARRES_YEAR, m_strYear);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBCGPAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CBCGPAboutDlg)
	ON_BN_CLICKED(IDC_BCGBARRES_PURCHASE, OnBcgbarresPurchase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPAboutDlg message handlers

void CBCGPAboutDlg::OnBcgbarresPurchase() 
{
	CString strURL = _T("http://www.bcgsoft.com/register-bcgcbpe.htm");
	::ShellExecute (NULL, _T("open"), strURL, NULL, NULL, SW_SHOWNORMAL);

	EndDialog (IDOK);
}

BOOL CBCGPAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//------------------
	// Create bold font:
	//------------------
	CFont* pFont = m_wndAppName.GetFont ();
	ASSERT_VALID (pFont);

	LOGFONT lf;
	memset (&lf, 0, sizeof (LOGFONT));

	pFont->GetLogFont (&lf);

	lf.lfWeight = FW_BOLD;
	m_fontBold.CreateFontIndirect (&lf);

	m_wndAppName.SetFont (&m_fontBold);

	//-----------------------------
	// Setup URL and e-mail fields:
	//-----------------------------
	m_wndEMail.SetURLPrefix (_T("mailto:"));
	m_wndEMail.SetURL (_T("info@bcgsoft.com"));
	m_wndEMail.SizeToContent ();
	m_wndEMail.SetTooltip (_T("Send mail to us"));
	m_wndEMail.m_bDrawFocus = FALSE;

	m_wndURL.m_bDrawFocus = FALSE;
	m_wndURL.SizeToContent ();

	//--------------------
	// Set dialog caption:
	//--------------------
	CString strCaption;
	strCaption.Format (_T("About %s"), m_strAppName);

	SetWindowText (strCaption);

	//------------------------------------------
	// Hide "Purchase" button in retail version:
	//------------------------------------------
#ifndef _BCGCBPRO_EVAL_
	m_wndPurchaseBtn.EnableWindow (FALSE);
	m_wndPurchaseBtn.ShowWindow (SW_HIDE);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void BCGPShowAboutDlg (LPCTSTR lpszAppName)
{
	CBCGPLocalResource locaRes;

	CBCGPAboutDlg dlg (lpszAppName);
	dlg.DoModal ();
}

void BCGPShowAboutDlg (UINT uiAppNameResID)
{
	CString strAppName;
	strAppName.LoadString (uiAppNameResID);

	BCGPShowAboutDlg (strAppName);
}

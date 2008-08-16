// BCGPGlobalUtils.h: interface for the CBCGPGlobalUtils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPGLOBALUTILS_H__FD3579BF_C46C_439C_AD13_5DFDA777F240__INCLUDED_)
#define AFX_BCGPGLOBALUTILS_H__FD3579BF_C46C_439C_AD13_5DFDA777F240__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGCBPro.h"

class CBCGPDockManager;
class CBCGPBaseControlBar;
class CBCGPDockingControlBar;
class CBCGPBarContainerManager;

class BCGCBPRODLLEXPORT CBCGPGlobalUtils  
{
public:
	CBCGPGlobalUtils();
	virtual ~CBCGPGlobalUtils();

	BOOL CheckAlignment (CPoint point, CBCGPBaseControlBar* pBar,
						 int nSencitivity,
                         const CBCGPDockManager* pDockManager,
                         BOOL bOuterEdge,
						 DWORD& dwAlignment, 
						 DWORD dwEnabledDockBars = CBRS_ALIGN_ANY, 
						 LPCRECT lpRectBounds = NULL) const;

	CBCGPDockManager* GetDockManager (CWnd* pWnd);
	
	void FlipRect (CRect& rect, int nDegrees);
	DWORD GetOppositeAlignment (DWORD dwAlign);
	void SetNewParent (CObList& lstControlBars, CWnd* pNewParent, 
						BOOL bCheckVisibility = TRUE);

	void CalcExpectedDockedRect (CBCGPBarContainerManager& barContainerManager, 
			CWnd* pWndToDock, CPoint ptMouse, CRect& rectResult, BOOL& bDrawTab, 
			CBCGPDockingControlBar** ppTargetBar);
	BOOL GetCBAndAlignFromPoint (CBCGPBarContainerManager& barContainerManager, 
								 CPoint pt, CBCGPDockingControlBar** ppTargetControlBar, 
							     DWORD& dwAlignment, BOOL& bTabArea, BOOL& bCaption);
	void ForceAdjustLayout (CBCGPDockManager* pDockManager, BOOL bForce = FALSE);
	BOOL IsWndCanFloatMulti (CWnd* pWnd) const;
	void AdjustRectToWorkArea (CRect& rect, CRect* pRectDelta = NULL);
	BOOL CanBeAttached (CWnd* pWnd) const;

	BOOL StringFromCy(CString& str, CY& cy);
	BOOL CyFromString(CY& cy, LPCTSTR psz);
	BOOL StringFromDecimal(CString& str, DECIMAL& decimal);
	BOOL DecimalFromString(DECIMAL& decimal, LPCTSTR psz);

	HICON GetWndIcon (CWnd* pWnd);

	BOOL	m_bDialogApp;
	BOOL	m_bIsDragging;
};

extern BCGCBPRODLLEXPORT CBCGPGlobalUtils globalUtils;

void BCGCBPRODLLEXPORT BCGPShowAboutDlg (LPCTSTR lpszAppName);
void BCGCBPRODLLEXPORT BCGPShowAboutDlg (UINT uiAppNameResID);

#endif // !defined(AFX_BCGPGLOBALUTILS_H__FD3579BF_C46C_439C_AD13_5DFDA777F240__INCLUDED_)

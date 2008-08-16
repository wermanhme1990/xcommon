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

#ifndef __GLOBAL_DATA_H
#define __GLOBAL_DATA_H

#include "BCGCBPro.h"

#if (!defined _BCGPCALENDAR_STANDALONE) && !(defined _BCGPGRID_STANDALONE) && !(defined _BCGPEDIT_STANDALONE)
#include "bcgpaccessibility.h"
#include <oleacc.h>

/////////////////////////////////////////////////////////////////////////////
// Auxiliary System/Screen metrics

typedef enum BCGP_DOCK_TYPE
{
	DT_UNDEFINED = 0,		// inherit from application
	DT_IMMEDIATE = 1,		// control bar torn off immediately and follows the mouse
	DT_STANDARD  = 2,		// user drags a frame
    DT_SMART = 0x80			// smart docking style
};

// autohide sliding modes
static const UINT BCGP_AHSM_MOVE		= 1;
static const UINT BCGP_AHSM_STRETCH		= 2;

#define BCGP_AUTOHIDE_LEFT		0x0001
#define BCGP_AUTOHIDE_RIGHT		0x0002
#define BCGP_AUTOHIDE_TOP		0x0004
#define BCGP_AUTOHIDE_BOTTOM	0x0008

typedef LRESULT (*BCGPGETACCOBJECTLRESULT)(BCGPACCDATA* pAccData, IAccessible* pAccess);
typedef BOOL (__stdcall * NOTIFYWINEVENT)(DWORD event, HWND hwnd, LONG idObject, LONG idChild);
#endif

#ifndef _UXTHEME_H_

// From uxtheme.h:
typedef HANDLE HTHEME;          // handle to a section of theme data for class
#endif // THEMEAPI

typedef HANDLE BCGPHPAINTBUFFER;  // handle to a buffered paint context

typedef BOOL (__stdcall * SETLAYEATTRIB)(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
typedef HRESULT (__stdcall * DRAWTHEMEPARENTBACKGROUND)(HWND hWnd, HDC hdc,const RECT *pRec);


// BCGP_BP_BUFFERFORMAT
typedef enum _BCGP_BP_BUFFERFORMAT
{
    BCGP_BPBF_COMPATIBLEBITMAP,    // Compatible bitmap
    BCGP_BPBF_DIB,                 // Device-independent bitmap
    BCGP_BPBF_TOPDOWNDIB,          // Top-down device-independent bitmap
    BCGP_BPBF_TOPDOWNMONODIB       // Top-down monochrome device-independent bitmap
} BCGP_BP_BUFFERFORMAT;

#define BCGP_BP_BUFFERFORMATBPBF_COMPOSITED BCGP_BP_BUFFERFORMATBPBF_DIB

// BCGP_BP_PAINTPARAMS
typedef struct _BCGP_BP_PAINTPARAMS
{
    DWORD                       cbSize;
    DWORD                       dwFlags; // BPPF_ flags
    const RECT *                prcExclude;
    const BLENDFUNCTION *       pBlendFunction;
} BCGP_BP_PAINTPARAMS;

typedef BCGPHPAINTBUFFER (__stdcall * BEGINBUFFEREDPAINT)(	HDC hdcTarget, const RECT* rcTarget, 
															BCGP_BP_BUFFERFORMAT dwFormat, 
															BCGP_BP_PAINTPARAMS *pPaintParams,
															HDC *phdc);

typedef HRESULT (__stdcall * ENDBUFFEREDPAINT)(BCGPHPAINTBUFFER hBufferedPaint, BOOL fUpdateTarget);

typedef struct _BCGPMARGINS {
    int cxLeftWidth;
    int cxRightWidth;
    int cyTopHeight;
    int cyBottomHeight;
} BCGPMARGINS;

typedef HRESULT (__stdcall * DWMEXTENDFRAMEINTOCLIENTAREA)(HWND hWnd, const BCGPMARGINS* pMargins);
typedef HRESULT (__stdcall * DWMDEFWINDOWPROC)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *plResult);
typedef HRESULT (__stdcall * DWMISCOMPOSITIONENABLED)(BOOL* pfEnabled);

typedef int (WINAPI *BCGPDTT_CALLBACK_PROC)
(
    HDC hdc,
    LPWSTR pszText,
    int cchText,
    LPRECT prc,
    UINT dwFlags,
    LPARAM lParam);

typedef struct _BCGPDTTOPTS {
    DWORD dwSize;
    DWORD dwFlags;
    COLORREF crText;
    COLORREF crBorder;
    COLORREF crShadow;
    int iTextShadowType;
    POINT ptShadowOffset;
    int iBorderSize;
    int iFontPropId;
    int iColorPropId;
    int iStateId;
    BOOL fApplyOverlay;
    int iGlowSize;
    BCGPDTT_CALLBACK_PROC pfnDrawTextCallback;
    LPARAM lParam;
} BCGPDTTOPTS;

typedef HRESULT (__stdcall * DRAWTHEMETEXTEX)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwFlags, LPRECT pRect, const BCGPDTTOPTS *pOptions);

struct BCGCBPRODLLEXPORT GLOBAL_DATA
{
	friend class CBCGPMemDC;

	// solid brushes with convenient gray colors and system colors
	HBRUSH hbrBtnHilite, hbrBtnShadow;

	HBRUSH hbrWindow;

	// color values of system colors used for CToolBar
	COLORREF clrBtnFace, clrBtnShadow, clrBtnHilite;
	COLORREF clrBtnText, clrWindowFrame;
	COLORREF clrBtnDkShadow, clrBtnLight;
	COLORREF clrGrayedText;
	COLORREF clrHilite;
	COLORREF clrTextHilite;
	COLORREF clrHotText;
	COLORREF clrHotLinkText;

	COLORREF clrBarWindow;
	COLORREF clrBarFace;
	COLORREF clrBarShadow, clrBarHilite;
	COLORREF clrBarDkShadow, clrBarLight;
	COLORREF clrBarText;

	COLORREF clrWindow;
	COLORREF clrWindowText;

	COLORREF clrCaptionText;

	COLORREF clrMenuText;

	COLORREF clrActiveCaption;
	COLORREF clrInactiveCaption;

	COLORREF clrInactiveCaptionText;

	COLORREF clrActiveBorder;
	COLORREF clrInactiveBorder;

	CBrush	brBtnFace;
	CBrush	brHilite;
	CBrush	brLight;
	CBrush	brBlack;
	CBrush	brActiveCaption;
	CBrush	brInactiveCaption;
	CBrush	brWindow;

	CBrush brBarFace;

	CPen	penHilite;
	CPen	penBarFace;
	CPen	penBarShadow;

	HCURSOR	m_hcurStretch;
	HCURSOR	m_hcurStretchVert;
	HCURSOR	m_hcurHand;
	HCURSOR	m_hcurSizeAll;
	HCURSOR	m_hcurMoveTab;
	HCURSOR	m_hcurNoMoveTab;
	HCURSOR	m_hcurSelectRow;

	HICON	m_hiconTool;

	// Shell icon sizes:
	CSize	m_sizeSmallIcon;

	// Toolbar and menu fonts:
	CFont				fontRegular;
	CFont				fontTooltip;
	CFont				fontBold;
	CFont				fontDefaultGUIBold;
	CFont				fontUnderline;
	CFont				fontDefaultGUIUnderline;
	CFont				fontVert;
	CFont				fontVertCaption;
	CFont				fontSmall;
						
	CFont				fontMarlett;	// Standard Windows menu symbols
						
	BOOL				bIsWindowsNT4;
	BOOL				bIsWindows9x;
	BOOL				bIsWindowsVista;
	BOOL				bDisableAero;
	int					m_nBitsPerPixel;

	BOOL				bIsRemoteSession;
	BOOL				bIsOSAlphaBlendingSupport;

	int					m_nDragFrameThiknessFloat;
	int					m_nDragFrameThiknessDock;
						
	int					m_nAutoHideToolBarSpacing;
	int					m_nAutoHideToolBarMargin;
						
	int					m_nCoveredMainWndClientAreaPercent;

	int					m_nMaxToolTipWidth;

	BOOL				m_bIsBlackHighContrast;
	BOOL				m_bIsWhiteHighContrast;

	BOOL				m_bUseBuiltIn32BitIcons;

	CRect				m_rectVirtual;

	BOOL				m_bMenuAnimation;
	BOOL				m_bMenuFadeEffect;

	int					m_nShellAutohideBars;
						
// Implementation
	GLOBAL_DATA();
	~GLOBAL_DATA();

	void UpdateSysColors();
	void UpdateFonts();
	void OnSettingChange ();

	BOOL SetMenuFont (LPLOGFONT lpLogFont, BOOL bHorz);

	int GetTextHeight (BOOL bHorz = TRUE)
	{
		return bHorz ? m_nTextHeightHorz : m_nTextHeightVert;
	}

	int GetTextWidth (BOOL bHorz = TRUE)
	{
		return bHorz ? m_nTextWidthHorz : m_nTextWidthVert;
	}

	BOOL IsWinXPDrawParentBackground () const
	{
		return m_pfDrawThemeBackground != NULL;
	}

	BOOL DrawParentBackground (CWnd* pWnd, CDC* pDC, LPRECT lpRect = NULL);
	void CleanUp ();

	COLORREF GetColor (int nColor);

	BOOL SetLayeredAttrib (HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
	BOOL IsWindowsLayerSupportAvailable () const
	{
		return m_pfSetLayeredWindowAttributes != NULL; 
	}

	BOOL Is32BitIcons () const
	{
		return m_bUseBuiltIn32BitIcons && m_nBitsPerPixel >= 16 && !m_bIsBlackHighContrast && !m_bIsWhiteHighContrast;
	}

	BOOL IsHighContastMode () const
	{
		return m_bIsWhiteHighContrast || m_bIsBlackHighContrast;
	}

#if (!defined _BCGPCALENDAR_STANDALONE) && !(defined _BCGPGRID_STANDALONE) && !(defined _BCGPEDIT_STANDALONE)
	BOOL IsAccessibilitySupport () const
	{
		return m_pGetAccObjectRes != NULL;
	}

	void EnableAccessibilitySupport (BOOL bEnable = TRUE);
	LRESULT GetAccObjectRes (BCGPACCDATA* pAccData, IAccessible* pAccess);
	BOOL NotifyWinEvent (DWORD event, HWND hwnd, LONG idObject, LONG idChild);
#endif

	CString RegisterWindowClass (LPCTSTR lpszClassNamePrefix);
	BOOL ExcludeTag (CString& strBuffer, LPCTSTR lpszTag, CString& strTag, BOOL bIsCharsList = FALSE);

	BOOL DwmExtendFrameIntoClientArea (HWND hWnd, BCGPMARGINS* pMargins);
	LRESULT DwmDefWindowProc (HWND hWnd, UINT message, WPARAM wp, LPARAM lp);
	BOOL DwmIsCompositionEnabled ();

	BOOL DrawTextOnGlass (	HTHEME hTheme, CDC* pDC, int iPartId, int iStateId, 
							CString strText, CRect rect, DWORD dwFlags,
							int nGlowSize = 0, COLORREF clrText = (COLORREF)-1);

protected:

	void UpdateTextMetrics ();
	HBITMAP CreateDitherBitmap (HDC hDC);
	
	int	m_nTextHeightHorz;
	int	m_nTextHeightVert;

	int	m_nTextWidthHorz;
	int	m_nTextWidthVert;

	HINSTANCE m_hinstUXThemeDLL;
	HINSTANCE m_hinstUser32;
	HINSTANCE m_hinstDwmapiDLL;

	SETLAYEATTRIB m_pfSetLayeredWindowAttributes;

#if (!defined _BCGPCALENDAR_STANDALONE) && !(defined _BCGPGRID_STANDALONE) && !(defined _BCGPEDIT_STANDALONE)
	HINSTANCE				m_hinstAccDLL;
	BCGPGETACCOBJECTLRESULT	m_pGetAccObjectRes;
	NOTIFYWINEVENT			m_pfNotifyWinEvent;
#endif

	DRAWTHEMEPARENTBACKGROUND		m_pfDrawThemeBackground;
	DRAWTHEMETEXTEX					m_pfDrawThemeTextEx;
	BEGINBUFFEREDPAINT				m_pfBeginBufferedPaint;
	ENDBUFFEREDPAINT				m_pfEndBufferedPaint;
	DWMEXTENDFRAMEINTOCLIENTAREA	m_pfDwmExtendFrameIntoClientArea;
	DWMDEFWINDOWPROC				m_pfDwmDefWindowProc;
	DWMISCOMPOSITIONENABLED			m_pfDwmIsCompositionEnabled;
};

extern BCGCBPRODLLEXPORT GLOBAL_DATA globalData;

#define IMAGE_MARGIN	4

// MFC Control bar compatibility 
#define CX_BORDER   1
#define CY_BORDER   1

#define CX_GRIPPER  3
#define CY_GRIPPER  3
#define CX_BORDER_GRIPPER 2
#define CY_BORDER_GRIPPER 2


/////////////////////////////////////////////////////////////////////////////

#endif // __GLOBAL_DATA_H

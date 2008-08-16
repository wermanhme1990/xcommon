#ifndef __BCGCBPRO_H
#define __BCGCBPRO_H

// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2000 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.

#ifndef __AFXCMN_H__
	#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif

#if defined _AFXDLL && !defined _BCGCBPRO_STATIC_
	#ifdef _BCGCBPRO_
	   #define BCGCBPRODLLEXPORT  _declspec(dllexport)
	#else
	   #define BCGCBPRODLLEXPORT  _declspec(dllimport)
	#endif
#else
	#define BCGCBPRODLLEXPORT
#endif

#include "BCGPUserToolsManager.h"

inline BOOL IsStandardCommand (UINT uiCmd)
{
	return	((uiCmd >= ID_FILE_MRU_FILE1 && 
				uiCmd <= ID_FILE_MRU_FILE16)		||	// MRU commands,
			(uiCmd >= 0xF000 && uiCmd < 0xF1F0)		||	// system commands,
			((int) uiCmd >= AFX_IDM_FIRST_MDICHILD)	||	// windows commands
			(uiCmd >= ID_OLE_VERB_FIRST && uiCmd <= ID_OLE_VERB_LAST) ||		// OLE commands
			g_pUserToolsManager != NULL && uiCmd == g_pUserToolsManager->GetToolsEntryCmd ());
}


BCGCBPRODLLEXPORT void BCGCBProSetResourceHandle (
	HINSTANCE hinstResDLL);

BCGCBPRODLLEXPORT HINSTANCE BCGCBProGetResourceHandle ();

BCGCBPRODLLEXPORT HINSTANCE BCGCBProGetInstanceHandle ();

BCGCBPRODLLEXPORT void BCGCBProCleanUp ();

extern BCGCBPRODLLEXPORT CFrameWnd* g_pTopLevelFrame;

inline BCGCBPRODLLEXPORT void BCGCBProSetTopLevelFrame (CFrameWnd* pFrame)
{
	g_pTopLevelFrame = pFrame;
}

inline BCGCBPRODLLEXPORT CFrameWnd* BCGCBProGetTopLevelFrame (const CWnd* pWnd)
{
	ASSERT_VALID (pWnd);
	return g_pTopLevelFrame == NULL ? pWnd->GetTopLevelFrame () : g_pTopLevelFrame;
}

BCGCBPRODLLEXPORT CFrameWnd* BCGPGetParentFrame (const CWnd* pWnd);

#define BCG_GET_X_LPARAM(lp)		((int)(short)LOWORD(lp))
#define BCG_GET_Y_LPARAM(lp)		((int)(short)HIWORD(lp))

class BCGCBPRODLLEXPORT CBCGPMemDC
{
public:
	static BOOL	m_bUseMemoryDC;

	CBCGPMemDC(CDC& dc, CWnd* pWnd);
	CBCGPMemDC(CDC& dc, const CRect& rect);

	virtual ~CBCGPMemDC();

	CDC& GetDC ()			{	return m_bMemDC ? m_dcMem : m_dc;	}
	BOOL IsMemDC () const	{	return m_bMemDC;					}
	BOOL IsVistaDC () const	{	return m_hBufferedPaint != NULL;	}

protected:
	CDC&		m_dc;
	BOOL		m_bMemDC;
	HANDLE		m_hBufferedPaint;
	CDC			m_dcMem;
	CBitmap		m_bmp;
	CBitmap*	m_pOldBmp;
	CRect		m_rect;
};

#ifndef WS_EX_LAYOUTRTL
#define WS_EX_LAYOUTRTL         0x00400000L // Right to left mirroring
#endif

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED           0x00080000
#endif

#ifndef LWA_COLORKEY
#define LWA_COLORKEY            0x00000001
#endif

#ifndef LWA_ALPHA
#define LWA_ALPHA               0x00000002
#endif

#ifndef TTM_ADJUSTRECT
#define TTM_ADJUSTRECT          (WM_USER + 31)
#endif

#if _MSC_VER < 1300

//------------------------------
// Windows 64 bit compatibility:
//------------------------------

#ifndef GetClassLongPtr
#define GetClassLongPtr		GetClassLong
#endif

#ifndef SetClassLongPtr
#define SetClassLongPtr		SetClassLong
#endif

#ifndef SetWindowLongPtr
#define SetWindowLongPtr	SetWindowLong
#endif

#ifndef GetWindowLongPtr
#define GetWindowLongPtr	GetWindowLong
#endif

#define	DWORD_PTR			DWORD
#define	INT_PTR				int
#define	UINT_PTR			UINT
#define	LONG_PTR			LONG

#ifndef GWLP_WNDPROC
#define	GWLP_WNDPROC		GWL_WNDPROC
#endif

#ifndef GCLP_HICON
#define	GCLP_HICON			GCL_HICON
#endif

#ifndef GCLP_HICONSM
#define	GCLP_HICONSM		GCL_HICONSM
#endif

#ifndef GCLP_HBRBACKGROUND
#define GCLP_HBRBACKGROUND	GCL_HBRBACKGROUND
#endif
                
#endif // _MSC_VER

#if _MSC_VER >= 1400
#define BCGNcHitTestType	LRESULT
#else
#define BCGNcHitTestType	UINT
#endif

// Windows Vista Notifications:
#ifndef WM_DWMCOMPOSITIONCHANGED
#define WM_DWMCOMPOSITIONCHANGED        0x031E
#endif

#ifndef WM_DWMNCRENDERINGCHANGED
#define WM_DWMNCRENDERINGCHANGED        0x031F
#endif

#ifndef WM_DWMCOLORIZATIONCOLORCHANGED
#define WM_DWMCOLORIZATIONCOLORCHANGED  0x0320
#endif

#ifndef WM_DWMWINDOWMAXIMIZEDCHANGE
#define WM_DWMWINDOWMAXIMIZEDCHANGE     0x0321
#endif

//---------------------------------------------------
// Uncomment some of these definitions to exlude
// non-required features and reduce the library size:
//---------------------------------------------------

//#define BCGP_EXCLUDE_GRID_CTRL
//#define BCGP_EXCLUDE_PLANNER
//#define BCGP_EXCLUDE_EDIT_CTRL
//#define BCGP_EXCLUDE_PROP_LIST
//#define BCGP_EXCLUDE_POPUP_WINDOW
//#define BCGP_EXCLUDE_SHELL
//#define BCGP_EXCLUDE_TOOLBOX
//#define BCGP_EXCLUDE_HOT_SPOT_IMAGE
//#define BCGP_EXCLUDE_ANIM_CTRL
//#define BCGP_EXCLUDE_TASK_PANE
//#define BCGP_EXCLUDE_RIBBON

#endif // __BCGCBPRO_H

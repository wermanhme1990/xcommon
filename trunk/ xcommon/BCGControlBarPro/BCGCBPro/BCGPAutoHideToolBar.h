#if !defined(AFX_BCGPAutoHideTOOLBAR_H__DDB558FD_2415_4DAD_BE40_ED8BB0E1B159__INCLUDED_)
#define AFX_BCGPAutoHideTOOLBAR_H__DDB558FD_2415_4DAD_BE40_ED8BB0E1B159__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BCGPAutoHideToolBar.h : header file
//

#include "BCGCBPro.h"
#include "BCGPControlBar.h"

class CBCGPAutoHideButton;
class CBCGPDockingControlBar;

/////////////////////////////////////////////////////////////////////////////
// CBCGPAutoHideToolBar window

class BCGCBPRODLLEXPORT CBCGPAutoHideToolBar : public CBCGPControlBar
{
	DECLARE_DYNCREATE(CBCGPAutoHideToolBar)
// Construction
public:
	CBCGPAutoHideToolBar();

// Attributes
public:
	static int m_nShowAHWndDelay;
// Operations
public:

	CBCGPAutoHideButton* AddAutoHideWindow (CBCGPDockingControlBar* pAutoHideWnd, DWORD dwAlignment);
	BOOL RemoveAutoHideWindow (CBCGPDockingControlBar* pAutoHideWnd);
	BOOL ShowAutoHideWindow (CBCGPDockingControlBar* pAutoHideWnd, BOOL bShow, BOOL bDelay);

	void UpdateVisibleState ();
	int GetVisibleCount ();

	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CSize StretchControlBar (int nLength, BOOL bVert);

	virtual void SetActiveInGroup (BOOL bActive);

	void SetRecentVisibleState (BOOL bState) {m_bRecentVisibleState = bState;}
	void UnSetAutoHideMode (CBCGPDockingControlBar* pFirstBarInGroup);

	CBCGPDockingControlBar* GetFirstAHWindow ();

	static CRuntimeClass* m_pAutoHideButtonRTS;
	
	virtual BOOL OnShowControlBarMenu (CPoint /*point*/)	{	return FALSE;	}
	virtual BOOL AllowShowOnControlMenu () const			{	return FALSE;	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPAutoHideToolBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBCGPAutoHideToolBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPAutoHideToolBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CBCGPAutoHideButton* ButtonFromPoint			(CPoint pt);
	CBCGPAutoHideButton* ButtonFromAutoHideWindow	(CBCGPDockingControlBar* pWnd);
	void CleanUpAutoHideButtons ();

	virtual void DoPaint(CDC* pDC);

protected:
	CObList					m_lstAutoHideButtons;
	CBCGPAutoHideButton*	m_pLastActiveButton;

	BOOL					m_bReadyToDisplayAHWnd;
	UINT_PTR				m_nDisplayAHWndTimerID;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPAutoHideTOOLBAR_H__DDB558FD_2415_4DAD_BE40_ED8BB0E1B159__INCLUDED_)

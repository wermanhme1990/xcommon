// BCGPFullScreenImpl.h: interface for the BCGPFullScreenImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPFULLSCREENIMPL_H__6FBF448A_6F12_4008_97C1_6682B67ECE10__INCLUDED_)
#define AFX_BCGPFULLSCREENIMPL_H__6FBF448A_6F12_4008_97C1_6682B67ECE10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBCGPToolBar;
class CBCGPFrameImpl;

#include "BCGCBPro.h"

class BCGCBPRODLLEXPORT CBCGPFullScreenImpl  
{
	friend class CBCGPFrameImpl;

public:
	CBCGPFullScreenImpl(CBCGPFrameImpl* pFrameImpl);
	virtual ~CBCGPFullScreenImpl();

	void ShowFullScreen();

	void ShowFullScreen(CFrameWnd* pFrame);
	void RestoreState(CFrameWnd* pFrame);

	CRect GetFullScreenRect() const { return m_rectFullScreenWindow; }

	BOOL IsFullScreen() const { return m_bFullScreen; }

	void EnableMainMenu(BOOL bShow = TRUE)
	{
		m_bShowMenu = bShow;
	}
	void SetFullScreenID(UINT uiFullScreenID)
	{
		m_uiFullScreenID = uiFullScreenID;
	}

	void EnableTabsArea(BOOL bShowTabs)
	{
		m_bTabsArea = bShowTabs;
	}

	void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);

protected:
	CRect					m_rectFullScreenWindow;
	CBCGPToolBar*			m_pwndFullScreenBar;
	BOOL					m_bFullScreen;
	BOOL					m_bShowMenu;
	CRect					m_rectFramePrev;
	CBCGPFrameImpl*         m_pImpl;
	UINT					m_uiFullScreenID;
	BOOL					m_bTabsArea;
	CString					m_strRegSection;

protected:
	void UnDockAndHideControlBars(CFrameWnd* pFrame);
};

#endif // !defined(AFX_BCGPFULLSCREENIMPL_H__6FBF448A_6F12_4008_97C1_6682B67ECE10__INCLUDED_)

// MSNVisualManager.h: interface for the CMSNVisualManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSNVISUALMANAGER_H__4D0D5C93_717B_498F_B567_A8120EA51CDE__INCLUDED_)
#define AFX_MSNVISUALMANAGER_H__4D0D5C93_717B_498F_B567_A8120EA51CDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMSNVisualManager : public CBCGPWinXPVisualManager
{
	DECLARE_DYNCREATE(CMSNVisualManager)

public:
	CMSNVisualManager();
	virtual ~CMSNVisualManager();

	virtual void OnFillPopupWindowBackground (CDC* pDC, CRect rect);
	virtual void OnDrawPopupWindowBorder (CDC* pDC, CRect rect);
	virtual COLORREF OnDrawPopupWindowCaption (CDC* pDC, CRect rectCaption, CBCGPPopupWindow* pPopupWnd);
};

#endif // !defined(AFX_MSNVISUALMANAGER_H__4D0D5C93_717B_498F_B567_A8120EA51CDE__INCLUDED_)

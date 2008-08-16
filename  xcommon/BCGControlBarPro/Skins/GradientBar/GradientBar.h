// GradientBar.h: interface for the CGradientBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRADIENTBAR_H__918E8076_7590_442A_ADA0_8A1E897D73F0__INCLUDED_)
#define AFX_GRADIENTBAR_H__918E8076_7590_442A_ADA0_8A1E897D73F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGradientBar : public CBCGPVisualManagerXP
{
	DECLARE_DYNCREATE(CGradientBar)

public:
	CGradientBar();
	virtual ~CGradientBar();

	virtual void OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
									CRect rectClient, CRect rectClip,
									BOOL bNCArea = FALSE);
	virtual void OnDrawBarBorder (CDC* pDC, CBCGPBaseControlBar* pBar, CRect& rect);
};

#endif // !defined(AFX_GRADIENTBAR_H__918E8076_7590_442A_ADA0_8A1E897D73F0__INCLUDED_)

// CustomVisualManager.h: interface for the CCustomVisualManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMVISUALMANAGER_H__4C2E1CEA_916E_4CB6_A768_A82BE9ADA450__INCLUDED_)
#define AFX_CUSTOMVISUALMANAGER_H__4C2E1CEA_916E_4CB6_A768_A82BE9ADA450__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCustomVisualManager : public CBCGPVisualManager2003
{
	DECLARE_DYNCREATE(CCustomVisualManager)

	virtual void OnFillTasksPaneBackground(CDC* pDC, CRect rectWorkArea);

	virtual void OnDrawTasksGroupCaption(CDC* pDC, CBCGPTasksGroup* pGroup, BOOL bIsHighlighted = FALSE, BOOL bIsSelected = FALSE, BOOL bCanCollapse = FALSE);

	virtual void OnFillTasksGroupInterior(CDC* pDC, CRect rect, BOOL bSpecial = FALSE);
	virtual void OnDrawTasksGroupAreaBorder(CDC* pDC, CRect rect, BOOL bSpecial = FALSE, BOOL bNoTitle = FALSE);
	virtual void OnDrawTask(CDC* pDC, CBCGPTask* pTask, CImageList* pIcons, BOOL bIsHighlighted = FALSE, BOOL bIsSelected = FALSE);

	virtual void OnDrawAppBarBorder (CDC* pDC, CBCGPAppBarWnd* pAppBarWnd,
									CRect rectBorder, CRect rectBorderSize);

	virtual void OnDrawAppBarCaption (	CDC* pDC, CBCGPAppBarWnd* pAppBarWnd, 
										CRect rectCaption, CString strCaption);

	virtual void OnDrawScrollButtons(CDC* pDC, const CRect& rect, const int nBorderSize,
									int iImage, BOOL bHilited);

public:
	CCustomVisualManager ();

};

#endif // !defined(AFX_CUSTOMVISUALMANAGER_H__4C2E1CEA_916E_4CB6_A768_A82BE9ADA450__INCLUDED_)

// BCGPDlgImpl.h: interface for the CBCGPDlgImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPDLGIMPL_H__18772215_4E74_4900_82E4_288CA46AB7E0__INCLUDED_)
#define AFX_BCGPDLGIMPL_H__18772215_4E74_4900_82E4_288CA46AB7E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGCBPro.h"

class CBCGPPopupMenu;

class BCGCBPRODLLEXPORT CBCGPDlgImpl  
{
	friend class CBCGPDialog;
	friend class CBCGPPropertyPage;
	friend class CBCGPPropertySheet;

protected:
	CBCGPDlgImpl(CWnd& dlg);
	virtual ~CBCGPDlgImpl();

	static LRESULT CALLBACK BCGDlgMouseProc (int nCode, WPARAM wParam, LPARAM lParam);

	void SetActiveMenu (CBCGPPopupMenu* pMenu);

	BOOL ProcessMouseClick (POINT pt);
	BOOL ProcessMouseMove (POINT pt);

	BOOL PreTranslateMessage(MSG* pMsg);
	BOOL OnCommand (WPARAM wParam, LPARAM lParam);
	void OnNcActivate (BOOL& bActive);
	void OnActivate(UINT nState, CWnd* pWndOther);

	void OnDestroy ();

	CWnd&					m_Dlg;
	static HHOOK			m_hookMouse;
	static CBCGPDlgImpl*	m_pMenuDlgImpl;
};

#endif // !defined(AFX_BCGPDLGIMPL_H__18772215_4E74_4900_82E4_288CA46AB7E0__INCLUDED_)

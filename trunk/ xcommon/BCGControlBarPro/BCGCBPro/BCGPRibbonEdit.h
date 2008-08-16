//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGPro Library
// Copyright (C) 1998-2006 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPRibbonEdit.h: interface for the CBCGPRibbonEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPRIBBONEDIT_H__FE5913DD_04A0_45EC_A7AF_DA1EA9090D39__INCLUDED_)
#define AFX_BCGPRIBBONEDIT_H__FE5913DD_04A0_45EC_A7AF_DA1EA9090D39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGPRibbonButton.h"

#ifndef BCGP_EXCLUDE_RIBBON

class CBCGPRibbonEditCtrl;
class CBCGPRibbonSpinButtonCtrl;

class BCGCBPRODLLEXPORT CBCGPRibbonEdit : public CBCGPRibbonButton  
{
	friend class CBCGPRibbonEditCtrl;

	DECLARE_DYNCREATE(CBCGPRibbonEdit)

// Construction:
public:
	CBCGPRibbonEdit(UINT nID, int nWidth, LPCTSTR lpszLabel = NULL, int nImage = -1);
	virtual ~CBCGPRibbonEdit();

protected:
	CBCGPRibbonEdit();

// Attributes:
public:
	CString GetEditText () const
	{
		return m_strEdit;
	};

	void SetEditText (CString strText);

	void EnableSpinButtons (int nMin, int nMax);

// Overrides
public:
	virtual CSize GetRegularSize (CDC* pDC);
	virtual CSize GetCompactSize (CDC* pDC);

	virtual void Redraw ();

	virtual BOOL IsHighlighted () const
	{
		return m_bIsHighlighted || m_bIsEditFocused;
	}

	virtual BOOL HasFocus () const
	{
		return m_bIsEditFocused;
	}

	virtual void OnDraw (CDC* pDC);
	virtual void OnDrawOnList (CDC* pDC, CString strText, int nTextOffset, CRect rect, BOOL bIsSelected, BOOL bHighlighted);

	virtual void OnLButtonDown (CPoint point);
	virtual void OnLButtonUp (CPoint point);

	virtual void CopyFrom (const CBCGPBaseRibbonElement& src);

	virtual BOOL CanBeStretched ()
	{
		return FALSE;
	}

	virtual void OnAfterChangeRect (CDC* pDC);
	virtual void OnShow (BOOL bShow);

	virtual CBCGPRibbonEditCtrl* CreateEdit (CWnd* pWndParent, DWORD dwEditStyle);

	void CommonInit ();

	virtual void OnEnable (BOOL bEnable);
	virtual void DropDownList () {}

// Attributes:
protected:
	BOOL						m_bIsEditFocused;
	CString						m_strEdit;
	CBCGPRibbonEditCtrl*		m_pWndEdit;
	CBCGPRibbonSpinButtonCtrl*	m_pWndSpin;
	int							m_nWidth;
	BOOL						m_bHasDropDownList;
	BOOL						m_bHasSpinButtons;
	int							m_nMin;
	int							m_nMax;
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonEditCtrl

class BCGCBPRODLLEXPORT CBCGPRibbonEditCtrl : public CRichEditCtrl
{
// Construction
public:
	CBCGPRibbonEditCtrl(CBCGPRibbonEdit& edit);

// Attributes
protected:
	CBCGPRibbonEdit&	m_edit;
	BOOL				m_bTracked;
	BOOL				m_bIsContextMenu;
	CString				m_strOldText;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPRibbonEditCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBCGPRibbonEditCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPRibbonEditCtrl)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM,LPARAM);
	afx_msg void OnChange();
	DECLARE_MESSAGE_MAP()
};

#endif // BCGP_EXCLUDE_RIBBON

#endif // !defined(AFX_BCGPRIBBONEDIT_H__FE5913DD_04A0_45EC_A7AF_DA1EA9090D39__INCLUDED_)

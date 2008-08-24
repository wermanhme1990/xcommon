/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XFlatEdit.h					
文件内容:	
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/08/19
*********************************************************************/

#ifndef HWX_XFLATEDIT_H
#define HWX_XFLATEDIT_H


#if _MSC_VER > 1000
#pragma once
#endif 

class CXFlatEdit : public CEdit
{
	DECLARE_DYNAMIC(CXFlatEdit)
public:
	CXFlatEdit();	
	virtual ~CXFlatEdit();
public:

	void DisableFlatLook(BOOL bDisable);	
	BOOL IsFlat();
	virtual BOOL PointInRect();protected:
	static void DrawBorders(CDC* , CXFlatEdit*, const CRect&, BOOL blMouseOn = FALSE);
	DECLARE_MESSAGE_MAP()
	//{{AFX_MSG(CXFlatEdit)	
	afx_msg	void OnNcPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnPrintClient(WPARAM wp, LPARAM lp);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE
protected:
	void DrawBorders(CDC* , const CRect&);
	
protected:
	BOOL m_bPainted;  // Used during paint operations.
	BOOL m_bHasFocus; // TRUE if the control has focus.
	BOOL m_bFlatLook; // TRUE if the control is flat.
	DWORD m_nStyle;   // Stores the standard window styles for the control.
	DWORD m_nStyleEx; // Stores the extended window styles for the control.
};

//////////////////////////////////////////////////////////////////////


#endif // HWX_XFLATEDIT_H

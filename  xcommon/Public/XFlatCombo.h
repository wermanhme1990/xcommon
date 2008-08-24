
#ifndef XUI__XCOMBOBOX
#define XUI__XCOMBOBOX

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCJComboBox window

typedef enum tagDRAWSTATE
{
	FC_DRAWNORMAL = 0,
	FC_DRAWRAISED,
	FC_DRAWPRESSD
} DRAWSTATE;

class CXFlatComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CXFlatComboBox)

// Construction
public:
	CXFlatComboBox();

// Attributes
public:
protected:
	BOOL m_bLBtnDown;
	BOOL m_bPainted;
	BOOL m_bFlatLook;
// Operations
public:
protected:
	void DrawCombo(DRAWSTATE eStyle, COLORREF clrTopLeft, COLORREF clrBottomRight);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXFlatComboBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXFlatComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXFlatComboBox)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // XUI__XCOMBOBOX

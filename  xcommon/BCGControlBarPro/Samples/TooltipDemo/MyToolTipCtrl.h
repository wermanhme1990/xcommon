#if !defined(AFX_MYTOOLTIPCTRL_H__4DFCDE8B_A73B_41FB_BA79_BB0F0D68DBB2__INCLUDED_)
#define AFX_MYTOOLTIPCTRL_H__4DFCDE8B_A73B_41FB_BA79_BB0F0D68DBB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyToolTipCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyToolTipCtrl window

class CMyToolTipCtrl : public CBCGPToolTipCtrl
{
	DECLARE_DYNCREATE(CMyToolTipCtrl)

// Construction
public:
	CMyToolTipCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyToolTipCtrl)
	//}}AFX_VIRTUAL

	virtual void SetDescription (const CString strDesrciption);
	virtual CSize GetIconSize ();
	virtual void OnFillBackground (CDC* pDC, CRect rect, COLORREF& clrText, COLORREF& clrLine);

	virtual BOOL OnDrawIcon (CDC* pDC, CRect rectImage);
	virtual void OnDrawSeparator (CDC* pDC, int x1, int x2, int y);

// Implementation
public:
	virtual ~CMyToolTipCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyToolTipCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	HICON	m_hIcon;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTOOLTIPCTRL_H__4DFCDE8B_A73B_41FB_BA79_BB0F0D68DBB2__INCLUDED_)

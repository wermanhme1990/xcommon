
#ifndef XUI__XCOMBOBOX
#define XUI__XCOMBOBOX

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCJComboBox window

class CXFlatComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CXFlatComboBox)

// Construction
public:
	CXFlatComboBox();
// Attributes
protected:	
	BOOL m_bFlatLook;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXFlatComboBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXFlatComboBox();	
protected:
	virtual void NotifyOwner(UINT nCode);
	// Generated message map functions
protected:
	//{{AFX_MSG(CXFlatComboBox)	
	afx_msg void OnPaint();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
public:
protected:	
	virtual void PreSubclassWindow();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // XUI__XCOMBOBOX

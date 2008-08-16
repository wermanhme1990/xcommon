// TooltipDemoView.h : interface of the CTooltipDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLTIPDEMOVIEW_H__EC299164_3DDE_46F8_A2EC_237CCB0938CF__INCLUDED_)
#define AFX_TOOLTIPDEMOVIEW_H__EC299164_3DDE_46F8_A2EC_237CCB0938CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTooltipDemoView : public CFormView
{
protected: // create from serialization only
	CTooltipDemoView();
	DECLARE_DYNCREATE(CTooltipDemoView)

public:
	//{{AFX_DATA(CTooltipDemoView)
	enum { IDD = IDD_TOOLTIPDEMO_FORM };
	int		m_nTTType;
	BOOL	m_bBoldLabel;
	BOOL	m_bCustomColors;
	BOOL	m_bDrawDescription;
	BOOL	m_bDrawIcon;
	BOOL	m_bRoundedCorners;
	BOOL	m_bDrawSeparator;
	BOOL	m_bTTInPopupMenus;
	//}}AFX_DATA

// Attributes
public:
	CTooltipDemoDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTooltipDemoView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTooltipDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTooltipDemoView)
	afx_msg void OnApplyParams();
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()

	void EnableControls ();
};

#ifndef _DEBUG  // debug version in TooltipDemoView.cpp
inline CTooltipDemoDoc* CTooltipDemoView::GetDocument()
   { return (CTooltipDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLTIPDEMOVIEW_H__EC299164_3DDE_46F8_A2EC_237CCB0938CF__INCLUDED_)

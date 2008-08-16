// SmartDockingDemoView.h : interface of the CSmartDockingDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMARTDOCKINGDEMOVIEW_H__08BCFCA9_6683_4C8A_AD88_F47371F54227__INCLUDED_)
#define AFX_SMARTDOCKINGDEMOVIEW_H__08BCFCA9_6683_4C8A_AD88_F47371F54227__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSmartDockingDemoView : public CFormView
{
protected: // create from serialization only
	CSmartDockingDemoView();
	DECLARE_DYNCREATE(CSmartDockingDemoView)

public:
	//{{AFX_DATA(CSmartDockingDemoView)
	enum { IDD = IDD_SMARTDOCKINGDEMO_FORM };
	CStatic	m_wndColorLabel;
	CBCGPColorButton	m_wndColor;
	int		m_nMarkerType;
	BOOL	m_bPropViewVertical;
	//}}AFX_DATA

// Attributes
public:
	CSmartDockingDemoDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmartDockingDemoView)
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
	virtual ~CSmartDockingDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSmartDockingDemoView)
	afx_msg void OnChangeLook();
	afx_msg void OnPropViewVertical();
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()

	CPalette	m_palColorPicker;	// Palette for color picker
};

#ifndef _DEBUG  // debug version in SmartDockingDemoView.cpp
inline CSmartDockingDemoDoc* CSmartDockingDemoView::GetDocument()
   { return (CSmartDockingDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMARTDOCKINGDEMOVIEW_H__08BCFCA9_6683_4C8A_AD88_F47371F54227__INCLUDED_)

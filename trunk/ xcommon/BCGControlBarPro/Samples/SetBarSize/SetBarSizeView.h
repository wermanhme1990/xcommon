// SetBarSizeView.h : interface of the CSetBarSizeView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETBARSIZEVIEW_H__C3EB69FE_9BA0_4965_92C9_10F9F8F68528__INCLUDED_)
#define AFX_SETBARSIZEVIEW_H__C3EB69FE_9BA0_4965_92C9_10F9F8F68528__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSetBarSizeView : public CFormView
{
protected: // create from serialization only
	CSetBarSizeView();
	DECLARE_DYNCREATE(CSetBarSizeView)

public:
	//{{AFX_DATA(CSetBarSizeView)
	enum { IDD = IDD_SETBARSIZE_FORM };
	UINT	m_nSizePercent;
	UINT	m_nContainerSize;
	UINT	m_nHeightInPixels;
	UINT	m_nWidthInPixels;
	//}}AFX_DATA

// Attributes
public:
	CSetBarSizeDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetBarSizeView)
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
	virtual ~CSetBarSizeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSetBarSizeView)
	afx_msg void OnButtonSetDlgBarSize();
	afx_msg void OnButtonSetContainerSize();
	afx_msg void OnBtnSetHeightInPixels();
	afx_msg void OnBtnSetWidthInPixels();
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SetBarSizeView.cpp
inline CSetBarSizeDoc* CSetBarSizeView::GetDocument()
   { return (CSetBarSizeDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETBARSIZEVIEW_H__C3EB69FE_9BA0_4965_92C9_10F9F8F68528__INCLUDED_)

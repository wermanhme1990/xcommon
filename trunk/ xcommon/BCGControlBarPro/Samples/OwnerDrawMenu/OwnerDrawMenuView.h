// OwnerDrawMenuView.h : interface of the COwnerDrawMenuView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OWNERDRAWMENUVIEW_H__B12380C9_4BC0_4FA4_8F63_D3AEF2B8E329__INCLUDED_)
#define AFX_OWNERDRAWMENUVIEW_H__B12380C9_4BC0_4FA4_8F63_D3AEF2B8E329__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class COwnerDrawMenuView : public CView
{
protected: // create from serialization only
	COwnerDrawMenuView();
	DECLARE_DYNCREATE(COwnerDrawMenuView)

// Attributes
public:
	COwnerDrawMenuDoc* GetDocument();

protected:
	UINT m_uiLineStyleCmd;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COwnerDrawMenuView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COwnerDrawMenuView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(COwnerDrawMenuView)
	afx_msg void OnMoreLines();
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	afx_msg void OnLineStyle(UINT uiCmd);
	afx_msg void OnUpdateLineStyle(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in OwnerDrawMenuView.cpp
inline COwnerDrawMenuDoc* COwnerDrawMenuView::GetDocument()
   { return (COwnerDrawMenuDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OWNERDRAWMENUVIEW_H__B12380C9_4BC0_4FA4_8F63_D3AEF2B8E329__INCLUDED_)

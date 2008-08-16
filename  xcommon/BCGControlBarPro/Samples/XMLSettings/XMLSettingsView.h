// XMLSettingsView.h : interface of the CXMLSettingsView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLSETTINGSVIEW_H__4ED4CCF6_E01C_4F53_8F07_BE89E5FBAA4F__INCLUDED_)
#define AFX_XMLSETTINGSVIEW_H__4ED4CCF6_E01C_4F53_8F07_BE89E5FBAA4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXMLSettingsView : public CView
{
protected: // create from serialization only
	CXMLSettingsView();
	DECLARE_DYNCREATE(CXMLSettingsView)

// Attributes
public:
	CXMLSettingsDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMLSettingsView)
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
	virtual ~CXMLSettingsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CXMLSettingsView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in XMLSettingsView.cpp
inline CXMLSettingsDoc* CXMLSettingsView::GetDocument()
   { return (CXMLSettingsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMLSETTINGSVIEW_H__4ED4CCF6_E01C_4F53_8F07_BE89E5FBAA4F__INCLUDED_)

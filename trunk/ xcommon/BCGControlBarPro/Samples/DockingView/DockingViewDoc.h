// DockingViewDoc.h : interface of the CDockingViewDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOCKINGVIEWDOC_H__6F2181AB_E486_4BA5_BE57_1BE6BD956E51__INCLUDED_)
#define AFX_DOCKINGVIEWDOC_H__6F2181AB_E486_4BA5_BE57_1BE6BD956E51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDockingViewDoc : public CDocument
{
protected: // create from serialization only
	CDockingViewDoc();
	DECLARE_DYNCREATE(CDockingViewDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDockingViewDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDockingViewDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDockingViewDoc)
	afx_msg void OnViewUpdateallviews();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOCKINGVIEWDOC_H__6F2181AB_E486_4BA5_BE57_1BE6BD956E51__INCLUDED_)

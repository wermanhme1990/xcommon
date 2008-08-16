// BCGPExplorerDoc.h : interface of the CBCGPExplorerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPEXPLORERDOC_H__D94573A4_57A5_4913_AC6D_4DA5B0A065D5__INCLUDED_)
#define AFX_BCGPEXPLORERDOC_H__D94573A4_57A5_4913_AC6D_4DA5B0A065D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBCGPExplorerDoc : public CDocument
{
protected: // create from serialization only
	CBCGPExplorerDoc();
	DECLARE_DYNCREATE(CBCGPExplorerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPExplorerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBCGPExplorerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPExplorerDoc)
	afx_msg void OnCommandHistory();
	afx_msg void OnGoBack();
	afx_msg void OnGoForward();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPEXPLORERDOC_H__D94573A4_57A5_4913_AC6D_4DA5B0A065D5__INCLUDED_)

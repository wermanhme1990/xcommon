// BCGCBDotNetExampleDoc.h : interface of the CBCGCBDotNetExampleDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGCBDOTNETEXAMPLEDOC_H__1260A711_4153_43F3_ADAF_952FAD30AC46__INCLUDED_)
#define AFX_BCGCBDOTNETEXAMPLEDOC_H__1260A711_4153_43F3_ADAF_952FAD30AC46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBCGCBDotNetExampleDoc : public CDocument
{
protected: // create from serialization only
	CBCGCBDotNetExampleDoc();
	DECLARE_DYNCREATE(CBCGCBDotNetExampleDoc)

// Attributes
public:
	UINT	m_uiExampleID;
	UINT	m_uiExampleXMLSettingsID;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGCBDotNetExampleDoc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual void SetTitle(LPCTSTR lpszTitle);
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL
	
// Implementation
public:
	void CheckTitle();

	virtual ~CBCGCBDotNetExampleDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL	m_bTitleMark;

	void CommonInit ();

// Generated message map functions
protected:
	//{{AFX_MSG(CBCGCBDotNetExampleDoc)
	afx_msg void OnDummyBuild();
	afx_msg void OnDummyCompile();
	afx_msg void OnDummyExecute();
	afx_msg void OnDummyGo();
	afx_msg void OnDummySelectActiveConfiguration();
	afx_msg void OnDummyClean();
	afx_msg void OnDummyRebuildAll();
	afx_msg void OnToolsMacro();
	afx_msg void OnNewDialog();
	afx_msg void OnNewAccelerator();
	afx_msg void OnNewBitmap();
	afx_msg void OnNewCursor();
	afx_msg void OnNewIcon();
	afx_msg void OnNewMenu();
	afx_msg void OnNewStringTable();
	afx_msg void OnNewToolbar();
	afx_msg void OnNewVersion();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGCBDOTNETEXAMPLEDOC_H__1260A711_4153_43F3_ADAF_952FAD30AC46__INCLUDED_)

// RibbonMDIDemoDoc.h : interface of the CRibbonMDIDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIBBONMDIDEMODOC_H__30D7EC58_0849_4B46_8FBE_41948835771A__INCLUDED_)
#define AFX_RIBBONMDIDEMODOC_H__30D7EC58_0849_4B46_8FBE_41948835771A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRibbonMDIDemoDoc : public CDocument
{
protected: // create from serialization only
	CRibbonMDIDemoDoc();
	DECLARE_DYNCREATE(CRibbonMDIDemoDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRibbonMDIDemoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRibbonMDIDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRibbonMDIDemoDoc)
	afx_msg void OnFileClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RIBBONMDIDEMODOC_H__30D7EC58_0849_4B46_8FBE_41948835771A__INCLUDED_)

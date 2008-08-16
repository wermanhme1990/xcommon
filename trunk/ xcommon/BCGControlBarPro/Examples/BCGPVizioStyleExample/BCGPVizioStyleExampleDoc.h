// BCGPVizioStyleExampleDoc.h : interface of the CBCGPVizioStyleExampleDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPVIZIOSTYLEEXAMPLEDOC_H__AFBF51EE_5868_4D3C_B42A_687BCAC1E201__INCLUDED_)
#define AFX_BCGPVIZIOSTYLEEXAMPLEDOC_H__AFBF51EE_5868_4D3C_B42A_687BCAC1E201__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBCGPVizioStyleExampleDoc : public CDocument
{
protected: // create from serialization only
	CBCGPVizioStyleExampleDoc();
	DECLARE_DYNCREATE(CBCGPVizioStyleExampleDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPVizioStyleExampleDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBCGPVizioStyleExampleDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPVizioStyleExampleDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPVIZIOSTYLEEXAMPLEDOC_H__AFBF51EE_5868_4D3C_B42A_687BCAC1E201__INCLUDED_)

// DrawEffectsDoc.h : interface of the CDrawEffectsDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWEFFECTSDOC_H__5C23C425_4F81_4BE9_B8FA_F1107B32C914__INCLUDED_)
#define AFX_DRAWEFFECTSDOC_H__5C23C425_4F81_4BE9_B8FA_F1107B32C914__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDrawEffectsDoc : public CDocument
{
protected: // create from serialization only
	CDrawEffectsDoc();
	DECLARE_DYNCREATE(CDrawEffectsDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawEffectsDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDrawEffectsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDrawEffectsDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWEFFECTSDOC_H__5C23C425_4F81_4BE9_B8FA_F1107B32C914__INCLUDED_)

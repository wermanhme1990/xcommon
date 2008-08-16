// ActiveAccessibilityDemoDoc.h : interface of the CActiveAccessibilityDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACTIVEACCESSIBILITYDEMODOC_H__79CCA407_E48A_4DA3_93B8_50F8247963E5__INCLUDED_)
#define AFX_ACTIVEACCESSIBILITYDEMODOC_H__79CCA407_E48A_4DA3_93B8_50F8247963E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CActiveAccessibilityDemoDoc : public CDocument
{
protected: // create from serialization only
	CActiveAccessibilityDemoDoc();
	DECLARE_DYNCREATE(CActiveAccessibilityDemoDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActiveAccessibilityDemoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CActiveAccessibilityDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CActiveAccessibilityDemoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIVEACCESSIBILITYDEMODOC_H__79CCA407_E48A_4DA3_93B8_50F8247963E5__INCLUDED_)

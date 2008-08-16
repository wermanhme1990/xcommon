// UserToolsDoc.h : interface of the CUserToolsDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERTOOLSDOC_H__08794E75_EB6B_4F1E_BBB9_90E8530F42CF__INCLUDED_)
#define AFX_USERTOOLSDOC_H__08794E75_EB6B_4F1E_BBB9_90E8530F42CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CUserToolsDoc : public CDocument
{
protected: // create from serialization only
	CUserToolsDoc();
	DECLARE_DYNCREATE(CUserToolsDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserToolsDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUserToolsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CUserToolsDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERTOOLSDOC_H__08794E75_EB6B_4F1E_BBB9_90E8530F42CF__INCLUDED_)

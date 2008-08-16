// MDITabsDemoDoc.h : interface of the CMDITabsDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDITABSDEMODOC_H__31F7119E_C6B0_4DAE_BD77_A45E11F9A677__INCLUDED_)
#define AFX_MDITABSDEMODOC_H__31F7119E_C6B0_4DAE_BD77_A45E11F9A677__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMDITabsDemoDoc : public CDocument
{
protected: // create from serialization only
	CMDITabsDemoDoc();
	DECLARE_DYNCREATE(CMDITabsDemoDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDITabsDemoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMDITabsDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMDITabsDemoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDITABSDEMODOC_H__31F7119E_C6B0_4DAE_BD77_A45E11F9A677__INCLUDED_)

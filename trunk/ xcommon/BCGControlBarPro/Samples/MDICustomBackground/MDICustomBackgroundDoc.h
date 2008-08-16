// MDICustomBackgroundDoc.h : interface of the CMDICustomBackgroundDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDICUSTOMBACKGROUNDDOC_H__4E1C1FBC_A2C3_4960_AAA8_4CB213F22D67__INCLUDED_)
#define AFX_MDICUSTOMBACKGROUNDDOC_H__4E1C1FBC_A2C3_4960_AAA8_4CB213F22D67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMDICustomBackgroundDoc : public CDocument
{
protected: // create from serialization only
	CMDICustomBackgroundDoc();
	DECLARE_DYNCREATE(CMDICustomBackgroundDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDICustomBackgroundDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMDICustomBackgroundDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMDICustomBackgroundDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDICUSTOMBACKGROUNDDOC_H__4E1C1FBC_A2C3_4960_AAA8_4CB213F22D67__INCLUDED_)

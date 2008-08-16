// XMLSettingsDoc.h : interface of the CXMLSettingsDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLSETTINGSDOC_H__B654E278_AAFC_4A98_9E8C_6E31D6C886D7__INCLUDED_)
#define AFX_XMLSETTINGSDOC_H__B654E278_AAFC_4A98_9E8C_6E31D6C886D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXMLSettingsDoc : public CDocument
{
protected: // create from serialization only
	CXMLSettingsDoc();
	DECLARE_DYNCREATE(CXMLSettingsDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMLSettingsDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXMLSettingsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CXMLSettingsDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMLSETTINGSDOC_H__B654E278_AAFC_4A98_9E8C_6E31D6C886D7__INCLUDED_)

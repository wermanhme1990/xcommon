// MDIBarsDoc.h : interface of the CMDIBarsDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDIBARSDOC_H__5FB564EE_0998_11D5_95C8_00A0C9289F1B__INCLUDED_)
#define AFX_MDIBARSDOC_H__5FB564EE_0998_11D5_95C8_00A0C9289F1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMDIBarsDoc : public CDocument
{
protected: // create from serialization only
	CMDIBarsDoc();
	DECLARE_DYNCREATE(CMDIBarsDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDIBarsDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMDIBarsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMDIBarsDoc)
	afx_msg void OnCommand1();
	afx_msg void OnCommand2();
	afx_msg void OnCommand3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDIBARSDOC_H__5FB564EE_0998_11D5_95C8_00A0C9289F1B__INCLUDED_)

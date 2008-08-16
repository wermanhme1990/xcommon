// OutlookMultiViewsDoc.h : interface of the COutlookMultiViewsDoc class
//
/////////////////////////////////////////////////////////////////////////////
//
// This sample is based on Vswap32 sample:
// http://support.microsoft.com/default.aspx?scid=kb;en-us;141334

#if !defined(AFX_OUTLOOKMULTIVIEWSDOC_H__4F3E8852_E9AF_4E6B_94F8_392A97FD63D8__INCLUDED_)
#define AFX_OUTLOOKMULTIVIEWSDOC_H__4F3E8852_E9AF_4E6B_94F8_392A97FD63D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class COutlookMultiViewsDoc : public CDocument
{
protected: // create from serialization only
	COutlookMultiViewsDoc();
	DECLARE_DYNCREATE(COutlookMultiViewsDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutlookMultiViewsDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COutlookMultiViewsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(COutlookMultiViewsDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTLOOKMULTIVIEWSDOC_H__4F3E8852_E9AF_4E6B_94F8_392A97FD63D8__INCLUDED_)

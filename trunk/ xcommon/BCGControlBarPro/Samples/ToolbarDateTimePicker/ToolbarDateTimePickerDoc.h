// ToolbarDateTimePickerDoc.h : interface of the CToolbarDateTimePickerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBARDATETIMEPICKERDOC_H__59449EAA_D51E_42FC_8364_38B835B1AC28__INCLUDED_)
#define AFX_TOOLBARDATETIMEPICKERDOC_H__59449EAA_D51E_42FC_8364_38B835B1AC28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CToolbarDateTimePickerDoc : public CDocument
{
protected: // create from serialization only
	CToolbarDateTimePickerDoc();
	DECLARE_DYNCREATE(CToolbarDateTimePickerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolbarDateTimePickerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolbarDateTimePickerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CToolbarDateTimePickerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARDATETIMEPICKERDOC_H__59449EAA_D51E_42FC_8364_38B835B1AC28__INCLUDED_)

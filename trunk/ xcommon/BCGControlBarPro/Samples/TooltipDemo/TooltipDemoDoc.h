// TooltipDemoDoc.h : interface of the CTooltipDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLTIPDEMODOC_H__DB230C3D_5C03_46AB_A36A_CF42EDBDF619__INCLUDED_)
#define AFX_TOOLTIPDEMODOC_H__DB230C3D_5C03_46AB_A36A_CF42EDBDF619__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTooltipDemoDoc : public CDocument
{
protected: // create from serialization only
	CTooltipDemoDoc();
	DECLARE_DYNCREATE(CTooltipDemoDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTooltipDemoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTooltipDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTooltipDemoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLTIPDEMODOC_H__DB230C3D_5C03_46AB_A36A_CF42EDBDF619__INCLUDED_)

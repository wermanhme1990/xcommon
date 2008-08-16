// PaletteDemoDoc.h : interface of the CPaletteDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PALETTEDEMODOC_H__3F9B386C_0A56_11D5_95C8_00A0C9289F1B__INCLUDED_)
#define AFX_PALETTEDEMODOC_H__3F9B386C_0A56_11D5_95C8_00A0C9289F1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPaletteDemoDoc : public CDocument
{
protected: // create from serialization only
	CPaletteDemoDoc();
	DECLARE_DYNCREATE(CPaletteDemoDoc)

// Attributes
public:
	UINT	m_uiCurrTool;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaletteDemoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPaletteDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPaletteDemoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnPaletteTool(UINT uiCmd);
	afx_msg void OnUpdatePaletteTool(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PALETTEDEMODOC_H__3F9B386C_0A56_11D5_95C8_00A0C9289F1B__INCLUDED_)

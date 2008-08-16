#if !defined(AFX_TBLISTCTRL_H__72726133_C364_4E7B_86C6_FA7B52358B10__INCLUDED_)
#define AFX_TBLISTCTRL_H__72726133_C364_4E7B_86C6_FA7B52358B10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TBListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTBListCtrl window

class CTBListCtrl;

class CTBData
{
public:
	CTBData ();

	static CLIPFORMAT GetClipboardFormat ();
	static LPCTSTR GetClipboardFormatName ();
	static BOOL IsClipboardFormatAvailable (COleDataObject* pDataObject);

	static BOOL SerializeTo (CFile& file, CBCGPToolBoxButton* pTool, int nPage, int nTool);
	static BOOL SerializeFrom (CFile& file, CString& strLabel, CImageList& imageList,
							 int& nPage, int& nTool);

protected:
	static CLIPFORMAT s_ClpFormat;
	static CString s_ClpFormatName;

	static BOOL Serialize (CArchive& ar, CString& strLabel, CImageList& imageList,
						 int& nPage, int& nTool);
};

class CTBDropTarget : public COleDropTarget
{
public:
	CTBDropTarget();
	virtual ~CTBDropTarget();

// Operations
public:

	BOOL Register (CTBListCtrl* pCtrl);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPPlannerDropTarget)
	public:
	virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave(CWnd* pWnd);
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDropEx(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, DROPEFFECT dropList, CPoint point);
	//}}AFX_VIRTUAL

protected:
	CTBListCtrl*				m_pCtrl;
};

class CTBListCtrl : public CListCtrl
{
	friend class CTBDropTarget;
// Construction
public:
	CTBListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTBListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTBListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTBListCtrl)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);

protected:
	CTBDropTarget m_DropTarget;

	CImageList m_ImageList;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TBLISTCTRL_H__72726133_C364_4E7B_86C6_FA7B52358B10__INCLUDED_)

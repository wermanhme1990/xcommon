// ToolBox.h: interface for the CToolBoxBar class.
//

#if !defined(AFX_TOOLBOX_H__F9C28947_2C41_47AD_9BAB_E2516CEEC6E0__INCLUDED_)
#define AFX_TOOLBOX_H__F9C28947_2C41_47AD_9BAB_E2516CEEC6E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CToolBoxBar : public CBCGPToolBox
{
// Construction
public:
	CToolBoxBar();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBoxBar)
	//}}AFX_VIRTUAL

	virtual BOOL OnShowToolboxMenu (CPoint point, CBCGPToolBoxPage* pPage, int nHit);

// Implementation
public:
	virtual ~CToolBoxBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolBoxBar)
	afx_msg void OnToolboxAddTab();
	afx_msg void OnToolboxChooseItems();
	afx_msg void OnToolboxCopy();
	afx_msg void OnToolboxCut();
	afx_msg void OnToolboxDelete();
	afx_msg void OnToolboxListView();
	afx_msg void OnToolboxPaste();
	afx_msg void OnToolboxRename();
	afx_msg void OnToolboxReset();
	afx_msg void OnUpdateToolboxListView(CCmdUI* pCmdUI);
	afx_msg void OnToolboxShowAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CToolBoxBarEx : public CBCGPToolBoxEx
{
// Construction
public:
	CToolBoxBarEx();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBoxBarEx)
	//}}AFX_VIRTUAL

	virtual BOOL OnShowToolboxMenu (CPoint point, CBCGPToolBoxPage* pPage, int nHit);

// Implementation
public:
	virtual ~CToolBoxBarEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolBoxBarEx)
	afx_msg void OnToolboxAddTab();
	afx_msg void OnToolboxChooseItems();
	afx_msg void OnToolboxCopy();
	afx_msg void OnToolboxCut();
	afx_msg void OnToolboxDelete();
	afx_msg void OnToolboxListView();
	afx_msg void OnToolboxPaste();
	afx_msg void OnToolboxRename();
	afx_msg void OnToolboxReset();
	afx_msg void OnUpdateToolboxListView(CCmdUI* pCmdUI);
	afx_msg void OnToolboxShowAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_TOOLBOX_H__F9C28947_2C41_47AD_9BAB_E2516CEEC6E0__INCLUDED_)

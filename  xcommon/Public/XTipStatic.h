/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XTipStatic.h					
文件内容:	
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/08/18
*********************************************************************/
#ifndef X_TIPSTATIC_H
#define X_TIPSTATIC_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CXTipStatic window
#define WM_CREATETIP			WM_USER + 100
#define WM_UPDATETIP			WM_USER + 101
#define WM_ENABLETIP			WM_USER + 102
class CXTipStatic : public CStatic
{
	// Construction/destruction
public:
	CXTipStatic();
	virtual ~CXTipStatic();

	// Attributes
public:		
	void EnableToolTip(BOOL = TRUE);
	void SetToolTip(LPCTSTR);
	CString GetToolTip() const;
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXTipStatic)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// Implementation
protected:   

	// Protected attributes
protected:  
	CToolTipCtrl m_ToolTip;                         // The tooltip			
	CString  m_strToolTip;   
protected:
	//{{AFX_MSG(CXTipStatic) 
	afx_msg LRESULT OnCreateToolTip(WPARAM w, LPARAM l);
	afx_msg LRESULT OnUpdateTip(WPARAM, LPARAM);
	afx_msg LRESULT OnEnableToolTip(WPARAM w, LPARAM l);
	//}}AFX_MSG    
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // X_TIPSTATIC_H

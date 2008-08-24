/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XEMaillink.h					
文件内容:	
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/08/21
*********************************************************************/
#ifndef HWX_EMAILLINK
#define HWX_EMAILLINK

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XHyperlink.h"
/////////////////////////////////////////////////////////////////////////////
// CXEMailLink window

class DLLXEXPORT CXEMailLink : public CXHyperLink
{
// Construction/destruction
public:
    CXEMailLink();
    virtual ~CXEMailLink();
// Attributes
public:

// Operations
public:    
    //}}AFX_VIRTUAL
	virtual BOOL GotoURL(LPCTSTR url, int showcmd);
// Implementation
protected: 
	virtual void ReportError();
// Protected attributes
protected:   
protected:
    //{{AFX_MSG(CXEMailLink)    
    //}}AFX_MSG    
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // HWX_EMAILLINK

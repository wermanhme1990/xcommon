/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XTipStatic.cpp					
文件内容:	
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/08/18
*********************************************************************/
#include "stdafx.h"
#include "XTipStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CXTipStatic
CXTipStatic::CXTipStatic()
{	
	
}

CXTipStatic::~CXTipStatic()
{
	
}

BEGIN_MESSAGE_MAP(CXTipStatic, CStatic)

//{{AFX_MSG_MAP(CXTipStatic)		
ON_MESSAGE(WM_CREATETIP, OnCreateToolTip)
ON_MESSAGE(WM_UPDATETIP, OnUpdateTip)
ON_MESSAGE(WM_ENABLETIP, OnEnableToolTip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTipStatic message handlers
BOOL CXTipStatic::PreTranslateMessage(MSG* pMsg) 
{
	if (::IsWindow(m_ToolTip.GetSafeHwnd()))
	{
		m_ToolTip.RelayEvent(pMsg);
	}	
	return __super::PreTranslateMessage(pMsg);
}

void CXTipStatic::PreSubclassWindow() 
{
	PostMessage(WM_CREATETIP);
	__super::PreSubclassWindow();
}

void CXTipStatic::SetToolTip(LPCTSTR lpszToolTip)
{
	m_strToolTip = lpszToolTip;
	PostMessage(WM_UPDATETIP);
}
CString CXTipStatic::GetToolTip() const
{
	return m_strToolTip;
}
void CXTipStatic::EnableToolTip( BOOL blEnable/*= TRUE*/ )
{
	PostMessage(WM_ENABLETIP, WPARAM(blEnable), 0);	
}
LRESULT CXTipStatic::OnCreateToolTip( WPARAM w, LPARAM l )
{
	if (!::IsWindow(m_ToolTip.GetSafeHwnd()))
	{
		//没有Notify属性 static不捕捉鼠标消息
		DWORD dwStyle = GetStyle();	
		::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);
		CString strWndText;
		m_ToolTip.Create(this);
		GetWindowText(strWndText);
		CRect rtClient; 
		GetClientRect(rtClient);
		m_ToolTip.AddTool(this, m_strToolTip/*, rtClient, GetDlgCtrlID()*/);	

		if (!strWndText.IsEmpty()) 
		{		
			SetToolTip(strWndText);
		}
	}		
	return 1L;
}

LRESULT CXTipStatic::OnUpdateTip(WPARAM w, LPARAM l)
{	
	if (::IsWindow(m_ToolTip.GetSafeHwnd())) 
	{		
		m_ToolTip.UpdateTipText(m_strToolTip, this/*, GetDlgCtrlID()*/);	
	}
	return 1L;
}

LRESULT CXTipStatic::OnEnableToolTip(WPARAM w, LPARAM l)
{
	BOOL blEnable = (BOOL) w;
	BOOL blReturn = FALSE;
	if (::IsWindow(m_ToolTip.GetSafeHwnd()))
	{		
		blReturn = m_ToolTip.EnableWindow(blEnable);
	}
	return blReturn;
}
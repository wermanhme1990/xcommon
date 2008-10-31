#include "stdafx.h"
#include "ProcessMainWnd.h"
CProcessMainWndArray::CProcessMainWndArray( DWORD dwProcessId /*= 0*/ ) 
: m_dwProcessId(dwProcessId)
{
	Attach(m_dwProcessId);	
}
CProcessMainWndArray::~CProcessMainWndArray()
{

}
BOOL __stdcall CProcessMainWndArray::EnumProc( HWND hWnd, LPARAM lParam )
{
	CProcessMainWndArray* pWndIterator = (CProcessMainWndArray*)lParam;
	DWORD dwCurProcessId = 0;
	GetWindowThreadProcessId(hWnd, &dwCurProcessId);
	if (GetParent(hWnd) == NULL &&
		(dwCurProcessId == pWndIterator->GetCurProcessId())) 
	{
		TRACE(TEXT("Wnd:%d\n"), hWnd);
		pWndIterator->GetMainHwnd()->push_back(hWnd);
	}
	return TRUE;
}
DWORD CProcessMainWndArray::GetCurProcessId()
{
	return m_dwProcessId;
}
std::vector<HWND>* CProcessMainWndArray::GetMainHwnd()
{
	return &m_vHwnd;
}
BOOL CProcessMainWndArray::Empty()
{
	return m_vHwnd.empty();
}
LONG CProcessMainWndArray::Count() const
{
	return m_vHwnd.size();
}
HWND CProcessMainWndArray::operator[]( LONG nIndex )
{
	return m_vHwnd[nIndex];
}
void CProcessMainWndArray::Attach( DWORD dwPID)
{
	Detach();
	m_dwProcessId = dwPID;
	EnumWindows(EnumProc, (LPARAM)this);
}
void CProcessMainWndArray::Detach()
{
	m_vHwnd.clear();
	m_dwProcessId = 0;
}
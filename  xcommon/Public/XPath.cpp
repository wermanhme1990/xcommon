/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XPath.cpp					
文件内容:	
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/08/22
*********************************************************************/

#include "stdafx.h"
#include "XPath.h"
#include <shlwapi.h>
#pragma  comment (lib, "shlwapi.lib")

CXPathA::CXPathA( LPCSTR lpszPath) : CXStringA(lpszPath)
{

}
CXPathA::CXPathA() : CXStringA()
{
	
}
CXPathA::~CXPathA()
{

}
void CXPathA::Append( LPCSTR lpszPath)
{
	assert(lpszPath);
	assert(lpszPath);
	LONG nLen = lstrlenA(lpszPath) + StrLen();
	nLen = _min(nLen, MAX_PATH + 1);
	LPSTR pchBuffer = GetBuffer(nLen);
	PathAppendA(pchBuffer, lpszPath);
	ReleaseBuffer();
}
const CXPathA& CXPathA::operator=(LPCSTR lpszPath)
{
	Reset(lpszPath);
	return *this;
}
BOOL CXPathA::FileExists() const
{
	return PathFileExistsA(C_Str());	
}
BOOL CXPathA::IsDirectory() const
{
	return PathIsDirectoryA(C_Str());
}
BOOL CXPathA::LongPathToShortPath()
{
	CHAR achPath[MAX_PATH];
	if (GetShortPathNameA(C_Str(), achPath, MAX_PATH))
	{
		Reset(achPath);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CXPathA::ShortPathToLongPath()
{
	CHAR achPath[MAX_PATH];
	if (GetLongPathNameA(C_Str(), achPath, MAX_PATH))
	{
		Reset(achPath);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CXPathA::FindExt( LPSTR lpszExt) const
{		
	return  !_splitpath_s(C_Str(), NULL, 0, NULL, 0, NULL, 0, lpszExt, MAX_EXT);	
}

CXPathW::CXPathW( LPCWSTR lpszPath) : CXStringW(lpszPath)
{

}
CXPathW::CXPathW() : CXStringW()
{

}
CXPathW::~CXPathW()
{

}
void CXPathW::Append( LPCWSTR lpszPath)
{
	assert(lpszPath);
	LONG nLen = lstrlenW(lpszPath) + StrLen();
	nLen = _min(nLen, MAX_PATH + 1);
	LPWSTR pwhBuffer = GetBuffer(nLen);
	PathAppendW(pwhBuffer, lpszPath);
	ReleaseBuffer();
}

const CXPathW& CXPathW::operator=(LPCWSTR lpszPath)
{
	Reset(lpszPath);
	return *this;
}
BOOL CXPathW::FileExists() const
{
	return PathFileExistsW(C_Str());	
}
BOOL CXPathW::IsDirectory() const
{
	return PathFileExistsW(C_Str());
}
BOOL CXPathW::LongPathToShortPath()
{
	WCHAR achPath[MAX_PATH];
	if (GetShortPathNameW(C_Str(), achPath, MAX_PATH))
	{
		Reset(achPath);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CXPathW::ShortPathToLongPath()
{
	WCHAR achPath[MAX_PATH];
	if (GetLongPathNameW(C_Str(), achPath, MAX_PATH))
	{
		Reset(achPath);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CXPathW::FindExt( LPWSTR lpszExt) const
{
	return  !_wsplitpath_s(C_Str(), NULL, 0, NULL, 0, NULL, 0, lpszExt, MAX_EXT);	
}
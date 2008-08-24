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
	nLen = _max(nLen, MAX_PATH + 1);
	LPSTR pchBuffer = GetBuffer(nLen);
	PathAppendA(pchBuffer, lpszPath);
	ReleaseBuffer();
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
	nLen = _max(nLen, MAX_PATH + 1);
	LPWSTR pwhBuffer = GetBuffer(nLen);
	PathAppendW(pwhBuffer, lpszPath);
	ReleaseBuffer();
}


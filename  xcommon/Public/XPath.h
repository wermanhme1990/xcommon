/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XPath.h					
文件内容:	
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/08/22
*********************************************************************/
#ifndef HWX_PATH_H
#define HWX_PATH_H
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "HWXString.h"

class DLLXEXPORT CXPathA : public CXStringA
{
public:
	CXPathA(LPCSTR);
	CXPathA();
	virtual ~CXPathA();
	void Append(LPCSTR);

};

class DLLXEXPORT CXPathW : public CXStringW
{
public:
	CXPathW(LPCWSTR);
	CXPathW();
	virtual ~CXPathW();
	void Append(LPCWSTR);
};


#ifdef UNICODE
typedef CXPathW CXPathT;
#else 
typedef CXPathA CXPathT;
#endif
//typedef CXPathT CXPath;


#endif// HWX_PATH_H
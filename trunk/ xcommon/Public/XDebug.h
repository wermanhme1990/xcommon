/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XDebug.h					
文件内容:		
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/10/06
*********************************************************************/
#ifndef HWX_DEBUG_H
#define HWX_DEBUG_H
#include <Ext_Type.h>



#ifndef TRACE
#if defined(DEBUG) || defined(_DEBUG)
#define TRACE		XTrace
#else 
#define TRACE		__noop
#endif
#endif

#ifdef UNICODE
#define XTrace		XTraceW
#else 
#define XTrace		XTraceA
#endif

#ifdef __cplusplus
extern "C"
{
#endif
	//释放内存
	DLLXEXPORT void WINAPI    XTraceW(LPCWSTR, ...);	
	DLLXEXPORT void WINAPI		XTraceA(LPCSTR , ...);
#ifdef __cplusplus
}
#endif


#endif//HWX_DEBUG_H
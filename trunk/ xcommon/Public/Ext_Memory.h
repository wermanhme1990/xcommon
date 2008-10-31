/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称:   Ext_Memory.h          
文件内容:  内存操作的封装 可以提高程序可调适性
版本历史:  1.0
作者:    xuejuntao xuejuntao@hanwang.com.cn 2008/02/26
*********************************************************************/
#ifndef HWXUE_EXT_MEMORY_H_INC
#define HWXUE_EXT_MEMORY_H_INC

#include "Ext_Type.h"
#include <malloc.h>
//此文件中的函数 Ext_Mem_NewMemory Ext_Mem_ResizeMemory Ext_Mem_FreeMemory 在核心中不能使用
#undef  ZeroMemory
#define ZeroMemory					XMemZero
#define Mem_ResizeMemory    XMemResize
#define Mem_FreeMemory      XMemFree
#define Mem_NewMemory       XMemAlloc
#define Mem_FillMemory      XMemFill
#define Mem_ZeroMemory      XMemZero
#define Mem_EqualMemory     XMemEqual
#define Mem_CopyMemory      XMemCopy

#ifdef __cplusplus
extern "C"{
#endif
//释放内存
DLLXEXPORT void WINAPI    XMemFree(void** pvMem);
/*分配新的内存*/
DLLXEXPORT BOOL WINAPI    XMemAlloc(void** ppvMem, LONG nSize);
/*调整内存大小*/
DLLXEXPORT BOOL WINAPI    XMemResize(void** ppvMem, LONG nSizeNew);
DLLXEXPORT BOOL WINAPI    XMemFill(void *pvMem, char bChrFill, LONG nSize);
/*内存置0*/
DLLXEXPORT BOOL WINAPI    XMemZero(void *pvMem, LONG nSize);
/*内存是否相等*/
DLLXEXPORT BOOL  WINAPI   XMemEqual(const void *pvMemDest, const void *pvMemSrc, LONG nSize);
DLLXEXPORT  LONG WINAPI   XMemSet(void *p, const CHAR& b, const LONG& n );
DLLXEXPORT  LONG WINAPI   XMemCopy(void *pDest, const void *const pSource, const LONG& n );
DLLXEXPORT  LONG WINAPI   XMemMove(void *pDest, const void *const pSrc, const LONG& n );
DLLXEXPORT  LONG WINAPI   XMemCompare(void *const pDest, void *const pSrc, const LONG& n );
#ifdef __cplusplus
}
#endif

#endif//HWXUE_EXT_MEMORY_H_INC




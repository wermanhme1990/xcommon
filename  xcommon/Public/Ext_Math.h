
/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称:   Ext_Math.h          
文件内容:  简单的一些常用数学操作
版本历史:  1.0
作者:    xuejuntao xuejuntao@hanwang.com.cn 2008/02/26
*********************************************************************/
#ifndef  _HWXUE_Ext_Math_INC
#define _HWXUE_Ext_Math_INC
#include "Ext_Type.h"
#include <XNotesAPI.h>
//求点到线段的最短距离 及最短距离点
long Mth_GetMinDist(const HWPoint &tPtA, const HWPoint &tPtB, const HWPoint &tPtC, HWPoint &tNearestPt);
long DivRound(long x, long div);
long MulDivRound(long x, long mul, long div);

#ifdef __cplusplus
extern "C"{
#endif
  DLLXEXPORT long XSqrt(const long& nRoot);//整数开方函数，
  DLLXEXPORT void XQsort( void *base, long num, long width, long *pComp);//快速排序函数
  DLLXEXPORT void ZB_SwapIdx ( char *a,	char *b, unsigned long nIdxWidth);

#ifdef __cplusplus
}
#endif

#endif // !defined(_HWXUE_Ext_Math_INC)

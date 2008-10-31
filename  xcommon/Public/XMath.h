/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XMath.h					
文件内容:	
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/09/01
*********************************************************************/
#ifndef  HWX_MATH_H
#define HWX_MATH_H
#include "Ext_Type.h"

template<typename LPXPoint>
RECT XMth_GetBoxEx(LPXPoint pBgn, LPXPoint pEnd);

template<typename LPXPoint>
RECT XMth_GetBox(LPXPoint pBgn, LPXPoint pEnd);


#include "XMath.cpp"
#endif // !defined(HWX_MATH_H)

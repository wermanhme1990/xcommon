/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XPoint.h					
文件内容:	    代替MFC中CXPoint
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/06/20
*********************************************************************/
#pragma once
#include "Ext_Type.h"
#include <wtypes.h>
#include "XSize.h"

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
#endif

class CXSize;
class CXRect;
class DLLXEXPORT CXPoint : public tagPOINT
{
public:
  // Constructors

  // create an uninitialized point
  CXPoint() throw();
  // create from two integers
  CXPoint(const LONG& initX, const LONG& initY) throw();
  // create from another point
  CXPoint(const POINT& initPt) throw();
  // create from a size
  CXPoint(const SIZE& initSize) throw();
  // create from an LPARAM: x = LOWORD(dw) y = HIWORD(dw)
  CXPoint(LPARAM dwPoint) throw();


  // Operations

  // translate the point
  void Offset(const LONG& xOffset, const LONG& yOffset) throw();
  void Offset(const POINT& point) throw();
  void Offset(const SIZE& size) throw();
  void SetPoint(const LONG& X, const LONG& Y) throw();

  BOOL operator==(const POINT& point) const throw();
  BOOL operator!=(const POINT& point) const throw();
  void operator+=(const SIZE& size) throw();
  void operator-=(const SIZE& size) throw();
  void operator+=(const POINT& point) throw();
  void operator-=(const POINT& point) throw();

  // Operators returning CXPoint values
  CXPoint operator+(const SIZE& size) const throw();
  CXPoint operator-(const SIZE& size) const throw();
  CXPoint operator-() const throw();
  CXPoint operator+(const POINT& point) const throw();

  // Operators returning CSize values
  CXSize operator-(const POINT& point) const throw();

  // Operators returning CRect values
  CXRect operator+(const RECT* lpRect) const throw();
  CXRect operator-(const RECT* lpRect) const throw();
};


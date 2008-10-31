/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XSize.h					
文件内容:	    代替MFC中CSize
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/06/20
*********************************************************************/
#pragma once
#include "Ext_Type.h"
#include <wtypes.h>
#include "XPoint.h"

class CXSize;
class CXRect;
class CXPoint;

class DLLXEXPORT CXSize : public tagSIZE
{
public:
  // Constructors
  // construct an uninitialized size
  CXSize() throw();
  // create from two integers
  CXSize(const LONG& initCX, const LONG& initCY) throw();
  // create from another size
  CXSize(const SIZE& initSize) throw();
  // create from a point
  CXSize(const POINT& initPt) throw();
  // create from a DWORD: cx = LOWORD(dw) cy = HIWORD(dw)
  CXSize(const DWORD& dwSize) throw();

  // Operations
  BOOL operator==(const SIZE& size) const throw();
  BOOL operator!=(const SIZE& size) const throw();
  void operator+=(const SIZE& size) throw();
  void operator-=(const SIZE& size) throw();
  void SetSize(const LONG& CX, const LONG& CY) throw();

  // Operators returning CXSize values
  CXSize operator+(const SIZE& size) const throw();
  CXSize operator-(const SIZE& size) const throw();
  CXSize operator-() const throw();

  // Operators returning CPoint values
  CXPoint operator+(const POINT& point) const throw();
  CXPoint operator-(const POINT& point) const throw();

  // Operators returning CRect values
  CXRect operator+(const RECT* lpRect) const throw();
  CXRect operator-(const RECT* lpRect) const throw();
};

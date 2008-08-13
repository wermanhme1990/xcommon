/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XRect.h					
文件内容:	    代替MFC中CXRect
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/06/20
*********************************************************************/
#pragma once
#include <wtypes.h>
#include "XSize.h"
#include "XPoint.h"

class CXSize;
class CXRect;
class CXPoint;

class DLLXEXPORT CXRect : public tagRECT
{
  // Constructors
public:
  // uninitialized rectangle
  CXRect() throw();
  // from left, top, right, and bottom
  CXRect(const LONG& l, const LONG& t, const LONG& r, const LONG& b) throw();
  // copy constructor
  CXRect(const RECT& srcRect) throw();
  // from a pointer to another rect
  CXRect(LPCRECT lpSrcRect) throw();
  // from a point and size
  CXRect(const POINT& point, const SIZE& size) throw();
  // from two points
  CXRect(const POINT& topLeft, const POINT& bottomRight) throw();
  /*CXRect t;*/
  // Attributes (in addition to RECT members)

  // retrieves the width
  LONG Width() const throw();
  // returns the height
  LONG Height() const throw();
  // returns the size
  CXSize Size() const throw();
  // reference to the top-left point
  CXPoint& TopLeft() throw();
  // reference to the bottom-right point
  CXPoint& BottomRight() throw();
  // const reference to the top-left point
  const CXPoint& TopLeft() const throw();
  // const reference to the bottom-right point
  const CXPoint& BottomRight() const throw();
  // the geometric center point of the rectangle
  CXPoint CenterPoint() const throw();
  // swap the left and right
  void SwapLeftRight() throw();
  static void WINAPI SwapLeftRight(LPRECT lpRect) throw();

  // convert between CXRect and LPRECT/LPCRECT (no need for &)
  operator LPRECT() throw();
  operator LPCRECT() const throw();

  // returns TRUE if rectangle has no area
  BOOL IsRectEmpty() const throw();
  // returns TRUE if rectangle is at (0,0) and has no area
  BOOL IsRectNull() const throw();
  // returns TRUE if point is within rectangle
  BOOL PtInRect(POINT point) const throw();

  // Operations

  // set rectangle from left, top, right, and bottom
  void SetRect(const LONG& x1, const LONG& y1, const LONG& x2, const LONG& y2) throw();
  void SetRect(const POINT& topLeft, const POINT& bottomRight) throw();
  // empty the rectangle
  void SetRectEmpty() throw();
  // copy from another rectangle
  void CopyRect(LPCRECT lpSrcRect) throw();
  // TRUE if exactly the same as another rectangle
  BOOL EqualRect(LPCRECT lpRect) const throw();

  // Inflate rectangle's width and height by
  // x units to the left and right ends of the rectangle
  // and y units to the top and bottom.
  void InflateRect(const LONG& x, const LONG& y) throw();
  // Inflate rectangle's width and height by
  // size.cx units to the left and right ends of the rectangle
  // and size.cy units to the top and bottom.
  void InflateRect(const SIZE& size) throw();
  // Inflate rectangle's width and height by moving individual sides.
  // Left side is moved to the left, right side is moved to the right,
  // top is moved up and bottom is moved down.
  void InflateRect(LPCRECT lpRect) throw();
  void InflateRect(const LONG& l, const LONG& t, const LONG& r, const LONG& b) throw();

  // deflate the rectangle's width and height without
  // moving its top or left
  void DeflateRect(const LONG& x, const LONG& y) throw();
  void DeflateRect(const SIZE& size) throw();
  void DeflateRect(LPCRECT lpRect) throw();
  void DeflateRect(const LONG& l, const LONG& t, const LONG& r, const LONG& b) throw();

  // translate the rectangle by moving its top and left
  void OffsetRect(const LONG& x, const LONG& y) throw();
  void OffsetRect(const SIZE& size) throw();
  void OffsetRect(const POINT& point) throw();
  void NormalizeRect() throw();

  // absolute position of rectangle
  void MoveToY(const LONG& y) throw();
  void MoveToX(const LONG& x) throw();
  void MoveToXY(const LONG& x, const LONG& y) throw();
  void MoveToXY(const POINT& point) throw();

  // set this rectangle to intersection of two others
  BOOL IntersectRect(LPCRECT lpRect1, LPCRECT lpRect2) throw();

  // set this rectangle to bounding union of two others
  BOOL UnionRect(LPCRECT lpRect1, LPCRECT lpRect2) throw(); 
  // set this rectangle to minimum of two others
  BOOL SubtractRect(LPCRECT lpRectSrc1, LPCRECT lpRectSrc2) throw();

  // Additional Operations
  void operator=(const RECT& srcRect) throw();
  BOOL operator==(const RECT& rect) const throw();
  BOOL operator!=(const RECT& rect) const throw();
  void operator+=(const POINT& point) throw();
  void operator+=(const SIZE& size) throw();
  void operator+=(LPCRECT lpRect) throw();
  void operator-=(const POINT& point) throw();
  void operator-=(const SIZE& size) throw();
  void operator-=(LPCRECT lpRect) throw();
  void operator&=(const RECT& rect) throw();
  void operator|=(const RECT& rect) throw();

  // Operators returning CXRect values
  CXRect operator+(const POINT& point) const throw();
  CXRect operator-(const POINT& point) const throw();
  CXRect operator+(LPCRECT lpRect) const throw();
  CXRect operator+(const SIZE& size) const throw();
  CXRect operator-(const SIZE& size) const throw();
  CXRect operator-(LPCRECT lpRect) const throw();
  CXRect operator&(const RECT& rect2) const throw();
  CXRect operator|(const RECT& rect2) const throw();
  CXRect MulDiv(const LONG& nMultiplier, const LONG& nDivisor) const throw();
};


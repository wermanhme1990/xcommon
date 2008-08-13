/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XRect.cpp					
文件内容:	    代替MFC中CXRect
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/06/20
*********************************************************************/
#include "stdafx.h"
#include "XRect.h"

// CXRect
CXRect::CXRect() throw()
{ 
  left = 0; top = 0; right = 0; bottom = 0; 
}
CXRect::CXRect(const LONG& l, const LONG&  t, const LONG&  r, const LONG&  b) throw()
{ 
  left = l; top = t; right = r; bottom = b; 
}
CXRect::CXRect(const RECT& srcRect) throw()
{ 
  ::CopyRect(this, &srcRect); 
}
CXRect::CXRect(LPCRECT lpSrcRect) throw()
{ 
  ::CopyRect(this, lpSrcRect); 
}
CXRect::CXRect(const POINT& point, const SIZE& size) throw()
{ 
  right = (left = point.x) + size.cx; bottom = (top = point.y) + size.cy; 
}
CXRect::CXRect(const POINT& topLeft, const POINT& bottomRight) throw()
{ 
  left = topLeft.x; 
  top = topLeft.y;
  right = bottomRight.x;
  bottom = bottomRight.y;
}
LONG CXRect::Width() const throw()
{ 
  return right - left; 
}
LONG CXRect::Height() const throw()
{
  return bottom - top; 
}
CXSize CXRect::Size() const throw()
{ 
  return CXSize(right - left, bottom - top);
}
CXPoint& CXRect::TopLeft() throw()
{ 
  return *((CXPoint*)this); 
}
CXPoint& CXRect::BottomRight() throw()
{ 
  return *((CXPoint*)this+1);
}
const CXPoint& CXRect::TopLeft() const throw()
{
  return *((CXPoint*)this); 
}
const CXPoint& CXRect::BottomRight() const throw()
{
  return *((CXPoint*)this+1); 
}
CXPoint CXRect::CenterPoint() const throw()
{ 
  return CXPoint((left + right)/2, (top + bottom)/2); 
}
void CXRect::SwapLeftRight() throw()
{ 
  SwapLeftRight(LPRECT(this)); 
}
void WINAPI CXRect::SwapLeftRight(LPRECT lpRect) throw()
{ 
  _swap(lpRect->left, lpRect->right);  
}
CXRect::operator LPRECT() throw()
{ 
  return this; 
}
CXRect::operator LPCRECT() const throw()
{ 
  return this;
}
BOOL CXRect::IsRectEmpty() const throw()
{ 
  return ::IsRectEmpty(this);
}
BOOL CXRect::IsRectNull() const throw()
{ 
  return (left == 0 && right == 0 && top == 0 && bottom == 0); 
}
BOOL CXRect::PtInRect(POINT point) const throw()
{
  return ::PtInRect(this, point); 
}
void CXRect::SetRect(const LONG& x1, const LONG& y1, const LONG& x2, const LONG& y2) throw()
{ 
  ::SetRect(this, x1, y1, x2, y2); 
}
void CXRect::SetRect(const POINT& topLeft, const POINT& bottomRight) throw()
{ 
  ::SetRect(this, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y); 
}
void CXRect::SetRectEmpty() throw()
{ 
  ::SetRectEmpty(this); 
}
void CXRect::CopyRect(LPCRECT lpSrcRect) throw()
{ 
  ::CopyRect(this, lpSrcRect); 
}
BOOL CXRect::EqualRect(LPCRECT lpRect) const throw()
{
  return ::EqualRect(this, lpRect);
}
void CXRect::InflateRect(const LONG& x, const LONG& y) throw()
{ 
  ::InflateRect(this, x, y); 
}
void CXRect::InflateRect(const SIZE& size) throw()
{ 
  ::InflateRect(this, size.cx, size.cy); 
}
void CXRect::DeflateRect(const LONG& x, const LONG& y) throw()
{
  ::InflateRect(this, -x, -y);
}
void CXRect::DeflateRect(const SIZE& size) throw()
{ 
  ::InflateRect(this, -size.cx, -size.cy); 
}
void CXRect::OffsetRect(const LONG& x, const LONG& y) throw()
{
  ::OffsetRect(this, x, y); 
}
void CXRect::OffsetRect(const POINT& point) throw()
{ 
  ::OffsetRect(this, point.x, point.y);
}
void CXRect::OffsetRect(const SIZE& size) throw()
{ 
  ::OffsetRect(this, size.cx, size.cy); 
}
void CXRect::MoveToY(const LONG& y) throw()
{
  bottom = Height() + y; 
  top = y;
}
void CXRect::MoveToX(const LONG& x) throw()
{ 
  right = Width() + x; left = x; 
}
void CXRect::MoveToXY(const LONG& x, const LONG& y) throw()
{ 
  MoveToX(x); MoveToY(y); 
}
void CXRect::MoveToXY(const POINT& pt) throw()
{ 
  MoveToX(pt.x); 
	MoveToY(pt.y);
}
BOOL CXRect::IntersectRect(LPCRECT lpRect1, LPCRECT lpRect2) throw()
{ 
  return ::IntersectRect(this, lpRect1, lpRect2);
}
BOOL CXRect::UnionRect(LPCRECT lpRect1, LPCRECT lpRect2) throw()
{ 
  return ::UnionRect(this, lpRect1, lpRect2); 
}
void CXRect::operator=(const RECT& srcRect) throw()
{ 
  ::CopyRect(this, &srcRect); 
}
BOOL CXRect::operator==(const RECT& rect) const throw()
{ 
  return ::EqualRect(this, &rect);
}
BOOL CXRect::operator!=(const RECT& rect) const throw()
{ 
  return !::EqualRect(this, &rect);
}
void CXRect::operator+=(const POINT& point) throw()
{
  ::OffsetRect(this, point.x, point.y);
}
void CXRect::operator+=(const SIZE& size) throw()
{
  ::OffsetRect(this, size.cx, size.cy); 
}
void CXRect::operator+=(LPCRECT lpRect) throw()
{
  InflateRect(lpRect); 
}
void CXRect::operator-=(const POINT& point) throw()
{
  ::OffsetRect(this, -point.x, -point.y); 
}
void CXRect::operator-=(const SIZE& size) throw()
{ 
  ::OffsetRect(this, -size.cx, -size.cy); 
}
void CXRect::operator-=(LPCRECT lpRect) throw()
{
  DeflateRect(lpRect); 
}
void CXRect::operator&=(const RECT& rect) throw()
{ 
  ::IntersectRect(this, this, &rect); 
}
void CXRect::operator|=(const RECT& rect) throw()
{ 
  ::UnionRect(this, this, &rect);
}
CXRect CXRect::operator+(const POINT& pt) const throw()
{
  CXRect rect(*this); 
  ::OffsetRect(&rect, pt.x, pt.y); 
  return rect; 
}
CXRect CXRect::operator-(const POINT& pt) const throw()
{
  CXRect rect(*this); 
  ::OffsetRect(&rect, -pt.x, -pt.y); 
  return rect;
}
CXRect CXRect::operator+(const SIZE& size) const throw()
{ 
  CXRect rect(*this); 
  ::OffsetRect(&rect, size.cx, size.cy); 
  return rect;
}
CXRect CXRect::operator-(const SIZE& size) const throw()
{
  CXRect rect(*this); 
  ::OffsetRect(&rect, -size.cx, -size.cy); 
  return rect; 
}
CXRect CXRect::operator+(LPCRECT lpRect) const throw()
{ 
  CXRect rect(this); 
  rect.InflateRect(lpRect);
  return rect; 
}
CXRect CXRect::operator-(LPCRECT lpRect) const throw()
{
  CXRect rect(this); 
  rect.DeflateRect(lpRect); 
  return rect; 
}
CXRect CXRect::operator&(const RECT& rect2) const throw()
{
  CXRect rect; 
  ::IntersectRect(&rect, this, &rect2);
  return rect; 
}
CXRect CXRect::operator|(const RECT& rect2) const throw()
{
  CXRect rect; 
  ::UnionRect(&rect, this, &rect2);
  return rect; 
}
BOOL CXRect::SubtractRect(LPCRECT lpRectSrc1, LPCRECT lpRectSrc2) throw()
{ 
  return ::SubtractRect(this, lpRectSrc1, lpRectSrc2); 
}

void CXRect::NormalizeRect() throw()
{  
  if (left > right)
  {
    _swap(left, right);
  }
  if (top > bottom)
  {
    _swap(right, bottom);
  }
}

void CXRect::InflateRect(LPCRECT lpRect) throw()
{
  left -= lpRect->left;		
  top -= lpRect->top;
  right += lpRect->right;	
  bottom += lpRect->bottom;
}

void CXRect::InflateRect(const LONG& l, const LONG& t, const LONG& r, const LONG& b) throw()
{
  left -= l;			top -= t;
  right += r;			bottom += b;
}

void CXRect::DeflateRect(LPCRECT lpRect) throw()
{
  left += lpRect->left;	top += lpRect->top;
  right -= lpRect->right;	bottom -= lpRect->bottom;
}

void CXRect::DeflateRect(const LONG& l, const LONG& t, const LONG& r, const LONG& b) throw()
{
  left += l;		top += t;
  right -= r;		bottom -= b;
}

CXRect CXRect::MulDiv(const LONG& nMultiplier, const LONG& nDivisor) const throw()
{
  return CXRect(
    ::MulDiv(left, nMultiplier, nDivisor),
    ::MulDiv(top, nMultiplier, nDivisor),
    ::MulDiv(right, nMultiplier, nDivisor),
    ::MulDiv(bottom, nMultiplier, nDivisor));
}


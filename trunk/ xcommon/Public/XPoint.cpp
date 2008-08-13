/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XPoint.cpp					
文件内容:	    代替MFC中CXPoint
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/06/20
*********************************************************************/
#include "stdafx.h"
#include "XPoint.h"
#include "XRect.h"
// CXPoint
CXPoint::CXPoint() throw()
{ 
  x = 0; 
  y = 0; 
}
CXPoint::CXPoint(const LONG& initX, const LONG& initY) throw()
{ 
  x = initX;
  y = initY; 
}
CXPoint::CXPoint(const POINT& initPt) throw()
{ 
  *(POINT*)this = initPt; 
}
CXPoint::CXPoint(const SIZE& initSize) throw()
{ 
  *(SIZE*)this = initSize; 
}
CXPoint::CXPoint(LPARAM dwPoint) throw()
{
  x = (short)GET_X_LPARAM(dwPoint);
  y = (short)GET_Y_LPARAM(dwPoint);
}
void CXPoint::Offset(const LONG& xOffset, const LONG& yOffset) throw()
{ 
  x += xOffset; 
  y += yOffset; 
}
void CXPoint::Offset(const POINT& point) throw()
{
  x += point.x; 
  y += point.y; 
}
void CXPoint::Offset(const SIZE& size) throw()
{ 
  x += size.cx; 
  y += size.cy; 
}
void CXPoint::SetPoint(const LONG& X, const LONG& Y) throw()
{
  x = X; 
  y = Y; 
}
BOOL CXPoint::operator==(const POINT& point) const throw()
{ 
  return (x == point.x && y == point.y); 
}
BOOL CXPoint::operator!=(const POINT& point) const throw()
{ 
  return (x != point.x || y != point.y); 
}
void CXPoint::operator+=(const SIZE& size) throw()
{ 
  x += size.cx; 
  y += size.cy; 
}
void CXPoint::operator-=(const SIZE& size) throw()
{ 
  x -= size.cx; 
  y -= size.cy; 
}
void CXPoint::operator+=(const POINT& point) throw()
{ 
  x += point.x; 
  y += point.y; 
}
void CXPoint::operator-=(const POINT& point) throw()
{ 
  x -= point.x; 
  y -= point.y; 
}
CXPoint CXPoint::operator+(const SIZE& size) const throw()
{ 
  return CXPoint(x + size.cx, y + size.cy); 
}
CXPoint CXPoint::operator-(const SIZE& size) const throw()
{ 
  return CXPoint(x - size.cx, y - size.cy); 
}
CXPoint CXPoint::operator-() const throw()
{
  return CXPoint(-x, -y); 
}
CXPoint CXPoint::operator+(const POINT& point) const throw()
{ 
  return CXPoint(x + point.x, y + point.y); 
}
CXSize CXPoint::operator-(const POINT& point) const throw()
{ 
  return CSize(x - point.x, y - point.y); 
}
CXRect CXPoint::operator+(const RECT* lpRect) const throw()
{ 
  return CXRect(lpRect) + *this; 
}
CXRect CXPoint::operator-(const RECT* lpRect) const throw()
{
  return CXRect(lpRect) - *this; 
}

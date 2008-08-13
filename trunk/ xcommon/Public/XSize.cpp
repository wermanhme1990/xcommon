/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XSize.cpp					
文件内容:	    代替MFC中CXSize
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/06/20
*********************************************************************/
#include "stdafx.h"
#include "XSize.h"
#include "XRect.h"
// CXSize
CXSize::CXSize() throw()
{ 
  cx = 0;
  cy = 0;
}
CXSize::CXSize(const LONG& initCX, const LONG& initCY) throw()
{ 
  cx = initCX; cy = initCY; 
}
CXSize::CXSize(const SIZE& initSize) throw()
{ 
  *(SIZE*)this = initSize; 
}
CXSize::CXSize(const POINT& initPt) throw()
{ 
  *(POINT*)this = initPt; 
}
CXSize::CXSize(const DWORD& dwSize) throw()
{
  cx = (short)LOWORD(dwSize);
  cy = (short)HIWORD(dwSize);
}
BOOL CXSize::operator==(const SIZE& size) const throw()
{ 
  return (cx == size.cx && cy == size.cy); 
}
BOOL CXSize::operator!=(const SIZE& size) const throw()
{ 
  return (cx != size.cx || cy != size.cy);
}
void CXSize::operator+=(const SIZE& size) throw()
{ 
  cx += size.cx; 
  cy += size.cy; 
}
void CXSize::operator-=(const SIZE& size) throw()
{ 
  cx -= size.cx; 
  cy -= size.cy; 
}
void CXSize::SetSize(const LONG& CX, const LONG& CY) throw()
{
  cx = CX; 
  cy = CY; 
}	
CXSize CXSize::operator+(const SIZE& size) const throw()
{ 
  return CXSize(cx + size.cx, cy + size.cy); 
}
CXSize CXSize::operator-(const SIZE& size) const throw()
{ 
  return CXSize(cx - size.cx, cy - size.cy); 
}
CXSize CXSize::operator-() const throw()
{ 
  return CXSize(-cx, -cy); 
}
CXPoint CXSize::operator+(const POINT& point) const throw()
{
  return CPoint(cx + point.x, cy + point.y); 
}
CXPoint CXSize::operator-(const POINT& point) const throw()
{ 
  return CXPoint(cx - point.x, cy - point.y); 
}
CXRect CXSize::operator+(const RECT* lpRect) const throw()
{ 
  return CXRect(lpRect) + *this; 
}
CXRect CXSize::operator-(const RECT* lpRect) const throw()
{
  return CXRect(lpRect) - *this; 
}

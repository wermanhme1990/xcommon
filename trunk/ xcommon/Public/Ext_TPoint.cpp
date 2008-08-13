/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称:   Ext_TPoint.cpp          
文件内容:  
版本历史:  1.0
作者:    xuejuntao xuejuntao@hanwang.com.cn 2008/02/26
*********************************************************************/

#include "stdafx.h"
#include "Ext_TPoint.h"
#include <math.h>
#pragma warning(disable : 4244 )

const CTPoint g_atEndMark[2] = 
{
  CTPoint(HW_StrokeEndMark, 0),
  CTPoint(HW_StrokeEndMark, HW_StrokeEndMark)
};

const WORD  g_wUnicodeFileTag = 0xFEFF;


CTPoint::CTPoint()
{
  x = 0;
  y = 0;
}
CTPoint::CTPoint(long a, long b)
{
  x = a;
  y = b;
}
CTPoint::CTPoint(const POINT &tA)
{
  x = tA.x;
  y = tA.y;
}
CTPoint::CTPoint(const CPoint &tA)
{
  x = tA.x;
  y = tA.y;
}
CTPoint::CTPoint(const CTPoint &tA)
{
  x = tA.x; 
  y = tA.y;
}
CTPoint::~CTPoint()
{
  x = 0;
  y = 0;
}

BOOL CTPoint::operator==(const CTPoint& tB) const
{
  BOOL blRet = TRUE;
  if (x != tB.x || y != tB.y)
  {
    blRet = FALSE;
  }
  return blRet;
}
BOOL CTPoint::operator ==( const POINT &tB) const
{  
  return *this == CPoint(tB);
}
BOOL CTPoint::operator ==( const CPoint &tB) const
{
  BOOL blRet = TRUE;
  if (x != tB.x || y != tB.y)
  {
    blRet = FALSE;
  }
  return blRet;;
}
BOOL CTPoint::operator !=(const CTPoint &tB) const
{
  return !(*this == tB);
}
BOOL CTPoint::operator != (const POINT &tB) const
{
  return !(*this == tB);
}
BOOL CTPoint::operator != (const CPoint &tB) const
{
  return !(*this == tB);
}
BOOL operator==( const POINT& tA, const CTPoint& tB)
{
  return tB == tA;
}
BOOL operator==( const CPoint& tA, const CTPoint& tB)
{
  return tB == tA;
}
BOOL operator !=(const POINT &tA, const CTPoint &tB)
{
  return !(tB == tA);
}
BOOL operator != (const CPoint &tA, const CTPoint &tB)
{
  return !(tB == tA);
}
const CTPoint CTPoint::operator-() const
{
  return CTPoint(-x, -y);
}  
const CTPoint CTPoint::operator +(long nXOffset) const
{
  return CTPoint(this->x + nXOffset, this->y + nXOffset);
}
const CTPoint CTPoint::operator+(const CTPoint &tB) const
{
  return CTPoint(this->x + tB.x, this->y + tB.y);
}
const CTPoint CTPoint::operator+(const CSize &tSizeB) const
{
  return CTPoint(this->x + tSizeB.cx, this->y + tSizeB.cy);
}
const CTPoint& CTPoint::operator=(const POINT & tB)
{
  Reset(tB.x, tB.y);
  return *this;
}

const CTPoint& CTPoint::operator=(const CPoint & tB)
{
  Reset(tB.x, tB.y);
  return *this;
}
const CTPoint CTPoint::operator -(const CTPoint &tB) const
{
  return CTPoint(this->x - tB.x, this->y - tB.y);
}
const CTPoint& CTPoint::operator-=(const CTPoint &tPtA)
{
  this->x -= tPtA.x;
  this->y -= tPtA.y;
  return *this;
}
const CTPoint& CTPoint::operator+=(const CTPoint &tPtA)
{
  this->x += tPtA.x;
  this->y += tPtA.y;
  return *this;
}
CTPoint::operator const POINT()
{  
  CPoint tPoint(x, y);

  return POINT(tPoint);  
}
CTPoint::operator const CPoint()
{
  return CPoint(x, y);
}
CTPoint::operator const Point_T()
{
  return Point_T::createPoint(x, y);
}
#ifdef _GDIPLUS_H
  CTPoint::operator const Gdiplus::Point() const
  {
    return Gdiplus::Point(x, y);
  }
#endif
BOOL CTPoint::Reset(long x, long y)
{
  this->x = x;
  this->y = y;
  return TRUE;
}
BOOL CTPoint::Offset(long x, long y)
{
  this->x += x, this->y += y;
  return TRUE;
}
const double CTPoint::GetDistance( const CTPoint & tPtA) const
{
  double dDist = 0.0;
  CTPoint tPtOffset = *this - tPtA;
  dDist = sqrt(double(tPtOffset.x * tPtOffset.x + tPtOffset.y * tPtOffset.y));
  return dDist;
}

const double CTPoint::operator*(const CTPoint & tPtA) const
{
  return double(x * tPtA.x + y * tPtA.y);
}
/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称:   Ext_TPoint.h          
文件内容:  自定义点的实现 沿袭核心编码部分TPoint的定义
版本历史:  1.0
作者:    xuejuntao xuejuntao@hanwang.com.cn 2008/02/26
*********************************************************************/

#ifndef  _HWXUE_Ext_TPoint_INC
#define _HWXUE_Ext_TPoint_INC
#include "Ext_Type.h"
#include "..//..//core//tree//tree.hpp"
using namespace hwnotes::core::tree;
#include <Gdiplus.h>

class CTPoint : public TPoint
{
public:
  CTPoint();
  CTPoint(long, long);
  CTPoint(const POINT&);
  CTPoint(const CPoint &);
  CTPoint(const CTPoint &);
  ~CTPoint();
  
  BOOL operator==(const CTPoint &) const;
  BOOL operator==(const POINT &) const;
  BOOL operator==(const CPoint &) const;
  BOOL operator!=(const CTPoint &) const;
  BOOL operator!=(const POINT &) const;
  BOOL operator!=(const CPoint &) const;  
  friend BOOL operator==(const POINT& tA, const CTPoint& tB);  
  friend BOOL operator==(const CPoint& tA, const CTPoint& tB);
  friend BOOL operator!=(const POINT& tA, const CTPoint& tB);  
  friend BOOL operator!=(const CPoint& tA, const CTPoint& tB);

  const CTPoint operator-()  const;
  const CTPoint operator+(long) const;
  const CTPoint operator+(const CTPoint &) const;
  const CTPoint operator+(const CSize &) const;
  const CTPoint &operator=(const POINT&);
  const CTPoint &operator=(const CPoint&);
  


  const CTPoint operator-(const CTPoint &) const;
  const CTPoint &operator -=(const CTPoint &);
  const CTPoint &operator +=(const CTPoint &);
  //类型转换 重载
  operator const POINT();
  operator const CPoint();
  operator const Point_T();
#ifdef _GDIPLUS_H
  operator const Gdiplus::Point() const;
#endif
  
  const double GetDistance(const CTPoint &) const;
  //集成了一些vector运算
  const double operator*(const CTPoint &) const;  
  
private:  
  BOOL Reset(long, long);
  BOOL Offset(long, long);

public:
//   short x;
//   short y;

};
typedef CTPoint* LPCTPoint;
typedef const CTPoint* LPCCTPoint;
extern const CTPoint g_atEndMark[2];
#endif // !defined(_HWXUE_Ext_TPoint_INC)

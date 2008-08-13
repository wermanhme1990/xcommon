/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	GlobalEx.h					
文件内容:	    一些全局函数
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/03/22
*********************************************************************/
#pragma once
#include "Ext_Type.h"

enum HWOSType
{
  OS_Unknown = 0, 
  OS_NT = 1,
  OS_WindowsNT4SP3OrLater = 2,
  OS_XP = 3, 
  OS_Vista = 4,
  OS_64 = 5
};
#ifdef _UNICODE
#define StrChrChr     XStrChrChrW
#define TCharDup      XStrDupW
#define WCharToTChar  XStrDupW
#define TCharToWChar  XStrDupW
#define TCharToChar   WCharToChar

#else

#define StrChrChr     XStrChrChrA
#define TCharDup      XStrDupA
#define WCharToTChar  WCharToChar
#define TCharToWChar  CharToWChar
#define TCharToChar   XStrDupA

#endif


#ifdef __cplusplus
extern "C"{
#endif

  DLLXEXPORT void WINAPI XStrChrChrA(LPSTR, const CHAR&, const CHAR&);
  DLLXEXPORT void  WINAPI XStrChrChrW(LPWSTR, const WCHAR&, const WCHAR&);

  DLLXEXPORT LPSTR  WINAPI WCharToChar(LPCWSTR);
  DLLXEXPORT LPWSTR WINAPI CharToWChar(LPCSTR);  

  DLLXEXPORT LPWSTR WINAPI XStrDupW(LPCWSTR);
  DLLXEXPORT LPSTR  WINAPI XStrDupA(LPCSTR);

  DLLXEXPORT HWOSType WINAPI XGetOSType();

  DLLXEXPORT float WINAPI XPixelsToTwips(const LONG& nDPI, const float& fPixels);
  DLLXEXPORT float WINAPI XTwipsToPixel(const LONG& nDPI, const float& nTwips);
  DLLXEXPORT float WINAPI XPixelToMillimeter(const LONG& nDPI, const float &fPiexls );
  DLLXEXPORT float WINAPI XPixelToInch(const LONG& nDPI, const float &fPiexls );
  DLLXEXPORT float WINAPI XMillimeterToPixel( const LONG& nDPI, const float &nMm );
  DLLXEXPORT float WINAPI XInchToPixel( const LONG& nDPI, const float &nInch );


#ifdef __cplusplus
}
#endif

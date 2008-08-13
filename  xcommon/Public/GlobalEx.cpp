
/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	GlobalEx.h					
文件内容:	    一些全局函数
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/03/22
*********************************************************************/

#include "stdafx.h"
#include "GlobalEx.h"
#include "Ext_Memory.h"
#include <mbstring.h>
const LONG cnInchToPoint = 72; //1 inch = 72 points
const LONG cnInchToTwip = 1440;// 1 inch = 1440 twips
const LONG cnInchToEMU = 914400; // 1 inch = 914400 EMUs

void WINAPI XStrChrChrA(LPSTR strString, const CHAR& A, const CHAR& B)
{
  if (!strString)
  {
    return;
  }	
  while (*strString)
  {
    if (A == *strString)
    {
      *strString = B;
    }    		
		strString ++;
  }
}
void WINAPI XStrChrChrW(LPWSTR strString, const WCHAR& A, const WCHAR& B)
{
  if (!strString)
  {
    return;
  }
  while (*strString)
  {
    if (A == *strString)
    {
      *strString = B;
    }		
    strString ++;		
  }
}

LPSTR WINAPI WCharToChar(LPCWSTR pwszStr)
{
  CHAR *pchString = NULL;
  if (pwszStr)
  {
    LONG nLen = (LONG)wcslen(pwszStr);
    if (nLen > 0)
    {
      LONG nBytes = WideCharToMultiByte(CP_ACP, 0, pwszStr, nLen, NULL, NULL, NULL, NULL); 
      assert(nBytes > 0);
      VERIFY(Mem_NewMemory((void **)&pchString, nBytes + 1));      
      WideCharToMultiByte(CP_ACP, 0, pwszStr, nLen,  pchString, nBytes, 0 , 0);
      pchString[nBytes] = 0;  
    }
  }  
  return pchString;
}

LPWSTR WINAPI CharToWChar(LPCSTR pszStr)
{ 
  WCHAR *pwhString = NULL;
  if (pszStr)
  {
    LONG nLen = (LONG)strlen(pszStr);
    if (nLen > 0)
    {
      LONG nBytes = MultiByteToWideChar(CP_ACP, 0, pszStr, nLen, NULL, NULL); 
      assert(nBytes > 0);
      VERIFY(Mem_NewMemory((void **)&pwhString, (nBytes + 1) * sizeof(WCHAR)));     
      MultiByteToWideChar(CP_ACP, 0, pszStr,  nLen,  pwhString, nLen);
      pwhString[nBytes] = 0;       
    }
  }    
  return pwhString;
}

LPWSTR WINAPI XStrDupW(LPCWSTR pwStr)
{
  WCHAR *pwhString = NULL;
  if (pwStr)
  {
    LONG nLen = (LONG)wcslen(pwStr);
    if (nLen > 0)
    {
      VERIFY(Mem_NewMemory((void **)&pwhString, (nLen + 1) * sizeof(WCHAR))); 
      wcscpy_s(pwhString, nLen, pwStr);
      pwhString[nLen] = 0;            
    }
  }    
  return pwhString;
}
LPSTR WINAPI XStrDupA( LPCSTR pszStr)
{
  CHAR *pchString = NULL;
  if (pszStr)
  {
    LONG nLen = (LONG)strlen(pszStr);
    if (nLen > 0)
    {
      VERIFY(Mem_NewMemory((void **)&pchString, nLen + 1));
      strcpy_s(pchString, nLen, pszStr);      
      pchString[nLen] = 0; 
    }
  }  
  return pchString;
}


HWOSType WINAPI HWGetOSType()
{
  HWOSType tRet = OS_Unknown;
  OSVERSIONINFO osVer;
  Mem_ZeroMemory(&osVer, sizeof(osVer));
  osVer.dwOSVersionInfoSize = sizeof(osVer);
  GetVersionEx (&osVer);	
  if (osVer.dwMajorVersion < 6)
  {//非vista
    tRet = OS_XP;	
  }
  else
  {//vista
    tRet = OS_Vista;	
  }	
  return tRet;
}

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
BOOL WINAPI Is64BitProcess()
{  
  BOOL	bIsWow64 = FALSE;
  HINSTANCE	hDll = NULL; 
  __try
  {	  
    hDll = LoadLibrary(TEXT("KERNEL32.DLL"));
    if( hDll )
    {
      LPFN_ISWOW64PROCESS  fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress( hDll, "IsWow64Process");

      if(fnIsWow64Process)
        fnIsWow64Process( GetCurrentProcess(), &bIsWow64 );		  
    }
  }
  __finally
  {
    if (hDll)
    {
      FreeLibrary(hDll);
    }
  }  
  return bIsWow64;
}
HWOSType WINAPI XGetOSType()
{
  HWOSType tRet = OS_Unknown;

  OSVERSIONINFO	tOSV;
  int				    iServicePack;
  TCHAR         *pthPointer;   
  Mem_ZeroMemory(&tOSV, sizeof( tOSV ) );
  tOSV.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&tOSV);
  for (pthPointer = tOSV.szCSDVersion; *pthPointer  && !isdigit(*pthPointer); pthPointer ++);
  iServicePack = _ttoi(pthPointer); 
  if (tOSV.dwPlatformId == VER_PLATFORM_WIN32_NT)
  {     
    switch(tOSV.dwMajorVersion)
    {
    case 4:
      {
        if (iServicePack >= 3)
        {
          tRet = OS_WindowsNT4SP3OrLater;
        }
        else
        {
          tRet = OS_NT;
        }
        break;
      }
    case 5:
      {
        tRet = OS_XP;
        break;
      }
    case 6:
      {
        tRet = OS_Vista;
        break;
      }    
    }    
    if(Is64BitProcess())
    {
      tRet = OS_64;
    }
  }
  return tRet;
}

float WINAPI XPixelsToTwips(const LONG& nDPI, const float& fPixels)
{
  //修正不正确的分辨率
  LONG nInnerDPI = nDPI;
  if (nInnerDPI < 70)
  {
    nInnerDPI = 300;
  }
  return ((float)(fPixels * cnInchToTwip)/(float)nInnerDPI);
}

float WINAPI XTwipsToPixel(const LONG& nDPI, const float& nTwips)
{
  //修正不正确的分辨率
  LONG nInnerDPI = nDPI;
  if (nInnerDPI < 70)
  {
    nInnerDPI = 300;
  }
  return ((float)(nTwips * nInnerDPI)/(float)cnInchToTwip);
}
float WINAPI XPixelToMillimeter( const LONG& nDPI, const float &fPiexls )
{
  float nInch = XPixelToInch(nDPI, fPiexls);
  return nInch * 25.4;
}
float WINAPI XMillimeterToPixel( const LONG& nDPI, const float &nMm )
{
  float nInch = nMm / 25.4;
  return XInchToPixel(nDPI, nInch);  
}
float WINAPI XPixelToInch( const LONG& nDPI, const float &fPiexls )
{
  LONG nInnerDPI = nDPI;
  if (nInnerDPI < 70)
  {
    nInnerDPI = 300;
  }
  return (float)fPiexls / nDPI ;
}
float WINAPI XInchToPixel( const LONG& nDPI, const float &nInch )
{
  LONG nInnerDPI = nDPI;
  if (nInnerDPI < 70)
  {
    nInnerDPI = 300;
  }
  return nInch * nDPI;
}


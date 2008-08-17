//  Direct Hardware Access Under Windows 9x/NT/2000/XP  //

#include <windows.h>
#include <winioctl.h>
#include <tchar.h>
#include "k32call.h"
#include "phys32.h"
#include "..\drv\nt\winio_nt.h"
#include "HWIO.h"

HANDLE  g_hDriver = INVALID_HANDLE_VALUE;
BOOL    g_blIsNT;
BOOL    g_blIsInitialized = FALSE;
TCHAR   g_athIoDriverPath[MAX_PATH];
LPCTSTR g_pthServiceName = TEXT("HanvonInputXue");



BOOL IsWinNT()
{
  OSVERSIONINFO OSVersionInfo;
  OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&OSVersionInfo);
  return OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT;
}


BOOL GetDriverPath()
{
  LPTSTR pszSlash;
  if (!GetModuleFileName(GetModuleHandle(NULL), g_athIoDriverPath, 
    sizeof(g_athIoDriverPath) / sizeof(*g_athIoDriverPath)))
  {
    return FALSE;
  }

  pszSlash = _tcsrchr(g_athIoDriverPath, TEXT('\\'));

  if (pszSlash)
    pszSlash[1] = 0;
  else
    return FALSE;

  _tcscat(g_athIoDriverPath, TEXT("winio.sys"));

  return TRUE;
}


BOOL WINAPI Initialize()
{
  BOOL blResult;
  DWORD dwBytesReturned;

  g_blIsNT = IsWinNT();

  if (g_blIsNT)
  {
    g_hDriver = CreateFile(TEXT("\\\\.\\WINIO"),
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         NULL,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         NULL);
    // If the driver is not running, install it
    if (INVALID_HANDLE_VALUE == g_hDriver)
    {
      GetDriverPath();

      blResult = InstallDriver(g_athIoDriverPath, TRUE);

      if (!blResult)
        return FALSE;

      blResult = StartDriver();

      if (!blResult)
        return FALSE;

      g_hDriver = CreateFile(TEXT("\\\\.\\WINIO"),
                           GENERIC_READ | GENERIC_WRITE,
                           0,
                           NULL,
                           OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL,
                           NULL);

      if (g_hDriver == INVALID_HANDLE_VALUE)
      {
        return FALSE;
      }
    }

    // Enable I/O port access for this process
    if (!DeviceIoControl(g_hDriver, IOCTL_ENABLEDIRECTIO, NULL,
                         0, NULL, 0, &dwBytesReturned, NULL))
    {
      return FALSE;
    }
  }
  else
  {
    VxDCall = (DWORD (WINAPI *)(DWORD,DWORD,DWORD))GetK32ProcAddress(1);
    g_hDriver = CreateFile(TEXT("\\\\.\\WINIO.VXD"), 0, 0, 0,
      CREATE_NEW, FILE_FLAG_DELETE_ON_CLOSE, 0);

    if (INVALID_HANDLE_VALUE == g_hDriver)
      return FALSE;
  }

  g_blIsInitialized = TRUE;

  return TRUE;
}


void WINAPI Shutdown()
{
  DWORD dwBytesReturned;

  if (g_blIsNT)
  {
    if (g_hDriver != INVALID_HANDLE_VALUE)
    {      
      DeviceIoControl(g_hDriver, IOCTL_DISABLEDIRECTIO, NULL,
                      0, NULL, 0, &dwBytesReturned, NULL);
      CloseHandle(g_hDriver);
    }
    RemoveDriver();
  }
  else
    CloseHandle(g_hDriver);

  g_blIsInitialized = FALSE;
}

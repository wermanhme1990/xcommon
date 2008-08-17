#ifndef WINIO_H
#define WINIO_H

#define DLL_EXPORT

#ifdef DLL_EXPORT
#define DLL_EXPORT _declspec(dllexport) 
#else
#define DLL_EXPORT _declspec(dllimport) 
#endif

extern "C"
{  
  DLL_EXPORT BOOL   Initialize();
  DLL_EXPORT void   Shutdown();
  DLL_EXPORT PBYTE  MapPhysToLin(PBYTE pbPhysAddr, DWORD dwPhysSize, HANDLE *pPhysicalMemoryHandle);
  DLL_EXPORT BOOL   UnmapPhysicalMemory(HANDLE PhysicalMemoryHandle, PBYTE pbLinAddr);
  DLL_EXPORT BOOL   GetPhysLong(PBYTE pbPhysAddr, PDWORD pdwPhysVal);
  DLL_EXPORT BOOL   SetPhysLong(PBYTE pbPhysAddr, DWORD dwPhysVal);
  DLL_EXPORT BOOL   GetPortVal(WORD wPortAddr, PDWORD pdwPortVal, BYTE bSize);
  DLL_EXPORT BOOL   SetPortVal(WORD wPortAddr, DWORD dwPortVal, BYTE bSize);
  DLL_EXPORT BOOL   InstallDriver(LPCTSTR pszWinIoDriverPath, BOOL IsDemandLoaded = FALSE);
  DLL_EXPORT BOOL   RemoveDriver();
}

extern BOOL g_blIsNT;
extern HANDLE g_hDriver;
extern BOOL g_blIsInitialized;


extern LPCTSTR g_pthServiceName;
BOOL _stdcall StartDriver();
BOOL _stdcall StopDriver();

#endif

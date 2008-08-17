#ifndef HWIO_H
#define HWIO_H

//Export
BOOL   WINAPI Initialize();
void   WINAPI Shutdown();
PBYTE  WINAPI MapPhysToLin(PBYTE pbPhysAddr, DWORD dwPhysSize, HANDLE *pPhysicalMemoryHandle);
BOOL   WINAPI UnmapPhysicalMemory(HANDLE PhysicalMemoryHandle, PBYTE pbLinAddr);
BOOL   WINAPI GetPhysLong(PBYTE pbPhysAddr, PDWORD pdwPhysVal);
BOOL   WINAPI SetPhysLong(PBYTE pbPhysAddr, DWORD dwPhysVal);
BOOL   WINAPI GetPortVal(WORD wPortAddr, PDWORD pdwPortVal, BYTE bSize);
BOOL   WINAPI SetPortVal(WORD wPortAddr, DWORD dwPortVal, BYTE bSize);
BOOL   WINAPI InstallDriver(LPCTSTR pszWinIoDriverPath, BOOL IsDemandLoaded = FALSE);
BOOL   WINAPI RemoveDriver();


//Local
extern BOOL g_blIsNT;
extern HANDLE g_hDriver;
extern BOOL g_blIsInitialized;


extern LPCTSTR g_pthServiceName;

BOOL StartDriver();
BOOL StopDriver();

#endif//HWIO_H

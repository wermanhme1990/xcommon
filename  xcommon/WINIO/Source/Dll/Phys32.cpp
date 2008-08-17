//  Direct Hardware Access Under Windows 9x/NT/2000/XP  //

#include <windows.h>
#include <winioctl.h>
#include "phys32.h"
#include "..\drv\nt\winio_nt.h"
#include "winio.h"

// VWIN32 DPMI services

#define VWIN32_DPMI_CALL 0x2A0029

// KERNEL32's undocumented VxDCall function is exported as ordinal #1

DWORD (WINAPI *VxDCall)(DWORD Service, DWORD EAX_Reg, DWORD ECX_Reg);

#define DPMICall(EAX_Reg, ECX_Reg) VxDCall(VWIN32_DPMI_CALL, EAX_Reg, ECX_Reg)


PBYTE MapPhysToLin(PBYTE pbPhysAddr, DWORD dwPhysSize, HANDLE *pPhysicalMemoryHandle)
{
  PBYTE pbLinAddr;
  TPhys32Struct tPhys32Struct;
  DWORD dwBytesReturned;

  if (!g_blIsInitialized)
    return NULL;

  if (g_blIsNT)
  {
    tPhys32Struct.dwPhysMemSizeInBytes = dwPhysSize;
    tPhys32Struct.pvPhysAddress = pbPhysAddr;

    if (!DeviceIoControl(g_hDriver, IOCTL_MAPPHYSTOLIN, &tPhys32Struct,
                         sizeof(tPhys32Struct), &tPhys32Struct, sizeof(tPhys32Struct),
                         &dwBytesReturned, NULL))
      return NULL;
    else
    {
      pbLinAddr = (PBYTE)((DWORD)tPhys32Struct.pvPhysMemLin + (DWORD)pbPhysAddr - (DWORD)tPhys32Struct.pvPhysAddress);
      *pPhysicalMemoryHandle = tPhys32Struct.PhysicalMemoryHandle;
    }
  }
  else
  {
    _asm
    {
      Mov BX, WORD PTR [pbPhysAddr + 2]
      Mov CX, WORD PTR [pbPhysAddr]
      Mov SI, WORD PTR [dwPhysSize + 2]
      Mov DI, WORD PTR [dwPhysSize]
    }

    // Call DPMI function MapPhysicalToLinear

    DPMICall(0x800, (DWORD)pbPhysAddr);    

    _asm
    {
      Jnc Success
      Xor BX, BX
      Xor CX, CX

    Success:

      Mov WORD PTR [pbLinAddr + 2], BX
      Mov WORD PTR [pbLinAddr], CX
    }
  }

  return pbLinAddr;
}


BOOL UnmapPhysicalMemory(HANDLE PhysicalMemoryHandle, PBYTE pbLinAddr)
{
  TPhys32Struct tPhys32Struct;
  DWORD dwBytesReturned;

  if (g_blIsNT)
  {
    if (!g_blIsInitialized)
      return FALSE;

    tPhys32Struct.PhysicalMemoryHandle = PhysicalMemoryHandle;
    tPhys32Struct.pvPhysMemLin = pbLinAddr;

    if (!DeviceIoControl(g_hDriver, IOCTL_UNMAPPHYSADDR, &tPhys32Struct,
                         sizeof(tPhys32Struct), NULL, 0, &dwBytesReturned, NULL))
      return FALSE;
  }

  return TRUE;
}

// Support functions for Visual-Basic users
BOOL GetPhysLong(PBYTE pbPhysAddr, PDWORD pdwPhysVal)
{
  PDWORD pdwLinAddr;
  HANDLE hPhysicalMemory;

  if (!g_blIsInitialized)
    return FALSE;

  pdwLinAddr = (PDWORD)MapPhysToLin(pbPhysAddr, 4, &hPhysicalMemory);

  if (pdwLinAddr == NULL)
    return FALSE;

  *pdwPhysVal = *pdwLinAddr;

  UnmapPhysicalMemory(hPhysicalMemory, (PBYTE)pdwLinAddr);

  return TRUE;
}


BOOL SetPhysLong(PBYTE pbPhysAddr, DWORD dwPhysVal)
{
  PDWORD pdwLinAddr;
  HANDLE PhysicalMemoryHandle;

  if (!g_blIsInitialized)
    return FALSE;

  pdwLinAddr = (PDWORD)MapPhysToLin(pbPhysAddr, 4, &PhysicalMemoryHandle);

  if (pdwLinAddr == NULL)
    return FALSE;

  *pdwLinAddr = dwPhysVal;

  UnmapPhysicalMemory(PhysicalMemoryHandle, (PBYTE)pdwLinAddr);

  return TRUE;
}

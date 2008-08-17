//  Direct Hardware Access Under Windows 9x/NT/2000/XP  //


#include <windows.h>
#include "winio.h"

BOOL InstallDriver(LPCTSTR pthDriverPath, BOOL IsDemandLoaded)
{
  SC_HANDLE hSCManager;
  SC_HANDLE hService;
  // Remove any previous instance of the driver
  RemoveDriver();
  hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

  if (hSCManager)
  {       
    hService = CreateService(hSCManager,
                             g_pthServiceName,
                             g_pthServiceName,
                             SERVICE_ALL_ACCESS,
                             SERVICE_KERNEL_DRIVER,
                             (FALSE != IsDemandLoaded) ? SERVICE_DEMAND_START : SERVICE_SYSTEM_START,
                             SERVICE_ERROR_NORMAL,
                             pthDriverPath,
                             NULL,
                             NULL,
                             NULL,
                             NULL,
                             NULL);

    CloseServiceHandle(hSCManager);

    if (NULL == hService)
      return FALSE;
  }
  else
    return FALSE;
  CloseServiceHandle(hService);  
  return TRUE;
}


BOOL RemoveDriver()
{
  SC_HANDLE hSCManager;
  SC_HANDLE hService;
  BOOL blResult;

  StopDriver();

  hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

  if (hSCManager)
  {
    hService = OpenService(hSCManager, g_pthServiceName, SERVICE_ALL_ACCESS);

    CloseServiceHandle(hSCManager);

    if (hService)
    {
      blResult = DeleteService(hService);
      
      CloseServiceHandle(hService);
    }
    else
      return FALSE;
  }
  else
    return FALSE;

  return blResult;
}


BOOL _stdcall StartDriver()
{
  SC_HANDLE hSCManager;
  SC_HANDLE hService;
  BOOL blResult;

  hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

  if (hSCManager)
  {
    hService = OpenService(hSCManager, g_pthServiceName, SERVICE_ALL_ACCESS);

    CloseServiceHandle(hSCManager);

    if (hService)
    {
      blResult = StartService(hService, 0, NULL) || GetLastError() == ERROR_SERVICE_ALREADY_RUNNING;

      CloseServiceHandle(hService);
    }
    else
      return FALSE;
  }
  else
    return FALSE;

  return blResult;
}


BOOL _stdcall StopDriver()
{
  SC_HANDLE hSCManager;
  SC_HANDLE hService;
  SERVICE_STATUS ServiceStatus;
  BOOL blResult;

  hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

  if (hSCManager)
  {
    hService = OpenService(hSCManager, g_pthServiceName, SERVICE_ALL_ACCESS);

    CloseServiceHandle(hSCManager);

    if (hService)
    {
      blResult = ControlService(hService, SERVICE_CONTROL_STOP, &ServiceStatus);

      CloseServiceHandle(hService);
    }
    else
      return FALSE;
  }
  else
    return FALSE;

  return blResult;
}



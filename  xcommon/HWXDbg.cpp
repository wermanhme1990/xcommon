#include "..\\HWXDbg\\HWXDbg.h"
#include <assert.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <stdarg.h>
#include <crtdbg.h>
#include <time.h>

TCHAR CHWXLog::m_athMutexName[NAME_MAX] = _T("HWX_DBG_MUTEX");
HANDLE CHWXLog::m_MutexLog = NULL;
TCHAR CHWXLog::m_athLogFile[PATH_MAX] = _T("D:\\MyLog.txt");

void CHWXLog::HWLog(const TCHAR *pthFormat, ...)
{      
  const long nLimit = 1024;
  TCHAR achBuffer[1024] = {0};
  long nBufferLen = 0;
  TCHAR *pthBgn = achBuffer;  
  CHWXLog &tLog = GetInstance();
  DWORD dwWaitRet = tLog.WaitForMutex();
  if(dwWaitRet)
  {    
    va_list tArgs;   
    va_start( tArgs, pthFormat);    
    nBufferLen = _vsctprintf(pthFormat, tArgs) + 20;
    if (nBufferLen > nLimit)
    {
      VERIFY(pthBgn = (TCHAR *)_alloca(nBufferLen * sizeof(TCHAR)));
    }
    _tstrtime(pthBgn);
    long nTimeLen = _tcslen(pthBgn);
    pthBgn[nTimeLen ++] = TEXT('\t');
    //pthBgn[nTimeLen ++] = 0;       
    _vstprintf(pthBgn + nTimeLen, pthFormat, tArgs); 
    tLog.HWCout(pthBgn);       
    va_end(tArgs);
    if (!tLog.ReleaseMutex())
    {           
      tLog.HWCout(TEXT("ReleaseMutex Error\n"));
    }
  }
  else
  {  
    tLog.HWCout(TEXT("Wait Error: WAIT_FAILED\n"));
  }
}
void CHWXLog::PrintProcessName()
{ 
  TCHAR athName[MAX_PATH];
  DWORD dwPID = GetCurrentProcessId();
  DWORD dwTID = GetCurrentThreadId();
  GetModuleFileName(NULL, athName, MAX_PATH);
  HWLog(_T("Process:%s  PID=%d TID=%d\n"), athName, (long)dwPID, (long)dwTID);
}
void CHWXLog::PrintCurError()
{
  DWORD dwErr = GetLastError();
  LPVOID lpMsgBuf;
  DWORD dwPID, dwTID;
  dwPID = GetCurrentProcessId();
  dwTID = GetCurrentThreadId();
  if (!FormatMessage( 
    FORMAT_MESSAGE_ALLOCATE_BUFFER | 
    FORMAT_MESSAGE_FROM_SYSTEM | 
    FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    GetLastError(),
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
    (LPTSTR) &lpMsgBuf,
    0,
    NULL ))
  {      
    return;
  }    
  HWLog(_T("PID=%d TID=%d CurError:ID= %d  %s\n"), (long)dwPID, (long)dwTID, (long)dwErr, lpMsgBuf);
  // Free the buffer.
  LocalFree( lpMsgBuf );
  return ;
}
CHWXLog::CHWXLog()
{ //创建Log互斥变量
  if (!(m_MutexLog = OpenMutex(MUTEX_ALL_ACCESS, TRUE, m_athMutexName)))
  {    
    VERIFY(m_MutexLog = CreateMutex(NULL, FALSE, m_athMutexName));
  } 
  _ASSERT(m_MutexLog);  
  //_tremove(m_athLogFile);
}
CHWXLog::~CHWXLog()
{
  if (m_MutexLog)
  {
    CloseHandle(m_MutexLog);
    m_MutexLog = NULL;
  }  
}
CHWXLog& CHWXLog:: GetInstance()
{
  static CHWXLog tLog;
  return tLog;
}
void CHWXLog::HWCout(const TCHAR *ptString)
{  
  //创建文件
  FILE *m_pFile= NULL;
  m_pFile = _tfopen(m_athLogFile, TEXT("a"));
  if (m_pFile)
  {
    fwrite(ptString, sizeof(TCHAR), _tcslen(ptString), m_pFile);
    fclose(m_pFile); 
    m_pFile = NULL;
  }     
}

BOOL CHWXLog::WaitForMutex()
{  
  return WaitForSingleObject(m_MutexLog, INFINITE) == WAIT_OBJECT_0;
}
BOOL CHWXLog::ReleaseMutex()
{
  return ::ReleaseMutex(m_MutexLog);
}

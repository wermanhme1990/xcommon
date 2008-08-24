#ifndef __INC_HWXDFB_
#define __INC_HWXDFB_
#include <windows.h>
#include <tchar.h>

#ifndef VERIFY
  #ifdef _DEBUG
    #define VERIFY(f)          _ASSERT(f)//{ if(FALSE == (f)) { HWLog(_T("XUE\t%s\n"), __LINE__); }}//
  #else
     #define VERIFY(f)          ((void)(f))
  #endif    
#endif

#ifndef PATH_MAX
  #define PATH_MAX    (512)
#endif
#ifndef NAME_MAX
  #define NAME_MAX    (256)
#endif

class CHWXLog
{
public:  
  static void HWLog(const TCHAR *pchFormat, ...);
  static void PrintProcessName();
  static void PrintCurError();
public:
  CHWXLog();  
  virtual ~CHWXLog();  
private:
  static CHWXLog& GetInstance();
  void HWCout(const TCHAR *ptString);
  BOOL WaitForMutex();
  BOOL ReleaseMutex();
private:
  static TCHAR m_athMutexName[NAME_MAX];
  static HANDLE m_MutexLog;
  static TCHAR m_athLogFile[PATH_MAX]; 
  BOOL GenTimeString(TCHAR *pthTime);
};


#endif //!__INC_HWXDFB_
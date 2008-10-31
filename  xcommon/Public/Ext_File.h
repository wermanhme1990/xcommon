#ifndef _DBG_FILE_H_INC_
#define _DBG_FILE_H_INC_
#include "Ext_Type.h"
#include "HWXPackage.h"
#include "HWXString.h"

#include <vector>



#ifdef _UNICODE
#define HWXIsFileInUse    XIsFileInUseW
#define HWXCreateDir      XCreateDirW
#define HWXGenCurPath     XGenCurPathW
#define HWXCreateFile     XCreateFileW
#define HWXIsDirectoryExist        XIsDirectoryExistW
#define HWXIsFileExist    XIsFileExistW
#define HWXRemoveFile     XRemoveFileW
#define HWXDeleteDirectory  XDeleteDirectoryW
#define HWXGetFilePath    XGetFileDirectoryW
#define HWXGetFileExt     XGetFileExtW
#define HWXGetFileTitle   XGetFileTitleW
#define HWXGetFileDirAndName  XGetFileDirAndNameW
#define HWXGetFileDirectory  XGetFileDirectoryW
#define HWXGetFileName    XGetFileNameW   
#define HWXGetModuleFilePath  XGetModuleFilePathW
#define HWXReplaceFileName  XReplaceFileNameW

#else

#define HWXIsFileInUse    XIsFileInUseA
#define HWXCreateDir      XCreateDirA
#define HWXGenCurPath     XGenCurPathA
#define HWXCreateFile     XCreateFileA
#define HWXIsDirectoryExist        XIsDirectoryExistA
#define HWXIsFileExist    XIsFileExistA
#define HWXRemoveFile     XRemoveFileA
#define HWXDeleteDirectory  XDeleteDirectoryA
#define HWXGetFilePath    XGetFileDirectoryA
#define HWXGetFileExt     XGetFileExtA
#define HWXGetFileTitle   XGetFileTitleA
#define HWXGetFileDirAndName  XGetFileDirAndNameA
#define HWXGetFileDirectory  XGetFileDirectoryA
#define HWXGetFileName    XGetFileNameA 
#define HWXGetModuleFilePath  XGetModuleFilePathA
#define HWXReplaceFileName  XReplaceFileNameA

#endif




//  #ifdef __cplusplus
//  extern "C"{
//  #endif
//将二进制数据写到文件结尾
//基本操作
DLLXEXPORT BOOL WINAPI File_GenTitle(HWX_IN const TCHAR *pchFileName, HWX_OUT TCHAR *ptchTitle);
DLLXEXPORT BOOL  WINAPI File_GenName(HWX_IN const TCHAR *pchFilePath,  HWX_OUT TCHAR *pchName);
DLLXEXPORT BOOL  WINAPI File_CreateFile(HWX_IN const TCHAR * ptchDstFilePath);
DLLXEXPORT BOOL  WINAPI File_CreateDirs(HWX_IN const TCHAR *ptchDstFilePath );
DLLXEXPORT BOOL  WINAPI File_GenCurPath(HWX_OUT TCHAR * ptchPath);
DLLXEXPORT void  WINAPI File_PushBinary(HWX_IN const TCHAR *pchFileName, HWX_IN const void *pvBuffer, HWX_IN size_t stSize);
DLLXEXPORT void  WINAPI File_WriteBinary(HWX_IN const TCHAR *pchFileName, HWX_IN const void *pvBuffer, HWX_IN size_t stSize, HWX_IN BOOL nIsAppend = TRUE);
//将格式化文本写到文件结尾
DLLXEXPORT void  WINAPI File_PushTextA(HWX_IN const TCHAR *pchFilePath, HWX_IN const char *pchFormat, ...);
//将文件按照Tpackage读取出来 
DLLXEXPORT BOOL  WINAPI File_ReadPackage(HWX_IN const TCHAR *pchFilePath, HWX_IN LONG nType, HWX_OUT std::vector<CHWXPackage> &vPackage);
//将Tpackage写到文件
DLLXEXPORT BOOL  WINAPI File_WritePackage(HWX_IN const TCHAR *pchFilePath, HWX_IN std::vector<CHWXPackage> &vPackage, HWX_IN BOOL nIsAppend = TRUE);
//读取文件
DLLXEXPORT BOOL  WINAPI File_ReadFileBinary(HWX_IN const TCHAR *szFilePath, HWX_OUT CHWXPackage *pclsPackage);


DLLXEXPORT  BOOL    WINAPI    XIsFileInUseA(LPCSTR strFileName); 
DLLXEXPORT  BOOL    WINAPI    XIsFileInUseW(LPCWSTR strFileName); 
DLLXEXPORT  BOOL    WINAPI    XCreateDirW(LPCWSTR pwhDstFilePath);
DLLXEXPORT  BOOL    WINAPI    XCreateDirA(LPCSTR pchDstFilePath);
DLLXEXPORT  BOOL    WINAPI    XGenCurPathW(HWX_OUT LPCWSTR pwhPath);
DLLXEXPORT  BOOL    WINAPI    XGenCurPathA(HWX_OUT LPCSTR pchPath);
DLLXEXPORT  BOOL    WINAPI    XCreateFileW(HWX_IN LPCWSTR pwhDstFilePath, BOOL blOverwrite = TRUE);
DLLXEXPORT  BOOL    WINAPI    XCreateFileA(HWX_IN LPCSTR pwhDstFilePath, BOOL blOverwrite = TRUE);
DLLXEXPORT  BOOL    WINAPI    XIsDirectoryExistW(LPCWSTR pwhDir);
DLLXEXPORT  BOOL    WINAPI    XIsDirectoryExistA(LPCSTR pchDir);
DLLXEXPORT  BOOL    WINAPI    XIsFileExistW(LPCWSTR );
DLLXEXPORT  BOOL    WINAPI    XIsFileExistA(LPCSTR );
DLLXEXPORT  BOOL    WINAPI    XRemoveFileW(LPCWSTR );
DLLXEXPORT  BOOL    WINAPI    XRemoveFileA(LPCSTR );
DLLXEXPORT  BOOL    WINAPI    XDeleteDirectoryW(LPCWSTR , BOOL blDelAll);
DLLXEXPORT  BOOL    WINAPI    XDeleteDirectoryA(LPCSTR , BOOL blDelAll);
DLLXEXPORT  CXStringW  WINAPI XGetFileDirectoryW(LPCWSTR );
DLLXEXPORT  CXStringA  WINAPI XGetFileDirectoryA(LPCSTR );
DLLXEXPORT  CXStringW  WINAPI XGetFileExtW(LPCWSTR );
DLLXEXPORT  CXStringA  WINAPI XGetFileExtA(LPCSTR );
DLLXEXPORT  CXStringW  WINAPI XGetFileTitleW(LPCWSTR );
DLLXEXPORT  CXStringA  WINAPI XGetFileTitleA(LPCSTR );
DLLXEXPORT  CXStringW  WINAPI XGetFileDirAndNameW(LPCWSTR );
DLLXEXPORT  CXStringA  WINAPI XGetFileDirAndNameA(LPCSTR );
DLLXEXPORT  CXStringW  WINAPI XGetFileNameW(LPCWSTR );
DLLXEXPORT  CXStringA  WINAPI XGetFileNameA(LPCSTR );
DLLXEXPORT  CXStringA  WINAPI XGetModuleFilePathA(HMODULE hModule);
DLLXEXPORT  CXStringW  WINAPI XGetModuleFilePathW(HMODULE hModule);
DLLXEXPORT  CXStringA  WINAPI XReplaceFileNameA(LPCSTR, LPCSTR);
DLLXEXPORT  CXStringW  WINAPI XReplaceFileNameW(LPCWSTR, LPCWSTR);
// #ifdef __cplusplus
//  }
// #endif

#endif//_DBG_FILE_H_INC_
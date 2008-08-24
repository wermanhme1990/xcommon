#include "stdafx.h"

#include "Ext_File.h"
#include "Ext_Memory.h"
#include "HWXString.h"
#include <tchar.h>
#include <assert.h>
#include <stdio.h>
#include <io.h>
#include <stdio.h>
#include <direct.h>
#include <sys/stat.h>

#define   BUFFER_Max    (4096)

BOOL  WINAPI File_GenTitle(HWX_IN const TCHAR *ptchFilePath, HWX_OUT TCHAR *ptchTitle)
{  
  LONG nTitleLen = 0;
  const TCHAR * ptchFllwHead = ptchFilePath;  
  const TCHAR *  ptchFllwTail = ptchFilePath;
  BOOL blRet = TRUE;
  assert(ptchFilePath && ptchTitle);

  ptchFllwHead = _tcsrchr( ptchFilePath, _T('\\'));
  ptchFllwTail = _tcsrchr( ptchFilePath, _T('.'));
  if (!(ptchFllwHead && ptchFllwTail && (nTitleLen = ptchFllwTail - ptchFllwHead -  1) > 0))
  {
    blRet = FALSE;
    goto Exit_;
  }  
  _tcsncpy(ptchTitle, ptchFllwHead + 1, nTitleLen);

Exit_:  
  return blRet;
}
BOOL WINAPI File_GenName(HWX_IN const TCHAR *ptchFilePath, HWX_OUT TCHAR *ptchName)
{
  LONG nNameLen = 0;
  const TCHAR * ptchFllwHead = ptchFilePath;  
  const TCHAR * ptchFllwTail = ptchFilePath;
  BOOL blRet = TRUE;
  assert(ptchFilePath && ptchName);

  ptchFllwHead = _tcsrchr(ptchFilePath, _T('\\'));  
  ptchFllwTail = ptchFilePath + _tcslen(ptchFilePath);
  if (!(ptchFllwHead && ptchFllwTail && (nNameLen = ptchFllwTail - ptchFllwHead -  1) > 0))
  {
    blRet = FALSE;
    goto Exit_;
  }  
  _tcsncpy(ptchName, ptchFllwHead + 1, nNameLen);

Exit_:  
  return blRet;
}
BOOL WINAPI File_GenFilePath(HWX_IN const TCHAR *ptchFullPath, HWX_OUT TCHAR *ptchPath )
{
  assert(NULL != ptchFullPath);
  assert(NULL != ptchPath);
    
  const TCHAR *pchHead = ptchFullPath, *pchFllwHead = NULL;
  BOOL blRet = TRUE;
  LONG nPathLen;
  
  if (NULL == ptchFullPath || NULL == ptchPath)
  {
    blRet = FALSE;
    goto _Exit;
  }  
  if (!(pchFllwHead = _tcsrchr(pchHead, _T('\\'))))
  {
    blRet = FALSE;
    goto _Exit;
  }
  pchHead = pchFllwHead + 1;
  
  nPathLen = (LONG)_tcslen( ptchFullPath ) - (LONG)_tcslen( pchHead );
  assert(nPathLen > 0);
  _tcsncpy( ptchPath, ptchFullPath, nPathLen );
    
_Exit:
  
  return blRet;
}


BOOL WINAPI File_GenCurPath(HWX_OUT TCHAR * ptchPath)
{
  TCHAR atchPath[MAX_NAME];
  TCHAR *ptchIndex;
  LONG nLen = GetModuleFileName(NULL, atchPath, MAX_NAME);
  assert (nLen < MAX_NAME);
  VERIFY(NULL != (ptchIndex = _tcsrchr(atchPath, _T('\\'))));
  *(ptchIndex) = 0;
  _tcscpy(ptchPath, atchPath);  
  return TRUE;
}

BOOL WINAPI File_CreateFile(HWX_IN const TCHAR *ptchDstFilePath)
{
  FILE *pFile = NULL;  
  BOOL blRet = TRUE;
  assert(ptchDstFilePath);
  if (!ptchDstFilePath)
  {
    blRet = FALSE;
    goto _Error;
  }
  VERIFY(FALSE != File_CreateDirs(ptchDstFilePath));
  
  if (NULL == (pFile = _tfopen(ptchDstFilePath, _T("a+b"))))
  {
    blRet = FALSE;
  }
  fclose(pFile);
_Error:
  return blRet;
}

BOOL WINAPI File_CreateDirs(HWX_IN const TCHAR *ptchDstFilePath )
{  
  const TCHAR *pchTail = ptchDstFilePath;  
  TCHAR szPairentPath[MAX_PATH];
  BOOL blRet = TRUE;
  int nPairentPathLen;
  if (!ptchDstFilePath)
  {
    blRet = FALSE;
    goto _Error;
  }
  
  while(pchTail)
  {    
    pchTail = _tcschr( pchTail, _T('\\'));
    if(pchTail)
    {
      nPairentPathLen = (LONG)_tcslen( ptchDstFilePath ) - (LONG)_tcslen( pchTail );
      _tcsncpy(szPairentPath, ptchDstFilePath, nPairentPathLen);      
      _tmkdir(szPairentPath);      
      pchTail ++;
    }
  }
_Error:
  
  return blRet;
}

void WINAPI File_WriteBinary(HWX_IN const TCHAR *pchFilePath, HWX_IN const void *pvBuffer, HWX_IN size_t stSize, HWX_IN BOOL nIsAppend)
{  
  FILE *pFile = NULL;
  assert(NULL != pchFilePath && NULL != pvBuffer);  
  //创建文件夹 免得不能创建文件
  if (0xFFFFFFFF == GetFileAttributes(pchFilePath))
  {
    VERIFY(FALSE != File_CreateDirs(pchFilePath));
  }
  if(NULL == (pFile = _tfopen(pchFilePath, FALSE != nIsAppend? _T("a+b") : _T("wb"))))
  {    
    return;
  }  
  fwrite(pvBuffer, 1, stSize, pFile);
  fclose(pFile);
  return;  
}



//将二进制添加到文件末尾
void WINAPI File_PushBinary(HWX_IN const TCHAR *pchFilePath, HWX_IN const void *pvBuffer, HWX_IN size_t stSize)
{  
  
  File_WriteBinary(pchFilePath, pvBuffer, stSize, TRUE);

  return;
  
}
void WINAPI FILE_PushBinary(HWX_IN const TCHAR *pchFilePath, HWX_IN const CHWXPackage *ptPackage)
{  
  assert(NULL != ptPackage);
  if (NULL == ptPackage || ptPackage->IsEmpty())
  {
    return;
  }  
  File_WriteBinary(pchFilePath, ptPackage->GetData(), ptPackage->GetSize(), TRUE);

  return;
  
}
//////////////////////////////////////////////////////////////////////////
void WINAPI File_PushTextA(HWX_IN const TCHAR *pchFilePath, HWX_IN const char *pchFormat, ...)
{
  assert(NULL != pchFilePath);
  
  FILE *pFile = NULL;
  char achBuffer[BUFFER_Max] = {0};
  va_list arglist;  
  //创建文件夹 免得不能创建文件
  if (0xFFFFFFFF == GetFileAttributes(pchFilePath))
  {
    File_CreateDirs(pchFilePath);
  }  
  if(NULL == (pFile = _tfopen(pchFilePath, _T("a+t"))))
  {    
    return;
  }
  
  va_start(arglist, pchFormat);
  _vsnprintf(achBuffer, BUFFER_Max, pchFormat, arglist);
  va_end(arglist);
  
  fwrite(achBuffer, sizeof(*achBuffer), strlen(achBuffer), pFile);
  //fflush(pFile);
  fclose(pFile);  
}



BOOL WINAPI File_ReadFileBinary(HWX_IN const TCHAR *ptchFilePath, HWX_OUT CHWXPackage *pclsPackage)
{
  assert(NULL != pclsPackage);
  assert(NULL != ptchFilePath);
  BOOL blRet = TRUE;
  FILE *pFile = NULL;
  DWORD dwLen = 0, dwFileLen = 0;
  BYTE *pbBuffer = NULL;  
  if (NULL == ptchFilePath)
  {
    blRet = FALSE;
    goto Exit_;
  }
  if(NULL == (pFile = _tfopen(ptchFilePath, _T("rb"))))
  {
    goto Exit_; 
  }
  fseek(pFile, 0, SEEK_END);
  dwFileLen = ftell(pFile);  
  if (0 == dwFileLen)
  {    
    goto Exit_;
  }
  fseek(pFile, 0, SEEK_SET);  
  VERIFY(NULL != (pbBuffer = pclsPackage->GetBufferEx(dwFileLen)));  
  VERIFY(dwFileLen == (dwLen = (DWORD)fread(pbBuffer, 1, dwFileLen, pFile)));
  pclsPackage->ReleaseBufferEx(dwFileLen);
Exit_:
  if (NULL != pFile)
  {
    fclose(pFile);
    pFile = NULL;
  }  
  return blRet;
}

BOOL WINAPI File_WritePackage(HWX_IN const TCHAR *pchFilePath, HWX_IN std::vector<CHWXPackage> &vPackage, HWX_IN BOOL nIsAppend)
{
  assert(NULL != pchFilePath);
  
  LONG nRet = TRUE;  
  size_t i = 0;  
  FILE *pFile = NULL;
  if (0xFFFFFFFF == GetFileAttributes(pchFilePath))
  {
    File_CreateFile(pchFilePath);
   }
  if (NULL == (pFile = _tfopen(pchFilePath, FALSE != nIsAppend ? _T("a+b") : _T("wb"))))
  {
    nRet = FALSE;
    goto _Exit;
  }  
  //将包存储到文件中
  for (i = 0; i < vPackage.size(); i ++)
  {
    fwrite(vPackage[i].GetData(), 1, vPackage[i].GetSize(), pFile);    
  }
  fclose(pFile);
_Exit:    
  return nRet;
}

BOOL WINAPI File_ReadPackage(HWX_IN const TCHAR *pchFilePath, HWX_IN LONG nType, HWX_OUT std::vector<CHWXPackage> &vPackage)
{
  assert(NULL != pchFilePath);

  BOOL nRet = TRUE;
  BYTE *pbFileBuffer = NULL;
  DWORD unFileSize = 0;
  DWORD unOffset = 0;
  LONG nThisPackageSize = 0;
  CHWXPackage tPackage;
  CHWXPackage  tFileContent;  
  tPackage.SetIncStep(2048);
  tFileContent.SetIncStep(2048);
  //vPackage.clear();
  if(FALSE == File_ReadFileBinary(pchFilePath, &tFileContent))
  {
    nRet = FALSE;
    goto _Exit;
  }
  unFileSize = tFileContent.GetSize();
  pbFileBuffer = (BYTE *)tFileContent.GetData();
  //将文件内容打散保存
  unOffset = 0; 
  //不同的文件 在包的大小上是不同定义的 有int 也有word
  if (4 == nType)
  {
    while(unOffset < unFileSize)
    {
      nThisPackageSize = *(LONG *)(pbFileBuffer + unOffset);

      assert(0 < nThisPackageSize);
      if (nThisPackageSize + unOffset > unFileSize)
      {//错误数据
        break;
      }
      tPackage.Reset(pbFileBuffer + unOffset, nThisPackageSize);      
      vPackage.push_back(tPackage);      

      unOffset += nThisPackageSize;    
    }
  }
  else if(2 == nType)
  {
    while(unOffset < unFileSize)
    {
      nThisPackageSize = *(WORD *)(pbFileBuffer + unOffset);

      assert(0 < nThisPackageSize);
      if (nThisPackageSize + unOffset > unFileSize)
      {//错误数据
        break;
      }
      tPackage.Reset(pbFileBuffer + unOffset, nThisPackageSize);      
      vPackage.push_back(tPackage);      

      unOffset += nThisPackageSize;    
    }
  }

_Exit:
  tFileContent.Clear();
  return nRet;
}

BOOL WINAPI XIsFileInUseW(LPCWSTR strFileName)  
{  
  assert(strFileName);
  BOOL   blRet = FALSE;
  if (0 != _waccess(strFileName, 0))
  {
    return blRet;
  }
  HANDLE Handle = CreateFileW(strFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);  
  if (INVALID_HANDLE_VALUE != Handle)  
  {
    CloseHandle(Handle); 
  }  
  else
  {
    blRet = TRUE;
  }
  return   blRet;  
}
BOOL WINAPI XIsFileInUseA(LPCSTR strFileName)  
{  
  assert(strFileName);
  BOOL   blRet = FALSE;
  if (0 != _access(strFileName, 0))
  {
    return blRet;
  }
  HANDLE Handle = CreateFileA(strFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);  
  if (INVALID_HANDLE_VALUE != Handle)  
  {
    CloseHandle(Handle); 
  }  
  else
  {
    blRet = TRUE;
  }
  return   blRet;  
}
CXStringW WINAPI XGetFileDirectoryW(LPCWSTR pwhFilePath)
{
  WCHAR szDir[_MAX_DIR];
  WCHAR szDrive[_MAX_DRIVE];
  _wsplitpath(pwhFilePath, szDrive, szDir, NULL, NULL);
  return  CXStringW(szDrive) + szDir;
}
CXStringA WINAPI XGetFileDirectoryA(LPCSTR pchFilePath)
{
  CHAR szDir[_MAX_DIR];
  CHAR szDrive[_MAX_DRIVE];
  _splitpath(pchFilePath, szDrive, szDir, NULL, NULL);
  return  CXStringA(szDrive) + szDir;
}
CXStringW WINAPI XGetFileExtW(LPCWSTR pwhFilePath)
{
  WCHAR szExt[_MAX_EXT];
  _wsplitpath(pwhFilePath, NULL, NULL, NULL, szExt);
  return CXStringW(szExt);
}
CXStringA WINAPI XGetFileExtA(LPCSTR pchFilePath)
{
  CHAR szExt[_MAX_EXT];
  _splitpath(pchFilePath, NULL, NULL, NULL, szExt);
  return CXStringA(szExt);
}


CXStringW WINAPI XGetFileTitleW(LPCWSTR lpszFilePath)
{
  WCHAR szFname[_MAX_FNAME];
  _wsplitpath(lpszFilePath, NULL, NULL, szFname, NULL);
  return  CXStringW(szFname);
}
CXStringA WINAPI XGetFileTitleA(LPCSTR lpszFilePath)
{
  CHAR szFname[_MAX_FNAME];
  _splitpath(lpszFilePath, NULL, NULL, szFname, NULL);
  return  CXStringA(szFname);
}

CXStringW WINAPI XGetFileDirAndNameW(LPCWSTR lpszFilePath)
{
  WCHAR szDir[_MAX_DIR];
  WCHAR szFname[_MAX_FNAME];
  WCHAR szExt[_MAX_EXT];
  _wsplitpath(lpszFilePath, NULL , szDir, szFname, szExt);
  CXStringW strDir = szDir;
  strDir.TrimLeft(L"\\");
  return  strDir + szFname + szExt;
}
CXStringA WINAPI XGetFileDirAndNameA(LPCSTR lpszFilePath)
{
  CHAR szDir[_MAX_DIR];
  CHAR szFname[_MAX_FNAME];
  CHAR szExt[_MAX_EXT];
  _splitpath(lpszFilePath, NULL , szDir, szFname, szExt);
  CXStringA strDir = szDir;
  strDir.TrimLeft("\\");
  return  strDir + szFname + szExt;
}

CXStringW WINAPI XGetFileNameW(LPCWSTR lpszFilePath)
{
  WCHAR szExt[_MAX_EXT];
  WCHAR szName[_MAX_FNAME];
  _wsplitpath(lpszFilePath, NULL, NULL, szName, szExt);
  return CXStringW(szName) + szExt;
}
CXStringA WINAPI XGetFileNameA(LPCSTR lpszFilePath)
{
  CHAR szExt[_MAX_EXT];
  CHAR szName[_MAX_FNAME];
  _splitpath(lpszFilePath, NULL, NULL, szName, szExt);
  return CXStringA(szName) + szExt;
}

BOOL WINAPI XCreateDirA(HWX_IN LPCSTR pchDstFilePath )
{   
  LPCSTR pchTail = pchDstFilePath;  
  CHAR szPairentPath[MAX_PATH] = {0};
  BOOL blRet = TRUE;
  int nPairentPathLen;
  if (!pchDstFilePath)
  {
    blRet = FALSE;
    goto _Error;
  }

  while(pchTail)
  {    
    pchTail = strchr( pchTail, '\\');
    if(pchTail)
    {
      nPairentPathLen = (LONG)strlen( pchDstFilePath ) - (LONG)strlen( pchTail ) + 1;
      strncpy(szPairentPath, pchDstFilePath, nPairentPathLen); 
      szPairentPath[nPairentPathLen + 1] = 0;
      if (!mkdir(szPairentPath))
      {
				blRet = FALSE;
				break;
      }            
      pchTail ++;
    }
  }
_Error:

  return blRet;
}
BOOL WINAPI XCreateDirW(HWX_IN LPCWSTR pwhDstFilePath )
{    
  LPCWSTR pwhTail = pwhDstFilePath;  
  WCHAR szPairentPath[MAX_PATH] = {0};
  BOOL blRet = TRUE;
  int nPairentPathLen;
  if (!pwhDstFilePath)
  {
    blRet = FALSE;
    goto _Error;
  }

  while(pwhTail)
  {    
    pwhTail = wcschr( pwhTail, L'\\');
    if(pwhTail)
    {
      nPairentPathLen = (LONG)wcslen( pwhDstFilePath ) - (LONG)wcslen( pwhTail ) + 1;
      wcsncpy(szPairentPath, pwhDstFilePath, nPairentPathLen);  
      szPairentPath[nPairentPathLen + 1] = 0;
      if (!_wmkdir(szPairentPath))
      {
				blRet = FALSE;
				break;
      }         
      pwhTail ++;
    }
  }
_Error:

  return blRet;
}

BOOL WINAPI XGenCurPathA(HWX_OUT LPSTR pchPath)
{
  CHAR achPath[MAX_PATH];
  CHAR *pchIndex;
  LONG nLen = GetModuleFileNameA(NULL, achPath, MAX_PATH);
  assert (nLen < MAX_PATH); 
  VERIFY(NULL != (pchIndex = strrchr(achPath, '\\')));
  *(pchIndex) = 0;
  strcpy(pchPath, achPath);  
  return TRUE;
}
BOOL WINAPI XGenCurPathW(HWX_OUT LPWSTR pwhPath)
{
  WCHAR awhPath[MAX_PATH];
  WCHAR *pwhIndex;
  LONG nLen = GetModuleFileNameW(NULL, awhPath, MAX_PATH);
  assert (nLen < MAX_PATH);
  VERIFY(NULL != (pwhIndex = wcsrchr(awhPath, L'\\')));
  *(pwhIndex) = 0;
  wcscpy(pwhPath, awhPath);  
  return TRUE;
}

BOOL WINAPI XCreateFileW(HWX_IN LPCWSTR pwhDstFilePath)
{
  FILE *pFile = NULL;  
  BOOL blRet = TRUE;
  assert(pwhDstFilePath);
  if (!pwhDstFilePath)
  {
    blRet = FALSE;
    goto _Error;
  }
  VERIFY(FALSE != XCreateDirW(pwhDstFilePath));

  if (NULL == (pFile = _wfopen(pwhDstFilePath, L"a+b")))
  {
    blRet = FALSE;
  }
  fclose(pFile);
_Error:
  return blRet;
}
BOOL WINAPI XCreateFileA(HWX_IN LPCSTR pchDstFilePath, BOOL blOverwrite)
{  
  BOOL blRet = FALSE;
  assert(pchDstFilePath);
  if (!pchDstFilePath)
  {
    blRet = FALSE;
    return blRet;
  }
  VERIFY(FALSE != XCreateDirA(pchDstFilePath));
 
  
  HANDLE nFileHandle = NULL;
  if (blOverwrite)
  {
    nFileHandle = CreateFileA(pchDstFilePath, GENERIC_READ | GENERIC_WRITE, 0, 
      NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  
  }
  else
  {
    nFileHandle = CreateFileA(pchDstFilePath, GENERIC_READ | GENERIC_WRITE, 0, 
      NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);  
  }  
  if (INVALID_HANDLE_VALUE != nFileHandle)  
  {
    CloseHandle(nFileHandle); 
    blRet = TRUE;
  }   
  return   blRet; 
}
BOOL WINAPI XCreateFileW(HWX_IN LPCWSTR pwhDstFilePath, BOOL blOverwrite)
{  
  BOOL blRet = FALSE;
  assert(pwhDstFilePath);
  if (!pwhDstFilePath)
  {
    blRet = FALSE;
    return blRet;
  }
  VERIFY(FALSE != XCreateDirW(pwhDstFilePath));


  HANDLE nFileHandle = NULL;
  if (blOverwrite)
  {
    nFileHandle = CreateFileW(pwhDstFilePath, GENERIC_READ | GENERIC_WRITE, 0, 
      NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  
  }
  else
  {
    nFileHandle = CreateFileW(pwhDstFilePath, GENERIC_READ | GENERIC_WRITE, 0, 
      NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);  
  }  
  if (INVALID_HANDLE_VALUE != nFileHandle)  
  {
    CloseHandle(nFileHandle); 
    blRet = TRUE;
  }   
  return   blRet; 
}

BOOL WINAPI XIsDirectoryExistW(LPCWSTR pwhDir)
{  
  wchar_t wchPath[_MAX_PATH];  
  BOOL blRet = TRUE;
  if (!_wgetcwd(wchPath, _MAX_PATH))
  {
    blRet = FALSE;
  }
  else
  {
    if (_wchdir(pwhDir))
    {
      blRet = FALSE;
    }
    else
    {
      _wchdir(wchPath);
    }    
  }
  return blRet;
}
BOOL WINAPI XIsDirectoryExistA(LPCSTR pchDir)
{
  char achPath[_MAX_PATH];  
  BOOL blRet = TRUE;
  if (!getcwd(achPath, _MAX_PATH))
  {
    blRet = FALSE;
  }
  else
  {
    if (chdir(pchDir))
    {
      blRet = FALSE;
    }
    else
    {
      chdir(achPath);
    }    
  }
  return blRet;
}
BOOL WINAPI XIsFileExistW(LPCWSTR pwhPath)
{
  assert(pwhPath);
  BOOL blRet = TRUE;
  if (0 != _waccess(pwhPath, 0))
  { 
    blRet = FALSE;
  }   
  return blRet;
}
BOOL WINAPI XIsFileExistA(LPCSTR pchPath)
{
  assert(pchPath);
  BOOL blRet = TRUE;
  if (0 != _access(pchPath, 0))
  { 
    blRet = FALSE;
  }   
  return blRet;
}
BOOL WINAPI XRemoveFileW(LPCWSTR pwhPath)
{
  assert(pwhPath);
  BOOL blRet = TRUE;
  if (0 == _waccess(pwhPath, 0) && 0 != _wremove(pwhPath))
  {    
    _wchmod(pwhPath, _S_IREAD | _S_IWRITE);  
    blRet = (-1 != _wremove(pwhPath)) ? TRUE : FALSE;
  }   
  return blRet;
}
BOOL WINAPI XRemoveFileA(LPCSTR pchPath)
{
  assert(pchPath);
  BOOL blRet = TRUE;
  if (0 == _access(pchPath, 0) && 0 != remove(pchPath))
  {  
    _chmod(pchPath, _S_IREAD | _S_IWRITE);  
    blRet = (-1 != remove(pchPath)) ? TRUE : FALSE;
  }   
  return blRet;
}
BOOL WINAPI XDeleteDirectoryW(LPCWSTR pwhDir, BOOL blDelAll)
{
  BOOL blReturn = TRUE;
  WCHAR awhDir[_MAX_PATH];
  awhDir[0] = 0;
  wcscpy(awhDir, pwhDir);
  LONG nTail = (LONG)wcslen(awhDir) - 1;
  if (nTail <= 0)
  {
    return FALSE;
  }
  if (awhDir[nTail] == L'\\' || awhDir[nTail] == L'/')
  {
    awhDir[nTail] = 0;
  }

  WCHAR awhFile[MAX_PATH];
  awhFile[0] = 0;
  swprintf(awhFile, L"%s\\*.*", awhDir);

  struct _wfinddata_t tFileSet;
  intptr_t hFile;	
  if ((hFile = _wfindfirst(awhFile, &tFileSet)) != -1L)
  {
    BOOL blFinded = TRUE;
    while (blFinded)
    {
      if (tFileSet.attrib & _A_SUBDIR)
      {
        if (!(0 == wcscmp(tFileSet.name,L"." )|| 0 == wcscmp(tFileSet.name, L"..")))
        {////删除子目录 排除.和..目录
          swprintf(awhFile, L"%s\\%s", awhDir, tFileSet.name);
          blReturn = XDeleteDirectoryW(awhFile, blDelAll);
        }			
      }
      else
      {	//删除文件
        swprintf(awhFile, L"%s\\%s", awhDir, tFileSet.name);
        //判定文件是否是只读的
        if (tFileSet.attrib & _A_RDONLY)
        {
          if (blDelAll && (0 == _wchmod(awhFile, _S_IREAD|_S_IWRITE)))
          {
            blReturn = (-1 != _wremove(awhFile)) ? TRUE : FALSE;
          }
          else
          {
            blReturn = FALSE;
          }
        }
        else
        {
          blReturn = (-1 != _wremove(awhFile)) ? TRUE : FALSE;
        }
      }
      blFinded = (0 == _wfindnext(hFile, &tFileSet))? TRUE : FALSE;
    }
  }
  _findclose(hFile);

  if (0 == _wrmdir(awhDir))
  {
    blReturn = TRUE;
  }
  return blReturn;
}
BOOL WINAPI XDeleteDirectoryA(LPCSTR pchDir, BOOL blDelAll)
{
  BOOL blReturn = TRUE;
  char achDir[_MAX_PATH];
  achDir[0] = 0;
  strcpy(achDir, pchDir);
  LONG nTail = (LONG)strlen(achDir) - 1;
  if (nTail <= 0)
  {
    return FALSE;
  }
  if (achDir[nTail] == '\\' || achDir[nTail] == '/')
  {
    achDir[nTail] = 0;
  }

  char achFile[_MAX_PATH];
  achFile[0] = 0;
  sprintf(achFile, "%s\\*.*", achDir);

  struct _finddata_t tFileSet;
  intptr_t hFile;	
  if ((hFile = _findfirst(achFile, &tFileSet)) != -1L)
  {
    BOOL blFinded = TRUE;
    while (blFinded)
    {
      if (tFileSet.attrib & _A_SUBDIR)
      {
        if (!(0 == strcmp(tFileSet.name, "." )|| 0 == strcmp(tFileSet.name, "..")))
        {////删除子目录 排除.和..目录
          sprintf(achFile, "%s\\%s", achDir, tFileSet.name);
          blReturn = XDeleteDirectoryA(achFile, blDelAll);
        }			
      }
      else
      {	//删除文件
        sprintf(achFile, "%s\\%s", achDir, tFileSet.name);
        //判定文件是否是只读的
        if (tFileSet.attrib & _A_RDONLY)
        {
          if (blDelAll && (0 == _chmod(achFile, _S_IREAD|_S_IWRITE)))
          {
            blReturn = (-1 != remove(achFile)) ? TRUE : FALSE;
          }
          else
          {
            blReturn = FALSE;
          }
        }
        else
        {
          blReturn = (-1 != remove(achFile)) ? TRUE : FALSE;
        }
      }
      blFinded = (0 == _findnext(hFile, &tFileSet))? TRUE : FALSE;
    }
  }
  _findclose(hFile);

  if (0 == _rmdir(achDir))
  {
    blReturn = TRUE;
  }
  return blReturn;
}
CXStringA WINAPI XGetModuleFilePathA( HMODULE hModule )
{
  CHAR achPath[MAX_PATH];
  GetModuleFileNameA(hModule, achPath, MAX_PATH - 1);
	CXStringA strPath(achPath);
	strPath.ReplaceChar('\\', '/');
  return strPath;
}
CXStringW WINAPI XGetModuleFilePathW( HMODULE hModule )
{
  WCHAR awhPath[MAX_PATH];
  LONG nLen = GetModuleFileNameW(hModule, awhPath, MAX_PATH - 1);
  assert (nLen < MAX_PATH);
	CXStringW strPath(awhPath);
	strPath.ReplaceChar('\\', '/');
  return CXStringW(awhPath);
}
CXStringA WINAPI XReplaceFileNameA( LPCSTR pchPath, LPCSTR pchName)
{
  assert(pchName && pchPath);  
  CXStringA strName(pchName);
  strName.TrimLeft("\\/");
  CXStringA strDictionary = XGetFileDirectoryA(pchPath);
  strDictionary += strName; 
  return strDictionary;
}
CXStringW WINAPI XReplaceFileNameW( LPCWSTR pchPath, LPCWSTR pchName)
{
  assert(pchName && pchPath);  
  CXStringW strName(pchName);
  strName.TrimLeft(L"\\/");
  CXStringW strDictionary = XGetFileDirectoryW(pchPath);
  strDictionary += strName; 
  return strDictionary;
}
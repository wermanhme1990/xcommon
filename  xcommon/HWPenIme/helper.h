#pragma once

#define LOGFILE "c:\\imesample.log"

#define GETLPCOMPSTR(lpcs) (LPTSTR)((LPBYTE)(lpcs) + (lpcs)->dwCompStrOffset)
#define GETLPRESULTSTR(lpcs) (LPTSTR)((LPBYTE)(lpcs) + (lpcs)->dwResultStrOffset)
#define GETLPCANDSTR(lpcs,i) (LPTSTR)((LPBYTE)(lpcs) + (lpcs)->dwOffset[i])
void Helper_Trace(TCHAR * pszFormat,...);

void Helper_DrawBMP(HDC hdc,RECT rc,UINT bmpID);

BOOL Helper_IsIMEMessage(UINT message);

void Helper_CenterWindow(HWND hWnd);

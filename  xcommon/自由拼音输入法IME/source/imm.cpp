/*
* Copyright (C) 1999.4  Li ZhenChun
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License; or
* (at your option) any later version.
*
* This program is distributed in the hope that is will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, M A 02139, USA.
*
* Author: Li ZhenChun  email: zhchli@163.net or zhchli@126.com
* 
*/
#include "stdafx.h"
#include "freepy.h"

BOOL WINAPI ImeInquire(LPIMEINFO lpIMEInfo,LPTSTR lpszUIClass,LPCTSTR lpszOption)
{
	TRACE(TEXT("ImeInquire\n"));
	lpIMEInfo->dwPrivateDataSize = sizeof(UIEXTRA);

	lpIMEInfo->fdwProperty = IME_PROP_KBD_CHAR_FIRST |
#ifdef _UNICODE
		IME_PROP_UNICODE |
#endif
		IME_PROP_SPECIAL_UI;

	lpIMEInfo->fdwConversionCaps = IME_CMODE_FULLSHAPE | IME_CMODE_NATIVE;
	lpIMEInfo->fdwSentenceCaps = IME_SMODE_NONE;
	lpIMEInfo->fdwUICaps = UI_CAP_2700;
	lpIMEInfo->fdwSCSCaps = 0;
	lpIMEInfo->fdwSelectCaps = SELECT_CAP_CONVERSION;
	_tcscpy(lpszUIClass, UICLASSNAME);

	return TRUE;
}

BOOL WINAPI ImeConfigure(HKL hKL,HWND hWnd, DWORD dwMode, LPVOID lpData)
{
	TRACE(TEXT("ImeConfigure\n"));
	DialogBox( hInst, MAKEINTRESOURCE(DIALOGCONFIG), hWnd, ConfigDialogProc);
	InvalidateRect(hWnd, NULL, FALSE);
	return TRUE;
}

DWORD WINAPI ImeConversionList(HIMC hIMC,LPCTSTR lpSource, LPCANDIDATELIST lpCandList,DWORD dwBufLen,UINT uFlag)
{
	TRACE(TEXT("ImeConversionList\n"));
	return 0;
}

BOOL WINAPI ImeDestroy(UINT uForce)
{
	TRACE(TEXT("ImeDestroy\n"));
	return FALSE;
}

LRESULT WINAPI ImeEscape(HIMC hIMC,UINT uSubFunc,LPVOID lpData)
{
	TRACE(TEXT("ImeEscape\n"));
	return FALSE;
}

BOOL WINAPI ImeProcessKey(HIMC hIMC,UINT vKey,LPARAM lKeyData,CONST LPBYTE lpbKeyState)
{	
	BOOL fRet = FALSE;
	BOOL fOpen;
	BOOL fCompStr = FALSE;
	LPINPUTCONTEXT lpIMC;
	LPCOMPOSITIONSTRING lpCompStr;
	static BOOL fPressOther = FALSE;
	static BOOL fFirst = TRUE;
	TRACE(TEXT("ImeProcessKey\n"));
	if ((lKeyData & 0x80000000) && vKey != VK_CONTROL)
	{
		return FALSE;
	}
	if (!(lKeyData & 0x80000000) && vKey == VK_CONTROL)
	{
		return FALSE;
	}
	if (lpbKeyState[VK_MENU] & 0x80 ) 
	{
		return FALSE;
	}
	if(vKey != VK_CONTROL && lpbKeyState[VK_CONTROL] & 0x80 ) 
	{
		fPressOther = TRUE;
		return FALSE;
	}
	if (!(lpIMC = ImmLockIMC(hIMC)))
	{
		return FALSE;
	}
	fOpen = lpIMC->fOpen;
	if(vKey == VK_CONTROL && (lKeyData & 0x80000000) && !fPressOther && !fFirst)
	{
		GENEMSG GnMsg;
		fPressOther = FALSE;
		if(fOpen) 
		{
			lpIMC->fOpen = FALSE;
			MakeResultString(hIMC, FALSE);
		}
		else 
		{
			lpIMC->fOpen = TRUE;
		}
		GnMsg.msg = WM_IME_NOTIFY;
		GnMsg.wParam = IMN_SETOPENSTATUS;
		GnMsg.lParam = 0;
		GenerateMessage(hIMC, (LPGENEMSG)&GnMsg);
		return FALSE;
	}
	fPressOther = FALSE;
	if(fFirst)
	{
		fFirst = FALSE;
	}
	/*
	//if CapsLock is pressed ,then return false;
	if (lpbKeyState[VK_CAPITAL] & 0x01) {
	return FALSE;
	}

	if (fOpen)
	{
	if (lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr))
	{
	if ((lpCompStr->dwSize > sizeof(COMPOSITIONSTRING)) && 
	(lpCompStr->dwCompStrLen))
	fCompStr = TRUE;
	ImmUnlockIMCC(lpIMC->hCompStr);
	}

	if (dwConversionMode & CONVERSION_MODE_FULLSHAPE){
	if (lpbKeyState[VK_SHIFT] & 0x80)
	{
	if (fCompStr)
	fRet = (BOOL)bFullCompSht[vKey];
	else
	fRet = (BOOL)bFullNoCompSht[vKey];
	}
	else 
	{
	if (fCompStr)
	fRet = (BOOL)bFullComp[vKey];
	else
	fRet = (BOOL)bFullNoComp[vKey];
	}
	}
	else{
	if (lpbKeyState[VK_SHIFT] & 0x80)
	{
	if (fCompStr)
	fRet = (BOOL)bCompSht[vKey];
	else
	fRet = (BOOL)bNoCompSht[vKey];
	}
	else 
	{
	if (fCompStr)
	fRet = (BOOL)bComp[vKey];
	else
	fRet = (BOOL)bNoComp[vKey];
	}
	}
	}
	*/
	if (fOpen)
	{
		if (lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr))
		{
			if ((lpCompStr->dwSize > sizeof(COMPOSITIONSTRING)) && (lpCompStr->dwCompStrLen))
			{
				fCompStr = TRUE;
			}
			ImmUnlockIMCC(lpIMC->hCompStr);
		}
		if (fCompStr)
		{
			fRet = (BOOL)bComp[vKey];
		}
		else
		{
			fRet = (BOOL)bNoComp[vKey];
		}
	}
	ImmUnlockIMC(hIMC);
	return fRet;
}


BOOL WINAPI ImeSelect(HIMC hIMC, BOOL fSelect)
{
	LPINPUTCONTEXT lpIMC;
	TRACE(TEXT("ImeSelect: %d\n"), fSelect);
	if (fSelect)
	{
		UpdateIndicIcon(hIMC);
	}
	if (!hIMC)
	{
		return TRUE;
	}

	if (lpIMC = ImmLockIMC(hIMC))
	{
		if (fSelect)
		{
			LPCOMPOSITIONSTRING lpCompStr;
			LPCANDIDATEINFO lpCandInfo;
			HKEY hKey;
			DWORD dwConvSet;
			DWORD dwData = sizeof(DWORD);
			BOOL fFlag = TRUE;

			lpIMC->fOpen=TRUE;

			lpIMC->hCompStr = ImmReSizeIMCC(lpIMC->hCompStr,sizeof(MYCOMPSTR));
			if (lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr)) 
			{
				InitCompStr(lpCompStr);
				ImmUnlockIMCC(lpIMC->hCompStr);
			}
			lpIMC->hCandInfo = ImmReSizeIMCC(lpIMC->hCandInfo,sizeof(MYCAND));
			if (lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo)) 
			{
				InitCandInfo(lpCandInfo);
				ImmUnlockIMCC(lpIMC->hCandInfo);
			}
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\freepy"), 0, KEY_READ, &hKey) == ERROR_SUCCESS )
			{
				if( RegQueryValueEx( hKey, _T("ConversionSet"),	NULL,	NULL,	(LPBYTE)&dwConvSet,	&dwData) == ERROR_SUCCESS )
				{
					wConversionSet = (WORD) dwConvSet;
					fFlag = FALSE;
				}
				RegCloseKey(hKey);
			}
			if( fFlag) 
			{
				wConversionSet = CONVERSION_SET_PUNCT;
				wConversionSet |= CONVERSION_SET_SORT;
				wConversionSet |= CONVERSION_SET_FOLLOW;
				wConversionSet |= CONVERSION_SET_GBK;
			}
		}
	}
	ImmUnlockIMC(hIMC);
	return TRUE;
}

BOOL WINAPI ImeSetActiveContext(HIMC hIMC, BOOL fFlag)
{
	TRACE(TEXT("ImeSetActiveContext %d\n"), fFlag);
	UpdateIndicIcon(hIMC);
	return TRUE;
}

UINT WINAPI ImeToAsciiEx (UINT uVKey,UINT uScanCode,CONST LPBYTE lpbKeyState, LPDWORD lpdwTransKey,UINT fuState, HIMC hIMC)
{
	LPARAM lParam;
	LPINPUTCONTEXT lpIMC;
	BOOL fOpen = FALSE;
	TRACE(TEXT("ImeToAsciiEx\n"));	
	lParam = ((DWORD)uScanCode << 16) + 1L;	
	if (!hIMC)
	{
		return 0;
	}
	if (!(lpIMC = ImmLockIMC(hIMC)))
	{
		return 0;
	}
	fOpen = lpIMC->fOpen;
	ImmUnlockIMC(hIMC);	
	lpIMC = NULL;
	if (!fOpen)
	{
		goto my_exit;
	}

	if (uScanCode & 0x8000)
	{
		IMEKeyupHandler( hIMC, uVKey, lParam, lpbKeyState);
	}
	else
	{
		IMEKeydownHandler( hIMC, uVKey, lParam, lpbKeyState);
	}
		
my_exit:
	lpIMC = ImmLockIMC(hIMC);
	TRACE(TEXT("ImeToAsciiEx MessageCount : %d\n"), lpIMC->dwNumMsgBuf);
	return lpIMC->dwNumMsgBuf;
	if (fOverTransKey)
	{
		return 0;
	}
	TRACE(TEXT("MessageCount : %d\n"), 0);
	return 0;
}

BOOL WINAPI NotifyIME(HIMC hIMC, DWORD dwAction,DWORD dwIndex,DWORD dwValue)
{
	BOOL bRet = FALSE;
	LPINPUTCONTEXT lpIMC;

	switch(dwAction)
	{
	case NI_OPENCANDIDATE:
		TRACE(TEXT("NotifyIME:NI_OPENCANDIDATE\n"));
		break;
	case NI_CLOSECANDIDATE:
		TRACE(TEXT("NotifyIME:NI_CLOSECANDIDATE\n"));
		break;
	case NI_SELECTCANDIDATESTR:
		TRACE(TEXT("NotifyIME:NI_SELECTCANDIDATESTR %d\n"), dwIndex);
		break;
	case NI_CHANGECANDIDATELIST:
		TRACE(TEXT("NotifyIME:NI_CHANGECANDIDATELIST %d\n"), dwIndex);
		break;
	case NI_SETCANDIDATE_PAGESTART:
		TRACE(TEXT("NotifyIME:NI_SETCANDIDATE_PAGESTART %d\n"), dwIndex);
		break;
	case NI_SETCANDIDATE_PAGESIZE:
		TRACE(TEXT("NotifyIME:NI_SETCANDIDATE_PAGESIZE %d\n"),dwIndex);
		break;
	case NI_CONTEXTUPDATED:		
		switch (dwValue)
		{
		case IMC_SETCONVERSIONMODE:
			TRACE(TEXT("NotifyIME:NI_CONTEXTUPDATED:IMC_SETCONVERSIONMODE\n"));
			break;
		case IMC_SETSENTENCEMODE:
			TRACE(TEXT("NotifyIME:NI_CONTEXTUPDATED:IMC_SETSENTENCEMODE\n"));
			break;
		case IMC_SETCANDIDATEPOS:
			TRACE(TEXT("NotifyIME:NI_CONTEXTUPDATED:IMC_SETCANDIDATEPOS\n"));
			break;
		case IMC_SETCOMPOSITIONFONT:
			TRACE(TEXT("NotifyIME:NI_CONTEXTUPDATED:IMC_SETCOMPOSITIONFONT\n"));
			break;
		case IMC_SETCOMPOSITIONWINDOW:
			TRACE(TEXT("NotifyIME:NI_CONTEXTUPDATED:IMC_SETCOMPOSITIONWINDOW\n"));
			break;
		case IMC_SETOPENSTATUS:
			TRACE(TEXT("NotifyIME:NI_CONTEXTUPDATED:IMC_SETOPENSTATUS\n"));
			lpIMC = ImmLockIMC(hIMC);
			if (lpIMC)
			{
				if (!lpIMC->fOpen && IsCompStr(hIMC))
					MakeResultString( hIMC,FALSE);
				ImmUnlockIMC(hIMC);
			}

			UpdateIndicIcon(hIMC);
			bRet = TRUE;
			break;
		default:
			break;
		}
		break;

	case NI_COMPOSITIONSTR:		
		switch (dwIndex)
		{
		case CPS_COMPLETE:
			TRACE(TEXT("NotifyIME:NI_COMPOSITIONSTR:CPS_COMPLETE\n"));
			break;

		case CPS_CONVERT:
			TRACE(TEXT("NotifyIME:NI_COMPOSITIONSTR:CPS_CONVERT\n"));
			break;

		case CPS_REVERT:
			TRACE(TEXT("NotifyIME:NI_COMPOSITIONSTR:CPS_REVERT\n"));
			break;

		case CPS_CANCEL:
			TRACE(TEXT("NotifyIME:NI_COMPOSITIONSTR:CPS_CANCEL\n"));
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}
	return bRet;
}

BOOL WINAPI ImeRegisterWord(LPCTSTR lpRead, DWORD dw, LPCTSTR lpStr)
{
	TRACE(TEXT("ImeRegisterWord\n"));
	return FALSE;
}

BOOL WINAPI ImeUnregisterWord(LPCTSTR lpRead, DWORD dw, LPCTSTR lpStr)
{
	TRACE(TEXT("ImeUnregisterWord\n"));
	return FALSE;
}

UINT WINAPI ImeGetRegisterWordStyle(UINT nItem, LPSTYLEBUF lp)
{
	TRACE(TEXT("ImeGetRegisterWordStyle\n"));
	return 0;
}

UINT WINAPI ImeEnumRegisterWord(REGISTERWORDENUMPROC lpfn, LPCTSTR lpRead, DWORD dw, LPCTSTR lpStr, LPVOID lpData)
{
	TRACE(TEXT("ImeEnumRegisterWord\n"));
	return 0;
}

BOOL WINAPI ImeSetCompositionString(HIMC hIMC, DWORD dwIndex, LPCVOID lpComp, DWORD dwComp, LPCVOID lpRead, DWORD dwRead)
{
	TRACE(TEXT("ImeSetCompositionString\n"));
	return FALSE;
}
DWORD WINAPI ImeGetImeMenuItems(HIMC hIMC, DWORD dwFlags,	DWORD dwType,
																LPIMEMENUITEMINFO lpImeParentMenu,	LPIMEMENUITEMINFO lpImeMenu,
																DWORD dwSize
																)
{
	TRACE(TEXT("ImeGetImeMenuItems\n"));
	return 0;
}
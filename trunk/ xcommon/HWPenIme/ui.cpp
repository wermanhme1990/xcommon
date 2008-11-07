
#include "stdafx.h"
#include "ui.h"
#include "HWPenIme.h"
#define			FIGWL_SVRWND				(8)
/**********************************************************************/
/* 显示状态栏                                                       */
/**********************************************************************/
void PASCAL UIWnd_OpenStatus( HWND hUIWnd ,BOOL bShow)
{
	HGLOBAL        hUIPrivate;
	LPUIPRIV       lpUIPrivate;
	HIMC           hIMC;
	int            nShowCmd=bShow?SW_SHOWNOACTIVATE:SW_HIDE;

	hUIPrivate = (HGLOBAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
	if (!hUIPrivate)           // can not darw status window
		return;


	lpUIPrivate = (LPUIPRIV)GlobalLock(hUIPrivate);
	if (!lpUIPrivate)          // can not draw status window
		return;


	hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
	if (!hIMC) 
	{
		nShowCmd = SW_HIDE;
	} 
	else if (!ImmLockIMC(hIMC))
	{
		nShowCmd = SW_HIDE;
	}
	else
	{
		ImmUnlockIMC(hIMC);
	}

	if(!lpUIPrivate->hStatusWnd)
	{
		RECT rcWorkArea;
		LPINPUTCONTEXT lpIMC = (LPINPUTCONTEXT)ImmLockIMC(hIMC);
		SystemParametersInfo(SPI_GETWORKAREA,0,&rcWorkArea,0);
		lpUIPrivate->hStatusWnd = CreateWindowEx(0, CLSNAME_STATUS, NULL, WS_POPUP | WS_DISABLED,	rcWorkArea.right - 200, rcWorkArea.bottom - 25,
			200, 25,	hUIWnd, (HMENU)NULL, g_hInst, NULL);
	}
	ShowWindow( lpUIPrivate->hStatusWnd, nShowCmd);
	GlobalUnlock(hUIPrivate);
	return;
}

/**********************************************************************/
/* 显示状态栏                                                       */
/**********************************************************************/
void PASCAL UIWnd_OpenInput(HWND hUIWnd ,BOOL bShow)
{
	HGLOBAL        hUIPrivate;
	LPUIPRIV       lpUIPrivate;
	HIMC           hIMC;
	LPINPUTCONTEXT lpIMC;
	POINT	 pt;
	int    nShowCmd=bShow?SW_SHOWNOACTIVATE:SW_HIDE;

	hUIPrivate = (HGLOBAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
	if (!hUIPrivate)           // can not darw status window
		return;


	lpUIPrivate = (LPUIPRIV)GlobalLock(hUIPrivate);
	if (!lpUIPrivate)          // can not draw status window
		return;


	hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
	lpIMC = (LPINPUTCONTEXT)ImmLockIMC(hIMC);
	pt.x=LOWORD(lpIMC->dwReserve[0]);
	pt.y=HIWORD(lpIMC->dwReserve[0]);
	ImmUnlockIMC(hIMC);

	if(!lpUIPrivate->hInputWnd)
	{
		lpUIPrivate->hInputWnd = CreateWindowEx(0, CLSNAME_INPUT, NULL, WS_POPUP | WS_DISABLED,
			pt.x, pt.y,	200,50, hUIWnd, (HMENU)NULL, g_hInst, NULL);
	}
	else if(bShow)
	{
		SetWindowPos(lpUIPrivate->hInputWnd,NULL,pt.x,pt.y,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
	}

	ShowWindow( lpUIPrivate->hInputWnd, nShowCmd);
	GlobalUnlock(hUIPrivate);
	return;
}

/**********************************************************************/
/* UIWnd_GetStatusWnd                                                         */
/* Return Value :                                                     */
/*      window handle of candidatte                                   */
/**********************************************************************/
HWND PASCAL UIWnd_GetStatusWnd(
															 HWND hUIWnd)                // UI window
{
	HGLOBAL  hUIPrivate;
	LPUIPRIV lpUIPrivate;
	HWND     hWnd;

	hUIPrivate = (HGLOBAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
	if (!hUIPrivate) 
	{          // can not darw candidate window
		return (HWND)NULL;
	}

	lpUIPrivate = (LPUIPRIV)GlobalLock(hUIPrivate);
	if (!lpUIPrivate) 
	{         // can not draw candidate window
		return (HWND)NULL;
	}

	hWnd = lpUIPrivate->hStatusWnd;

	GlobalUnlock(hUIPrivate);
	return (hWnd);
}

/**********************************************************************/
/* UIWnd_GetInputWnd                                                         */
/* Return Value :                                                     */
/*      window handle of candidatte                                   */
/**********************************************************************/
HWND PASCAL UIWnd_GetInputWnd(
															HWND hUIWnd)                // UI window
{
	HGLOBAL  hUIPrivate;
	LPUIPRIV lpUIPrivate;
	HWND     hWnd;

	hUIPrivate = (HGLOBAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
	if (!hUIPrivate) 
	{
		return (HWND)NULL;
	}

	lpUIPrivate = (LPUIPRIV)GlobalLock(hUIPrivate);
	if (!lpUIPrivate) 
	{ 
		return (HWND)NULL;
	}

	hWnd = lpUIPrivate->hInputWnd;
	GlobalUnlock(hUIPrivate);
	return (hWnd);
}

//设置输入窗口位置
void UIWnd_SetInputWndPos(HWND hUIWnd,LPINPUTCONTEXT lpIMC)
{
	POINT pt=lpIMC->cfCompForm.ptCurrentPos;
	int	 hLineHei=16;
	HFONT hUIFont=(HFONT)SendMessage(lpIMC->hWnd,WM_GETFONT,0,0);
	ClientToScreen(lpIMC->hWnd,&pt);
	if(hUIFont)
	{
		HDC hDC = GetDC(lpIMC->hWnd);
		HFONT oldFont = (HFONT)SelectObject(hDC, hUIFont);
		SIZE sizeChar;
		GetTextExtentPoint(hDC, TEXT("A"),1,&sizeChar);
		SelectObject(hDC, oldFont);
		ReleaseDC(lpIMC->hWnd,hDC);
		hLineHei=sizeChar.cy;
	}
	lpIMC->dwReserve[0]=MAKELONG(pt.x,pt.y);	//将坐标保存在lpIMC->dwReserve[0],在显示输入窗口时再从它取数据
}


LONG UIWnd_Notify(HIMC hIMC, HWND hUIWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LONG lRet = 0L;
	LPINPUTCONTEXT lpIMC;

	if (!(lpIMC = ImmLockIMC(hIMC)))
	{		
		return 0L;
	}


	switch (wParam)
	{
	case IMN_OPENSTATUSWINDOW:
		UIWnd_OpenStatus(hUIWnd,TRUE);
		Helper_Trace(TEXT("UIWnd:IMN_OPENSTATUSWINDOW\n"));
		Helper_Trace(TEXT("UIWnd:UIWnd_CreateWaitWindow\n"));
		UIWnd_CreateWaitWindow(hUIWnd);		
		break;
	case IMN_CLOSESTATUSWINDOW:
		UIWnd_OpenStatus(hUIWnd,FALSE);
		Helper_Trace(TEXT("UIWnd:IMN_CLOSESTATUSWINDOW\n"));
		Helper_Trace(TEXT("UIWnd_CloseWaitWindow\n"));
		UIWnd_CloseWaitWindow(hUIWnd);
		break;
	case IMN_OPENCANDIDATE:
		UIWnd_OpenInput(hUIWnd,TRUE);
		break;
	case IMN_CLOSECANDIDATE:
		UIWnd_OpenInput(hUIWnd,FALSE);
		break;
	case IMN_CHANGECANDIDATE:
		break;
	case IMN_SETOPENSTATUS:
		break;
	case IMN_SETCONVERSIONMODE:
		break;
	case IMN_SETSENTENCEMODE:
		break;
	case IMN_SETCOMPOSITIONWINDOW:
		UIWnd_SetInputWndPos(hUIWnd,lpIMC);
		break;
	case IMN_SETSTATUSWINDOWPOS:
		break;
	default:
		break;
	}
	ImmUnlockIMC(hIMC);

	return lRet;
}

/**********************************************************************/
/* UIPaint()                                                          */
/**********************************************************************/
LRESULT PASCAL UIWnd_Paint(HWND   hUIWnd)
{
	PAINTSTRUCT ps;
	MSG sMsg;
	// for safety
	BeginPaint(hUIWnd, &ps);
	EndPaint(hUIWnd, &ps);

	// some application will not remove the WM_PAINT messages
	PeekMessage(&sMsg, hUIWnd, WM_PAINT, WM_PAINT, PM_REMOVE|PM_NOYIELD);
	return (0L);
}

/**********************************************************************/
/* UIWnd_Create()                                                   */
/**********************************************************************/
void PASCAL UIWnd_Create( HWND hUIWnd)
{
	HGLOBAL hUIPrivate;

	// create storage for UI setting
	hUIPrivate = GlobalAlloc(GHND, sizeof(UIPRIV));
	if (!hUIPrivate) 
	{     
		return;
	}

	SetWindowLong(hUIWnd, IMMGWL_PRIVATE, (LONG)hUIPrivate);	
	SetWindowPos(hUIWnd, NULL, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER);
	ShowWindow(hUIWnd, SW_SHOWNOACTIVATE);

	return;
}
void PASCAL UIWnd_CreateWaitWindow( HWND hUIWnd)
{	
	HGLOBAL        hUIPrivate;
	LPUIPRIV       lpUIPrivate;
	HIMC           hIMC;
	LPINPUTCONTEXT lpIMC;	

	hUIPrivate = (HGLOBAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
	if (!hUIPrivate)           // can not darw status window
	{
		return;
	}
	lpUIPrivate = (LPUIPRIV)GlobalLock(hUIPrivate);
	if (!lpUIPrivate) 
	{
		return;
	}
	if(!lpUIPrivate->m_hHWPenWnd)
	{
		lpUIPrivate->m_hHWPenWnd = CreateWindowEx(0, CLASSNAME_HWPEN, HWX_WINDOWNAME, WS_POPUP | WS_DISABLED,
			0, 0,	200, 200, hUIWnd, (HMENU)NULL, g_hInst, NULL);
		SetWindowLong(lpUIPrivate->m_hHWPenWnd, FIGWL_SVRWND,(DWORD)hUIWnd);
	}

	ShowWindow(lpUIPrivate->m_hHWPenWnd, SW_SHOWNOACTIVATE);

	GlobalUnlock(hUIPrivate);
	return;
}
void PASCAL UIWnd_CloseWaitWindow( HWND hUIWnd)
{	
	HGLOBAL        hUIPrivate;
	LPUIPRIV       lpUIPrivate;
	HIMC           hIMC;
	LPINPUTCONTEXT lpIMC;
	POINT	 pt;	

	hUIPrivate = (HGLOBAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
	if (!hUIPrivate)           // can not darw status window
	{
		return;
	}
	lpUIPrivate = (LPUIPRIV)GlobalLock(hUIPrivate);
	if (!lpUIPrivate) 
	{
		return;
	}
	if(lpUIPrivate->m_hHWPenWnd)
	{
		DestroyWindow(lpUIPrivate->m_hHWPenWnd);
		lpUIPrivate->m_hHWPenWnd = NULL;
	}


	GlobalUnlock(hUIPrivate);
	return;
}
/**********************************************************************/
/* DestroyUIWindow()                                                  */
/**********************************************************************/
void PASCAL UIWnd_Destroy(HWND hUIWnd)
{
	HGLOBAL  hUIPrivate;
	LPUIPRIV lpUIPrivate;

	hUIPrivate = (HGLOBAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
	if (!hUIPrivate) 
	{     
		return;
	}

	lpUIPrivate = (LPUIPRIV)GlobalLock(hUIPrivate);
	if (!lpUIPrivate) 
	{    
		return;
	}	
	if (lpUIPrivate->hStatusWnd) 
	{
		DestroyWindow(lpUIPrivate->hStatusWnd);
	}
	if (lpUIPrivate->hInputWnd) 
	{
		DestroyWindow(lpUIPrivate->hInputWnd);
	}
	if (lpUIPrivate->m_hHWPenWnd)
	{
		DestroyWindow(lpUIPrivate->m_hHWPenWnd);
		lpUIPrivate->m_hHWPenWnd = NULL;
	}

	GlobalUnlock(hUIPrivate);

	
	GlobalFree(hUIPrivate);

	return;
}

LRESULT WINAPI UIWndProc(HWND hUIWnd,	UINT message,WPARAM wParam,	LPARAM lParam)
{
	LONG           lRet = 0L;
	HIMC hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
	if (!hIMC)
	{
		if(Helper_IsIMEMessage(message)) 
		{
			return 0;
		}
	}

	switch (message)
	{
	case WM_CREATE://做窗口的初始化工作
		UIWnd_Create(hUIWnd);
		break;
	case WM_DESTROY:
		UIWnd_Destroy(hUIWnd);
		break;
	case WM_IME_NOTIFY:
		lRet = UIWnd_Notify(hIMC, hUIWnd,message,wParam,lParam);
		break; 
	case WM_PAINT:
		lRet= UIWnd_Paint(hUIWnd);
		break;
	case WM_IME_STARTCOMPOSITION:
		UIWnd_OpenInput(hUIWnd,TRUE);
		break;
	case WM_IME_ENDCOMPOSITION:
		UIWnd_OpenInput(hUIWnd,FALSE);
		break;
	case WM_IME_COMPOSITION:
		if(lParam & GCS_COMPSTR) 
		{
			InvalidateRect(UIWnd_GetInputWnd(hUIWnd),NULL,FALSE);
		}
		break;		
	case WM_IME_SETCONTEXT://更新当前的状态
		{			
			Helper_Trace(TEXT("UIWnd:WM_IME_SETCONTEXT\n"));
			if (wParam)
			{
				Helper_Trace(TEXT("UIWnd:UIWnd_CreateWaitWindow\n"));
				UIWnd_CreateWaitWindow(hUIWnd);
			}
			else
			{
				Helper_Trace(TEXT("UIWnd:UIWnd_CloseWaitWindow\n"));
				UIWnd_CloseWaitWindow(hUIWnd);
			}
			
		}
		break;
	case WM_MOUSEACTIVATE:
		return MA_NOACTIVATE;
	default:
		return DefWindowProc(hUIWnd,message,wParam,lParam);
	}
	return lRet;
}

//除uiWnd外所有窗口处理
//提示信息显示在编码窗口中
//输入法创建的所有窗口（不含对话框）都应该是WS_DISABLE的，也就是说不能响应鼠标消息
//为了响应鼠标消息需要在WM_SETCURSOR中获取鼠标按钮状态来模拟
#include "stdafx.h"
#include "HWPenIme.h"



BOOL WINAPI AboutDialogProc(HWND hWnd ,	UINT message ,WPARAM wParam ,LPARAM lParam )
{

	switch(message) 
	{
	case WM_INITDIALOG:
		{
			Helper_CenterWindow(hWnd);
		}
		return TRUE;
	case WM_COMMAND: 
		switch(wParam)
		{ 
			break;
		case IDOK:
		case IDCANCEL:
			EndDialog(hWnd,TRUE);
			break;
		default:
			return FALSE;
			break;
		}
		break;
	default:
		return FALSE;
		break;
	}
	return TRUE;
}

LRESULT WINAPI InputWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	LRESULT lRet=0;
	switch(message)
	{
	case WM_CREATE:
		{
			RECT rc;
			GetClientRect(hWnd,&rc);
			rc.left+=10;
			rc.bottom/=2;
			CreateWindow(CLSNAME_COMP,NULL,WS_CHILD|WS_VISIBLE,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,hWnd,(HMENU)0,g_hInst,NULL);
			OffsetRect(&rc,0,rc.bottom);
			CreateWindow(CLSNAME_CAND,NULL,WS_CHILD|WS_VISIBLE,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,hWnd,(HMENU)0,g_hInst,NULL);
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc=BeginPaint(hWnd,&ps);
			RECT rc;
			HBRUSH hr=CreateSolidBrush(RGB(255,0,0));
			GetClientRect(hWnd,&rc);
			rc.right=rc.left+10;
			FillRect(hdc,&rc,hr);
			DeleteObject(hr);
			EndPaint(hWnd,&ps);
		}
		break;
	default:
		if(!Helper_IsIMEMessage(message))
			lRet=DefWindowProc(hWnd,message,wParam,lParam);
		break;
	}
	return lRet;
}

LRESULT WINAPI CandWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	LRESULT lRet=0;
	switch(message)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc=BeginPaint(hWnd,&ps);
			RECT rc;
			GetClientRect(hWnd,&rc);
			FillRect(hdc, &rc, (HBRUSH)GetStockObject(DKGRAY_BRUSH));
			TextOut(hdc,0,0, TEXT("Cand"),4);
			EndPaint(hWnd,&ps);
		}
		break;
	default:
		if(!Helper_IsIMEMessage(message))
			lRet=DefWindowProc(hWnd,message,wParam,lParam);
		break;
	}
	return lRet;
}

LRESULT WINAPI HWWindowWaitForInput(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	LRESULT lRet = 0;
	TCHAR athTitle[] = TEXT("HWXUE");
	switch(message)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc=BeginPaint(hWnd,&ps);
			RECT rc;
			GetClientRect(hWnd,&rc);
			FillRect(hdc, &rc, (HBRUSH)GetStockObject(DKGRAY_BRUSH));
			TextOut(hdc, 0, 0, athTitle,  _tcslen(athTitle));
			EndPaint(hWnd,&ps);
		}
		break;
	default:
		if (message == g_dwInputMessage)
		{	
			Helper_Trace(TEXT("GetTheMessage\n"));

			TRANSMSG GnMsg;
			LPCOMPOSITIONSTRING lpCompStr;
			LPINPUTCONTEXT lpIMC;
			LPTSTR lpConvStr,lpPreResultStr;
			LPCANDIDATEINFO lpCandInfo;
			LPCANDIDATELIST lpCandList;
			
			HWND hSvrWnd = (HWND)GetWindowLong(hWnd, FIGWL_SVRWND);
			
			if (hSvrWnd)
			{
				HIMC hIMC = (HIMC)GetWindowLong(hSvrWnd, IMMGWL_IMC);
				lpIMC = ImmLockIMC(hIMC);
				TCHAR athBuffer[10];
				//lpIMC->hCompStr = ImmReSizeIMCC(lpIMC->hCompStr, sizeof(athBuffer));
				lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);

				static TCHAR szSample[] = TEXT("启程软件");
				LPTSTR pResult = GETLPRESULTSTR(lpCompStr);
				_tcscpy(pResult, szSample);
				lpCompStr->dwResultStrLen = _tcslen(szSample);
				lpCompStr->dwCompStrLen = 0;			

				//lpConvStr = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.szConvCompStr;
				//lpPreResultStr = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.szPreResultStr;
				//_tcscpy_s(lpPreResultStr, MAXCOMPSIZE, lpConvStr);


				//_tcscpy(GETLPRESULTSTR(lpCompStr), athBuffer);
				//lpCompStr->dwResultStrLen = _tcslen(lpConvStr);			
				//lpCompStr->dwCompStrLen = 0;

				//_tcscpy_s(GETLPCANDSTR(lpCandList, 0), 3, _T(""));
				//_tcscpy_s(GETLPCANDSTR(lpCandList, 1), 3, _T(""));

				ImmUnlockIMCC(lpIMC->hCompStr);
				//ImmUnlockIMCC(lpIMC->hCandInfo);


				GenerateMessage(hIMC, WM_IME_STARTCOMPOSITION, 0, 0);					
				GenerateMessage(hIMC, WM_IME_COMPOSITION, 0, GCS_RESULTSTR | GCS_COMPSTR);						
				GenerateMessage(hIMC, WM_IME_ENDCOMPOSITION, 0, 0);
				ImmUnlockIMC(hIMC);
			}
			
			break;
		}
		else
		{
			if(!Helper_IsIMEMessage(message))
			{
				lRet=DefWindowProc(hWnd,message,wParam,lParam);
			}
		}		
		break;
	}
	return lRet;
}

LRESULT WINAPI CompWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	LRESULT lRet=0;
	switch(message)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc=BeginPaint(hWnd,&ps);
			RECT rc;
			HWND hUIWnd=GetWindow(GetParent(hWnd),GW_OWNER);
			HIMC hIMC=(HIMC)GetWindowLong(hUIWnd,IMMGWL_IMC);
			LPINPUTCONTEXT lpIMC=(LPINPUTCONTEXT)ImmLockIMC(hIMC);
			GetClientRect(hWnd,&rc);
			FillRect(hdc,&rc, (HBRUSH)GetStockObject(GRAY_BRUSH));
			if(lpIMC)
			{
				LPCOMPOSITIONSTRING lpCompStr=(LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);
				if(lpCompStr)
				{
					LPCTSTR pComp=GETLPCOMPSTR(lpCompStr);
					TextOut(hdc,0,1,pComp,lpCompStr->dwCompStrLen);
					ImmUnlockIMCC(lpIMC->hCompStr);
				}
				ImmUnlockIMC(hIMC);
			}
			EndPaint(hWnd,&ps);
		}
		break;
	default:
		if(!Helper_IsIMEMessage(message))
			lRet=DefWindowProc(hWnd,message,wParam,lParam);
		break;
	}
	return lRet;
}

void UIStatus_OnPaint(HWND hWnd,HDC hdc)
{
	RECT rc;
	//pic box
	GetClientRect(hWnd,&rc);
	Rectangle(hdc,rc.left,rc.top,rc.right,rc.bottom);
	DrawText(hdc, TEXT("IME编程模板") ,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	TextOut(hdc,1,1, TEXT("?"),1);
}

LRESULT WINAPI StatusWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	LRESULT lRet=0;
	switch(message)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc=BeginPaint(hWnd,&ps);
			UIStatus_OnPaint(hWnd,hdc);
			EndPaint(hWnd,&ps);
			lRet=1;
		}
		break;
	case WM_SETCURSOR:
		{//模拟鼠标消息
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hWnd,&pt);
			SendMessage(hWnd,HIWORD(lParam),0,MAKELONG(pt.x,pt.y));
		}
		break;
	case WM_LBUTTONDOWN:
		{
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUT), GetActiveWindow(), (DLGPROC)AboutDialogProc);
		}
		break;
	default:
		lRet=DefWindowProc(hWnd,message,wParam,lParam);
		break;
	}
	return lRet;
}


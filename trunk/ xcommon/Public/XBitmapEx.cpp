/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XBitmapEx.cpp			
文件内容:			Bitmap简单处理
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/08/04
*********************************************************************/
#include "StdAfx.h"
#include "XBitmapEx.h"


static COLORREF DarkenColor(COLORREF crColor, double dFactor)
{
	if (dFactor > 0.0 && dFactor <= 1.0)
	{
		BYTE red,green,blue,lightred,lightgreen,lightblue;
		red = GetRValue(crColor);
		green = GetGValue(crColor);
		blue = GetBValue(crColor);
		lightred = (BYTE)(red - (dFactor * red));
		lightgreen = (BYTE)(green - (dFactor * green));
		lightblue = (BYTE)(blue - (dFactor * blue));
		crColor = RGB(lightred, lightgreen, lightblue);
	} // if
	return crColor;
} // End of DarkenColor

HBITMAP WINAPI CreateGrayscaleBitmap(HBITMAP hBitmap, COLORREF crTrans)
{
	HBITMAP	hGrayBitmap = NULL;
	HDC	hMainDC = NULL, hMemDC1 = NULL, hMemDC2 = NULL;
	HBITMAP		hOldBmp1 = NULL, hOldBmp2 = NULL;
	
	BITMAP tBitmap;
	__try
	{
		if (!hBitmap)
		{
			__leave;
		}
		if (!(hMainDC = ::GetDC(NULL)))	
		{
			__leave;
		}
		if (!(hMemDC1 = ::CreateCompatibleDC(hMainDC)))
		{
			//::ReleaseDC(NULL, hMainDC);
			__leave;
		} // if
		if (!(hMemDC2 = ::CreateCompatibleDC(hMainDC)))
		{
			//::DeleteDC(hMemDC1);
			//::ReleaseDC(NULL, hMainDC);
			__leave;
		} // if		
		if (!GetObject(hBitmap, sizeof(tBitmap), &tBitmap))
		{
			__leave;
		}
		DWORD dwWidth = tBitmap.bmWidth, dwHeight = tBitmap.bmHeight;
		if (dwWidth <= 0 || dwHeight <= 0)
		{
			__leave;
		}
		hGrayBitmap = ::CreateCompatibleBitmap(hMainDC, dwWidth, dwHeight);
		if (hGrayBitmap)
		{
			hOldBmp1 = (HBITMAP)::SelectObject(hMemDC1, hGrayBitmap);
			hOldBmp2 = (HBITMAP)::SelectObject(hMemDC2, hBitmap);

			//::BitBlt(hMemDC1, 0, 0, dwWidth, dwHeight, hMemDC2, 0, 0, SRCCOPY);

			DWORD			dwLoopY = 0, dwLoopX = 0;
			COLORREF	crPixel = 0;
			BYTE			byNewPixel = 0;

			for (dwLoopY = 0; dwLoopY < dwHeight; dwLoopY++)
			{
				for (dwLoopX = 0; dwLoopX < dwWidth; dwLoopX++)
				{
					crPixel = ::GetPixel(hMemDC2, dwLoopX, dwLoopY);
					byNewPixel = (BYTE)((GetRValue(crPixel) * 0.299) + (GetGValue(crPixel) * 0.587) + (GetBValue(crPixel) * 0.114));

					if (crPixel != crTrans)
					{
						::SetPixel(hMemDC1, dwLoopX, dwLoopY, RGB(byNewPixel, byNewPixel, byNewPixel));
					}
					else
					{
						::SetPixel(hMemDC1, dwLoopX, dwLoopY, crPixel);
					}
				} // for
			} // for

			::SelectObject(hMemDC1, hOldBmp1);
			::SelectObject(hMemDC2, hOldBmp2);
		} // if
	}
	__finally
	{
		if (hMemDC1)
		{
			::DeleteDC(hMemDC1);
		}
		if (hMemDC2)
		{
			::DeleteDC(hMemDC2);
		}
		if (hMainDC)
		{
			::ReleaseDC(NULL, hMainDC);		
		}		
	}		
	return hGrayBitmap;
} 


HBITMAP WINAPI CreateDarkerBitmap(HBITMAP hBitmap, COLORREF crTrans)
{
	HBITMAP		hGrayBitmap = NULL;
	HDC			hMainDC = NULL, hMemDC1 = NULL, hMemDC2 = NULL;
	HBITMAP		hOldBmp1 = NULL, hOldBmp2 = NULL;
	__try
	{		
		if (!(hMainDC = ::GetDC(NULL)))	
		{
			__leave;
		}		
		if(!(hMemDC1 = ::CreateCompatibleDC(hMainDC)))
		{
			//::ReleaseDC(NULL, hMainDC);
			__leave;
		} // if
		if (!(hMemDC2 = ::CreateCompatibleDC(hMainDC)))
		{
			//::DeleteDC(hMemDC1);
			//::ReleaseDC(NULL, hMainDC);
			//return NULL;
			__leave;
		} // if
		BITMAP tBitmap;
		if (!(::GetObject(hBitmap, sizeof(tBitmap), &tBitmap)))
		{
			__leave;
		}
		DWORD dwWidth = tBitmap.bmWidth, dwHeight = tBitmap.bmHeight;
		if (dwWidth <= 0 || dwHeight <= 0)
		{
			__leave;
		}
		if (hGrayBitmap = ::CreateCompatibleBitmap(hMainDC, dwWidth, dwHeight))
		{
			hOldBmp1 = (HBITMAP)::SelectObject(hMemDC1, hGrayBitmap);
			hOldBmp2 = (HBITMAP)::SelectObject(hMemDC2, hBitmap);
			//::BitBlt(hMemDC1, 0, 0, dwWidth, dwHeight, hMemDC2, 0, 0, SRCCOPY);

			DWORD		dwLoopY = 0, dwLoopX = 0;
			COLORREF	crPixel = 0;

			for (dwLoopY = 0; dwLoopY < dwHeight; dwLoopY++)
			{
				for (dwLoopX = 0; dwLoopX < dwWidth; dwLoopX++)
				{
					crPixel = ::GetPixel(hMemDC2, dwLoopX, dwLoopY);

					if (crPixel != crTrans)
						::SetPixel(hMemDC1, dwLoopX, dwLoopY, DarkenColor(crPixel, 0.25));
					else
						::SetPixel(hMemDC1, dwLoopX, dwLoopY, crPixel);
				} // for
			} // for

			::SelectObject(hMemDC1, hOldBmp1);
			::SelectObject(hMemDC2, hOldBmp2);
		} // if
	}
	__finally
	{
		if (hMemDC1)
		{
			::DeleteDC(hMemDC1);
			hMemDC1 = NULL;
		}
		if (hMemDC2)
		{
			::DeleteDC(hMemDC2);
			hMemDC2 = NULL;
		}
		if (hMainDC)
		{
			::ReleaseDC(NULL, hMainDC);
		}		
	}	
	return hGrayBitmap;
} 


HBITMAP WINAPI CreateBitmapMask(HBITMAP hBitmap,  COLORREF crTransColor)
{
	HBITMAP		hMask			= NULL;
	HDC				hdcSrc		= NULL;
	HDC				hdcDest		= NULL;
	HBITMAP		hbmSrcT		= NULL;
	HBITMAP		hbmDestT	= NULL;
	COLORREF	crSaveBk;
	__try
	{
		BITMAP tBitmap;
		if (!(::GetObject(hBitmap, sizeof(tBitmap), &tBitmap)))
		{
			__leave;
		}
		DWORD dwWidth = tBitmap.bmWidth, dwHeight = tBitmap.bmHeight;
		if (dwWidth <= 0 || dwHeight <= 0)
		{
			__leave;
		}
		if (!(hMask = ::CreateBitmap(dwWidth, dwHeight, 1, 1, NULL)))
		{
			__leave;
		}
		
		hdcSrc	= ::CreateCompatibleDC(NULL);
		hdcDest	= ::CreateCompatibleDC(NULL);

		hbmSrcT = (HBITMAP)::SelectObject(hdcSrc, hBitmap);
		hbmDestT = (HBITMAP)::SelectObject(hdcDest, hMask);

		crSaveBk = ::SetBkColor(hdcSrc, crTransColor);

		::BitBlt(hdcDest, 0, 0, dwWidth, dwHeight, hdcSrc, 0, 0, SRCCOPY);
		//crSaveDestText = ::SetTextColor(hdcSrc, RGB(255, 255, 255));
		//::SetBkColor(hdcSrc, RGB(0, 0, 0));
		::BitBlt(hdcSrc, 0, 0, dwWidth, dwHeight, hdcDest, 0, 0, SRCINVERT);


		::SetBkColor(hdcSrc, crSaveBk);
		//::SetTextColor(hdcSrc, crSaveDestText);

		::SelectObject(hdcSrc, hbmSrcT);
		::SelectObject(hdcDest, hbmDestT);
	}
	__finally
	{
		if (hdcSrc)
		{
			::DeleteDC(hdcSrc);
			hdcSrc = NULL;
		}
		if (hdcDest)
		{
			::DeleteDC(hdcDest);
			hdcDest = NULL;
		}		
	}
	return hMask;
} 
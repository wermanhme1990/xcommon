#include "stdafx.h"
#include "XBitmap.h"
#include "Ext_Memory.h"
#include <ocidl.h>
#include <olectl.h>
CXBitmap::CXBitmap() : m_hBitmap(NULL)
{

}
CXBitmap::CXBitmap(const HBITMAP& hBitmap)
{
	_ASSERT(hBitmap);  
	m_hBitmap = (HBITMAP)CopyImage((HANDLE)hBitmap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);	
	AssertValid();
}
CXBitmap::CXBitmap(const CXBitmap& tBitmap)
{
	_ASSERT(tBitmap.m_hBitmap);
	m_hBitmap = (HBITMAP)CopyImage((HANDLE)tBitmap.m_hBitmap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
}
CXBitmap::CXBitmap(HDC hDC, const LONG& dx, const LONG& dy) : m_hBitmap(NULL)
{
	CreateCompatible(hDC, dx, dy);
}
CXBitmap::~CXBitmap()
{	
	Free();
}
BOOL CXBitmap::Attach( HBITMAP hBitmap)
{
	BOOL blRet = FALSE;
	if (m_hBitmap = hBitmap)
	{    
		blRet = TRUE;
	}  
	return blRet;
}
HBITMAP CXBitmap::Detach()
{
	HBITMAP hBitmap = m_hBitmap;
	m_hBitmap = NULL;  
	return hBitmap;
}

BOOL CXBitmap::Load (HINSTANCE hInst, TCHAR const * pthResName)
{  
	Free();	
	m_hBitmap = (HBITMAP) ::LoadImage (hInst, pthResName,	IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	return NULL != m_hBitmap; 
}
// Load a CXBitmap from a resource
BOOL CXBitmap::Load(HINSTANCE hInst, LONG nID)
{
	Free();	
	m_hBitmap = (HBITMAP) ::LoadImage (hInst, MAKEINTRESOURCE (nID),IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	return NULL != m_hBitmap;
}

BOOL CXBitmap::Load(const TCHAR *const pthPath)
{
	Free();
	m_hBitmap = (HBITMAP) ::LoadImage (0, pthPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);	
	return NULL != m_hBitmap;
}

void CXBitmap::GetSize (LONG & nWidth, LONG & nHeight) const
{	
	CXSize tSize = GetSize();
	nWidth = tSize.cx;
	nHeight = tSize.cy;
}
CXSize CXBitmap::GetSize() const
{
	CXSize tSize(0, 0);
	BITMAP tBitmap;
	if (m_hBitmap && GetObject(m_hBitmap, sizeof(tBitmap), &tBitmap))
	{
		tSize.cx = tBitmap.bmWidth;
		tSize.cy = tBitmap.bmHeight;
	}
	return tSize;
}



void CXBitmap::CreateCompatible(HDC hDC, const LONG& nWidth, const LONG& nHeight)
{
	Free();
	m_hBitmap = ::CreateCompatibleBitmap (hDC, nWidth, nHeight);
}
void CXBitmap::Free()
{
	if (m_hBitmap)
	{
		::DeleteObject (m_hBitmap);
		m_hBitmap = NULL;
	}	
}

COLORREF CXBitmap::DarkenColor(COLORREF crColor, double dFactor)
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
	} 
	return crColor;
}

HBITMAP CXBitmap::CreateGrayBitmap(HBITMAP hBitmap, COLORREF crTrans)
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

HBITMAP CXBitmap::CreateGrayBitmap( COLORREF crTrans )
{
	return CreateGrayBitmap(m_hBitmap, crTrans);
}

HBITMAP CXBitmap::CreateDarkerBitmap(HBITMAP hBitmap, COLORREF crTrans)
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

HBITMAP CXBitmap::CreateDarkerBitmap( COLORREF crTrans )
{
	return CreateDarkerBitmap(m_hBitmap , crTrans);
}

HBITMAP CXBitmap::CreateBitmapMask(HBITMAP hBitmap,  COLORREF crTransColor)
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

HBITMAP CXBitmap::CreateBitmapMask( COLORREF crTransColor )
{
	return CreateBitmapMask(m_hBitmap, crTransColor);
}

CXBitmap::operator HBITMAP()
{
	return m_hBitmap; 
}
void CXBitmap::operator=(const CXBitmap & tBmp)
{  
	Free();    
	if (tBmp.m_hBitmap)
	{
		m_hBitmap = (HBITMAP)CopyImage((HANDLE)tBmp.m_hBitmap, 
			IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	}
}
BOOL CXBitmap::AssertValid() const
{	
	BOOL blRet = TRUE;
#if defined(DEBUG) || defined(_DEBUG)
	{
		if (m_hBitmap)
		{
			BITMAP tBitmap;
			LONG nBit = GetObject(m_hBitmap, sizeof(tBitmap), &tBitmap);
			assert(nBit > 0);
			assert(tBitmap.bmBits);		
		}		
	}
#endif
	return blRet;
}
// load *.bmp, *.jpg, *.gif, *.ico, *.emf, or *.wmf files
BOOL CXBitmap::LoadImage( LPCTSTR szFile)
{
	LPPICTURE gpPicture = NULL;	
	DWORD dwFileSize = 0;
	HANDLE hFile = NULL;
	HGLOBAL hGlobal = NULL;	
	OLE_HANDLE m_picHandle;
	LPVOID pvData = NULL;
	LPSTREAM pIStream = NULL; 
	HRESULT hResult;
	HDC hDC = NULL;
	__try
	{
		Free();

		if(INVALID_HANDLE_VALUE == (hFile = CreateFile(szFile, GENERIC_READ, 
			0, NULL, OPEN_EXISTING, 0, NULL)))
		{		
			hFile = NULL;
			__leave;
		}
		// get file size
		dwFileSize = GetFileSize(hFile, NULL);
		if(INVALID_FILE_SIZE == dwFileSize)
		{
			dwFileSize = 0;
			__leave;
		}

		if(!(hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwFileSize)))
		{
			__leave;
		}		
		VERIFY(pvData = GlobalLock(hGlobal));

		DWORD dwBytesRead = 0;		
		BOOL bRead = ReadFile(hFile, pvData, dwFileSize, &dwBytesRead, NULL);
		assert(FALSE != bRead);

		GlobalUnlock(hGlobal);
		CloseHandle(hFile);
		hFile = NULL;

		// create IStream* from global memory
		hResult = CreateStreamOnHGlobal(hGlobal, TRUE, &pIStream);

		if (!(SUCCEEDED(hResult) && pIStream))
		{
			__leave;
		}		

		// Create IPicture from image file
		hResult = ::OleLoadPicture(pIStream, dwFileSize, FALSE, IID_IPicture, (LPVOID *)&gpPicture);
		if (!(SUCCEEDED(hResult) && gpPicture))
		{
			__leave;
		}

		pIStream->Release();		
		pIStream = NULL;

		gpPicture->get_Handle(&m_picHandle);

		hDC = CreateCompatibleDC(NULL);
		if (!hDC)
		{
			__leave;
		}
		SelectObject(hDC, (HGDIOBJ) m_picHandle);
		m_hBitmap = (HBITMAP)GetCurrentObject(hDC, OBJ_BITMAP);

		DeleteDC(hDC);
		hDC = NULL;
	}
	__finally
	{
		if (!hFile)
		{
			CloseHandle(hFile);
			hFile = NULL;
		}		
		if (hDC)
		{
			DeleteDC(hDC);
			hDC = NULL;
		}
		if (pIStream)
		{
			pIStream->Release();
			pIStream = NULL;
		}
	}

	return TRUE;
}


HBITMAP CXBitmap::Clip(const LONG& xSrc, const LONG& ySrc, 
											 const LONG& nWidth, const LONG& nHeight) const
{
	HBITMAP hRet = NULL;
	HDC hDC = NULL;
	HDC hDCDest = NULL;
	__try
	{
		if (!m_hBitmap)
		{
			__leave;
		}
		hDC = ::CreateCompatibleDC(NULL);
		::SelectObject(hDC, m_hBitmap);

		hDCDest = ::CreateCompatibleDC(hDC);		
		hRet = ::CreateCompatibleBitmap(hDC, nWidth, nHeight);

		::SelectObject(hDCDest, hRet);

		::BitBlt(hDCDest, 0, 0, nWidth, nHeight, hDC, xSrc, ySrc, SRCCOPY);	
	}
	__finally
	{
		if (hDCDest)
		{
			::DeleteDC(hDCDest);
			hDCDest = NULL;
		}
		if (hDC)
		{
			::DeleteDC(hDC);
			hDC = NULL;
		}
	}	
	return hRet;
}
HBITMAP CXBitmap::Clip( const CXRect& rtClip) const
{
	return Clip(rtClip.left, rtClip.top, rtClip.Width(), rtClip.Height());
}


HRGN CXBitmap::BitmapToRegion (HBITMAP hBmp, COLORREF cTransparentColor, 
															 COLORREF cTolerance)
{
	HRGN hRgn = NULL;
	HDC hMemDC = NULL, hDC = NULL;
	HBITMAP hbm32 = NULL, hOldBmpDC = NULL, hOldBmpMemDC = NULL;
	HANDLE hData = NULL;
	BYTE lr = GetRValue(cTransparentColor);
	BYTE lg = GetGValue(cTransparentColor);
	BYTE lb = GetBValue(cTransparentColor);
	BYTE hr = min(0xff, lr + GetRValue(cTolerance));
	BYTE hg = min(0xff, lg + GetGValue(cTolerance));
	BYTE hb = min(0xff, lb + GetBValue(cTolerance));
	__try
	{
		if (!hBmp)
		{
			__leave;
		}
		// Create a memory DC inside which we will scan the bitmap content
		if(!(hMemDC = ::CreateCompatibleDC(NULL)))
		{
			__leave;
		}	
		// Get bitmap size
		BITMAP tBmpBuff;
		::GetObject(hBmp, sizeof(tBmpBuff), &tBmpBuff);

		// Create a 32 bits depth bitmap and select it into the memory DC 
		BITMAPINFOHEADER RGB32BITSBITMAPINFO = 
		{	
			sizeof(BITMAPINFOHEADER),	// biSize 
			tBmpBuff.bmWidth,					// biWidth; 
			tBmpBuff.bmHeight,				// biHeight; 
			1,							// biPlanes; 
			32,							// biBitCount 
			BI_RGB,					// biCompression; 
			0,							// biSizeImage; 
			0,							// biXPelsPerMeter; 
			0,							// biYPelsPerMeter; 
			0,							// biClrUsed; 
			0								// biClrImportant; 
		};
		VOID * pbits32; 
		if(!(hbm32 = ::CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, 
			DIB_RGB_COLORS, &pbits32, NULL, 0)))
		{
			__leave;
		}		
		hOldBmpMemDC = (HBITMAP)::SelectObject(hMemDC, hbm32);

		// Create a DC just to copy the bitmap into the memory DC
		if (!(hDC = ::CreateCompatibleDC(hMemDC)))
		{
			__leave;
		}	

		// Get how many bytes per row we have for the bitmap bits (rounded up to 32 bits)
		BITMAP bm32;
		::GetObject(hbm32, sizeof(bm32), &bm32);
		while (bm32.bmWidthBytes % 4)
		{
			bm32.bmWidthBytes ++;
		}

		// Copy the bitmap into the memory DC
		hOldBmpDC = (HBITMAP)::SelectObject(hDC, hBmp);
		::BitBlt(hMemDC, 0, 0, tBmpBuff.bmWidth, tBmpBuff.bmHeight, hDC, 0, 0, SRCCOPY);

		// For better performances, we will use the ExtCreateRegion() function to create the
		// region. This function take a RGNDATA structure on entry. We will add rectangles by
		// amount of ALLOC_UNIT number in this structure.
#define ALLOC_UNIT	100
		DWORD maxRects = ALLOC_UNIT;
		if(!(hData = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects))))
		{
			__leave;
		}
		RGNDATA *pData = (RGNDATA *)::GlobalLock(hData);
		pData->rdh.dwSize = sizeof(RGNDATAHEADER);
		pData->rdh.iType = RDH_RECTANGLES;
		pData->rdh.nCount = pData->rdh.nRgnSize = 0;
		::SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

		// Keep on hand highest and lowest values for the "transparent" pixels

		// Scan each bitmap row from bottom to top (the bitmap is inverted vertically)
		BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
		for (LONG y = 0; y < tBmpBuff.bmHeight; y ++)
		{
			// Scan each bitmap pixel from left to right
			for (LONG x = 0; x < tBmpBuff.bmWidth; x ++)
			{
				// Search for a continuous range of "non transparent pixels"
				LONG x0 = x;
				LONG *p = (LONG *)p32 + x;
				while (x < tBmpBuff.bmWidth)
				{
					BYTE b = GetRValue(*p);
					if (b >= lr && b <= hr)
					{
						b = GetGValue(*p);
						if (b >= lg && b <= hg)
						{
							b = GetBValue(*p);
							if (b >= lb && b <= hb)
								// This pixel is "transparent"
								break;
						}
					}
					p ++;
					x ++;
				}

				if (x > x0)
				{
					// Add the pixels (x0, y) to (x, y+1) as a new rectangle in the region
					if (pData->rdh.nCount >= maxRects)
					{
						::GlobalUnlock(hData);
						maxRects += ALLOC_UNIT;
						hData = ::GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), 
							GMEM_MOVEABLE);
						pData = (RGNDATA *)::GlobalLock(hData);
					}
					RECT *pr = (RECT *)&pData->Buffer;
					::SetRect(&pr[pData->rdh.nCount], x0, y, x, y + 1);
					if (x0 < pData->rdh.rcBound.left)
						pData->rdh.rcBound.left = x0;
					if (y < pData->rdh.rcBound.top)
						pData->rdh.rcBound.top = y;
					if (x > pData->rdh.rcBound.right)
						pData->rdh.rcBound.right = x;
					if (y+1 > pData->rdh.rcBound.bottom)
						pData->rdh.rcBound.bottom = y + 1;
					pData->rdh.nCount ++;

					// On Windows98, ExtCreateRegion() may fail if the number of rectangles is too
					// large (ie: > 4000). Therefore, we have to create the region by multiple steps.
					if (pData->rdh.nCount == 2000)
					{
						HRGN h = ::ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), 
							pData);
						if (hRgn)
						{
							::CombineRgn(hRgn, hRgn, h, RGN_OR);
							::DeleteObject(h);
						}
						else
						{
							hRgn = h;
						}
						pData->rdh.nCount = 0;
						::SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
					}
				}
			}
			// Go to next row (remember, the bitmap is inverted vertically)
			p32 -= bm32.bmWidthBytes;
		}

		// Create or extend the region with the remaining rectangles
		HRGN h = ::ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
		if (hRgn)
		{
			::CombineRgn(hRgn, hRgn, h, RGN_OR);
			::DeleteObject(h);
		}
		else
		{
			hRgn = h;
		}	

		//::DeleteObject(::SelectObject(hMemDC, hOldBmp));	

	}
	__finally
	{
		if (hOldBmpDC && hDC)
		{
			::SelectObject(hDC, hOldBmpDC);
			hOldBmpDC = NULL;
		}
		if (hOldBmpMemDC && hMemDC)
		{
			::SelectObject(hMemDC, hOldBmpMemDC);
			hOldBmpMemDC = NULL;
		}
		if (hbm32)
		{
			::DeleteObject(hbm32);
			hbm32 = NULL;
		}
		if (hData)
		{
			::GlobalFree(hData);
			hData = NULL;
		}
		if (hDC)
		{
			::DeleteDC(hDC);
			hDC = NULL;
		}
		if (hMemDC)
		{
			::DeleteDC(hMemDC);		
			hMemDC = NULL;
		}
	}
	return hRgn;
}
HRGN CXBitmap::BitmapToRegion( COLORREF cTransparentColor, COLORREF cTolerance /*= 0x101010*/ )
{
	return BitmapToRegion(m_hBitmap, cTransparentColor, cTolerance);
}
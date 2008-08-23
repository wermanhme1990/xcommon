// CImage.cpp : implementation file

// Transparent draw function by Troels K.

/*
 * File:	cimage.h
 * Purpose:	Declaration of the Platform Independent Image Class
 * Author:	Alejandro Aguilar Sierra
 * Created:	1995
 * Copyright:	(c) 1995, Alejandro Aguilar Sierra <asierra@servidor.unam.mx>
 *
 * This software is based in part on the work of the Independent JPEG Group.
 *
 */

/** 07/08/2001 v1.00
 * - ing.davide.pizzolato@libero.it
 ** version 1.01 24/Aug/2001
 *  - Mirror bug fix <Brian R. Zielinski>
 *  + GrayScale function <Troels K.>
 *	+ Rotate function <Ulrich von Zadow>
 */

#include "..\include\cimage.h"
#pragma hdrstop

#include <math.h>

#include "imajpeg.h"
#include "imagif.h"
#include "imapng.h"
#include "imabmp.h"
#include "imaicon.h"
#include "imatiff.h"

/////////////////////////////////////////////////////////////////////
// CImageBase

CImageBase::CImageBase(void) : m_handle(NULL)
{
}

CImageBase::~CImageBase()
{
   if(0)if (IsOpen())
   {
      Close();
   }
}

void CImageBase::Close(void)
{
	free(m_handle); m_handle = NULL;
}

/////////////////////////////////////////////////////////////////////
// CImage 

void CImage::Startup()
{
	memset(&m_header,0,sizeof(m_header));
   m_header.biSize = sizeof(m_header);
	memset(&m_info,0,sizeof(m_info));
   m_info.nQuality=75;
	m_info.nBkgndIndex=-1;
   m_info.dwFileType = ENUM_cimage_format_empty;
}

CImage::CImage(enum cximage_type type) : CImageBase()
{
	Startup();

   m_info.dwFileType = type;
}

CImage::CImage()
{
	Startup();
}

CImage::CImage(DWORD dwWidth, DWORD dwHeight, WORD wBpp) : CImageBase()
{
	Startup();
	Create(dwWidth,dwHeight,wBpp);
}

CImage::CImage(const CImage& src) : CImageBase()
{
	Startup();
   Copy(src);
}

CImage::CImage(HANDLE hMem) : CImageBase()
{
	Startup();
	BYTE *lpVoid;
	BITMAPINFOHEADER *pHead;
	lpVoid = (BYTE *)GlobalLock(hMem);
	pHead = (BITMAPINFOHEADER *)lpVoid;
	if (lpVoid){
		memcpy(&m_header,pHead,sizeof(BITMAPINFOHEADER));
		if(pHead->biCompression != BI_RGB)
      {
			GlobalUnlock(lpVoid);
			return;
		}
		if ((pHead->biBitCount >= 15)&&(pHead->biBitCount != 24))
      {
			GlobalUnlock(lpVoid);
			return;
		}
		if(!Create(m_header.biWidth,m_header.biHeight,m_header.biBitCount))
      {
			GlobalUnlock(lpVoid);
			return;
		}
		memcpy(m_handle,lpVoid,GetLength());
		GlobalUnlock(lpVoid);
		return;
	}
}

CImage::CImage(HBITMAP handle, int BPP) : CImageBase()
{
	Startup();
	if (NULL != handle) 
   { 
      BITMAP bm;
      GetObject(handle, sizeof(bm), &bm);
      if (-1 != BPP) bm.bmBitsPixel = BPP;
      Create(bm.bmWidth, bm.bmHeight, bm.bmBitsPixel);
      LPBITMAPINFO bmi = (LPBITMAPINFO)m_handle;
      HDC dc = ::GetDC(NULL);
      if (::GetDIBits(dc, handle, 0, m_header.biHeight, m_info.pImage, bmi, DIB_RGB_COLORS))
      {
		   SetPalette(((LPBITMAPINFO)m_handle)->bmiColors);
      }
      else
      {
         strcpy(m_info.szLastError,"GetDIBits failed");
      }
      ::ReleaseDC(NULL, dc); // remember this!
   }
}

CImage::CImage(HICON handle) : CImageBase()
{
	Startup();
	
   CImageIcon temp;
   if (temp.Open(handle))
   {
		Transfer(&temp);
	} 
   else 
   {
		strcpy(m_info.szLastError,temp.GetLastError());
	}
}

CImage::CImage(LPCSTR filename, enum cximage_type imagetype) : CImageBase()
{
	Startup();
	CreateFromFile(filename,imagetype);
}

CImage::CImage(LPCWSTR filename, enum cximage_type imagetype) : CImageBase()
{
	Startup();
	CreateFromFile(filename,imagetype);
}

CImage::~CImage()
{
	if (IsOpen())
   {
      Close();
   }
}

void CImage::Close(void)
{
	if ((NULL != m_handle) && (m_info.pGhost==NULL) )
   {
      CImageBase::Close();
   }
   m_handle = NULL;
}

CImage& CImage::operator=(const CImage& src)
{
	Copy(src);
	return *this;
}

BOOL CImage::Attach(const CImage& src)
{
	return Copy(src);
}

BOOL CImage::Detach(CImage* dst)
{
   BOOL bOK = dst->Copy(*this);
   if (bOK)
   {
      //Close();
   }
   return bOK;   
}

DWORD CImage::GetPaletteSize()
{
	return (m_header.biClrUsed * sizeof(RGBQUAD));
}

// returns the image bits
BYTE* CImage::GetBits()
{ 
	if (m_handle)	return ((BYTE*)m_handle + *(LPDWORD)m_handle + GetPaletteSize()); 
	return NULL;
}

void* CImage::Create(DWORD dwWidth, DWORD dwHeight, WORD wBpp)
{
	if (IsOpen()) Close();

    // Make sure bits per pixel is valid
    if		(wBpp <= 1)	wBpp = 1;
    else if (wBpp <= 4)	wBpp = 4;
    else if (wBpp <= 8)	wBpp = 8;
    else				wBpp = 24;

    switch (wBpp){
        case 1:
            m_header.biClrUsed = 2;	break;
        case 4:
            m_header.biClrUsed = 16; break;
        case 8:
            m_header.biClrUsed = 256; break;
        default:
            m_header.biClrUsed = 0;
    }

	m_info.bColorType = (wBpp>8) ? COLORTYPE_COLOR : COLORTYPE_PALETTE;
    m_info.dwEffWidth = ((((wBpp * dwWidth) + 31) / 32) * 4);

    // initialize BITMAPINFOHEADER
    m_header.biWidth = dwWidth;		// fill in width from parameter
    m_header.biHeight = dwHeight;	// fill in height from parameter
    m_header.biPlanes = 1;			// must be 1
    m_header.biBitCount = wBpp;		// from parameter
    //m_header.biCompression = BI_RGB;    TRK RLE OK !
    m_header.biSizeImage = m_info.dwEffWidth * dwHeight;
    m_header.biXPelsPerMeter = 0;
    m_header.biYPelsPerMeter = 0;
    m_header.biClrImportant = 0;

	m_handle = malloc(GetLength()); // alloc memory block to store our bitmap
    if (!m_handle) return NULL;

    // use our bitmap m_info structure to fill in first part of
    // our DIB with the BITMAPINFOHEADER
    LPBITMAPINFOHEADER  lpbi;
	lpbi = (LPBITMAPINFOHEADER)m_handle;
    *lpbi = m_header;

	m_info.pImage=GetBits();

   return m_handle; //return handle to the DIB
}

// If you want transparency for non-transparent file formats, eg. bmp. Palette files only.
BOOL CImage::SetBkColor(COLORREF crTransparent) 
{
	const BITMAPINFO* bmi = GetBitmapInfo();
   BOOL bOK = FALSE;
   for (int i = 0, count = bmi->bmiHeader.biClrUsed; i < count; i++)
   {
      RGBQUAD rgb = bmi->bmiColors[i];
      COLORREF crColor = RGB(rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue);
      if (crColor == crTransparent)
      {
         m_info.nBkgndIndex = i;
         bOK = TRUE;
         break;
      }
   }
   return bOK;
}

EXTERN_C BOOL WINAPI DrawTransparentBitmap(HDC hdc, HDC dcSrc, COLORREF crTransparent, int xDst, int yDst, int cxDst, int cyDst, int cxSrc, int cySrc);

BOOL CImage::DrawImplementation(HDC hdc, int xDst /*= 0*/, int yDst /*= 0*/, int cxDst /*= -1*/, int cyDst /*= -1*/, 
                           int xSrc /*= 0*/, int ySrc /*= 0*/, int cxSrc /*= -1*/, int cySrc /*= -1*/)
{
 	// TRK - added transparency
   if (cxDst == -1) cxDst = m_header.biWidth;
	if (cyDst == -1) cyDst = m_header.biHeight;
   if (cxSrc == -1) cxSrc = m_header.biWidth;
	if (cySrc == -1) cySrc = m_header.biHeight;
   BOOL bGDI = TRUE;
   BOOL bTransparent = (m_info.nBkgndIndex != -1);
	BOOL bOK = (NULL != m_handle) && (NULL != hdc);
	const BITMAPINFO* bmi = GetBitmapInfo();
   //bTransparent = FALSE;
   if (bOK && (!bTransparent))
   {
      bGDI = bGDI && SetStretchBltMode(hdc,COLORONCOLOR);
		bOK = StretchDIBits(hdc, xDst, yDst, cxDst, cyDst, xSrc, ySrc, cxSrc, cySrc, 
         m_info.pImage,bmi,DIB_RGB_COLORS,SRCCOPY);
   }
   else if (bOK)
   {
      const int safe = 1; // or else GDI fails in the following - sometimes 
      RECT rcDst;
      rcDst.left = xDst + safe;
      rcDst.top  = yDst + safe;
      rcDst.right = xDst + cxDst;
      rcDst.bottom = yDst + cyDst;
      if (RectVisible(hdc, &rcDst))
      {
         HDC dcMem = CreateCompatibleDC(NULL);
         HBITMAP bitmap = CreateCompatibleBitmap(hdc, cxSrc, cySrc);
         HGDIOBJ old = SelectObject(dcMem, bitmap);
		   bGDI = bGDI && SetStretchBltMode(dcMem,COLORONCOLOR);
		   bOK = StretchDIBits(dcMem, 0, 0, cxSrc, cySrc, xSrc, ySrc, cxSrc, cySrc, m_info.pImage,GetBitmapInfo(),DIB_RGB_COLORS,SRCCOPY);
         RGBQUAD rgb = bmi->bmiColors[GetInfo()->nBkgndIndex];
         bOK = ::DrawTransparentBitmap(hdc, dcMem, RGB(rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue), xDst, yDst, cxDst, cyDst, cxSrc, cySrc);
         SelectObject(dcMem, old); 
         bGDI = bGDI && DeleteDC(dcMem);
         bGDI = bGDI && DeleteObject(bitmap);
      }
   }
   return bOK;
}

BOOL CImage::Tile(HDC pDC, LPCRECT rc)
{
	BOOL bOK = ((m_handle)&&(pDC)&&(rc));
   if (bOK)
   {
		const int w = rc->right - rc->left;
		const int h = rc->bottom - rc->top;
		const int bx=m_header.biWidth;
		      int by=m_header.biHeight;
		for (int x, y = rc->top, z; y < h ; y += by)
      {
			if ((y+by)>h) by=h-y;
			z=bx;
			for (x = rc->left ; x < w ; x += z)
         {
				if ((x+z)>w) z=w-x;
				Draw(pDC, x, y);
			}
		}
	}
	return bOK;
}

BOOL CImage::SetPaletteIndex(int idx, BYTE r, BYTE g, BYTE b)
{
	BOOL bOK = ( (NULL != m_handle) && (idx >= 0) && (idx < m_header.biClrUsed) );
   if (bOK)
   {
		BYTE* iDst = (BYTE*)(m_handle) + sizeof(BITMAPINFOHEADER);
		int ldx=idx*sizeof(RGBQUAD);
		iDst[ldx++] = (BYTE) b;
		iDst[ldx++] = (BYTE) g;
		iDst[ldx++] = (BYTE) r;
		iDst[ldx  ] = (BYTE) 0;
	}
   return bOK;
}

BOOL CImage::SetPaletteIndex(int idx, RGBQUAD color)
{
	BOOL bOK = ( (NULL != m_handle) && (idx >= 0) && (idx < m_header.biClrUsed) );
   if (bOK)
   {
		BYTE* iDst = (BYTE*)(m_handle) + sizeof(BITMAPINFOHEADER);
		int ldx=idx*sizeof(RGBQUAD);
		iDst[ldx++] = (BYTE) color.rgbBlue;
		iDst[ldx++] = (BYTE) color.rgbGreen;
		iDst[ldx++] = (BYTE) color.rgbRed;
		iDst[ldx  ] = (BYTE) 0;
	}
   return bOK;
}

BOOL CImage::SetPaletteIndex(int idx, COLORREF color)
{
	BOOL bOK = ( (NULL != m_handle) && (idx >= 0) && (idx < m_header.biClrUsed) );
   if (bOK)
   {
		BYTE* iDst = (BYTE*)(m_handle) + sizeof(BITMAPINFOHEADER);
		int ldx=idx*sizeof(RGBQUAD);
		iDst[ldx++] = (BYTE) GetBValue(color);
		iDst[ldx++] = (BYTE) GetGValue(color);
		iDst[ldx++] = (BYTE) GetRValue(color);
		iDst[ldx] = (BYTE) 0;
	}
   return bOK;
}

int CImage::FindPaletteIndex(COLORREF color) const
{
   int index = -1;	
   if (m_header.biBitCount<=8)
   {
		RGBQUAD* ppal=GetPalette();
      for (int i = 0; i < m_header.biClrUsed; i++)
      {
         if (RGB(ppal[i].rgbRed, ppal[i].rgbGreen, ppal[i].rgbBlue) == color)
         {
            index = i;
            break;
         }
		}
	} 
   return index;
}

RGBQUAD* CImage::GetPalette() const
{
	if ((m_handle)&&(m_header.biClrUsed))
		return (RGBQUAD*)((BYTE*)m_handle + sizeof(BITMAPINFOHEADER));
	return NULL;
}

RGBQUAD CImage::GetPaletteIndex(int idx) const
{
	RGBQUAD rgb = {0,0,0,0};
	if ((m_handle)&&(m_header.biClrUsed))
   {
		BYTE* iDst = (BYTE*)(m_handle) + sizeof(BITMAPINFOHEADER);
		if ((idx>=0)&&(idx<m_header.biClrUsed))
      {	
			int ldx=idx*sizeof(RGBQUAD);
			rgb.rgbBlue = iDst[ldx++];
			rgb.rgbGreen=iDst[ldx++];
			rgb.rgbRed =iDst[ldx];
			rgb.rgbReserved = 0;
		}
	}
   else
   {
   }
	return rgb;
}

int CImage::GetPixelIndex(int x,int y)
{
	int result = 0;
   if ((m_handle==NULL)||(m_header.biClrUsed==0)|| (x<0)||(y<0)||(x>=m_header.biWidth)||(y>=m_header.biHeight))
   {
      if (m_info.nBkgndIndex != -1)
      {
         result = m_info.nBkgndIndex;
      }
   }
   else if (m_header.biBitCount==8)
   {
		result = m_info.pImage[y*m_info.dwEffWidth + x];
	} 
   else 
   {
		BYTE pos;
		BYTE iDst= m_info.pImage[y*m_info.dwEffWidth + (x*m_header.biBitCount >> 3)];
		if (m_header.biBitCount==4)
      {
			pos = 4*(1-x%2);
			iDst &= (0x0F<<pos);
			result = iDst >> pos;
		} 
      else if (m_header.biBitCount==1)
      {
			pos = 7-x%8;
			iDst &= (0x01<<pos);
			result = iDst >> pos;
		}
	}
	return result ;
}

RGBQUAD CImage::GetPixelColor(int x,int y)
{
	RGBQUAD rgb={0,0,0,0};
	if ((m_handle==NULL)||(x<0)||(y<0)|| (x>=m_header.biWidth)||(y>=m_header.biHeight)) 
   {
      if (m_info.nBkgndIndex != -1)
      {
         rgb = GetPaletteIndex(m_info.nBkgndIndex);
      }
   }
   else if (m_header.biClrUsed) 
   {
      rgb = GetPaletteIndex(GetPixelIndex(x,y));
   }
	else 
   {
		BYTE* iDst = m_info.pImage + y*m_info.dwEffWidth + x*3;
		rgb.rgbBlue = *iDst++;
		rgb.rgbGreen= *iDst++;
		rgb.rgbRed =*iDst;
	}
   return rgb;
}

void CImage::SetPixelIndex(int x,int y,BYTE i)
{
	if ((m_handle==NULL)||(m_header.biClrUsed==0)|| (x<0)||(y<0)||(x>=m_header.biWidth)||(y>=m_header.biHeight))
   {
   }
	else if (m_header.biBitCount==8)
   {
		m_info.pImage[y*m_info.dwEffWidth + x]=i;
	} 
   else 
   {
		BYTE pos;
		BYTE* iDst= m_info.pImage + y*m_info.dwEffWidth + (x*m_header.biBitCount >> 3);
		if (m_header.biBitCount==4)
      {
			pos = 4*(1-x%2);
			*iDst &= ~(0x0F<<pos);
			*iDst |= (i<<pos);
		} 
      else if (m_header.biBitCount==1)
      {
			pos = 7-x%8;
			*iDst &= ~(0x01<<pos);
			*iDst |= (i<<pos);
		}
	}
}

void CImage::SetPixelColor(int x,int y,COLORREF cr)
{
	SetPixelColor(x,y,RGB2RGBQUAD(cr));
}

void CImage::SetPixelColor(int x,int y,RGBQUAD c)
{
	if ((m_handle==NULL)||(x<0)||(y<0)||
		(x>=m_header.biWidth)||(y>=m_header.biHeight))
   {
      return;
   }
	if (m_header.biClrUsed)
   {
		SetPixelIndex(x,y,GetNearestIndex(c));
   }
	else 
   {
		BYTE* iDst = m_info.pImage + y*m_info.dwEffWidth + x*3;
		*iDst++ = c.rgbBlue;
		*iDst++ = c.rgbGreen;
		*iDst   = c.rgbRed;
	}
}

BYTE CImage::GetNearestIndex(RGBQUAD c)
{
	if ((m_handle==NULL)||(m_header.biClrUsed==0)) return 0;
	BYTE* iDst = (BYTE*)(m_handle) + sizeof(BITMAPINFOHEADER);
	int distance=200000;
	int i,j = -1;
	int k,l;
	for (i=0,l=0;i<(int)m_header.biClrUsed;i++,l+=sizeof(RGBQUAD))
   {
		k = (iDst[l]-c.rgbBlue)*(iDst[l]-c.rgbBlue)+
			(iDst[l+1]-c.rgbGreen)*(iDst[l+1]-c.rgbGreen)+
			(iDst[l+2]-c.rgbRed)*(iDst[l+2]-c.rgbRed);
		if (k==0)
      {
			j=i;
			break;
		}
		if (k<distance)
      {
			distance=k;
			j=i;
		}
	}
	return j;
}

void CImage::RGB2BGR(BYTE *buffer, int length)
{
	BYTE temp;
	if (buffer)
   {
		for (int i=0;i<length;i+=3)
      {
			temp = buffer[i]; buffer[i] = buffer[i+2]; buffer[i+2] = temp;
		}
	}
}

#define  HSLMAX   240	/* H,L, and S vary over 0-HSLMAX */
#define  RGBMAX   255   /* R,G, and B vary over 0-RGBMAX */
                        /* HSLMAX BEST IF DIVISIBLE BY 6 */
                        /* RGBMAX, HSLMAX must each fit in a BYTE. */
/* Hue is undefined if Saturation is 0 (grey-scale) */
/* This value determines where the Hue scrollbar is */
/* initially set for achromatic colors */
#define UNDEFINED (HSLMAX*2/3)

RGBQUAD CImage::RGBtoHSL(RGBQUAD lRGBColor)
{
	BYTE R,G,B;					/* input RGB values */
	BYTE H,L,S;					/* output HSL values */
	BYTE cMax,cMin;				/* max and min RGB values */
	WORD Rdelta,Gdelta,Bdelta;	/* intermediate value: % of spread from max*/

	R = lRGBColor.rgbRed;	/* get R, G, and B out of DWORD */
	G = lRGBColor.rgbGreen;
	B = lRGBColor.rgbBlue;

	cMax = max( max(R,G), B);	/* calculate lightness */
	cMin = min( min(R,G), B);
	L = (((cMax+cMin)*HSLMAX)+RGBMAX)/(2*RGBMAX);

	if (cMax==cMin){			/* r=g=b --> achromatic case */
		S = 0;					/* saturation */
		H = UNDEFINED;			/* hue */
	} else {					/* chromatic case */
		if (L <= (HSLMAX/2))	/* saturation */
			S = (((cMax-cMin)*HSLMAX)+((cMax+cMin)/2))/(cMax+cMin);
		else
			S = (((cMax-cMin)*HSLMAX)+((2*RGBMAX-cMax-cMin)/2))/(2*RGBMAX-cMax-cMin);
		/* hue */
		Rdelta = ( ((cMax-R)*(HSLMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);
		Gdelta = ( ((cMax-G)*(HSLMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);
		Bdelta = ( ((cMax-B)*(HSLMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);

		if (R == cMax)
			H = Bdelta - Gdelta;
		else if (G == cMax)
			H = (HSLMAX/3) + Rdelta - Bdelta;
		else /* B == cMax */
			H = ((2*HSLMAX)/3) + Gdelta - Rdelta;

		if (H < 0) H += HSLMAX;
		if (H > HSLMAX) H -= HSLMAX;
	}
	RGBQUAD hsl={L,S,H,0};
	return hsl;
}

WORD CImage::HueToRGB(WORD n1,WORD n2,WORD hue)
{ 
	/* range check: note values passed add/subtract thirds of range */
	if (hue < 0) hue += HSLMAX;
	if (hue > HSLMAX) hue -= HSLMAX;

	/* return r,g, or b value from this tridrant */
	if (hue < (HSLMAX/6))
		return ( n1 + (((n2-n1)*hue+(HSLMAX/12))/(HSLMAX/6)) );
	if (hue < (HSLMAX/2))
		return ( n2 );
	if (hue < ((HSLMAX*2)/3))
		return ( n1 + (((n2-n1)*(((HSLMAX*2)/3)-hue)+(HSLMAX/12))/(HSLMAX/6))); 
	else
		return ( n1 );
} 

RGBQUAD CImage::HSLtoRGB(COLORREF cHSLColor)
{
	return HSLtoRGB(RGB2RGBQUAD(cHSLColor));
}

RGBQUAD CImage::HSLtoRGB(RGBQUAD lHSLColor)
{ 
	WORD hue,lum,sat;
	BYTE R,G,B;					/* RGB component values */
	WORD Magic1,Magic2;			/* calculated magic numbers (really!) */

	hue = lHSLColor.rgbRed;	/* get H, S, and L out of DWORD */
	sat = lHSLColor.rgbGreen;
	lum = lHSLColor.rgbBlue;

	if (sat == 0) {            /* achromatic case */
		R=G=B=(lum*RGBMAX)/HSLMAX;
	} else {                   /* chromatic case */
		/* set up magic numbers */
		if (lum <= (HSLMAX/2))
			Magic2 = (lum*(HSLMAX + sat) + (HSLMAX/2))/HSLMAX;
		else
			Magic2 = lum + sat - ((lum*sat) + (HSLMAX/2))/HSLMAX;
		Magic1 = 2*lum-Magic2;

		/* get RGB, change units from HSLMAX to RGBMAX */
		R = (HueToRGB(Magic1,Magic2,(WORD)(hue+(HSLMAX/3)))*RGBMAX+(HSLMAX/2))/HSLMAX; 
		G = (HueToRGB(Magic1,Magic2,hue)*RGBMAX + (HSLMAX/2)) / HSLMAX;
		B = (HueToRGB(Magic1,Magic2,(WORD)(hue-(HSLMAX/3)))*RGBMAX+(HSLMAX/2))/HSLMAX; 
	}
	RGBQUAD rgb={B,G,R,0};
	return rgb;
}

RGBQUAD CImage::RGB2RGBQUAD(COLORREF cr)
{
	RGBQUAD c;
	c.rgbRed = GetRValue(cr);	/* get R, G, and B out of DWORD */
	c.rgbGreen = GetGValue(cr);
	c.rgbBlue = GetBValue(cr);
	c.rgbReserved=0;
	return c;
}

COLORREF CImage::RGBQUAD2RGB (RGBQUAD c)
{
	return RGB(c.rgbRed,c.rgbGreen,c.rgbBlue);
}

BOOL CImage::GetRGB(int i, BYTE* r, BYTE* g, BYTE* b)
{
	RGBQUAD* ppal=GetPalette();
	if (ppal) {
		*r = ppal[i].rgbRed;
		*g = ppal[i].rgbGreen;
		*b = ppal[i].rgbBlue; 
		return TRUE;
	}
	return FALSE;
}

void CImage::SetPalette(DWORD n, BYTE *r, BYTE *g, BYTE *b)
{
	if ((!r)||(m_handle==NULL)||(m_header.biClrUsed==0)) return;
	if (!g) g = r;
	if (!b) b = g;
	RGBQUAD* ppal=GetPalette();
	DWORD m=min(n,m_header.biClrUsed);
	for (DWORD i=0; i<m;i++){
		ppal[i].rgbRed=r[i];
		ppal[i].rgbGreen=g[i];
		ppal[i].rgbBlue=b[i];
	}

}

void CImage::SetPalette(const rgb_color_struct *rgb,DWORD nColors)
{
	if ((!rgb)||(m_handle==NULL)||(m_header.biClrUsed==0)) return;
	RGBQUAD* ppal=GetPalette();
	DWORD m=min(nColors,m_header.biClrUsed);
	for (DWORD i=0; i<m;i++){
		ppal[i].rgbRed=rgb[i].r;
		ppal[i].rgbGreen=rgb[i].g;
		ppal[i].rgbBlue=rgb[i].b;
	}
}

void CImage::SetPalette(const RGBQUAD* pPal,DWORD nColors)
{
	if ((pPal==NULL)||(m_handle==NULL)||(m_header.biClrUsed==0)) return;
	memcpy(GetPalette(),pPal,min(GetPaletteSize(),nColors*sizeof(RGBQUAD)));
}

void CImage::SetGreyPalette()
{
	if ((m_handle==NULL)||(m_header.biClrUsed==0)) return;
	RGBQUAD pal[256];
	RGBQUAD* ppal;
	BYTE* iDst;
	ULONG ni;
	ppal=(RGBQUAD*)&pal[0];
	iDst = (BYTE*)(m_handle) + sizeof(BITMAPINFOHEADER);
	for (ni=0;ni<m_header.biClrUsed;ni++)
		pal[ni].rgbBlue=pal[ni].rgbGreen = pal[ni].rgbRed = (BYTE)((ni*256)/m_header.biClrUsed);
	memcpy(iDst,ppal,GetPaletteSize());
}

void CImage::BlendPalette(COLORREF cr,int perc)
{
	if ((m_handle==NULL)||(m_header.biClrUsed==0)) return;
	BYTE* iDst = (BYTE*)(m_handle) + sizeof(BITMAPINFOHEADER);
	DWORD i,r,g,b;
	RGBQUAD* pPal=(RGBQUAD*)iDst;
	r = GetRValue(cr);
	g = GetGValue(cr);
	b = GetBValue(cr);
	if (perc>100) perc=100;
	for(i=0;i<m_header.biClrUsed;i++)
   {
		pPal[i].rgbBlue =(BYTE)((pPal[i].rgbBlue*(100-perc)+b*perc)/100);
		pPal[i].rgbGreen=(BYTE)((pPal[i].rgbGreen*(100-perc)+g*perc)/100);
		pPal[i].rgbRed  =(BYTE)((pPal[i].rgbRed*(100-perc)+r*perc)/100);
	}
}

// returns the whole m_handle dimension
int CImage::GetLength()
{
	return m_header.biSize + m_header.biSizeImage + GetPaletteSize();
}

BOOL CImage::IsInside(int x, int y)
{
  return (0<=y && y<m_header.biHeight && 0<=x && x<m_header.biWidth);
}

BOOL CImage::Copy(const CImage& src)
{
   BOOL bOK = (NULL != Create(src.GetWidth(),src.GetHeight(),src.GetBPP()));
   if (bOK)
   {
	   memcpy(m_handle, src.m_handle, GetLength());
      m_info.nBkgndIndex = src.m_info.nBkgndIndex;
   }
   return bOK;
}

HANDLE CImage::CopyToHandle(HANDLE hMem)
{
	if (hMem)
   {
		GlobalFree(hMem);
		hMem = NULL;
	}
	if (m_handle)
   {
		hMem= GlobalAlloc(GHND, GetLength());
		BYTE* pDst=(BYTE*)GlobalLock(hMem);
		memcpy(pDst,m_handle,GetLength());
		GlobalUnlock(hMem);
	}
	return hMem;
}

void CImage::Clear(BYTE bval)
{
	if (m_handle) memset(m_info.pImage,bval,m_header.biSizeImage);
}

// moves everything from (*from) to (this), (*from) become empty
void CImage::Transfer(CImage *src)
{
	if (src)
   {
		if (IsOpen()) Close();

		memcpy(&m_header,&src->m_header,sizeof(BITMAPINFOHEADER));
		memcpy(&m_info,&src->m_info,sizeof(CXIMAGEINFO));
		m_handle = src->m_handle;
      m_info.nBkgndIndex = src->m_info.nBkgndIndex;

		memset(&src->m_header,0,sizeof(BITMAPINFOHEADER));
		memset(&src->m_info,0,sizeof(CXIMAGEINFO));
		src->m_handle=NULL;
	}
}

// (this) points to the same m_handle owned by (*from), the image remains in (*from)
// but (this) has the access to the bits. Use carefully !!!
void CImage::Ghost(CImage *from)
{
	if (from)
   {
		memcpy(&m_header,&from->m_header,sizeof(BITMAPINFOHEADER));
		memcpy(&m_info,&from->m_info,sizeof(CXIMAGEINFO));
		m_handle = from->m_handle;
		m_info.pGhost=from;
	}
}

BOOL CImage::Save(LPCSTR filename, enum cximage_type imagetype)
{
   FILE* stream = fopen(filename, "wb");
   BOOL bOK = (stream != NULL);
   if (bOK)
   {
      bOK = Write(stream, imagetype);
      fclose(stream);
   }
   return bOK;
}

BOOL CImage::Save(LPCWSTR filename, enum cximage_type imagetype)
{
   FILE* stream = _wfopen(filename, L"wb");
   BOOL bOK = (stream != NULL);
   if (bOK)
   {
      bOK = Write(stream, imagetype);
      fclose(stream);
   }
   return bOK;
}

BOOL CImage::Write(FILE* hFile, enum cximage_type imagetype)
{
	BOOL bOK=FALSE;
	
   m_info.dwFileType = imagetype;

	switch (imagetype)
   {
      case ENUM_cimage_format_bmp:
      {
		   CImageBMP temp;
		   temp.Ghost(this);
		   bOK = temp.Write(hFile);
		   strcpy(m_info.szLastError,temp.GetLastError());
         break;
	   }
      case ENUM_cimage_format_icon:
      {
		   CImageIcon temp;
		   temp.Ghost(this);
		   bOK = temp.Write(hFile);
		   strcpy(m_info.szLastError,temp.GetLastError());
         break;
	   }
      case ENUM_cimage_format_tiff:
      {
		   CImageTIFF temp;
		   temp.Ghost(this);
		   bOK = temp.Write(hFile);
		   strcpy(m_info.szLastError,temp.GetLastError());
         break;
	   }
      case ENUM_cimage_format_jpeg:
      {
		   CImageJPEG temp;
		   temp.Ghost(this);
		   bOK = temp.Write(hFile);
		   strcpy(m_info.szLastError,temp.GetLastError());
         break;
	   }
      case ENUM_cimage_format_gif:
      {
		   CImageGIF temp;
		   temp.Ghost(this);
		   bOK = temp.Write(hFile);
		   strcpy(m_info.szLastError,temp.GetLastError());
         break;
	   }
      case ENUM_cimage_format_png:
      {
		   CImagePNG temp;
		   temp.Ghost(this);
		   bOK = temp.Write(hFile);
		   strcpy(m_info.szLastError,temp.GetLastError());
         break;
	   }
   }
	return bOK;
}

BOOL CImage::CreateFromFile(LPCSTR filename, enum cximage_type imagetype)
{
   FILE* stream = fopen(filename, "rb");
   BOOL bOK = (stream != NULL);
   if (bOK)
   {
      bOK = Read(stream, imagetype);
      fclose(stream);
   }
   return bOK;
}

BOOL CImage::CreateFromFile(LPCWSTR filename, enum cximage_type imagetype)
{
   FILE* stream = _wfopen(filename, L"rb");
   BOOL bOK = (stream != NULL);
   if (bOK)
   {
      bOK = Read(stream, imagetype);
      fclose(stream);
   }
   return bOK;
}

BOOL CImage::Read(FILE* hFile, enum cximage_type imagetype)
{
	BOOL bOK = FALSE;
   m_info.dwFileType = imagetype;

	switch (imagetype)
   {
      case ENUM_cimage_format_bmp:
      {
		   CImageBMP temp;
		   bOK = temp.Read(hFile);
         if (bOK)
         {
			   Transfer(&temp);
		   } 
         else
         {
            strcpy(m_info.szLastError,temp.GetLastError());
         }
         break;
	   }
      case ENUM_cimage_format_icon:
      {
		   CImageIcon temp;
         bOK = temp.Read(hFile);
         if (bOK)
         {
			   Transfer(&temp);
		   } 
         else 
         {
            strcpy(m_info.szLastError,temp.GetLastError());
         }
         break;
	   }
      case ENUM_cimage_format_tiff:
      {
		   CImageTIFF temp;
		   bOK = temp.Read(hFile);
         if (bOK)
         {
			   Transfer(&temp);
		   } 
         else 
         {
            strcpy(m_info.szLastError,temp.GetLastError());
         }
         break;
	   }
      case ENUM_cimage_format_jpeg:
      {
		   CImageJPEG temp;
		   bOK = temp.Read(hFile);
         if (bOK)
         {
			   Transfer(&temp);
		   }
         else 
         {
            strcpy(m_info.szLastError,temp.GetLastError());
         }
         break;
      }
      case ENUM_cimage_format_gif:
      {
		   CImageGIF temp;
		   bOK = temp.Read(hFile);
         if (bOK)
         {
			   Transfer(&temp);
		   }
         else 
         {
            strcpy(m_info.szLastError,temp.GetLastError());
         }
         break;
	   }
      case ENUM_cimage_format_png:
      {
		   CImagePNG temp;
		   bOK = temp.Read(hFile);
         if (bOK)
         {
			   Transfer(&temp);
		   }
         else 
         {
            strcpy(m_info.szLastError,temp.GetLastError());
         }
         break;
	   }
   }
	return bOK;
}

BOOL CImage::Flip()
{
	if (!IsOpen()) return FALSE;
	CImage temp(*this);
	if (!temp.IsOpen()) return FALSE;
	BYTE *iSrc,*iDst;
	iSrc=m_info.pImage + (m_header.biHeight-1)*m_info.dwEffWidth;
	iDst=temp.m_info.pImage;
   for(int y=0; y < m_header.biHeight; y++)
   {
		memcpy(iDst,iSrc,m_info.dwEffWidth);
		iSrc-=m_info.dwEffWidth;
		iDst+=m_info.dwEffWidth;
	}
	Transfer(&temp);
	return TRUE;
}

BOOL CImage::Mirror()
{
	if (!IsOpen()) return FALSE;
	CImage temp(*this);
	BYTE *iSrc,*iDst;
	int wdt=(m_header.biWidth-1) * (m_header.biBitCount==24 ? 3:1);
	iSrc=m_info.pImage + wdt;
	iDst=temp.m_info.pImage;
	int x,y;
	switch (m_header.biBitCount){
	case 24:
		for(y=0; y < m_header.biHeight; y++){
			for(x=0; x <= wdt; x+=3){
				*(iDst+x)=*(iSrc-x);
				*(iDst+x+1)=*(iSrc-x+1);
				*(iDst+x+2)=*(iSrc-x+2);
			}
			iSrc+=m_info.dwEffWidth;
			iDst+=m_info.dwEffWidth;
		}
		break;
	case 8:
		for(y=0; y < m_header.biHeight; y++){
			for(x=0; x <= wdt; x++)
				*(iDst+x)=*(iSrc-x);
			iSrc+=m_info.dwEffWidth;
			iDst+=m_info.dwEffWidth;
		}
		break;
	default:
		for(y=0; y < m_header.biHeight; y++){
			for(x=0; x <= wdt; x++)
				temp.SetPixelIndex(x,y,GetPixelIndex(wdt-x,y));
		}
	}
	Transfer(&temp);
	return TRUE;
}

BOOL CImage::Negative()
{
	if (!IsOpen()) return FALSE;

	if (m_header.biBitCount<=8)
   {
		RGBQUAD* ppal=GetPalette();
		for(DWORD i=0;i<m_header.biClrUsed;i++)
      {
			ppal[i].rgbBlue = 255-ppal[i].rgbBlue;
			ppal[i].rgbGreen= 255-ppal[i].rgbGreen;
			ppal[i].rgbRed  = 255-ppal[i].rgbRed;
		}
	} 
   else 
   {
		BYTE *iSrc=m_info.pImage;
		int n=m_header.biHeight*m_info.dwEffWidth;
		for(int i=0; i < n; i++)
      {
			*iSrc=~(*(iSrc));
			iSrc++;
		}
	}
	return TRUE;
}

static COLORREF DIB_Grey(BYTE r, BYTE g, BYTE b, LPVOID)
{
   const int color = ((( b * 11 ) + ( g * 59 ) + ( r * 30 )) / 100 );
   return RGB(color, color, color);
}

static COLORREF DIB_Exchange(BYTE r, BYTE g, BYTE b, LPVOID lpUser)
{
   const COLORREF* aColor = (const COLORREF*)lpUser;
   COLORREF clr;
   if (aColor[0] == RGB(r, g, b))
   {
      clr = aColor[1];
   }
   else if (aColor[1] == RGB(r, g, b))
   {
      clr = aColor[0];
   }
   else
   {
      clr = RGB(r,g,b);
   }
   return clr;
}

BOOL CImage::WalkColors(COLORREF(*manipulate)(BYTE r, BYTE g, BYTE b, LPVOID), LPVOID lpUser)
{
	COLORREF cr;
	BOOL bOK = IsOpen();
   if (bOK)
   {
	   if (m_header.biBitCount<=8)
      {
		   RGBQUAD* ppal=GetPalette();
         for (DWORD i=0;i<m_header.biClrUsed;i++)
         {
			   cr = (*manipulate)(ppal[i].rgbRed, ppal[i].rgbGreen, ppal[i].rgbBlue, lpUser);
            ppal[i].rgbRed   = GetRValue(cr);
            ppal[i].rgbGreen = GetGValue(cr);
            ppal[i].rgbBlue  = GetBValue(cr);
		   }
	   } 
      else 
      {
	      const int wdt=(m_header.biWidth-1) * (m_header.biBitCount==24 ? 3:1);
   	   BYTE* iDst=m_info.pImage;
      
         for(int y=0; y < m_header.biHeight; y++)
         {
		      for(int x=0; x < wdt; x+=3)
            {
			      cr = (*manipulate)(*(iDst+x+0), *(iDst+x+1), *(iDst+x+2), lpUser);
			      *(iDst+x+0)=GetRValue(cr);
			      *(iDst+x+1)=GetGValue(cr);
			      *(iDst+x+2)=GetBValue(cr);
		      }
		      iDst+=m_info.dwEffWidth;
	      }
	   }
   }
	return bOK;
}

BOOL CImage::Exchange(COLORREF cr0 /*= CLR_INVALID*/, COLORREF cr1 /*= CLR_INVALID*/)
{
   if (cr0 == CLR_INVALID) cr0 = RGB(0x00,0x00,0x00);
   if (cr1 == CLR_INVALID) cr1 = RGB(0xFF,0xFF,0xFF);
   COLORREF aclr[2];
   aclr[0] = cr0;
   aclr[1] = cr1;
   return WalkColors(DIB_Exchange, aclr);
}

BOOL CImage::GreyScale()
{
   return WalkColors(DIB_Grey);
}

BOOL CImage::Rotate(double angle)
{
	BOOL bOK = IsOpen();
   if (bOK)
   {

   //  $Id: FilterRotate.cpp,v 1.10 2000/12/18 22:42:53 uzadow Exp $
   //  Copyright (c) 1996-1998 Ulrich von Zadow

	   // Negative the angle, because the y-axis is negative.
	   const double ang = -angle*acos(0)/90;
	   const int nWidth = GetWidth();
	   const int nHeight= GetHeight();
	   const double cos_angle = cos(ang);
	   const double sin_angle = sin(ang);

	   // Calculate the size of the new bitmap
	   POINT p1={0,0};
	   POINT p2={nWidth,0};
	   POINT p3={0,nHeight};
	   POINT p4={nWidth-1,nHeight};
	   POINT newP1,newP2,newP3,newP4, leftTop, rightTop, leftBottom, rightBottom;

	   newP1.x = p1.x;
	   newP1.y = p1.y;
	   newP2.x = (int)(p2.x*cos_angle - p2.y*sin_angle);
	   newP2.y = (int)(p2.x*sin_angle + p2.y*cos_angle);
	   newP3.x = (int)(p3.x*cos_angle - p3.y*sin_angle);
	   newP3.y = (int)(p3.x*sin_angle + p3.y*cos_angle);
	   newP4.x = (int)(p4.x*cos_angle - p4.y*sin_angle);
	   newP4.y = (int)(p4.x*sin_angle + p4.y*cos_angle);

	   leftTop.x = min(min(newP1.x,newP2.x),min(newP3.x,newP4.x));
	   leftTop.y = min(min(newP1.y,newP2.y),min(newP3.y,newP4.y));
	   rightBottom.x = max(max(newP1.x,newP2.x),max(newP3.x,newP4.x));
	   rightBottom.y = max(max(newP1.y,newP2.y),max(newP3.y,newP4.y));
	   leftBottom.x = leftTop.x;
	   leftBottom.y = rightBottom.y;
	   rightTop.x = rightBottom.x;
	   rightTop.y = leftTop.y;

	   const int newWidth = rightTop.x - leftTop.x;
	   const int newHeight= leftBottom.y - leftTop.y;
	   CImage imgDest(newWidth,newHeight,GetBPP());
	   imgDest.SetPalette(GetPalette());
      imgDest.m_info.nBkgndIndex = m_info.nBkgndIndex;// TRK

	   int x,y,newX,newY,oldX,oldY;

	   if (m_header.biClrUsed==0)
      { 
         //RGB
		   for (y = leftTop.y, newY = 0; y<=leftBottom.y; y++,newY++)
         {
		      for (x = leftTop.x, newX = 0; x<=rightTop.x; x++,newX++)
		      {
			      oldX = (int)(x*cos_angle + y*sin_angle - 0.5);
			      oldY = (int)(y*cos_angle - x*sin_angle - 0.5);
			      imgDest.SetPixelColor(newX,newY,GetPixelColor(oldX,oldY));
		      }
         }
	   } 
      else 
      { 
         //PALETTE
		   for (y = leftTop.y, newY = 0; y<=leftBottom.y; y++,newY++)
         {
		      for (x = leftTop.x, newX = 0; x<=rightTop.x; x++,newX++)
		      {
			      oldX = (int)(x*cos_angle + y*sin_angle - 0.5);
			      oldY = (int)(y*cos_angle - x*sin_angle - 0.5);
			      imgDest.SetPixelIndex(newX,newY,GetPixelIndex(oldX,oldY));
		      }
         }
	   }

	   Transfer(&imgDest);
   }
	return bOK;
}

BOOL CImage::LoadResource(HRSRC hRes, enum cximage_type imagetype, HMODULE hModule /*= NULL*/)
{
   DWORD rsize  = ::SizeofResource(hModule,hRes);
	HGLOBAL hMem = ::LoadResource(hModule,hRes);
	BOOL bOK = (hMem != NULL);
   if (bOK)
   {
		LPVOID lp = LockResource(hMem);
		if (NULL != lp)
      {
         FILE* stream = tmpfile();
			if (NULL != stream)
         {
				fwrite(lp, rsize, 1, stream);
				fseek(stream,0,SEEK_SET);
				switch (imagetype)
            {
               case ENUM_cimage_format_jpeg:
               {
					   CImageJPEG temp;
					   bOK = temp.Read(stream);
                  if (bOK)
                  {
						   Transfer(&temp);
					   } 
                  else 
                  {
						   strcpy(m_info.szLastError,temp.GetLastError());
					   }
                  break;
				   }
               case ENUM_cimage_format_icon:
               {
	               CImageIcon temp;
	               bOK = temp.Read(stream);
                  if (bOK)
                  {
		               Transfer(&temp);
	               } 
                  else 
                  {
		               strcpy(m_info.szLastError,temp.GetLastError());
	               }
                  break;
               }
               case ENUM_cimage_format_gif:
               {
					   CImageGIF temp;
					   bOK = temp.Read(stream);
                  if (bOK)
                  {
						   Transfer(&temp);
					   } 
                  else 
                  {
						   strcpy(m_info.szLastError,temp.GetLastError());
					   }
                  break;
				   }
               case ENUM_cimage_format_png:
               {
					   CImagePNG temp;
					   bOK = temp.Read(stream);
                  if (bOK)
                  {
						   Transfer(&temp);
					   } 
                  else 
                  {
						   strcpy(m_info.szLastError,temp.GetLastError());
					   }
                  break;
				   }
            }
				fclose(stream);
				strcpy(m_info.szLastError,"Unsupported operation for this format");
			}
		}
	} 
   else
   {
      strcpy(m_info.szLastError,"Unable to load resource!");
   }
   return bOK;
}


HBITMAP CImage::MakeBitmap()
{
	HBITMAP handle = NULL;
   HDC dc = ::CreateCompatibleDC(NULL);
	if (dc)
	{
        // tmpBitmap is a dummy, to satisfy ::CreateCompatibleDC (it
        // is a memory dc that must have a handle selected into it)
        HDC dc2 = GetDC(NULL);
        HBITMAP tmpBitmap = ::CreateCompatibleBitmap(dc2, GetWidth(), GetHeight());
        ReleaseDC(NULL, dc2);
        HBITMAP oldBitmap = (HBITMAP) ::SelectObject(dc, tmpBitmap);

/*
        if ( GetPalette() )
        {
            HPALETTE oldPal = ::SelectPalette(dc, (HPALETTE) GetPalette()->m_hObject, FALSE);
		      ::RealizePalette(dc);
        }
*/
        handle = ::CreateDIBitmap(dc, &m_header, CBM_INIT, m_info.pImage, GetBitmapInfo(), DIB_RGB_COLORS);

        ::SelectPalette(dc, NULL, TRUE);
        ::SelectObject(dc, oldBitmap);
        ::DeleteObject(tmpBitmap);
        ::DeleteDC(dc);

	}
	return handle;
}

/////////////////////////////////////////////////////////////////////////////

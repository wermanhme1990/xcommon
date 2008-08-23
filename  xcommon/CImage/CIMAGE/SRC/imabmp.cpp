// imabmp.cpp

/*
 * File:	wbimage.cc
 * Purpose:	Platform Independent Image Base Class (Windows version)
 * Author:	Alejandro Aguilar Sierra
 * Created:	1995
 * Copyright:	(c) 1995 Alejandro Aguilar Sierra <asierra@servidor.unam.mx>
 */
// Removed fopen/fclose mess. Troels K.
/*
 * File:	ximabmp.cpp
 * Purpose:	Platform Independent BMP Image Class Loader and Writer
 * 07/Aug/2001 <ing.davide.pizzolato@libero.it>
 */

#include "..\include\cimage.h"
#pragma hdrstop
#include "imabmp.h"

#define IMAGE_BITMAP_SIGNATURE 0x4D42 // BM

BOOL CImageBMP::Read(FILE* hFile)
{
	return DibOpenFile(hFile);
}

BOOL CImageBMP::Write(FILE* hFile)
{                         
	BITMAPFILEHEADER	hdr;

	hdr.bfType = IMAGE_BITMAP_SIGNATURE;   // 'BM'
	hdr.bfSize = GetLength() + sizeof(hdr);
	hdr.bfReserved1 = hdr.bfReserved2 = 0;
	hdr.bfOffBits = (DWORD) sizeof(hdr)+ m_header.biSize + GetPaletteSize();

    // Write the file header
	fwrite(&hdr,sizeof(hdr),1,hFile);
    // Write the DIB header and the bits
	fwrite(m_handle,GetLength(),1,hFile);
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////

BOOL CImageBMP::DibOpenFile(FILE* hFile)
{
    BITMAPFILEHEADER   bf;
    DWORD off = fseek(hFile,0L,SEEK_CUR);
    if (fread((LPSTR)&bf,sizeof(bf),1,hFile)==0) return 0;
    if (bf.bfType != BFT_BITMAP) { //do we have a RC HEADER?
        bf.bfOffBits = 0L;
        fseek(hFile,off,SEEK_SET);
    }
	
	if (!DibReadBitmapInfo(hFile,&m_header)) return FALSE;
   if(0)if (m_header.biCompression){ // TRK RLE !!!
		strcpy(m_info.szLastError,"RLE decompression not implemented");
		return FALSE;
	}

	Create(m_header.biWidth,m_header.biHeight,m_header.biBitCount);

    RGBQUAD *pRgb = GetPalette();
    if (NULL != pRgb)
    {
        if (m_header.biSize == sizeof(BITMAPCOREHEADER))
        {
             // convert a old color table (3 byte entries) to a new
             // color table (4 byte entries)
            fread((LPVOID)pRgb,DibNumColors(&m_header) * sizeof(RGBTRIPLE),1,hFile);
            for (int i=DibNumColors(&m_header)-1; i>=0; i--){
                pRgb[i].rgbRed      = ((RGBTRIPLE FAR *)pRgb)[i].rgbtRed;
                pRgb[i].rgbBlue     = ((RGBTRIPLE FAR *)pRgb)[i].rgbtBlue;
                pRgb[i].rgbGreen    = ((RGBTRIPLE FAR *)pRgb)[i].rgbtGreen;
                pRgb[i].rgbReserved = (BYTE)0;
            }
        } 
        else 
        {
            fread(pRgb,DibNumColors(&m_header) * sizeof(RGBQUAD),1,hFile);
        }
    }

    if (bf.bfOffBits != 0L) fseek(hFile,off + bf.bfOffBits,SEEK_SET);

    if (m_info.pImage)  // read in the bits 
		  fread(m_info.pImage, m_header.biSizeImage,1,hFile);

    return TRUE;
}
///////////////////////////////////////////////////////////////////////////
/*
 *  ReadDibBitmapInfo()
 *
 *  Will read a file in DIB format and return a global HANDLE to its
 *  BITMAPINFO.  This function will work with both "old" and "new"
 *  bitmap formats, but will always return a "new" BITMAPINFO.
 */
BOOL CImageBMP::DibReadBitmapInfo(FILE* fh,/*BITMAPINFOHEADER */PDIB pdib)
{
	BOOL bOK = !((fh==NULL)||(pdib==NULL));
   if (bOK)
   {
       bOK = 1 == fread(pdib,sizeof(BITMAPINFOHEADER),1,fh);
   }
   if (bOK)
   {
       BITMAPCOREHEADER   bc;

       switch (pdib->biSize) // what type of bitmap info is this?
       {
           case sizeof(BITMAPCOREHEADER):
           {
               bc = *(BITMAPCOREHEADER*)pdib;
               pdib->biSize               = sizeof(BITMAPINFOHEADER);
               pdib->biWidth              = (DWORD)bc.bcWidth;
               pdib->biHeight             = (DWORD)bc.bcHeight;
               pdib->biPlanes             =  (UINT)bc.bcPlanes;
               pdib->biBitCount           =  (UINT)bc.bcBitCount;
               pdib->biCompression        = BI_RGB;
               pdib->biSizeImage          = 0;
               pdib->biXPelsPerMeter      = 0;
               pdib->biYPelsPerMeter      = 0;
               pdib->biClrUsed            = 0;
               pdib->biClrImportant       = 0;

               fseek(fh,(LONG)sizeof(BITMAPCOREHEADER)-sizeof(BITMAPINFOHEADER),SEEK_CUR);
               break;
           }
           case sizeof(BITMAPINFOHEADER):
           default:
           {
               int x = sizeof(BITMAPINFOHEADER);
               x;
               break;
           }
       }
    }
   if (bOK)
   {
      FixBitmapInfo(pdib);
   }
   return bOK;
}
///////////////////////////////////////////////////////////////////////////
void CImageBMP::xlatClut8(BYTE FAR *pb, DWORD dwSize, BYTE FAR *xlat)
{
    DWORD dw;
	for (dw = 0; dw < dwSize; dw++, ((BYTE *&)pb)++){ *pb = xlat[*pb]; }
}
///////////////////////////////////////////////////////////////////////////
 void CImageBMP::xlatClut4(BYTE FAR *pb, DWORD dwSize, BYTE FAR *xlat)
{
	 DWORD dw;
	 for (dw = 0; dw < dwSize; dw++, ((BYTE  *&)pb)++)
		 *pb = (BYTE)(xlat[*pb & 0x0F] | (xlat[(*pb >> 4) & 0x0F] << 4));
}
///////////////////////////////////////////////////////////////////////////
#define RLE_ESCAPE  0
#define RLE_EOL     0
#define RLE_EOF     1
#define RLE_JMP     2
///////////////////////////////////////////////////////////////////////////
void CImageBMP::xlatRle8(BYTE FAR *pb, DWORD dwSize, BYTE FAR *xlat)
{
    BYTE    cnt;
    BYTE    b;
    BYTE  *prle = pb;

    for(;;){
        cnt = *prle++;
        b   = *prle;
        if (cnt == RLE_ESCAPE){
            prle++;
            switch (b)
            {
                case RLE_EOF:
                    return;
                case RLE_EOL:
                    break;
                case RLE_JMP:
                    prle++;     // skip dX
                    prle++;     // skip dY
                    break;
                default:
                    cnt = b;
                    for (b=0; b<cnt; b++,prle++)
                        *prle = xlat[*prle];

                    if (cnt & 1) prle++;

                    break;
            }
        } else {
            *prle++ = xlat[b];
        }
    }
}
///////////////////////////////////////////////////////////////////////////
void CImageBMP::xlatRle4(BYTE FAR *pb, DWORD dwSize, BYTE FAR *xlat)
{ }
///////////////////////////////////////////////////////////////////////////
 /*
 *  DibMapToPalette(pdib, hpal)
 *
 *  Map the colors of the DIB, using GetNearestPaletteIndex, to
 *  the colors of the given palette.
 */
BOOL CImageBMP::DibMapToPalette(PDIB pdib, HPALETTE hpal)
{
	 LPBITMAPINFOHEADER  lpbi;
	 PALETTEENTRY        pe;
	 int                 n;
	 int                 nDibColors;
	 int                 nPalColors=0;
	 BYTE FAR *          lpBits;
	 RGBQUAD FAR *       lpRgb;
	 BYTE                xlat[256];
	 DWORD               SizeImage;

	 if (!hpal || !pdib)  return FALSE;

	 lpbi   = (LPBITMAPINFOHEADER)pdib;
	 lpRgb  = DibColors(pdib);

	 GetObject(hpal,sizeof(int),(LPSTR)&nPalColors);
	 nDibColors = DibNumColors(pdib);

	 if ((SizeImage = lpbi->biSizeImage) == 0)
		  SizeImage = DibSizeImage(lpbi);

	 //  build a xlat table. from the current DIB colors to the given
	 //  palette.
	 for (n=0; n<nDibColors; n++)
		  xlat[n] = (BYTE)GetNearestPaletteIndex(hpal,RGB(lpRgb[n].rgbRed,lpRgb[n].rgbGreen,lpRgb[n].rgbBlue));

	 lpBits = (LPBYTE)DibPtr(lpbi);
	 lpbi->biClrUsed = nPalColors;

	 // re-size the DIB
	 if (nPalColors > nDibColors) {
		  realloc(lpbi, lpbi->biSize + nPalColors*sizeof(RGBQUAD) + SizeImage);
		  memmove((BYTE  *)DibPtr(lpbi), (BYTE  *)lpBits, SizeImage);
		  lpBits = (LPBYTE)DibPtr(lpbi);
	 } else if (nPalColors < nDibColors) {
		  memcpy(DibPtr(lpbi), lpBits, SizeImage);
		  realloc(lpbi, lpbi->biSize + nPalColors*sizeof(RGBQUAD) + SizeImage);
		  lpBits = (LPBYTE)DibPtr(lpbi);
	 }

	 // translate the DIB bits
	 switch (lpbi->biCompression)
	 {
		  case BI_RLE8:
				xlatRle8(lpBits, SizeImage, xlat);
				break;

		  case BI_RLE4:
				xlatRle4(lpBits, SizeImage, xlat);
				break;

		  case BI_RGB:
				if (lpbi->biBitCount == 8)
					xlatClut8(lpBits, SizeImage, xlat);
				else
					xlatClut4(lpBits, SizeImage, xlat);
				break;
	 }

    //  Now copy the RGBs in the logical palette to the dib color table
    for (n=0; n<nPalColors; n++) {
        GetPaletteEntries(hpal,n,1,&pe);
		lpRgb[n].rgbRed      = pe.peRed;
		lpRgb[n].rgbGreen    = pe.peGreen;
		lpRgb[n].rgbBlue     = pe.peBlue;
		lpRgb[n].rgbReserved = (BYTE)0;
	}

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////

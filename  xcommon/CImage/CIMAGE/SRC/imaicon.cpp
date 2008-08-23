// imaicon.cpp

/*
 * File:	ximaico.cpp
 * Purpose:	Platform Independent ICON Image Class Loader and Writer (MS version)
 * 07/Aug/2001 <ing.davide.pizzolato@libero.it>
 */
// Removed fopen/fclose mess - Troels K.

#include "..\include\cimage.h"
#pragma hdrstop
#include "imaicon.h"

///////////////////////////////////////////////////////////////////////////
BOOL CImageIcon::Read(FILE *hFile)
{
	int	page=m_info.nFrame;	//internal icon structure indexes

	// read the first part of the header
	ICONHEADER icon_header;
	fread(&icon_header,sizeof(ICONHEADER),1,hFile);
	// check if it's an icon
	if ((icon_header.idReserved == 0) && (icon_header.idType == 1)) {

		m_info.nNumFrames = icon_header.idCount;

		// load the icon descriptions
		ICONDIRENTRY *icon_list = (ICONDIRENTRY *)malloc(icon_header.idCount * sizeof(ICONDIRENTRY));
		int c;
		for (c = 0; c < icon_header.idCount; c++)
			fread(icon_list + c, sizeof(ICONDIRENTRY), 1, hFile);
		
		if ((m_info.nFrame>=0)&&(m_info.nFrame<icon_header.idCount)){
			// calculate the bit count for the colors in the icon
			switch (icon_list[page].bColorCount){
			case 2:
				m_header.biBitCount=1;
				m_header.biClrUsed=2;
				break;
			case 16:
				m_header.biBitCount=4;
				m_header.biClrUsed=16;
				break;
			default:
				m_header.biBitCount=8;
				m_header.biClrUsed=256;
			}
			m_header.biWidth  = icon_list[page].bWidth;
			m_header.biHeight = icon_list[page].bHeight;
			m_info.bColorType = (m_header.biBitCount>8) ? COLORTYPE_COLOR : COLORTYPE_PALETTE;
			m_info.nBkgndIndex =-1;

			// allocate memory for one icon
			Create(m_header.biWidth,m_header.biHeight,m_header.biBitCount);	//image creation
			// read the palette
			RGBQUAD pal[256];
			fseek(hFile, icon_list[page].dwImageOffset+sizeof(BITMAPINFOHEADER), SEEK_SET);
			fread(pal,m_header.biClrUsed*sizeof(RGBQUAD), 1, hFile);
			SetPalette(pal,m_header.biClrUsed);	//palette assign

			//read the icon
			fread(m_info.pImage, m_info.dwEffWidth*m_header.biHeight, 1, hFile);
			// apply the AND and XOR masks
         if(0)
         {
   		c=m_header.biWidth*m_header.biHeight/8; //size of masks
			BYTE *xor_mask = (BYTE *)malloc(c);
			BYTE *and_mask = (BYTE *)malloc(c);
			fread(xor_mask, c, 1, hFile);
			fread(and_mask, c, 1, hFile);
			for (int x = 0; x < m_header.biHeight; x++) {
				for (int y = 0; y < m_header.biWidth; y++) {
					if ((xor_mask[(y*m_header.biWidth+x)>>3]>>(x%8))&0x01){
						SetPixelColor(x,y,RGB(255,255,255));
					}
				}
			}
			free(and_mask);
			free(xor_mask);
         }
			free(icon_list);

			// icon has been loaded successfully!
			return TRUE;
		}
		free(icon_list);
	}
	return FALSE;
}

BOOL CImageIcon::Write(FILE* hFile)
{
	//check format limits
	if ((m_header.biBitCount>8)||(m_header.biWidth>255)||(m_header.biHeight>255)){
		strcpy(m_info.szLastError,"Can't save this image as icon");
		return FALSE;
	}

	//prepare the palette struct
	RGBQUAD* pal=GetPalette();
	if (pal==NULL) return FALSE;

	//fill the icon headers
	ICONHEADER icon_header={0,1,1};
	ICONDIRENTRY icon_list={(BYTE)m_header.biWidth,(BYTE)m_header.biHeight,(BYTE)m_header.biClrUsed ,0,0,m_header.biBitCount,
							sizeof(BITMAPINFOHEADER)+m_header.biClrUsed*sizeof(RGBQUAD)+
							m_header.biHeight*m_info.dwEffWidth+m_header.biHeight*m_header.biWidth/4,
							sizeof(ICONHEADER)+sizeof(ICONDIRENTRY)};
	BITMAPINFOHEADER bi={sizeof(BITMAPINFOHEADER),m_header.biWidth,2*m_header.biHeight,1,m_header.biBitCount,
						0,m_header.biHeight*m_info.dwEffWidth,0,0,0,0};

	fwrite(&icon_header,sizeof(ICONHEADER),1,hFile);			//write the headers
	fwrite(&icon_list,sizeof(ICONDIRENTRY),1,hFile);
	fwrite(&bi,sizeof(BITMAPINFOHEADER),1,hFile);
	fwrite(pal,m_header.biClrUsed*sizeof(RGBQUAD),1,hFile);			//write palette
	fwrite(m_info.pImage,m_info.dwEffWidth*m_header.biHeight,1,hFile);	//write image
	BYTE* buff=(BYTE*)calloc(m_header.biWidth*m_header.biHeight/4,1);	//create empty AND/XOR masks
	if (buff) 
   {
		fwrite(buff,m_header.biWidth*m_header.biHeight/4,1,hFile);		//write AND/XOR masks
		free(buff);
		return TRUE;
	}
	return FALSE;
}

BOOL CImageIcon::Open(HICON handle)
{
   // NOT IMPLEMENTED
   return FALSE;
}

///////////////////////////////////////////////////////////////////////////

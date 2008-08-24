/////////////////////////////////////////////////////////////////////////////
//
// NexgenIPL - Next Generation Image Processing Library
// Copyright (c) 1999-2004 Binary Technologies
// All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in this file is used in any commercial application 
// then a simple email would be nice.
//
// THIS SOFTWARE IS PROVIDED BY BINARY TECHNOLOGIES ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL BINARY TECHNOLOGIES OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// Binary Technologies
// http://www.binary-technologies.com
// info@binary-technologies.com
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BTCDECODERICO_INCLUDED
#define BTCDECODERICO_INCLUDED

#pragma once

#include "BTICodec.h"

typedef struct tagBTICONDIRENTRY
{
    unsigned char	bWidth;        // Width, in pixels, of the image
    unsigned char	bHeight;       // Height, in pixels, of the image
    unsigned char	bColorCount;   // Number of colors in image (0 if >=8bpp)
    unsigned char	bReserved;     // Reserved ( must be 0)
    unsigned short	wPlanes;       // Color Planes
    unsigned short	wBitCount;     // Bits per pixel
    unsigned long	dwBytesInRes;  // How many bytes in this resource?
    unsigned long	dwImageOffset; // Where in the file is this image?
} BTICONDIRENTRY, *LPBTICONDIRENTRY;

typedef struct tagBTICONHEADER
{
    unsigned short	idReserved;   // Reserved (must be 0)
    unsigned short	idType;       // Resource Type (1 for icons)
    unsigned short	idCount;      // How many images?
} BTICONHEADER, *LPBTICONHEADER;

class BTCDecoderOptionsICO : public BTCDecoderOptions
{
public:
	BTCDecoderOptionsICO()	:	m_nIconId(0),
								m_nIconCount(0) {}
	virtual ~BTCDecoderOptionsICO() {}

	enum {	BTDO_ICO_ICONINDEX = 0,
			BTDO_ICO_ICONCOUNT };

	virtual BTRESULT GetOption(const long lId, int &nValue);
	virtual BTRESULT SetOption(const long lId, const int nValue);

private:
	int	m_nIconId;    // [in]  Index of requested icon.
	int m_nIconCount; // [out] Number of icons in file.
};

class BTCDecoderICO : public BTCDecoder
{
public:
	BTCDecoderICO();
	virtual ~BTCDecoderICO();

	virtual BTRESULT IsValidType(BTCIStream* pIStream);

	virtual const bt_char *GetFormat();
	virtual const bt_char *GetDescription();
	virtual const bt_char *GetExtension();

	virtual bool Load(	BTCIStream*        pIStream,
						BTCImageData*      pImageData,
						BTINotification*   pNotification,
						BTCDecoderOptions* pOptions);
};

#endif // BTCDECODERICO_INCLUDED

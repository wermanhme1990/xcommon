#include "stdafx.h"
#include "BTCDecoderICO.h"

/////////////////////////////////////////////////////////////////////////////
// ICO decoder options implementation.
/////////////////////////////////////////////////////////////////////////////

BTRESULT BTCDecoderOptionsICO::GetOption(const long lId, int &nValue)
{
	if( BTDO_ICO_ICONINDEX == lId)
		nValue = m_nIconId;
	else if( BTDO_ICO_ICONCOUNT == lId)
		nValue = m_nIconCount;
	else
		return BTCDecoderOptions::GetOption( lId, nValue);

	return BT_S_OK;
}

BTRESULT BTCDecoderOptionsICO::SetOption(const long lId, const int nValue)
{
	if( BTDO_ICO_ICONINDEX == lId)
		m_nIconId = nValue;
	else if( BTDO_ICO_ICONCOUNT == lId)
		m_nIconCount = nValue;
	else
		return BTCDecoderOptions::SetOption( lId, nValue);

	return BT_S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// ICO decoder implementation.
/////////////////////////////////////////////////////////////////////////////

BTCDecoderICO::BTCDecoderICO()
{
}

BTCDecoderICO::~BTCDecoderICO()
{
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

const bt_char *BTCDecoderICO::GetFormat()      { return _BTTEXT("ICO"); }
const bt_char *BTCDecoderICO::GetDescription() { return _BTTEXT("Windows Icon"); }
const bt_char *BTCDecoderICO::GetExtension()   { return _BTTEXT("ico"); }

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

BTRESULT BTCDecoderICO::IsValidType(BTCIStream* pIStream)
{
	BTRESULT hResult = BT_S_FALSE;

	if( NULL == pIStream)
		return hResult;

	if( pIStream->Seek( 0L, BTCIOStream::Begin) == -1)
		return hResult;

	BTICONHEADER Header;

	pIStream->Read( (void*)&Header, sizeof(BTICONHEADER));

	if( Header.idType == 1 && Header.idReserved == 0)
		hResult = BT_S_OK;

	return hResult;
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

bool BTCDecoderICO::Load(BTCIStream*        pIStream,
                         BTCImageData*      pImageData,
                         BTINotification*   pNotification,
                         BTCDecoderOptions* pOptions)
{
	if(!pIStream)
		return false;

	BTICONHEADER     Header;
	LPBTICONDIRENTRY pIconList = NULL;

	int nIconId = 0;

	// Read icon header.
	pIStream->Read( (void*)&Header, sizeof(BTICONHEADER));

	// Allocate memory to hold icon descriptions.
	pIconList = (LPBTICONDIRENTRY)malloc( Header.idCount * sizeof(BTICONDIRENTRY));

	if(!pIconList)
		return false;

	// Read icon descriptions.
	pIStream->Read( (void*)pIconList, Header.idCount * sizeof(BTICONDIRENTRY));

	// Get requested icon index. If there are no options given, load first image.
	BTGETOPTION( pOptions, BTCDecoderOptionsICO::BTDO_ICO_ICONINDEX, nIconId);

	// Return number of icons in the file.
	BTSETOPTION( pOptions, BTCDecoderOptionsICO::BTDO_ICO_ICONCOUNT, Header.idCount);

	// Get requested icon.
	if( nIconId < Header.idCount)
	{
		BITMAPINFOHEADER bmih;

		pIStream->Seek( pIconList[nIconId].dwImageOffset, BTCIOStream::Begin);
		pIStream->Read( (void*)&bmih, sizeof(BITMAPINFOHEADER));

		int nWidth        = bmih.biWidth;
		int nHeight       = bmih.biHeight / 2;
		int nBitsPerPixel = bmih.biBitCount;

		if(!pImageData->Create( nWidth, nHeight, nBitsPerPixel))
		{
			free( pIconList);
			return false;
		}

		int nBytesPerLine = pImageData->GetBytesPerLine();
//		int nColorCount   = pIconList[nIconId].bColorCount == 0 ? bmih.biClrUsed : pIconList[nIconId].bColorCount;
		int nColorCount   = BTCImageData::GetNumColorEntries( nBitsPerPixel);

		// Read color table.
		pIStream->Read( (void*)pImageData->GetColorTable(), nColorCount * sizeof(RGBQUAD));

		// Extract image data.
		unsigned char* pXORMask    = (unsigned char*)malloc( nHeight * nBytesPerLine);
		unsigned char* pANDMask    = (unsigned char*)malloc( nHeight * nBytesPerLine);

		unsigned char* pXORMaskPtr = pXORMask;
		unsigned char* pANDMaskPtr = pANDMask;

		pIStream->Read( pXORMaskPtr, nHeight * nBytesPerLine);
		pIStream->Read( pANDMaskPtr, nHeight * nBytesPerLine);

		for( register int y = 0; y < nHeight; y++)
		{
			unsigned char* pBits = pImageData->GetBits( 0, nHeight - y - 1);

			for( register int x = 0; x < nBytesPerLine; x++)
			{
				*(pBits + x) &= pANDMaskPtr[x];
				*(pBits + x) ^= pXORMaskPtr[x];
			}

			pXORMaskPtr += nBytesPerLine;
			pANDMaskPtr += nBytesPerLine;

			if( pNotification)	// Step progress.
				pNotification->OnProgress( (int)((100.0 / nHeight) * y) + 1);
		}

		free( pANDMask);
		free( pXORMask);
	}

	free( pIconList);

	return true;
}

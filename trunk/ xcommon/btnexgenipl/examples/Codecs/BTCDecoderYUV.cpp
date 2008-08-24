#include "stdafx.h"

#include "BTDefines.h"
#include "BTCDecoderYUV.h"

BTCDecoderYUV::BTCDecoderYUV()
{
}

BTCDecoderYUV::~BTCDecoderYUV()
{
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

const bt_char *BTCDecoderYUV::GetFormat()      { return _BTTEXT("YUV"); }
const bt_char *BTCDecoderYUV::GetDescription() { return _BTTEXT("ITU-R BT 601 (CCIR 601) 4:2:2"); }
const bt_char *BTCDecoderYUV::GetExtension()   { return _BTTEXT("yuv"); }

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

BTRESULT BTCDecoderYUV::IsValidType(BTCIStream* pIStream)
{
	BTRESULT hResult = BT_S_FALSE;

	if(!pIStream)
		return hResult;

	if( pIStream->Seek( 0L, BTCIOStream::Begin) == -1)
		return hResult;

	long lSize = pIStream->GetSize();

	if( -1 == lSize)
		return hResult;

	int nWidth  = lSize < 500000 ? 360 : 720;
	int nHeight = lSize / (2 * nWidth);

	if((nHeight == 576 || nHeight == 486) && (nWidth == 360 || nWidth == 720))
		hResult = BT_S_OK;

	return hResult;
}

BTRGBQUAD bt_yuv2rgb(double y, double u, double v)
{
	double r, g, b;

	u -= 128.;
	v -= 128.;
	y  = 1.164 * (y - 16.);

	r = v * (2. - 2. * 0.299) + y;
	b = u * (2. - 2. * 0.114) + y;
	g = (y - 0.114 * b - 0.299 * r) / 0.587;
/*
	Default:
	r = v * (2. - 2. * Coef. for red)  + y;
	b = u * (2. - 2. * Coef. for blue) + y;
	g = (y - Coef. for blue * b - Coef. for red * r) / Coef. for green;

	Rec 601-1:
	r = v * (2. - 2. * 0.299) + y;
	b = u * (2. - 2. * 0.114) + y;
	g = (y - 0.114 * b - 0.299 * r) / 0.587;

	Rec 709/ITU:
	r = v * (2. - 2. * 0.2125) + y;
	b = u * (2. - 2. * 0.0721) + y;
	g = (y - 0.0721 * b - 0.2125 * r) / 0.7154;
*/
	if( r > 255.) r = 255.;
	if( g > 255.) g = 255.;
	if( b > 255.) b = 255.;

	if( r < 0.) r = 0.;
	if( g < 0.) g = 0.;
	if( b < 0.) b = 0.;

	BTRGBQUAD rgba;

	rgba.rgbRed      = (unsigned char)r;
	rgba.rgbGreen    = (unsigned char)g;
	rgba.rgbBlue     = (unsigned char)b;
	rgba.rgbReserved = 0;

	return rgba;
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

bool BTCDecoderYUV::Load(BTCIStream*        pIStream,
                         BTCImageData*      pImageData,
                         BTINotification*   pNotification,
                         BTCDecoderOptions* pOptions)
{
	if(!pIStream || !pImageData)
		return false;

	long lSize = pIStream->GetSize();

	if( -1 == lSize)
		return false;

	int nWidth  = lSize < 500000 ? 360 : 720;
	int nHeight = lSize / (2 * nWidth);

	if(!pImageData->Create( nWidth, nHeight, 24))
		return false;

	BTRGBQUAD     rgba;
	unsigned char uyvy[4];

	for( register int yy = 0; yy < nHeight; yy++)
	{
		for( register int xx = 0; xx < nWidth;)
		{
			pIStream->Read( &uyvy, 4);

			rgba = bt_yuv2rgb( uyvy[1], uyvy[0], uyvy[2]);

			pImageData->SetColorForPixel( xx++, yy, BTRGB( rgba.rgbRed, rgba.rgbGreen, rgba.rgbBlue));

			rgba = bt_yuv2rgb( uyvy[3], uyvy[0], uyvy[2]);

			pImageData->SetColorForPixel( xx++, yy, BTRGB( rgba.rgbRed, rgba.rgbGreen, rgba.rgbBlue));
		}

		if( pNotification) // Step progress.
			pNotification->OnProgress( (int)((100.0 / nHeight) * yy) + 1);
	}

	return true;
}

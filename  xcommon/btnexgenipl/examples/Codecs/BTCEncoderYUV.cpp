#include "stdafx.h"

#include "BTDefines.h"
#include "BTCEncoderYUV.h"

BTCEncoderYUV::BTCEncoderYUV()
{
}

BTCEncoderYUV::~BTCEncoderYUV()
{
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

const bt_char *BTCEncoderYUV::GetFormat()      { return _BTTEXT("YUV"); }
const bt_char *BTCEncoderYUV::GetDescription() { return _BTTEXT("ITU-R BT 601 (CCIR 601) 4:2:2"); }
const bt_char *BTCEncoderYUV::GetExtension()   { return _BTTEXT("yuv"); }

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

bool BTCEncoderYUV::Save(BTCOStream*        pOStream,
                         BTCImageData*      pImageData,
                         BTINotification*   pNotification,
                         BTCEncoderOptions* pOptions)
{
    if(!pOStream || !pImageData)
        return false;

	int nWidth  = pImageData->GetWidth();
	int nHeight = pImageData->GetHeight();

	// Check whether the image has the correct image size.
	if(!((nHeight == 576 || nHeight == 486) && (nWidth == 360 || nWidth == 720)))
		return false;

	BTCOLORREF    col;
	unsigned char uyvy[4];
	double        r, g, b, Y1, Cb1, Cr1, Y2, Cb2, Cr2;

	for( register int y = 0; y < nHeight; y++)
	{
		for( register int x = 0; x < nWidth;)
		{
			col = pImageData->GetColorFromPixel( x++, y);

			r = BTGetRValue( col);
			g = BTGetGValue( col);
			b = BTGetBValue( col);

			/*
			Rec 601-1:
			Y  = 0.299 * r + 0.587 * g + 0.114 * b;
			Cb = (b - Y) / (2 - 2 * 0.114);
			Cr = (r - Y) / (2 - 2 * 0.299);

			Rec 709:
			Y  = 0.2125 * r + 0.7154 * g + 0.0721 * b;
			Cb = (b - Y) / (2 - 2 * 0.0721);
			Cr = (r - Y) / (2 - 2 * 0.2125);
			*/

			Y1  = 0.299 * r + 0.587 * g + 0.114 * b;
			Cb1 = (b - Y1) / (2. - 2. * 0.114);
			Cr1 = (r - Y1) / (2. - 2. * 0.299);

			col = pImageData->GetColorFromPixel( x++, y);

			r = BTGetRValue( col);
			g = BTGetGValue( col);
			b = BTGetBValue( col);

			Y2  = 0.299 * r + 0.587 * g + 0.114 * b;
			Cb2 = (b - Y2) / (2. - 2. * 0.114);
			Cr2 = (r - Y2) / (2. - 2. * 0.299);

			uyvy[0] = (unsigned char)((128. + (Cb1 + Cb2) / 2) * 224./255.+ 16.); // u
            uyvy[1] = (unsigned char)(Y1 * 219./255. + 16.);                      // y1
            uyvy[2] = (unsigned char)((128. + (Cr1 + Cr2) / 2) * 224./255.+ 16.); // v
            uyvy[3] = (unsigned char)(Y2 * 219./255. + 16.);                      // y2

			pOStream->Write( &uyvy, 4);
		}

		if( pNotification) // Step progress.
			pNotification->OnProgress( (int)((100.0 / nHeight) * y) + 1);
	}

	return true;
}

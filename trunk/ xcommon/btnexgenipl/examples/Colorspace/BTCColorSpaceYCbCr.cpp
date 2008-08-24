#include "stdafx.h"
#include "BTCColorSpaceYCbCr.h"

// Color spaces FAQ - David Bourgin:
//
// +----------------+---------------+-----------------+----------------+
// | Recommendation | Coef. for red | Coef. for Green | Coef. for Blue |
// +----------------+---------------+-----------------+----------------+
// | Rec 601-1      | 0.2989        | 0.5866          | 0.1145         |
// | Rec 709        | 0.2126        | 0.7152          | 0.0722         |
// | ITU            | 0.2220        | 0.7067          | 0.0713         |
// +----------------+---------------+-----------------+----------------+
//
// RGB -> YCbCr:
// Y  = Coef. for red * Red + Coef. for green * Green + Coef. for blue * Blue
// Cb = (Blue - Y) / (2 - 2 * Coef. for blue)
// Cr = (Red  - Y) / (2 - 2 * Coef. for red)
//
// YCbCr -> RGB:
// Red   = Cr * (2 - 2 * Coef. for red) + Y
// Green = (Y - Coef. for blue * Blue - Coef. for red * Red) / Coef. for green
// Blue  = Cb * (2 - 2 * Coef. for blue) + Y

int BTCColorSpaceYCbCr::GetNumComponents()
{
	return BTColorSpace_YCbCr_MaxNumComponents;
}

BTColorSpace BTCColorSpaceYCbCr::GetType()
{
	return BTColorSpace_YCbCr;
}

bool BTCColorSpaceYCbCr::FromCIEXYZ(double xyz[3], double *col)
{
	BTIColorSpace *pIColorSpace_sRGB = GetColorSpace( BTColorSpace_sRGB);
	assert( pIColorSpace_sRGB);

	if( NULL == pIColorSpace_sRGB)
		return false;

	double srgb[3];

	// CIEXYZ (D50) -> sRGB (D65).
	if( false == pIColorSpace_sRGB->FromCIEXYZ( xyz, srgb))
		return false;

	// sRGB -> YCbCr.
	return FromRGB( srgb, col);
}

bool BTCColorSpaceRGB::FromRGB(double rgb[3], double *col)
{
	BTIColorSpace *pIColorSpace_RGB = GetColorSpace( BTColorSpace_RGB);
	assert( pIColorSpace_RGB);

	if( NULL == pIColorSpace_RGB)
		return false;

	double _rgb[3];

	// Non-linear sRGB -> linear RGB.
	if( false == pIColorSpace_RGB->FromRGB( rgb, _rgb))
		return false;

	// RGB -> YCbCr (with Rec 601-1 specs).
    col[0] =  0.2989 * _rgb[0] + 0.5866 * _rgb[1] + 0.1145 * _rgb[2]; // Y
    col[1] = -0.1688 * _rgb[0] - 0.3312 * _rgb[1] + 0.5000 * _rgb[2]; // Cb
    col[2] =  0.5000 * _rgb[0] - 0.4184 * _rgb[1] - 0.0816 * _rgb[2]; // Cr

	return true;
}

bool BTCColorSpaceYCbCr::ToCIEXYZ(double col[], double *xyz)
{
	double srgb[3];

	// YCrCb -> sRGB.
	if(!ToRGB( col, srgb))
		return false;

	BTIColorSpace *pIColorSpace_sRGB = GetColorSpace( BTColorSpace_sRGB);
	assert( pIColorSpace_sRGB);

	if( NULL == pIColorSpace_sRGB)
		return false;

	// sRGB (D65) -> CIEXYZ (D50).
	return pIColorSpace_sRGB->ToCIEXYZ( srgb, xyz);
}

bool BTCColorSpaceYCbCr::ToRGB(double col[], double *rgb)
{
	double _rgb[3];

	// YCbCr (with Rec 601-1 specs) -> RGB.
    _rgb[0] = col[0] + 0.0000 * col[1] + 1.4022 * col[2]; // R
    _rgb[1] = col[0] - 0.3456 * col[1] - 0.7145 * col[2]; // G
    _rgb[2] = col[0] + 1.7710 * col[1] + 0.0000 * col[2]; // B

	BTIColorSpace *pIColorSpace_RGB = GetColorSpace( BTColorSpace_RGB);
	assert( pIColorSpace_RGB);

	if( NULL == pIColorSpace_RGB)
		return false;

	// Linear RGB -> Non-linear sRGB.
	return pIColorSpace_RGB->ToRGB( _rgb, rgb);
}

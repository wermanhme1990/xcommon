#include "stdafx.h"
#include "BTCImageDataEx.h"

BTCImageDataEx::BTCImageDataEx()
{
}

BTCImageDataEx::BTCImageDataEx(const BTCImageData& ImageData)
{
	if( true == ImageData.IsLoaded())
		(BTCImageData&)*this = (BTCImageData&)ImageData;
}

BTCImageDataEx::~BTCImageDataEx()
{
}

BTCImageData& BTCImageDataEx::operator+=(BTCImageData& ImageData)
{
	if( true == IsLoaded() && true == ImageData.IsLoaded())
		Combine( &ImageData, BTCombineFunction::Normal);

	return *this;
}

BTCImageData& BTCImageDataEx::operator-=(BTCImageData& ImageData)
{
	if( true == IsLoaded() && true == ImageData.IsLoaded())
		Combine( &ImageData, BTCombineFunction::Subtract);

	return *this;
}

BTCImageData& BTCImageDataEx::operator*=(BTCImageData& ImageData)
{
	if( true == IsLoaded() && true == ImageData.IsLoaded())
		Combine( &ImageData, BTCombineFunction::Multiply);

	return *this;
}

BTCImageData& BTCImageDataEx::operator/=(BTCImageData& ImageData)
{
	if( true == IsLoaded() && true == ImageData.IsLoaded())
		Combine( &ImageData, BTCombineFunction::Divide);

	return *this;
}

BTCImageData& BTCImageDataEx::operator&=(BTCImageData& ImageData)
{
	if( true == IsLoaded() && true == ImageData.IsLoaded())
		Combine( &ImageData, BTCombineFunction::And);

	return *this;
}

BTCImageData& BTCImageDataEx::operator|=(BTCImageData& ImageData)
{
	if( true == IsLoaded() && true == ImageData.IsLoaded())
		Combine( &ImageData, BTCombineFunction::Or);

	return *this;
}

BTRESULT BTCImageDataEx::Append(BTCImageData *pImage, long x, long y)
{
     if(!m_pBitmapInfo || !pImage)
          return BT_E_POINTER;

     long lNewWidth  = GetWidth();
     long lNewHeight = GetHeight();

     long lWidth  = pImage->GetWidth();
     long lHeight = pImage->GetHeight();

     if( x + lWidth > GetWidth())
          lNewWidth = x + lWidth;

     if( y + lHeight > GetHeight())
          lNewHeight = y + lHeight;

     if( lNewWidth != GetWidth() || lNewHeight != GetHeight())
     {
          BTCImageData NewImage;

          if( false == NewImage.Create( lNewWidth, lNewHeight, GetBitsPerPixel()))
               return BT_S_FALSE;

          if( false == NewImage.Combine( this, BTCImageData::BTCombineFunction::Normal, 0, 0))
               return BT_S_FALSE;

          (BTCImageData&)*this = NewImage;
     }

     return Combine( pImage, BTCImageData::BTCombineFunction::Normal, x, y) ? BT_S_OK : BT_S_FALSE;
}

BTRESULT BTCImageDataEx::RotateEx(double dAngle, const enum BTDirection& Direction /* = Right */, BTCOLORREF colBackground /* = 0x00000000 */, bool bCrop /* = true */)
{
	if(!m_pBitmapInfo)
		return BT_E_POINTER;

	long lOldWidth  = GetWidth();
	long lOldHeight = GetHeight();

	if( false == Rotate( dAngle, Direction, colBackground))
		return BT_S_FALSE;

	if( true == bCrop)
	{
		long x = (GetWidth() / 2) - (lOldWidth / 2);
		long y = (GetHeight() / 2) - (lOldHeight / 2);

		return Crop( x, y, lOldWidth, lOldHeight) ? BT_S_OK : BT_S_FALSE;
	}

	return BT_S_OK;
}

/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XDirection.cpp				
文件内容:	    整型方向运算
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2007/08/01
*********************************************************************/

#include "StdAfx.h"
#include "XDirection.h"
static LONG BO_LeftShift( const LONG& nValue, const LONG& nBit )
{
  return (nValue << nBit );
};
static LONG  Drctn_GetMultiRsltnDrctn(const BYTE *const pbDirectionScale, const WORD *const pwBaseTime,
                                      const LONG& nX, const LONG& nY )
{
	LONG nSlope, nMin, nMax, wIdx;
  LONG nDeltaX = nX, nDeltaY = nY;
	LONG nMark = 0;
    if( nDeltaX < 0 )
	{
		nMark |= 1;
		nDeltaX = -nDeltaX;
	}
	
	if( nDeltaY < 0 )
		nDeltaY = -nDeltaY;
	else
		nMark |= 2;
	
	if( !nDeltaY )
	{
		if( nMark & 1 )
			return pwBaseTime[4];//180 --> 128 = 4*nBase
		else
			return 0;//0
	}
	
	if( !nDeltaX )
	{
		if( nMark & 2 )
			return pwBaseTime[2];//90 --> 64 = nBase *2
		else
			return pwBaseTime[6];//270 =192
	}
	
	if( nDeltaX == nDeltaY )
	{
		if( !nMark )// nMrk == 0
			return pwBaseTime[7];//315 --> 224 = nBase *7
		else if( 1 == nMark )
			return pwBaseTime[5];//225 --> 160 = nBase *5
		else if( 2 == nMark )
			return pwBaseTime[1];//45    =32
		else
			return pwBaseTime[3];//135  =96
	}
	
	if( nDeltaX > nDeltaY )
	{
		nMax = nDeltaX;
		nMin = nDeltaY;
	}
	else
	{
		nMax = nDeltaY;
		nMin = nDeltaX;
	}
	
	nSlope = BO_LeftShift(nMin, 8) / nMax, wIdx = 0;// *256
	while( pbDirectionScale[wIdx] < nSlope )
		wIdx++;
	
	if( nDeltaX < nDeltaY )
		wIdx = pwBaseTime[2] - wIdx;// 2*nBase
	
	if( !nMark )
	{// 
		if( wIdx )
			return  pwBaseTime[8] - wIdx;// 8*nBase
		else
			return 0;
	}
	else if( 1 == nMark )
		return pwBaseTime[4] + wIdx;
	else if( 2 == nMark)
		return wIdx;
	else// nMark == 3
		return pwBaseTime[4] - wIdx;
}
// 256 level
LONG  WINAPI XDirection256(const LONG& nDeltaX, const LONG& nDeltaY )
{
	//const LONG nBase=32;
	BYTE abDirectionScale[33]={
		3,  9, 16, 22, 28,    35, 42, 48, 53, 62,
			68, 74, 81, 88, 95,   102,110,117,125,133,
			141,150,158,167,176,   185,195,205,215,226,
			238,250,255
	};
	WORD  awBaseTime[9]= {0, 32, 64, 96, 128, 160, 192, 224, 256};
    return Drctn_GetMultiRsltnDrctn( abDirectionScale, awBaseTime, nDeltaX, nDeltaY );
}

// 32 level direction
LONG  WINAPI XDirection32(const LONG& nDeltaX, const LONG& nDeltaY )
{
	//const LONG nBase = 8;
	BYTE abDirectionScale[5]= { 25,  78,  137, 213, 255 };
	WORD  awBaseTime[9] = {0, 4, 8, 12, 16, 20, 24, 28, 32};
    return Drctn_GetMultiRsltnDrctn( abDirectionScale, awBaseTime, nDeltaX, nDeltaY );
}

// 16 level direction
LONG  WINAPI XDirection16(const LONG& nDeltaX, const LONG& nDeltaY )
{
	//const LONG nBase = 8;
	BYTE abDirectionScale[3] = { 51, 171, 255 };
	WORD  awBaseTime[9] = { 0, 2, 4, 6, 8, 10, 12, 14, 16};
    return Drctn_GetMultiRsltnDrctn( abDirectionScale, awBaseTime, nDeltaX, nDeltaY );
}

// 8 level direction
LONG  WINAPI XDirection8(const LONG& nDeltaX, const LONG& nDeltaY )
{
	BYTE abDirectionScale[2] = { 106, 255 };
	WORD  awBaseTime[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8};
    return Drctn_GetMultiRsltnDrctn( abDirectionScale, awBaseTime, nDeltaX, nDeltaY );
}


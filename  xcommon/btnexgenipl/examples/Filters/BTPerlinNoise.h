/////////////////////////////////////////////////////////////////////////////
//
// NexgenIPL - Next Generation Image Processing Library
// Copyright (c) 1999-2003 Binary Technologies
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

#ifndef BTPERLINNOISE_INCLUDED
#define BTPERLINNOISE_INCLUDED

#pragma once

#include <math.h>
#include "BTCImageData.h"

#ifndef PI
#define PI 3.14159265358979323846264338327950288419716939937510
#endif

inline double LinearInterpolate(double a, double b, double x) { return a * (1.0 - x) + b * x; }
inline double CosineInterpolate(double a, double b, double x) { double ft = x * PI; double f  = (1.0 - cos(ft)) * 0.5; return a * (1.0 - f) + b * f; }
inline double CubicInterpolate(double v0, double v1, double v2, double v3, double x) { double P = (v3 - v2) - (v0 - v1); double Q = (v0 - v1) - P; double R = v2 - v0; double S = v1; return P * (x * x * x) + Q * (x * x) + R * x + S; }

double Noise(int x, int y)
{
    static int n;

	n = x + y * 57;
    n = (n<<13) ^ n;

    return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

double SmoothNoise(int x, int y)
{
	double dCorners = (Noise( x - 1, y - 1) + Noise( x + 1, y - 1) + Noise( x - 1, y + 1) + Noise( x + 1, y + 1)) / 16;
	double dSides   = (Noise( x - 1, y) + Noise( x + 1, y) + Noise( x, y - 1) + Noise( x, y + 1)) / 8;
	double dCenter  =  Noise( x, y) / 4;

    return dCorners + dSides + dCenter;
}

double InterpolatedNoise(double x, double y)
{
	int    nX, nY;
	double dX, dY, v1, v2, v3, v4, i1, i2;

	nX = (int)x;
	dX = x - nX;

	nY = (int)y;
	dY = y - nY;

	v1 = SmoothNoise( nX,     nY);
	v2 = SmoothNoise( nX + 1, nY);
	v3 = SmoothNoise( nX,     nY + 1);
	v4 = SmoothNoise( nX + 1, nY + 1);

	i1 = CosineInterpolate( v1, v2, dX);
	i2 = CosineInterpolate( v3, v4, dX);

	return CosineInterpolate( i1, i2, dY);
}

// Taken from http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
double PerlinNoise(double x, double y, int nNumberOfOctaves, double dPersistence)
{
	double dTotal     = 0.0;
	double dFrequency = 0.0;
	double dAmplitude = 0.0;

	for( register int i = 0; i < (nNumberOfOctaves - 1); ++i)
	{
		dFrequency = pow( 2, i);
		dAmplitude = pow( dPersistence, i);

		dTotal = dTotal + InterpolatedNoise( x * dFrequency, y * dFrequency) * dAmplitude;
	}

	return dTotal;
}

bool BTPerlinNoise(BTCImageData *pImage = NULL, int nNumberOfOctaves = 4, double dPersistence = 0.5, double dMultiplicator = 30.0)
{
	if( NULL == pImage || pImage->GetBitsPerPixel() < 24)
		return false;

	long lWidth  = pImage->GetWidth();
	long lHeight = pImage->GetHeight();

	BTCOLORREF    col;
	unsigned char rgba[4];
	int           nValue;

	for( register long y = 0; y < lHeight; ++y)
	{
	    for( register long x = 0; x < lWidth; ++x)
		{
			col = pImage->GetColorFromPixel( x, y);

			rgba[0] = BTGetRValue( col); // R
			rgba[1] = BTGetGValue( col); // G
			rgba[2] = BTGetBValue( col); // B
			rgba[3] = BTGetAValue( col); // A

			nValue  = (int)(PerlinNoise( x, y, nNumberOfOctaves, dPersistence) * dMultiplicator);

			for( int i = 0; i < 3; ++i)
			{
				if( rgba[i] + nValue > 255)
					rgba[i] = 255;
				else if( rgba[i] + nValue < 0)
					rgba[i] = 0;
				else
					rgba[i] += nValue;
			}

			pImage->SetColorForPixel( x, y, BTRGBA( rgba[0], rgba[1], rgba[2], rgba[3]));
		}
	}

	return true;
}

#endif // BTPERLINNOISE_INCLUDED
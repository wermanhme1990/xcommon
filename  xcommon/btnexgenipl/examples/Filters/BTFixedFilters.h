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

#ifndef BTFIXEDFILTERS_INCLUDED
#define BTFIXEDFILTERS_INCLUDED

#pragma once

// Borderize (Kernelsize = 5, Division = 1, Bias = 0)
static double BTBorderizeKernel[] = { 0, 1,   2, 1, 0,
                                      1, 2,   3, 2, 1,
                                      2, 3, -35, 3, 2,
                                      1, 2,   3, 2, 1,
                                      0, 1,   2, 1, 0 };

// Define (Kernelsize = 7, Division = 1, Bias = 0)
static double BTDefineKernel[] = { -5, -2, 0, 0, 0, 0, 0, 
                                   -2,  0, 0, 0, 0, 0, 0, 
                                    0,  0, 0, 0, 0, 0, 0,
                                    0,  0, 0, 0, 0, 0, 0,
                                    0,  0, 0, 0, 0, 0, 0,
                                    0,  0, 0, 0, 0, 0, 2,
                                    0,  0, 0, 0, 0, 2, 6 };

// Big Edge (Kernelsize = 5, Division = 1, Bias = 0)
static double BTBigEdgeKernel[] = { -2, -2, -2, -2, -2,
                                    -2, -3, -3, -3, -2,
                                    -2, -3, 53, -3, -2,
                                    -2, -3, -3, -3, -2,
                                    -2, -2, -2, -2, -2 };

// Defocus (Kernelsize = 3, Division = 1, Bias = 0)
static double BTDefocusKernel[] = { 1,  1, 1,
                                    1, -7, 1,
                                    1,  1, 1 };

// Edge Detect (Kernelsize = 3, Division = 1, Bias = 50)
static double BTEdgeDetectKernel[] = { -5, -5, -5,
                                -5, 39, -5,
                                -5, -5, -5 };

// Highlight Filter (Kernelsize = 5, Division = 1, Bias = 0)
static double BTHighlightKernel[] = { -1, 0, 0, 0, 0,
                                       0, 0, 0, 0, 0,
                                       0, 0, 0, 0, 0,
                                       0, 0, 0, 0, 0,
                                       0, 0, 0, 0, 2 };

// Sharpen (Kernelsize = 3, Division = 1, Bias = 0)
static double BTSharpenKernel[] = { -1/8, -1/8, -1/8,
                                    -1/8,    2, -1/8,
                                    -1/8, -1/8, -1/8 };

// Average (Kernelsize = 3, Division = 1, Bias = 0)
static double BTAverageKernel[] = { 1, 1, 1,
                                    1, 1, 1,
                                    1, 1, 1 };

// Sharpen More (Kernelsize = 3, Division = 1, Bias = 0)
static double BTSharpenMoreKernel[] = { -1, -1, -1,
                                        -1,  9, -1,
                                        -1, -1, -1 };

// Prewitt Vertical (Kernelsize = 3, Division = 1, Bias = 0)
static double BTPrewittVerticalKernel[] = { -1, 0, 1,
                                            -1, 0, 1,
                                            -1, 0, 1 };

// Prewitt Horizontal (Kernelsize = 3, Division = 1, Bias = 0)
static double BTPrewittHorizontalKernel[] = {  1,  1,  1,
                                               0,  0,  0,
                                              -1, -1, -1 };

// Sobel Vertical (Kernelsize = 3, Division = 1, Bias = 0)
static double BTSobelVerticalKernel[] = { -1, 0, 1,
                                          -2, 0, 2,
                                          -1, 0, 1 };

// Sobel Horizontal (Kernelsize = 3, Division = 1, Bias = 0)
static double BTSobelHorizontalKernel[] = {  1,  2,  1,
                                             0,  0,  0,
                                            -1, -2, -1 };

// Laplacian (Kernelsize = 3, Division = 1, Bias = 0)
static double BTLaplacianKernel[] = { -1, -1, -1,
                                      -1,  8, -1,
                                      -1, -1, -1 };

// Laplacian 5x5 (Kernelsize = 5, Division = 1, Bias = 0)
static double BTLaplacian5x5Kernel[] = { -1, -3, -4, -3, -1,
                                         -3,  0,  6,  0, -3,
                                         -4,  6, 20,  6, -4,
                                         -3,  0,  6,  0, -3,
                                         -1, -3, -4, -3, -1 };

// Gaussian (Kernelsize = 3, Division = 1, Bias = 0)
static double BTGaussianKernel[] = { 1, 2, 1,
                                     2, 4, 2,
                                     1, 2, 1 };

// Gaussian 5x5 (Kernelsize = 5, Division = 1, Bias = 0)
static double BTGaussian5x5Kernel[] = {  2,  7,  12,  7,  2,
                                         7, 31,  52, 31,  7,
                                        12, 52, 127, 52, 12,
                                         7, 31,  52, 31,  7,
                                         2,  7,  12,  7,  2 };

// High Pass (Kernelsize = 3, Division = 1, Bias = 0)
static double BTHighPassKernel[] = { -1, -1, -1,
                                     -1,  8, -1,
                                     -1, -1, -1 };

// High Pass 5x5 (Kernelsize = 5, Division = 1, Bias = 0)
static double BTHighPass5x5Kernel[] = { -1, -1, -1, -1, -1, 
                                        -1, -1, -1, -1, -1,
                                        -1, -1, 24, -1, -1,
                                        -1, -1, -1, -1, -1,
                                        -1, -1, -1, -1, -1 };

// Low Pass 1 (Kernelsize = 3, Division = 1, Bias = 0)
static double BTLowPass1Kernel[] = { 1, 1, 1,
                                     1, 2, 1,
                                     1, 1, 1 };

// Low Pass 2 (Kernelsize = 3, Division = 1, Bias = 0)
static double BTLowPass2Kernel[] = { 1, 1, 1,
                                     1, 4, 1,
                                     1, 1, 1 };

// Low Pass 3 (Kernelsize = 3, Division = 1, Bias = 0)
static double BTLowPass3Kernel[] = { 1,  1, 1,
                                     1, 12, 1,
                                     1,  1, 1 };

// Contour 1 (Kernelsize = 3, Division = 1, Bias = 0)
static double BTContour1Kernel[] = {  1, 0, -1,
                                      0, 0,  0,
                                     -1, 0,  1 };

// Contour 2 (Kernelsize = 3, Division = 1, Bias = 0)
static double BTContour2Kernel[] = { -1, 0,  1,
                                      2, 0, -2,
                                     -1, 0,  1 };

// Contour 3 (Kernelsize = 3, Division = 1, Bias = 0)
static double BTContour3Kernel[] = { -1,  2, -1,
                                      0,  0,  0,
                                      1, -2,  1 };

// Deep Emboss (Kernelsize = 7, Division = 3, Bias = 52)
static double BTDeepEmbossKernel[] = { -1, 0,  0,  1, 0, 0, 1,
                                        1, 0,  0,  0, 0, 0, 1,
                                        0, 0, -1, -1, 0, 0, 0,
                                        0, 0,  1, -5, 1, 0, 0,
                                        0, 0,  0,  0, 1, 0, 1,
                                        0, 0,  0,  0, 0, 0, 0,
                                       -1, 0,  0,  0, 0, 0, 1 };

// Foil Engrave (Kernelsize = 7, Division = 8, Bias = 0)
static double BTFoilEngraveKernel[] = { 0, 0, 0,   1,   0,  1, -6,
                                        0, 0, 0, -13,  -2,  0,  1,
                                        0, 0, 0,  -2,  -4,  0,  1,
                                        0, 0, 1,   0, -15,  0,  5,
                                        0, 0, 0,  30,   1, -1,  2,
                                        0, 0, 0,   0,   0,  0,  0,
                                        0, 0, 0,   0,   2,  0,  1 };

class BTCGenericFixedFilter
{
public:
	BTCGenericFixedFilter(int nKernelSize, int nDivisor, int nBias)
	 : m_nKernelSize(nKernelSize), m_nDivisor(nDivisor), m_nBias(nBias) {}

	virtual ~BTCGenericFixedFilter() {}

	int GetKernelSize() const { return m_nKernelSize; }
	int GetDivisor()    const { return m_nDivisor; }
	int GetBias()       const { return m_nBias; }

	virtual double *GetKernel() = 0;

protected:
    int m_nKernelSize;
	int m_nDivisor;
	int m_nBias;
};

class BTCBorderizeFixedFilter : public BTCGenericFixedFilter
{
public:
    BTCBorderizeFixedFilter() : BTCGenericFixedFilter( 5, 1, 0) {}
    virtual ~BTCBorderizeFixedFilter() {}

	virtual double *GetKernel() { return BTBorderizeKernel; }
};

class BTCDefineFixedFilter : public BTCGenericFixedFilter
{
public:
    BTCDefineFixedFilter() : BTCGenericFixedFilter( 7, 1, 0) {}
    virtual ~BTCDefineFixedFilter() {}

	virtual double *GetKernel() { return BTDefineKernel; }
};

class BTCBigEdgeFixedFilter : public BTCGenericFixedFilter
{
public:
    BTCBigEdgeFixedFilter() : BTCGenericFixedFilter( 5, 1, 0) {}
    virtual ~BTCBigEdgeFixedFilter() {}

	virtual double *GetKernel() { return BTBigEdgeKernel; }
};

class BTCDefocusFixedFilter : public BTCGenericFixedFilter
{
public:
    BTCDefocusFixedFilter() : BTCGenericFixedFilter( 3, 1, 0) {}
    virtual ~BTCDefocusFixedFilter() {}

	virtual double *GetKernel() { return BTDefocusKernel; }
};

class BTCEdgeDetectFixedFilter : public BTCGenericFixedFilter
{
public:
    BTCEdgeDetectFixedFilter() : BTCGenericFixedFilter( 3, 1, 50) {}
    virtual ~BTCEdgeDetectFixedFilter() {}

	virtual double *GetKernel() { return BTEdgeDetectKernel; }
};

class BTCHighlightFixedFilter : public BTCGenericFixedFilter
{
public:
    BTCHighlightFixedFilter() : BTCGenericFixedFilter( 5, 1, 0) {}
    virtual ~BTCHighlightFixedFilter() {}

	virtual double *GetKernel() { return BTHighlightKernel; }
};

class BTCAverageFixedFilter : public BTCGenericFixedFilter
{
public:
    BTCAverageFixedFilter() : BTCGenericFixedFilter( 3, 1, 0) {}
    virtual ~BTCAverageFixedFilter() {}

	virtual double *GetKernel() { return BTAverageKernel; }
};

class BTCSharpenFixedFilter : public BTCGenericFixedFilter
{
public:
    BTCSharpenFixedFilter() : BTCGenericFixedFilter( 3, 1, 0) {}
    virtual ~BTCSharpenFixedFilter() {}

	virtual double *GetKernel() { return BTSharpenKernel; }
};

class BTCSharpenMoreFixedFilter : public BTCGenericFixedFilter
{
public:
    BTCSharpenMoreFixedFilter() : BTCGenericFixedFilter( 3, 1, 0) {}
    virtual ~BTCSharpenMoreFixedFilter() {}

	virtual double *GetKernel() { return BTSharpenMoreKernel; }
};

class BTCPrewittVerticalFixedFilter : public BTCGenericFixedFilter
{
public:
    BTCPrewittVerticalFixedFilter() : BTCGenericFixedFilter( 3, 1, 0) {}
    virtual ~BTCPrewittVerticalFixedFilter() {}

	virtual double *GetKernel() { return BTPrewittVerticalKernel; }
};

class BTCPrewittHorizontalFixedFilter : public BTCGenericFixedFilter
{
public:
    BTCPrewittHorizontalFixedFilter() : BTCGenericFixedFilter( 3, 1, 0) {}
    virtual ~BTCPrewittHorizontalFixedFilter() {}

	virtual double *GetKernel() { return BTPrewittHorizontalKernel; }
};

class BTCSobelVerticalFixedFilter : public BTCGenericFixedFilter
{
public:
    BTCSobelVerticalFixedFilter() : BTCGenericFixedFilter( 3, 1, 0) {}
    virtual ~BTCSobelVerticalFixedFilter() {}

	virtual double *GetKernel() { return BTSobelVerticalKernel; }
};

class BTCSobelHorizontalFixedFilter : public BTCGenericFixedFilter
{
public:
    BTCSobelHorizontalFixedFilter() : BTCGenericFixedFilter( 3, 1, 0) {}
    virtual ~BTCSobelHorizontalFixedFilter() {}

	virtual double *GetKernel() { return BTSobelHorizontalKernel; }
};

class BTCLaplacianFixedFilter : public BTCGenericFixedFilter
{
public:
    BTCLaplacianFixedFilter() : BTCGenericFixedFilter( 3, 1, 0) {}
    virtual ~BTCLaplacianFixedFilter() {}

	virtual double *GetKernel() { return BTLaplacianKernel; }
};

class BTCLaplacian5x5FixedFilter : public BTCGenericFixedFilter
{
public:
    BTCLaplacian5x5FixedFilter() : BTCGenericFixedFilter( 5, 1, 0) {}
    virtual ~BTCLaplacian5x5FixedFilter() {}

	virtual double *GetKernel() { return BTLaplacian5x5Kernel; }
};

class BTCGaussianFixedFilter : public BTCGenericFixedFilter
{
public:
    BTCGaussianFixedFilter() : BTCGenericFixedFilter( 3, 1, 0) {}
    virtual ~BTCGaussianFixedFilter() {}

	virtual double *GetKernel() { return BTGaussianKernel; }
};

class BTCGaussian5x5FixedFilter : public BTCGenericFixedFilter
{
public:
    BTCGaussian5x5FixedFilter() : BTCGenericFixedFilter( 5, 1, 0) {}
    virtual ~BTCGaussian5x5FixedFilter() {}

	virtual double *GetKernel() { return BTGaussian5x5Kernel; }
};

class BTCHighPassFixedFilter : public BTCGenericFixedFilter
{
public:
    BTCHighPassFixedFilter() : BTCGenericFixedFilter( 3, 1, 0) {}
    virtual ~BTCHighPassFixedFilter() {}

	virtual double *GetKernel() { return BTHighPassKernel; }
};

class BTCHighPass5x5FixedFilter : public BTCGenericFixedFilter
{
public:
    BTCHighPass5x5FixedFilter() : BTCGenericFixedFilter( 5, 1, 0) {}
    virtual ~BTCHighPass5x5FixedFilter() {}

	virtual double *GetKernel() { return BTHighPass5x5Kernel; }
};

class BTCLowPass1FixedFilter : public BTCGenericFixedFilter
{
public:
    BTCLowPass1FixedFilter() : BTCGenericFixedFilter( 3, 1, 0) {}
    virtual ~BTCLowPass1FixedFilter() {}

	virtual double *GetKernel() { return BTLowPass1Kernel; }
};

class BTCLowPass2FixedFilter : public BTCGenericFixedFilter
{
public:
    BTCLowPass2FixedFilter() : BTCGenericFixedFilter( 3, 1, 0) {}
    virtual ~BTCLowPass2FixedFilter() {}

	virtual double *GetKernel() { return BTLowPass2Kernel; }
};

class BTCLowPass3FixedFilter : public BTCGenericFixedFilter
{
public:
    BTCLowPass3FixedFilter() : BTCGenericFixedFilter( 3, 1, 0) {}
    virtual ~BTCLowPass3FixedFilter() {}

	virtual double *GetKernel() { return BTLowPass3Kernel; }
};

class BTCContour1FixedFilter : public BTCGenericFixedFilter
{
public:
    BTCContour1FixedFilter() : BTCGenericFixedFilter( 3, 1, 0) {}
    virtual ~BTCContour1FixedFilter() {}

	virtual double *GetKernel() { return BTContour1Kernel; }
};

class BTCContour2FixedFilter : public BTCGenericFixedFilter
{
public:
    BTCContour2FixedFilter() : BTCGenericFixedFilter( 3, 1, 0) {}
    virtual ~BTCContour2FixedFilter() {}

	virtual double *GetKernel() { return BTContour2Kernel; }
};

class BTCContour3FixedFilter : public BTCGenericFixedFilter
{
public:
    BTCContour3FixedFilter() : BTCGenericFixedFilter( 3, 1, 0) {}
    virtual ~BTCContour3FixedFilter() {}

	virtual double *GetKernel() { return BTContour3Kernel; }
};

class BTCDeepEmbossFixedFilter : public BTCGenericFixedFilter
{
public:
    BTCDeepEmbossFixedFilter() : BTCGenericFixedFilter( 7, 3, 52) {}
    virtual ~BTCDeepEmbossFixedFilter() {}

	virtual double *GetKernel() { return BTDeepEmbossKernel; }
};

class BTCFoilEngraveFixedFilter : public BTCGenericFixedFilter
{
public:
    BTCFoilEngraveFixedFilter() : BTCGenericFixedFilter( 7, 8, 0) {}
    virtual ~BTCFoilEngraveFixedFilter() {}

	virtual double *GetKernel() { return BTFoilEngraveKernel; }
};

/////////////////////////////////////////////////////////////////////////////
//
// Example using the edge detect filter:
//
// BTCImageObject Object;
// BTCImageData   Image;
//
// Object.Load( "test.bmp");
// Image = Object.GetObjectData();
//
// BTCFixedFilter<BTCEdgeDetectFixedFilter> Filter;
// Filter.Process( &Image);
//
/////////////////////////////////////////////////////////////////////////////

template <class _Filter>
class BTCFixedFilter
{
public:
	BTCFixedFilter() {}
	virtual ~BTCFixedFilter() {}

	bool Process(BTCImageData *pImage)
	{
		if( NULL == pImage)
			return false;

		_Filter Filter;

		return pImage->Convolve( Filter.GetKernel(), Filter.GetKernelSize(), Filter.GetDivisor(), Filter.GetBias());
	}
};

#endif // BTFIXEDFILTERS_INCLUDED

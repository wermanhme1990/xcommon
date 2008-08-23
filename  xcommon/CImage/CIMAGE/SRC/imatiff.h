// imatiff.h : interface of the CImageTIFF class

/*
 * Purpose:	TIFF Image Class Loader and Writer
 */
/* === C R E D I T S  &  D I S C L A I M E R S ==============
 * CImageTIFF (c) 07/Aug/2001 <ing.davide.pizzolato@libero.it>
 * Permission is given by the author to freely redistribute and include
 * this code in any program as long as this credit is given where due.
 *
 * Parts of the code come from FreeImage 2
 * Design and implementation by 
 * - Floris van den Berg (flvdberg@wxs.nl)
 * - Hervé Drolon (drolon@iut.univ-lehavre.fr)
 * - Markus Loibl (markus.loibl@epost.de)
 * - Luca Piergentili (l.pierge@terra.es)
 *
 * LibTIFF is:
 * Copyright (c) 1988-1997 Sam Leffler
 * Copyright (c) 1991-1997 Silicon Graphics, Inc.
 *
 * COVERED CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY
 * OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES
 * THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE
 * OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED
 * CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT
 * THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY
 * SERVICING, REPAIR OR CORRECTION. THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL
 * PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER
 * THIS DISCLAIMER.
 *
 * Use at your own risk!
 * ==========================================================
 */

#ifndef __TIFF_H__
#define __TIFF_H__

#include "..\include\cimage.h"

class CImageTIFF: public CImage
{
public:
	CImageTIFF(): CImage() {}
	
// Implementation
public:
   virtual BOOL Read(FILE*);
	virtual BOOL Write(FILE*);
};

#endif // __TIFF_H__

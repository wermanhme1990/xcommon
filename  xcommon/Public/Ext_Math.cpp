/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称:   Ext_Math.h          
文件内容:  简单的一些常用数学操作
版本历史:  1.0
作者:    xuejuntao xuejuntao@hanwang.com.cn 2008/02/26
*********************************************************************/

#include "stdafx.h"
#include "Ext_Math.h"
#include <cmath>

LONG Mth_GetMinDist(const HWPoint &tPtA, const HWPoint &tPtB, const HWPoint &tPtC, 
           HWPoint &tNearestPt)
{  
  HWPoint tPointAToB = HWPoint(tPtB.x - tPtA.x, tPtB.y - tPtA.y);  
  HWPoint tPointAToC = HWPoint(tPtC.x - tPtA.x, tPtC.y - tPtA.y);      
  double dM = tPointAToB.x * tPointAToC.x + tPointAToB.y * tPointAToC.y;
  double dN = tPointAToB.x * tPointAToB.x + tPointAToB.y * tPointAToB.y;
  double dDelta = dM / dN;
  if (dDelta < 0.0)
  {
    tNearestPt = tPtA;    
  }
  else if (dDelta > 1.0)
  {
    tNearestPt = tPtB;    
  }
  else
  {
    tNearestPt = tPointAToB;
    tNearestPt.x *= dDelta;  
    tNearestPt.y *= dDelta;
    tNearestPt.x += tPtA.x;  
    tNearestPt.y += tPtA.y; 
  }    
  return std::sqrt(1.0 * (
    (tNearestPt.x - tPtC.x) * (tNearestPt.x - tPtC.x) + 
        (tNearestPt.y - tPtC.y) * (tNearestPt.y - tPtC.y))
    );
}
long DivRound( long x, long div )
{
  return (x + (div >> 1)) / div;
}
long MulDivRound(long x, long mul, long div)
{
  return (x * mul + (div >> 1)) / div;
}


//整数开方，没有系统自带的浮点开方快
long XSqrt(const long& n) 
{ 
  long i;
  long nSqrt = 0; 
  LONG nRoot = n;
  for ( i = 0x10000000; i != 0; i >>= 2) 
  { 
    long nTemp = nSqrt + i; 
    nSqrt >>= 1; 
    if (nTemp <= nRoot) 
    { 
      nRoot -= nTemp; 
      nSqrt += i; 
    } 
  } 
  return nSqrt; 
} 

/***
*qsort.c - quicksort algorithm; qsort() library function for sorting arrays
*
*       Copyright (c) 2003-2006, Hanwang Corporation. All rights reserved.
*
*Purpose:
*       To implement the qsort() routine for sorting arrays.
*
*******************************************************************************/



#define CUTOFF 8            /* testing shows that this is good value */

// variant for quick sort


/***
*ZB_SwapIdx(a, b, nIdxWidth) - ZB_SwapIdx two elements
*
*Purpose:
*       swaps the two array elements of size nIdxWidth
*
*Entry:
*       char *a, *b = pointer to two elements to ZB_SwapIdx
*       unsigned nIdxWidth = nIdxWidth in bytes of each array element
*
*Exit:
*       returns void
*
*Exceptions:
*
*******************************************************************************/

void ZB_SwapIdx (
                 char *a,
                 char *b,
                 unsigned long nIdxWidth
                 )
{
  char tmp;

  if ( a != b )
    /* Do the ZB_SwapIdx one character at a time to avoid potential alignment
    problems. */
    while ( nIdxWidth-- )
    {
      tmp = *a;
      *a++ = *b;
      *b++ = tmp;
    }
}

/***
*qsort.c - quicksort algorithm; qsort() library function for sorting arrays
*
*       Copyright (c) 1985-1997, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       To implement the qsort() routine for sorting arrays.
*
*******************************************************************************/



/***
*ZB_ShortSort(hi, lo, width, comp) - insertion sort for sorting short arrays
*
*Purpose:
*       sorts the sub-array of elements between lo and hi (inclusive)
*       side effects:  sorts in place
*       assumes that lo < hi
*
*Entry:
*       char *lo = pointer to low element to sort
*       char *hi = pointer to high element to sort
*       unsigned width = width in bytes of each array element
*       long (*comp)() = pointer to function returning analog of strcmp for
*               strings, but supplied by user for comparing the array elements.
*               it accepts 2 pointers to elements and returns neg if 1<2, 0 if
*               1=2, pos if 1>2.
*
*Exit:
*       returns void
*
*Exceptions:
*
*******************************************************************************/

void ZB_ShortSort (
                   char *lo,
                   char *hi,
                   unsigned long width,
                   long (*comp)(const void *, const void *)
                   )
{
  char *p, *max;

  /* Note: in assertions below, i and j are alway inside original bound of
  array to sort. */

  while (hi > lo) {
    /* A[i] <= A[j] for i <= j, j > hi */
    max = lo;
    for (p = lo+width; p <= hi; p += width) {
      /* A[i] <= A[max] for lo <= i < p */
      if (comp(p, max) > 0) {
        max = p;
      }
      /* A[i] <= A[max] for lo <= i <= p */
    }

    /* A[i] <= A[max] for lo <= i <= hi */

    ZB_SwapIdx(max, hi, width);

    /* A[i] <= A[hi] for i <= hi, so A[i] <= A[j] for i <= j, j >= hi */

    hi -= width;

    /* A[i] <= A[j] for i <= j, j > hi, loop top condition established */
  }
  /* A[i] <= A[j] for i <= j, j > lo, which implies A[i] <= A[j] for i < j,
  so array is sorted */
}


/* this parameter defines the cutoff between using quick sort and
insertion sort for arrays; arrays with lengths shorter or equal to the
below value use insertion sort */

/***
*qsort(base, num, wid, comp) - quicksort function for sorting arrays
*
*Purpose:
*       quicksort the array of elements
*       side effects:  sorts in place
*
*Entry:
*       char *base = pointer to base of array
*       unsigned num  = number of elements in the array
*       unsigned width = width in bytes of each array element
*       long (*comp)() = pointer to function returning analog of strcmp for
*               strings, but supplied by user for comparing the array elements.
*               it accepts 2 pointers to elements and returns neg if 1<2, 0 if
*               1=2, pos if 1>2.
*
*Exit:
*       returns void
*
*Exceptions:
*
*******************************************************************************/



/* sort the array between lo and hi (inclusive) */
void XQsort ( void *base, long num, long width, long *pComp )
{
  char *lo, *hi;              /* ends of sub-array currently sorting */
  char *mid;                  /* points to middle of subarray */
  char *loguy, *higuy;        /* traveling pointers for partition step */
  unsigned long size;              /* size of the sub-array */
  char *lostk[30], *histk[30];
  long stkptr;                 /* stack for saving sub-array to be processed */

  long (*comp)(const void *, const void *);
  comp = (long (*)(const void *, const void *))pComp;

  /* Note: the number of stack entries required is no more than
  1 + log2(size), so 30 is sufficient for any array */

  if (num < 2 || width == 0)
    return;                 /* nothing to do */

  stkptr = 0;                 /* initialize stack */

  lo = (char*)base;
  hi = (char *)base + width * (num-1);        /* initialize limits */

  /* this entry point is for pseudo-recursion calling: setting
  lo and hi and jumping to here is like recursion, but stkptr is
  prserved, locals aren't, so we preserve stuff on the stack */
recurse:

  size = (hi - lo) / width + 1;        /* number of el's to sort */

  /* below a certain size, it is faster to use a O(n^2) sorting method */
  if (size <= CUTOFF) {
    ZB_ShortSort(lo, hi, width, comp);
  }
  else {
    /* First we pick a partititioning element.  The efficiency of the
    algorithm demands that we find one that is approximately the
    median of the values, but also that we select one fast.  Using
    the first one produces bad performace if the array is already
    sorted, so we use the middle one, which would require a very
    wierdly arranged array for worst case performance.  Testing shows
    that a median-of-three algorithm does not, in general, increase
    performance. */

    mid = lo + (size / 2) * width;      /* find middle element */
    ZB_SwapIdx(mid, lo, width);               /* ZB_SwapIdx it to beginning of array */

    /* We now wish to partition the array into three pieces, one
    consisiting of elements <= partition element, one of elements
    equal to the parition element, and one of element >= to it.  This
    is done below; comments indicate conditions established at every
    step. */

    loguy = lo;
    higuy = hi + width;

    /* Note that higuy decreases and loguy increases on every iteration,
    so loop must terminate. */
    for (;;) {
      /* lo <= loguy < hi, lo < higuy <= hi + 1,
      A[i] <= A[lo] for lo <= i <= loguy,
      A[i] >= A[lo] for higuy <= i <= hi */

      do  {
        loguy += width;
      } while (loguy <= hi && comp(loguy, lo) <= 0);

      /* lo < loguy <= hi+1, A[i] <= A[lo] for lo <= i < loguy,
      either loguy > hi or A[loguy] > A[lo] */

      do  {
        higuy -= width;
      } while (higuy > lo && comp(higuy, lo) >= 0);

      /* lo-1 <= higuy <= hi, A[i] >= A[lo] for higuy < i <= hi,
      either higuy <= lo or A[higuy] < A[lo] */

      if (higuy < loguy)
        break;

      /* if loguy > hi or higuy <= lo, then we would have exited, so
      A[loguy] > A[lo], A[higuy] < A[lo],
      loguy < hi, highy > lo */

      ZB_SwapIdx(loguy, higuy, width);

      /* A[loguy] < A[lo], A[higuy] > A[lo]; so condition at top
      of loop is re-established */
    }

    /*     A[i] >= A[lo] for higuy < i <= hi,
    A[i] <= A[lo] for lo <= i < loguy,
    higuy < loguy, lo <= higuy <= hi
    implying:
    A[i] >= A[lo] for loguy <= i <= hi,
    A[i] <= A[lo] for lo <= i <= higuy,
    A[i] = A[lo] for higuy < i < loguy */

    ZB_SwapIdx(lo, higuy, width);     /* put partition element in place */

    /* OK, now we have the following:
    A[i] >= A[higuy] for loguy <= i <= hi,
    A[i] <= A[higuy] for lo <= i < higuy
    A[i] = A[lo] for higuy <= i < loguy    */

    /* We've finished the partition, now we want to sort the subarrays
    [lo, higuy-1] and [loguy, hi].
    We do the smaller one first to minimize stack usage.
    We only sort arrays of length 2 or more.*/

    if ( higuy - 1 - lo >= hi - loguy ) {
      if (lo + width < higuy) {
        lostk[stkptr] = lo;
        histk[stkptr] = higuy - width;
        ++stkptr;
      }                           /* save big recursion for later */

      if (loguy < hi) {
        lo = loguy;
        goto recurse;           /* do small recursion */
      }
    }
    else {
      if (loguy < hi) {
        lostk[stkptr] = loguy;
        histk[stkptr] = hi;
        ++stkptr;               /* save big recursion for later */
      }

      if (lo + width < higuy) {
        hi = higuy - width;
        goto recurse;           /* do small recursion */
      }
    }
  }

  /* We have sorted the array, except for any pending sorts on the stack.
  Check if there are any, and do them. */

  --stkptr;
  if (stkptr >= 0) {
    lo = lostk[stkptr];
    hi = histk[stkptr];
    goto recurse;           /* pop subarray from stack */
  }
  else
    return;                 /* all subarrays done */
}

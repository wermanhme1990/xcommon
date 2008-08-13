
/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称:   Ext_Memory.h          
文件内容:  内存操作的封装 可以提高程序可调适性
版本历史:  1.0
作者:    xuejuntao xuejuntao@hanwang.com.cn 2008/02/26
*********************************************************************/
#include "stdafx.h"
#include "Ext_Memory.h"
#include "Ext_Type.h"
#include <assert.h>
#define Mem_MinOptLen (8)

static LONG BO_LeftShift( const LONG& nValue, const LONG& nBit )
{
  return (nValue << nBit );
};

BOOL WINAPI XMemFill(void *pvMem, CHAR bChrFill, LONG nSize)
{  
  assert(NULL != pvMem);
  assert(0 < nSize);
  LONG nRet = TRUE;

#ifdef  _DEBUG//////////////////////////////////////////////////////////////////////////
  //  assert(!IsBadWritePtr(pvMem, nSize));  
#endif//////////////////////////////////////////////////////////////////////////

  XMemSet(pvMem, bChrFill, nSize);

  return nRet;
}


BOOL WINAPI XMemZero(void *pvMem, LONG nSize)
{
  assert(NULL != pvMem);
  assert(0 < nSize);
  return XMemFill(pvMem,  0, nSize);
}

BOOL WINAPI XMemEqual(const void *pvMemDest, const void *pvMemSrc, LONG nSize)
{
  assert(NULL != pvMemDest);
  assert(NULL != pvMemSrc);
  assert(0 < nSize);
  return !XMemCompare((void *)pvMemDest, (void *)pvMemSrc, nSize);
}



void WINAPI XMemFree(void** ppvMem)
{
  assert(NULL != ppvMem);

#ifdef _DEBUG//////////////////////////////////////////////////////////////////////////
  {
    if (NULL != *ppvMem)
    {
      LONG nSize = (LONG)_msize(*ppvMem);
      if (0 != nSize)
      {
        XMemSet(*ppvMem, 0, nSize);
      }
    }        
  }
#endif//////////////////////////////////////////////////////////////////////////
  free(*ppvMem);  
  *ppvMem = NULL;
}

BOOL WINAPI XMemAlloc(void** ppvMem, LONG nSize)
{
  BYTE** ppbMem = (BYTE**)ppvMem;

  assert(NULL != ppvMem && 0 < nSize);

  *ppbMem = (BYTE*)malloc(nSize);
  //*ppbMem = new BYTE[nSize];
#ifdef _DEBUG//////////////////////////////////////////////////////////////////////////
  {
    if(NULL !=  *ppbMem)
    {
      XMemZero(*ppbMem, nSize);
    }
  }
#endif//////////////////////////////////////////////////////////////////////////

  return(NULL != *ppbMem);
}

BOOL WINAPI XMemResize(void** ppvMem, LONG nSizeNew)
{
  BYTE **ppbMem = (BYTE **)ppvMem;
  BYTE *pbMem = *ppbMem;
  BYTE *pbMemResized = NULL;
  assert(NULL != ppbMem && 0 != nSizeNew);
#ifdef _DEBUG //////////////////////////////////////////////////////////////////////////
  LONG nSizeOrg = 0;
  {    
    if (NULL != pbMem)
    {
      nSizeOrg = (LONG)_msize(pbMem);
    }
    else
    {
      nSizeOrg = 0;
    } 
    /* 如果缩小，冲掉块尾释放的内容 */
    if(nSizeNew < nSizeOrg)
    {
      XMemZero(pbMem + nSizeNew, nSizeOrg - nSizeNew);
    }
    else if(nSizeNew > nSizeOrg && 0 != nSizeOrg)
    {
      BYTE* pbTmp = NULL;
      if(XMemAlloc((void **)&pbTmp, nSizeOrg))
      {
        XMemCopy(pbTmp, pbMem, nSizeOrg);
        XMemFree((void **)&pbMem);
        pbMem = pbTmp;
      }
    }
  }
#endif//////////////////////////////////////////////////////////////////////////
  pbMemResized = (BYTE*)realloc(pbMem, nSizeNew);  
  if (0 == nSizeNew)
  {//此时相当于调用free
    pbMemResized = NULL;
    *ppbMem = NULL;
  }
  if(NULL != pbMemResized)
  {
#ifdef _DEBUG//////////////////////////////////////////////////////////////////////////
    {      
      if(nSizeNew > nSizeOrg)
      {/* 如果扩大，对尾部增加的内容进行初始化 */
        XMemZero(pbMemResized + nSizeOrg, nSizeNew - nSizeOrg);
      }
    }
#endif//////////////////////////////////////////////////////////////////////////
    *ppbMem = pbMemResized;
  }

  return(NULL != pbMemResized);
}

LONG  WINAPI XMemSet( void *p, const CHAR& b, const LONG& n )
{
  CHAR *pchEnd;
  //LONG  nTail;
  LONG nHead , nMiddle;

  CHAR *pch = (CHAR*)p;
  DWORD bInit = (BYTE)b;

  if( pch == NULL || n <= 0 )
    return 0;
  pchEnd = ((CHAR*)p) + n;
  nHead = ((LONG)p) & 3;
  if( nHead )
  {
    nHead = 4 - nHead;
    nMiddle = n - nHead;//-= 4;
  }
  else
  {
    nMiddle = n;
  }

  //Deal with head part
  if( nHead )
  {
    CHAR *pchHeadEnd = pch + _min( n, nHead );
    for( ; pch < pchHeadEnd; pch++ )
    {
      *pch = b;
    }
  }

  //Deal with middle part
  if( nMiddle > 0 )
  {
    LONG nSetValue = BO_LeftShift( bInit, 24) + BO_LeftShift(bInit , 16) + BO_LeftShift(bInit, 8) + bInit;
    LONG *pnEnd = (LONG*)pch + ( nMiddle >> 2);
    for( ; (LONG*)pch < pnEnd; pch += 4 )
    {
      *((LONG*)pch) = nSetValue;
    }

  }

  //Deal with tail part
  //if( nTail )
  {
    //pchEnd = (CHAR*)p + n;
    for( ; pch < pchEnd; pch++ )
    {
      *pch = b;
    }
  }

  return n;
}



LONG WINAPI XMemCopy( void *pDest, const void *const pSrc, const LONG& n )
{
  register CHAR  *pchDst, *pchSrc, *pchDstEnd;

  LONG nAddrDiff, nMiddle, nHead;// = 3;

  if( n <= 0 || !pDest || !pSrc )
  {
    return 0;
  }
  if( pDest > pSrc )//&& pchSource + n > pchDest )
  {
    return XMemMove( pDest, pSrc, n);
  }
  pchDst = (CHAR*)pDest;
  pchSrc = (CHAR*)pSrc;
  pchDstEnd = ((CHAR*)pDest ) + n;

  if( n >= Mem_MinOptLen )
  {
    nAddrDiff = ( ((CHAR*)pchSrc) - ((CHAR*)pchDst) ) & 3;
  }
  else
  {
    nAddrDiff = 3;
  }

  switch( nAddrDiff ) 
  {
  case 0:// 4-byte aligned
    {			
      nHead =  (((LONG)pchDst) & 3);

      //compute the odd address part
      if( 0 != nHead )
      {
        nHead = 4 - nHead;
        nMiddle = (n - nHead);// & 0xfffffffc;
      }
      else
      {
        nMiddle = n;
      }

      //copy the head part
      if( nHead )
      {
        CHAR *pchDstHeadEnd = ((CHAR*)pchDst) + _min( nHead, n );
        for( ; pchDst < pchDstHeadEnd; pchDst++, pchSrc++  )
        {
          *pchDst = *pchSrc;
        }
      }

      //copy the middle part
      if( nMiddle > 0 )
      {
        LONG *pnDstEnd = ((LONG *)pchDst) + (nMiddle >> 2);
        for( ; ((LONG *)pchDst) < pnDstEnd; pchDst += 4, pchSrc += 4 )
        {
          *((LONG *)pchDst) = *((LONG *)pchSrc);
        }
      }
      // copy the tail part
      for( ; pchDst < pchDstEnd; pchDst++, pchSrc++ )
      {
        *pchDst = *pchSrc;
      }
    }
    break;
  case 2:
    {//2-byte aligned
      nHead = (((LONG)pchDst) & 1);

      //compute the odd address part
      if( 0 != nHead )
      {
        nHead = 2 - nHead;
        nMiddle = n - nHead;
      }
      else
      {
        nMiddle = n;
      }

      //copy the head part
      if( nHead )
      {
        CHAR *pchDstHeadEnd = ((CHAR*)pchDst) + _min(nHead, n);
        for( ; pchDst < pchDstHeadEnd; pchDst++, pchSrc++  )
        {
          *pchDst = *pchSrc;
        }
      }

      //copy the middle part
      if( nMiddle > 0)
      {
        short  *psDstEnd = ((short*)pchDst) + ( nMiddle >> 1);
        for( ; ((short*)pchDst) < psDstEnd; pchDst += 2, pchSrc += 2 )
        {
          *((short*)pchDst) = *((short*)pchSrc);
        }
      }
      // copy the tail part
      for( ; pchDst < pchDstEnd; pchDst++, pchSrc++ )
      {
        *pchDst = *pchSrc;
      }
    }
    break;
  default:
    // copy the tail part
    for( ; pchDst < pchDstEnd; pchDst++, pchSrc++ )
    {
      *pchDst = *pchSrc;
    }	
  }


  return n;
}


LONG WINAPI XMemMove( void *pDest, const void *const pSrc, const LONG& n )
{
  register CHAR *pchDest, *pchSrc, *pchDestBgn ;
  LONG nAddrDiff, nMiddle, nTail;// = 3;


  if( n <= 0 || !pDest ||  !pSrc )
  {
    return 0;
  }

  if( (DWORD)pDest < (DWORD)pSrc )//
    //|| (CHAR*)pSrc + n < (CHAR*)pDest )// not overlap
  {
    return XMemCopy( pDest, pSrc, n);
  }

  pchDestBgn = (CHAR*)pDest;
  pchDest = (CHAR*)pDest + n;
  pchSrc = (CHAR*)pSrc + n;

  if( n >= Mem_MinOptLen )
    nAddrDiff = (pchDest - pchSrc) & 3;
  else
    nAddrDiff = 3;
  switch( nAddrDiff ) 
  {
  case 0:
    {
      nTail = (((LONG)pchDest) & 3);
      if( nTail )
      {				
        nMiddle = n - nTail;// & 0xfffffffc;
      }
      else
        nMiddle = n;

      //Deal with the tail part
      if( nTail )
      {
        CHAR *pchDstTailBgn = pchDest - _min( nTail, n);
        for( ; pchDest > pchDstTailBgn;  )
        {
          pchDest--, pchSrc--;
          *pchDest = *pchSrc;
        }
      }

      //copy the middle part
      if( nMiddle > 0 )
      {
        LONG  *pnDstBgn = ((LONG*)pchDest) - ( nMiddle >> 2);
        for( ; ((LONG*)pchDest) > pnDstBgn; )
        {
          pchDest -= 4;
          pchSrc -= 4;
          *((LONG*)pchDest) = *((LONG*)pchSrc);
        }
      }
      //Copy the head part
      for( ; pchDest > pchDestBgn;  )
      {
        pchDest--, pchSrc--;
        *pchDest = *pchSrc;
      }
    }
    break;
  case 2:
    {
      nTail = (((LONG)pchDest) & 1);
      if( nTail )
      {				
        nMiddle = n - nTail;
      }
      else
        nMiddle = n;

      //Deal with the tail part
      if( nTail )
      {
        CHAR *pchDstTailBgn = pchDest - _min( nTail, n );
        for( ; pchDest > pchDstTailBgn;  )
        {
          pchDest--, pchSrc--;
          *pchDest = *pchSrc;
        }
      }

      //copy the middle part
      if( nMiddle > 0 )
      {
        short *psDstBgn = ((short*)pchDest) - ( nMiddle >> 1);
        for( ; (short*)pchDest > psDstBgn; )
        {
          pchDest -= 2;
          pchSrc -= 2;
          *((short*)pchDest) = *((short*)pchSrc);
        }
      }
      //Copy the head part
      for( ; pchDest > pchDestBgn;  )
      {
        pchDest--, pchSrc--;
        *pchDest = *pchSrc;
      }
    }
    break;
  default:
    //Copy the head part
    for( ; pchDest > pchDestBgn;  )
    {
      pchDest--, pchSrc--;
      *pchDest = *pchSrc;
    }
  }

  return n;
}

//XUE写的
LONG WINAPI XMemCompare( void *const pDest, void *const pSrc, const LONG& n )
{
  register CHAR  *pchDst = NULL, *pchSrc = NULL, *pchDstEnd = NULL;	
  register LONG iRet = 0;

  if( n <= 0 || pDest == NULL || pSrc == NULL )
  {
    iRet = 0;
    goto _Exit;
  }
  LONG nLen = n;
  pchDst = (CHAR*)pDest;
  pchSrc = (CHAR*)pSrc;
  while(nLen --  > 0 && 0 == (iRet = *(pchDst ++ ) - *(pchSrc ++)))
  {
    NULL;
  }	

_Exit:
  return iRet;
}
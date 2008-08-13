
#include "stdafx.h"

#include "Ext_General.h"
#include "Ext_Memory.h"
#include "Ext_Math.h"
#include <windows.h>
#include <stdlib.h>
#include <assert.h>
#include <cmath>
const HWPoint g_atEndMark[2] = 
{
  HWPoint(HW_StrokeEndMark, 0),
  HWPoint(HW_StrokeEndMark, HW_StrokeEndMark)
};


BOOL WCharToChar(const wchar_t* pwszStr, char* pszStr, long nLen)
{
  int n = -1;  
  assert(nLen > 0 && NULL != pszStr && NULL != pwszStr);
  if (nLen <= 0 || NULL == pwszStr || NULL == pszStr)
  {
    return FALSE;
  }    
  Mem_ZeroMemory(pszStr, sizeof(*pszStr) * nLen);  
  if (-1 == (n = WideCharToMultiByte(CP_ACP,   0,   pwszStr,   
    (long)wcslen(pwszStr),  pszStr, nLen,   0 , 0)))
  {
    return FALSE;
  }
  return TRUE;
}
BOOL CharToWChar(const char* pszStr, wchar_t* pwszStr, long nLen)
{
  int n= -1;  
  assert(nLen > 0 && NULL != pszStr && NULL != pwszStr);
  if (nLen <= 0 || NULL == pszStr || NULL == pwszStr)
  {
    return FALSE;
  }    
  Mem_ZeroMemory(pwszStr, sizeof(*pwszStr) * nLen);  
  if (-1 == (n = MultiByteToWideChar(CP_ACP,   0,   pszStr,   
    (long)strlen(pszStr),  pwszStr, nLen)))
  {
    return FALSE;
  }  
  return TRUE;
}

BOOL IsHexDigit( const TCHAR *pszStr )
{
  assert(pszStr);  
  size_t nIndex = _tcsspn(pszStr, _T("0123456789ABCDEFabcdef"));
  if (_tcslen(pszStr) == nIndex)
  {
    return FALSE;
  }
  return TRUE;
}
HWX_OUT CRect GetRect(HWX_IN const HWPoint *ptTrace)
{
  assert(NULL != ptTrace);  
  CRect rtRect;
  rtRect.SetRect(0, 0, 0, 0);
  while (!(g_atEndMark[1] == *ptTrace))
  {
    if (!(g_atEndMark[0] == *ptTrace))
    {
      rtRect.SetRect(ptTrace->x, ptTrace->y, ptTrace->x, ptTrace->y);
      break;
    }        
    ptTrace ++;
  }

  while (!(g_atEndMark[1] == *ptTrace))
  {
    if (!(g_atEndMark[0] == *ptTrace))
    {
      if (rtRect.left > ptTrace->x)
      {
        rtRect.left = ptTrace->x;
      }
      else if (rtRect.right < ptTrace->x)
      {
        rtRect.right = ptTrace->x;
      }
      if (rtRect.top > ptTrace->y)
      {
        rtRect.top = ptTrace->y;
      }
      else if (rtRect.bottom < ptTrace->y)
      {
        rtRect.bottom = ptTrace->y;
      }
    }    
    ptTrace ++;
  }  
  return rtRect;
}
CRect GetTraceBoundbox(const std::vector<HWPoint> &vStrokeTrace)
{//只有一条笔迹 内部都是有效点
  CRect rtRectBoundBox;    
  std::vector<HWPoint>::const_iterator itvStrokeTrace = vStrokeTrace.begin();
  if (itvStrokeTrace != vStrokeTrace.end())
  {
    rtRectBoundBox.left = rtRectBoundBox.right = itvStrokeTrace->x;
    rtRectBoundBox.top = rtRectBoundBox.bottom = itvStrokeTrace->y;
    itvStrokeTrace ++;
    while (!(*itvStrokeTrace == g_atEndMark[1]))
    {
      if (!(*itvStrokeTrace == g_atEndMark[0]))
      {
        if (rtRectBoundBox.left > itvStrokeTrace->x)
        {
          rtRectBoundBox.left = itvStrokeTrace->x;
        }
        else if(rtRectBoundBox.right < itvStrokeTrace->x)
        {
          rtRectBoundBox.right = itvStrokeTrace->x;
        }
        if (rtRectBoundBox.top > itvStrokeTrace->y)
        {
          rtRectBoundBox.top = itvStrokeTrace->y;
        }
        else if(rtRectBoundBox.bottom < itvStrokeTrace->y)
        {
          rtRectBoundBox.bottom = itvStrokeTrace->y;
        }
      }  
      itvStrokeTrace ++;
    }
  }    
  return rtRectBoundBox;
}

CRect CalcBox(const std::vector<HWPoint> &vStrokeTrace)
{//只有一条笔迹 内部都是有效点
  CRect rtRectBoundBox;    
  std::vector<HWPoint>::const_iterator itvStrokeTrace = vStrokeTrace.begin();
  if (itvStrokeTrace != vStrokeTrace.end())
  {
    rtRectBoundBox.left = rtRectBoundBox.right = itvStrokeTrace->x;
    rtRectBoundBox.top = rtRectBoundBox.bottom = itvStrokeTrace->y;
    itvStrokeTrace ++;
    while (itvStrokeTrace != vStrokeTrace.end() && !(*itvStrokeTrace == g_atEndMark[1]) )
    {
      if (!(*itvStrokeTrace == g_atEndMark[0]))
      {
        if (rtRectBoundBox.left > itvStrokeTrace->x)
        {
          rtRectBoundBox.left = itvStrokeTrace->x;
        }
        else if(rtRectBoundBox.right < itvStrokeTrace->x)
        {
          rtRectBoundBox.right = itvStrokeTrace->x;
        }
        if (rtRectBoundBox.top > itvStrokeTrace->y)
        {
          rtRectBoundBox.top = itvStrokeTrace->y;
        }
        else if(rtRectBoundBox.bottom < itvStrokeTrace->y)
        {
          rtRectBoundBox.bottom = itvStrokeTrace->y;
        }
      }  
      itvStrokeTrace ++;
    }
  }    
  return rtRectBoundBox;
}
CRect GetRect(const std::vector<HWPoint> &vStrokeTrace)
{//只有一条笔迹 内部都是有效点
  CRect rtRectBoundBox;    
  std::vector<HWPoint>::const_iterator itvStrokeTrace = vStrokeTrace.begin();
  if (itvStrokeTrace != vStrokeTrace.end())
  {
    rtRectBoundBox.left = rtRectBoundBox.right = itvStrokeTrace->x;
    rtRectBoundBox.top = rtRectBoundBox.bottom = itvStrokeTrace->y;
    itvStrokeTrace ++;
    for (; itvStrokeTrace != vStrokeTrace.end(); itvStrokeTrace ++)
    {
      if (rtRectBoundBox.left > itvStrokeTrace->x)
      {
        rtRectBoundBox.left = itvStrokeTrace->x;
      }
      else if(rtRectBoundBox.right < itvStrokeTrace->x)
      {
        rtRectBoundBox.right = itvStrokeTrace->x;
      }
      if (rtRectBoundBox.top > itvStrokeTrace->y)
      {
        rtRectBoundBox.top = itvStrokeTrace->y;
      }
      else if(rtRectBoundBox.bottom < itvStrokeTrace->y)
      {
        rtRectBoundBox.bottom = itvStrokeTrace->y;
      }
    }
  }    
  return rtRectBoundBox;
}
BOOL FILE_GenFmt(const BYTE *pbData, TFormat_FMT *ptFormatFmt)
{
  assert(NULL != ptFormatFmt);
  assert(NULL != pbData);
  
  BOOL blRet = TRUE;
  const BYTE *pbBuffer = pbData;
  Mem_ZeroMemory(ptFormatFmt, sizeof(*ptFormatFmt));
  
  ptFormatFmt->wPackageSize  = ((WORD *)pbBuffer)[0];
  ptFormatFmt->wStrLen    = ((WORD *)pbBuffer)[1];
  ptFormatFmt->pbRefUnicode  = (BYTE *)&((WORD *)pbBuffer)[2];  
  ptFormatFmt->pbTrace    = pbBuffer + (ptFormatFmt->wStrLen + 2 + 2) * sizeof(WORD);
  
  return blRet;
}


BOOL Trc_GenStrokeInfo(const HWPoint *ptTrace, TStroke *ptStroke, long *pnStrokeCount)  
{
  assert(NULL != ptTrace);
  assert(NULL != ptStroke);
  assert(NULL != pnStrokeCount);
  BOOL blRet = TRUE;
  long nStrokeCount = 0;  
  const HWPoint *ptUseData = ptTrace;
  //分配内存
  if (NULL == ptTrace|| NULL == ptStroke)
  {
    blRet = FALSE;
    goto _Exit;
  }
  //填充比划信息
  ptStroke[nStrokeCount].m_nBeginPos = 0;
  ptStroke[nStrokeCount].m_nEndPos = 0;
  ptStroke[nStrokeCount].m_nStrokeLen = 0;
  ptStroke[nStrokeCount].m_tBox.left = (ptStroke)[0].m_tBox.right = ptUseData->x;
  ptStroke[nStrokeCount].m_tBox.top = (ptStroke)[0].m_tBox.bottom = ptUseData->y;
  /*循环处理每一个笔画*/  
  while (!(g_atEndMark[1] == *ptUseData))
  {
    if (g_atEndMark[0] == *ptUseData)
    {
      ptStroke[nStrokeCount].m_nEndPos = (ptUseData - ptTrace);        /*记录笔画结束位置*/  
      //去除长度为零的笔画
      if (ptStroke[nStrokeCount].m_nStrokeLen < 1)
      {
        //  nStrokeCount --;      
      }      
      /*如果当前笔画不是最后一画，就要为下一笔画进行数据初始化*/
      if (!(g_atEndMark[1] == *(ptUseData + 1)))
      {
        nStrokeCount ++;        
        ptStroke[nStrokeCount].m_nBeginPos = (ptUseData - ptTrace) + 1;        /*起始位置*/
        ptStroke[nStrokeCount].m_nEndPos = (ptUseData - ptTrace) + 1;          /*结束位置*/
        ptStroke[nStrokeCount].m_nStrokeLen = 0;                 /*笔画长度*/
        /*包围盒初值*/
        ptStroke[nStrokeCount].m_tBox.left = (ptStroke)[nStrokeCount].m_tBox.right = (ptUseData + 1)->x;
        ptStroke[nStrokeCount].m_tBox.top = (ptStroke)[nStrokeCount].m_tBox.bottom = (ptUseData + 1)->y;        
      }
    }
    else
    {
      /*更新比划包围盒*/
      if (ptUseData->x < (ptStroke)[nStrokeCount].m_tBox.left)
        ptStroke[nStrokeCount].m_tBox.left = ptUseData->x;
      else if (ptUseData->x > (ptStroke)[nStrokeCount].m_tBox.right)
        ptStroke[nStrokeCount].m_tBox.right = ptUseData->x;
      if (ptUseData->y < (ptStroke)[nStrokeCount].m_tBox.top)
        ptStroke[nStrokeCount].m_tBox.top = ptUseData->y;
      else if (ptUseData->y > (ptStroke)[nStrokeCount].m_tBox.bottom)
        ptStroke[nStrokeCount].m_tBox.bottom = ptUseData->y;
      //计算笔画长度
      if (!(g_atEndMark[0] == *ptUseData))
      {
        ptStroke[nStrokeCount].m_nStrokeLen += 
          std::sqrt(double(ptUseData->x * ptUseData->y + (ptUseData + 1)->x * (ptUseData + 1)->y));
      }
    }

    ptUseData ++;
  }
  *pnStrokeCount = nStrokeCount + 1;
  if (*pnStrokeCount <= 0)
  {
    blRet = FALSE;    
  }
_Exit:  
  //判断是否有有效笔画数
  return blRet;      
}

HWPoint TransformPos(const HWPoint &tPtOrg, const CRect& rtScr, 
                     const CRect& rtDist, BOOL blStretch)
{//这里都用浮点进行运算  因为在rtSrc中宽度或者长度过小的时候 整数运算往往使得轨迹画到rect外面来
  //中心显示 为了防止太靠边 进行适当的防缩 防缩尽量不形变  
  long x = 0, y = 0;  
  float fZoom_X = 0.0f, fZoom_Y = 0.0f;  
  CPoint tPoint = rtDist.CenterPoint();
  HWPoint tCenterPointDist(tPoint.x, tPoint.y);
  tPoint = rtScr.CenterPoint();
  HWPoint tCenterPointSrc(tPoint.x, tPoint.y);
  tPoint = rtDist.TopLeft();
  HWPoint tPointLeftTop (tPoint.x, tPoint.y);
  tPoint = rtDist.BottomRight();
  HWPoint tPointRightBottom(tPoint.x, tPoint.y);
  long nSrcHeight = rtScr.Height();
  long nSrcWidth = rtScr.Width();

  if (nSrcWidth  <= 0)
  {
    nSrcWidth = 1;
  }
  if (nSrcHeight  <= 0)
  {
    nSrcHeight = 1;
  }
  fZoom_X = (tPointRightBottom.x - tPointLeftTop.x) * 1.0f/ nSrcWidth;  
  fZoom_Y = (tPointRightBottom.y - tPointLeftTop.y) * 1.0f/ nSrcHeight; 
  if (!blStretch)
  {
    fZoom_X = fZoom_Y = _min(fZoom_X, fZoom_Y);
  }
  //fZoom_Whole = __min(fZoom_X, fZoom_Y);
  x = (float)(tPtOrg.x - tCenterPointSrc.x) * fZoom_X;
  y = (float)(tPtOrg.y - tCenterPointSrc.y) * fZoom_Y;

  return HWPoint(x + tCenterPointDist.x, y + tCenterPointDist.y);    
}
HWX_OUT BOOL TraceTransform(HWX_IN_OUT HWPoint *ptTrace, HWX_IN const CRect &rtRectS, HWX_IN const CRect &rtRectD)
{
  assert(ptTrace);  
  BOOL blRet = TRUE;  
  HWPoint *ptIndex = ptTrace;  
  CPoint  tPointTmp = rtRectD.CenterPoint();
  HWPoint tCenterPointD(tPointTmp.x, tPointTmp.y);
  tPointTmp = rtRectS.CenterPoint();
  HWPoint tCenterPointS(tPointTmp.x, tPointTmp.y);  
  float fZoom_Whole = 0.0, fZoom_X = 0.0f, fZoom_Y = 0.0f;
  long nHeightS = rtRectS.Height(), nWidthS = rtRectS.Width();
  long nHeightD = rtRectD.Height(), nWidthD = rtRectD.Width();
  if (nWidthS  <= 0)
  {
    nWidthS = 1;
  }
  if (nHeightS  <= 0)
  {
    nHeightS = 1;
  }
  fZoom_X = nWidthD * 1.0f / nWidthS;  
  fZoom_Y = nHeightD * 1.0f / nHeightS;  
  fZoom_Whole = __min(fZoom_X, fZoom_Y);
  while(!(g_atEndMark[1] == *ptIndex))
  {  
    if (!(g_atEndMark[0] == *ptIndex))
    {
      ptIndex->x = (float)(ptIndex->x - tCenterPointS.x) * fZoom_Whole + tCenterPointD.x;
      ptIndex->y = (float)(ptIndex->y - tCenterPointS.y) * fZoom_Whole + tCenterPointD.y;
    }    
    ptIndex ++;
  }
  return blRet;
}

HWX_OUT BOOL TraceTransform(HWX_IN_OUT std::vector<HWPoint>& vtTrace, 
                            HWX_IN const CRect &rtRectS, HWX_IN const CRect &rtRectD)
{  
  BOOL blRet = TRUE;    
  std::vector<HWPoint>::iterator itvTrace = vtTrace.begin();
  CPoint tPointTmp = rtRectD.CenterPoint();
  HWPoint tCenterPointD  = HWPoint(tPointTmp.x, tPointTmp.y);
  tPointTmp = rtRectS.CenterPoint();
  HWPoint tCenterPointS  =  HWPoint(tPointTmp.x, tPointTmp.y);
  float fZoom_Whole = 0.0, fZoom_X = 0.0f, fZoom_Y = 0.0f;
  long nHeightS = rtRectS.Height(), nWidthS = rtRectS.Width();
  long nHeightD = rtRectD.Height(), nWidthD = rtRectD.Width();
  if (nWidthS  <= 0)
  {
    nWidthS = 1;
  }
  if (nHeightS  <= 0)
  {
    nHeightS = 1;
  }
  fZoom_X = nWidthD * 1.0f / nWidthS;  
  fZoom_Y = nHeightD * 1.0f / nHeightS;  
  fZoom_Whole = __min(fZoom_X, fZoom_Y);
  for (; itvTrace < vtTrace.end(); itvTrace ++)
  {
    itvTrace->x = (float)(itvTrace->x - tCenterPointS.x) * fZoom_Whole + tCenterPointD.x;
    itvTrace->y = (float)(itvTrace->y - tCenterPointS.y) * fZoom_Whole + tCenterPointD.y;
  }  
  return blRet;
}

HWX_OUT HWPoint GetNearestPointOnTrace(const HWPoint *ptTrace, const HWPoint &tPtCur)
{
  assert(ptTrace);
  HWPoint tPointNearest(0, 0);
  double dDistance = MAXSHORT;
  HWPoint tPointNearestTmp;
  double dDistanceTmp = MAXSHORT;    
  const HWPoint *ptCur = ptTrace;
  BOOL blNewStrokeMark = TRUE;
  HWPoint  tPointPre;  
  while(*ptCur == g_atEndMark[1])
  {    
    if(TRUE == blNewStrokeMark)//如果是笔序开始 则第一个结点为当前点
    {        
      if(g_atEndMark[0] == *ptCur)
      {
        blNewStrokeMark = TRUE;
      }
      else
      {
        blNewStrokeMark = FALSE;
        //判断这个笔画是否为单点
        if(g_atEndMark[0] == *(ptCur + 1) || g_atEndMark[1] == *(ptCur + 1))
        {//单点笔画不进行处理
          NULL;
        }
        else
        {              
          tPointPre = *ptCur;
        }
      }
      goto _Continue;
    }          
    if(g_atEndMark[0] == *ptCur)
    {          
      blNewStrokeMark = TRUE;
      goto _Continue;
    }
    //draw      
    dDistanceTmp = Mth_GetMinDist(tPointPre, *ptCur, tPtCur, tPointNearestTmp);
    if (dDistanceTmp < dDistance)
    {
      dDistance = dDistanceTmp;
      tPointNearest = tPointNearestTmp;
    }
    tPointPre = *ptCur;
_Continue:
    ptCur += 1;
  }
  return tPointNearest;  
}
HWX_OUT HWPoint GetNearestPointOnLines(const std::vector<HWPoint> &vLines, const HWPoint &tPtCur)
{
  HWPoint tPointNearest(0, 0);
  double dDistance = MAX_NAME ;
  HWPoint tPointNearestTmp;
  double dDistanceTmp = MAX_NAME;  
  assert(!vLines.empty());   
  HWPoint  tPointPre;
  if (vLines.size() < 1)
  {
    return tPointNearest;
  }  
  std::vector<HWPoint>::const_iterator itvTrace = vLines.begin();
  tPointNearest = *itvTrace;
  tPointPre = *itvTrace ++;
  while(itvTrace < vLines.end())
  {            
    dDistanceTmp = Mth_GetMinDist(tPointPre, *itvTrace, tPtCur, tPointNearestTmp);
    if (dDistanceTmp < dDistance)
    {
      dDistance = dDistanceTmp;
      tPointNearest = tPointNearestTmp;
    }
    tPointPre = *itvTrace;
    itvTrace ++;
  }
  return tPointNearest;  
}
HWX_OUT BOOL IsPointOnLines( const std::vector<HWPoint> &vLines, const HWPoint &tPtCur )
{
  const long nRestrict = 2;
  HWPoint tPointNearest;
  double dDistance = MAX_SHORT ;
  double dDistanceTmp = MAX_SHORT;  
  assert(!vLines.empty());   
  HWPoint  tPointPre, tPointNearestTmp;
  std::vector<HWPoint>::const_iterator itvTrace = vLines.begin();
  tPointPre = *itvTrace ++;
  while(itvTrace < vLines.end())
  {    
    dDistanceTmp = Mth_GetMinDist(tPointPre, *itvTrace, tPtCur, tPointNearestTmp);
    if (dDistanceTmp < dDistance)
    {
      dDistance = dDistanceTmp;
      tPointNearest = tPointNearestTmp;
    }
    tPointPre = *itvTrace;
    itvTrace ++;
  }
  return nRestrict >= (long)dDistance;  
}

HWX_OUT HWPoint PtTrans(CPoint& tPt, CRect& in_srcRect, CRect& in_dstRect)
  {
  double dbScaleX = 1.0, dbScaleY = 1.0;
  CPoint tPtCenterS, tPtCenterD;
  HWPoint tRet;
  if (in_srcRect.Width() > 1.0)
    {
    dbScaleX = in_dstRect.Width() * 1.0 / in_srcRect.Width();
    }
  if (in_srcRect.Height() > 1.0)
    {
    dbScaleY = in_dstRect.Height() * 1.0 / in_srcRect.Height();
    }
  tPtCenterS = in_srcRect.CenterPoint();
  tPtCenterD = in_dstRect.CenterPoint();
  //dbScaleX =  dbScaleY = _min(dbScaleX, dbScaleY);
  tRet.x = (tPt.x - tPtCenterS.x) * dbScaleX + tPtCenterD.x;
  tRet.y = (tPt.y - tPtCenterS.y) * dbScaleY + tPtCenterD.y;
  return tRet;
  }
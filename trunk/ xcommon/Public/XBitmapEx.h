/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XBitmapEx.h					
文件内容:			Bitmap简单处理
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/08/04
*********************************************************************/

#ifndef X_BITMAPEX
#define X_BITMAPEX


#ifdef __cplusplus
extern "C"{
#endif

	DLLXEXPORT HBITMAP WINAPI CreateGrayscaleBitmap(HBITMAP hBitmap, COLORREF crTrans);
	DLLXEXPORT HBITMAP WINAPI CreateDarkerBitmap(HBITMAP hBitmap, COLORREF crTrans);
	DLLXEXPORT HBITMAP WINAPI CreateBitmapMask(HBITMAP hSourceBitmap, COLORREF crTransColor);


#ifdef __cplusplus
}
#endif


#endif //X_BITMAPEX
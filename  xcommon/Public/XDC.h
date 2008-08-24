/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XDC.h					
文件内容:	
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/08/19
*********************************************************************/

#ifndef HWX_XDC_H
#define HWX_XDC_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTransparentBitmap
{
public:
	CXTransparentBitmap(HBITMAP hBitmap);
	COLORREF GetTransparentColor() const;
	HICON ConvertToIcon() const;
protected:
	HBITMAP m_hBitmap;  
};
class CXBufferDC : public CDC
{
public:
	CXBufferDC(HDC hDestDC, const CRect& rcPaint);	
	CXBufferDC(CPaintDC& paintDC);	
	virtual ~CXBufferDC();
	void Discard();	
	CDC* GetDestDC();

protected:

	HDC     m_hDestDC;    // Handle to the destination device context.
	CBitmap m_bitmap;     // Bitmap in memory device context
	CRect   m_rect;       // Size of the area to paint.
	HGDIOBJ m_hOldBitmap; // Handle to the previously selected bitmap.
};

class CXBufferDCEx : public CDC
{
public:
	CXBufferDCEx(HDC hDestDC, const CRect rcPaint);
	virtual ~CXBufferDCEx();

protected:

	HDC     m_hDestDC;    // Handle to the destination device context.
	HBITMAP m_bitmap;     // Bitmap in memory device context
	CRect   m_rect;       // Size of the area to paint.
	HGDIOBJ m_hOldBitmap; // Handle to the previously selected bitmap.
};

class CXBitmapDC
{
public:
	CXBitmapDC(CDC* pDC, CBitmap* pBitmap);
	CXBitmapDC(CDC* pDC, HBITMAP hBitmap);  
	virtual ~CXBitmapDC();
	void SetBitmap(CBitmap* pBitmap);
protected:
	HDC     m_hDC;        // Device context handle.
	HGDIOBJ m_hOldBitmap; // Handle to the previously selected bitmap.
};

class CXFontDC
{
public:
	CXFontDC(CDC* pDC, CFont* pFont = NULL);
	CXFontDC(CDC* pDC, CFont* pFont, COLORREF clrTextColor); 
	virtual ~CXFontDC();
	void SetFont(CFont* pFont);
	void SetColor(COLORREF clrTextColor);	
	void SetFontColor(CFont* pFont, COLORREF clrTextColor);
	void ReleaseFont();
	void ReleaseColor();

protected:
	CDC*     m_pDC;             // Stored pointer to a device context.
	CFont*   m_pOldFont;        // Stored pointer to an initial font from the device context.
	COLORREF m_clrOldTextColor; // Stored an initial text color from the device context.
};
class CXPenDC
{
public:
	CXPenDC(CDC* pDC, CPen* pPen);
	CXPenDC(HDC hDC, COLORREF crColor); 
	virtual ~CXPenDC();	
	COLORREF Color();
	void Color(COLORREF crColor); // <combine CXPenDC::Color>

protected:
	CPen m_pen;     // Pen selected into device context.
	HDC  m_hDC;     // Device context handle.
	HPEN m_hOldPen; // Handle to the previously selected pen.
};

class CXBrushDC
{
public:	
	CXBrushDC(HDC hDC, COLORREF crColor);	
	virtual ~CXBrushDC();	
	void Color(COLORREF crColor);
protected:
	CBrush m_brush;     // Brush selected into device context.
	HDC    m_hDC;       // Device context handle.
	HBRUSH m_hOldBrush; // Handle to the previously selected brush.
};

class CXCompatibleDC : public CDC
{
public:	
	CXCompatibleDC(CDC* pDC, CBitmap* pBitmap);
	CXCompatibleDC(CDC* pDC, HBITMAP hBitmap); 	
	virtual ~CXCompatibleDC();
protected:
	HBITMAP m_hOldBitmap; 
};

#endif //HWX_XDC_H

#ifndef X_BITMAP
#define X_BITMAP

#include "XSize.h"
#include "XRect.h"
#include <windows.h>

class DLLXEXPORT CXBitmap  
{
public:
	CXBitmap ();
	CXBitmap (const HBITMAP& hBitmap);
	CXBitmap (const CXBitmap & bmp);	
	virtual ~CXBitmap();
	operator HBITMAP() const;
	BOOL Load(HINSTANCE hInst, const TCHAR *const);
	BOOL Load(HINSTANCE hInst, LONG nID);
	BOOL Load(const TCHAR *const path);
	BOOL LoadImage(LPCTSTR);
	virtual BOOL AssertValid() const;
  BOOL Attach(HBITMAP);
  HBITMAP Detach();
	HBITMAP Clip(const LONG& xSrc, const LONG& ySrc, const LONG& nWidth, const LONG& nHeight) const;
	HBITMAP Clip(const CXRect&) const;
	void GetSize(LONG & width, LONG & height) const;
	CXSize GetSize() const;  
	CXBitmap (HDC canvas, const LONG& nWidth, const LONG& nHeight);
	void CreateCompatible (HDC canvas, const LONG& nWidth, const LONG& nHeight);
  void Free();
  HBITMAP CreateGrayBitmap(COLORREF crTrans);
  HBITMAP CreateDarkerBitmap(COLORREF crTrans);
	HBITMAP CreateBitmapMask(COLORREF crTransColor);
	HRGN  BitmapToRegion (COLORREF cTransparentColor, COLORREF cTolerance = 0x101010);

  static HBITMAP CreateGrayBitmap(HBITMAP , COLORREF crTrans);
  static HBITMAP CreateDarkerBitmap(HBITMAP , COLORREF crTrans);
	static HBITMAP CreateBitmapMask(HBITMAP , COLORREF crTrans);
	static HRGN  BitmapToRegion (HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance = 0x101010);
  void operator=(const CXBitmap &);	 
	operator HBITMAP();
protected:
  static COLORREF DarkenColor(COLORREF crColor, double dFactor);
protected:
	HBITMAP m_hBitmap;	
};

#endif // _BITMAP_H_

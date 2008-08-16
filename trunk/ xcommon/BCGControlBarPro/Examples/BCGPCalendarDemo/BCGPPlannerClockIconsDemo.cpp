// BCGPPlannerClockIcons.cpp : implementation file
//

#include "stdafx.h"
#include "bcgpcalendardemo.h"
#include "BCGPPlannerClockIconsDemo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPPlannerClockIconsDemo

IMPLEMENT_DYNCREATE(CBCGPPlannerClockIconsDemo, CBCGPPlannerClockIcons)

CBCGPPlannerClockIconsDemo::CBCGPPlannerClockIconsDemo()
{
}

CBCGPPlannerClockIconsDemo::~CBCGPPlannerClockIconsDemo()
{
}

BOOL CBCGPPlannerClockIconsDemo::Initialize ()
{
	if (IsInitialized ())
	{
		return TRUE;
	}

	CImageList	ilClock;
	CImageList	ilHours;
	CImageList	ilMinuts;

	CSize szSize (0, 0);

	{
		CBitmap bmp;
		if (!bmp.LoadBitmap (IDB_CALENDAR_CLOCK_C))
		{
			return FALSE;
		}

		BITMAP bm;
		bmp.GetBitmap (&bm);

		szSize.cx = bm.bmWidth / 2;
		szSize.cy = bm.bmHeight;

		ilClock.Create (szSize.cx, szSize.cy, ILC_COLOR24, 2, 0);
		ilClock.Add (&bmp, (CBitmap*)NULL);
	}

	if (ilClock.GetSafeHandle () == NULL)
	{
		return FALSE;
	}

	{
		CBitmap bmp;
		if (!bmp.LoadBitmap (IDB_CALENDAR_CLOCK_H))
		{
			return FALSE;
		}

		BITMAP bm;
		bmp.GetBitmap (&bm);

		ASSERT (szSize.cx == bm.bmWidth / 24);
		ASSERT (szSize.cy == bm.bmHeight);

		ilHours.Create (szSize.cx, szSize.cy, ILC_COLOR24 | ILC_MASK, 24, 0);
		ilHours.Add (&bmp, RGB(255, 0, 255));
	}

	if (ilHours.GetSafeHandle () == NULL)
	{
		return FALSE;
	}

	{
		CBitmap bmp;
		if (!bmp.LoadBitmap (IDB_CALENDAR_CLOCK_M))
		{
			return FALSE;
		}

		BITMAP bm;
		bmp.GetBitmap (&bm);

		ASSERT (szSize.cx == bm.bmWidth / 12);
		ASSERT (szSize.cy == bm.bmHeight);

		ilMinuts.Create (szSize.cx, szSize.cy, ILC_COLOR24 | ILC_MASK, 12, 0);
		ilMinuts.Add (&bmp, RGB(255, 0, 255));
	}

	if (ilMinuts.GetSafeHandle () == NULL)
	{
		return FALSE;
	}

	CBitmap bmp;

	HDC hDC = ::GetDC (NULL);
	CDC* pDC = CDC::FromHandle (hDC);

	CDC dc;
	if (dc.CreateCompatibleDC (pDC))
	{
		BITMAPINFO bi;

		// Fill in the BITMAPINFOHEADER
		bi.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
		bi.bmiHeader.biWidth         = szSize.cx * 24 * 12;
		bi.bmiHeader.biHeight        = szSize.cy;
		bi.bmiHeader.biPlanes        = 1;
		bi.bmiHeader.biBitCount      = 32;
		bi.bmiHeader.biCompression   = BI_RGB;
		bi.bmiHeader.biSizeImage     = bi.bmiHeader.biWidth * bi.bmiHeader.biHeight;
		bi.bmiHeader.biXPelsPerMeter = 0;
		bi.bmiHeader.biYPelsPerMeter = 0;
		bi.bmiHeader.biClrUsed       = 0;
		bi.bmiHeader.biClrImportant  = 0;

		COLORREF* pBits = NULL;
		HBITMAP hmbpDib = CreateDIBSection (
			dc.GetSafeHdc (), &bi, DIB_RGB_COLORS, (void **)&pBits,
			NULL, NULL);

		if (hmbpDib == NULL)
		{
			ASSERT (FALSE);
			return FALSE;
		}

		bmp.Attach (hmbpDib);

		if (bmp.GetSafeHandle () != NULL)
		{
			CBitmap* pOld = dc.SelectObject (&bmp);

			for (int nHour = 0; nHour < 24; nHour++)
			{
				BOOL bPM = nHour >= 12;

				for (int nMinuts = 0; nMinuts < 12; nMinuts++)
				{
					CPoint pt ((nHour * 12 + nMinuts) * szSize.cx, 0);

					ilClock.Draw (&dc, bPM ? 1 : 0, 
						pt, ILD_IMAGE);

					ilHours.Draw (&dc, (nHour - (bPM ? 12 : 0)) * 2 + 
						(nMinuts >= 5 ? 1 : 0), 
						pt, ILD_NORMAL);

					ilMinuts.Draw(&dc, nMinuts,	pt, ILD_NORMAL);
				}
			}

		    dc.SelectObject (pOld);
		}
	}

	::ReleaseDC (NULL, hDC);

	if (bmp.GetSafeHandle () != NULL)
	{
		m_szSize = szSize;

		m_ilClockIcons.Create (m_szSize.cx, m_szSize.cy, ILC_COLOR24 | ILC_MASK, 
			24 * 12, 0);
		m_ilClockIcons.Add (&bmp, RGB (255, 0, 255));
		m_bInitialized = TRUE;
	}

	return m_bInitialized;
}

#ifdef _DEBUG
void CBCGPPlannerClockIconsDemo::AssertValid() const
{
	CBCGPPlannerClockIcons::AssertValid();
}

void CBCGPPlannerClockIconsDemo::Dump(CDumpContext& dc) const
{
	CBCGPPlannerClockIcons::Dump(dc);
}
#endif

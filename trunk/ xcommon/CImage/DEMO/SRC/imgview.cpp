// imgview.cpp : implementation of the CImageView class
//

#include "stdafx.h"
#include "app.h"
#include "frame.h"
#include "helpers.h"

#include "imgdoc.h"
#include "imgview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static const char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageView

IMPLEMENT_DYNCREATE(CImageView, CScrollView)

BEGIN_MESSAGE_MAP(CImageView, CScrollView)
	//{{AFX_MSG_MAP(CImageView)
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_CIMAGE_ROTATE, OnRotate)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageView construction/destruction
CImageView::CImageView() : CScrollView()
{
   VERIFY(m_brHatch.CreateHatchBrush(HS_DIAGCROSS, RGB(0, 0, 0)));
}

/////////////////////////////////////////////////////////////////////////////
CImageView::~CImageView()
{
}
/////////////////////////////////////////////////////////////////////////////
BOOL CImageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}
/////////////////////////////////////////////////////////////////////////////
// CImageView drawing

static BOOL bFlickerFree = TRUE;

/* OnDraw implemented by Troels K. */
void CImageView::OnDraw(CDC* pDC)
{
   const BOOL bPrinting = pDC->IsPrinting();

   CImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CImageObject& image = pDoc->GetImage(); 

   CMemDC* pMemDC = NULL;
   if (bFlickerFree && (!bPrinting))
   {
      pDC = pMemDC = new CMemDC(pDC);
   }

   if (!bPrinting)
   {
      CRect rect;
      GetClientRect(&rect);

      //pDC->FillSolidRect(rect, RGB(0,255,0));

      rect.right  = max(rect.right , m_totalDev.cx);
      rect.bottom = max(rect.bottom, m_totalDev.cy);

      CBrush* old = ::SelectObject(pDC, &m_brHatch);
      pDC->FillRect(&rect, &m_brHatch);
      pDC->SelectObject(old);
   }
   
   if (image.IsOpen())
   {
      if (bPrinting)
      {
         const CSize ext(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
         CRect rect;
         RectBestFit(image.GetSize(), CRect(0, 0, ext.cx/2, ext.cy/2), &rect, TRUE, TRUE, TRUE);
         image.Draw(pDC, rect);
      }
      else if (pDoc->GetStretchMode())
      {
	      CRect rect;
	      GetClientRect(&rect);
         image.Draw(pDC, rect);
      }
      else
      {
         VERIFY(image.Draw(pDC));
      }
   }
   delete pMemDC;
}
/////////////////////////////////////////////////////////////////////////////
// CImageView printing
BOOL CImageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
   pInfo->SetMaxPage(1);
	return DoPreparePrinting(pInfo);
}

void CImageView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
   CScrollView::OnBeginPrinting(pDC, pInfo);
}

void CImageView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
   CScrollView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CImageView diagnostics

#ifdef _DEBUG
void CImageView::AssertValid() const
{
	CScrollView::AssertValid();
}
/////////////////////////////////////////////////////////////////////////////
void CImageView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
/////////////////////////////////////////////////////////////////////////////
CImageDoc* CImageView::GetDocument() // non-debug version is inline
{
   ASSERT_KINDOF(CImageDoc, m_pDocument);
	return (CImageDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageView message handlers
void CImageView::OnInitialUpdate() 
{
   SetScrollSizes();
   CScrollView::OnInitialUpdate();
   //PostMessage(WM_COMMAND, ID_FILE_PRINT_PREVIEW);
}

/////////////////////////////////////////////////////////////////////////////
void CImageView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// We get the RGB values at the point the user selects
   point+=GetScrollPosition();
	TCHAR s[80];
	RGBQUAD rgb={0,0,0};
	float x = (float)point.x;
	float y = (float)point.y;
	long yflip;
	//CPoint pos(-GetScrollPosition());
	// Subtract the position that the image is being drawn
	// at - not necessarily (0, 0)

	CRect rect;
	GetClientRect(&rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	CImageDoc* pDoc = GetDocument();
	CImageObject&  image = pDoc->GetImage();

	if (!image.IsOpen())	return;

	if (pDoc->GetStretchMode())	
   {
		x *= image.GetWidth()/(float)width;
		y *= image.GetHeight()/(float)height;
	}
	if (image.IsInside((int)x, (int)y))	
   {
		yflip=(long)(image.GetHeight() - y - 1);
		_stprintf(s,_T("X = %.0f Y = %.0f  Idx=%d"), x, y, image.GetPixelIndex((long)x, yflip));
		rgb=image.GetPixelColor((long)x, yflip);
		_stprintf(&s[lstrlen(s)],_T("  RGB = (%d, %d, %d)"), rgb.rgbRed , rgb.rgbGreen , rgb.rgbBlue );

		//Enable these lines if you want draw over the image	
		//if ((nFlags & MK_LBUTTON)==MK_LBUTTON){
		//	ima->SetPixelColor((long)x,yflip,RGB(rand()/(RAND_MAX/256),rand()/(RAND_MAX/256),rand()/(RAND_MAX/256)));
		//	Invalidate(0);
		//}
	
	} 
   else 
   {
      _tcscpy(s,_T(" "));
   }
	
   CStatusBar& statusBar = ((CFrame*)CApp::GetApp().m_pMainWnd)->GetStatusBar();
	statusBar.SetPaneText(0, s);
	
	CScrollView::OnMouseMove(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
BOOL CImageView::OnEraseBkgnd(CDC* /*pDC*/) 
{
   return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
void CImageView::OnSetFocus(CWnd* pOldWnd) 
{
	CScrollView::OnSetFocus(pOldWnd);
	
	CImageDoc* pDoc = GetDocument();
	if (pDoc) pDoc->UpdateStatusBar();
}

void CImageView::SetScrollSizes()
{
   CImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

   CImageObject& image = pDoc->GetImage();

   BOOL bResize = FALSE;
   CSize size(0, 0);
   if ((!pDoc->GetStretchMode()) && image.IsOpen())
   {
		size = image.GetSize();
      bResize = (size.cx > 0) && (size.cy);
   }
   CScrollView::SetScrollSizes(MM_TEXT, size);
   if (bResize)ResizeParentToFit();
}

void CImageView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
   switch (lHint)
   {
      case CImageDoc::ENUM_hint_newimage:
         SetScrollSizes();
         break;
      default:
         break;
   }
   CScrollView::OnUpdate(pSender, lHint, pHint);
}

extern BOOL DoModal_Rotate(CWnd* pParentWnd, double* angle);

void CImageView::OnRotate() 
{
	CImageDoc* pDoc = GetDocument();
   CImageObject& image = pDoc->GetImage();
	if (image.IsOpen())
   {
		double angle = 90.0;
      if (!DoModal_Rotate(this, &angle))
      {
         return;
      }
      if (!pDoc->Rotate(angle))
      {
         MessageBox(_T("Failed to rotate"));
      }
	}
}


// ReportDemoView.cpp : implementation of the CReportDemoView class
//

#include "stdafx.h"
#include "ReportDemo.h"

#include "ReportDemoDoc.h"
#include "ReportDemoView.h"
#include "BCGPGridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlagItem class

CFlagItem::CFlagItem(
	CBCGPToolBarImages&	icons,
	int nSelectedIcon,
	DWORD dwData) :
	m_Icons (icons)
{
	m_varValue = (long) nSelectedIcon;
	AllowEdit (FALSE);
}

BOOL CFlagItem::OnClickValue (UINT uiMsg, CPoint point)
{
	if (CBCGPGridItem::OnClickValue (uiMsg, point))
	{
		return TRUE;
	}

	if (uiMsg == WM_LBUTTONDOWN)
	{
		int nIndex = (long) m_varValue;

		if (nIndex < 0)
		{
			return FALSE;
		}

		if (nIndex > m_Icons.GetCount ())
		{
			return FALSE;
		}

		// shift nIndex
		nIndex++;
		nIndex %= m_Icons.GetCount ();

		m_varValue = (long) nIndex;

		CBCGPGridItemID id = GetGridItemID ();
		m_pGridRow->OnItemChanged (this, id.m_nRow, id.m_nColumn);
		Redraw ();

		return TRUE;
	}

	return FALSE;
}

void CFlagItem::OnDrawValue (CDC* pDC, CRect rect)
{
	int nIndex = (long) m_varValue;

	// fill background
	CRect rectFill = rect;
	rectFill.top++;
	rectFill.right++;
	pDC->FillRect (rectFill, &globalData.brLight);

	if (nIndex < 0)
	{
		return;
	}

	if (nIndex > m_Icons.GetCount ())
	{
		return;
	}

	IMAGEINFO imageInfo;
	memset (&imageInfo, 0, sizeof (IMAGEINFO));

	CPoint pt = rect.TopLeft ();
	CSize sizeIcon = m_Icons.GetImageSize ();

	int xOffset = (rect.Width () - sizeIcon.cx) / 2;
	int yOffset = (rect.Height () - sizeIcon.cy) / 2;
	pt.Offset (max (0, xOffset), max (0, yOffset));

	CBCGPDrawState ds;
	m_Icons.PrepareDrawImage (ds);
	m_Icons.Draw (pDC, pt.x, pt.y, nIndex);
	m_Icons.EndDrawImage (ds);
}

/////////////////////////////////////////////////////////////////////////////
// CReportDemoView

IMPLEMENT_DYNCREATE(CReportDemoView, CBCGPReportView)

BEGIN_MESSAGE_MAP(CReportDemoView, CBCGPReportView)
	//{{AFX_MSG_MAP(CReportDemoView)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_COLUMNSSELECTOR, OnViewColumnsselector)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLUMNSSELECTOR, OnUpdateViewColumnsselector)
	ON_WM_DESTROY()
	ON_COMMAND(ID_VIEW_GROUPBOX, OnViewGroupbox)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GROUPBOX, OnUpdateViewGroupbox)
	ON_COMMAND(ID_VIEW_COLUMN_AUTO_RESIZE, OnViewColumnAutoResize)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLUMN_AUTO_RESIZE, OnUpdateViewColumnAutoResize)
	ON_COMMAND(ID_REPORT_EXPAND_ALL, OnReportExpandAll)
	ON_COMMAND(ID_REPORT_COLLAPSE_ALL, OnReportCollapseAll)
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportDemoView construction/destruction

CReportDemoView::CReportDemoView()
{
	// TODO: add construction code here

}

CReportDemoView::~CReportDemoView()
{
}

BOOL CReportDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CBCGPReportView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CReportDemoView drawing

void CReportDemoView::OnDraw(CDC* pDC)
{
	CBCGPReportView::OnDraw (pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CReportDemoView printing

void CReportDemoView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CReportDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CReportDemoView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CBCGPReportView::OnBeginPrinting (pDC, pInfo);
}

void CReportDemoView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CBCGPReportView::OnEndPrinting (pDC, pInfo);
}

void CReportDemoView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	ASSERT_VALID (pDC);
	ASSERT (pInfo != NULL);

	// Page margins:
	double dLeftOffset = 0.5;
	double dTopOffset = 0.5;
	double dRightOffset = 0.5;
	double dBottomOffset = 0.5;
	pInfo->m_rectDraw.DeflateRect(
		(int)(pDC->GetDeviceCaps(LOGPIXELSX) * dLeftOffset),
		(int)(pDC->GetDeviceCaps(LOGPIXELSY) * dTopOffset),
		(int)(pDC->GetDeviceCaps(LOGPIXELSX) * dRightOffset),
		(int)(pDC->GetDeviceCaps(LOGPIXELSY) * dBottomOffset));
	
	CBCGPReportView::OnPrint(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CReportDemoView diagnostics

#ifdef _DEBUG
void CReportDemoView::AssertValid() const
{
	CBCGPReportView::AssertValid();
}

void CReportDemoView::Dump(CDumpContext& dc) const
{
	CBCGPReportView::Dump(dc);
}

CReportDemoDoc* CReportDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CReportDemoDoc)));
	return (CReportDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CReportDemoView message handlers

void CReportDemoView::OnContextMenu(CWnd*, CPoint point)
{
	if (point == CPoint (-1, -1))
	{
		CBCGPGridRow* pSel = GetReportCtrl ()->GetCurSel ();
		if (pSel != NULL)
		{
			CRect rect = pSel->GetRect ();
			ClientToScreen (&rect);

			point.x = rect.left;
			point.y = rect.bottom;
		}
	}

	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}

BOOL CReportDemoView::AddMail (int nIcon, LPCTSTR strFrom, LPCTSTR strTo,
		 LPCTSTR strSubject, COleDateTime dateCreated, COleDateTime dateReceived, long lSize, 
		 IMPORTANCE importance/* = IMPORTANCE_NORMAL*/,
		 BOOL bHasAttachment/* = FALSE*/,
		 int nFlag/* = 0*/)
{
	CBCGPReportCtrl* pReportCtrl = GetReportCtrl ();
	CBCGPGridRow* pRow = pReportCtrl->CreateRow (pReportCtrl->GetColumnCount ());

	//----------------
	// Set importance:
	//----------------
	int nImportanceIcon = -1;
	switch (importance)
	{
	case IMPORTANCE_HIGH:
		nImportanceIcon = 2;
		break;

	case IMPORTANCE_LOW:
		nImportanceIcon = 3;
		break;
	}

	pRow->GetItem (0)->SetImage (nImportanceIcon, FALSE);
	pRow->GetItem (1)->SetImage (nIcon, FALSE);
	pRow->GetItem (2)->SetImage (bHasAttachment ? 5 : -1, FALSE);

	pRow->GetItem (3)->SetValue (strFrom, FALSE);
	pRow->GetItem (4)->SetValue (strSubject, FALSE);
	pRow->GetItem (5)->SetValue (strTo, FALSE);

	pRow->ReplaceItem (7, new CBCGPGridDateTimeItem (dateCreated), FALSE);
	pRow->ReplaceItem (8, new CBCGPGridDateTimeItem (dateReceived), FALSE);

	pRow->GetItem (9)->SetValue (lSize, FALSE);

	pRow->ReplaceItem (11, new CFlagItem (m_Flags, nFlag), FALSE);

	pReportCtrl->AddRow (pRow, FALSE);

	return TRUE;
}

inline int Rand (int nMax)
{
	int nRes = rand () % nMax;
	return min (nRes, nMax - 1);
}

void CReportDemoView::OnInitialUpdate() 
{
	CBCGPReportView::OnInitialUpdate();
	
	CBCGPReportCtrl* pReportCtrl = GetReportCtrl ();
	ASSERT_VALID (pReportCtrl);

	pReportCtrl->RemoveAll ();

	#define NAMES_NUM	7
	LPCTSTR arNames [NAMES_NUM] =
	{
		_T("John Smith"),
		_T("Support Team"),
		_T("Peter Brown"),
		_T("Matt Johnson"),
		_T("James Lee"),
		_T("Customer Service"),
		_T("Webmaster"),
	};

	#define SUBJ_NUM	8
	LPCTSTR arSubjs [SUBJ_NUM] =
	{
		_T("Important information"),
		_T("Customer Newsletter"),
		_T("Question"),
		_T("Business proposal"),
		_T("Request more info about your products"),
		_T("Thank you!"),
		_T("Your application has been approved"),
		_T("Please read this message"),
	};

	srand( (unsigned)time( NULL ) );

	for (int i = 0; i < 100; i++)
	{
		COleDateTime now = COleDateTime::GetCurrentTime ();
		COleDateTimeSpan span (
			Rand (5),
			Rand (23),
			Rand (59),
			0);
		COleDateTimeSpan span2 (
			0,
			Rand (2),
			Rand (59),
			Rand (59));

		AddMail (Rand (2), // Icon
			arNames [Rand (NAMES_NUM)],
			arNames [Rand (NAMES_NUM)],
			arSubjs [Rand (SUBJ_NUM)],
			now - span - span2,
			now - span, 
			Rand (200),
			(IMPORTANCE) Rand (3),
			Rand (2), 
			Rand (7));
	}

	pReportCtrl->AdjustLayout ();
}

int CReportDemoView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPReportView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CBCGPReportCtrl* pReportCtrl = GetReportCtrl ();
	ASSERT_VALID (pReportCtrl);

	//------------------
	// Load grid images:
	//------------------
	CBitmap bmp;
	bmp.LoadBitmap (IDB_LIST_IMAGES);

	m_Images.Create (16, 16, ILC_COLOR24 | ILC_MASK, 0, 0);
	m_Images.Add (&bmp, RGB (255, 0, 255));

	pReportCtrl->SetImageList (&m_Images);

	//------------------------------
	// Load images for "flag" items:
	//------------------------------
	m_Flags.SetImageSize (CSize (14, 14));
	m_Flags.SetTransparentColor (RGB (255, 0, 255));
	m_Flags.Load (IDB_FLAGS256);

	//----------------
	// Insert columns:
	//----------------
	pReportCtrl->InsertColumn (0, _T("Importance"), 20, 1);
	pReportCtrl->InsertColumn (1, _T("Icon"), 20, 0);
	pReportCtrl->InsertColumn (2, _T("Attachment"), 20, 2);
	pReportCtrl->InsertColumn (3, _T("From"), 150);
	pReportCtrl->InsertColumn (4, _T("Subject"), 150);
	pReportCtrl->InsertColumn (5, _T("To"), 140);
	pReportCtrl->InsertColumn (6, _T("CC"), 120);
	pReportCtrl->InsertColumn (7, _T("Created"), 130);
	pReportCtrl->InsertColumn (8, _T("Received"), 130);
	pReportCtrl->InsertColumn (9, _T("Size"), 80);
	pReportCtrl->InsertColumn (10, _T("Categories"), 80);
	pReportCtrl->InsertColumn (11, _T("Flag Status"), 20, 3);

	pReportCtrl->SetColumnAlign (1, HDF_RIGHT);	// Icon
	pReportCtrl->SetColumnAlign (9, HDF_RIGHT);	// Size

	pReportCtrl->SetColumnLocked (0);
	pReportCtrl->SetColumnLocked (2);
	pReportCtrl->SetColumnLocked (11);

	pReportCtrl->SetColumnVisible (4, FALSE);
	pReportCtrl->SetColumnVisible (5, FALSE);
	pReportCtrl->SetColumnVisible (6, FALSE);
	pReportCtrl->SetColumnVisible (8, FALSE);
	pReportCtrl->SetColumnVisible (10, FALSE);

	//-------------------
	// Set group columns:
	//-------------------
	pReportCtrl->InsertGroupColumn (0, 8 /* Received */);
	pReportCtrl->InsertGroupColumn (1, 4 /* Subject */);

	//--------------------------
	// Create header image list:
	//--------------------------
	const int nImageWidth = 12;
	m_ImagesHeader.Create (IDB_HEADER_IMAGES, nImageWidth, 0, RGB (255, 0, 255));

	pReportCtrl->SetHeaderImageList (&m_ImagesHeader);

	pReportCtrl->EnableHeader ();
	pReportCtrl->EnableColumnAutoSize (TRUE);

	pReportCtrl->SetWholeRowSel (TRUE);
	pReportCtrl->SetSingleSel (TRUE);
	pReportCtrl->EnableMarkSortedColumn (TRUE);

	pReportCtrl->EnableGroupByBox (TRUE);
	
	pReportCtrl->LoadState (NULL);

	return 0;
}

void CReportDemoView::OnViewColumnsselector() 
{
	GetReportCtrl ()->ShowColumnsChooser (
		!GetReportCtrl ()->IsColumnsChooserVisible ());
}

void CReportDemoView::OnUpdateViewColumnsselector(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (GetReportCtrl ()->IsColumnsChooserVisible ());
}

void CReportDemoView::OnDestroy() 
{
	CBCGPReportCtrl* pReportCtrl = GetReportCtrl ();
	ASSERT_VALID (pReportCtrl);

	pReportCtrl->SaveState (NULL);

	CBCGPReportView::OnDestroy();
}

void CReportDemoView::OnViewGroupbox() 
{
	GetReportCtrl ()->EnableGroupByBox (!GetReportCtrl ()->IsGroupByBox ());
}

void CReportDemoView::OnUpdateViewGroupbox(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (GetReportCtrl ()->IsGroupByBox ());
}

void CReportDemoView::OnViewColumnAutoResize() 
{
	GetReportCtrl ()->EnableColumnAutoSize (!GetReportCtrl ()->IsColumnAutoSizeEnabled ());
}

void CReportDemoView::OnUpdateViewColumnAutoResize(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (GetReportCtrl ()->IsColumnAutoSizeEnabled ());
}

void CReportDemoView::OnReportExpandAll() 
{
	GetReportCtrl ()->ExpandAll ();
}

void CReportDemoView::OnReportCollapseAll() 
{
	GetReportCtrl ()->ExpandAll (FALSE);
}

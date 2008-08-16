// BCGPMSOffice2007DemoView.cpp : implementation of the CBCGPMSOffice2007DemoView class
//

#include "stdafx.h"
#include "BCGPMSOffice2007Demo.h"
#include "MainFrm.h"

#include "BCGPMSOffice2007DemoDoc.h"
#include "CntrItem.h"
#include "BCGPMSOffice2007DemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if _MSC_VER >= 1300
	#define WPD_CHARFORMAT CHARFORMAT2
	#define WPD_PARAFORMAT PARAFORMAT2
#else
	#define WPD_CHARFORMAT CHARFORMAT
	#define WPD_PARAFORMAT PARAFORMAT
#endif

struct CCharFormat : public WPD_CHARFORMAT
{
	CCharFormat() {cbSize = sizeof(WPD_CHARFORMAT);}
	BOOL operator==(CCharFormat& cf);
};

int GetTwipSize (CString str)
{
	LPCTSTR lpszText = str;

	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	if (lpszText[0] == NULL)
		return -1; // no text in control

	double d = _tcstod(lpszText, (LPTSTR*)&lpszText);
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	if (*lpszText != NULL)
		return -2;   // not terminated properly

	return (d<0.) ? 0 : (int)(d*20.);
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPMSOffice2007DemoView

BOOL CBCGPMSOffice2007DemoView::m_bForceReloadBmps = FALSE;

IMPLEMENT_DYNCREATE(CBCGPMSOffice2007DemoView, CRichEditView)

BEGIN_MESSAGE_MAP(CBCGPMSOffice2007DemoView, CRichEditView)
	//{{AFX_MSG_MAP(CBCGPMSOffice2007DemoView)
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_WRITE_CLIPBOARD, OnClipboardDialog)
	ON_COMMAND(ID_WRITE_FONT, OnFontDialog)
	ON_COMMAND(ID_WRITE_PARAGRAPH, OnParagraphDialog)
	ON_COMMAND(ID_WRITE_STYLE, OnStyleDialog)
	ON_COMMAND(ID_PARA_PAGESETUP, OnPagesetupDialog)
	ON_WM_RBUTTONUP()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_PAGELAYOUT_INDENT_LEFT, OnIndentLeft)
	ON_COMMAND(ID_PAGELAYOUT_INDENT_RIGHT, OnIndentRight)
	ON_COMMAND(ID_PAGELAYOUT_SPACE_AFTER, OnSpaceAfter)
	ON_COMMAND(ID_PAGELAYOUT_SPACE_BEFORE, OnSpaceBefore)
	ON_COMMAND(ID_VIEW_RULER, OnViewRuler)
	ON_COMMAND(ID_VIEW_GRIDLINES, OnViewGridLines)
	ON_COMMAND(ID_VIEW_PROPERTIES, OnViewProps)
	ON_COMMAND(ID_VIEW_DOCUMENTMAP, OnViewDocMap)
	ON_COMMAND(ID_VIEW_TRUMBNAILS, OnViewThumb)
	ON_COMMAND(ID_VIEW_ACTIONBAR, OnViewActionBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RULER, OnUpdateViewRuler)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRIDLINES, OnUpdateViewGridLines)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTIES, OnUpdateViewProps)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DOCUMENTMAP, OnUpdateViewDocMap)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TRUMBNAILS, OnUpdateViewThumb)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ACTIONBAR, OnUpdateViewActionBar)
	ON_COMMAND(ID_INSERT_PICTURE, OnInsertPicture)
	ON_COMMAND(ID_INSERT_CHART, OnInsertChart)
	ON_COMMAND(ID_INSERT_DRAWTABLE, OnInsertTable)
	ON_COMMAND(ID_STATUSBAR_ZOOM, OnZoom)
	ON_COMMAND(ID_STATUSBAR_ZOOM_SLIDER, OnZoomSlider)
	ON_WM_CREATE()
	ON_COMMAND(ID_FONT_COLOR, OnFontColor)
	ON_COMMAND(ID_FONT_FONT, OnFontname)
	ON_COMMAND(ID_FONT_FONTSIZE, OnFontsize)
	ON_COMMAND(ID_FONT_BOLD, OnCharBold)
	ON_UPDATE_COMMAND_UI(ID_FONT_BOLD, OnUpdateCharBold)
	ON_COMMAND(ID_FONT_ITALIC, OnCharItalic)
	ON_UPDATE_COMMAND_UI(ID_FONT_ITALIC, OnUpdateCharItalic)
	ON_COMMAND(ID_FONT_UNDERLINE, OnCharUnderline)
	ON_UPDATE_COMMAND_UI(ID_FONT_UNDERLINE, OnUpdateCharUnderline)
	ON_COMMAND(ID_PARA_CENTER, OnParaCenter)
	ON_UPDATE_COMMAND_UI(ID_PARA_CENTER, OnUpdateParaCenter)
	ON_COMMAND(ID_PARA_ALIGNLEFT, OnParaLeft)
	ON_UPDATE_COMMAND_UI(ID_PARA_ALIGNLEFT, OnUpdateParaLeft)
	ON_COMMAND(ID_PARA_ALIGNRIGHT, OnParaRight)
	ON_UPDATE_COMMAND_UI(ID_PARA_ALIGNRIGHT, OnUpdateParaRight)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_PARA_BULLETS, CRichEditView::OnBullet)
	ON_UPDATE_COMMAND_UI(ID_PARA_BULLETS, CRichEditView::OnUpdateBullet)
	ON_EN_CHANGE(AFX_IDW_PANE_FIRST, OnEditChange)
	ON_COMMAND_RANGE(ID_ACADEMIC, ID_VIEW_ACTIONBAR, OnDummy)
	ON_COMMAND_RANGE(ID_VIEW_ARRANGEALL, ID_WRITE_SELECTFORMATING, OnDummy)
	ON_COMMAND_RANGE(ID_PICTURE_FORMAT_TOOLS_BRIGHTNESS, ID_STATUSBAR_PAGE, OnDummy)
	ON_NOTIFY_REFLECT(EN_SELCHANGE, OnSelChange)
	ON_UPDATE_COMMAND_UI(ID_PRINT_LAYOUT, OnUpdatePrintLayout)
	ON_COMMAND(ID_INSERT_BULLET_FROM_PALETTE, OnInsertBulletFromPalette)
	ON_COMMAND(ID_INSERT_NUMBER_FROM_PALETTE, OnInsertNumberFromPalette)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPMSOffice2007DemoView construction/destruction

CBCGPMSOffice2007DemoView::CBCGPMSOffice2007DemoView()
{
	m_bIsEndOfPrint = FALSE;

	m_bViewRuler = TRUE;
	m_bViewGridLines = TRUE;
	m_bViewProps = FALSE;
	m_bViewDocMap = FALSE;
	m_bViewThumb = FALSE;
	m_bViewActionBar = FALSE;
}

CBCGPMSOffice2007DemoView::~CBCGPMSOffice2007DemoView()
{
}

BOOL CBCGPMSOffice2007DemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CRichEditView::PreCreateWindow(cs);
}

void CBCGPMSOffice2007DemoView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();

	// Set the printing margins (720 twips = 1/2 inch).
	SetMargins(CRect(720, 720, 720, 720));

	CCharFormat cf;
	GetDefaultFont (cf);

	SetCharFormat (cf);
	SyncFont ();
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPMSOffice2007DemoView printing

void CBCGPMSOffice2007DemoView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CBCGPMSOffice2007DemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBCGPMSOffice2007DemoView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	if (m_bIsEndOfPrint)
	{
		pInfo->m_bContinuePrinting = FALSE;
		return;
	}

	CRichEditView::OnPrepareDC(pDC, pInfo);
}

void CBCGPMSOffice2007DemoView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used.
 	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }

   CRichEditView::OnDestroy();
}


/////////////////////////////////////////////////////////////////////////////
// CBCGPMSOffice2007DemoView diagnostics

#ifdef _DEBUG
void CBCGPMSOffice2007DemoView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CBCGPMSOffice2007DemoView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CBCGPMSOffice2007DemoDoc* CBCGPMSOffice2007DemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBCGPMSOffice2007DemoDoc)));
	return (CBCGPMSOffice2007DemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBCGPMSOffice2007DemoView message handlers

void CBCGPMSOffice2007DemoView::OnDummy (UINT id)
{
}

void CBCGPMSOffice2007DemoView::OnEditChange()
{
}

void CBCGPMSOffice2007DemoView::OnFontColor() 
{
	CBCGPRibbonBar* pRibbon = ((CMainFrame*) GetTopLevelFrame ())->GetRibbonBar ();
	ASSERT_VALID (pRibbon);

	CBCGPRibbonColorButton* pFontColorBtn = DYNAMIC_DOWNCAST (
		CBCGPRibbonColorButton, pRibbon->FindByID (ID_FONT_COLOR));

	if (pFontColorBtn != NULL)
	{
		COLORREF color = pFontColorBtn->GetColor ();
		CRichEditView::OnColorPick (color == -1 ? ::GetSysColor (COLOR_WINDOWTEXT) : color);
	}
}

void CBCGPMSOffice2007DemoView::OnFontname() 
{
	USES_CONVERSION;

	CBCGPRibbonBar* pRibbon = ((CMainFrame*) GetTopLevelFrame ())->GetRibbonBar ();
	ASSERT_VALID (pRibbon);

	CBCGPRibbonFontComboBox* pFontCombo = DYNAMIC_DOWNCAST (
		CBCGPRibbonFontComboBox, pRibbon->FindByID (ID_FONT_FONT));
	if (pFontCombo == NULL)
	{
		return;
	}

	CCharFormat cf;
	cf.szFaceName[0] = NULL;
	cf.dwMask = CFM_FACE | CFM_CHARSET;

	const CBCGPFontDesc* pDesc = pFontCombo->GetFontDesc ();
	ASSERT_VALID (pDesc);
	ASSERT(pDesc->m_strName.GetLength() < LF_FACESIZE);

#if _MSC_VER >= 1300
	lstrcpyn(cf.szFaceName, pDesc->m_strName, LF_FACESIZE);
#else
	lstrcpynA(cf.szFaceName,
		T2A((LPTSTR) (LPCTSTR) pDesc->m_strName), LF_FACESIZE);
#endif

	cf.bCharSet = pDesc->m_nCharSet;
	cf.bPitchAndFamily = pDesc->m_nPitchAndFamily;

	SetCharFormat (cf);
}

void CBCGPMSOffice2007DemoView::OnFontsize ()
{
	CBCGPRibbonBar* pRibbon = ((CMainFrame*) GetTopLevelFrame ())->GetRibbonBar ();
	ASSERT_VALID (pRibbon);

	CBCGPRibbonComboBox* pSizeCombo = DYNAMIC_DOWNCAST (
		CBCGPRibbonComboBox, pRibbon->FindByID (ID_FONT_FONTSIZE));
	if (pSizeCombo == NULL)
	{
		return;
	}

	int nSize = GetTwipSize (pSizeCombo->GetItem (-1));

	if (nSize == -2)
	{
		return;
	}

	if ((nSize >= 0 && nSize < 20) || nSize > 32760)
	{
		return;
	}

	if (nSize > 0)
	{
		CCharFormat cf;
		cf.dwMask = CFM_SIZE;
		cf.yHeight = nSize;

		SetCharFormat (cf);
	}
}

void CBCGPMSOffice2007DemoView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRichEditView::OnLButtonUp(nFlags, point);

	if (GetSelectedItem () != NULL || CBCGPPopupMenu::GetActiveMenu () != NULL ||
		GetRichEditCtrl().GetSelText().IsEmpty ())
	{
		return;
	}

	if (!theApp.m_bShowFloaty)
	{
		return;
	}

	CBCGPRibbonFloaty* pFloaty = new CBCGPRibbonFloaty;

	CList<UINT, UINT> lstCmds;

	lstCmds.AddTail (ID_FONT_FONT);
	lstCmds.AddTail (ID_FONT_GROWFONT);
	lstCmds.AddTail (ID_FONT_SHRINK);
	lstCmds.AddTail (IDR_STYLE_MENU);
	lstCmds.AddTail (ID_EDIT_COPYFORMAT);
	lstCmds.AddTail (ID_FONT_BOLD);
	lstCmds.AddTail (ID_FONT_ITALIC);
	lstCmds.AddTail (ID_PARA_CENTER);
	lstCmds.AddTail (ID_FONT_COLOR);
	lstCmds.AddTail (ID_PARA_DECREASEINDENT);
	lstCmds.AddTail (ID_PARA_INCREASEINDENT);
	lstCmds.AddTail (IDR_PARA_BULLETS_MENU);

	pFloaty->SetCommands (((CMainFrame*) GetTopLevelFrame ())->GetRibbonBar (),
		lstCmds);

	ClientToScreen (&point);
	pFloaty->Show (point.x, point.y);
}

void CBCGPMSOffice2007DemoView::OnClipboardDialog() 
{
	AfxMessageBox (_T("Show clipboard task pane here."));
}

void CBCGPMSOffice2007DemoView::OnFontDialog() 
{
	AfxMessageBox (_T("Show your Font dialog here."));
}

void CBCGPMSOffice2007DemoView::OnParagraphDialog() 
{
	AfxMessageBox (_T("Show your Paragraph dialog here."));
}

void CBCGPMSOffice2007DemoView::OnStyleDialog() 
{
	AfxMessageBox (_T("Show your Style dialog here."));
}

void CBCGPMSOffice2007DemoView::OnPagesetupDialog() 
{
	AfxMessageBox (_T("Show your page setup dialog here."));
}

BOOL CBCGPMSOffice2007DemoView::ShowContextMenu (CPoint point)
{
	if (DYNAMIC_DOWNCAST (CBCGPFrameWnd, GetParentFrame ()) == NULL)
	{
		// Maybe, server mode, show the regular menu!
		return FALSE;
	}

	CRichEditCntrItem* pItem = GetSelectedItem();
	if (pItem == NULL || !pItem->IsInPlaceActive())
	{
		theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
		return TRUE;
	}

	return FALSE;
}

void CBCGPMSOffice2007DemoView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	long nStartChar, nEndChar;
	GetRichEditCtrl().GetSel(nStartChar, nEndChar);
	if (nEndChar - nStartChar <= 1)
	{
		SendMessage (WM_LBUTTONDOWN, nFlags, MAKELPARAM (point.x, point.y));
	}

	CPoint ptScreen = point;
	ClientToScreen (&ptScreen);

	if (!ShowContextMenu (ptScreen))
	{
		Default ();
	}
}

void CBCGPMSOffice2007DemoView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (!ShowContextMenu (point))
	{
		Default ();
	}
}
void CBCGPMSOffice2007DemoView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_bIsEndOfPrint = FALSE;

	CRichEditView::OnBeginPrinting(pDC, pInfo);
}

void CBCGPMSOffice2007DemoView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_bIsEndOfPrint = TRUE;

	CRichEditView::OnEndPrinting(pDC, pInfo);
}

void CBCGPMSOffice2007DemoView::OnInsertPicture()
{
	InsertBitmap (IDB_PICTURE);
}

void CBCGPMSOffice2007DemoView::OnInsertChart()
{
	InsertBitmap (IDB_CHART);
}

void CBCGPMSOffice2007DemoView::OnInsertTable()
{
	InsertBitmap (IDB_TABLE);
}

BOOL CBCGPMSOffice2007DemoView::InsertBitmap (UINT uiBmpResID)
{
	CWaitCursor wait;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpResID))
	{
		return FALSE;
	}
	
	COleDataSource* pDataSrc = new COleDataSource;
	COleDataObject* pData = new COleDataObject;
	
	CBCGPMSOffice2007DemoCntrItem* pItem = NULL;
	
	TRY
	{
		STGMEDIUM stgm;
		stgm.hGlobal = bmp.GetSafeHandle();
		stgm.tymed = TYMED_GDI;
		stgm.pUnkForRelease = NULL;

		pDataSrc->CacheData (CF_BITMAP, &stgm);
		
		LPDATAOBJECT lpdata;

		if (FAILED (pDataSrc->m_xDataObject.QueryInterface (IID_IDataObject,
			(LPVOID FAR*) &lpdata)))
		{
			AfxThrowUserException();
		}
		
		pData->Attach (lpdata);
		
		CBCGPMSOffice2007DemoDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);

		pItem = new CBCGPMSOffice2007DemoCntrItem (NULL, pDoc);
		ASSERT_VALID(pItem);

		if (!pItem->CreateStaticFromData (pData))
		{
			AfxThrowUserException();
		}
		
		pItem->m_uiCategoryID = uiBmpResID;
		InsertItem (pItem);

		pDoc->UpdateAllViews (this);
	}
	CATCH(CException, e)
	{
		if (pItem != NULL)
		{
			ASSERT_VALID(pItem);
			pItem->Delete ();
		}

		delete pData;
		delete pDataSrc;

		return FALSE;
	}
	END_CATCH

	delete pData;
	delete pDataSrc;

	CMainFrame* pMainFrame = ((CMainFrame*) GetTopLevelFrame ());
	ASSERT_VALID (pMainFrame);

	pMainFrame->ActivateRibbonContextCategory (uiBmpResID);

	return TRUE;
}

void CBCGPMSOffice2007DemoView::OnSelChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	CRichEditView::OnSelChange(pNMHDR, pResult);

	CMainFrame* pMainFrame = ((CMainFrame*) GetTopLevelFrame ());
	ASSERT_VALID (pMainFrame);

	UINT uiCategoryID = 0;

	CBCGPMSOffice2007DemoCntrItem* pItem = (CBCGPMSOffice2007DemoCntrItem*) GetSelectedItem ();

	if (pItem != NULL)
	{
		uiCategoryID = pItem->m_uiCategoryID;
	}

	pMainFrame->SetRibbonContextCategory (uiCategoryID);

	SyncFont ();
}

void CBCGPMSOffice2007DemoView::OnUpdatePrintLayout(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck ();
}

void CBCGPMSOffice2007DemoView::OnZoom()
{
	MessageBox (_T("Zoom dialog box..."));
}

void CBCGPMSOffice2007DemoView::OnZoomSlider()
{
	CMainFrame* pMainFrame = ((CMainFrame*) GetTopLevelFrame ());
	ASSERT_VALID (pMainFrame);

	pMainFrame->OnZoomSlider();
}

void CBCGPMSOffice2007DemoView::OnIndentLeft()
{
}

void CBCGPMSOffice2007DemoView::OnIndentRight()
{
}

void CBCGPMSOffice2007DemoView::OnSpaceAfter()
{
}

void CBCGPMSOffice2007DemoView::OnSpaceBefore()
{
}

void CBCGPMSOffice2007DemoView::SyncFont ()
{
	USES_CONVERSION;

	CString strFontName;

	// get the current font from the view and update
	WPD_CHARFORMAT cf = GetCharFormatSelection ();

	CBCGPRibbonBar* pRibbon = ((CMainFrame*) GetTopLevelFrame ())->GetRibbonBar ();
	ASSERT_VALID (pRibbon);

	CBCGPRibbonFontComboBox* pFontCombo = DYNAMIC_DOWNCAST (
		CBCGPRibbonFontComboBox, pRibbon->FindByID (ID_FONT_FONT));

	if (pFontCombo != NULL && !pFontCombo->HasFocus ())
	{
		if ((cf.dwMask & (CFM_FACE|CFM_CHARSET)) == (CFM_FACE|CFM_CHARSET))
		{
#if _MSC_VER >= 1300
			strFontName = cf.szFaceName;
#else
			strFontName = A2T(cf.szFaceName);
#endif
			pFontCombo->SetFont (strFontName, DEFAULT_CHARSET, TRUE);
		}
		else
		{
			pFontCombo->SetText(_T(""));
		}
	}

	SetFocus ();
}

void CBCGPMSOffice2007DemoView::GetDefaultFont(CCharFormat& cf)
{
	USES_CONVERSION;
	
	CString strDefFont = _T("Tahoma");

	ASSERT(cf.cbSize == sizeof(WPD_CHARFORMAT));
	cf.dwMask = CFM_BOLD|CFM_ITALIC|CFM_UNDERLINE|CFM_STRIKEOUT|CFM_SIZE|
		CFM_COLOR|CFM_OFFSET|CFM_PROTECTED;
	cf.dwEffects = CFE_AUTOCOLOR;
	cf.yHeight = 200; //10pt
	cf.yOffset = 0;
	cf.crTextColor = RGB(0, 0, 0);
	cf.bCharSet = 0;
	cf.bPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	ASSERT(strDefFont.GetLength() < LF_FACESIZE);
#if _MSC_VER >= 1300
	lstrcpyn(cf.szFaceName, strDefFont, LF_FACESIZE);
#else
	lstrcpynA(cf.szFaceName, T2A((LPTSTR) (LPCTSTR) strDefFont), LF_FACESIZE);
#endif
	cf.dwMask |= CFM_FACE;
}


void CBCGPMSOffice2007DemoView::OnViewRuler()
{
	m_bViewRuler = !m_bViewRuler;
}

void CBCGPMSOffice2007DemoView::OnViewGridLines()
{
	m_bViewGridLines = !m_bViewGridLines;
}

void CBCGPMSOffice2007DemoView::OnViewProps()
{
	m_bViewProps = !m_bViewProps;
}

void CBCGPMSOffice2007DemoView::OnViewDocMap()
{
	m_bViewDocMap = !m_bViewDocMap;
}

void CBCGPMSOffice2007DemoView::OnViewThumb()
{
	m_bViewThumb = !m_bViewThumb;
}

void CBCGPMSOffice2007DemoView::OnViewActionBar()
{
	m_bViewActionBar = !m_bViewActionBar;
}

void CBCGPMSOffice2007DemoView::OnUpdateViewRuler(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (m_bViewRuler);
}

void CBCGPMSOffice2007DemoView::OnUpdateViewGridLines(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (m_bViewGridLines);
}

void CBCGPMSOffice2007DemoView::OnUpdateViewProps(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (m_bViewProps);
}

void CBCGPMSOffice2007DemoView::OnUpdateViewDocMap(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (m_bViewDocMap);
}

void CBCGPMSOffice2007DemoView::OnUpdateViewThumb(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (m_bViewThumb);
}

void CBCGPMSOffice2007DemoView::OnUpdateViewActionBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (m_bViewActionBar);
}

void CBCGPMSOffice2007DemoView::OnInsertBulletFromPalette() 
{
	CString str;
	str.Format (_T("Insert Bullet from Palette %d"), 
		CBCGPRibbonPaletteButton::GetLastSelectedItem (ID_INSERT_BULLET_FROM_PALETTE));

	MessageBox (str);
}

void CBCGPMSOffice2007DemoView::OnInsertNumberFromPalette() 
{
	CString str;
	str.Format (_T("Insert Number from Palette %d"),
		CBCGPRibbonPaletteButton::GetLastSelectedItem (ID_INSERT_NUMBER_FROM_PALETTE));

	MessageBox (str);
}

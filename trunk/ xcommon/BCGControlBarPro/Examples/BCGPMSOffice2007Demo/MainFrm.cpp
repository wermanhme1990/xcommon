// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "BCGPMSOffice2007Demo.h"
#include "OptionsPage.h"
#include "ResourcePage.h"
#include "MainFrm.h"

#ifndef _BCGCBPRO_STATIC_
#ifdef _AFXDLL
#define COMPILE_MULTIMON_STUBS
#endif // _AFXDLL
#endif

#include "multimon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int IdStartProgressTimer = 1;
const int IdShowProgressTimer = 2;

const DWORD idTabDeveloper = 101;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CBCGPFrameWnd)


BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_2003, ID_VIEW_APPLOOK_2007_4, OnAppLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_2003, ID_VIEW_APPLOOK_2007_4, OnUpdateAppLook)
	ON_COMMAND(ID_STATUSBAR_REFRESH, OnRefresh)
	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_REFRESH, OnUpdateRefresh)
	ON_COMMAND(ID_STATUSBAR_SPELL, OnSpell)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	ON_REGISTERED_MESSAGE(BCGM_ON_RIBBON_CUSTOMIZE, OnRibbonCustomize)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_rectFill.SetRectEmpty ();
	m_rectSizing.SetRectEmpty ();

	m_nProgressValue = -1;
	m_bInfiniteProgressMode = TRUE;
}

CMainFrame::~CMainFrame()
{
	for (POSITION pos = m_TooltipImages.GetStartPosition (); pos != NULL;)
	{
		CBCGPToolBarImages* pBmp = NULL;
		UINT uiID;

		m_TooltipImages.GetNextAssoc (pos, uiID, pBmp);
		delete pBmp;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	OnAppLook (theApp.m_nAppLook);

	// Load background patterns:
	m_Pat[0].Create (CBCGPControlRendererParams (IDB_VIEWPAT_1, RGB(255, 0, 255), 
		CRect (0, 0, 8, 8), CRect (0, 0, 0, 0)));
	m_Pat[1].Create (CBCGPControlRendererParams (IDB_VIEWPAT_2, -1, 
		CRect (0, 0, 499, 157), CRect (0, 0, 0, 0), CRect (480, 0, 0, 0), CRect (480, 0, 499, 157)));
	m_Pat[2].Create (CBCGPControlRendererParams (IDB_VIEWPAT_3, -1, 
		CRect (0, 0, 499, 157), CRect (0, 0, 0, 0), CRect (480, 0, 0, 0), CRect (480, 0, 499, 157)));
	m_Pat[3].Create (CBCGPControlRendererParams (IDB_VIEWPAT_4, -1, 
		CRect (0, 0, 499, 157), CRect (0, 0, 0, 0), CRect (480, 0, 0, 0), CRect (480, 0, 499, 157)));	

	if (!CreateRibbonBar ())
	{
		TRACE0("Failed to create ribbon bar\n");
		return -1;      // fail to create
	};

	if (!CreateStatusBar ())
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	EnableDocking (CBRS_ALIGN_ANY);

	m_wndRibbonBar.RecalcLayout ();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style |= WS_CLIPCHILDREN;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers



void CMainFrame::OnAppLook(UINT id)
{
	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_2003:
		CBCGPVisualManager::SetDefaultManager (
			RUNTIME_CLASS (CBCGPVisualManager2003));
		break;

	default:
#ifndef _BCGCBPRO_STATIC_
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_2007_1:
			CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_2007_2:
			CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_2007_3:
			CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_Aqua);
			break;

		case ID_VIEW_APPLOOK_2007_4:
			CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_Silver);
			break;
		}
#endif
		CBCGPVisualManager::SetDefaultManager (
			RUNTIME_CLASS (CBCGPVisualManager2007));
	}

	CRect rectWindow;
	GetWindowRect (rectWindow);

	SetWindowPos(&wndTop, 0, 0, rectWindow.Width (), rectWindow.Height (), 
		SWP_NOMOVE | SWP_NOZORDER);

	RecalcLayout ();

	if (m_MemBitmap.GetSafeHandle () != NULL)
	{
		m_MemBitmap.DeleteObject ();
	}

	RedrawWindow (NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt (_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateAppLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio (theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::CreateRibbonBar ()
{
	if (!m_wndRibbonBar.Create (this))
	{
		return -1;      // fail to create
	}

	//-------------------
	// Load panel images:
	//-------------------
	m_PanelImages.SetImageSize (CSize (16, 16));
	if (!m_PanelImages.Load (IDB_BUTTONS))
	{
		TRACE0("Failed to load panel images\n");
		return -1;
	}

	//-----------------
	// Add main button:
	//-----------------
	AddMainCategory ();

	//----------
	// Add tabs:
	//----------
	AddTab_Home ();
	AddTab_Insert ();
	AddTab_PageLayout ();
	AddTab_References ();
	AddTab_Mailings ();
	AddTab_View ();

	if (theApp.m_bShowDevTab)
	{
		AddTab_Developer ();
	}

	//------------------
	// Add context tabs:
	//------------------
	AddContextTab_Picture ();
	AddContextTab_Chart ();
	AddContextTab_Table ();

	//-----------------------------------
	// Add quick access toolbar commands:
	//-----------------------------------
	CBCGPRibbonQATDefaultState qatState;

	qatState.AddCommand (ID_FILE_NEW, FALSE);
	qatState.AddCommand (ID_FILE_OPEN, FALSE);
	qatState.AddCommand (ID_FILE_SAVE);
	qatState.AddCommand (ID_FILE_MAIL, FALSE);
	qatState.AddCommand (ID_FILE_PRINT_DIRECT);
	qatState.AddCommand (ID_FILE_PRINT_PREVIEW, FALSE);
	qatState.AddCommand (ID_EDIT_UNDO);
	qatState.AddCommand (ID_INSERT_DRAWTABLE, FALSE);

	m_wndRibbonBar.SetQuickAccessDefaultState (qatState);

	//----------------------------------------
	// Add elements to the right side of tabs:
	//----------------------------------------
	CBCGPRibbonButton* pVisualStyleButton = new CBCGPRibbonButton (-1, _T("Style"), -1, -1);

#ifndef _BCGCBPRO_STATIC_
	pVisualStyleButton->SetMenu (IDR_THEME_MENU, FALSE /* No default command */, TRUE /* Right align */);
#else
	pVisualStyleButton->SetMenu (IDR_THEME_MENU_STATIC, FALSE /* No default command */, TRUE /* Right align */);
#endif

	pVisualStyleButton->SetToolTipText (_T("Modify Visual Style"));
	pVisualStyleButton->SetDescription (_T("Choose one of the following looks:\r\nMS Office 2003 or MS Office 2007"));
	m_wndRibbonBar.AddToTabs (pVisualStyleButton);

	m_wndRibbonBar.AddToTabs (new CBCGPRibbonButton (ID_APP_ABOUT, _T(""), m_PanelImages.ExtractIcon (0)));

	m_wndRibbonBar.EnableToolTips (theApp.m_bShowToolTips, theApp.m_bShowToolTipDescr);

	return TRUE;
}
//***********************************************************************************
void CMainFrame::AddMainCategory ()
{
	m_MainButton.SetImage (IDB_MAIN);
	m_MainButton.SetToolTipText (_T("Main Button"));
	m_MainButton.SetDescription (_T("Click here to see everything you can do with your document, including saving, printing or sharing it with others"));
	m_MainButton.SetID (ID_MAIN_BUTTON);

	m_wndRibbonBar.SetMainButton (&m_MainButton, CSize (45, 45));

	CBCGPRibbonMainPanel* pMainPanel = m_wndRibbonBar.AddMainCategory (
		_T("Main Menu"), IDB_FILESMALL, IDB_FILELARGE);

	pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_NEW, _T("New"), 0, 0));
	pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_OPEN, _T("Open..."), 1, 1));

	pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_SAVE, _T("Save"), 2, 2));

	pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_SAVE_AS, _T("Save As..."), 3, 3));
	pMainPanel->Add (new CBCGPRibbonSeparator (TRUE));
	pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_FINISH, _T("Finish"), 4, 4));

	CBCGPRibbonButton* pBtnShare = new CBCGPRibbonButton (ID_FILE_SHARE, _T("Send"), 5, 5);

	pBtnShare->SetDefaultCommand (FALSE);

	pBtnShare->AddSubItem (new CBCGPRibbonLabel (_T("Send a copy of the document to other people")));
	pBtnShare->AddSubItem (new CBCGPRibbonButton (ID_FILE_MAIL, _T("EMail..."), 10, 10, TRUE));
	pBtnShare->AddSubItem (new CBCGPRibbonButton (ID_FILE_FAX, _T("Internet Fax..."), 11, 11, TRUE));
	pBtnShare->AddSubItem (new CBCGPRibbonButton (ID_FILE_DOCMANAGE_SERVER, _T("Document Management Server..."), 12, 12, TRUE));
	pBtnShare->AddSubItem (new CBCGPRibbonButton (ID_FILE_DOCWORKSPACE, _T("Create Document Workspace..."), 13, 13, TRUE));

	pMainPanel->Add (pBtnShare);

	CBCGPRibbonButton* pBtnPrint = new CBCGPRibbonButton (ID_FILE_PRINT, _T("Print"), 6, 6);

	pBtnPrint->AddSubItem (new CBCGPRibbonLabel (_T("Preview and print the document")));
	pBtnPrint->AddSubItem (new CBCGPRibbonButton (ID_FILE_PRINT_DIRECT, _T("Quick Print"), 7, 7, TRUE));
	pBtnPrint->AddSubItem (new CBCGPRibbonButton (ID_FILE_PRINT_PREVIEW, _T("Print Preview"), 8, 8, TRUE));

	pMainPanel->Add (pBtnPrint);

	pMainPanel->Add (new CBCGPRibbonSeparator (TRUE));
	pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_CLOSE, _T("Close"), 9, 9));

	pMainPanel->AddRecentFilesList (_T("Recent Documents"));

	pMainPanel->AddToBottom (new CBCGPRibbonMainPanelButton (ID_TOOLS_OPTIONS, _T("Options"), 14));
	pMainPanel->AddToBottom (new CBCGPRibbonMainPanelButton (ID_APP_EXIT, _T("Exit"), 15));
}
//***********************************************************************************
void CMainFrame::AddTab_Home ()
{
	//-----------------------------
	// Create a new category (tab):
	//-----------------------------
	CBCGPRibbonCategory* pCategory = m_wndRibbonBar.AddCategory (
		_T("Home"),
		IDB_WRITE,
		IDB_WRITELARGE);

	//--------------------------
	// Create "Clipboard" panel:
	//--------------------------
	CBCGPRibbonPanel* pPanelClipboard = pCategory->AddPanel (
		_T("Clipboard"), 
		m_PanelImages.ExtractIcon (27));

	CBCGPRibbonButton* pBtnPaste = new CBCGPRibbonButton (ID_EDIT_PASTE, _T("Paste"), 0, 0);
	pBtnPaste->SetMenu (IDR_PASTE_MENU, TRUE);
	pPanelClipboard->Add (pBtnPaste);

	pPanelClipboard->Add (new CBCGPRibbonButton (ID_EDIT_CUT, _T(""), 1));
	pPanelClipboard->Add (new CBCGPRibbonButton (ID_EDIT_COPY, _T(""), 2));
	pPanelClipboard->Add (new CBCGPRibbonButton (ID_EDIT_COPYFORMAT, _T(""), 3));

	//--------------------------------------------------
	// Add the launch button at the bottom of the panel:
	//--------------------------------------------------
	pPanelClipboard->EnableLaunchButton (ID_WRITE_CLIPBOARD, 0);

	//---------------------
	// Create "Font" panel:
	//---------------------
	CBCGPRibbonPanel* pPanelFont = pCategory->AddPanel (
		_T("Font"), 
		m_PanelImages.ExtractIcon (1));

	//-------------------------------------------------------------
	// Create a new group with 2 elements: font name and font size:
	//-------------------------------------------------------------
	CBCGPRibbonButtonsGroup* pFontGroup = new CBCGPRibbonButtonsGroup;

	CBCGPRibbonFontComboBox::m_bDrawUsingFont = TRUE;

	CBCGPRibbonFontComboBox* pFontCombo = new CBCGPRibbonFontComboBox (ID_FONT_FONT);
	pFontCombo->SelectItem (10);
	pFontGroup->AddButton (pFontCombo);

	CBCGPRibbonComboBox* pFontSizeCombo = new CBCGPRibbonComboBox (ID_FONT_FONTSIZE, TRUE, 20);
	pFontSizeCombo->AddItem (_T("8"));
	pFontSizeCombo->AddItem (_T("9"));
	pFontSizeCombo->AddItem (_T("10"));
	pFontSizeCombo->AddItem (_T("11"));
	pFontSizeCombo->AddItem (_T("12"));
	pFontSizeCombo->AddItem (_T("14"));
	pFontSizeCombo->AddItem (_T("16"));
	pFontSizeCombo->AddItem (_T("18"));
	pFontSizeCombo->AddItem (_T("20"));
	pFontSizeCombo->AddItem (_T("22"));
	pFontSizeCombo->AddItem (_T("24"));
	pFontSizeCombo->AddItem (_T("26"));
	pFontSizeCombo->AddItem (_T("28"));
	pFontSizeCombo->AddItem (_T("36"));
	pFontSizeCombo->AddItem (_T("48"));
	pFontSizeCombo->AddItem (_T("72"));
	pFontSizeCombo->SelectItem (3);
	pFontGroup->AddButton (pFontSizeCombo);

	pPanelFont->Add (pFontGroup);

	//--------------------------------------------------------
	// Add toolbar (all toolbar buttons will be automatically
	// converted to ribbon elements:
	//--------------------------------------------------------
	pPanelFont->AddToolBar (IDR_FONT);

	//----------------------------------------
	// Add menu to ID_FONT_CHANGECASE element:
	//----------------------------------------
	pPanelFont->SetElementMenu (ID_FONT_CHANGECASE, IDR_FONTCASE_MENU);

	//----------------------------------------------------------
	// Replace ID_FONT_COLOR and ID_FONT_TEXTHIGHLIGHT elements
	// by color pickers:
	//----------------------------------------------------------
	CBCGPRibbonColorButton* pFontColorBtn = new CBCGPRibbonColorButton ();
	pFontColorBtn->EnableAutomaticButton (_T("Automatic"), RGB (0, 0, 0));
	pFontColorBtn->EnableOtherButton (_T("Other..."));
	pFontColorBtn->EnableDocumentColors (_T("Document's Colors"));
	pFontColorBtn->SetColumns (10);
	pFontColorBtn->SetColor (-1 /* Automatic */);

	CPalette palText;
	CreateTextPalette (palText);
	pFontColorBtn->SetPalette (&palText);

	pPanelFont->ReplaceByID (ID_FONT_COLOR, pFontColorBtn);

	CBCGPRibbonColorButton* pFontColorHighlightBtn = new CBCGPRibbonColorButton ();
	pFontColorHighlightBtn->EnableOtherButton (_T("Other..."));
	pFontColorHighlightBtn->SetColor (RGB (255, 255, 87));

	pPanelFont->ReplaceByID (ID_FONT_TEXTHIGHLIGHT, pFontColorHighlightBtn);

	//--------------------------------------------------
	// Add the launch button at the bottom of the panel:
	//--------------------------------------------------
	pPanelFont->EnableLaunchButton (ID_WRITE_FONT);

	//--------------------------
	// Create "Paragraph" panel:
	//--------------------------
	CBCGPRibbonPanel* pPanelParagraph = pCategory->AddPanel (
		_T("Paragraph"), 
		m_PanelImages.ExtractIcon (2));

	pPanelParagraph->AddToolBar (IDR_PARAGRAPH);

	//------------------------------------
	// Attach palette to "Bullets" button:
	//------------------------------------
	CBCGPRibbonPaletteButton* pPaletteBullet = new CBCGPRibbonPaletteButton (ID_PARA_BULLETS, _T(""), -1, -1);

	pPanelParagraph->ReplaceByID (ID_PARA_BULLETS, pPaletteBullet);
	InitBulletPalette (pPaletteBullet);
	pPaletteBullet->EnableMenuResize ();

	//--------------------------------------
	// Attach palette to "Numbering" button:
	//--------------------------------------
	CBCGPRibbonPaletteButton* pPaletteNum = new CBCGPRibbonPaletteButton (ID_PARA_NUMBERING, _T(""), -1, -1);

	pPanelParagraph->ReplaceByID (ID_PARA_NUMBERING, pPaletteNum);
	InitNumberingPalette (pPaletteNum);
	pPaletteNum->EnableMenuResize ();

	pPanelParagraph->SetElementMenu (ID_PARA_NUMBERING, IDR_PARA_NUMBERING_MENU, TRUE);
	pPanelParagraph->SetElementMenu (ID_PARA_MULTILEVEL, IDR_PARA_MULTILIST_MENU);
	pPanelParagraph->SetElementMenu (ID_PARA_LINESPACING, IDR_PARA_LINESPACING_MENU);

	CBCGPRibbonColorButton* pParaColorBtn = new CBCGPRibbonColorButton ();
	pParaColorBtn->EnableOtherButton (_T("Other..."));
	pParaColorBtn->SetColor (RGB (240, 240, 240));

	pPanelParagraph->ReplaceByID (ID_PARA_SHADING, pParaColorBtn);

	pPanelParagraph->EnableLaunchButton (ID_WRITE_PARAGRAPH);

	//----------------------
	// Create "Style" panel:
	//----------------------
	CBCGPRibbonPanel* pPanelStyle = pCategory->AddPanel (
		_T("Style"), 
		m_PanelImages.ExtractIcon (4));

	CBCGPRibbonPaletteButton* pStyleBtn = new CBCGPRibbonPaletteButton 
		(ID_WRITE_QUICKSTYLES, _T("Quick Styles"), -1, 1, IDB_STYLES, 64);

	pStyleBtn->AddSubItem (
		new CBCGPRibbonButton (ID_WRITE_SAVESTYLE, _T("Save Selection as a New Quick Style..."), -1, -1));
	pStyleBtn->AddSubItem (
		new CBCGPRibbonButton (ID_WRITE_CLEARFORMATING, _T("Clear Formating"), 8, -1));
	pStyleBtn->AddSubItem (
		new CBCGPRibbonButton (ID_WRITE_APPLYSTYLES, _T("Apply Styles"), 9, -1));

	pStyleBtn->EnableMenuResize ();
	pPanelStyle->Add (pStyleBtn);

	CBCGPRibbonButton* pStyleChangeBtn = new CBCGPRibbonButton (ID_WRITE_CHANGESTYLES, _T("Change Styles"), -1, 2);
	pStyleChangeBtn->SetMenu (IDR_STYLECHANGE_MENU);
	pPanelStyle->Add (pStyleChangeBtn);

	pPanelStyle->EnableLaunchButton (ID_WRITE_STYLE);

	//---------------------
	// Create "Find" panel:
	//---------------------
	CBCGPRibbonPanel* pPanelFind = pCategory->AddPanel (
		_T("Find"), 
		m_PanelImages.ExtractIcon (3));

	pPanelFind->Add (new CBCGPRibbonButton (ID_EDIT_FIND, _T("Find"), -1, 3));
	pPanelFind->Add (new CBCGPRibbonButton (ID_EDIT_REPLACE, _T("R&eplace"), 5));
	pPanelFind->Add (new CBCGPRibbonButton (ID_WRITE_GOTO, _T("Go To"), 6));

	CBCGPRibbonButton* pFindSelectBtn = new CBCGPRibbonButton (ID_WRITE_SELECT, _T("Select"), 7);
	pFindSelectBtn->SetMenu (IDR_SELECT_MENU);
	pPanelFind->Add (pFindSelectBtn);

	//--------------------------
	// Add some hidden elements:
	//--------------------------
	pCategory->AddHidden (new CBCGPRibbonButton (ID_EDIT_UNDO, _T("Undo"), 10));
	pCategory->AddHidden (new CBCGPRibbonButton (ID_SELECTOBJECTS, _T("Select Objects"), 7));
}
//***********************************************************************************
void CMainFrame::AddTab_Insert ()
{
	CBCGPRibbonCategory* pCategory = m_wndRibbonBar.AddCategory (
		_T("Insert"),
		IDB_INSERT,
		IDB_INSERTLARGE);

	CBCGPRibbonPanel* pPanelInsertPages = pCategory->AddPanel (
		_T("Pages"), m_PanelImages.ExtractIcon (5));

	CBCGPRibbonButton* pInsertCoverPageBtn = new CBCGPRibbonButton (ID_INSERT_COVERPAGE, _T("Cover Page"), 1, 0);
	pInsertCoverPageBtn->SetMenu (IDR_INSERT_COVERPAGE_MENU);
	pPanelInsertPages->Add (pInsertCoverPageBtn);

	pPanelInsertPages->Add (new CBCGPRibbonButton (ID_INSERT_BLACKPAGE, _T("Blank Page"), 0, 1));
	pPanelInsertPages->Add (new CBCGPRibbonButton (ID_INSERT_PAGEBREAK, _T("Page Break"), 2, 2));

	CBCGPRibbonPanel* pPanelInsertTable = pCategory->AddPanel (
		_T("Table"));

	CBCGPRibbonButton* pTableMenuBtn = new CBCGPRibbonButton (ID_INSERT_TABLE, _T("Table"), -1, 3);
	pTableMenuBtn->SetMenu (IDR_TABLE_MENU);
	pPanelInsertTable->Add (pTableMenuBtn);

	CBCGPRibbonPanel* pPanelInsertIllustrations = pCategory->AddPanel (
		_T("Illustrations"), m_PanelImages.ExtractIcon (9));

	CBCGPRibbonButton* pInsertPictureBtn = new CBCGPRibbonButton (
		ID_INSERT_PICTURE, _T("Picture"), 22, 4);
	pPanelInsertIllustrations->Add (pInsertPictureBtn);

	CBCGPRibbonPaletteButton* pInsertShapesBtn = 
		new CBCGPRibbonPaletteButton (ID_INSERT_SHAPES, _T("Shapes"), 10, 7);

	pInsertShapesBtn->AddGroup (_T("Recently Used Shapes"), IDB_SHAPE1, 20);
	pInsertShapesBtn->AddGroup (_T("Lines"), IDB_SHAPE2, 20);
	pInsertShapesBtn->AddGroup (_T("Basic Shapes"), IDB_SHAPE3, 20);
	pInsertShapesBtn->AddGroup (_T("Block Arrows"), IDB_SHAPE4, 20);
	pInsertShapesBtn->AddGroup (_T("Flowchart"), IDB_SHAPE5, 20);
	pInsertShapesBtn->AddGroup (_T("Callouts"), IDB_SHAPE6, 20);
	pInsertShapesBtn->AddGroup (_T("Stars and Banners"), IDB_SHAPE7, 20);

	pInsertShapesBtn->AddSubItem (new CBCGPRibbonButton (ID_INSERT_SHAPESNEW, _T("New Drawing"), 10, -1));

	pPanelInsertIllustrations->Add (pInsertShapesBtn);

	pPanelInsertIllustrations->Add (new CBCGPRibbonButton (ID_INSERT_DIAGRAM, _T("SmartArt"), 8, 5));
	pPanelInsertIllustrations->Add (new CBCGPRibbonButton (ID_INSERT_CHART, _T("Chart"), 9, 6));
	
	CBCGPRibbonPanel* pPanelInsertLinks = pCategory->AddPanel (
		_T("Links"), m_PanelImages.ExtractIcon (6));

	pPanelInsertLinks->Add (new CBCGPRibbonButton (ID_INSERT_HYPERLINK, _T("Hyperlink"), 11, 8));
	pPanelInsertLinks->Add (new CBCGPRibbonButton (ID_INSERT_BOOKMARK, _T("Bookmark"), 12, 9));
	pPanelInsertLinks->Add (new CBCGPRibbonButton (ID_INSERT_REFERENCE, _T("Cross-Reference"), 13, 10));

	CBCGPRibbonPanel* pPanelHeaderFooter = pCategory->AddPanel (
		_T("Header & Footer"), m_PanelImages.ExtractIcon (7));

	CBCGPRibbonButton* pInsertHeaderBtn = new CBCGPRibbonButton (ID_INSERT_HEADER, _T("Header"), 14, 11);
	pInsertHeaderBtn->SetMenu (IDR_INSERT_HEADER_MENU);
	pPanelHeaderFooter->Add (pInsertHeaderBtn);

	CBCGPRibbonButton* pInsertFooterBtn = new CBCGPRibbonButton (ID_INSERT_FOOTER, _T("Footer"), 15, 12);
	pInsertFooterBtn->SetMenu (IDR_INSERT_FOOTER_MENU);
	pPanelHeaderFooter->Add (pInsertFooterBtn);

	CBCGPRibbonButton* pInsertPageNumbersBtn = new CBCGPRibbonButton (ID_INSERT_PAGENUMBER, _T("Page Number"), 16, 13);
	pInsertPageNumbersBtn->SetMenu (IDR_INSERT_PAGENUMBERS_MENU);
	pPanelHeaderFooter->Add (pInsertPageNumbersBtn);

	CBCGPRibbonPanel* pPanelText = pCategory->AddPanel (
		_T("Text"), m_PanelImages.ExtractIcon (8));

	pPanelText->Add (new CBCGPRibbonButton (ID_INSERT_TEXTBOX, _T("Text Box"), -1, 14));
	pPanelText->Add (new CBCGPRibbonButton (ID_INSERT_QUICKPARTS, _T("Quick Parts"), 17, 15));
	pPanelText->Add (new CBCGPRibbonButton (ID_INSERT_WORDART, _T("Word Art"), 18, 16));
	pPanelText->Add (new CBCGPRibbonButton (ID_INSERT_DROPCAP, _T("Drop Cap"), 19, 17));
	pPanelText->Add (new CBCGPRibbonButton (ID_INSERT_SIGNATURELINE, _T("Signature Line"), 20));
	pPanelText->Add (new CBCGPRibbonButton (ID_INSERT_DATETIME, _T("Date & Time"), 21));
	pPanelText->Add (new CBCGPRibbonButton (ID_INSERT_OBJECT, _T("Object"), 22));

	CBCGPRibbonPanel* pPanelSymbols = pCategory->AddPanel (
		_T("Symbols"), m_PanelImages.ExtractIcon (10));

	pPanelSymbols->Add (new CBCGPRibbonButton (ID_INSERT_EQUATION, _T("Equation"), 23, 18));
	pPanelSymbols->Add (new CBCGPRibbonButton (ID_INSERT_SYMBOL, _T("Symbol"), 24, 19));

	pCategory->AddHidden (new CBCGPRibbonButton (ID_INSERT_DRAWTABLE, _T("Draw Table"), 4));
	pCategory->AddHidden (new CBCGPRibbonButton (ID_INSERT_CONVERTTOTABLE, _T("Convert to Table"), 5));
	pCategory->AddHidden (new CBCGPRibbonButton (ID_INSERT_EXCELSPREADSHEET, _T("Excel Spreadsheet"), 6));
	pCategory->AddHidden (new CBCGPRibbonButton (ID_INSERT_QUICKTABLES, _T("Quick Tables"), 7));
}
//***********************************************************************************
void CMainFrame::AddTab_PageLayout ()
{
	CBCGPRibbonCategory* pCategory = m_wndRibbonBar.AddCategory (
		_T("Page Layout"),
		IDB_PAGELAYOUT,
		IDB_PAGELAYOUTLARGE);

	CBCGPRibbonPanel* pPanelPL_Themes = pCategory->AddPanel (
		_T("Themes"), m_PanelImages.ExtractIcon (15));

	CBCGPRibbonButton* pBtn = new CBCGPRibbonButton (-1, _T("Themes"), -1, 0);
	pBtn->SetMenu (IDR_PAGELAYOUT_THEMES);
	pPanelPL_Themes->Add (pBtn);

	pBtn = new CBCGPRibbonButton (-1, _T("Colors"), 0, -1);
	pBtn->SetMenu (IDR_PAGELAYOUT_COLORS);
	pPanelPL_Themes->Add (pBtn);

	pPanelPL_Themes->Add (new CBCGPRibbonButton (ID_PAGELAYOUT_THEMES_FONTS, _T("Fonts"), 1, -1));
	
	pBtn = new CBCGPRibbonButton (-1, _T("Effects"), 2, -1);
	pBtn->SetMenu (IDR_PAGELAYOUT_EFFECTS);
	pPanelPL_Themes->Add (pBtn);

	CBCGPRibbonPanel* pPanelPL_PageSetup = pCategory->AddPanel (
		_T("Page Setup"), m_PanelImages.ExtractIcon (16));

	pBtn = new CBCGPRibbonButton (-1, _T("Margins"), -1, 1);
	pBtn->SetMenu (IDR_PAGELAYOUT_MARGINS);
	pPanelPL_PageSetup->Add (pBtn);

	pBtn = new CBCGPRibbonButton (-1, _T("Orientation"), 4, 2);
	pBtn->SetMenu (IDR_PAGELAYOUT_ORIENTATION);
	pPanelPL_PageSetup->Add (pBtn);

	pPanelPL_PageSetup->Add (new CBCGPRibbonButton (ID_PAGELAYOUT_PAGESETUP_SIZE, _T("Size"), 5, 3));

	pBtn = new CBCGPRibbonButton (-1, _T("Columns"), 6, 4);
	pBtn->SetMenu (IDR_PAGELAYOUT_COLUMNS);
	pPanelPL_PageSetup->Add (pBtn);

	pPanelPL_PageSetup->Add (new CBCGPRibbonButton (ID_PAGELAYOUT_PAGESETUP_BREAKS, _T("Breaks"), 7, -1));
	pPanelPL_PageSetup->Add (new CBCGPRibbonButton (ID_PAGELAYOUT_PAGESETUP_LINENUMBERS, _T("Line Numbers"), 8, -1));
	pPanelPL_PageSetup->Add (new CBCGPRibbonButton (ID_PAGELAYOUT_PAGESETUP_HYPHENATION, _T("Hyphenation"), 9, -1));

	pPanelPL_PageSetup->EnableLaunchButton (ID_PARA_PAGESETUP);

	CBCGPRibbonPanel* pPanelPL_PageBackground = pCategory->AddPanel (
		_T("Page Background"), m_PanelImages.ExtractIcon (17));

	pPanelPL_PageBackground->Add (new CBCGPRibbonButton (ID_PAGELAYOUT_PAGEBACKGROUND_WATERMARK, _T("Watermark"), 10, 5));
	pPanelPL_PageBackground->Add (new CBCGPRibbonButton (ID_PAGELAYOUT_PAGEBACKGROUND_PAGECOLOR, _T("Page Color"), 11, 6));
	pPanelPL_PageBackground->Add (new CBCGPRibbonButton (ID_PAGELAYOUT_PAGEBACKGROUND_PAGEBORDERS, _T("Page Borders"), 12, 7));

	CBCGPRibbonPanel* pPanelPL_Paragraph = pCategory->AddPanel (
		_T("Paragraph"));

	pPanelPL_Paragraph->Add (new CBCGPRibbonLabel (_T("Indent")));

	CBCGPRibbonEdit* pEditIndentLeft = new CBCGPRibbonEdit (ID_PAGELAYOUT_INDENT_LEFT, 60, _T("Left:"), 13);
	pEditIndentLeft->EnableSpinButtons (0, 100);
	pEditIndentLeft->SetEditText (_T("0"));
	pPanelPL_Paragraph->Add (pEditIndentLeft);

	CBCGPRibbonEdit* pEditIndentRight = new CBCGPRibbonEdit (ID_PAGELAYOUT_INDENT_RIGHT, 60, _T("Right:"), 14);
	pEditIndentRight->EnableSpinButtons (0, 100);
	pEditIndentRight->SetEditText (_T("0"));
	pPanelPL_Paragraph->Add (pEditIndentRight);

	pPanelPL_Paragraph->AddSeparator ();

	pPanelPL_Paragraph->Add (new CBCGPRibbonLabel (_T("Spacing")));

	CBCGPRibbonEdit* pEditSpaceBefore = new CBCGPRibbonEdit (ID_PAGELAYOUT_SPACE_BEFORE, 60, _T("Before:"), 15);
	pEditSpaceBefore->EnableSpinButtons (0, 100);
	pEditSpaceBefore->SetEditText (_T("0"));
	pPanelPL_Paragraph->Add (pEditSpaceBefore);

	CBCGPRibbonEdit* pEditSpaceAfter = new CBCGPRibbonEdit (ID_PAGELAYOUT_SPACE_AFTER, 60, _T("After:"), 16);
	pEditSpaceAfter->EnableSpinButtons (0, 100);
	pEditSpaceAfter->SetEditText (_T("0"));
	pPanelPL_Paragraph->Add (pEditSpaceAfter);

	pPanelPL_Paragraph->SetJustifyColumns ();

	pPanelPL_PageBackground->Add (new CBCGPRibbonButton (ID_PAGELAYOUT_PAGEBACKGROUND_WATERMARK, _T("Watermark"), 10, 5));
	pPanelPL_PageBackground->Add (new CBCGPRibbonButton (ID_PAGELAYOUT_PAGEBACKGROUND_PAGECOLOR, _T("Page Color"), 11, 6));
	pPanelPL_PageBackground->Add (new CBCGPRibbonButton (ID_PAGELAYOUT_PAGEBACKGROUND_PAGEBORDERS, _T("Page Borders"), 12, 7));

	AddPanelArrange (pCategory, 19, 17, 8);
}
//***********************************************************************************
void CMainFrame::AddTab_References ()
{
	CBCGPRibbonCategory* pCategory = m_wndRibbonBar.AddCategory (
		_T("References"),
		IDB_REFERENCES,
		IDB_REFERENCESLARGE);

	CBCGPRibbonPanel* pPanelRef_TC = pCategory->AddPanel (
		_T("Table of Contents"), m_PanelImages.ExtractIcon (20));

	CBCGPRibbonButton* pBtn = new CBCGPRibbonButton (-1, _T("Table of Contents"), -1, 0);
	pPanelRef_TC->Add (pBtn);

	pBtn = new CBCGPRibbonButton (-1, _T("Add Text"), 0, -1);
	pBtn->SetMenu (IDR_REFERENCES_ADDTEXT);
	pPanelRef_TC->Add (pBtn);

	pPanelRef_TC->Add (new CBCGPRibbonButton (ID_REFERENCES_TC_UPDATETABLE, _T("Update Table"), 1, -1));

	CBCGPRibbonPanel* pPanelRef_Footnotes = pCategory->AddPanel (
		_T("Footnotes"), m_PanelImages.ExtractIcon (21));

	pPanelRef_Footnotes->Add (new CBCGPRibbonButton (ID_REFERENCES_FOOTNOTES_INSERTFOOTNOTE, _T("Insert Footnote"), -1, 1));
	pPanelRef_Footnotes->Add (new CBCGPRibbonButton (ID_REFERENCES_FOOTNOTES_INSERTENDNOTE, _T("Insert Endnote"), 2, -1));

	pBtn = new CBCGPRibbonButton (ID_NEXTFOOTNOTE, _T("Next Footnote"), 3, -1);
	pBtn->SetMenu (IDR_REFERENCES_NEXTFOOTNOTE, TRUE);
	pPanelRef_Footnotes->Add (pBtn);

	pPanelRef_Footnotes->Add (new CBCGPRibbonButton (ID_REFERENCES_FOOTNOTES_SHOWNOTES, _T("Show Notes"), 4, -1));

	CBCGPRibbonPanel* pPanelRef_CB = pCategory->AddPanel (
		_T("Citations & Bibliography"), m_PanelImages.ExtractIcon (22));

	pBtn = new CBCGPRibbonButton (-1, _T("Insert Citation"), -1, 2);
	pBtn->SetMenu (IDR_REFERENCES_INSERTCITATION);
	pPanelRef_CB->Add (pBtn);

	pPanelRef_CB->Add (new CBCGPRibbonButton (ID_REFERENCES_CB_MANAGESOURCES, _T("Manage Sources"), 5, -1));
	pPanelRef_CB->Add (new CBCGPRibbonButton (ID_REFERENCES_CB_INSERTBIBLIOGRAPHY, _T("Insert Bibliography"), 7, -1));

	CBCGPRibbonPanel* pPanelRef_Captions = pCategory->AddPanel (
		_T("Captions"), m_PanelImages.ExtractIcon (23));

	pPanelRef_Captions->Add (new CBCGPRibbonButton (ID_REFERENCES_CAPTIONS_INSERTCAPTION, _T("Insert Caption"), -1, 3));
	pPanelRef_Captions->Add (new CBCGPRibbonButton (ID_REFERENCES_CAPTIONS_INSERTTOF, _T("Insert Table of Figures"), 8, -1));
	pPanelRef_Captions->Add (new CBCGPRibbonButton (ID_REFERENCES_CAPTIONS_UPDATETABLE, _T("Update Table"), 9, -1));
	pPanelRef_Captions->Add (new CBCGPRibbonButton (ID_REFERENCES_CAPTIONS_CROSSREFERENCE, _T("Cross-reference"), 10, -1));

	CBCGPRibbonPanel* pPanelRef_Index = pCategory->AddPanel (
		_T("Index"), m_PanelImages.ExtractIcon (24));

	pPanelRef_Index->Add (new CBCGPRibbonButton (ID_REFERENCES_INDEX_MARKENTRY, _T("Mark Entry"), -1, 4));
	pPanelRef_Index->Add (new CBCGPRibbonButton (ID_REFERENCES_INDEX_INSERTINDEX, _T("Insert Index"), 8, -1));
	pPanelRef_Index->Add (new CBCGPRibbonButton (ID_REFERENCES_INDEX_UPDATEINDEX, _T("Update Index"), 9, -1));

	CBCGPRibbonPanel* pPanelRef_TA = pCategory->AddPanel (
		_T("Table of Authorities"), m_PanelImages.ExtractIcon (25));

	pPanelRef_TA->Add (new CBCGPRibbonButton (ID_REFERENCES_TA_MARKCITATION, _T("Mark Citation"), -1, 5));
	pPanelRef_TA->Add (new CBCGPRibbonButton (ID_REFERENCES_TA_INSERTTOA, _T("Insert Table of Authorities"), 11, -1));
	pPanelRef_TA->Add (new CBCGPRibbonButton (ID_REFERENCES_TA_UPDATETABLE, _T("Update Table"), 9, -1));
}
//***********************************************************************************
void CMainFrame::AddTab_Mailings ()
{
	CBCGPRibbonCategory* pCategory = m_wndRibbonBar.AddCategory (
		_T("Mailings"),
		IDB_MAILINGS,
		IDB_MAILINGSLARGE);

	CBCGPRibbonPanel* pPanelCreate = pCategory->AddPanel (
		_T("Create"), m_PanelImages.ExtractIcon (11));

	pPanelCreate->Add (new CBCGPRibbonButton (ID_MAILINGS_CREATE, _T("Create Envelopes"), 2, 0));
	pPanelCreate->Add (new CBCGPRibbonButton (ID_MAILINGS_CREATELABELS, _T("Create Labels"), 30, 1));

	CBCGPRibbonPanel* pPanelStartMerge = pCategory->AddPanel (
		_T("Start Mail Merge"), m_PanelImages.ExtractIcon (12));

	CBCGPRibbonButton* pMailMergeMenuBtn = new CBCGPRibbonButton (ID_MAILINGS_STARTMAILMERGE, _T("Start Mail Merge"), 6, 2);
	pMailMergeMenuBtn->SetMenu (IDR_MAIL_MERGE_MENU);
	pPanelStartMerge->Add (pMailMergeMenuBtn);

	CBCGPRibbonButton* pSelectRecipientMenuBtn = new CBCGPRibbonButton (ID_MAILINGS_SELECTRECIPIENTS, _T("Select Recipients"), 22, 3);
	pSelectRecipientMenuBtn->SetMenu (IDR_MAIL_RECIPIENTS_MENU);
	pPanelStartMerge->Add (pSelectRecipientMenuBtn);

	pPanelStartMerge->Add (new CBCGPRibbonButton (ID_MAILINGS_EDITRECIPIENTLIST, _T("Edit Recipient List"), 23, 4));

	CBCGPRibbonPanel* pPanelWriteFields = pCategory->AddPanel (
		_T("Write & Insert Fields"), m_PanelImages.ExtractIcon (13));

	pPanelWriteFields->Add (new CBCGPRibbonButton (ID_MAILINGS_HIGHLIGHT, _T("Highlight Merge Fields"), 24, 5));
	pPanelWriteFields->AddSeparator ();
	pPanelWriteFields->Add (new CBCGPRibbonButton (ID_MAILINGS_ADDRESSBLOCK, _T("Address Block"), 25, 6));
	pPanelWriteFields->Add (new CBCGPRibbonButton (ID_MAILINGS_GREATINGLINE, _T("Greating Line"), 26, 7));
	pPanelWriteFields->Add (new CBCGPRibbonButton (ID_MAILINGS_INSERTMERGEDFIELD, _T("Insert Merged Field"), 27, 8));
	pPanelWriteFields->Add (new CBCGPRibbonButton (ID_MAILINGS_RULES, _T("Rules"), 10));
	pPanelWriteFields->Add (new CBCGPRibbonButton (ID_MAILINGS_MATCHFIELDS, _T("Match Fields"), 11));
	pPanelWriteFields->Add (new CBCGPRibbonButton (ID_MAILINGS_UPDATELABELS, _T("Update Labels"), 12));

	CBCGPRibbonPanel* pPanelReview = pCategory->AddPanel (
		_T("Preview Results"), m_PanelImages.ExtractIcon (14));

	pPanelReview->Add (new CBCGPRibbonButton (ID_MAILINGS_VIEWMERGEDDATA, _T("Preview Results"), -1, 9));
	pPanelReview->AddSeparator ();

	CBCGPRibbonButtonsGroup* pButtonsListRecords = new CBCGPRibbonButtonsGroup;

	pButtonsListRecords->AddButton (new CBCGPRibbonButton (ID_MAILINGS_FIRSTRECORD, _T(""), 13));
	pButtonsListRecords->AddButton (new CBCGPRibbonButton (ID_MAILINGS_PREVIOUSRECORD, _T(""), 14));

	pButtonsListRecords->AddButton (new CBCGPRibbonEdit (ID_MAILINGS_GOTORECORD, 40));

	pButtonsListRecords->AddButton (new CBCGPRibbonButton (ID_MAILINGS_NEXTRECORD, _T(""), 15));
	pButtonsListRecords->AddButton (new CBCGPRibbonButton (ID_MAILINGS_LASTRECORD, _T(""), 16));

	pPanelReview->Add (pButtonsListRecords);

	pPanelReview->Add (new CBCGPRibbonButton (ID_MAILINGS_AUTOCHECHERRORS, _T("Auto Check for Errors"), 18));
	pPanelReview->Add (new CBCGPRibbonButton (ID_MAILINGS_FINDRECIPIENT, _T("Find Recipient"), 17));

	CBCGPRibbonPanel* pPanelFinish = pCategory->AddPanel (_T("Finish"));
	pPanelFinish->Add (new CBCGPRibbonButton (ID_MAILINGS_FINISH, _T("Finish && Merge"), -1, 10));

	pCategory->AddHidden (new CBCGPRibbonButton (ID_MAILINGS_TYPENEWLIST, _T("Type New List..."), 7));
	pCategory->AddHidden (new CBCGPRibbonButton (ID_MAILINGS_USEEXISTINGLIST, _T("Use Existing List..."), 8));
	pCategory->AddHidden (new CBCGPRibbonButton (ID_MAILINGS_SELECTFROMCONTACTS, _T("Select from Contacts..."), 9));

	pCategory->AddHidden (new CBCGPRibbonButton (ID_MAILINGS_LETTERS, _T("Letters"), 0));
	pCategory->AddHidden (new CBCGPRibbonButton (ID_MAILINGS_EMAILMESSAGES, _T("E-mail Messages"), 1));
	pCategory->AddHidden (new CBCGPRibbonButton (ID_MAILINGS_ENVELOPS, _T("Envelops..."), 2));
	pCategory->AddHidden (new CBCGPRibbonButton (ID_MAILINGS_LABELS, _T("Labels..."), 3));
	pCategory->AddHidden (new CBCGPRibbonButton (ID_MAILINGS_DIRECTORY, _T("Directory"), 4));
	pCategory->AddHidden (new CBCGPRibbonButton (ID_MAILINGS_NORMALDOCUMENT, _T("Normal Document"), 5));
	pCategory->AddHidden (new CBCGPRibbonButton (ID_MAILINGS_STEPBYSTEPMAILMERGEWIZARD, _T("Step by Step Mail Merge Wizard..."), 6));
}
//***********************************************************************************
void CMainFrame::AddTab_View ()
{
	CBCGPRibbonCategory* pCategory = m_wndRibbonBar.AddCategory (
		_T("View"),
		IDB_VIEW,
		IDB_VIEWLARGE);

	CBCGPRibbonPanel* pPanelDocViews = pCategory->AddPanel (
		_T("Document Views"));

	pPanelDocViews->Add (new CBCGPRibbonButton (ID_VIEW_PRINTLAYOUT, _T("Print Layout"), 7, 0));
	pPanelDocViews->Add (new CBCGPRibbonButton (ID_VIEW_FULLSCREEN, _T("Full Screen Reading"), 4, 1));
	pPanelDocViews->Add (new CBCGPRibbonButton (ID_VIEW_WEBLAYOUT, _T("Web Layout"), 12, 2));
	pPanelDocViews->Add (new CBCGPRibbonButton (ID_VIEW_OUTLINE, _T("Outline"), 6, 3));
	pPanelDocViews->Add (new CBCGPRibbonButton (ID_VIEW_DRAFT, _T("Draft"), 3, 4));

	CBCGPRibbonPanel* pPanelShowHide = pCategory->AddPanel (
		_T("Show/Hide"), m_PanelImages.ExtractIcon (26));

	pPanelShowHide->Add (new CBCGPRibbonCheckBox (ID_VIEW_RULER, _T("Ruler")));
	pPanelShowHide->Add (new CBCGPRibbonCheckBox (ID_VIEW_GRIDLINES, _T("Gridlines")));
	pPanelShowHide->Add (new CBCGPRibbonCheckBox (ID_VIEW_PROPERTIES, _T("Properties")));
	pPanelShowHide->Add (new CBCGPRibbonCheckBox (ID_VIEW_DOCUMENTMAP, _T("Document Map")));
	pPanelShowHide->Add (new CBCGPRibbonCheckBox (ID_VIEW_TRUMBNAILS, _T("Thumbnails")));
	pPanelShowHide->Add (new CBCGPRibbonCheckBox (ID_VIEW_ACTIONBAR, _T("Document Action Bar")));

	CBCGPRibbonPanel* pPanelZoom = pCategory->AddPanel (
		_T("Zoom"));

	pPanelZoom->Add (new CBCGPRibbonButton (ID_VIEW_ZOOM, _T("Zoom"), 13, 5));

	CBCGPRibbonPanel* pPanelWindow = pCategory->AddPanel (
		_T("Window"), m_PanelImages.ExtractIcon (26));

	pPanelWindow->Add (new CBCGPRibbonButton (ID_VIEW_NEWWINDOW, _T("New Window"), 5, 6));
	pPanelWindow->Add (new CBCGPRibbonButton (ID_VIEW_ARRANGEALL, _T("Arrange All"), 0, 7));
	pPanelWindow->Add (new CBCGPRibbonButton (ID_VIEW_SPLIT, _T("Split"), 9, 8));
	pPanelWindow->AddSeparator ();
	pPanelWindow->Add (new CBCGPRibbonButton (ID_VIEW_COMPARE, _T("Compare Side by Side"), 8));
	pPanelWindow->Add (new CBCGPRibbonButton (ID_VIEW_SYNCSCROLLING, _T("Synchronous Scrolling"), 10));
	pPanelWindow->Add (new CBCGPRibbonButton (ID_VIEW_RESETWINDOW, _T("Reset Window Position"), 8));
	pPanelWindow->AddSeparator ();
	pPanelWindow->Add (new CBCGPRibbonButton (ID_VIEW_SWITCHWINDOW, _T("Switch Windows"), -1, 9));
}
//***********************************************************************************
void CMainFrame::AddTab_Developer ()
{
	CBCGPRibbonCategory* pCategory = m_wndRibbonBar.AddCategory (
		_T("Developer"), 0, 0);

	pCategory->SetData (idTabDeveloper);
}
//***********************************************************************************
void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CBCGPFrameWnd::OnGetMinMaxInfo(lpMMI);

	ASSERT (lpMMI != NULL);

	lpMMI->ptMinTrackSize.y = GetSystemMetrics (SM_CXFIXEDFRAME) * 3 +
		GetSystemMetrics (SM_CYCAPTION);

	lpMMI->ptMinTrackSize.x = 
		GetSystemMetrics (SM_CYCAPTION) * 2 +
		GetSystemMetrics (SM_CXFIXEDFRAME) * 2 +
		GetSystemMetrics (SM_CXSIZE) * 3;
}

CBCGPToolBarImages* CMainFrame::GetTooltipImage (UINT uiID)
{
	if (!theApp.m_bShowToolTipDescr)
	{
		return NULL;
	}

	UINT uiResID = 0;

	switch (uiID)
	{
	case ID_MAIN_BUTTON:
		uiResID = IDB_TOOLTIP_MAIN;
		break;

	case ID_WRITE_CLIPBOARD:
		uiResID = IDB_TOOLTIP_CLIPBOARD;
		break;

	case ID_WRITE_FONT:
		uiResID = IDB_TOOLTIP_FONTDLG;
		break;

	case ID_WRITE_PARAGRAPH:
		uiResID = IDB_TOOLTIP_PARAGRAPH;
		break;
	}

	if (uiResID == 0)
	{
		return NULL;
	}

	CBCGPToolBarImages* pBmp = NULL;

	if (!m_TooltipImages.Lookup (uiID, pBmp))
	{
		pBmp = new CBCGPToolBarImages;
		pBmp->Load (uiResID);
		pBmp->SetSingleImage ();

		m_TooltipImages.SetAt (uiID, pBmp);
	}

	return pBmp;
}
//*****************************************************************************
void CMainFrame::AdjustClientArea ()
{
	if (m_pPrintPreviewFrame != NULL)
	{
		CBCGPFrameWnd::AdjustClientArea ();
		return;
	}

	CWnd* pChildWnd = GetDlgItem (AFX_IDW_PANE_FIRST);
	if (pChildWnd != NULL)
	{
		CRect rectClientAreaBounds = m_dockManager.GetClientAreaBounds ();

		rectClientAreaBounds.left += m_rectBorder.left;
		rectClientAreaBounds.top  += m_rectBorder.top;
		rectClientAreaBounds.right -= m_rectBorder.right;
		rectClientAreaBounds.bottom -= m_rectBorder.bottom;

		if (rectClientAreaBounds.Width () > 100 &&
			rectClientAreaBounds.Height () > 100)
		{
			m_rectFill = rectClientAreaBounds;
			rectClientAreaBounds.DeflateRect (40, 20, 40, 20);
		}
		else
		{
			m_rectFill.SetRectEmpty ();
		}

		pChildWnd->CalcWindowRect (rectClientAreaBounds);

		if (!pChildWnd->IsKindOf (RUNTIME_CLASS (CSplitterWnd)))
		{
			pChildWnd->ModifyStyle (0, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

			if (!m_rectFill.IsRectEmpty ())
			{
				pChildWnd->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);
				pChildWnd->ModifyStyle (0, WS_BORDER);
			}
		}
		else
		{
			pChildWnd->ModifyStyle (0, WS_CLIPSIBLINGS);
		}

		
		pChildWnd->SetWindowPos (&wndBottom, rectClientAreaBounds.left, 
										rectClientAreaBounds.top, 
										rectClientAreaBounds.Width (), 
										rectClientAreaBounds.Height (),
										SWP_NOACTIVATE);
	}

	if (!m_rectFill.IsRectEmpty ())
	{
		RedrawWindow (m_rectFill);
	}
}

void CMainFrame::OnPaint() 
{
	if (m_rectFill.IsRectEmpty ())
	{
		Default ();
		return;
	}

	CPaintDC dc (this); // device context for painting

	CRect rectChild;
	rectChild.SetRectEmpty ();

	CWnd* pChildWnd = GetDlgItem (AFX_IDW_PANE_FIRST);
	if (pChildWnd != NULL)
	{
		pChildWnd->GetWindowRect (rectChild);
		ScreenToClient (rectChild);
	}

	CRect rectScreen;

	MONITORINFO mi;
	mi.cbSize = sizeof (MONITORINFO);
	if (GetMonitorInfo (MonitorFromPoint (CPoint (0, 0), MONITOR_DEFAULTTONEAREST),
		&mi))
	{
		rectScreen = mi.rcWork;
	}
	else
	{
		::SystemParametersInfo (SPI_GETWORKAREA, 0, &rectScreen, 0);
	}

	if (m_MemBitmap.GetSafeHandle () != NULL)
	{
		BITMAP bmp;
		m_MemBitmap.GetBitmap (&bmp);

		if (bmp.bmWidth != rectScreen.Width () || bmp.bmHeight != rectScreen.Height ())
		{
			m_MemBitmap.DeleteObject ();
		}
	}

	if (m_MemBitmap.GetSafeHandle () == NULL)
	{
		CDC dcMem;
		dcMem.CreateCompatibleDC (&dc);

		m_MemBitmap.CreateCompatibleBitmap (&dc, rectScreen.Width (), rectScreen.Height ());
		
		CBitmap* pBmpOld = dcMem.SelectObject (&m_MemBitmap);

		CBCGPDrawManager dm (dcMem);

		rectScreen.OffsetRect (-rectScreen.TopLeft ());

		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_2003:
			::FillRect (dcMem.GetSafeHdc (), rectScreen, ::GetSysColorBrush (COLOR_APPWORKSPACE));
			break;

		case ID_VIEW_APPLOOK_2007_3:
			dm.FillGradient (rectScreen, RGB (114, 125, 152), RGB (178, 185, 202), TRUE);
			break;

		default:
			{
				const BOOL bLuna     = theApp.m_nAppLook == ID_VIEW_APPLOOK_2007_1;
				const BOOL bObsidian = theApp.m_nAppLook == ID_VIEW_APPLOOK_2007_2;

				CBCGPControlRenderer& pat = bLuna
					? m_Pat[1]
					: bObsidian 
						? m_Pat[2]
						: m_Pat[3];

				COLORREF clrF = bLuna
					? RGB (136, 170, 214)
					: bObsidian
						? RGB ( 71,  71,  71)
						: RGB (164, 170, 186);
				COLORREF clrM = bLuna
					? RGB ( 86, 125, 176)
					: bObsidian
						? RGB ( 56,  56,  56)
						: RGB (156, 160, 167);
				COLORREF clrL = bLuna
					? RGB (101, 145, 205)
					: bObsidian
						? RGB ( 11,  11,  11)
						: RGB (204, 207, 216);

				const CSize szPat = pat.GetParams ().m_rectImage.Size ();

				CRect rectPat = rectScreen;

				rectPat.bottom = rectPat.top + szPat.cy;

				CRect rectGradient = rectScreen;
				rectGradient.top += szPat.cy;

				if (rectGradient.Height () < 50 ||
					(rectGradient.Height () - 120) < 50)
				{
					rectGradient.bottom = rectGradient.top + 50;
				}
				else
				{
					rectGradient.bottom -= 120;
				}

				dm.FillGradient (rectGradient, clrM, clrF, TRUE);

				if (rectGradient.bottom < rectScreen.bottom)
				{
					rectGradient.top    = rectGradient.bottom;
					rectGradient.bottom = rectScreen.bottom;

					dm.FillGradient (rectGradient, clrL, clrM, TRUE);
				}

				pat.Draw (&dcMem, rectPat, 0);
			}
		}

		dcMem.SelectObject (pBmpOld);
	}

	CRect rectFill = m_rectFill;

	if (!m_rectSizing.IsRectEmpty ())
	{
		dc.FillRect (m_rectSizing, &globalData.brBarFace);
		rectFill.right += m_rectSizing.Width ();
	}

	dc.DrawState (rectFill.TopLeft (), rectFill.Size (), &m_MemBitmap, DSS_NORMAL);
}

BOOL CMainFrame::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	if (!CBCGPFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	CBCGPDockManager* pDockManager = GetDockManager ();

	if (pDockManager != NULL)
	{
		CWnd* pDockSite = pDockManager->GetDockSite ();

		if (pDockSite != NULL)
		{
			CRect rectWnd;
			pDockManager->GetDockSite ()->SetRedraw (FALSE);
			pDockManager->GetDockSite ()->GetWindowRect (rectWnd);
			pDockManager->GetDockSite ()->SetWindowPos (NULL, -1, -1, 
				rectWnd.Width () + 1, rectWnd.Height () + 1, 
				SWP_NOZORDER |  SWP_NOMOVE | SWP_NOACTIVATE);
			pDockManager->GetDockSite ()->SetWindowPos (NULL, -1, -1, 
				rectWnd.Width (), rectWnd.Height (), 
				SWP_NOZORDER |  SWP_NOMOVE  | SWP_NOACTIVATE);
			pDockManager->GetDockSite ()->SetRedraw (TRUE);
			pDockManager->GetDockSite ()->RedrawWindow (NULL, NULL, 
				RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
		}
	}

	// Force resize:
	CRect rectWindow;
	GetWindowRect (rectWindow);

	rectWindow.right++;
	MoveWindow (rectWindow);

	// Show progress bar after the short delay:
	SetTimer (IdStartProgressTimer, 500, NULL);

	// Start animation:
	CBCGPRibbonStatusBarPane* pPane = DYNAMIC_DOWNCAST (
		CBCGPRibbonStatusBarPane,
		m_wndStatusBar.FindElement (ID_STATUSBAR_SPELL));

	if (pPane != NULL)
	{
		pPane->StartAnimation (500 /* Frame delay*/, 5000 /* Duration */);
	}

	return TRUE;
}

void CMainFrame::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CRect rectOld;
	GetWindowRect (rectOld);

	CRect rectNew = pRect;

	if (rectNew.right > rectOld.right)
	{
		m_rectSizing = rectNew;
		m_rectSizing.left = rectOld.right;

		ScreenToClient (m_rectSizing);
	}
	else
	{
		m_rectSizing.SetRectEmpty ();
	}

	CBCGPFrameWnd::OnSizing(fwSide, pRect);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPFrameWnd::OnSize(nType, cx, cy);
	
	if (nType == SIZE_MAXIMIZED || nType == SIZE_RESTORED)
	{
		RedrawWindow (NULL, NULL,
			RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

void CMainFrame::CreateTextPalette (CPalette& pal)
{
	static COLORREF colors [] = 
	{
		RGB (255, 255, 255),
		RGB (0, 0, 0),
		RGB (238, 236, 225),
		RGB (31, 73, 125),
		RGB (79, 129, 189),
		RGB (192, 80, 77),
		RGB (155, 187, 89),
		RGB (128, 100, 162),
		RGB (75, 172, 198),
		RGB (245, 150, 70),

		RGB (242, 242, 242),
		RGB (127, 127, 127),
		RGB (214, 212, 202),
		RGB (210, 218, 229),
		RGB (217, 228, 240),
		RGB (244, 219, 218),
		RGB (234, 241, 221),
		RGB (229, 223, 235),
		RGB (216, 237, 242),
		RGB (255, 234, 218),

		RGB (215, 215, 215),
		RGB (89, 89, 89),
		RGB (177, 176, 167),
		RGB (161, 180, 201),
		RGB (179, 202, 226),
		RGB (233, 184, 182),
		RGB (213, 226, 188),
		RGB (203, 191, 215),
		RGB (176, 220, 231),
		RGB (255, 212, 181),

		RGB (190, 190, 190),
		RGB (65, 65, 65),
		RGB (118, 117, 112),
		RGB (115, 143, 175),
		RGB (143, 177, 213),
		RGB (222, 149, 147),
		RGB (192, 213, 155),
		RGB (177, 160, 197),
		RGB (137, 203, 218),
		RGB (255, 191, 145),

		RGB (163, 163, 163),
		RGB (42, 42, 42),
		RGB (61, 61, 59),
		RGB (20, 57, 92),
		RGB (54, 96, 139),
		RGB (149, 63, 60),
		RGB (114, 139, 71),
		RGB (97, 76, 119),
		RGB (41, 128, 146),
		RGB (190, 112, 59),

		RGB (126, 126, 126),
		RGB (20, 20, 20),
		RGB (29, 29, 28),
		RGB (17, 40, 64),
		RGB (38, 66, 94),
		RGB (100, 44, 43),
		RGB (77, 93, 49),
		RGB (67, 53, 81),
		RGB (31, 86, 99),
		RGB (126, 77, 42),

		RGB (200, 15, 18),
		RGB (255, 20, 24),
		RGB (255, 191, 40),
		RGB (255, 255, 49),
		RGB (138, 207, 87),
		RGB (0, 175, 84),
		RGB (0, 174, 238),
		RGB (0, 111, 189),
		RGB (0, 36, 95),
		RGB (114, 50, 157)
	};

	int nNumColours = sizeof (colors) / sizeof (COLORREF);

	struct 
	{
		LOGPALETTE    LogPalette;
		PALETTEENTRY  PalEntry [255];
	}
	palette;

	LOGPALETTE* pLogPalette = (LOGPALETTE*) &palette;
	pLogPalette->palVersion    = 0x300;
	pLogPalette->palNumEntries = (WORD) nNumColours; 

	for (int i = 0; i < nNumColours; i++)
	{
		pLogPalette->palPalEntry[i].peRed   = GetRValue(colors[i]);
		pLogPalette->palPalEntry[i].peGreen = GetGValue(colors[i]);
		pLogPalette->palPalEntry[i].peBlue  = GetBValue(colors[i]);
		pLogPalette->palPalEntry[i].peFlags = 0;
	}

	pal.CreatePalette (pLogPalette);
}

void CMainFrame::ActivateRibbonContextCategory (UINT uiCategoryID)
{
	if (m_wndRibbonBar.GetHideFlags () == 0)
	{
		m_wndRibbonBar.ActivateContextCategory (uiCategoryID);
	}
}

void CMainFrame::SetRibbonContextCategory (UINT uiCategoryID)
{
	BOOL bRecalc = m_wndRibbonBar.HideAllContextCategories ();

	if (uiCategoryID != 0)
	{
		m_wndRibbonBar.ShowContextCategories (uiCategoryID);
		bRecalc = TRUE;
	}

	if (bRecalc)
	{
		m_wndRibbonBar.RecalcLayout ();
		m_wndRibbonBar.RedrawWindow ();

		SendMessage (WM_NCPAINT, 0, 0);
	}
}

CBCGPRibbonPanel* CMainFrame::AddPanelArrange (CBCGPRibbonCategory* pCategory, UINT imageP, UINT imageS, UINT imageL)
{
	ASSERT_VALID(pCategory);

	CBCGPRibbonPanel* pPanelPL_Arrange = pCategory->AddPanel (
		_T("Arrange"), m_PanelImages.ExtractIcon (imageP));

	pPanelPL_Arrange->Add (new CBCGPRibbonButton (ID_PAGELAYOUT_ARRANGE_POSITION, _T("Position"), -1, imageL));

	CBCGPRibbonButton* pBtn = new CBCGPRibbonButton (ID_BRINGTOFRONT, _T("Bring to Front"), imageS, -1);
	pBtn->SetMenu (IDR_PAGELAYOUT_BRINGTOFRONT, TRUE);
	pPanelPL_Arrange->Add (pBtn);

	pBtn = new CBCGPRibbonButton (ID_SENDTOBACK, _T("Send to Back"), imageS + 2, -1);
	pBtn->SetMenu (IDR_PAGELAYOUT_SENDTOBACK, TRUE);
	pPanelPL_Arrange->Add (pBtn);

	pBtn = new CBCGPRibbonButton (-1, _T("Text Wrapping"), imageS + 4, -1);
	pBtn->SetMenu (IDR_PAGELAYOUT_TEXTWRAPPING, FALSE);
	pPanelPL_Arrange->Add (pBtn);

	pPanelPL_Arrange->Add (new CBCGPRibbonButton (ID_PAGELAYOUT_ARRANGE_ALIGN, _T("Align"), imageS + 1, -1));
	pPanelPL_Arrange->Add (new CBCGPRibbonButton (ID_PAGELAYOUT_ARRANGE_GROUP, _T("Group"), imageS + 3, -1));
	pPanelPL_Arrange->Add (new CBCGPRibbonButton (ID_PAGELAYOUT_ARRANGE_ROTATE, _T("Rotate"), imageS + 5, -1));

	return pPanelPL_Arrange;
}

void CMainFrame::AddContextTab_Picture ()
{
	CBCGPRibbonCategory* pCategory = m_wndRibbonBar.AddContextCategory (_T("Format"),
		_T("Picture tools"), IDB_PICTURE, BCGPCategoryColor_Red,
		IDB_PICTURE_FORMAT, IDB_PICTURE_FORMATLARGE);

	CBCGPRibbonPanel* pPanelTools = pCategory->AddPanel (
		_T("Picture Tools"), m_PanelImages.ExtractIcon (20));

	pPanelTools->Add (new CBCGPRibbonButton (ID_PICTURE_FORMAT_TOOLS_BRIGHTNESS, _T("Brightness"), 0, -1));
	pPanelTools->Add (new CBCGPRibbonButton (ID_PICTURE_FORMAT_TOOLS_CONTRAST, _T("Contrast"), 1, -1));
	pPanelTools->Add (new CBCGPRibbonButton (ID_PICTURE_FORMAT_TOOLS_RECOLOR, _T("Recolor"), 2, -1));
	pPanelTools->Add (new CBCGPRibbonButton (ID_PICTURE_FORMAT_TOOLS_COMPRESS, _T("Compress Pictures"), 3, -1));
	pPanelTools->Add (new CBCGPRibbonButton (ID_PICTURE_FORMAT_TOOLS_CHANGE, _T("Change Picture"), 4, -1));
	pPanelTools->Add (new CBCGPRibbonButton (ID_PICTURE_FORMAT_TOOLS_RESET, _T("Reset Picture"), 5, -1));

	AddPanelArrange (pCategory, 19, 6, 0);
}

void CMainFrame::AddContextTab_Chart ()
{
	CBCGPRibbonCategory* pCategory = m_wndRibbonBar.AddContextCategory (_T("Design"),
		_T("Chart tools"), IDB_CHART, BCGPCategoryColor_Green,
		IDB_CHART_DESIGN, IDB_CHART_DESIGNLARGE);

	CBCGPRibbonPanel* pPanelDesign_Type = pCategory->AddPanel (
		_T("Type"), m_PanelImages.ExtractIcon (20));

	pPanelDesign_Type->Add (new CBCGPRibbonButton (ID_CHART_DESIGN_TYPE_CHANGE, _T("Change Chart Type"), -1, 0));
	pPanelDesign_Type->Add (new CBCGPRibbonButton (ID_CHART_DESIGN_TYPE_SAVETEMPLATE, _T("Save Template"), -1, 1));

	CBCGPRibbonPanel* pPanelDesign_Data = pCategory->AddPanel (
		_T("Data"), m_PanelImages.ExtractIcon (20));

	pPanelDesign_Data->Add (new CBCGPRibbonButton (ID_CHART_DESIGN_DATA_SWITCH, _T("Switch Row/Column"), -1, 2));
	pPanelDesign_Data->Add (new CBCGPRibbonButton (ID_CHART_DESIGN_DATA_EDIT, _T("Edit Data Source"), -1, 3));
	pPanelDesign_Data->Add (new CBCGPRibbonButton (ID_CHART_DESIGN_DATA_SHOW, _T("Show Data"), -1, 4));

	
	CBCGPRibbonCategory* pCategory2 = m_wndRibbonBar.AddContextCategory (_T("Layout"),
		_T("Chart tools"), IDB_CHART, BCGPCategoryColor_Green,
		IDB_CHART_LAYOUT, IDB_CHART_LAYOUTLARGE);

	CBCGPRibbonPanel* pPanelLayout_Shapes = pCategory2->AddPanel (
		_T("Shapes"), m_PanelImages.ExtractIcon (20));

	pPanelLayout_Shapes->Add (new CBCGPRibbonButton (ID_CHART_LAYOUT_SHAPES_PICTURE, _T("Picture"), 0, 0));
	pPanelLayout_Shapes->Add (new CBCGPRibbonButton (ID_CHART_LAYOUT_SHAPES_SHAPES, _T("Shapes"), 1, 1));
	pPanelLayout_Shapes->Add (new CBCGPRibbonButton (ID_CHART_LAYOUT_SHAPES_TEXTBOX, _T("Draw Text Box"), 2, 2));

	CBCGPRibbonPanel* pPanelLayout_Labels = pCategory2->AddPanel (
		_T("Labels"), m_PanelImages.ExtractIcon (20));

	pPanelLayout_Labels->Add (new CBCGPRibbonButton (ID_CHART_LAYOUT_LABELS_TITLE, _T("Chart Title"), -1, 3));
	pPanelLayout_Labels->Add (new CBCGPRibbonButton (ID_CHART_LAYOUT_LABELS_AXIS, _T("Axis Titles"), -1, 4));
	pPanelLayout_Labels->Add (new CBCGPRibbonButton (ID_CHART_LAYOUT_LABELS_LEGEND, _T("Legend"), 3, 5));
	pPanelLayout_Labels->Add (new CBCGPRibbonButton (ID_CHART_LAYOUT_LABELS_DATALABELS, _T("Data Labels"), 4, 6));
	pPanelLayout_Labels->Add (new CBCGPRibbonButton (ID_CHART_LAYOUT_LABELS_DATATABLE, _T("Data Table"), 5, 7));

	CBCGPRibbonPanel* pPanelLayout_Axes = pCategory2->AddPanel (
		_T("Axes"), m_PanelImages.ExtractIcon (20));

	pPanelLayout_Axes->Add (new CBCGPRibbonButton (ID_CHART_LAYOUT_AXES_AXES, _T("Axes"), -1, 8));
	pPanelLayout_Axes->Add (new CBCGPRibbonButton (ID_CHART_LAYOUT_AXES_GRIDLINES, _T("Gridlines "), -1, 9));


	CBCGPRibbonCategory* pCategory3 = m_wndRibbonBar.AddContextCategory (_T("Format"),
		_T("Chart tools"), IDB_CHART, BCGPCategoryColor_Green,
		IDB_CHART_FORMAT, IDB_CHART_FORMATLARGE);

	CBCGPRibbonPanel* pPanelFormat_Sel = pCategory3->AddPanel (
		_T("Current Selection"), m_PanelImages.ExtractIcon (20));

	pPanelFormat_Sel->Add (new CBCGPRibbonButton (ID_CHART_FORMAT_CURSEL_FORMAT, _T("Format Selection"), 0, -1));
	pPanelFormat_Sel->Add (new CBCGPRibbonButton (ID_CHART_FORMAT_CURSEL_RESET, _T("Reset to Match Style"), 1, -1));

	CBCGPRibbonPanel* pPanelFormat_Shape = pCategory3->AddPanel (
		_T("Shape Styles"), m_PanelImages.ExtractIcon (20));

	pPanelFormat_Shape->Add (new CBCGPRibbonButton (ID_CHART_FORMAT_SHAPES_FILL, _T("Shape Fill"), 2, -1));
	pPanelFormat_Shape->Add (new CBCGPRibbonButton (ID_CHART_FORMAT_SHAPES_OUTLINE, _T("Shape Outline"), 3, -1));
	pPanelFormat_Shape->Add (new CBCGPRibbonButton (ID_CHART_FORMAT_SHAPES_EFFECTS, _T("Shape Effects"), 4, -1));

	AddPanelArrange (pCategory3, 19, 5, 0);
}

void CMainFrame::AddContextTab_Table ()
{
	CBCGPRibbonCategory* pCategory = m_wndRibbonBar.AddContextCategory (_T("Design"),
		_T("Table tools"), IDB_TABLE, BCGPCategoryColor_Yellow,
		IDB_INSERT, IDB_INSERTLARGE);

	CBCGPRibbonCategory* pCategory2 = m_wndRibbonBar.AddContextCategory (_T("Layout"),
		_T("Table tools"), IDB_TABLE, BCGPCategoryColor_Yellow,
		IDB_INSERT, IDB_INSERTLARGE);
}

BOOL CMainFrame::CreateStatusBar ()
{
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return FALSE;
	}

	m_wndStatusBar.AddElement (new CBCGPRibbonStatusBarPane (
		ID_STATUSBAR_PAGE, _T("Page 1")), _T("Formatted Page Number"));

	m_wndStatusBar.AddElement (new CBCGPRibbonStatusBarPane (
		ID_STATUSBAR_LINE, _T("Line 1")), _T("Line Number"));

	m_wndStatusBar.AddElement (new CBCGPRibbonStatusBarPane (
		ID_STATUSBAR_COLUMN, _T("Column 1")), _T("Column"));

	m_wndStatusBar.AddSeparator ();

	HICON hIconSpell = (HICON) ::LoadImage (
			AfxGetResourceHandle (),
			MAKEINTRESOURCE (IDI_SPELL),
			IMAGE_ICON,
			::GetSystemMetrics (SM_CXSMICON),
			::GetSystemMetrics (SM_CYSMICON),
			LR_SHARED);

	m_wndStatusBar.AddElement (new CBCGPRibbonStatusBarPane (
		ID_STATUSBAR_SPELL, _T(""), (UINT) IDB_SPELL, 
		16, RGB (192, 192, 192), hIconSpell),
		_T("Spelling and Grammar Check"));

	m_wndStatusBar.AddSeparator ();

	HICON hIconRefresh = (HICON) ::LoadImage (
			AfxGetResourceHandle (),
			MAKEINTRESOURCE (IDI_REFRESH),
			IMAGE_ICON,
			::GetSystemMetrics (SM_CXSMICON),
			::GetSystemMetrics (SM_CYSMICON),
			LR_SHARED);

	m_wndStatusBar.AddElement (new CBCGPRibbonStatusBarPane (
		ID_STATUSBAR_REFRESH, _T("Refresh"), FALSE, hIconRefresh), 
		_T("Refresh"));

	CBCGPRibbonButtonsGroup* pSBGroup = new CBCGPRibbonButtonsGroup;

	CBCGPToolBarImages images1;
	images1.SetImageSize (CSize (14, 14));
	images1.Load (IDB_LAYOUT);

	pSBGroup->SetImages (&images1, NULL, NULL);
	pSBGroup->SetID (ID_GROUP_SHORTCUTS);

	pSBGroup->AddButton (new CBCGPRibbonButton (ID_PRINT_LAYOUT, _T(""), 0));
	pSBGroup->AddButton (new CBCGPRibbonButton (ID_FULL_SCREEN_READING, _T(""), 1));
	pSBGroup->AddButton (new CBCGPRibbonButton (ID_WEB_LAYOUT, _T(""), 2));
	pSBGroup->AddButton (new CBCGPRibbonButton (ID_MASTER_DOCUMENT_TOOLS, _T(""), 3));
	pSBGroup->AddButton (new CBCGPRibbonButton (ID_DRAFT, _T(""), 4));

	m_wndStatusBar.AddExtendedElement (pSBGroup, _T("View Shortcuts"));

	m_wndStatusBar.AddExtendedElement (new CBCGPRibbonStatusBarPane (
		ID_STATUSBAR_ZOOM, _T("100%"), FALSE, NULL, _T("1000%")), _T("Zoom"));

	CBCGPRibbonSlider* pSlider = new CBCGPRibbonSlider (ID_STATUSBAR_ZOOM_SLIDER);
	pSlider->SetZoomButtons ();
	pSlider->SetRange (0, 200);
	pSlider->SetPos (100);

	m_wndStatusBar.AddExtendedElement (pSlider, _T("Zoom Slider"));

	return TRUE;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent) 
{
	if (nIDEvent == IdStartProgressTimer)
	{
		ShowProgress ();
		KillTimer (IdStartProgressTimer);
	}

	if (nIDEvent == IdShowProgressTimer)
	{
		m_nProgressValue++;

		if (m_nProgressValue > 100)
		{
			ShowProgress (FALSE);
		}
		else
		{
			CBCGPRibbonProgressBar* pProgress = DYNAMIC_DOWNCAST (
				CBCGPRibbonProgressBar,
				m_wndStatusBar.FindElement (ID_STATUSBAR_PROGRESS));
			ASSERT_VALID (pProgress);

			pProgress->SetPos (m_nProgressValue, TRUE);
		}
	}
}

void CMainFrame::ShowProgress (BOOL bShow)
{
	if (bShow)
	{
		int cxFree = m_wndStatusBar.GetSpace ();
		if (cxFree < 20)
		{
			// Not enough space for progress bar
			return;
		}

		int cxProgress = min (cxFree, 150);

		CBCGPRibbonProgressBar* pProgressBar = 
			new CBCGPRibbonProgressBar (ID_STATUSBAR_PROGRESS, cxProgress);

		pProgressBar->SetInfiniteMode (m_bInfiniteProgressMode);

		m_wndStatusBar.AddDynamicElement (pProgressBar);

		m_nProgressValue = 0;

		SetTimer (IdShowProgressTimer, 10, NULL);
	}
	else
	{
		KillTimer (IdShowProgressTimer);
		m_wndStatusBar.RemoveElement (ID_STATUSBAR_PROGRESS);
		m_nProgressValue = -1;
	}

	m_wndStatusBar.RecalcLayout ();
	m_wndStatusBar.RedrawWindow ();

	CBCGPPopupMenu::UpdateAllShadows ();
}

void CMainFrame::OnZoomSlider()
{
	CBCGPRibbonSlider* pSlider = DYNAMIC_DOWNCAST (
		CBCGPRibbonSlider,
		m_wndStatusBar.FindElement (ID_STATUSBAR_ZOOM_SLIDER));
	ASSERT_VALID (pSlider);

	int nPos = pSlider->GetPos ();

	CBCGPRibbonStatusBarPane* pZoom = DYNAMIC_DOWNCAST (
		CBCGPRibbonStatusBarPane,
		m_wndStatusBar.FindElement (ID_STATUSBAR_ZOOM));
	ASSERT_VALID (pZoom);

	CString strZoom;
	strZoom.Format (_T("%d%%"), nPos);

	pZoom->SetText (strZoom);
	pZoom->Redraw ();
}

void CMainFrame::OnRefresh()
{
	m_bInfiniteProgressMode = FALSE;
	ShowProgress ();
}

void CMainFrame::OnUpdateRefresh(CCmdUI* pCmdUI)
{
	pCmdUI->Enable (m_nProgressValue < 0);
}

void CMainFrame::OnSpell()
{
	CBCGPRibbonStatusBarPane* pPane = DYNAMIC_DOWNCAST (
		CBCGPRibbonStatusBarPane,
		m_wndStatusBar.FindElement (ID_STATUSBAR_SPELL));

	if (pPane == NULL)
	{
		return;
	}

	if (pPane->IsAnimation ())
	{
		pPane->StopAnimation ();
	}
	else
	{
		pPane->StartAnimation (	500		/* 1/2 sec, Frame delay */,
								10000	/* 10 sec, animation duration */);
	}
}

BOOL CMainFrame::OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup)
{
    CBCGPFrameWnd::OnShowPopupMenu (pMenuPopup);

	if (pMenuPopup == NULL)
	{
		return TRUE;
	}

	CBCGPPopupMenuBar* pMenuBar = pMenuPopup->GetMenuBar ();
	ASSERT_VALID (pMenuBar);

	int nBulletIndex = pMenuBar->CommandToIndex (ID_PARA_BULLETS);

    if (nBulletIndex >= 0)
    {
		CBCGPToolbarButton* pExButton = pMenuBar->GetButton (nBulletIndex);
		ASSERT_VALID (pExButton);

		CBCGPRibbonPaletteMenuButton paletteBullet (
			pExButton->m_nID, pExButton->GetImage (), pExButton->m_strText);

		InitBulletPalette (&paletteBullet.GetPalette ());

		pMenuBar->ReplaceButton (ID_PARA_BULLETS, paletteBullet);
	}

	int nNumIndex = pMenuBar->CommandToIndex (ID_PARA_NUMBERING);

    if (nNumIndex >= 0)
    {
		CBCGPToolbarButton* pExButton = pMenuBar->GetButton (nNumIndex);
		ASSERT_VALID (pExButton);

		CBCGPRibbonPaletteMenuButton paletteNum (
			pExButton->m_nID, pExButton->GetImage (), pExButton->m_strText);

		InitNumberingPalette (&paletteNum.GetPalette ());

		pMenuBar->ReplaceButton (ID_PARA_NUMBERING, paletteNum);
	}

	return TRUE;
}

void CMainFrame::InitBulletPalette (CBCGPRibbonPaletteButton* pPalette)
{
	ASSERT_VALID (pPalette);

	pPalette->SetPaletteID (ID_INSERT_BULLET_FROM_PALETTE);

	pPalette->AddGroup (_T("Recently Used Bullets"), IDB_BULLETS_RECENT, 40);
	pPalette->AddGroup (_T("Bullets"), IDB_BULLETS, 40);

	pPalette->AddSubItem (
		new CBCGPRibbonButton (ID_PARA_CHANGE_LIST_LEVEL, _T("Change List Level")));

	pPalette->AddSubItem (
		new CBCGPRibbonButton (ID_PARA_DEFINENEWBULLET, _T("Define New Bullet...")));

	pPalette->SetIconsInRow (6);
}

void CMainFrame::InitNumberingPalette (CBCGPRibbonPaletteButton* pPalette)
{
	ASSERT_VALID (pPalette);

	pPalette->SetPaletteID (ID_INSERT_NUMBER_FROM_PALETTE);

	pPalette->AddGroup (_T("Numbering Library"), IDB_NUM, 76);

	pPalette->AddSubItem (
		new CBCGPRibbonButton (ID_PARA_CHANGE_LIST_LEVEL, _T("Change List Level")));

	pPalette->AddSubItem (
		new CBCGPRibbonButton (ID_PARA_DEFINENEWNUMBERFORMAT, _T("Define New Number Format...")));

	pPalette->SetIconsInRow (3);
}

void CMainFrame::OnToolsOptions ()
{
	ShowOptions (0);
}

LRESULT CMainFrame::OnRibbonCustomize (WPARAM wp, LPARAM lp)
{
	ShowOptions (1);
	return 1;
}

void CMainFrame::ShowOptions (int nPage)
{
	// Create "Customize" page:
	CBCGPRibbonCustomizePage pageCustomize (&m_wndRibbonBar);

	// Add "popular" items:
	CList<UINT, UINT> lstPopular;

	lstPopular.AddTail (ID_FILE_NEW);
	lstPopular.AddTail (ID_FILE_OPEN);
	lstPopular.AddTail (ID_FILE_SAVE);
	lstPopular.AddTail (ID_FILE_PRINT_PREVIEW);
	lstPopular.AddTail (ID_FILE_PRINT_DIRECT);
	lstPopular.AddTail (ID_FILE_MAIL);

	lstPopular.AddTail (ID_EDIT_UNDO);

	lstPopular.AddTail (ID_PAGELAYOUT_PAGEBACKGROUND_PAGEBORDERS);
	lstPopular.AddTail (ID_PAGELAYOUT_PAGESETUP_BREAKS);

	lstPopular.AddTail (ID_INSERT_DRAWTABLE);
	lstPopular.AddTail (ID_INSERT_HYPERLINK);
	lstPopular.AddTail (ID_INSERT_PICTURE);

	pageCustomize.AddCustomCategory (_T("Popular Commands"), lstPopular);

	// Add hidden commands:
	CList<UINT,UINT> lstHidden;
	m_wndRibbonBar.GetItemIDsList (lstHidden, TRUE);

	pageCustomize.AddCustomCategory (_T("Commands not in the Ribbon"), lstHidden);

	// Add all commands:
	CList<UINT,UINT> lstAll;
	m_wndRibbonBar.GetItemIDsList (lstAll);

	pageCustomize.AddCustomCategory (_T("All Commands"), lstAll);

	// Create "Options" and "Resources" pages:
	COptionsPage pageOptions;
	CResourcePage pageRes;

	// Create property sheet:
	COptionsPropSheet propSheet (this, nPage);
	propSheet.EnablePageHeader (max (60, globalData.GetTextHeight () * 3));

	propSheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;

	propSheet.SetLook (CBCGPPropertySheet::PropSheetLook_List, 124);

	propSheet.AddPage (&pageOptions);
	propSheet.AddPage (&pageCustomize);
	propSheet.AddPage (&pageRes);

	if (propSheet.DoModal() != IDOK)
	{
		return;
	}

	// Show ot hide "Developer" tab:
	int nTabDevIndex = m_wndRibbonBar.FindCategoryIndexByData (idTabDeveloper);

	if (theApp.m_bShowDevTab)
	{
		if (nTabDevIndex < 0)
		{
			AddTab_Developer ();
		}
	}
	else
	{
		if (nTabDevIndex >= 0)
		{
			m_wndRibbonBar.RemoveCategory (nTabDevIndex );
		}
	}

	// Change application theme and tooltips:
	OnAppLook (theApp.m_nAppLook);

	m_wndRibbonBar.EnableToolTips (theApp.m_bShowToolTips, theApp.m_bShowToolTipDescr);
}


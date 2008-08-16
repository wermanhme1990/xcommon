// ColorGridCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPGridExample.h"
#include "ColorGridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//------------------------------
// Grid color themes definition:
//------------------------------

static BCGP_GRID_COLOR_DATA theme1 = 
{
	-1,	// Grid background color
	-1,	// Grid foreground color
	-1,	// Header foreground color

	{	// Even rows colors:
		RGB (246, 250, 253), -1, -1, 0, -1
	},

	{	// Odd rows colors:
		-1, -1, -1, 0, -1
	},

	{	// Group colors:
		RGB (210, 230, 249), RGB (16, 37, 127), RGB (228, 241, 251), 90, -1
	},

	{	// Selected group colors:
		RGB (250, 199, 97), RGB (0, 0, 0), -1, 0, RGB (192, 192, 192)
	},

	{	// Selected rows colors:
		RGB (255, 229, 153), -1, -1, 0, RGB (192, 192, 192)
	},

	{	// Header item colors:
		RGB (154, 194, 237), RGB (16, 37, 127), RGB (189, 214, 242), 90, RGB (136, 176, 228)
	},

	{	// Selected header item colors:
		-1, -1, -1, 0, -1
	},

	{	// Left offset colors:
		RGB (207, 221, 240), -1, RGB (227, 234, 244), 90, RGB (136, 176, 228)
	},

	-1,	// Grid horizontal line text
	-1,	// Grid vertical line text
};

static BCGP_GRID_COLOR_DATA theme2 = 
{
	-1,	// Grid background color
	RGB (50, 50, 50),	// Grid foreground color
	-1,	// Header foreground color

	{	// Even rows colors:
		RGB (238, 238, 238), -1, -1, 0, -1
	},

	{	// Odd rows colors:
		-1, -1, -1, 0, -1
	},

	{	// Group colors:
		RGB (222, 230, 240), RGB (72, 105, 148), RGB (255, 255, 255), 90, -1
	},

	{	// Selected group colors:
		RGB (117, 150, 188), RGB (249, 250, 252), RGB (183, 200, 220), 90, -1
	},

	{	// Selected colors:
		RGB (175, 194, 217), RGB (52, 77, 108), RGB (222, 230, 240), 90, RGB (175, 194, 217)
	},

	{	// Header item colors:
		RGB (215, 218, 227), -1, RGB (244, 245, 248), 90, RGB (173, 183, 205)
	},

	{	// Selected header item colors:
		RGB (251, 157, 105), -1, RGB (254, 204, 153), 90, RGB (173, 183, 205)
	},

	{	// Left offset colors:
		RGB (215, 218, 227), -1, RGB (244, 245, 248), 0, RGB (173, 183, 205)
	},

	-1,	// Grid horizontal line
	-1,	// Grid vertical line
};

static BCGP_GRID_COLOR_DATA theme3 = 
{
	RGB (241, 242, 229),	// Grid background color
	RGB (10, 72, 0),		// Grid foreground color
	-1,	// Header foreground color

	{	// Even rows color
		RGB (218, 222, 185), -1, -1, 0, -1
	},

	{	// Odd rows colors:
		RGB (241, 242, 229), -1, -1, 0, -1
	},

	{	// Group colors:
		RGB (163, 173, 81), RGB (76, 80, 38), RGB (218, 222, 185), 90, RGB (163, 173, 81)
	},

	{	// Selected group colors:
		RGB (112, 163, 106), RGB (0, 0, 0), RGB (154, 190, 149), 90, RGB (112, 163, 106)
	},

	{	// Selected rows colors:
		RGB (233, 207, 181), RGB (76, 9, 0), -1, 0, RGB (214, 141, 32)
	},

	{	// Header item colors:
		RGB (205, 180, 163), RGB (128, 37, 0), RGB (217, 198, 186), 90, RGB (199, 171, 153)
	},

	{	// Selected header item color:
		RGB (233, 207, 181), RGB (128, 37, 0), RGB (234, 223, 216), 90, RGB (150, 155, 102)
	},

	{	// Left offset colors:
		RGB (196, 165, 145), -1, RGB (220, 202, 191), 45, RGB (150, 155, 102)
	},

	-1,	// Grid horizontal line text
	-1,	// Grid vertical line text
};

/////////////////////////////////////////////////////////////////////////////
// CColorGridCtrl

CColorGridCtrl::CColorGridCtrl()
{
}

CColorGridCtrl::~CColorGridCtrl()
{
}

BEGIN_MESSAGE_MAP(CColorGridCtrl, CBCGPGridCtrl)
	//{{AFX_MSG_MAP(CColorGridCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CColorGridCtrl message handlers

int CColorGridCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	//------------------------------
	// Intialize color themes array:
	//------------------------------
	m_Themes [0] = theme1;
	m_Themes [1] = theme2;
	m_Themes [2] = theme3;

	//--------------------------
	// Set grid general options:
	//--------------------------
	EnableMarkSortedColumn (TRUE, FALSE);
	EnableHeader (TRUE, BCGP_GRID_HEADER_MOVE_ITEMS | BCGP_GRID_HEADER_SORT);

	//-------------
	// Add columns:
	//-------------
	const int nColumns = 4;
	
	for (int nColumn = 0; nColumn < nColumns; nColumn++)
	{
		CString strColumn;
		strColumn.Format (_T("%c"), _T('A') + nColumn);

		InsertColumn (nColumn, strColumn, 80);
	}

	//-----------------------
	// Group by first column:
	//-----------------------
	InsertGroupColumn (0, 0);

	//-----------------
	// Add simple data:
	//-----------------
	for (int nRow = 0; nRow < 29; nRow++)
	{
		CBCGPGridRow* pRow = CreateRow (GetColumnCount ());

		for (int i = 0; i < GetColumnCount (); i++)
		{
			pRow->GetItem (i)->SetValue ((long) ((nRow + 1) * (i + 1)));
		}

		AddRow (pRow, FALSE);
	}

	LoadState (_T("ColorGrid"));

	//---------------------
	// Select 4 grid cells:
	//---------------------
	SetCurSel (CBCGPGridItemID (3, 1));
	SetCurSel (CBCGPGridItemID (5, 2), SM_CONTINUE_SEL_GROUP | SM_SINGLE_ITEM);

	AdjustLayout ();

	return 0;
}
//********************************************************************************
void CColorGridCtrl::OnDestroy() 
{
	SaveState (_T("ColorGrid"));
	CBCGPGridCtrl::OnDestroy();
}
//********************************************************************************
void CColorGridCtrl::UpdateColorTheme ()
{
	if (theApp.m_Options.m_nColorTheme < 0 || 
		theApp.m_Options.m_nColorTheme >= COLOR_THEMES)
	{
		ASSERT (FALSE);
		return;
	}

	SetColorTheme (m_Themes [theApp.m_Options.m_nColorTheme]);
}
//********************************************************************************
int CColorGridCtrl::CompareGroup (const CBCGPGridRow* pRow1, const CBCGPGridRow* pRow2, int iColumn)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pRow1);
	ASSERT_VALID (pRow2);

	long lVal1 = (long) pRow1->GetItem (0)->GetValue ();
	long lVal2 = (long) pRow2->GetItem (0)->GetValue ();

	return lVal1 / 10 * 10 - lVal2 / 10 * 10;
}
//********************************************************************************
CString CColorGridCtrl::GetGroupName (int /*iColumn*/, CBCGPGridItem* pItem)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pItem);

	long lVal = (long) pItem->GetValue ();
	long lMin = lVal / 10 * 10;
	long lMax = lMin + 9;

	CString str;
	str.Format (_T("Range: [%d - %d]"), lMin, lMax);

	return str;
}

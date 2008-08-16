// ListCtrlDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlDemo.h"
#include "ListCtrlDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlDemoDlg dialog

CListCtrlDemoDlg::CListCtrlDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CListCtrlDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListCtrlDemoDlg)
	m_nColumn = 0;
	m_nRow = 0;
	m_bMarkSortedColumn = TRUE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CListCtrlDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListCtrlDemoDlg)
	DDX_Control(pDX, IDC_SPIN_ROW, m_wndSpin2);
	DDX_Control(pDX, IDC_SPIN_COLUMN, m_wndSpin1);
	DDX_Control(pDX, IDC_TEXT_COLOR, m_clrText);
	DDX_Control(pDX, IDC_BKGR_COLOR, m_clrBkgr);
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	DDX_Text(pDX, IDC_COLUMN, m_nColumn);
	DDX_Text(pDX, IDC_ROW, m_nRow);
	DDX_Check(pDX, IDC_MARK_SORTED_COLOR, m_bMarkSortedColumn);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CListCtrlDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CListCtrlDemoDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SET_COOR, OnSetCoor)
	ON_BN_CLICKED(IDC_MARK_SORTED_COLOR, OnMarkSortedColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlDemoDlg message handlers

const int nColumns = 6;
const int nRows = 50;

BOOL CListCtrlDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_wndList.InsertColumn (0, _T("#"), LVCFMT_LEFT, 20);

	int nColumn = 0;

	for (nColumn = 1; nColumn < nColumns - 1; nColumn++)
	{
		m_wndList.InsertColumn (nColumn, CString ((TCHAR)(_T('A') + nColumn - 1)), LVCFMT_LEFT, 70);
	}

	for (int i = 0; i < nRows; i++)
	{
		CString str;
		str.Format ("%d", i);
		m_wndList.InsertItem (i, str);
		m_wndList.SetItemData (i, i);

		for (nColumn = 1; nColumn < nColumns - 1; nColumn++)
		{
			str.Format ("Item (%d, %d)", nColumn - 1, i);
			m_wndList.SetItemText (i, nColumn, str);
		}
	}
	
	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,
						LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndList.EnableMarkSortedColumn ();


	m_clrText.SetColor (m_wndList.GetTextColor ());
	m_clrText.EnableOtherButton (_T("Other..."));

	m_clrBkgr.SetColor (m_wndList.GetBkColor ());
	m_clrBkgr.EnableOtherButton (_T("Other..."));

	m_wndSpin1.SetRange (0, nColumns - 2);
	m_wndSpin2.SetRange (0, nRows - 1);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CListCtrlDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CListCtrlDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

COLORREF CMyListCtrl::OnGetCellTextColor (int nRow, int nColumn)
{
	for (POSITION pos = m_ItemColors.GetHeadPosition (); pos != NULL;)
	{
		CMyListItem* pItem = m_ItemColors.GetNext (pos);
		ASSERT_VALID (pItem);

		if (pItem->m_x == nColumn && pItem->m_y == nRow)
		{
			if (pItem->m_clrText != -1)
			{
				return pItem->m_clrText;
			}

			break;
		}
	}

	return CBCGPListCtrl::OnGetCellTextColor (nRow, nColumn);
}

COLORREF CMyListCtrl::OnGetCellBkColor (int nRow, int nColumn)
{
	for (POSITION pos = m_ItemColors.GetHeadPosition (); pos != NULL;)
	{
		CMyListItem* pItem = m_ItemColors.GetNext (pos);
		ASSERT_VALID (pItem);

		if (pItem->m_x == nColumn && pItem->m_y == nRow)
		{
			if (pItem->m_clrBckg != -1)
			{
				return pItem->m_clrBckg;
			}

			break;
		}
	}

	return CBCGPListCtrl::OnGetCellBkColor (nRow, nColumn);
}

int CMyListCtrl::OnCompareItems (LPARAM lParam1, LPARAM lParam2, int iColumn)
{
	CString str1 = GetItemText ((int) lParam1, iColumn);
	CString str2 = GetItemText ((int) lParam2, iColumn);

	return str1.Compare (str2);
}

void CListCtrlDemoDlg::OnSetCoor() 
{
	UpdateData ();

	if (m_nColumn < 0 || m_nColumn > nColumns - 2)
	{
		CString str;
		str.Format (_T("Column shoud be 0 - %d"), nColumns - 2);

		MessageBox (str);
		return;
	}

	if (m_nRow < 0 || m_nRow > nRows - 1)
	{
		CString str;
		str.Format (_T("Row shoud be 0 - %d"), nRows - 1);

		MessageBox (str);
		return;
	}

	CMyListItem* pItem = NULL;

	for (POSITION pos = m_wndList.m_ItemColors.GetHeadPosition (); pos != NULL;)
	{
		CMyListItem* pListItem = m_wndList.m_ItemColors.GetNext (pos);
		ASSERT_VALID (pListItem);

		if (pListItem->m_x == m_nColumn && pListItem->m_y == m_nRow)
		{
			pItem = pListItem;
			break;
		}
	}

	if (pItem == NULL)
	{
		pItem = new CMyListItem (m_nColumn, m_nRow, m_clrText.GetColor (), m_clrBkgr.GetColor ());
		m_wndList.m_ItemColors.AddTail (pItem);
	}
	else
	{
		pItem->m_clrText = m_clrText.GetColor ();
		pItem->m_clrBckg = m_clrBkgr.GetColor ();
	}

	m_wndList.RedrawWindow ();
}

void CListCtrlDemoDlg::OnMarkSortedColor() 
{
	UpdateData ();
	m_wndList.EnableMarkSortedColumn (m_bMarkSortedColumn);
}

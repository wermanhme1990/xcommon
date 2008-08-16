// DBGridDemoView.cpp : implementation of the CDBGridDemoView class
//

#include "stdafx.h"
#include "DBGridDemo.h"

#include "DBGridDemoDoc.h"
#include "DBGridDemoView.h"

#include "TableDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBGridDemoView

IMPLEMENT_DYNCREATE(CDBGridDemoView, CBCGPGridView)

BEGIN_MESSAGE_MAP(CDBGridDemoView, CBCGPGridView)
	//{{AFX_MSG_MAP(CDBGridDemoView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CBCGPGridView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CBCGPGridView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBGridDemoView construction/destruction

CDBGridDemoView::CDBGridDemoView()
{
	// TODO: add construction code here

}

CDBGridDemoView::~CDBGridDemoView()
{
}

BOOL CDBGridDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CBCGPGridView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDBGridDemoView drawing

void CDBGridDemoView::OnDraw(CDC* pDC)
{
	CDBGridDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CDBGridDemoView printing

void CDBGridDemoView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CDBGridDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDBGridDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDBGridDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDBGridDemoView diagnostics

#ifdef _DEBUG
void CDBGridDemoView::AssertValid() const
{
	CBCGPGridView::AssertValid();
}

void CDBGridDemoView::Dump(CDumpContext& dc) const
{
	CBCGPGridView::Dump(dc);
}

CDBGridDemoDoc* CDBGridDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDBGridDemoDoc)));
	return (CDBGridDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDBGridDemoView message handlers

void CDBGridDemoView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}

void CDBGridDemoView::OnInitialUpdate() 
{
	CDBGridDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CBCGPGridView::OnInitialUpdate();

	if (pDoc->m_strDBPath.IsEmpty ())
	{
		return;
	}

	CBCGPDBGridCtrl* pGrid = DYNAMIC_DOWNCAST(CBCGPDBGridCtrl, 
		GetGridCtrl ());
	ASSERT_VALID (pGrid);

	try
	{
		if (!pGrid->OpenMSAcessFile (pDoc->m_strDBPath))
		{
			return;
		}

		CStringList lstTable;
		pGrid->GetTableList (lstTable);

		if (lstTable.IsEmpty ())
		{
			return;
		}

		CTableDlg dlg (lstTable);
		if (dlg.DoModal () != IDOK)
		{
			pGrid->Close ();
			return;
		}

		CWaitCursor wait;
		if (!pGrid->OpenTable (dlg.m_strTable))
//		if (!pGrid->OpenSQL ("SELECT TITLE, PRODUCT FROM BUGS"))
		{
			pGrid->Close ();
			return;
		}
	}
	catch (CDBException* pEx)
	{
		pEx->ReportError ();
		pEx->Delete ();
	}
	catch (CDaoException* pEx)
	{
		pEx->ReportError ();
		pEx->Delete ();
	}
	catch (_com_error& err)
	{
		MessageBox (err.ErrorMessage ());
	}
}

void CDBGridDemoView::DeleteAll ()
{
	CBCGPDBGridCtrl* pGrid = DYNAMIC_DOWNCAST(CBCGPDBGridCtrl, 
		GetGridCtrl ());
	ASSERT_VALID (pGrid);

	try
	{
		pGrid->Close ();
	}
	catch (CDBException* pEx)
	{
		pEx->ReportError ();
		pEx->Delete ();
	}
	catch (CDaoException* pEx)
	{
		pEx->ReportError ();
		pEx->Delete ();
	}
	catch (_com_error& err)
	{
		MessageBox (err.ErrorMessage ());
	}
}

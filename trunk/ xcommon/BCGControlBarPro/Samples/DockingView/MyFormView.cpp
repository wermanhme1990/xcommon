// MyFormView.cpp : implementation file
//

#include "stdafx.h"
#include "DockingView.h"
#include "MyFormView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFormView

IMPLEMENT_DYNCREATE(CMyFormView, CFormView)

CMyFormView::CMyFormView()
	: CFormView(CMyFormView::IDD)
{
	//{{AFX_DATA_INIT(CMyFormView)
	m_strCount = _T("0");
	//}}AFX_DATA_INIT

	m_nCount = 0;
}

CMyFormView::~CMyFormView()
{
}

void CMyFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyFormView)
	DDX_Text(pDX, IDC_UPDATE_COUNT, m_strCount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyFormView, CFormView)
	//{{AFX_MSG_MAP(CMyFormView)
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFormView diagnostics

#ifdef _DEBUG
void CMyFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMyFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyFormView message handlers

BOOL CMyFormView::CreateView(CWnd* pParent, CCreateContext* pContext)
{
    CRect rect(0,0,300,300);

    if (!Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
        rect, pParent, AFX_IDW_PANE_FIRST, pContext))
    {
        TRACE0("Warning: couldn't create CMyFormView!\n");
        return FALSE;
    }

    return TRUE;
}

void CMyFormView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	m_nCount++;

	m_strCount.Format (_T("%d"), m_nCount);
	UpdateData (FALSE);
}

int CMyFormView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

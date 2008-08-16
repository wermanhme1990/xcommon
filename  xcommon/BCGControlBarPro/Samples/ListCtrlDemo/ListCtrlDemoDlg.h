// ListCtrlDemoDlg.h : header file
//

#if !defined(AFX_LISTCTRLDEMODLG_H__5824D41D_974F_478B_929F_7EB656543F60__INCLUDED_)
#define AFX_LISTCTRLDEMODLG_H__5824D41D_974F_478B_929F_7EB656543F60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyListItem : public CObject
{
public:
	CMyListItem(int x, int y, COLORREF clrText, COLORREF clrBckg) :
		m_x (x),
		m_y (y),
		m_clrText (clrText),
		m_clrBckg (clrBckg)
	{
	}

	const int	m_x;
	const int	m_y;
	COLORREF	m_clrText;
	COLORREF	m_clrBckg;
};

class CMyListCtrl : public CBCGPListCtrl
{
	friend class CListCtrlDemoDlg;

	virtual ~CMyListCtrl ()
	{
		while (!m_ItemColors.IsEmpty ())
		{
			delete m_ItemColors.RemoveHead ();
		}
	}

	virtual int OnCompareItems (LPARAM lParam1, LPARAM lParam2, int iColumn);

	virtual COLORREF OnGetCellTextColor (int nRow, int nColum);
	virtual COLORREF OnGetCellBkColor (int nRow, int nColum);

	CList<CMyListItem*,CMyListItem*>	m_ItemColors;
};

/////////////////////////////////////////////////////////////////////////////
// CListCtrlDemoDlg dialog

#define CDialog CBCGPDialog

class CListCtrlDemoDlg : public CDialog
{
// Construction
public:
	CListCtrlDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CListCtrlDemoDlg)
	enum { IDD = IDD_LISTCTRLDEMO_DIALOG };
	CSpinButtonCtrl	m_wndSpin2;
	CSpinButtonCtrl	m_wndSpin1;
	CBCGPColorButton	m_clrText;
	CBCGPColorButton	m_clrBkgr;
	CMyListCtrl	m_wndList;
	int		m_nColumn;
	int		m_nRow;
	BOOL	m_bMarkSortedColumn;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON	m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CListCtrlDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSetCoor();
	afx_msg void OnMarkSortedColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLDEMODLG_H__5824D41D_974F_478B_929F_7EB656543F60__INCLUDED_)

#if !defined(AFX_REPORTGRIDCTRL_H__DBD71C42_FC2A_41EB_ADC9_6AEF77275A6D__INCLUDED_)
#define AFX_REPORTGRIDCTRL_H__DBD71C42_FC2A_41EB_ADC9_6AEF77275A6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportGridCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFlagItem class

class CFlagItem: public CBCGPGridItem
{
public:
	CFlagItem(
		CBCGPToolBarImages& icons,
		int nSelectedIcon,
		DWORD dwData = 0
	);

public:
	virtual BOOL OnClickValue (UINT uiMsg, CPoint point);
	
protected:
	virtual void OnDrawValue (CDC* pDC, CRect rect);
	virtual void OnPrintValue (CDC* /*pDC*/, CRect /*rect*/) {}
	CBCGPToolBarImages&	m_Icons;
};

/////////////////////////////////////////////////////////////////////////////
// CReportGridCtrl window

class CReportGridCtrl : public CBCGPReportCtrl
{
// Construction
public:
	CReportGridCtrl();

// Attributes
public:
	enum IMPORTANCE
	{
		IMPORTANCE_NORMAL,
		IMPORTANCE_HIGH,
		IMPORTANCE_LOW
	};

// Operations
public:
	BOOL AddMail (int nIcon, LPCTSTR strFrom, LPCTSTR strTo,
			 LPCTSTR strSubject, COleDateTime dateCreated, 
			 COleDateTime dateReceived, long lSize, 
			 IMPORTANCE importance = IMPORTANCE_NORMAL,
			 BOOL bHasAttachment = FALSE,
			 int nFlag = 0);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportGridCtrl)
	//}}AFX_VIRTUAL

	virtual int CompareGroup (const CBCGPGridRow* pRow1, const CBCGPGridRow* pRow2, int iColumn);
	virtual CString GetGroupName (int nGroupCol, CBCGPGridItem* pItem);

	virtual BOOL OnHideInsertedGroupColumn (int nColumn) const
	{
		return TRUE;
	}

// Implementation
public:
	virtual ~CReportGridCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CReportGridCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CImageList			m_ImagesHeader;
	CImageList			m_Images;
	CBCGPToolBarImages	m_Flags;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTGRIDCTRL_H__DBD71C42_FC2A_41EB_ADC9_6AEF77275A6D__INCLUDED_)

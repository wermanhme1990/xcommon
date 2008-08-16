#if !defined(AFX_BCGPMASKEDIT_H__F333B383_1E7B_4C5B_A133_24CC735EAE84__INCLUDED_)
#define AFX_BCGPMASKEDIT_H__F333B383_1E7B_4C5B_A133_24CC735EAE84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BCGPMaskEdit.h : header file
//

#include "BCGCBPro.h"

/////////////////////////////////////////////////////////////////////////////
// CBCGPMaskEdit window

class BCGCBPRODLLEXPORT CBCGPMaskEdit : public CEdit
{
	DECLARE_DYNAMIC(CBCGPMaskEdit)

// Construction
public:
	CBCGPMaskEdit();
	~CBCGPMaskEdit();

// Implementation
public:
	void EnableMask(LPCTSTR lpszMask, LPCTSTR lpszInputTemplate, 
					TCHAR chMaskInputTemplate = _T('_'), LPCTSTR lpszValid = NULL);
	void DisableMask();

	void SetValidChars(LPCTSTR lpszValid = NULL);
	void EnableGetMaskedCharsOnly(BOOL bEnable = TRUE) { m_bGetMaskedCharsOnly = bEnable; }
	void EnableSetMaskedCharsOnly(BOOL bEnable = TRUE) { m_bSetMaskedCharsOnly = bEnable; }
	void EnableSelectByGroup(BOOL bEnable = TRUE) { m_bSelectByGroup = bEnable; }
	
	// Replace standard operations
	// CWnd:	SetWindowText, GetWindowText
	void SetWindowText(LPCTSTR lpszString);
	int GetWindowText(LPTSTR lpszStringBuf, int nMaxCount) const;
	void GetWindowText(CString& rstrString) const;

protected:
	virtual BOOL IsMaskedChar(TCHAR chChar, TCHAR chMaskChar) const;

	const CString GetValue() const { return m_str;}
	const CString GetMaskedValue(BOOL bWithSpaces = TRUE) const;
	BOOL SetValue(LPCTSTR lpszString, BOOL bWithDelimiters = TRUE);

private:
	BOOL CheckChar(TCHAR chChar, int nPos);
	void OnCharPrintchar(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnCharBackspace(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnCharDelete(UINT nChar, UINT nRepCnt, UINT nFlags);
	void GetGroupBounds(int &nBegin, int &nEnd, int nStartPos=0, BOOL bForward=TRUE);
	
// Attributes
private:
	CString		m_str;					// Initial value
	CString		m_strMask;				// The mask string
	CString		m_strInputTemplate;		// "_" char = character entry
	TCHAR		m_chMaskInputTemplate;	// Default char
	CString		m_strValid;             // Valid string characters
	BOOL		m_bGetMaskedCharsOnly;
	BOOL		m_bSetMaskedCharsOnly;
	BOOL		m_bSelectByGroup;
	BOOL		m_bMaskKeyInProgress;		
	BOOL		m_bPasteProcessing;

protected:
	// Generated message map functions
	//{{AFX_MSG(CBCGPMaskEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnUpdateR();
	afx_msg void OnSetFocusR();
	//}}AFX_MSG
	LRESULT OnPaste(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPMASKEDIT_H__F333B383_1E7B_4C5B_A133_24CC735EAE84__INCLUDED_)

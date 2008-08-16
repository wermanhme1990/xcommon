// BCGPUserTool.h: interface for the CBCGPUserTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPUSERTOOL_H__00F1EBC5_61BF_11D5_A304_F156F535EA11__INCLUDED_)
#define AFX_BCGPUSERTOOL_H__00F1EBC5_61BF_11D5_A304_F156F535EA11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGCBPro.h"

class BCGCBPRODLLEXPORT CBCGPUserTool : public CObject  
{
	friend class CBCGPUserToolsManager;

	DECLARE_SERIAL(CBCGPUserTool)

public:
// Construction
	CBCGPUserTool();
	virtual ~CBCGPUserTool();

// Overrides
	virtual void Serialize (CArchive& ar);
	virtual BOOL Invoke ();
	virtual HICON SetToolIcon ();

// Operations:
public:
	void DrawToolIcon (CDC* pDC, const CRect& rectImage);
	void SetCommand (LPCTSTR lpszCmd);
	BOOL CopyIconToClipboard ();

protected:
	virtual HICON LoadDefaultIcon ();
	virtual void DeleteIcon ();

// Attributes:
public:
	UINT GetCommandId () const
	{
		return m_uiCmdId;
	}

	const CString& GetCommand () const
	{
		return m_strCommand;
	}

	CString		m_strLabel;
	CString		m_strArguments;
	CString		m_strInitialDirectory;

protected:
	UINT		m_uiCmdId;
	CString		m_strCommand;
	HICON		m_hIcon;
};

#endif // !defined(AFX_BCGPUSERTOOL_H__00F1EBC5_61BF_11D5_A304_F156F535EA11__INCLUDED_)

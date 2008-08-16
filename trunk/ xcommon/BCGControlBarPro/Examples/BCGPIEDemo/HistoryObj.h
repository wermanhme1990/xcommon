// HistoryObj.h: interface for the CHistoryObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISTORYOBJ_H__F83C8E1D_F33E_11D2_A713_0090274409AC__INCLUDED_)
#define AFX_HISTORYOBJ_H__F83C8E1D_F33E_11D2_A713_0090274409AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHistoryObj  
{
public:
	CHistoryObj(const CString& strTitle, const CString& strURL, const UINT uiCommand);
	virtual ~CHistoryObj();

	LPCTSTR GetTitle () const
	{
		return m_strTitle;
	}

	LPCTSTR GetURL () const
	{
		return m_strURL;
	}

	UINT GetCommand () const
	{
		return m_uiCommand;
	}

protected:
	const CString	m_strTitle;
	const CString	m_strURL;
	const UINT		m_uiCommand;
};

#endif // !defined(AFX_HISTORYOBJ_H__F83C8E1D_F33E_11D2_A713_0090274409AC__INCLUDED_)

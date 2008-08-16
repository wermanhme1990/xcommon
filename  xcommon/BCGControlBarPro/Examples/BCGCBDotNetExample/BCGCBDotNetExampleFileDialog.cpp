// BCGCBDotNetExampleFileDialog.cpp: implementation of the CBCGCBDotNetExampleFileDialog class
//
//////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGCBDotNetExample.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGCBDotNetExampleFileDialog

IMPLEMENT_DYNAMIC(CBCGCBDotNetExampleFileDialog, CFileDialog)

CBCGCBDotNetExampleFileDialog::CBCGCBDotNetExampleFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
							 DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
}


BEGIN_MESSAGE_MAP(CBCGCBDotNetExampleFileDialog, CFileDialog)
	//{{AFX_MSG_MAP(CBCGCBDotNetExampleFileDialog)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBCGCBDotNetExampleFileDialog message handlers

void CBCGCBDotNetExampleFileDialog::OnInitDone()
{
	CString strFileName(m_ofn.lpstrFile);

	if (!strFileName.IsEmpty())
	{
		TCHAR szFileExt[_MAX_EXT];
		_tsplitpath(strFileName, NULL, NULL, NULL, szFileExt);

		m_strSafeExt = szFileExt;

		if (m_strSafeExt.IsEmpty())
		{
			m_strSafeExt = _T('.');
		}
		else
		{
			if (szFileExt[0] != _T('.'))
			{
				m_strSafeExt = _T('.') + m_strSafeExt;
			}
		}

		m_nSafeFilterIndex = FindFilterIndex(m_strSafeExt);

		CComboBox* pCombo = (CComboBox*)GetActiveWindow()->GetDlgItem(IDC_FILELIST);
		pCombo->SetCurSel(m_nSafeFilterIndex - 1);
	}
	else
	{
		m_strSafeExt = _T('.');
	}
}

void CBCGCBDotNetExampleFileDialog::OnTypeChange()
{
	CFileDialog::OnTypeChange();

	CString str(m_ofn.lpstrFile);

	if (!m_bOpenFileDialog)
	{
		CWnd* pEdit = GetActiveWindow()->GetDlgItem(IDC_SELFILE);
		CString strFileName;
		pEdit->GetWindowText(strFileName);

		if (!strFileName.IsEmpty())
		{
			TCHAR szFileName[_MAX_FNAME];
			TCHAR szFileExt[_MAX_EXT];
			
			_tsplitpath(strFileName, NULL, NULL, szFileName, szFileExt);

			int ind = m_ofn.nFilterIndex;
			LPCTSTR lpszFilterExt = m_ofn.lpstrFilter;

			lpszFilterExt += _tcslen(lpszFilterExt) + 1;

			while (--ind)
			{
				lpszFilterExt += _tcslen(lpszFilterExt) + 1;
				lpszFilterExt += _tcslen(lpszFilterExt) + 1;
			}

			CString strFileExt;

			if (FindExt(lpszFilterExt,(LPCTSTR)m_strSafeExt))
			{
				strFileExt = m_strSafeExt;
			}
			else if (FindExt(lpszFilterExt,(LPCTSTR)szFileExt))
			{
				strFileExt = szFileExt;

				if (szFileExt[0] != _T('.'))
				{
					strFileExt = _T('.') + m_strSafeExt;
				}
			}
			else
			{
				GetFirstExt(lpszFilterExt, strFileExt);
			}

			strFileName = szFileName + strFileExt;
			pEdit->SetWindowText(strFileName);
		}
	}
}

DWORD CBCGCBDotNetExampleFileDialog::FindFilterIndex(const CString& strSafeExt) const
{
	if (strSafeExt.IsEmpty() ||
		strSafeExt == _T("."))
	{
		return 1;
	}

	DWORD ind = 1;

	LPCTSTR lpszFilterExt = m_ofn.lpstrFilter;

	lpszFilterExt += _tcslen(lpszFilterExt) + 1;

	while (*lpszFilterExt != _T('\0') &&
		   !FindExt(lpszFilterExt, strSafeExt))
	{
		ind++;

		if (*(lpszFilterExt += _tcslen(lpszFilterExt) + 1) != _T('\0'))
		{
			lpszFilterExt += _tcslen(lpszFilterExt) + 1;
		}
	}

	return ((*lpszFilterExt == _T('\0'))? 1: ind);
}

void CBCGCBDotNetExampleFileDialog::GetFirstExt(LPCTSTR lpszFilterExt, CString& strFileExt) const
{
	strFileExt = _T("");

	while (*lpszFilterExt != _T('\0'))
	{
		if (*lpszFilterExt++ == _T('.'))
		{
			int ind = 0;

			while (lpszFilterExt[ind] != _T('\0') &&
				   lpszFilterExt[ind] != _T(';'))
			{
				ind++;
			}

			LPTSTR pszTmp = strFileExt.GetBuffer(ind + 1);
			::CopyMemory((PVOID)pszTmp, lpszFilterExt, ind * sizeof(TCHAR));
			pszTmp[ind] = _T('\0');
			strFileExt.ReleaseBuffer();

			break;
		}
	}
	
	strFileExt = _T('.') + strFileExt;
}

BOOL CBCGCBDotNetExampleFileDialog::FindExt(LPCTSTR lpszFilterExt, LPCTSTR lpszFileExt) const
{
	DWORD bRet = FALSE;

	if (*lpszFileExt == _T('.'))
	{
		lpszFileExt++;
	}
	
	if (*lpszFilterExt != _T('\0'))
	{
		do
		{
			if (*lpszFilterExt == _T('.'))
			{
				int ind = 0;
				lpszFilterExt++;

				while (*lpszFilterExt != _T('\0') &&
					   *lpszFilterExt != _T(';') &&
					   *lpszFilterExt == lpszFileExt[ind])
				{
					lpszFilterExt++;
					ind++;
				}

				if ((*lpszFilterExt == _T('\0') ||
					 *lpszFilterExt == _T(';')) &&
					lpszFileExt[ind] == _T('\0'))
				{
					bRet = TRUE;
					break;
				}

				while (*lpszFilterExt != _T('\0') &&
					   *lpszFilterExt != _T(';'))
				{
					lpszFilterExt++;
				}

				if (*lpszFilterExt == _T('\0'))
				{
					break;
				}
			}
		}
		while (*(lpszFilterExt += 1) != _T('\0'));
	}

	return bRet;
}
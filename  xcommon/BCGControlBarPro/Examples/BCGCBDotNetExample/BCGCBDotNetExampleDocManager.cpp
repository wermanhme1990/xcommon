// BCGCBDotNetExampleDocManager.cpp: implementation of the CBCGCBDotNetExampleDocManager class
//
//////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGCBDotNetExample.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGCBDotNetExampleDocManager

IMPLEMENT_DYNAMIC(CBCGCBDotNetExampleDocManager, CDocManager)

/////////////////////////////////////////////////////////////////////////////
// Construction/Destruction

CBCGCBDotNetExampleDocManager::CBCGCBDotNetExampleDocManager()
{

}

CBCGCBDotNetExampleDocManager::~CBCGCBDotNetExampleDocManager()
{

}

/////////////////////////////////////////////////////////////////////////////
// CBCGCBDotNetExampleDocManager diagnostics

#ifdef _DEBUG
void CBCGCBDotNetExampleDocManager::AssertValid() const
{
	CDocManager::AssertValid();
}

void CBCGCBDotNetExampleDocManager::Dump(CDumpContext& dc) const
{
	CDocManager::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGCBDotNetExampleDocManager overridables

BOOL CBCGCBDotNetExampleDocManager::DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate)
{
	CBCGCBDotNetExampleFileDialog dlgFile(bOpenFileDialog);

	CString title;
	VERIFY(title.LoadString(nIDSTitle));

	dlgFile.m_ofn.Flags |= lFlags;

	CString strFilter;
	CString strDefault;
	if (pTemplate != NULL)
	{
		ASSERT_VALID(pTemplate);
		AppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate, &strDefault);
	}
	else
	{
		// do for all doc template
		POSITION pos = m_templateList.GetHeadPosition();
		BOOL bFirst = TRUE;
		while (pos != NULL)
		{
			CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
			AppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate,
				bFirst ? &strDefault : NULL);
			bFirst = FALSE;
		}
	}

	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	int nResult = dlgFile.DoModal();
	fileName.ReleaseBuffer();
	return nResult == IDOK;
}

/////////////////////////////////////////////////////////////////////////////
// CBCGCBDotNetExampleDocManager implementations

void CBCGCBDotNetExampleDocManager::AppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
	CDocTemplate* pTemplate, CString* pstrDefaultExt)
{
	ASSERT_VALID(pTemplate);
	ASSERT_KINDOF(CDocTemplate, pTemplate);

	CString strFilterExt, strFilterName;
	if (pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt) &&
	 !strFilterExt.IsEmpty() &&
	 pTemplate->GetDocString(strFilterName, CDocTemplate::filterName) &&
	 !strFilterName.IsEmpty())
	{
		// a file based document template - add to filter list
		switch (strFilterExt[0]) {
		case _T('.'):
		{
			if (pstrDefaultExt != NULL)
			{
				// set the default extension
				*pstrDefaultExt = ((LPCTSTR)strFilterExt) + 1;  // skip the '.'
				ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(*pstrDefaultExt);
				ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
			}

			// add to filter
			filter += strFilterName;
			ASSERT(!filter.IsEmpty());  // must have a file type name
			filter += (TCHAR)'\0';  // next string please
			filter += (TCHAR)'*';
			filter += strFilterExt;
			filter += (TCHAR)'\0';  // next string please
			ofn.nMaxCustFilter++;
		}
			break;
		case _T('*'):
		{
			int nExtStart,
				nNameStart,
				nExtEnd = -1,
				nNameEnd = -1;
			const int nExtLength = strFilterExt.GetLength();
			const int nNameLength = strFilterName.GetLength();

			while ((nExtStart = nExtEnd + 1) < nExtLength)
			{
				nNameStart = nNameEnd + 1;

				// Looking for next '|' delimiter from nNameStart position into name string
				if ((nNameEnd = strFilterName.Find(_T('|'),nNameStart)) == -1)
				{
					nNameEnd = nNameLength;
				}

				// Looking for next '|' delimiter from nExtStart position into extension string
				if ((nExtEnd = strFilterExt.Find(_T('|'),nExtStart)) == -1)
				{
					nExtEnd = nExtLength;
				}

				// Format "name" & "extension" to CString
				CString strName;
				int nNameLen = nNameEnd - nNameStart;

				if (nNameLen > 0)
				{
					strName = strFilterName.Mid(nNameStart,nNameLen);
				}

				const CString& strExt = strFilterExt.Mid(nExtStart,nExtEnd - nExtStart);
				
				// Append "name" & "extension" to filter string
				if (!strExt.IsEmpty())
				{
					if (strName.IsEmpty())
					{
						strName = _T('(') + strExt + _T(')');
					}

					filter += strName;
					filter += (TCHAR)'\0';  // next string please
					filter += strExt;
					filter += (TCHAR)'\0';  // next string please
					ofn.nMaxCustFilter++;
				}
				else
				{
					ASSERT(FALSE);
				}
			}
		}
			break;
		default:
			ASSERT(FALSE);
		}
	}
}

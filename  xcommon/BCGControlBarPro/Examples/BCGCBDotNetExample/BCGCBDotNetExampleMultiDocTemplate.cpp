// BCGCBDotNetExampleMultiDocTemplate.cpp: implementation of the CBCGCBDotNetExampleMultiDocTemplate class
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGCBDotNetExample.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGCBDotNetExampleDocManager

IMPLEMENT_DYNAMIC(CBCGCBDotNetExampleMultiDocTemplate, CMultiDocTemplate)

/////////////////////////////////////////////////////////////////////////////
// Construction/Destruction

CBCGCBDotNetExampleMultiDocTemplate::CBCGCBDotNetExampleMultiDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
										 CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass):
CMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{

}

CBCGCBDotNetExampleMultiDocTemplate::~CBCGCBDotNetExampleMultiDocTemplate()
{

}

/////////////////////////////////////////////////////////////////////////////
// CBCGCBDotNetExampleDocManager diagnostics

#ifdef _DEBUG
void CBCGCBDotNetExampleMultiDocTemplate::AssertValid() const
{
	CMultiDocTemplate::AssertValid();
}

void CBCGCBDotNetExampleMultiDocTemplate::Dump(CDumpContext& dc) const
{
	CMultiDocTemplate::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGCBDotNetExampleDocManager overridables

extern BOOL AFXAPI AfxComparePath(LPCTSTR lpszPath1, LPCTSTR lpszPath2);

CDocTemplate::Confidence CBCGCBDotNetExampleMultiDocTemplate::MatchDocType(LPCTSTR lpszPathName, CDocument*& rpDocMatch)
{
	ASSERT(lpszPathName != NULL);
	rpDocMatch = NULL;

	// go through all documents
	POSITION pos = GetFirstDocPosition();
	while (pos != NULL)
	{
		CDocument* pDoc = GetNextDoc(pos);
		if (AfxComparePath(pDoc->GetPathName(), lpszPathName))
		{
			// already open
			rpDocMatch = pDoc;
			return yesAlreadyOpen;
		}
	}

	// see if it matches our default suffix
	CString strFilterExt;
	if (GetDocString(strFilterExt, CDocTemplate::filterExt) &&
	  !strFilterExt.IsEmpty())
	{
		// see if extension matches

		switch (strFilterExt[0]) {
		case _T('.'):
		{
			LPCTSTR lpszDot = _tcsrchr(lpszPathName, '.');
			if (lpszDot != NULL && lstrcmpi(lpszDot, strFilterExt) == 0)
			{
				return yesAttemptNative; // extension matches, looks like ours
			}
		}
		case _T('*'):
		{
			int nExtStart,
				nExtEnd = -1;
			const int nExtLength = strFilterExt.GetLength();
			LPCTSTR lpszDot = _tcsrchr(lpszPathName, '.');

			if (lpszDot != NULL)
			{
				CString strCurExt(lpszDot);
				strCurExt.MakeLower();
				strCurExt = _T('*') + strCurExt;

				while ((nExtStart = nExtEnd + 1) < nExtLength)
				{
					// Looking for next '|' delimiter from nExtStart position into extension string
					if ((nExtEnd = strFilterExt.Find(_T('|'),nExtStart)) == -1)
					{
						nExtEnd = nExtLength;
					}

					// Format "extension" to CString and compare with current file extension
					CString strExt = strFilterExt.Mid(nExtStart, nExtEnd - nExtStart);
					strExt.MakeLower();

					if (strExt == strCurExt)
					{
						return yesAttemptNative; // extension matches, looks like ours
					}
				}
			}
		}
		}
	}

	// otherwise we will guess it may work
	return yesAttemptForeign;
}



#pragma once

#include <windef.h>
#include <Windows.h>
#include <XFlatCombo.h>
#include "XLogFont.h"
#include <list>
using namespace std;




enum XFontStyle
{
	xFontUnknown   = 0x00, // Display type not defined.
	xFontGUI       = 0x01, // Display font name with GUI font style.
	xFontSample    = 0x02, // Display font name with its own font style.
	xFontBoth      = 0x03, // Display font name with GUI font style, then a sample display to the right.
};


class CXFontCombo : public CXFlatComboBox
{
	DECLARE_DYNAMIC(CXFontCombo)

public:
	CXFontCombo();

	virtual ~CXFontCombo();

public:	
	virtual BOOL GetSelFont(CXLogFont& lf);
	virtual BOOL GetSelFont(CString& strFaceName); 

	virtual BOOL SetSelFont(CXLogFont& lf);
	virtual BOOL SetSelFont(const CString& strFaceName); 
  virtual void PreSubclassWindow(); 
	void SetListStyle(DWORD dwStyle);
	virtual void InitControl(LPCTSTR lpszFaceName = NULL, UINT nWidth = 0, BOOL bEnable = TRUE);
	void SetBitmap(CImageList*);
	void SetIcon(HICON);
protected:
	virtual void NotifyOwner(UINT nCode);
  static BOOL CALLBACK EnumFontFamExProc(ENUMLOGFONTEX* pelf, 
    NEWTEXTMETRICEX* lpntm,	DWORD dwFontType, LPARAM lParam);
  void InitFontLst(CDC* pDC = NULL, BYTE nCharSet = DEFAULT_CHARSET);
  BOOL AddFont(const LOGFONT* pLF, DWORD dwType);
  LONG GetMaxWidth();	
  BOOL DoesFontExist(CString& strFaceName);
  CXLogFont* GetLogFont(const CString& strFaceName);
	//BOOL FontCmp(const CXLogFont&, const CXLogFont&);
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXFontCombo)
	virtual void	DrawItem(LPDRAWITEMSTRUCT);
	virtual void	MeasureItem(LPMEASUREITEMSTRUCT);
	virtual int		CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual BOOL	PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXFontCombo)
	afx_msg BOOL OnDropDown();
	//}}AFX_MSG


//{{AFX_CODEJOCK_PRIVATE
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif
//}}AFX_CODEJOCK_PRIVATE

protected:
	DWORD       m_dwStyle;      // Enumerated style indicating how to display the font list.
	CString     m_csSymbol;     // String displayed for symbol characters.
	CImageList  m_ilFontType;   // true type font image list.
	HICON				m_hIconFontType;
  list<CXLogFont> m_listFonts;
private:
	CString     m_csSelected;   // Selected text set when CBN_DROPDOWN is called.
	HFONT				m_FontDefaultGUI;	
};

//////////////////////////////////////////////////////////////////////


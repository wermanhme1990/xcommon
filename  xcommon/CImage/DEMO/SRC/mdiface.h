// mdiface.h : header file

/////////////////////////////////////////////////////////////////////////////
// CMDIFaceWnd window

#include "..\..\cimage\include\cimage.h"

class CMDIFaceWnd : public CWnd
{
// Construction
public:
    CMDIFaceWnd();
// Implementation
public:
    virtual ~CMDIFaceWnd();
    // Generated message map functions
protected:
    CSize m_sizeClient ;
    CFont m_font;
	CString m_string;
	CImage m_image;
    //{{AFX_MSG(CMDIFaceWnd)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

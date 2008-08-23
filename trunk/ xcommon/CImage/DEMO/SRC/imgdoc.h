// imgdoc.h : interface of the CImageDoc class
//
/////////////////////////////////////////////////////////////////////////////

#include "imgobj.h"

/////////////////////////////////////////////////////////////////////////////
// CImageDoc

class CImageDoc : public CDocument
{
protected: // create from serialization only
	CImageDoc();
	DECLARE_DYNCREATE(CImageDoc)

// Attributes
public:
   enum
   {
      ENUM_hint_newimage = 101,
      MAX_UNDO_LEVELS = 8
   };

   CImageObject m_image;
	BOOL m_stretchMode;
	CImageObject* imageUndo[MAX_UNDO_LEVELS];
	int m_UndoLevel;

// Operations
public:
	inline CImageObject& GetImage() { return m_image; }
	inline BOOL GetStretchMode() { return m_stretchMode; }
	static int ComputePixel(float x, float y, float &x1, float &y1);
	void ComputeNewImage(void);
   void UpdateStatusBar(void);
   BOOL Rotate(double);
protected:
	void SubmitUndo();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive&);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL DoSave(LPCTSTR pszPathName, BOOL bReplace =TRUE);
	virtual void DeleteContents();
	virtual void SetTitle(LPCTSTR lpszTitle);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual CFile* GetFile(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError);

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImageDoc)
	afx_msg void OnUpdateIsOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStretchMode(CCmdUI* pCmdUI);
	afx_msg void OnTransformEllipse();
	afx_msg void OnWindowDuplicate();
	afx_msg void OnEditCopy();
	afx_msg void OnFlip();
	afx_msg void OnMirror();
	afx_msg void OnNegative();
	afx_msg void OnGreyscale();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnStretchMode();
   //}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

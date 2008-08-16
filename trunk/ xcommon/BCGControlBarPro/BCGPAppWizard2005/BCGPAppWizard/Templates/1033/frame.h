// [!output MAIN_FRAME_HEADER] : interface of the [!output MAIN_FRAME_CLASS] class
//


#pragma once
[!if APP_TYPE_SDI || APP_TYPE_MTLD]
[!if PROJECT_STYLE_EXPLORER]

class [!output VIEW_CLASS];
[!else]
[!if !DOCVIEW]

#include "[!output WND_VIEW_HEADER]"
[!endif]
[!endif]
[!endif] 
[!if WORKSPACEBAR]
#include "WorkSpaceBar.h"
#include "WorkSpaceBar2.h"
[!endif]
[!if OUTPUTBAR]
#include "OutputBar.h"
[!endif]

[!if APP_TYPE_MDI]
class [!output MAIN_FRAME_CLASS] : public [!output MAIN_FRAME_BASE_CLASS]
{
	DECLARE_DYNAMIC([!output MAIN_FRAME_CLASS])
public:
	[!output MAIN_FRAME_CLASS]();
[!else]
class [!output MAIN_FRAME_CLASS] : public [!output MAIN_FRAME_BASE_CLASS]
{
	
[!if !DOCVIEW]
public:
	[!output MAIN_FRAME_CLASS]();
protected: 
	DECLARE_DYNAMIC([!output MAIN_FRAME_CLASS])
[!else]
protected: // create from serialization only
	[!output MAIN_FRAME_CLASS]();
	DECLARE_DYNCREATE([!output MAIN_FRAME_CLASS])
[!endif]
[!endif]

// Attributes
[!if SPLITTER || PROJECT_STYLE_EXPLORER]
[!if APP_TYPE_SDI || APP_TYPE_MTLD]
protected:
	CSplitterWnd m_wndSplitter;
[!endif]
[!endif]
public:

// Operations
public:

// Overrides
public:
[!if SPLITTER || PROJECT_STYLE_EXPLORER]
[!if APP_TYPE_SDI || APP_TYPE_MTLD]
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
[!endif] 
[!endif] 
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
[!if !DOCVIEW]
[!if APP_TYPE_SDI]
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
[!endif] 
[!endif]
[!if APP_TYPE_MTLD]
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
[!endif]

// Implementation
public:
	virtual ~[!output MAIN_FRAME_CLASS]();
[!if PROJECT_STYLE_EXPLORER]
[!if APP_TYPE_SDI || APP_TYPE_MTLD]
	[!output VIEW_CLASS]* GetRightPane();
[!endif]
[!endif]
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
[!if DOCKING_TOOLBAR || STATUS_BAR]

protected:  // control bar embedded members
[!if MENUBAR]
	CBCGPMenuBar		m_wndMenuBar;
[!endif]
[!if STATUS_BAR]
	CBCGPStatusBar  m_wndStatusBar;
[!endif]
[!if DOCKING_TOOLBAR]
	CBCGPToolBar    m_wndToolBar;
[!endif]
[!if WORKSPACEBAR]
	CWorkSpaceBar	m_wndWorkSpace;
	CWorkSpaceBar2	m_wndWorkSpace2;
[!endif]
[!if OUTPUTBAR]
	COutputBar	m_wndOutput;
[!endif]
[!if OUTLOOKBAR]
	CBCGPOutlookBar	m_wndShortcutsBar;
[!if OUTLOOK_PAGES]
	CBCGPOutlookBarPane	m_wndShortcutsPane1;
	CBCGPOutlookBarPane	m_wndShortcutsPane2;
[!else]
	CBCGPOutlookBarPane	m_wndShortcutsPane1;
[!endif]
[!if OUTLOOK_EMBEDDED_CONTROL]
	CTreeCtrl		m_wndShortcutsTreeWnd;
[!endif]
[!endif]
[!if CAPTIONBAR]
	CBCGPCaptionBar	m_wndCaptionBar;
	CBitmap			m_bmpCaption;
[!endif]
[!if TOOLBAR_STYLE_REBAR]
	CBCGPReBar      m_wndReBar;
	CDialogBar      m_wndDlgBar;
[!endif]
[!endif]
[!if !DOCVIEW]
[!if APP_TYPE_SDI]
	[!output WND_VIEW_CLASS]    m_wndView;
[!endif] 
[!endif]
[!if USER_IMAGES]

	CBCGPToolBarImages	m_UserImages;
[!endif]
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
[!if !DOCVIEW]
[!if APP_TYPE_SDI]
	afx_msg void OnSetFocus(CWnd *pOldWnd);
[!endif] 
[!endif]
[!if APP_TYPE_MTLD]
	afx_msg void OnFileClose();
[!if !DOCVIEW]
	afx_msg void OnClose();
[!endif]
[!endif]
[!if PROJECT_STYLE_EXPLORER]
[!if APP_TYPE_SDI || APP_TYPE_MTLD]
[!if LIST_VIEW]
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewStyle(UINT nCommandID);
[!endif]
[!endif]
[!endif]
[!if TOOLBAR_CUSTOMIZATION]
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
[!endif]
[!if USER_TOOLBAR]
	void OnToolsViewUserToolbar (UINT id);
	void OnUpdateToolsViewUserToolbar (CCmdUI* pCmdUI);
[!endif]
[!if HELP]
[!if TOOLBAR_CUSTOMIZATION]
	afx_msg LRESULT OnHelpCustomizeToolbars(WPARAM wp, LPARAM lp);
[!endif]
[!endif]
[!if OUTLOOKBAR]
	void OnOutlookBarShortcut(UINT id);
[!endif]
[!if WINDOWS_DLG]
	afx_msg void OnWindowManager();
[!endif]
[!if VISUAL_MANAGER]
	afx_msg void OnAppLook(UINT id);
	afx_msg void OnUpdateAppLook(CCmdUI* pCmdUI);
[!endif]
[!if MDI_TAB_GROUPS]
	afx_msg void OnMdiMoveToNextGroup();
	afx_msg void OnMdiMoveToPrevGroup();
	afx_msg void OnMdiNewHorzTabGroup();
	afx_msg void OnMdiNewVertGroup();
	afx_msg void OnMdiCancel();
[!endif]
[!if APP_TYPE_MDI && DOCVIEW]
	afx_msg void OnClose();
[!endif]
	DECLARE_MESSAGE_MAP()

[!if MDI_TAB_GROUPS]
	virtual BOOL OnShowMDITabContextMenu (CPoint point, DWORD dwAllowedItems, BOOL bDrop);
[!endif]
[!if PROJTYPE_MDI && DOCVIEW]
	virtual CBCGPMDIChildWnd* CreateDocumentWindow (LPCTSTR lpcszDocName, CObject* /*pObj*/);
[!endif]
[!if OUTLOOKBAR]

	BOOL CreateShortcutsBar ();
[!endif]
[!if VISUAL_MANAGER]

	UINT	m_nAppLook;
[!endif]
};



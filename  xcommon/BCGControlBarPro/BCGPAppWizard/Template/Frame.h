// $$frame_hfile$$.h : interface of the $$FRAME_CLASS$$ class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)
#define $$FILE_NAME_SYMBOL$$_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
$$IF(PROJTYPE_SDI)
$$IF(PROJTYPE_EXPLORER)

class $$VIEW_CLASS$$;
$$ELIF(NODOCVIEW)

#include "$$wndview_hfile$$.h"
$$ENDIF
$$ENDIF 
$$IF(WORKSPACEBAR)
#include "$$workspace_hfile$$.h"
#include "$$workspace2_hfile$$.h"
$$ENDIF 
$$IF(OUTPUTBAR)
#include "$$outputbar_hfile$$.h"
$$ENDIF 
$$IF(TASKPANE)
#include "$$taskpane_hfile$$.h"
$$ENDIF 

$$IF(PROJTYPE_MDI)
#define $$FRAME_BASE_CLASS$$ CBCGPMDIFrameWnd

class $$FRAME_CLASS$$ : public $$FRAME_BASE_CLASS$$
{
	DECLARE_DYNAMIC($$FRAME_CLASS$$)
public:
	$$FRAME_CLASS$$();
$$ELSE // SDI
#define $$FRAME_BASE_CLASS$$ CBCGPFrameWnd

class $$FRAME_CLASS$$ : public $$FRAME_BASE_CLASS$$
{
	
$$IF(NODOCVIEW)
public:
	$$FRAME_CLASS$$();
protected: 
	DECLARE_DYNAMIC($$FRAME_CLASS$$)
$$ELSE
protected: // create from serialization only
	$$FRAME_CLASS$$();
	DECLARE_DYNCREATE($$FRAME_CLASS$$)
$$ENDIF
$$ENDIF // PROJTYPEMDI

// Attributes
$$IF(SPLITTER_SDI || PROJTYPE_EXPLORER)
$$IF(PROJTYPE_SDI)
protected:
	CSplitterWnd m_wndSplitter;
$$ENDIF 
$$ENDIF //SPLITTER_SDI
public:

// Operations
public:
$$IF(!PROJTYPE_DLG)
$$IF(!NODOCVIEW)
$$IF(TASKPANE)
	void UpdateMRUFilesList ()
	{
		m_wndTaskPane.UpdateMRUFilesList ();
	}
$$ENDIF
$$ENDIF
$$ENDIF

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL($$FRAME_CLASS$$)
$$IF(SPLITTER_SDI || FRAME_STYLES || PROJTYPE_EXPLORER)
	public:
$$ENDIF
$$IF(SPLITTER_SDI || PROJTYPE_EXPLORER)
$$IF(PROJTYPE_SDI)
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
$$ENDIF 
$$ENDIF
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
$$IF(NODOCVIEW)
$$IF(PROJTYPE_SDI)
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
$$ENDIF 
$$ENDIF
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~$$FRAME_CLASS$$();
$$IF(PROJTYPE_SDI)
$$IF(PROJTYPE_EXPLORER)
$$IF(CListView)
	$$VIEW_CLASS$$* GetRightPane();
$$ENDIF	// CListView
$$ENDIF	// PROJTYPE_EXPLORER
$$ENDIF	// PROJTYPE_SDI
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
$$IF(TOOLBAR || STATUSBAR || REBAR || MENUBAR || OUTLOOKBAR || CAPTIONBAR)

protected:  // control bar embedded members
$$IF(MENUBAR)
	CBCGPMenuBar		m_wndMenuBar;
$$ENDIF	// MENUBAR
$$IF(STATUSBAR)	
	CBCGPStatusBar	m_wndStatusBar;
$$ENDIF //STATUSBAR
$$IF(TOOLBAR)
	CBCGPToolBar		m_wndToolBar;
$$ENDIF //TOOLBAR
$$IF(WORKSPACEBAR)
	$$WORKSPACEBAR_CLASS$$	m_wndWorkSpace;
	$$WORKSPACEBAR_CLASS2$$	m_wndWorkSpace2;
$$ENDIF //WORKSPACEBAR
$$IF(OUTPUTBAR)
	$$OUTPUTBAR_CLASS$$	m_wndOutput;
$$ENDIF //OUTPUTBAR
$$IF(TASKPANE)
	$$TASKPANE_CLASS$$	m_wndTaskPane;
$$ENDIF //TASKPANE
$$IF(OUTLOOKBAR)
	CBCGPOutlookBar		m_wndShortcutsBar;
	CBCGPOutlookBarPane	m_wndShortcutsPane1;
	CBCGPOutlookBarPane	m_wndShortcutsPane2;
$$IF(OUTLOOK_EMBEDDED_CONTROL)
	CTreeCtrl		m_wndShortcutsTreeWnd;
$$ENDIF	// OUTLOOK_EMBEDDED_CONTROL
$$ENDIF //OUTLOOKBAR
$$IF(CAPTIONBAR)
	CBCGPCaptionBar	m_wndCaptionBar;
	CBitmap			m_bmpCaption;
$$ENDIF //CAPTIONBAR
$$IF(REBAR)
	CBCGPReBar		m_wndReBar;
$$ENDIF //REBAR
$$ENDIF //TOOLBAR || STATUSBAR || REBAR
$$IF(NODOCVIEW)
$$IF(PROJTYPE_SDI)
	$$WNDVIEW_CLASS$$    m_wndView;
$$ENDIF 
$$ENDIF
$$IF(USER_IMAGES)

	CBCGPToolBarImages	m_UserImages;
$$ENDIF

// Generated message map functions
protected:
	//{{AFX_MSG($$FRAME_CLASS$$)
$$IF(TOOLBAR || STATUSBAR || REBAR || MENUBAR || OUTLOOKBAR || CAPTIONBAR || WORKSPACEBAR || TASKPANE)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
$$ENDIF //TOOLBAR || STATUSBAR || REBAR || MENUBAR || OUTLOOKBAR || CAPTIONBAR || WORKSPACEBAR || TASKPANE
$$IF(NODOCVIEW)
$$IF(PROJTYPE_SDI)
	afx_msg void OnSetFocus(CWnd *pOldWnd);
$$ENDIF 
$$ENDIF
$$IF(VERBOSE)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
$$ENDIF
	//}}AFX_MSG
$$IF(TOOLBAR_CUSTOMIZATION)
	afx_msg void OnViewCustomize();
$$ENDIF
$$IF(PROJTYPE_EXPLORER)
$$IF(PROJTYPE_SDI)
$$IF(CListView)
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewStyle(UINT nCommandID);
$$ENDIF
$$ENDIF
$$ENDIF
$$IF(TOOLBAR_CUSTOMIZATION)
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
$$ENDIF
$$IF(USER_TOOLBAR)
	void OnToolsViewUserToolbar (UINT id);
	void OnUpdateToolsViewUserToolbar (CCmdUI* pCmdUI);
$$ENDIF
$$IF(HELP)
$$IF(TOOLBAR_CUSTOMIZATION)
	afx_msg LRESULT OnHelpCustomizeToolbars(WPARAM wp, LPARAM lp);
$$ENDIF
$$ENDIF
$$IF(TASKPANE)
	afx_msg void OnTask();
$$ENDIF
$$IF(OUTLOOKBAR)
	void OnOutlookBarShortcut(UINT id);
$$ENDIF
$$IF(WINDOWS_DLG)
	afx_msg void OnWindowManager();
$$ENDIF
$$IF(VISUAL_MANAGER)
	afx_msg void OnAppLook(UINT id);
	afx_msg void OnUpdateAppLook(CCmdUI* pCmdUI);
$$ENDIF
$$IF(MDI_TAB_GROUPS)
	afx_msg void OnMdiMoveToNextGroup();
	afx_msg void OnMdiMoveToPrevGroup();
	afx_msg void OnMdiNewHorzTabGroup();
	afx_msg void OnMdiNewVertGroup();
	afx_msg void OnMdiCancel();
$$ENDIF
$$IF(PROJTYPE_MDI)
	afx_msg void OnClose();
$$ENDIF
	DECLARE_MESSAGE_MAP()

$$IF(MDI_TAB_GROUPS)
	virtual BOOL OnShowMDITabContextMenu (CPoint point, DWORD dwAllowedItems, BOOL bDrop);
$$ENDIF
$$IF(PROJTYPE_MDI)
	virtual CBCGPMDIChildWnd* CreateDocumentWindow (LPCTSTR lpcszDocName, CObject* /*pObj*/);
$$ENDIF
$$IF(OUTLOOKBAR)

	BOOL CreateShortcutsBar ();
$$ENDIF
$$IF(TASKPANE)
	BOOL CreateTaskPane ();
$$ENDIF
$$IF(VISUAL_MANAGER)

	UINT	m_nAppLook;
$$ENDIF
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// $$INSERT_LOCATION_COMMENT$$

#endif // !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)

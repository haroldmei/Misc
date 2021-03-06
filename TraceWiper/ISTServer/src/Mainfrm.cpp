////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "resource.h"
#include "ContainerApp.h"
#include "mainfrm.h"
#include "resrc1.h"
#include "EntrySetDialog.h"
#include "cfgdlg.h"

extern bool   LoadCfgFile() ;

// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_DockView as the view window of the frame
	SetView(m_DockView);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\Container Docking"));

	
	LoadCfgFile();
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	// OnCommand responds to menu and and toolbar input
	switch(LOWORD(wParam))
	{
	case IDM_FILE_EXIT:
		// End the application
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	case IDM_DOCK_DEFAULT:
		m_DockView.CloseAllDockers();
		LoadDefaultDockers();
		return TRUE;
	case IDM_DOCK_CLOSEALL:
		m_DockView.CloseAllDockers();
		return TRUE;
	case IDW_VIEW_STATUSBAR:
		OnViewStatusbar();
		return TRUE;
	case IDW_VIEW_TOOLBAR:
		OnViewToolbar();
		return TRUE;
	case IDM_HELP_ABOUT:
		// Display the help dialog
		OnHelp();
		return TRUE;
	case ID_SETTINGS:{
		CCfgDlg *pDlg = new CCfgDlg(IDD_DIALOG_SERVER_PARA);
		pDlg->DoModal();
		return TRUE;}
	case IDM_FILE_NEW:
		{
			GetToolbar().DisableButton(IDM_FILE_NEW);
			GetToolbar().EnableButton(IDM_FILE_OPEN);
			return TRUE;
		}
	case IDM_FILE_OPEN:
		{
			GetToolbar().EnableButton(IDM_FILE_NEW);
			GetToolbar().DisableButton(IDM_FILE_OPEN);
			return TRUE;
		}
	case ID_ADD_USER:
	case ID_EDIT_USER:
		{
			CEntrySetDlg* m_pDialog;
			m_pDialog = new CEntrySetDlg(IDD_EDIT_USER, m_hWnd);
			m_pDialog->DoModal();
			
			// Clean up
			delete m_pDialog;
			m_pDialog = NULL;
						
			return TRUE; 
		}
		break;
	
	case ID_DELETE_USER:
		break;
	}

	return FALSE;
}

void CMainFrame::OnCreate()
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// The default for the following variables is TRUE

	// m_bShowIndicatorStatus = FALSE;	// Don't show statusbar indicators
	// m_bShowMenuStatus = FALSE;		// Don't show toolbar or menu status
	// m_bUseRebar = FALSE;				// Don't use rebars
	// m_bUseThemes = FALSE;            // Don't use themes
	// m_bUseToolbar = FALSE;			// Don't use a toolbar

	// call the base class function
	CFrame::OnCreate();
}

void CMainFrame::OnInitialUpdate()
{
	m_DockView.SetDockStyle(DS_CLIENTEDGE);

	// Load dock settings
	if (!m_DockView.LoadRegistrySettings(GetRegistryKeyName()))
		LoadDefaultDockers();

	// PreCreate initially set the window as invisible, so show it now.
	ShowWindow();
}

void CMainFrame::LoadDefaultDockers()
{
	// Note: The  DockIDs are used for saving/restoring the dockers state in the registry

	DWORD dwStyle = DS_CLIENTEDGE; // The style added to each docker
	
	// Add the parent dockers
	//CDocker* pDockRight  = m_DockView.AddDockedChild(new CDockClasses, DS_DOCKED_LEFT | dwStyle, 200, ID_DOCK_CLASSES1);	
	CDocker* pDockBottom = m_DockView.AddDockedChild(new CDockText, DS_DOCKED_BOTTOM | dwStyle, 100, ID_DOCK_TEXT1);

	// Add the remaining dockers
	//pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | dwStyle, 200, ID_DOCK_FILES1);
	//pDockRight->AddDockedChild(new CDockClasses, DS_DOCKED_CONTAINER | dwStyle, 200, ID_DOCK_CLASSES2);
	//pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | dwStyle, 200, ID_DOCK_FILES2);

	pDockBottom->AddDockedChild(new CDockOutput, DS_DOCKED_CONTAINER | dwStyle, 100, ID_DOCK_OUTPUT1);
	//pDockBottom->AddDockedChild(new CDockText, DS_DOCKED_CONTAINER | dwStyle, 100, ID_DOCK_TEXT2);
	//pDockBottom->AddDockedChild(new CDockOutput, DS_DOCKED_CONTAINER | dwStyle, 100, ID_DOCK_OUTPUT2);
}

void CMainFrame::PreCreate(CREATESTRUCT &cs)
{
	// Call the base class function first
	CFrame::PreCreate(cs);
	
	// Hide the window initially by removing the WS_VISIBLE style
	cs.style &= ~WS_VISIBLE;
}

void CMainFrame::SaveRegistrySettings()
{
	CFrame::SaveRegistrySettings();
	m_DockView.SaveRegistrySettings(GetRegistryKeyName());
}

void CMainFrame::SetupToolbar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolbarButton( IDM_FILE_NEW,   TRUE );
	AddToolbarButton( IDM_FILE_OPEN,  FALSE );
	AddToolbarButton( IDM_FILE_SAVE,  TRUE );
	
	AddToolbarButton( 0 );	// Separator
	AddToolbarButton( IDM_EDIT_CUT,   TRUE );
	AddToolbarButton( ID_SETTINGS,  TRUE );
	AddToolbarButton( IDM_EDIT_PASTE, TRUE );
	
	AddToolbarButton( 0 );	// Separator
	AddToolbarButton( IDM_FILE_PRINT, TRUE );
	
	AddToolbarButton( 0 );	// Separator
	AddToolbarButton( IDM_HELP_ABOUT );
}

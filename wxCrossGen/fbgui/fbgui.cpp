///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan 29 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "fbgui.h"

///////////////////////////////////////////////////////////////////////////

VMainFrame::VMainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 600,340 ), wxDefaultSize );
	
	mMenu = new wxMenuBar( 0 );
	miFile = new wxMenu();
	wxMenuItem* miOpenGrid;
	miOpenGrid = new wxMenuItem( miFile, wxID_OPEN, wxString( _("&Open grid") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	miFile->Append( miOpenGrid );
	
	wxMenuItem* miGenerate;
	miGenerate = new wxMenuItem( miFile, wxID_ANY, wxString( _("&Generate") ) , wxEmptyString, wxITEM_NORMAL );
	miFile->Append( miGenerate );
	
	wxMenuItem* miExit;
	miExit = new wxMenuItem( miFile, wxID_EXIT, wxString( _("E&xit") ) + wxT('\t') + wxT("Ctrl+Q"), wxEmptyString, wxITEM_NORMAL );
	miFile->Append( miExit );
	
	mMenu->Append( miFile, _("File") );
	
	miHelp = new wxMenu();
	wxMenuItem* miAbout;
	miAbout = new wxMenuItem( miHelp, wxID_ABOUT, wxString( _("&About") ) , wxEmptyString, wxITEM_NORMAL );
	miHelp->Append( miAbout );
	
	mMenu->Append( miHelp, _("Help") );
	
	this->SetMenuBar( mMenu );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	tPath = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( tPath, 1, wxEXPAND|wxLEFT|wxTOP, 5 );
	
	bPath = new wxButton( this, wxID_ANY, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer4->Add( bPath, 0, wxALL, 5 );
	
	bSizer2->Add( bSizer4, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	tOutput = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	bSizer3->Add( tOutput, 1, wxALL|wxEXPAND, 5 );
	
	bPreview = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( bPreview, 0, wxALL, 5 );
	
	bSizer2->Add( bSizer3, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bGenerate = new wxButton( this, wxID_ANY, _("Generate"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( bGenerate, 0, wxBOTTOM|wxRIGHT, 5 );
	
	bSizer2->Add( bSizer5, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer2 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( VMainFrame::onExitClick ) );
	this->Connect( miOpenGrid->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VMainFrame::onOpenGridClick ) );
	this->Connect( miGenerate->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VMainFrame::onGenerateClick ) );
	this->Connect( miExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VMainFrame::onExitClick ) );
	this->Connect( miAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VMainFrame::onAboutClick ) );
	bPath->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( VMainFrame::onOpenGridClick ), NULL, this );
	bGenerate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( VMainFrame::onGenerateClick ), NULL, this );
}

VMainFrame::~VMainFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( VMainFrame::onExitClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VMainFrame::onOpenGridClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VMainFrame::onGenerateClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VMainFrame::onExitClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VMainFrame::onAboutClick ) );
	bPath->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( VMainFrame::onOpenGridClick ), NULL, this );
	bGenerate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( VMainFrame::onGenerateClick ), NULL, this );
}
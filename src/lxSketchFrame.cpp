
#include "lxSketchFrame.h"

#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/artprov.h>
#include <wx/msgdlg.h>
#include <wx/stattext.h>
#include <wx/choice.h>

//#include <wx/stdpaths.h>
//#include <wx/filename.h>


#include "lxSelectCanvastool.h"
#include "lxLineCanvasTool.h"
#include "lxRectCanvasTool.h"
#include "lxEllipseCanvasTool.h"
#include "lxPolygonCanvasTool.h"
#include "lxScribbleCanvasTool.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

lxSketchFrame::lxSketchFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title)
{
    create_tool_bar();

#if wxUSE_MENUS
    create_menu_bar();
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(wxT("Hello Code::Blocks user!"),0);
    SetStatusText(wxbuildinfo(short_f), 1);
#endif // wxUSE_STATUSBAR

   	SetMinSize(wxSize(640,480));

   	m_canvas = new lxCanvas(this);
   	m_canvas->setFillColour( *wxRED );

    //Register Tools
   	m_canvas->registerTool<lxSelectCanvastool>("select");
    m_canvas->registerTool<lxLineCanvasTool>("line");
    m_canvas->registerTool<lxRectCanvasTool>("rect");
    m_canvas->registerTool<lxEllipseCanvasTool>("ellipse");
    m_canvas->registerTool<lxPolygonCanvasTool>("polygon");
    m_canvas->registerTool<lxScribbleCanvasTool>("scribble");
    m_canvas->setCurrentTool("select");
    //Register Object
    m_canvas->registerObject<lxLineCanvasObject>();
    m_canvas->registerObject<lxRectCanvasObject>();
    m_canvas->registerObject<lxEllipseCanvasObject>();
    m_canvas->registerObject<lxPolygonCanvasObject>();
    m_canvas->registerObject<lxScribbleCanvasObject>();

   	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
   	mainSizer->Add(m_canvas, 1, wxEXPAND);

   	this->SetAutoLayout(true);
   	this->SetSizer(mainSizer);

	Layout();

	update_menus();

    //Connect events
    this->Bind(wxEVT_CLOSE_WINDOW, &lxSketchFrame::OnClose, this);
    //File
    this->Bind(wxEVT_MENU, &lxSketchFrame::OnQuit, this, wxID_EXIT);
    //Edit
    this->Bind(wxEVT_MENU, &lxSketchFrame::OnUndo, this, wxID_UNDO);
    this->Bind(wxEVT_MENU, &lxSketchFrame::OnRedo, this, wxID_REDO);
    this->Bind(wxEVT_MENU, &lxSketchFrame::OnSelectAll, this, wxID_SELECTALL);
    this->Bind(wxEVT_MENU, &lxSketchFrame::OnDuplicate, this, wxID_DUPLICATE);
    this->Bind(wxEVT_MENU, &lxSketchFrame::OnDelete, this, wxID_CLEAR);
    //Tools
    this->Bind(wxEVT_MENU, &lxSketchFrame::OnChooseTool, this, idToolSelect);
    this->Bind(wxEVT_MENU, &lxSketchFrame::OnChooseTool, this, idToolLine);
    this->Bind(wxEVT_MENU, &lxSketchFrame::OnChooseTool, this, idToolPolygon);
    this->Bind(wxEVT_MENU, &lxSketchFrame::OnChooseTool, this, idToolScribble);
    this->Bind(wxEVT_MENU, &lxSketchFrame::OnChooseTool, this, idToolRect);
    this->Bind(wxEVT_MENU, &lxSketchFrame::OnChooseTool, this, idToolEllipse);
    this->Bind(wxEVT_MENU, &lxSketchFrame::OnChooseTool, this, idToolText);
    //Object
    this->Bind(wxEVT_MENU, &lxSketchFrame::OnMoveFoward,     this, idObjMoveFoward);
    this->Bind(wxEVT_MENU, &lxSketchFrame::OnMoveToFront,    this, idObjMoveToFront);
    this->Bind(wxEVT_MENU, &lxSketchFrame::OnMoveBackward,   this, idObjMoveBackward);
    this->Bind(wxEVT_MENU, &lxSketchFrame::OnMoveToBack,     this, idObjMoveToBack);
    //Help
    this->Bind(wxEVT_MENU, &lxSketchFrame::OnAbout, this, wxID_ABOUT);
    //Canvas
    m_canvas->Bind(lxEVT_CANVAS_CHANGE, &lxSketchFrame::OnCanvasChange, this );
}

lxSketchFrame::~lxSketchFrame()
{
}


//private
void lxSketchFrame::create_menu_bar()
{
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();

    //File
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(wxID_EXIT, wxT("&Quit\tAlt-F4"), wxT("Quit the application"));
    mbar->Append(fileMenu, wxT("&File"));
    //Edit
    wxMenu* editMenu = new wxMenu(_T(""));
    editMenu->Append(wxID_UNDO,         "Undo\tCtrl-Z",         "Undo");
    editMenu->Append(wxID_REDO,         "Redo\tCtrl-Y",         "Redo");
    editMenu->AppendSeparator();
    editMenu->Append(wxID_SELECTALL,    "Select All\tCtrl-A",   "Select All");
    editMenu->AppendSeparator();
    editMenu->Append(wxID_DUPLICATE,    "Duplicate\tCtrl-D",    "Duplicate");
    editMenu->Append(wxID_PROPERTIES,   "Edit...\tCtrl-E",      "Edit object properties");
    editMenu->Append(wxID_CLEAR,        "Delete\tDel",          "Delete");

    mbar->Append(editMenu, wxT("&Edit"));

    //Tools
    wxMenu* toolsMenu = new wxMenu(_T(""));
    toolsMenu->AppendRadioItem(idToolSelect,    wxT("Selection"),   wxT("Select Object"));
    toolsMenu->AppendRadioItem(idToolLine,      wxT("Line"),        wxT("Draw Line"));
    toolsMenu->AppendRadioItem(idToolPolygon,   wxT("Polygon"),     wxT("Draw Polygon"));
    toolsMenu->AppendRadioItem(idToolScribble,  wxT("Scribble"),    wxT("Draw Scribble"));
    toolsMenu->AppendRadioItem(idToolRect,      wxT("Rectangle"),   wxT("Draw Rectangle"));
    toolsMenu->AppendRadioItem(idToolEllipse,   wxT("Ellipse"),     wxT("Draw Ellipse"));
    toolsMenu->AppendRadioItem(idToolText,      wxT("Text"),        wxT("Draw Text"));
    mbar->Append(toolsMenu, wxT("&Tools"));

    //Object
    wxMenu* objMenu = new wxMenu(_T(""));
    objMenu->Append(idObjMoveFoward,    wxT("Move Foward"),             wxT("Move Foward"));
    objMenu->Append(idObjMoveToFront,   wxT("Move to Front\tCtrl-F"),   wxT("Move to Front"));
    objMenu->Append(idObjMoveBackward,  wxT("Move Backward"),           wxT("Move Backward"));
    objMenu->Append(idObjMoveToBack,    wxT("Move to Back\tCtrl-B"),    wxT("Move to Back"));
    mbar->Append(objMenu, wxT("&Object"));

    //Help
    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(wxID_ABOUT, wxT("&About\tF1"), wxT("Show info about this application"));
    mbar->Append(helpMenu, wxT("&Help"));

    SetMenuBar(mbar);

}

void lxSketchFrame::create_tool_bar()
{
    // Create a tool bar
    wxSize sz = wxSize(15,15);
    wxToolBar *toolbar  = CreateToolBar(wxTB_HORIZONTAL | wxTB_FLAT  );

    toolbar->AddTool( wxID_NEW,     "New",   wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR, sz) );
    toolbar->AddTool( wxID_OPEN,    "Open",  wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR, sz) );
    toolbar->AddTool( wxID_SAVE,    "Save",  wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR, sz) );
    toolbar->AddTool( wxID_SAVEAS,  "Save As...",    wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_TOOLBAR, sz) );
    //-------
    toolbar->AddSeparator();
    toolbar->AddTool( wxID_UNDO, "Undo",    wxArtProvider::GetBitmap(wxART_UNDO, wxART_TOOLBAR, sz) );
    toolbar->AddTool( wxID_REDO, "Redo",    wxArtProvider::GetBitmap(wxART_REDO, wxART_TOOLBAR, sz) );
    toolbar->AddSeparator();
    wxBitmap duplicate_bmp;
    duplicate_bmp.LoadFile("images/duplicate.bmp", wxBITMAP_TYPE_BMP);
    toolbar->AddTool( wxID_DUPLICATE, "Duplicate", duplicate_bmp );
    //-------
    toolbar->AddSeparator();
    wxBitmap moveFoward_bmp;
    moveFoward_bmp.LoadFile("images/moveForward.bmp", wxBITMAP_TYPE_BMP);
    toolbar->AddTool( idObjMoveFoward, "Move Forward", moveFoward_bmp );
    wxBitmap moveBack_bmp;
    moveBack_bmp.LoadFile("images/moveBack.bmp", wxBITMAP_TYPE_BMP);
    toolbar->AddTool( idObjMoveBackward, "Move Backward", moveBack_bmp );
    //-------
    const wxString sizes[] = {
        "no line",
        "1-pixel line",
        "2-pixel line",
        "3-pixel line",
        "4-pixel line",
        "5-pixel line"
    };
    wxColourPickerCtrl* penColourPicker  = new wxColourPickerCtrl(toolbar, wxID_ANY, *wxBLACK);
    wxColourPickerCtrl* fillColourPicker = new wxColourPickerCtrl(toolbar, wxID_ANY, *wxRED);
    wxChoice* lineSize = new wxChoice(toolbar, wxID_ANY, wxDefaultPosition, wxDefaultSize, 6, sizes);
    lineSize->SetSelection(1);

    toolbar->AddSeparator();
    toolbar->AddControl( lineSize );
    toolbar->AddControl( new wxStaticText(toolbar, wxID_ANY, "Pen:"));
    toolbar->AddControl( penColourPicker );
    toolbar->AddControl( new wxStaticText(toolbar, wxID_ANY, "Fill:"));
    toolbar->AddControl( fillColourPicker );

    toolbar->Realize();
    //events
    lineSize->Bind(wxEVT_CHOICE, &lxSketchFrame::OnLineSizeChanged, this);
    penColourPicker->Bind(wxEVT_COLOURPICKER_CHANGED, &lxSketchFrame::OnPenColourChanged, this);
    fillColourPicker->Bind(wxEVT_COLOURPICKER_CHANGED, &lxSketchFrame::OnFillColourChanged, this);
}

void lxSketchFrame::update_menus(){

    GetMenuBar()->Enable(wxID_UNDO, m_canvas->canUndo() );
    GetMenuBar()->Enable(wxID_REDO, m_canvas->canRedo() );
    GetMenuBar()->Enable(wxID_DUPLICATE, m_canvas->hasSelection() );
    GetMenuBar()->Enable(wxID_PROPERTIES, false );
    GetMenuBar()->Enable(wxID_CLEAR, m_canvas->hasSelection() );

    GetMenuBar()->Enable(idToolText, false );

    GetMenuBar()->Enable(idObjMoveFoward, m_canvas->canMoveFoward() );
    GetMenuBar()->Enable(idObjMoveToFront, m_canvas->canMoveFoward() );
    GetMenuBar()->Enable(idObjMoveBackward, m_canvas->canMoveBackward() );
    GetMenuBar()->Enable(idObjMoveToBack, m_canvas->canMoveBackward() );


    //ToolBar
    GetToolBar()->EnableTool(wxID_UNDO, m_canvas->canUndo() );
    GetToolBar()->EnableTool(wxID_REDO, m_canvas->canRedo() );
    GetToolBar()->EnableTool(wxID_DUPLICATE, m_canvas->hasSelection() );
    GetToolBar()->EnableTool(idObjMoveFoward, m_canvas->canMoveFoward() );
    GetToolBar()->EnableTool(idObjMoveBackward, m_canvas->canMoveBackward() );
}


//Events
void lxSketchFrame::OnClose(wxCloseEvent &event){
//    printf("Close...\n");
    Destroy();
}

void lxSketchFrame::OnQuit(wxCommandEvent &event){
    Destroy();
}
void lxSketchFrame::OnUndo(wxCommandEvent& event){
    m_canvas->doUndo();
}

void lxSketchFrame::OnRedo(wxCommandEvent& event){
    m_canvas->doRedo();
}

void lxSketchFrame::OnSelectAll(wxCommandEvent& event){
    m_canvas->selectAll();
}

void lxSketchFrame::OnDuplicate(wxCommandEvent& event){
    m_canvas->doDuplicate();
}

void lxSketchFrame::OnDelete(wxCommandEvent& event){
    m_canvas->doDelete();
}

void lxSketchFrame::OnChooseTool(wxCommandEvent& event)
{
    switch(event.GetId()){
        case idToolSelect:
            m_canvas->setCurrentTool("select");
            break;
        case idToolLine:
            m_canvas->setCurrentTool("line");
            break;
        case idToolPolygon:
            m_canvas->setCurrentTool("polygon");
            break;
        case idToolScribble:
            m_canvas->setCurrentTool("scribble");
            break;
        case idToolRect:
            m_canvas->setCurrentTool("rect");
            break;
        case idToolEllipse:
            m_canvas->setCurrentTool("ellipse");
            break;
        case idToolText:
            m_canvas->setCurrentTool("text");
            break;
    }
}

void lxSketchFrame::OnMoveFoward(wxCommandEvent& event){
    m_canvas->doMoveForward();
}

void lxSketchFrame::OnMoveToFront(wxCommandEvent& event){
    m_canvas->doMoveToFront();
}

void lxSketchFrame::OnMoveBackward(wxCommandEvent& event){
    m_canvas->doMoveBackward();
}

void lxSketchFrame::OnMoveToBack(wxCommandEvent& event){
    m_canvas->doMoveToBack();
}

void lxSketchFrame::OnCanvasChange(wxCommandEvent& event){
    update_menus();
}

void lxSketchFrame::OnAbout(wxCommandEvent &event){
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, wxT("Welcome to..."));
}

void lxSketchFrame::OnLineSizeChanged(wxCommandEvent& event){
    m_canvas->setLineSize(event.GetSelection());
}
void lxSketchFrame::OnPenColourChanged(wxColourPickerEvent& event){
    m_canvas->setPenColour( event.GetColour() );
}
void lxSketchFrame::OnFillColourChanged(wxColourPickerEvent& event){
    m_canvas->setFillColour( event.GetColour() );
}

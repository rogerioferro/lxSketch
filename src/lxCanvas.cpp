#include "lxCanvas.h"
#include "lxDC.h"

#include <wx/dcbuffer.h>
#include <wx/dcclient.h>
#include <wx/graphics.h>
#include <wx/dcgraph.h>

//Size of the drawing page, in pixels.
#define SCROLL_RATE 20
#define PAGE_WIDTH  1000
#define PAGE_HEIGHT 1000

wxDEFINE_EVENT(lxEVT_CANVAS_CHANGE, wxCommandEvent);

lxCanvas::lxCanvas(wxWindow *parent)
    : wxScrolledWindow(parent)
{
    //ctor

    this->SetWindowStyle( wxSUNKEN_BORDER );

    this->SetBackgroundColour(*wxWHITE);
    this->EnableScrolling(true, true);
    this->SetScrollbars(1, 1, PAGE_WIDTH, PAGE_HEIGHT);

    //Connect Events
    this->Bind(wxEVT_IDLE,              &lxCanvas::OnIdle,               this);
    this->Bind(wxEVT_TIMER,             &lxCanvas::OnTimerDraw,          this);
    this->Bind(wxEVT_SIZE,              &lxCanvas::OnSize,               this);
    this->Bind(wxEVT_PAINT,             &lxCanvas::OnPaint,              this);
    this->Bind(wxEVT_ERASE_BACKGROUND,  &lxCanvas::OnEraseBackground,    this);

    //Scroll Events
    this->Bind(wxEVT_SCROLLWIN_TOP,             &lxCanvas::OnPanelScroll,        this);
    this->Bind(wxEVT_SCROLLWIN_BOTTOM,          &lxCanvas::OnPanelScroll,        this);
    this->Bind(wxEVT_SCROLLWIN_PAGEUP,          &lxCanvas::OnPanelScroll,        this);
    this->Bind(wxEVT_SCROLLWIN_PAGEDOWN,        &lxCanvas::OnPanelScroll,        this);
    this->Bind(wxEVT_SCROLLWIN_LINEUP,          &lxCanvas::OnPanelScroll,        this);
    this->Bind(wxEVT_SCROLLWIN_LINEDOWN,        &lxCanvas::OnPanelScroll,        this);
    this->Bind(wxEVT_SCROLLWIN_THUMBTRACK,      &lxCanvas::OnPanelScroll,        this);
    this->Bind(wxEVT_SCROLLWIN_THUMBRELEASE,    &lxCanvas::OnPanelScroll,        this);

    //Mouse Events
    this->Bind(wxEVT_MOTION,            &lxCanvas::OnMouseMove,          this);
    this->Bind(wxEVT_LEFT_DOWN,         &lxCanvas::OnMouseLeftDown,      this);
    this->Bind(wxEVT_LEFT_UP,           &lxCanvas::OnMouseLeftUp,        this);
    this->Bind(wxEVT_LEFT_DCLICK,       &lxCanvas::OnMouseLeftDClick,    this);

/**
    Start the background redraw timer
    This is optional, but it gives the double-buffered contents a
    chance to redraw even when idle events are disabled (like during
     resize and scrolling)
**/
    //Timer
    m_redrawTimer = new wxTimer();
    m_redrawTimer->SetOwner(this, 0);
    m_redrawTimer->Start(50);
}

lxCanvas::~lxCanvas(){
    //dtor
    m_redrawTimer->Stop();
}

//Public
void lxCanvas::requestRedraw(){
/**
    Requests a redraw of the drawing panel contents.
    The actual redrawing doesn't happen until the next idle time.
**/
    m_reInitBuffer = true;
}

void lxCanvas::addObject(std::unique_ptr<lxCanvasObject> obj, bool select){
/**
    Add a new drawing object to the canvas.
    If select is True then also select the object
**/
    if(!obj) return;
    saveUndoInfo();
    //self.dirty = True
    if(select) selectObject(obj.get());
    //setCurrentTool("select");
    m_contents.push_front(std::move(obj));
}

bool lxCanvas::setCurrentTool(const wxString& toolName){
/** Set the currently selected tool. **/
    auto it = m_tools.find(toolName);
    if( it != m_tools.end() ){
        if(!it->second) return false;
        m_curToolName   = it->first;
        m_curTool       = it->second.get();
        SetCursor(m_curTool->getDefaultCursor());
        return true;
    }
    return false;
}

void lxCanvas::selectAll(){
    for(std::unique_ptr<lxCanvasObject> &obj : m_contents ){
        m_selection.insert(obj.get());
    }
    _changed();
    requestRedraw();
}

void lxCanvas::deselectAll(){
    if(m_selection.empty()) return;
    m_selection.clear();
    _changed();
    requestRedraw();
}

void lxCanvas::selectMany(std::set<lxCanvasObject*> objs){
    m_selection.swap(objs);
    _changed();
    requestRedraw();
}

void lxCanvas::selectObject(lxCanvasObject* obj, bool add){
/**
    Select the given DrawingObject within our document.
    If 'add' is true obj is added onto the current selection
**/
    if(!obj) return;
    if(!add) m_selection.clear();
    auto ib = m_selection.insert(obj);
    if( ib.second ){
        _changed();
        requestRedraw();
    }
}
void lxCanvas::selectByRectangle(const lxRect& rect){
/** Select every DrawingObject in the given rectangular region. **/
    m_selection.clear();
    for(std::unique_ptr<lxCanvasObject> &obj : m_contents ){
        if( obj->withinRect(rect) ){
            m_selection.insert(obj.get());
        }
    }
    _changed();
    requestRedraw();
}

lxCanvasObject::Handle
lxCanvas::getSelectionHandleAt(const lxPoint& pt){
/**
    Return the object and selection handle at the given point.
    If the given point is within one of the selection handle rectangles,
    we return the associated object and a code indicating which selection
    handle the point is in.
**/
    lxCanvasObject::Handle handle;
    for(lxCanvasObject* obj : m_selection){
        handle = obj->getSelectionHandleContainingPoint(pt);
        if( handle.isOk() ) break;
    }
    return handle;
}
lxCanvasObject* lxCanvas::getObjectAt(const lxPoint& pt){
/** Return the first object found which is at the given point. **/
    for(std::unique_ptr<lxCanvasObject> &obj : m_contents){
        if( obj->containsPoint(pt) ) return obj.get();
    }
    return nullptr;
}


bool lxCanvas::canSave(){
    return false;
}

bool lxCanvas::canRevert(){
    return false;
}

bool lxCanvas::canUndo(){
    return !m_undoStack.empty();
}

bool lxCanvas::canRedo(){
    return !m_redoStack.empty();
}

bool lxCanvas::hasSelection(){
    return !m_selection.empty();
}

bool lxCanvas::canMoveFoward(){
    return m_selection.size() == 1 && !( *m_selection.begin() == m_contents.front().get() );
}

bool lxCanvas::canMoveBackward(){
    return m_selection.size() == 1 && !( *m_selection.begin() == m_contents.back().get() );
}


void lxCanvas::saveUndoInfo(){
/** Remember the current state of the document, to allow for undo.
    We make a copy of the document's contents, so that we can return to
    the previous contents if the user does something and then wants to
    undo the operation.
    This should be called only for a new modification to the document
    since it erases the redo history.
**/
    auto state = _buildStoredState();
    m_undoStack.push_back(state);
    m_redoStack.clear();

    //test
//    printf("\n\n<----");
//    for( auto &state : m_undoStack){
//        for(auto &cont : state){
//            printf("name = ");
//            printf(cont.type.mb_str()); printf("\n");
//            printf(cont.data.mb_str()); printf("\n");
//        }
//        printf("-----------------\n");
//    }
//    printf("---->\n\n");

//    self.dirty = True
}

void lxCanvas::doUndo(){
    if( m_undoStack.empty() ) return;
    m_redoStack.push_back( _buildStoredState() );
    _restoreStoredState( m_undoStack.back() );
    m_undoStack.pop_back();

//    printf("\n\n<----\n");
//    for( auto &state : m_redoStack){
//        for(auto &cont : state){
//            printf("type = ");
//            printf(cont.type.mb_str()); printf("\n");
//            printf(cont.data.mb_str()); printf("\n");
//            printf("\n");
//        }
//        printf("-----------------\n");
//    }
//    printf("---->\n\n");

}

void lxCanvas::doRedo(){
    if( m_redoStack.empty() ) return;
    m_undoStack.push_back( _buildStoredState() );
    _restoreStoredState( m_redoStack.back() );
    m_redoStack.pop_back();
}

void lxCanvas::doDuplicate(){
    if(m_selection.empty()) return;
    saveUndoInfo();
    std::set<lxCanvasObject*> objs;
    for(lxCanvasObject *obj : m_selection){
        auto it = m_factory.find(obj->getClassName());
        if(it == m_factory.end()) continue; //can't create object
        std::unique_ptr<lxCanvasObject> dup = (*it->second)();
        dup->setData(obj->getData());
        lxPoint pt = dup->getPosition() + lxPoint(10,10);
        dup->setPosition(pt);
        objs.insert(dup.get());
        m_contents.push_front(std::move(dup));
    }
    selectMany(objs);
}

void lxCanvas::doDelete(){
    if(m_selection.empty()) return;
    saveUndoInfo();
    auto it = m_contents.begin();
    while(it != m_contents.end()){
        if( m_selection.count(it->get()) ){
            it = m_contents.erase(it);
        }else{
            it++;
        }
    }
    deselectAll();
}

void lxCanvas::doMoveForward(){
    if(m_selection.size() != 1) return;

    lxCanvasObject* sel =  *m_selection.begin();
    std::list<std::unique_ptr<lxCanvasObject>>::iterator it;
    for(it = m_contents.begin(); it != m_contents.end(); it++){
        lxCanvasObject* obj =  it->get();
        if(obj == sel){
            if( it == m_contents.begin() ) break;
            saveUndoInfo();
            m_contents.splice(std::prev(it), m_contents, it);
            _changed();
            requestRedraw();
            break;
        }
    }
}

void lxCanvas::doMoveToFront(){
    if(m_selection.size() != 1) return;

    lxCanvasObject* sel =  *m_selection.begin();
    std::list<std::unique_ptr<lxCanvasObject>>::iterator it;
    for(it = m_contents.begin(); it != m_contents.end(); it++){
        lxCanvasObject* obj =  it->get();
        if(obj == sel){
            saveUndoInfo();
            m_contents.splice(m_contents.begin(), m_contents, it);
            _changed();
            requestRedraw();
            break;
        }
    }
}

void lxCanvas::doMoveBackward(){
    if(m_selection.size() != 1) return;

    lxCanvasObject* sel =  *m_selection.begin();
    std::list<std::unique_ptr<lxCanvasObject>>::iterator it;
    for(it = m_contents.begin(); it != m_contents.end(); it++){
        lxCanvasObject* obj =  it->get();
        if(obj == sel){
            if( it == std::prev(m_contents.end()) ) break;
            saveUndoInfo();
            m_contents.splice(std::next(it,2), m_contents, it);
            _changed();
            requestRedraw();
            break;
        }
    }
}

void lxCanvas::doMoveToBack(){
    if(m_selection.size() != 1) return;

    lxCanvasObject* sel =  *m_selection.begin();
    std::list<std::unique_ptr<lxCanvasObject>>::iterator it;
    for(it = m_contents.begin(); it != m_contents.end(); it++){
        lxCanvasObject* obj =  it->get();
        if(obj == sel){
            saveUndoInfo();
            m_contents.splice(m_contents.end(), m_contents, it);
            _changed();
            requestRedraw();
            break;
        }
    }
}

void lxCanvas::setPenColour(const wxColour& colour){
    if(!m_selection.empty()){
        saveUndoInfo();
        for(lxCanvasObject* obj : m_selection){
            obj->setPenColour(colour);
        }
        requestRedraw();
    }
    m_penColour = colour;
}
void lxCanvas::setFillColour(const wxColour& colour){
    if(!m_selection.empty()){
        saveUndoInfo();
        for(lxCanvasObject* obj : m_selection){
            obj->setFillColour(colour);
        }
        requestRedraw();
    }
    m_fillColour = colour;
}
void lxCanvas::setLineSize(double lineSize){
    if(!m_selection.empty()){
        saveUndoInfo();
        for(lxCanvasObject* obj : m_selection){
            obj->setLineSize(lineSize);
        }
        requestRedraw();
    }
    m_lineSize = lineSize;
}


//Private
void lxCanvas::_initBuffer(){
/**
    Initialize the bitmap used for buffering the display.
**/
    wxSize sz = this->GetSize();
    if( sz != m_buffer.GetSize() ){
        m_buffer = wxBitmap(std::max(1, sz.GetWidth()), std::max(1, sz.GetHeight()));
    }
    wxBufferedDC dc(nullptr, m_buffer);
    dc.SetBackground(wxBrush(GetBackgroundColour()));
    dc.Clear();
    _drawContents();
    m_reInitBuffer  = false;
    m_resize        = false;
}

void lxCanvas::_drawContents(){
    wxGCDC  gdc(m_buffer);
    PrepareDC(gdc);
    lxDC    dc(gdc);

    //First draw objects from back to front
    std::list<lxCanvasObject*> ordered_selection;
    std::list<std::unique_ptr<lxCanvasObject>>::reverse_iterator it;
    for(it = m_contents.rbegin(); it != m_contents.rend(); it++){
        lxCanvasObject *obj = (*it).get();
        if(m_selection.find(obj) != m_selection.end()){
            ordered_selection.push_back(obj);
            obj->draw(dc, true);
        }else{
            obj->draw(dc, false);
        }
    }

    //Draw tool
    if(m_curTool) m_curTool->draw(dc);

    //Draw Selection
    for(lxCanvasObject *obj : ordered_selection){
        obj->drawHandles(dc);
    }
}

void lxCanvas::_redraw(){
    if( m_reInitBuffer && this->IsShown() ){
        _initBuffer();
        this->Refresh(false);
    }
}

void lxCanvas::_changed(){
    //Notify Change
    wxCommandEvent event(lxEVT_CANVAS_CHANGE); // No specific id
    event.SetEventObject( this );
    // Then post the event
    wxPostEvent(this, event); // to parent
}

std::vector<lxCanvas::ObjState> lxCanvas::_buildStoredState(){
/** Remember the current state of the document, to allow for undo.
    We make a copy of the document's contents, so that we can return to
    the previous contents if the user does something and then wants to
    undo the operation.
    Returns an object representing the current document state.
**/
    std::vector<ObjState> savedContents;
    for(std::unique_ptr<lxCanvasObject> &obj : m_contents ){
        savedContents.emplace_back(
            obj->getClassName(), obj->getData(),
            m_selection.find(obj.get()) != m_selection.end() );
    }
    return savedContents;
}

void lxCanvas::_restoreStoredState(const std::vector<ObjState> &savedState){
/** Restore the state of the document to a previous point for undo/redo.
    Takes a stored state object and recreates the document from it.
    Used by undo/redo implementation.
**/
    m_contents.clear();
    m_selection.clear();
    for(const ObjState &state : savedState){
        auto it = m_factory.find(state.type);
        if(it == m_factory.end()) continue; //can't create object
        std::unique_ptr<lxCanvasObject> obj = (*it->second)();
        obj->setData(state.data);
        if(state.selected) m_selection.insert(obj.get());
        m_contents.push_back(std::move(obj));
    }
    //self.dirty = True
    _changed();
    requestRedraw();
}


//Events
void lxCanvas::OnIdle(wxIdleEvent& event){
/**
    If the size was changed then resize the bitmap used for double
    buffering to match the window size.  We do it in Idle time so
    there is only one refresh after resizing is done, not lots while
    it is happening.
**/
    _redraw();
}

void lxCanvas::OnTimerDraw(wxTimerEvent& event){
    _redraw();
}

void lxCanvas::OnSize(wxSizeEvent& event){
/**
    Called when the window is resized.  We set a flag so the idle
    handler will resize the buffer.
**/
    m_resize = true;
    this->requestRedraw();
}

void lxCanvas::OnPaint(wxPaintEvent& event)
{
    // Create a buffered paint DC.  It will create the real
    // wxPaintDC and then blit the bitmap to it when dc is
    // deleted.
    wxBufferedPaintDC bdc(this, m_buffer);

#if defined(__WXMSW__)
/**
    On Windows, if that's all we do things look a little rough
    So in order to make scrolling more polished-looking
    we iterate over the exposed regions and fill in unknown
    areas with a fall-back pattern.
**/
    if( !m_resize ) return;
    wxClientDC dc(this);
    wxRegion &region = GetUpdateRegion();

    // Now iterate over the remaining region rects and fill in with a pattern
    wxRegionIterator rgn_iter(region);
    if( rgn_iter.HaveRects() ){
        static wxBrush backgroundFillBrush;
        if( !backgroundFillBrush.IsOk() ){
            wxBitmap stippleBitmap = wxBitmap("\x06",2,2);
            stippleBitmap.SetMask(new wxMask(stippleBitmap));
            backgroundFillBrush = wxBrush(*wxWHITE, wxBRUSHSTYLE_STIPPLE_MASK_OPAQUE);
            backgroundFillBrush.SetStipple(stippleBitmap);
        }
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(backgroundFillBrush);
        dc.SetTextForeground(*wxLIGHT_GREY);
        dc.SetTextBackground(*wxWHITE);
    }
    while( rgn_iter ){
        dc.DrawRectangle(rgn_iter.GetRect());
        rgn_iter++;
    }
#endif // defined
}

void lxCanvas::OnEraseBackground(wxEraseEvent& event){
/**
    Overridden to do nothing to prevent flicker
**/
}

void lxCanvas::OnPanelScroll(wxScrollWinEvent& event){
/**
    make a note to ourselves to redraw when we get a chance
**/
    wxEventType type = event.GetEventType();
    if( type == wxEVT_SCROLLWIN_LINEUP || type == wxEVT_SCROLLWIN_LINEDOWN ){
        int orientation = event.GetOrientation();
        int pos         = GetScrollPos(orientation);
        if( type == wxEVT_SCROLLWIN_LINEUP ){
            pos -= SCROLL_RATE;
        }else{
            pos += SCROLL_RATE;
        }
        if(event.GetOrientation() == wxVERTICAL){
            Scroll(GetScrollPos(wxHORIZONTAL), pos);
        }else{
            Scroll(pos, GetScrollPos(wxVERTICAL));
        }
    }
    requestRedraw();
    event.Skip();
}

//Mouse Events
void lxCanvas::OnMouseLeftDown(wxMouseEvent& event){
    if(!m_curTool) return;
    CalcUnscrolledPosition(event.m_x, event.m_y, &event.m_x, &event.m_y);
    if( m_curTool->OnMouseLeftDown(this, event) ) return;
    //otherwise handle it ourselves
}
void lxCanvas::OnMouseMove(wxMouseEvent& event){
    if(!m_curTool) return;
    CalcUnscrolledPosition(event.m_x, event.m_y, &event.m_x, &event.m_y);
    if( m_curTool->OnMouseMove(this, event) ) return;
    //otherwise handle it ourselves
}
void lxCanvas::OnMouseLeftUp(wxMouseEvent& event){
    if(!m_curTool) return;
    CalcUnscrolledPosition(event.m_x, event.m_y, &event.m_x, &event.m_y);
    if( m_curTool->OnMouseLeftUp(this, event) ) return;
    //otherwise handle it ourselves
}
void lxCanvas::OnMouseLeftDClick(wxMouseEvent& event){
    if(!m_curTool) return;
    CalcUnscrolledPosition(event.m_x, event.m_y, &event.m_x, &event.m_y);
    if( m_curTool->OnMouseLeftDClick(this, event) ) return;
    //otherwise handle it ourselves
}

#include "lxCanvasObject.h"
#include <wx/dcmemory.h>

lxCanvasObject::lxCanvasObject(){
    //ctor
}

lxCanvasObject::~lxCanvasObject(){
    //dtor
}

void lxCanvasObject::writeMembers(lxWriter& writer){

    writer.Key("m_position");
    m_position.writeData(writer);

    writer.Key("m_size");
    m_size.writeData(writer);

    writer.Key("m_penColour");
    writer.String(m_penColour.GetAsString(wxC2S_HTML_SYNTAX).mbc_str());

    writer.Key("m_fillColour");
    writer.String(m_fillColour.GetAsString(wxC2S_HTML_SYNTAX).mbc_str());

    writer.Key("m_lineSize");
    writer.Double(m_lineSize);
}

void lxCanvasObject::parseMembers(lxParse& parser){

    m_position.parseData( parser["m_position"] );

    m_size.parseData( parser["m_size"] );

    lxParse &pen = parser["m_penColour"];
    if( pen.IsString() ) m_penColour = wxColour(pen.GetString());

    lxParse &fil = parser["m_fillColour"];
    if( fil.IsString() ) m_fillColour = wxColour(fil.GetString());

    lxParse &lin = parser["m_lineSize"];
    if( lin.IsDouble() ) m_lineSize = lin.GetDouble();
}

bool lxCanvasObject::hasPropertyEditor(){
    return false;
}

void lxCanvasObject::doPropertyEdit(){
    wxASSERT_MSG(false, "Must be overridden if hasPropertyEditor returns True");
}

void lxCanvasObject::setPosition(const lxPoint& position){
    m_position = position;
}

void lxCanvasObject::setSize(const lxSize& size){
    m_size = size;
}

void lxCanvasObject::setPenColour(const wxColour& colour){
    m_penColour = colour;
}

void lxCanvasObject::setFillColour(const wxColour& colour){
    m_fillColour = colour;
}

void lxCanvasObject::setLineSize(double lineSize){
    m_lineSize = lineSize;
}

void lxCanvasObject::draw(lxDC& dc, bool selected){
/**
    Draw this DrawingObject into our window.

    'dc' is the device context to use for drawing.

    If 'selected' is True, the object is currently selected.
    Drawing objects can use this to change the way selected objects
    are drawn, however the actual drawing of selection handles
    should be done in the 'drawHandles' method
**/

    if(m_lineSize == 0.0){
        dc.SetPen(*wxTRANSPARENT_PEN);
    }else{
        dc.SetPen(wxPen(m_penColour, m_lineSize));
    }
    dc.SetBrush(wxBrush(m_fillColour));

    drawObject(dc, m_position, selected);
}

void lxCanvasObject::drawHandles(lxDC& dc){
/** Draw selection handles for this DrawingObject **/

    // Default is to draw selection handles at all four corners.
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxBLACK_BRUSH);

    lxRect r(m_position, m_size);
    drawSelHandle(dc, r.GetLeftTop() );
    drawSelHandle(dc, r.GetRightTop() );
    drawSelHandle(dc, r.GetLeftBottom() );
    drawSelHandle(dc, r.GetRightBottom() );
}

bool lxCanvasObject::containsPoint(const lxPoint& point){
/**
    Returns True if this object contains the given point.
    This is used to determine if the user clicked on the object.
**/
    // Firstly, ignore any points outside of the object's bounds.

    if(point.m_x < m_position.m_x)                      return false;
    if(point.m_x > m_position.m_x + m_size.GetWidth())  return false;
    if(point.m_y < m_position.m_y)                      return false;
    if(point.m_y > m_position.m_y + m_size.GetHeight()) return false;

    //Now things get tricky.  There's no straightforward way of
    //knowing whether the point is within an arbitrary object's
    //bounds...to get around this, we draw the object into a
    //memory-based bitmap and see if the given point was drawn.
    //This could no doubt be done more efficiently by some tricky
    //maths, but this approach works and is simple enough.

    //Subclasses can implement smarter faster versions of this.

    wxBitmap bitmap( m_size.m_x + 10, m_size.m_y + 10 );
    wxMemoryDC dc;
    dc.SelectObject(bitmap);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetPen(wxPen(*wxBLACK, m_lineSize + 5));
    dc.SetBrush(*wxBLACK_BRUSH);
    lxDC ldc(dc);
    drawObject(ldc, wxPoint(5, 5), true);
    wxColour pixel;
    wxPoint pt = lxRound( point - m_position + lxPoint(5,5) );
    dc.GetPixel( pt, &pixel );
    if( (pixel.Red() == 0) && (pixel.Green() == 0) && (pixel.Blue() == 0) )
        return true;
    else
        return false;
}

lxCanvasObject::Handle lxCanvasObject::getSelectionHandleContainingPoint(const lxPoint& point){
/**
    Return the selection handle containing the given point, if any.
    We return one of the predefined selection handle ID codes.
**/
    // Default implementation assumes selection handles at all four box corners.
    // Return a list so we can modify the contents later in moveHandle()
    lxRect r(m_position, m_size);
    if( pointInSelHandle( point, r.GetLeftTop() ) )     return Handle(this, m_handle_LEFT_TOP);
    if( pointInSelHandle( point, r.GetRightTop() ) )    return Handle(this, m_handle_RIGHT_TOP);
    if( pointInSelHandle( point, r.GetLeftBottom() ) )  return Handle(this, m_handle_LEFT_BOTTOM);
    if( pointInSelHandle( point, r.GetRightBottom() ) ) return Handle(this, m_handle_RIGHT_BOTTOM);
    return Handle();
}

void lxCanvasObject::moveHandle(Handle& handle, const lxPoint& point){
/**
   Move the specified selection handle to given canvas location.
**/
    if( handle.getObject() != this ) return;

    // Default implementation assumes selection handles at all four box corners.
    lxRect r(m_position, m_size);
    switch(handle.getValue()){
        case m_handle_LEFT_TOP:
            r.SetLeftTop(point);
            break;
        case m_handle_RIGHT_TOP:
            r.SetRightTop(point);
            break;
        case m_handle_LEFT_BOTTOM:
            r.SetLeftBottom(point);
            break;
        case m_handle_RIGHT_BOTTOM:
            r.SetRightBottom(point);
            break;
    }

    // Finally, normalize so no negative widths or heights.
    // And update the handle variable accordingly.
    if( r.m_height < 0.0 ){
        r.m_y += r.m_height;
        r.m_height = - r.m_height;
        switch(handle.getValue()){
            case m_handle_LEFT_TOP:     handle = Handle(this, m_handle_LEFT_BOTTOM);    break;
            case m_handle_RIGHT_TOP:    handle = Handle(this, m_handle_RIGHT_BOTTOM);   break;
            case m_handle_LEFT_BOTTOM:  handle = Handle(this, m_handle_LEFT_TOP);       break;
            case m_handle_RIGHT_BOTTOM: handle = Handle(this, m_handle_RIGHT_TOP);      break;
        }
    }
    if( r.m_width < 0.0 ){
        r.m_x += r.m_width;
        r.m_width = -r.m_width;
        switch(handle.getValue()){
            case m_handle_LEFT_TOP:     handle = Handle(this, m_handle_RIGHT_TOP);      break;
            case m_handle_RIGHT_TOP:    handle = Handle(this, m_handle_LEFT_TOP);       break;
            case m_handle_LEFT_BOTTOM:  handle = Handle(this, m_handle_RIGHT_BOTTOM);   break;
            case m_handle_RIGHT_BOTTOM: handle = Handle(this, m_handle_LEFT_BOTTOM);    break;
        }
    }
    m_position  = r.GetPosition();
    m_size      = r.GetSize();
}

void lxCanvasObject::finalizeHandle(const Handle& handle, const lxPoint& point){
}

bool lxCanvasObject::withinRect(const lxRect& rect){
/**
   Return True if this object falls completely within the given rect.
**/
    return rect.Contains( lxRect(m_position, m_size) );
}

//protected:

void lxCanvasObject::drawSelHandle(lxDC& dc, const lxPoint& handlePoint){
/**
    Draw a selection handle around this DrawingObject.
    'dc' is the device context to draw the selection handle within,
    while handlePoint are the coordinates to use for the center of the
    selection handle.
**/
    lxRect r( handlePoint - lxPoint(3,3), lxSize(6,6) );
    dc.DrawRectangle(r);
}

bool lxCanvasObject::pointInSelHandle(const lxPoint& point, const lxPoint& handlePoint){
/**
    Return True if (x, y) is within the selection handle at handlePoint.
**/
    lxRect r( handlePoint - lxPoint(3,3), lxSize(6,6) );
    return r.Contains(point);
}

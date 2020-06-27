#include "lxLineCanvasObject.h"

lxLineCanvasObject::lxLineCanvasObject()
{
    //ctor
}

lxLineCanvasObject::~lxLineCanvasObject()
{
    //dtor
//    printf("Destroy lxLineCanvasObject.\n");
}

void lxLineCanvasObject::writeMembers(lxWriter& writer){
    lxCanvasObject::writeMembers(writer);
    writer.Key("m_startPt");
    m_startPt.writeData(writer);
    writer.Key("m_endPt");
    m_endPt.writeData(writer);

}

void lxLineCanvasObject::parseMembers(lxParse& parser){
    lxCanvasObject::parseMembers(parser);
    m_startPt.parseData( parser["m_startPt"] );
    m_endPt.parseData( parser["m_endPt"] );
}

void lxLineCanvasObject::drawObject(lxDC& dc, const lxPoint& position, bool selected){
    dc.DrawLine(position + m_startPt, position + m_endPt);
}

void lxLineCanvasObject::drawHandles(lxDC& dc){
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxBLACK_BRUSH);

    //Draw selection handles at the start and end points.
    drawSelHandle(dc, m_position + m_startPt);
    drawSelHandle(dc, m_position + m_endPt);
}

lxCanvasObject::Handle lxLineCanvasObject::getSelectionHandleContainingPoint(const lxPoint& point){
    // We have selection handles at the start and end points.
    if( pointInSelHandle(point, m_position + m_startPt) )   return Handle(this, m_handle_START_POINT);
    if( pointInSelHandle(point, m_position + m_endPt) )     return Handle(this, m_handle_END_POINT);
    return Handle();
}

void lxLineCanvasObject::moveHandle(Handle& handle, const lxPoint& point){
    if( handle.getObject() != this ) return;

    lxPoint ptTrans = point - m_position;
    switch(handle.getValue()){
        case m_handle_START_POINT:
            m_startPt = ptTrans;
            break;
        case m_handle_END_POINT:
            m_endPt = ptTrans;
            break;
    }
    _updateBoundingBox();
}

void lxLineCanvasObject::setStartPt(const lxPoint& startPt){
    m_startPt = startPt - m_position;
    _updateBoundingBox();
}

lxPoint lxLineCanvasObject::getStartPt(){
    return m_startPt + m_position;
}

void lxLineCanvasObject::setEndPt(const lxPoint& endPt){
    m_endPt = endPt - m_position;
    _updateBoundingBox();
}

lxPoint lxLineCanvasObject::getEndPt(){
    return m_endPt + m_position;
}

void lxLineCanvasObject::_updateBoundingBox(){
    double x0 = std::min( m_endPt.m_x, m_startPt.m_x );
    double x1 = std::max( m_endPt.m_x, m_startPt.m_x );
    double y0 = std::min( m_endPt.m_y, m_startPt.m_y );
    double y1 = std::max( m_endPt.m_y, m_startPt.m_y );

    lxPoint dp = lxPoint(-x0,-y0);

    m_position.m_x  += x0;
    m_position.m_y  += y0;
    m_size.m_x      = x1-x0;
    m_size.m_y      = y1-y0;

    m_startPt   += dp;
    m_endPt     += dp;
}

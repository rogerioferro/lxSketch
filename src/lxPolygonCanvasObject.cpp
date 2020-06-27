#include "lxPolygonCanvasObject.h"

lxPolygonCanvasObject::lxPolygonCanvasObject()
{
    //ctor
}

lxPolygonCanvasObject::~lxPolygonCanvasObject()
{
    //dtor
}

void lxPolygonCanvasObject::drawObject(lxDC& dc, const lxPoint& position, bool selected){
    dc.DrawPolygon(m_points, position);
}

void lxPolygonCanvasObject::drawHandles(lxDC& dc){
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxBLACK_BRUSH);

    for(lxPoint &pt : m_points){
        drawSelHandle(dc, m_position + pt);
    }
}

lxCanvasObject::Handle lxPolygonCanvasObject::getSelectionHandleContainingPoint(const lxPoint& point){
    int i = 1;
    for(lxPoint &pt : m_points){
        if( pointInSelHandle(point, m_position + pt) ) return Handle(this, i);
        i++;
    }
    return Handle();
}

void lxPolygonCanvasObject::moveHandle(Handle& handle, const lxPoint& point){
    if( handle.getObject() != this ) return;
    movePoint( handle.getValue()-1, point);
}


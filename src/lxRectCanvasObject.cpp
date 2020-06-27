#include "lxRectCanvasObject.h"

lxRectCanvasObject::lxRectCanvasObject(){
    //ctor
}

lxRectCanvasObject::~lxRectCanvasObject(){
    //dtor
}

//Methods
bool lxRectCanvasObject::containsPoint(const lxPoint& point){
/**
    Returns True if this object contains the given point.
    This is used to determine if the user clicked on the object.
**/
    return lxRect(m_position, m_size).Contains(point);
}

void lxRectCanvasObject::drawObject(lxDC& dc, const lxPoint& position, bool selected){
/**
    Private routine to draw this DrawingObject.
    'dc' is the device context to use for drawing, while 'position' is
    the position in which to draw the object.  If 'selected' is True,
    the object is drawn with selection handles.  This private drawing
    routine assumes that the pen and brush have already been set by the
    caller.
**/
    dc.DrawRectangle(position, m_size);
}

#include "lxEllipseCanvasObject.h"

lxEllipseCanvasObject::lxEllipseCanvasObject()
{
    //ctor
}

lxEllipseCanvasObject::~lxEllipseCanvasObject()
{
    //dtor
}

void lxEllipseCanvasObject::drawObject(lxDC& dc, const lxPoint& position, bool selected){
    dc.DrawEllipse(position, m_size);
}

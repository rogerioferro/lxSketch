#include "lxCurveCanvasTool.h"
#include "lxCanvas.h"

lxCurveCanvastool::lxCurveCanvastool(){
    //ctor
}

lxCurveCanvastool::~lxCurveCanvastool(){
    //dtor
}

void lxCurveCanvastool::draw(lxDC& dc){
    if(!m_newObject) return;
    m_newObject->draw(dc, true);
}

bool lxCurveCanvastool::OnMouseLeftDown(lxCanvas* canvas, wxMouseEvent& event){
    m_startPt = event.GetPosition();
    m_newObject.reset();
    event.Skip();
    return true;
}

bool lxCurveCanvastool::OnMouseMove(lxCanvas* canvas, wxMouseEvent& event){
    if( !event.Dragging() ) return false;

    if(!m_newObject){
        m_newObject = createObject();
        m_newObject->setPenColour( canvas->getPenColour() );
        m_newObject->setFillColour( canvas->getFillColour() );
        m_newObject->setLineSize( canvas ->getLineSize() );
    }
    updateObject(event.GetPosition());
    canvas->requestRedraw();
    event.Skip();
    return true;
}

bool lxCurveCanvastool::OnMouseLeftUp(lxCanvas* canvas, wxMouseEvent& event){
    if(!m_newObject) return false;
    updateObject(event.GetPosition());
    canvas->addObject(std::move(m_newObject));
    event.Skip();
    return true;
}

void lxCurveCanvastool::updateObject(const lxPoint& pt){
    double x0 = std::min( pt.m_x, m_startPt.m_x );
    double x1 = std::max( pt.m_x, m_startPt.m_x );
    double y0 = std::min( pt.m_y, m_startPt.m_y );
    double y1 = std::max( pt.m_y, m_startPt.m_y );

    m_newObject->setPosition(lxPoint(x0,y0));
    m_newObject->setSize(lxSize(x1-x0, y1-y0));
}


#include "lxPolygonCanvasTool.h"

#include "lxCanvas.h"

lxPolygonCanvasTool::lxPolygonCanvasTool()
{
    //ctor
}

lxPolygonCanvasTool::~lxPolygonCanvasTool()
{
    //dtor
}

void lxPolygonCanvasTool::draw(lxDC& dc){
    if(!m_newObject) return;
    m_newObject->draw(dc, true);
}

bool lxPolygonCanvasTool::OnMouseLeftDown(lxCanvas* canvas, wxMouseEvent& event){
    event.Skip();
    if(!m_newObject){
        m_newObject = std::make_unique<lxPolygonCanvasObject>();
        m_newObject->setPenColour( canvas->getPenColour() );
        m_newObject->setFillColour( canvas->getFillColour() );
        m_newObject->setLineSize( canvas ->getLineSize() );
        m_newObject->setPosition(event.GetPosition());
        m_newObject->addPoint(event.GetPosition());
        m_newObject->addPoint(event.GetPosition());
    }else{
        const int CLOSE_THRESH = 3;
        lxPoint pt0 = m_newObject->getPoint(0);
        if( pt0.GetDistance(event.GetPosition()) < CLOSE_THRESH ){
            m_newObject->popPoint();
            canvas->addObject(std::move(m_newObject));
        }else{
            m_newObject->addPoint(event.GetPosition());
        }
    }
    return true;
}

bool lxPolygonCanvasTool::OnMouseMove(lxCanvas* canvas, wxMouseEvent& event){
    event.Skip();
    if(m_newObject){
        m_newObject->movePoint(-1, event.GetPosition());
        canvas->requestRedraw();
        return true;
    }
    return false;
}

bool lxPolygonCanvasTool::OnMouseLeftUp(lxCanvas* canvas, wxMouseEvent& event){
    event.Skip();
    return true;
}

bool lxPolygonCanvasTool::OnMouseLeftDClick(lxCanvas* canvas, wxMouseEvent& event){
    event.Skip();
    if(m_newObject){
        const int CLOSE_THRESH = 3;
        lxPoint pt0 = m_newObject->getPoint(0);
        if( pt0.GetDistance(event.GetPosition()) < CLOSE_THRESH ){
            m_newObject->popPoint();
        }
        m_newObject->popPoint();
        canvas->addObject(std::move(m_newObject));
    }
    return true;
}

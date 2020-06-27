#include "lxScribbleCanvasTool.h"

#include "lxCanvas.h"

lxScribbleCanvasTool::lxScribbleCanvasTool()
{
    //ctor
}

lxScribbleCanvasTool::~lxScribbleCanvasTool()
{
    //dtor
}

void lxScribbleCanvasTool::draw(lxDC& dc){
    if(!m_newObject) return;
    m_newObject->draw(dc, true);
}

bool lxScribbleCanvasTool::OnMouseLeftDown(lxCanvas* canvas, wxMouseEvent& event){
        event.Skip();
        m_newObject = std::make_unique<lxScribbleCanvasObject>();
        m_newObject->setPenColour( canvas->getPenColour() );
        m_newObject->setFillColour( canvas->getFillColour() );
        m_newObject->setLineSize( canvas ->getLineSize() );
        m_newObject->setPosition(event.GetPosition());
        m_newObject->addPoint(event.GetPosition());
        return true;
}

bool lxScribbleCanvasTool::OnMouseMove(lxCanvas* canvas, wxMouseEvent& event){
    event.Skip();
    if(m_newObject){
        m_newObject->addPoint(event.GetPosition());
        canvas->requestRedraw();
        return true;
    }
    return false;
}

bool lxScribbleCanvasTool::OnMouseLeftUp(lxCanvas* canvas, wxMouseEvent& event){
    event.Skip();
    if(m_newObject){
        canvas->addObject(std::move(m_newObject));
    }
    return true;
}

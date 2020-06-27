#include "lxSelectCanvastool.h"

#include "lxCanvas.h"
lxSelectCanvastool::lxSelectCanvastool(){
    //ctor
}

//Methods
void lxSelectCanvastool::draw(lxDC& dc){
    if( _doingRectSelection() ){
        dc.SetPen(*wxBLACK_DASHED_PEN);
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawRectangle(_getRect());
    }
}

bool lxSelectCanvastool::OnMouseLeftDown(lxCanvas* canvas, wxMouseEvent& event){
    m_curPt = m_startPt = event.GetPosition();
    m_curHandle = canvas->getSelectionHandleAt( m_curPt );
    if( !m_curHandle.isOk() ){
        m_curObject = canvas->getObjectAt( m_curPt );
        if( m_curObject ){
            m_dragDelta = m_curObject->getPosition() - m_curPt;
            canvas->selectObject(m_curObject, event.ShiftDown());
        }
    }else{
        m_curObject = nullptr;
    }
    m_isLeftDown = true;
    return true;
}

bool lxSelectCanvastool::OnMouseMove(lxCanvas* canvas, wxMouseEvent& event){
    m_curPt = event.GetPosition();
    if( _doingDragHandle() ){
        _prepareToModify(canvas);
        m_curHandle.getObject()->moveHandle(m_curHandle, m_curPt);
        canvas->requestRedraw();
    }else if( _doingDragObject() ){
        _prepareToModify(canvas);
        m_curObject->setPosition( m_curPt + m_dragDelta );
        canvas->requestRedraw();
    }else if( _doingRectSelection() ){
        canvas->requestRedraw();
    }
    return true;
}

bool lxSelectCanvastool::OnMouseLeftUp(lxCanvas* canvas, wxMouseEvent& event){
    m_curPt = event.GetPosition();
    if( _doingDragHandle() ){
        m_curHandle.getObject()->moveHandle(m_curHandle, m_curPt);
        m_curHandle.getObject()->finalizeHandle(m_curHandle, m_curPt);
    }else if( _doingDragObject() ){
        m_curObject->setPosition( m_curPt + m_dragDelta );
    }else if( _doingRectSelection() ){
        canvas->selectByRectangle(_getRect());
    }
    m_curObject     = nullptr;
    m_curHandle     = lxCanvasObject::Handle();
    m_objModified    = false;
    m_isLeftDown    = false;
    canvas->requestRedraw();
    return true;
}

//private
void lxSelectCanvastool::_prepareToModify(lxCanvas* canvas){
    if(!m_objModified){
        canvas->saveUndoInfo();
        m_objModified = true;
    }
}

lxRect lxSelectCanvastool::_getRect(){
    double x0 = std::min( m_curPt.m_x, m_startPt.m_x );
    double x1 = std::max( m_curPt.m_x, m_startPt.m_x );
    double y0 = std::min( m_curPt.m_y, m_startPt.m_y );
    double y1 = std::max( m_curPt.m_y, m_startPt.m_y );
    return lxRect(x0,y0, x1-x0,y1-y0);
}

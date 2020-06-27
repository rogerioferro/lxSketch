#include "lxLineCanvasTool.h"

lxLineCanvasTool::lxLineCanvasTool()
{
    //ctor
}

lxLineCanvasTool::~lxLineCanvasTool()
{
    //dtor
}

std::unique_ptr<lxCanvasObject> lxLineCanvasTool::createObject(){
    return std::make_unique<lxLineCanvasObject>();
}

void lxLineCanvasTool::updateObject(const lxPoint& pt){
    lxLineCanvasObject *obj = (lxLineCanvasObject*)m_newObject.get();
    obj->setStartPt(m_startPt);
    obj->setEndPt(pt);
}

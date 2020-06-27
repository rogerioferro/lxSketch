#include "lxEllipseCanvasTool.h"

lxEllipseCanvasTool::lxEllipseCanvasTool()
{
    //ctor
}

lxEllipseCanvasTool::~lxEllipseCanvasTool()
{
    //dtor
}

//Methods
std::unique_ptr<lxCanvasObject> lxEllipseCanvasTool::createObject(){
    return std::make_unique<lxEllipseCanvasObject>();
}

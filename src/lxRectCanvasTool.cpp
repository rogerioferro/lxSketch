#include "lxRectCanvasTool.h"

lxRectCanvasTool::lxRectCanvasTool(){
    //ctor
}

//Methods
std::unique_ptr<lxCanvasObject> lxRectCanvasTool::createObject(){
    return std::make_unique<lxRectCanvasObject>();
}

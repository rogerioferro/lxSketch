#ifndef LX_RECT_CANVAS_TOOL_H
#define LX_RECT_CANVAS_TOOL_H

#include "lxCurveCanvasTool.h"
#include "lxRectCanvasObject.h"

class lxRectCanvasTool : public lxCurveCanvastool
{
    public://Constructor and Destructor
        lxRectCanvasTool();
        virtual ~lxRectCanvasTool() {
            //std::cout<<"lxRectCanvasTool::Destructor"<<std::endl;
        }
    protected://Methods
        virtual std::unique_ptr<lxCanvasObject> createObject() override;
};

#endif // LX_RECT_CANVAS_TOOL_H

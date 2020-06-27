#ifndef LX_ELLIPSE_CANVAS_TOOL_H
#define LX_ELLIPSE_CANVAS_TOOL_H

#include "lxCurveCanvasTool.h"
#include "lxEllipseCanvasObject.h"

class lxEllipseCanvasTool : public lxCurveCanvastool
{
    public:
        lxEllipseCanvasTool();
        virtual ~lxEllipseCanvasTool();

    protected://Methods
        virtual std::unique_ptr<lxCanvasObject> createObject() override;
};

#endif // LX_ELLIPSE_CANVAS_TOOL_H

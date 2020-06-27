#ifndef LX_LINE_CANVAS_TOOL_H
#define LX_LINE_CANVAS_TOOL_H

#include "lxCurveCanvasTool.h"
#include "lxLineCanvasObject.h"

class lxLineCanvasTool : public lxCurveCanvastool
{
    public:
        lxLineCanvasTool();
        virtual ~lxLineCanvasTool();

    protected://Methods
        virtual std::unique_ptr<lxCanvasObject> createObject() override;
        virtual void updateObject(const lxPoint &pt) override;

};

#endif // LX_LINE_CANVAS_TOOL_H

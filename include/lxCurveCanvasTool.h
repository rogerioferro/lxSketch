#ifndef LX_CURVE_CANVAS_TOOL_H
#define LX_CURVE_CANVAS_TOOL_H

#include "lxCanvasTool.h"
#include "lxCanvasObject.h"

#include <memory>

class lxCurveCanvastool : public lxCanvasTool
{
    public:
        lxCurveCanvastool();
        virtual ~lxCurveCanvastool();

    public://Methods
        virtual wxCursor getDefaultCursor() override { return *wxCROSS_CURSOR; }
        virtual void draw (lxDC &dc) override;
        //Mouse Events
        virtual bool OnMouseLeftDown (lxCanvas* canvas, wxMouseEvent& event) override;
        virtual bool OnMouseMove     (lxCanvas* canvas, wxMouseEvent& event) override;
        virtual bool OnMouseLeftUp   (lxCanvas* canvas, wxMouseEvent& event) override;

    protected://Methods
        virtual std::unique_ptr<lxCanvasObject> createObject() = 0;
        virtual void updateObject(const lxPoint &pt);

    protected://Attributes
        std::unique_ptr<lxCanvasObject> m_newObject;
        lxPoint                         m_startPt;
};

#endif // LX_CURVE_CANVAS_TOOL_H

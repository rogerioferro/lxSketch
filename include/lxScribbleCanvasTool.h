#ifndef LX_SCRIBBLE_CANVAS_TOOL_H
#define LX_SCRIBBLE_CANVAS_TOOL_H

#include "lxCanvasTool.h"
#include "lxScribbleCanvasObject.h"

#include <memory>


class lxScribbleCanvasTool : public lxCanvasTool
{
    public:
        lxScribbleCanvasTool();
        virtual ~lxScribbleCanvasTool();
    public://Methods
        virtual wxCursor getDefaultCursor() override { return wxCURSOR_PENCIL; }
        virtual void draw (lxDC &dc) override;

        //Mouse Events
        virtual bool OnMouseLeftDown    (lxCanvas* canvas, wxMouseEvent& event) override;
        virtual bool OnMouseMove        (lxCanvas* canvas, wxMouseEvent& event) override;
        virtual bool OnMouseLeftUp      (lxCanvas* canvas, wxMouseEvent& event) override;

    protected://Attributes
        std::unique_ptr<lxScribbleCanvasObject> m_newObject;
};

#endif // LX_SCRIBBLE_CANVAS_TOOL_H

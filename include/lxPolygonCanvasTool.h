#ifndef LX_POLYGON_CANVAS_TOOL_H
#define LX_POLYGON_CANVAS_TOOL_H

#include "lxCanvasTool.h"
#include "lxPolygonCanvasObject.h"

#include <memory>


class lxPolygonCanvasTool : public lxCanvasTool
{
    public:
        lxPolygonCanvasTool();
        virtual ~lxPolygonCanvasTool();
    public://Methods
        virtual wxCursor getDefaultCursor() override { return *wxCROSS_CURSOR; }
        virtual void draw (lxDC &dc) override;

        //Mouse Events
        virtual bool OnMouseLeftDown    (lxCanvas* canvas, wxMouseEvent& event) override;
        virtual bool OnMouseMove        (lxCanvas* canvas, wxMouseEvent& event) override;
        virtual bool OnMouseLeftUp      (lxCanvas* canvas, wxMouseEvent& event) override;
        virtual bool OnMouseLeftDClick  (lxCanvas* canvas, wxMouseEvent& event) override;

    protected://Attributes
        std::unique_ptr<lxPolygonCanvasObject> m_newObject;
};

#endif // LX_POLYGON_CANVAS_TOOL_H

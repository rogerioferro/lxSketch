#ifndef LX_POLYGON_CANVAS_OBJECT_H
#define LX_POLYGON_CANVAS_OBJECT_H

#include "lxScribbleCanvasObject.h"

class lxPolygonCanvasObject : public lxScribbleCanvasObject
{
    lxCLASS_NAME(lxPolygonCanvasObject)
    public:
        lxPolygonCanvasObject();
        virtual ~lxPolygonCanvasObject();
    public://Methods
        // Object Drawing Methods
        virtual void        drawHandles (lxDC& dc);

        //Selection Methods
        virtual Handle  getSelectionHandleContainingPoint   (const lxPoint& point);
        virtual void    moveHandle                          (Handle& handle, const lxPoint& point);

    protected://Methods
        virtual void    drawObject  (lxDC &dc, const lxPoint& position, bool selected) override;

};

#endif // LX_POLYGON_CANVAS_OBJECT_H

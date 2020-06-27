#ifndef LX_ELLIPSE_CANVAS_OBJECT_H
#define LX_ELLIPSE_CANVAS_OBJECT_H

#include "lxCanvasObject.h"

class lxEllipseCanvasObject : public lxCanvasObject
{
    lxCLASS_NAME(lxEllipseCanvasObject)
    public:
        lxEllipseCanvasObject();
        virtual ~lxEllipseCanvasObject();

    protected://Methods
        virtual void drawObject(lxDC &dc, const lxPoint& position, bool selected) override;
};

#endif // LX_ELLIPSE_CANVAS_OBJECT_H

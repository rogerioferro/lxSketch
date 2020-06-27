#ifndef LX_RECT_CANVAS_OBJECT_H
#define LX_RECT_CANVAS_OBJECT_H

#include "lxCanvasObject.h"

class lxRectCanvasObject : public lxCanvasObject
{
    lxCLASS_NAME(lxRectCanvasObject)

    public://Constructor and Destructor
        lxRectCanvasObject();
        virtual ~lxRectCanvasObject();

    public://Methods
        //lxCanvasObject
            virtual bool containsPoint(const lxPoint& point) override;

    protected://Methods
        virtual void drawObject(lxDC &dc, const lxPoint& position, bool selected) override;
};

#endif // LX_RECT_CANVAS_OBJECT_H

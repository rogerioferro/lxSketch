#ifndef LX_SCRIBBLE_CANVAS_OBJECT_H
#define LX_SCRIBBLE_CANVAS_OBJECT_H

#include "lxCanvasObject.h"

class lxScribbleCanvasObject : public lxCanvasObject
{
    lxCLASS_NAME(lxScribbleCanvasObject)
    public:
        lxScribbleCanvasObject();
        virtual ~lxScribbleCanvasObject();
    public://Methods
        virtual void moveHandle(Handle& handle, const lxPoint& point) override {}
        //
        void    addPoint    (const lxPoint &pt);
        lxPoint getPoint    (int idx);
        void    movePoint   (int idx, const lxPoint &pt);
        void    popPoint    ();

    protected://Methods
        virtual void    writeMembers(lxWriter &writer) override;
        virtual void    parseMembers(lxParse &parser) override;
        virtual void    drawObject  (lxDC &dc, const lxPoint& position, bool selected) override;

    protected://Attributes
        std::vector<lxPoint> m_points;

    private://Methods
        void _updateBoundingBox();
};

#endif // LX_SCRIBBLE_CANVAS_OBJECT_H

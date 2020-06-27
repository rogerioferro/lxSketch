#ifndef LX_LINE_CANVAS_OBJECT_H
#define LX_LINE_CANVAS_OBJECT_H

#include "lxCanvasObject.h"

class lxLineCanvasObject : public lxCanvasObject
{
    lxCLASS_NAME(lxLineCanvasObject)
    public:
        lxLineCanvasObject();
        virtual ~lxLineCanvasObject();
    public://Methods
        // Object Drawing Methods
        virtual void        drawHandles (lxDC& dc);

        //Selection Methods
        virtual Handle  getSelectionHandleContainingPoint   (const lxPoint& point);
        virtual void    moveHandle                          (Handle& handle, const lxPoint& point);

        //
        void    setStartPt  (const lxPoint &startPt);
        lxPoint getStartPt  ();
        void    setEndPt    (const lxPoint &endPt);
        lxPoint getEndPt    ();

    protected://Methods
        virtual void    writeMembers(lxWriter &writer) override;
        virtual void    parseMembers(lxParse &parser) override;
        virtual void    drawObject  (lxDC &dc, const lxPoint& position, bool selected) override;

    protected://Attributes
        lxPoint m_startPt;
        lxPoint m_endPt;

    private://Methods
        void _updateBoundingBox();

    private:
        //Handle IDs
        enum{
           m_handle_START_POINT,
           m_handle_END_POINT
        };

};

#endif // LX_LINE_CANVAS_OBJECT_H

#ifndef LX_CANVAS_OBJECT_H
#define LX_CANVAS_OBJECT_H
/**
 Base class for objects within the canvas.

    A lxSketch document consists of a front-to-back ordered list of
    CanvasObjects.  Each CanvasObject has the following properties:

        'position'      The position of the object within the document.
        'size'          The size of the object within the document.
        'penColour'     The colour to use for drawing the object's outline.
        'fillColour'    Colour to use for drawing object's interior.
        'lineSize'      Line width (in pixels) to use for object's outline.
**/

#include "lxDC.h"
#include "lxObject.h"

class lxCanvas;

class lxCanvasObject : public lxObject
{
    public://Constructor and Destructor
        lxCanvasObject();
        virtual ~lxCanvasObject();

    public://Methods
        virtual bool        hasPropertyEditor   ();
        virtual void        doPropertyEdit      ();
        virtual void        setPosition         (const lxPoint& position);
        virtual lxPoint     getPosition         () const { return m_position; }
        virtual void        setSize             (const lxSize& size);
        virtual lxSize      getSize             () const { return m_size; }
        virtual void        setPenColour        (const wxColour& colour);
        virtual wxColour    getPenColour        () const { return m_penColour; }
        virtual void        setFillColour       (const wxColour& colour);
        virtual wxColour    getFillColour       () const { return m_fillColour; }
        virtual void        setLineSize         (double lineSize);
        virtual double      getLineSize         () const { return m_lineSize; }

        // Object Drawing Methods
        virtual void        draw        (lxDC& dc, bool selected);
        virtual void        drawHandles (lxDC& dc);

        //Selection Methods
        class Handle{
            public:
                Handle() {}
                Handle(lxCanvasObject *obj, uint32_t value) {
                    m_obj   = obj;
                    m_value = value;
                }
            public: //Methods
                bool            isOk()      { return m_obj; }
                lxCanvasObject* getObject() { return m_obj; }
                uint32_t        getValue()  { return m_value; }
            private: //Attributes
                lxCanvasObject* m_obj   = nullptr;
                uint32_t        m_value = 0;
        };
        virtual bool    containsPoint                       (const lxPoint& point);
        virtual Handle  getSelectionHandleContainingPoint   (const lxPoint& point);
        virtual void    moveHandle                          (Handle& handle, const lxPoint& point);
        virtual void    finalizeHandle                      (const Handle& handle, const lxPoint& point);
        virtual bool    withinRect                          (const lxRect& rect);

    protected://Methods
        virtual void    writeMembers(lxWriter &writer) override;
        virtual void    parseMembers(lxParse &parser) override;
        /**
            Private routine to draw this DrawingObject.
            'dc' is the device context to use for drawing, while 'position' is
            the position in which to draw the object.
        **/
        virtual void    drawObject          (lxDC &dc, const lxPoint& position, bool selected) = 0;
        virtual void    drawSelHandle       (lxDC &dc, const lxPoint& handlePoint);
        virtual bool    pointInSelHandle    (const lxPoint& point, const lxPoint& handlePoint);

    protected://Attributes
        lxPoint     m_position      = lxPoint(0,0);
        lxSize      m_size          = lxSize(10,10);
        wxColour    m_penColour     = *wxBLACK;
        wxColour    m_fillColour    = *wxWHITE;
        double      m_lineSize      = 1;

    private:
        //Handle IDs
        enum{
           m_handle_LEFT_TOP,
           m_handle_RIGHT_TOP,
           m_handle_LEFT_BOTTOM,
           m_handle_RIGHT_BOTTOM
        };
};

#endif // LX_CANVAS_OBJECT_H

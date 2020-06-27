#ifndef LX_SELECT_CANVAS_TOOL_H
#define LX_SELECT_CANVAS_TOOL_H

#include "lxCanvasTool.h"
#include "lxCanvasObject.h"

class lxSelectCanvastool : public lxCanvasTool
{
    public://Constructor and Destructor
        lxSelectCanvastool();
        virtual ~lxSelectCanvastool() {
            //std::cout<<"lxSelectCanvastool::Destructor"<<std::endl;
        }

    public://Methods
        //virtual wxCursor getDefaultCursor() override { return *wxCROSS_CURSOR; }
        virtual void draw (lxDC &dc) override;

        //Mouse Events
        virtual bool OnMouseLeftDown (lxCanvas* canvas, wxMouseEvent& event) override;
        virtual bool OnMouseMove     (lxCanvas* canvas, wxMouseEvent& event) override;
        virtual bool OnMouseLeftUp   (lxCanvas* canvas, wxMouseEvent& event) override;

    protected:

    private://Methods
        void    _prepareToModify    (lxCanvas* canvas);
        bool    _doingRectSelection () { return m_isLeftDown && !m_curObject && !m_curHandle.isOk(); }
        bool    _doingDragObject    () { return m_curObject; }
        bool    _doingDragHandle    () { return m_curHandle.isOk(); }
        lxRect  _getRect            ();

    private://Attributes
        bool                    m_isLeftDown    = false;
        bool                    m_objModified    = false;
        lxCanvasObject*         m_curObject     = nullptr;
        lxCanvasObject::Handle  m_curHandle;
        lxPoint                 m_startPt;
        lxPoint                 m_curPt;
        lxPoint                 m_dragDelta;
};

#endif // LX_SELECT_CANVAS_TOOL_H

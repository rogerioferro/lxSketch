#ifndef LX_CANVAS_TOOL_H
#define LX_CANVAS_TOOL_H

#include "lxDC.h"

#include <memory>
#include <iostream>
#include <wx/event.h>
#include <wx/cursor.h>

class lxCanvas;

/** Base class for drawing tools */
class lxCanvasTool
{
    public://Constructor and Destructor
        lxCanvasTool(){}
        virtual ~lxCanvasTool() {
            //std::cout<<"lxCanvasTool::Destructor"<<std::endl;
        }

    public://Methods
        /** Return the cursor to use by default which this drawing tool is selected **/
        virtual wxCursor getDefaultCursor() { return *wxSTANDARD_CURSOR; }

        /** Draw tool action **/
        virtual void draw (lxDC &dc) = 0;

        //Mouse Events
        /**
            Mouse events passed in from the canvas.
            Returns True if the event is handled by the tool,
            False if the canvas can try to use it.
        **/
        virtual bool OnMouseLeftDown    (lxCanvas* canvas, wxMouseEvent& event)
            { return OnMouseEvent(event); }
        virtual bool OnMouseMove        (lxCanvas* canvas, wxMouseEvent& event)
            { return OnMouseEvent(event); }
        virtual bool OnMouseLeftUp      (lxCanvas* canvas, wxMouseEvent& event)
            { return OnMouseEvent(event); }
        virtual bool OnMouseLeftDClick  (lxCanvas* canvas, wxMouseEvent& event)
            { return OnMouseEvent(event); }



    protected://Attributes

    private:
        bool OnMouseEvent (wxMouseEvent& event){
            event.Skip();
            return false;
        }
};

#endif // LX_CANVAS_TOOL_H

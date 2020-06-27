#ifndef LX_SKETCH_FRAME_H
#define LX_SKETCH_FRAME_H

#include "lxSketchApp.h"

#include <wx/frame.h>
#include <wx/clrpicker.h>

#include "lxCanvas.h"

class lxSketchFrame: public wxFrame
{
    public:
        lxSketchFrame(wxFrame *frame, const wxString& title);
        ~lxSketchFrame();

    private://Attributes
        lxCanvas*   m_canvas;

    private:
        void    create_menu_bar     ();
        void    create_tool_bar     ();
        void    update_menus        ();

        //Menu IDs
        enum
        {
            //Tools
            idToolSelect    = 2000,
            idToolLine,
            idToolPolygon,
            idToolScribble,
            idToolRect,
            idToolEllipse,
            idToolText,
            //Objects
            idObjMoveFoward = 3000,
            idObjMoveToFront,
            idObjMoveBackward,
            idObjMoveToBack,
        };

        //Events
        void OnClose            (wxCloseEvent& event);
        void OnQuit             (wxCommandEvent& event);
        void OnUndo             (wxCommandEvent& event);
        void OnRedo             (wxCommandEvent& event);
        void OnSelectAll        (wxCommandEvent& event);
        void OnDuplicate        (wxCommandEvent& event);
        void OnDelete           (wxCommandEvent& event);
        void OnChooseTool       (wxCommandEvent& event);
        void OnMoveFoward       (wxCommandEvent& event);
        void OnMoveToFront      (wxCommandEvent& event);
        void OnMoveBackward     (wxCommandEvent& event);
        void OnMoveToBack       (wxCommandEvent& event);
        void OnCanvasChange     (wxCommandEvent& event);
        void OnAbout            (wxCommandEvent& event);

        //Objects Attributes
        void OnLineSizeChanged   (wxCommandEvent& event);
        void OnPenColourChanged  (wxColourPickerEvent& event);
        void OnFillColourChanged (wxColourPickerEvent& event);
};

#endif // LX_SKETCH_FRAME_H

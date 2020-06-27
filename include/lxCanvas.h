#ifndef LX_CANVAS_H
#define LX_CANVAS_H

#include "type_name.h"

#include "lxCanvasTool.h"
#include "lxCanvasObject.h"

#include <wx/scrolwin.h>
#include <wx/bitmap.h>
#include <wx/timer.h>

#include <memory>
#include <list>
#include <set>
#include <map>
#include <vector>

wxDECLARE_EVENT(lxEVT_CANVAS_CHANGE, wxCommandEvent);

class lxCanvas : public wxScrolledWindow
{
    public: //Constructor and Destructor
        lxCanvas(wxWindow *parent);
        virtual ~lxCanvas();

    public: //Methods
        void    requestRedraw           ();
        void    addObject               (std::unique_ptr<lxCanvasObject>, bool selectObject=true);
        bool    setCurrentTool          (const wxString &toolName);

        //Select
        void    selectAll               ();
        void    deselectAll             ();
        void    selectMany              (std::set<lxCanvasObject*> objs);
        void    selectObject            (lxCanvasObject *obj, bool add=false);
        void    selectByRectangle       (const lxRect &rect);
        lxCanvasObject::Handle
                getSelectionHandleAt    (const lxPoint &pt);
        lxCanvasObject*
                getObjectAt             (const lxPoint &pt);

        //Status
        bool    canSave         ();
        bool    canRevert       ();
        bool    canUndo         ();
        bool    canRedo         ();
        bool    hasSelection    ();
        bool    canMoveFoward   ();
        bool    canMoveBackward ();

        //Undo
        void saveUndoInfo   ();
        void doUndo         ();
        void doRedo         ();

        //Action
        void doDuplicate    ();
        void doDelete       ();

        //Move Object
        void doMoveForward  ();
        void doMoveToFront  ();
        void doMoveBackward ();
        void doMoveToBack   ();

        //Objects Attributes
        void        setPenColour    (const wxColour& colour);
        wxColour    getPenColour    () const { return m_penColour; }
        void        setFillColour   (const wxColour& colour);
        wxColour    getFillColour   () const { return m_fillColour; }
        void        setLineSize     (double lineSize);
        double      getLineSize     () const { return m_lineSize; }


        /** Register Methods**/
        template <typename TClass>
        void registerTool(const wxString &toolName)
            { m_tools[toolName] = std::make_unique<TClass>(); }

        template <typename TClass>
        void registerObject()
            { m_factory[type_name<TClass>()] = _createFunc<TClass>; }

    protected:

    private: //Classes
        class ObjState{
            public:
                ObjState(wxString &&_type, wxString &&_data, bool &&_selected)
                    : type(_type), data(_data), selected(_selected) {}
            public: //Attributes
                wxString    type;
                wxString    data;
                bool        selected;
        };
    private: //Attributes
        wxBitmap        m_buffer;
        bool            m_resize        = false;
        bool            m_reInitBuffer  = false;
        wxTimer*        m_redrawTimer   = nullptr;
        //Tool
        wxString        m_curToolName;
        lxCanvasTool*   m_curTool       = nullptr;
        //Objects Default Attributes
        wxColour        m_penColour     = *wxBLACK;
        wxColour        m_fillColour    = *wxWHITE;
        double          m_lineSize      = 1;

        /** Containers **/
        std::map< wxString, std::unique_ptr< lxCanvasTool > >
            m_tools;
        std::list< std::unique_ptr< lxCanvasObject > >
            m_contents;
        std::set< lxCanvasObject* >
            m_selection;
        std::vector< std::vector< ObjState > >
            m_undoStack, m_redoStack;

        /** Object Factory **/
        using TObjectCreate = std::unique_ptr< lxCanvasObject >(*)();
        std::map< wxString, TObjectCreate >
            m_factory;

        /** Register Methods**/
        template <typename TClass>
        static std::unique_ptr<lxCanvasObject> _createFunc()
            { return std::make_unique<TClass>();  }

    private: // Members
        void    _initBuffer     ();
        void    _drawContents   ();
        void    _redraw         ();
        void    _changed        ();
        //void    _showPopup      ()
        //
        std::vector<ObjState>   _buildStoredState();
        void                    _restoreStoredState(const std::vector<ObjState> &savedState);

    private: //Events
        void OnIdle             (wxIdleEvent& event);
        void OnSize             (wxSizeEvent& event);
        void OnPaint            (wxPaintEvent& event);
        void OnEraseBackground  (wxEraseEvent& event);
        void OnPanelScroll      (wxScrollWinEvent& event);
        void OnTimerDraw        (wxTimerEvent& event);

        //Mouse events
        void OnMouseMove        (wxMouseEvent& event);
        void OnMouseLeftDown    (wxMouseEvent& event);
        void OnMouseLeftUp      (wxMouseEvent& event);
        void OnMouseLeftDClick  (wxMouseEvent& event);

};

#endif // LX_CANVAS_H

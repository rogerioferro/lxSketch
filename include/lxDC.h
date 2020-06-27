#ifndef LX_DC_H
#define LX_DC_H

#include "lxGeometry.h"
#include <wx/graphics.h>
#include <wx/dc.h>

#include <vector>

class lxDC
{
public:
    lxDC(wxDC &dc)  : m_dc(dc)  {}
    lxDC(wxDC &&dc) : m_dc(dc)  {}
    virtual ~lxDC()             {}

    /** Members */
    wxDC&           GetDeviceContext()              { return m_dc; }
    void 	        Clear ()                        { m_dc.Clear(); }
    void            SetBrush (const wxBrush &brush) { m_dc.SetBrush(brush); }
    const wxBrush&  GetBrush () const               { return m_dc.GetBrush(); }
    void 	        SetPen (const wxPen &pen)       { m_dc.SetPen(pen); }
    const wxPen& 	GetPen () const                 { return m_dc.GetPen(); }
    void            SetBackground (const wxBrush &brush) { m_dc.SetBackground(brush); }
    void            SetTextForeground (const wxColour &colour) { m_dc.SetTextForeground(colour); }
    void            SetTextBackground (const wxColour &colour) { m_dc.SetTextBackground(colour); }


    //Text
    void    DrawText (const wxString &str, double x, double y, double angle = 0, bool round = true);
    lxSize 	GetTextExtent (const wxString &str) const { return m_dc.GetTextExtent(str);}
    double 	GetCharHeight () const { return m_dc.GetCharHeight(); }
    double 	GetCharWidth () const  { return m_dc.GetCharWidth(); }

    //Draw
    void 	StrokeArc (double x1, double y1, double x2, double y2, double xc, double yc, bool round = true);
    void    StrokeArc (const lxPoint& pt1, const lxPoint& pt2, const lxPoint& centre, bool round = true);
    void 	DrawLine (double x1, double y1, double x2, double y2, bool round = true);
    void 	DrawLine (const lxPoint &pt1, const lxPoint &pt2, bool round = true);
    void 	DrawRectangle (double x, double y, double width, double height, bool round = true);
    void 	DrawRectangle (const lxPoint &pt, const lxSize &sz, bool round = true);
    void 	DrawRectangle (const lxRect &rect, bool round = true);
    void 	DrawEllipse (double x, double y, double width, double height, bool round = true);
    void 	DrawEllipse (const lxPoint &pt, const lxSize &sz, bool round = true);
    void    DrawEllipse	(const lxRect &rect, bool round = true);
    void    DrawPolygon (const std::vector<lxPoint> &points, const lxPoint &offset = lxPoint(0,0), wxPolygonFillMode fillStyle = wxODDEVEN_RULE);
    void    DrawLines (const std::vector<lxPoint> &points, const lxPoint &offset = lxPoint(0,0));
protected:

private:
    wxDC &m_dc;
};

#endif // LX_DC_H

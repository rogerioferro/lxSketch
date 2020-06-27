#include "lxDC.h"

void lxDC::DrawText(const wxString& str, double x, double y, double angle, bool round)
{
    wxGraphicsContext *gc = m_dc.GetGraphicsContext();

    if(!gc || round){
        x = lxRound(x);
        y = lxRound(y);
    }

    if( gc ){
        gc->DrawText(str, x, y, angle);
    }else{
        m_dc.DrawRotatedText(str, x, y, angle);
    }
}


void lxDC::StrokeArc(double x1, double y1, double x2, double y2, double xc, double yc, bool round)
{
    wxGraphicsContext *gc = m_dc.GetGraphicsContext();

    if(!gc || round){
        x1 = lxRound(x1);
        y1 = lxRound(y1);
        x2 = lxRound(x2);
        y2 = lxRound(y2);
        xc = lxRound(xc);
        yc = lxRound(yc);
    }

    if( gc ){
        wxGraphicsPath path = gc->CreatePath();
        double r = sqrt( pow(x1-xc,2) + pow(y1-yc,2) );
        double startAngle =  atan2(y1-yc, x1-xc);
        double endAngle = atan2(y2-yc, x2-xc);
        path.AddArc(xc, yc, r, startAngle, endAngle, false);
        gc->StrokePath(path);
    }else{
        wxBrush brush = m_dc.GetBrush();
        m_dc.SetBrush( *wxTRANSPARENT_BRUSH );
        m_dc.DrawArc(x1, y1, x2, y2, xc, yc);
        m_dc.SetBrush( brush );
    }
}

void lxDC::StrokeArc(const lxPoint& pt1, const lxPoint& pt2, const lxPoint& centre, bool round)
{
    StrokeArc( pt1.m_x, pt1.m_y, pt2.m_x, pt2.m_y, centre.m_x, centre.m_y, round );
}

void lxDC::DrawLine(double x1, double y1, double x2, double y2, bool round)
{
    wxGraphicsContext *gc = m_dc.GetGraphicsContext();

    if(!gc || round){
        x1 = lxRound(x1);
        y1 = lxRound(y1);
        x2 = lxRound(x2);
        y2 = lxRound(y2);
    }

    if( gc )    gc->StrokeLine(x1, y1, x2, y2);
    else        m_dc.DrawLine(x1, y1, x2, y2);
}

void lxDC::DrawLine(const lxPoint& pt1, const lxPoint& pt2,  bool round)
{
    DrawLine(pt1.m_x, pt1.m_y, pt2.m_x, pt2.m_y, round);
}


void lxDC::DrawRectangle(double x, double y, double width, double height, bool round)
{
    wxGraphicsContext *gc = m_dc.GetGraphicsContext();

    if(!gc || round){
        x       = lxRound(x);
        y       = lxRound(y);
        width   = lxRound(width);
        height  = lxRound(height);
    }

    if( gc ){
        wxGraphicsPath path = gc->CreatePath();
        path.AddRectangle(x, y, width, height);
        gc->DrawPath(path);
    }else{
        m_dc.DrawRectangle(x, y, width, height);
    }
}

void lxDC::DrawRectangle(const lxPoint& pt, const lxSize& sz, bool round)
{
    DrawRectangle(pt.m_x, pt.m_y, sz.m_x, sz.m_y, round);
}

void lxDC::DrawRectangle(const lxRect& rect, bool round)
{
    DrawRectangle(rect.m_x, rect.m_y, rect.m_width, rect.m_height, round);
}

void lxDC::DrawEllipse(double x, double y, double width, double height, bool round)
{
    wxGraphicsContext *gc = m_dc.GetGraphicsContext();

    if(!gc || round){
        x       = lxRound(x);
        y       = lxRound(y);
        width   = lxRound(width);
        height  = lxRound(height);
    }

    if( gc ){
        wxGraphicsPath path = gc->CreatePath();
        path.AddEllipse(x, y, width, height);
        gc->DrawPath(path);
    }else{
        m_dc.DrawEllipse(x, y, width, height);
    }
}

void lxDC::DrawEllipse(const lxPoint& pt, const lxSize& sz, bool round)
{
    DrawEllipse(pt.m_x, pt.m_y, sz.m_x, sz.m_y, round);
}

void lxDC::DrawEllipse(const lxRect& rect, bool round)
{
    DrawEllipse(rect.m_x, rect.m_y, rect.m_width, rect.m_height, round);
}

void lxDC::DrawPolygon(const std::vector<lxPoint> &points, const lxPoint &offset, wxPolygonFillMode fillStyle)
{
    if(points.empty()) return;
    wxGraphicsContext *gc = m_dc.GetGraphicsContext();
    if( gc ){
        wxGraphicsPath path = gc->CreatePath();
        path.MoveToPoint(points[0] + offset);
        for(const lxPoint &pt : points){
            path.AddLineToPoint(pt + offset);
        }
        path.CloseSubpath();
        gc->DrawPath(path, fillStyle);
    }else{
        std::vector<wxPoint> pts;
        pts.reserve(points.size());
        for(const lxPoint &pt : points){
            pts.push_back(pt.round());
        }
        m_dc.DrawPolygon(pts.size(), pts.data(), offset.m_x, offset.m_y, fillStyle);
    }
}


void lxDC::DrawLines(const std::vector<lxPoint> &points, const lxPoint &offset)
{
    if(points.empty()) return;
    wxGraphicsContext *gc = m_dc.GetGraphicsContext();
    if( gc ){
        wxGraphicsPath path = gc->CreatePath();
        path.MoveToPoint(points[0] + offset);
        for(const lxPoint &pt : points){
            path.AddLineToPoint(pt + offset);
        }
 //       path.CloseSubpath();
        gc->StrokePath(path);
    }else{
        std::vector<wxPoint> pts;
        pts.reserve(points.size());
        for(const lxPoint &pt : points){
            pts.push_back(pt.round());
        }
        m_dc.DrawLines(pts.size(), pts.data(), offset.m_x, offset.m_y);
    }
}

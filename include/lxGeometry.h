#ifndef LX_GEOMETRY_H
#define LX_GEOMETRY_H

#include <wx/gdicmn.h>
#include <wx/geometry.h>
#include <limits>
#include <cmath>

#define lxLOWEST     std::numeric_limits<double>::lowest()
#define lxHIGHEST    std::numeric_limits<double>::max()
#define lxMIN        std::numeric_limits<double>::min()
#define lxMAX        std::numeric_limits<double>::max()
#define lxINFINITE   std::numeric_limits<double>::infinity()
#define lxNAN        std::numeric_limits<double>::quiet_NaN()

#include "lxObject.h"

inline int lxRound(double x){
    if( x < INT_MIN ) return INT_MIN;
    if( x > INT_MAX ) return INT_MAX;

    #if defined(HAVE_ROUND)
        return int(round(x));
    #else
        return (int)(x < 0 ? x - 0.5 : x + 0.5);
    #endif
}


class lxPoint :
    public lxObject,
    public wxPoint2DDouble
{
    lxCLASS_NAME(lxPoint)

public:
    /** Default constructor */
    lxPoint()                          : wxPoint2DDouble(){}
 	lxPoint(wxDouble x, wxDouble y)    : wxPoint2DDouble(x, y){}
 	lxPoint(const wxPoint &pt)         : wxPoint2DDouble(pt.x, pt.y){}
 	lxPoint(const wxRealPoint &pt)     : wxPoint2DDouble(pt.x, pt.y){}
 	lxPoint(const wxPoint2DDouble &pt) : wxPoint2DDouble(pt){}
 	lxPoint(const wxPoint2DInt &pt)    : wxPoint2DDouble(pt.m_x, pt.m_y){}
 	lxPoint(const wxSize &sz)          : wxPoint2DDouble(sz.x, sz.y){}

 	/** Members */
 	wxPoint  round() const{
        return wxPoint( lxRound(m_x), lxRound(m_y) );
 	}
protected:
    virtual void writeMembers(lxWriter &writer) override {
        writer.Key("m_x");
        writer.Double(m_x);
        writer.Key("m_y");
        writer.Double(m_y);
    }
    virtual void parseMembers(lxParse &parser) override {
        lxParse &x = parser["m_x"];
        if( x.IsDouble() ) m_x = x.GetDouble();
        lxParse &y = parser["m_y"];
        if( y.IsDouble() ) m_y = y.GetDouble();
    }
};

class lxSize : public lxPoint{
    lxCLASS_NAME(lxSize)
public:
    /** Default constructor */
    lxSize()                             : lxPoint(){}
 	lxSize(wxDouble w, wxDouble h)       : lxPoint(w, h){}
 	lxSize(const wxSize &sz)             : lxPoint(sz.GetWidth(), sz.GetHeight()){}
 	lxSize(const wxPoint2DInt &pt)       : lxPoint(pt){}
 	lxSize(const wxPoint2DDouble &pt)    : lxPoint(pt){}

 	/** Members */
 	void    SetWidth(double width) { m_x = width; }
 	double  GetWidth() const { return m_x; }
 	void    SetHeight(double height) { m_y = height; }
 	double  GetHeight() const { return m_y; }

    void Union(const lxPoint& pt){
        m_x = std::max(m_x, pt.m_x);
        m_y = std::max(m_y, pt.m_y);
    }

 	/** Members */
 	wxSize  round() const{
        return wxSize( lxRound(m_x), lxRound(m_y) );
 	}
};

class lxRect : public wxRect2DDouble {
public:
    /** Default constructor */
    lxRect()                                                 : wxRect2DDouble(){}
 	lxRect(const lxPoint &pt, const lxSize &sz)              : wxRect2DDouble(pt.m_x, pt.m_y, sz.m_x, sz.m_y){}
 	lxRect(wxDouble x, wxDouble y, wxDouble w, wxDouble h)   : wxRect2DDouble(x, y, w, h){}
 	lxRect(const wxRect &r)                                  : wxRect2DDouble(r.x, r.y, r.width, r.height){}
 	lxRect(const wxRect2DInt &r)                             : wxRect2DDouble(r.m_x, r.m_y, r.m_width, r.m_height){}

 	/** Members */
 	void SetSize(const lxSize &sz) { m_width = sz.m_x; m_height = sz.m_y; }
 	lxSize GetSize() const { return lxSize(m_width, m_height); }
 	double GetWidth() const { return m_width; }
 	double GetHeight() const { return m_height; }


 	void Inflate(wxDouble dx, wxDouble dy){
        Inset(-dx,-dy);
 	}

 	wxRect  round() const{
        return wxRect( lxRound(m_x), lxRound(m_y), lxRound(m_width), lxRound(m_height) );
 	}
};

inline wxPoint  lxRound(const lxPoint &pt)  { return pt.round();}
//inline wxSize   lxRound(const lxSize &sz)   { return sz.round();}
inline wxRect   lxRound(const lxRect &r)    { return r.round();}


#endif // LX_GEOMETRY_H

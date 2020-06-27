#include "lxScribbleCanvasObject.h"

lxScribbleCanvasObject::lxScribbleCanvasObject()
{
    //ctor
}

lxScribbleCanvasObject::~lxScribbleCanvasObject()
{
    //dtor
}

void lxScribbleCanvasObject::writeMembers(lxWriter& writer){
    lxCanvasObject::writeMembers(writer);
    writer.Key("m_points");
    writer.StartArray();
    for(lxPoint &pt : m_points){
        pt.writeData(writer);
    }
    writer.EndArray();
}

void lxScribbleCanvasObject::parseMembers(lxParse& parser){
    lxCanvasObject::parseMembers(parser);
    lxParse &arr = parser["m_points"];
    if(arr.IsArray()){
        m_points.clear();
        for(rapidjson::SizeType i = 0; i < arr.Size(); i++){
            lxPoint pt;
            pt.parseData(arr[i]);
            m_points.push_back(pt);
        }
    }
}

void lxScribbleCanvasObject::drawObject(lxDC& dc, const lxPoint& position, bool selected){
    dc.DrawLines(m_points, position);
}

void lxScribbleCanvasObject::addPoint(const lxPoint& pt){
    m_points.push_back(pt - m_position);
    _updateBoundingBox();
}

lxPoint lxScribbleCanvasObject::getPoint(int idx){
    if(idx < 0) idx += m_points.size();
    return m_position + m_points[idx];
}

void lxScribbleCanvasObject::movePoint(int idx, const lxPoint& pt){
    if(idx < 0) idx += m_points.size();
    m_points[idx] = pt - m_position;
    _updateBoundingBox();
}

void lxScribbleCanvasObject::popPoint(){
    m_points.pop_back();
    _updateBoundingBox();
}

void lxScribbleCanvasObject::_updateBoundingBox(){
    double x0 = lxHIGHEST;
    double x1 = lxLOWEST;
    double y0 = lxHIGHEST;
    double y1 = lxLOWEST;
    for(lxPoint &pt : m_points){
        if(x0 > pt.m_x) x0 = pt.m_x;
        if(x1 < pt.m_x) x1 = pt.m_x;
        if(y0 > pt.m_y) y0 = pt.m_y;
        if(y1 < pt.m_y) y1 = pt.m_y;
    }
    m_position.m_x  += x0;
    m_position.m_y  += y0;
    m_size.m_x      = x1 - x0;
    m_size.m_y      = y1 - y0;
    lxPoint delta(-x0, -y0);
    for(lxPoint &pt : m_points){
        pt += delta;
    }
}

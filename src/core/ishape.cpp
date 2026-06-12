#include "ishape.hpp"

bool ShapeProperties::operator < (const ShapeProperties& t) const
{
    return generateKey() < t.generateKey();
}

std::string ShapeProperties::generateKey() const
{
    return std::string(brush_color.name().toStdString() + 
            QString::number(brush_style).toStdString() + 
            pen_color.name().toStdString() +
            QString::number(pen_style).toStdString());
}

void ShapeProperties::writePropertiesTlv(TlvWriter& w) const
{
    w.pod(BRUSH_COLOR, brush_color.rgb());
    w.pod(BRUSH_STYLE, brush_style);
    w.pod(PEN_COLOR, pen_color.rgb());
    w.pod(PEN_STYLE, pen_style);
    w.pod(PEN_WIDTH, pen_width);
    w.pod(PEN_CAP_STYLE, pen_cap_style);
    w.pod(PEN_JOIN_STYLE, pen_join_style);
}

void ShapeProperties::readPropertiesTlv(TlvReader& r)
{
    TlvReader::Item it{}; 
    while(r.next(it))
    { 
        switch(it.id)
        { 
            case BRUSH_COLOR:
            {
                auto brush_color_rgb = 0;
                r.read_pod(it, brush_color_rgb);    
                brush_color = QColor(brush_color_rgb);
                break; 
            }
            case BRUSH_STYLE: 
                r.read_pod(it, brush_style); 
                break; 
            case PEN_COLOR: 
            {
                auto pen_color_rgb = 0;
                r.read_pod(it, pen_color_rgb);    
                pen_color = QColor(pen_color_rgb);
                break; 
            }
            case PEN_STYLE: 
                r.read_pod(it, pen_style); 
                break; 
            case PEN_WIDTH: 
                r.read_pod(it, pen_width); 
                break; 
            case PEN_CAP_STYLE: 
                r.read_pod(it, pen_cap_style); 
                break; 
            case PEN_JOIN_STYLE: 
                r.read_pod(it, pen_join_style); 
                break; 
            default:
                break; 
        } 
    } 
}

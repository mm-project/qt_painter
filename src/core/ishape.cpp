#include "ishape.hpp"

bool ShapeProperties::operator < (const ShapeProperties &t) const
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
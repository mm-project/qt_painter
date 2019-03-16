#ifndef BASIC_SHAPE_HPP
#define BASIC_SHAPE_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Includes
//

// Qt
#include <QColor>
#include <QPoint>

class QPainter;

///////////////////////////////////////////////////////////////////////////////
//
// @enum Object Type
//
enum ObjectType {
	LINE = 0,
	RECT,
	ELLIPSE,
	POLYGON
};

///////////////////////////////////////////////////////////////////////////////
//
// @struct ShapeProperties, collection of parameters and properties for drawing
//
struct ShapeProperties
{
	ShapeProperties() : 
		pen_color(Qt::white), 
		brush_color(Qt::black),
		pen_width(1),
		pen_style(Qt::SolidLine), 
		pen_cap_style(Qt::SquareCap), 
		pen_join_style(Qt::BevelJoin), 
		brush_style(Qt::SolidPattern) 
	{}

	QColor pen_color;
	QColor brush_color;
	int pen_width;
	Qt::PenStyle pen_style;
	Qt::PenCapStyle pen_cap_style;
	Qt::PenJoinStyle pen_join_style;
	Qt::BrushStyle brush_style;
};

///////////////////////////////////////////////////////////////////////////////
//
// interface for the shapes
// 
class IBasicShape
{
public:
	// @Constructor
	IBasicShape() {}
	IBasicShape(ObjectType t) :
		m_type(t) 
	{}
	IBasicShape(ObjectType t, ShapeProperties p) :
		m_type(t), 
		m_properties(p) 
	{}

	// @Destructor
	virtual ~IBasicShape() {}

public:

	virtual void reset() = 0;

	virtual void addPoint(const QPoint&) = 0;

	virtual void updateProperties(ShapeProperties b)
	{
		m_properties = b;
	}

	virtual bool is_draw_mode() {return false;}

public:

	virtual IBasicShape* clone() = 0;
	virtual void draw(QPainter*) = 0;

protected:
	//
	// Members
	//
	ObjectType m_type;
	ShapeProperties m_properties;
};

#endif

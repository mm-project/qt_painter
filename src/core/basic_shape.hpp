#ifndef BASIC_SHAPE_HPP
#define BASIC_SHAPE_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Includes
//

// Qt
#include <QColor>
#include <QPoint>

// STL
#include <string>
#include <vector>

class QPainter;

///////////////////////////////////////////////////////////////////////////////
//
// @enum Object Type
//
enum ObjectType {
	LINE = 0,
	RECTANGLE,
	ELLIPSE,
	POLYGON
};


namespace {
    std::string ObjType2String(const ObjectType& t) {
        switch (t) {
                case LINE :
                        return "Line";
                case RECTANGLE:
                        return "Rectangle";
                case ELLIPSE:
                        return "Ellipse";
                case POLYGON:
                        return "Polygon";
        }
        return "Unknown";
    }
    
}


///////////////////////////////////////////////////////////////////////////////
//
//	@struct ShapeProperties
//	collection of parameters and properties for drawing
//
struct ShapeProperties
{
	inline ShapeProperties() = default;

	QColor					pen_color = Qt::white;
	QColor					brush_color = Qt::black;
	int						pen_width = 1;
	Qt::PenStyle			pen_style = Qt::SolidLine;
	Qt::PenCapStyle			pen_cap_style = Qt::SquareCap;
	Qt::PenJoinStyle		pen_join_style = Qt::BevelJoin;
	Qt::BrushStyle			brush_style = Qt::SolidPattern;
};

///////////////////////////////////////////////////////////////////////////////
//
// interface for the shapes
// 
class IShape
{
public:
	//	dynamic_cast is too slow
	enum class Type
	{
		LINE = 0,
		RECTANGLE,
		ELLIPSE,
		POLYGON
	};

public:
	// @Constructor
	inline IShape() = default;
	inline IShape(ObjectType t) :
		m_type(t) {}

	inline IShape(ObjectType t, ShapeProperties p) :
		m_type(t), 
		m_properties(p) 
	{}

	// @Destructor
	virtual ~IShape() = default;

public:

	virtual void reset() = 0;

	virtual void addPoint(const QPoint&) = 0;

	virtual void updateProperties(ShapeProperties b)
	{
		m_properties = b;
	}

	virtual bool is_draw_mode() {return false;}

	virtual void movePoint(const QPoint&) {}

	virtual Type getType() const = 0;
  
public:

	virtual IShape* clone() = 0;
	virtual void draw(QPainter*) = 0;
        
        virtual std::vector<QPoint> getPoints() {
            std::vector<QPoint> res;
            res.push_back(QPoint(0,0));
            res.push_back(QPoint(100,150));
            
            return res;
        }

	//virtual bool intersects(const QRect&) const = 0;

	//virtual bool intersects(const QRect&) const = 0;

protected:
	//
	// Members
	//
	ObjectType m_type;
	ShapeProperties m_properties;
};

#endif

#ifndef BASIC_SHAPE_HPP
#define BASIC_SHAPE_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Includes
//

// Qt
#include <QColor>
#include <QRect>
#include <QPoint>

// STL
#include <map>
#include <memory>
#include <string>
#include <vector>

class QPainter;

///////////////////////////////////////////////////////////////////////////////
//
// @enum Object Type
//
enum ObjectType
{
    LINE = 0,
    RECTANGLE,
    ELLIPSE,
    POLYGON
};

namespace
{
std::string ObjType2String(const ObjectType &t)
{
    switch (t)
    {
    case LINE:
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

} // namespace

///////////////////////////////////////////////////////////////////////////////
//
//	@struct ShapeProperties
//	collection of parameters and properties for drawing
//
struct ShapeProperties
{
    inline ShapeProperties() = default;

    QColor pen_color = Qt::white;
    QColor brush_color = Qt::black;
    int pen_width = 1;
    Qt::PenStyle pen_style = Qt::SolidLine;
    Qt::PenCapStyle pen_cap_style = Qt::SquareCap;
    Qt::PenJoinStyle pen_join_style = Qt::BevelJoin;
    Qt::BrushStyle brush_style = Qt::SolidPattern;

    // fixme temporary fix
    std::map<std::string, int> toStringsMap() const
    {
        std::map<std::string, int> res;
        // res["color"]=1;//brush_color.name().toStdString();
        res["brush"] = pen_style;
        res["fill"] = brush_style;

        return res;
    }

    std::string toString() const
    {
        return std::string("  brushC -> " + brush_color.name().toStdString() + ", brushS -> " +
                           QString::number(brush_style).toStdString() + ", penC -> " + pen_color.name().toStdString() +
                           ", penS -> " + QString::number(pen_style).toStdString());
    }

    void fromString(const std::string &color, int pstyle, int bstyle)
    {
        pen_style = (Qt::PenStyle)pstyle;
        brush_style = (Qt::BrushStyle)bstyle;
        brush_color = QColor(QString(color.c_str()));
        // pen_color = QColor(QString(color.c_str()));
    }
};

///////////////////////////////////////////////////////////////////////////////
//
// interface for the shapes
//
class IShape
{
  public:
  public:
    // @Constructor
    inline IShape() = default;
    inline IShape(ObjectType t) : m_type(t)
    {
    }

    inline IShape(ObjectType t, ShapeProperties p) : m_type(t), m_properties(p)
    {
    }

    // @Destructor
    virtual ~IShape() = default;

  public:
    virtual void moveCenterToPoint(QPoint &) = 0;
    virtual void reset() = 0;

    virtual void addPoint(const QPoint &) = 0;
    virtual std::vector<QPoint> getPoints() = 0;
    //	make abstract
    virtual void movePoint(const QPoint &)
    {
    }

    virtual void updateProperties(ShapeProperties b)
    {
        m_properties = b;
    }

    virtual ShapeProperties getProperties() const
    {
        return m_properties;
    }

    // FIXME should return ObjectType instead
    virtual ObjectType getType() const = 0;

    virtual IShape *clone() = 0;

    virtual bool isDrawMode()
    {
        return false;
    }
    virtual void draw(QPainter *) = 0;

#ifdef NO_RQ
    virtual bool contains(const QPoint &point) const = 0;
#endif

    virtual bool contains( const QPoint& ) const = 0;
    virtual bool intersects( const QRect& ) const = 0;
    virtual bool isDisjointFrom( const QRect& ) const = 0;

    virtual QPoint center() const = 0;

  protected:
    //
    // Members
    //
    ObjectType m_type;
    ShapeProperties m_properties;
};

using IShapePtr = std::shared_ptr<IShape>;

#endif

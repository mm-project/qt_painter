#pragma once

///////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "../ishape.hpp"

// Qt
#include <QMouseEvent>
#include <QObject>
#include <QPoint>
#include <QPolygonF>
#include <QRectF>

///////////////////////////////////////////////////////////////////////////////
//
// @class polygon, wrapper for OA/Qt object
//
class Polygon : public IShape
{
public:
    SPD_DECLARE_TYPE(make_guid({0xCC,0xDD,0x56,0x78,0x90,0xAB,0xCD,0xEF,0x10,0x32,0x54,0x76,0x98,0xBA,0xDC,0xFE}))
    
    enum : FieldId { F_pts=1, F_fill=2, F_props=3 };

    Polygon(QPolygon = QPolygon(), ShapeProperties = ShapeProperties());
    virtual ~Polygon() = default;

public:
    Polygon *clone() override;
    void draw(QPainter *) override;

public:
    void reset() override;
    void addPoint(const QPoint &) override;
    void movePoint(const QPoint &) override;

    ObjectType getType() const override
    {
        return POLYGON;
    }

    QPoint getTopLeft() const;
    QPoint getBottomRight() const;

    bool contains(const QPoint &point) const override;

    bool intersects(const QRect &oRect) const override
    {
        return m_object.boundingRect().intersects(oRect);
    }

    virtual void moveCenterToPoint(QPoint &)
    {
        // m_object.moveTo(p);
    }

    // FIXME need proper fix and member handling
    std::vector<QPoint> getPoints() override
    {
        return std::vector<QPoint>(m_object.begin(), m_object.end());
    }

    QPoint center() const override;
    bool isDisjointFrom( const QRect& ) const override;

    QRectF getBBox() const override;

    void writeTlv(TlvWriter& w) const override;
    void readTlv(TlvReader& r) override;

  private:
    QPolygon m_object;
    bool m_first = true;
};

SPD_REGISTER_TYPE(Polygon)
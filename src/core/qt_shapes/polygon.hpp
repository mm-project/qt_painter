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

///////////////////////////////////////////////////////////////////////////////
//
// @class polygon, wrapper for OA/Qt object
//
class Polygon : public IShape
{
  public:
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

  private:
    QPolygon m_object;
    bool m_first = true;
};
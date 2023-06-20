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
// @class ellipse, wrapper for OA/Qt object
//
class Ellipse : public IShape
{
  public:
    Ellipse(QRect = QRect(), ShapeProperties = ShapeProperties());
    virtual ~Ellipse() = default;

  public:
    Ellipse *clone() override;
    void draw(QPainter *) override;

  public:
    void reset() override;
    void addPoint(const QPoint &) override;

  public:
    void setTopLeft(const QPoint &);
    void setBottomRight(const QPoint &);

    QPoint getTopLeft() const;
    QPoint getBottomRight() const;

    bool contains(const QPoint &point) const override
    {
        return m_object.contains(point);
    }
    bool intersects(const QRect &oRect) const override
    {
        return m_object.intersects(oRect);
    }

    virtual ObjectType getType() const override
    {
        return ELLIPSE;
    }

    virtual void moveCenterToPoint(QPoint &p)
    {
        m_object.moveTo(p);
    }

    std::vector<QPoint> getPoints() override
    {
        return std::vector<QPoint>(2) = {getBottomRight(), getTopLeft()};
    }

    QPoint center() const override;
    bool isDisjointFrom( const QRect& ) const override;
    
  private:
    QRect m_object;
    bool m_waitForSecondClick;
};
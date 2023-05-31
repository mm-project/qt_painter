#pragma once

///////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "../ishape.hpp"

// Qt
#include <QLine>
#include <QObject>

// stl
#include <vector>

///////////////////////////////////////////////////////////////////////////////
//
// @class line, wrapper for OA/Qt object
//
class Line : public IShape
{
  public:
    Line(QLine = QLine(), ShapeProperties = ShapeProperties());
    virtual ~Line() = default;

  public:
    Line *clone() override;
    void draw(QPainter *) override;

  public:
    void reset() override;
    void addPoint(const QPoint &) override;

  private:
    void setP1(const QPoint &);
    void setP2(const QPoint &);
    bool intersectsLine( const QPoint& a1, const QPoint& b1) const;

  public:
    QPoint getP1() const;
    QPoint getP2() const;

    virtual ObjectType getType() const override
    {
        return LINE;
    }

    virtual void moveCenterToPoint(QPoint &)
    {
        // m_object.moveTo(p);
    }

#ifdef NO_RQ
    virtual bool contains(const QPoint &point) const
    {
        return false;
    }
#endif

    // FIXME need proper fix and member handling
    std::vector<QPoint> getPoints() override
    {
        return std::vector<QPoint>(2) = {m_object.p1(), m_object.p2()};
    }

    bool contains( const QPoint& ) const override;
    bool intersects( const QRect& ) const override;
    bool isDisjointFrom( const QRect& ) const override;

    QPoint center() const override;

  private:
    QLine m_object;
    bool m_waitForSecondClick;
};
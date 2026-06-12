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
    SPD_DECLARE_TYPE(make_guid({0x12,0x34,0x56,0x78,0x90,0xAB,0xCD,0xEF,0x10,0x32,0x54,0x76,0x98,0xBA,0xDC,0xFE}))

    enum : FieldId { F_p0 = 1, F_p1 = 2 };

    Line(QLineF = QLineF(), ShapeProperties = ShapeProperties());
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

    // FIXME need proper fix and member handling
    std::vector<QPoint> getPoints() override
    {
        return std::vector<QPoint>(2) = {m_object.toLine().p1(), m_object.toLine().p2()};
    }

    bool contains( const QPoint& ) const override;
    bool intersects( const QRect& ) const override;
    bool isDisjointFrom( const QRect& ) const override;

    QPoint center() const override;

    QRectF getBBox() const override;

    void writeTlv(TlvWriter& w) const override;
    void readTlv(TlvReader& r) override;

private:
    QLineF m_object;
    bool m_waitForSecondClick;
};

SPD_REGISTER_TYPE(Line)

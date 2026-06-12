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
#include <QRect>
#include <QRectF> // Added include for QRectF

///////////////////////////////////////////////////////////////////////////////
//
// @class rectangle, wrapper for OA/Qt object
//
class Rectangle : public IShape
{
public:
    SPD_DECLARE_TYPE(make_guid({0xAA,0xBB,0x56,0x78,0x90,0xAB,0xCD,0xEF,0x10,0x32,0x54,0x76,0x98,0xBA,0xDC,0xFE}))
    enum : FieldId { F_p0 = 1, F_p1 = 2 };

    Rectangle(QRect = QRect(), ShapeProperties = ShapeProperties());
    virtual ~Rectangle() = default;

public:
    Rectangle *clone() override;
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

    ObjectType getType() const override
    {
        return RECTANGLE;
    }

    virtual void moveCenterToPoint(QPoint &p)
    {
        m_object.moveTo(p);
    }

    // FIXME need proper fix and member handling
    std::vector<QPoint> getPoints() override
    {
        return std::vector<QPoint>(2) = {getBottomRight(), getTopLeft()};
    }

    QPoint center() const override;
    bool isDisjointFrom( const QRect& ) const override;

    QRectF getBBox() const override; // Changed return type to QRectF

    void writeTlv(TlvWriter& w) const override;
    void readTlv(TlvReader& r) override;

private:
    QRect m_object;
    bool m_waitForSecondClick;
};

SPD_REGISTER_TYPE(Rectangle)
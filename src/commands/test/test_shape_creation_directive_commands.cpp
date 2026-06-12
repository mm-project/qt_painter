#include "../core/iobject_pool.hpp"
#include "../core/ishape.hpp"
#include "../shape_creation_directive_commands.hpp"

#include <cassert>
#include <string>
// IMocking!: Workingset
class MockWorkingSet : public IObjectPool
{

  public:
    virtual void clear() noexcept override
    {
    }
    virtual std::vector<IShapePtr> getObjects() const noexcept override
    {
        return {};
    }
    IShapePtr addObject(IShapePtr s) override
    {
        m_shapes_count++;
        return s;
    }
    virtual std::string getName() const noexcept override
    {
        return "MockWorkingSet";
    }
    virtual void dumpToFile(const std::string &) const
    {
    }
    virtual void removeObject(IShapePtr) noexcept
    {
    }
    virtual ~MockWorkingSet()
    {
    }
    int m_shapes_count = 0;
};

// IMocking!: Shape
class MockShape : public IShape
{
    virtual void reset()
    {
    }
    virtual void addPoint(const QPoint &)
    {
    }
    virtual void updateProperties(ShapeProperties)
    {
    }
    virtual bool is_draw_mode()
    {
        return false;
    }
    virtual void movePoint(const QPoint &)
    {
    }
    virtual IShape *clone()
    {
        return new MockShape;
    }
    virtual void draw(QPainter *)
    {
    }
    virtual std::vector<QPoint> getPoints()
    {
        return {};
    }
    virtual ObjectType getType() const {return ObjectType::LINE;};
    virtual void moveCenterToPoint(QPoint &)
    {
    }
    bool contains( const QPoint& ) const override
    {
        return false;
    }
    bool intersects( const QRect& ) const override
    {
        return false;
    }
    bool isDisjointFrom( const QRect& ) const override
    {
        return false;
    }
    QPoint center() const override
    {
        return {};
    }
    QRectF getBBox() const override
    {
        return {};
    }

    const ClassGuid& guid() const override { return staticGuid(); }

    static const ClassGuid& staticGuid() 
    { 
        static const ClassGuid g = make_guid({1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}); 
        return g; 
    }

};

// Mocking!: Implementations
ShapeCreator::ShapeCreator()
{
}
ShapeCreator::~ShapeCreator()
{
}
IShapePtr ShapeCreator::create(ObjectType)
{
    return std::shared_ptr<IShape>(new MockShape);
}

// Mocking!: Implementations
void Messenger::expose_msg(const LogMsgSeverity &, const std::string &, bool)
{
}
void Messenger::log_command(const std::string &, bool)
{
}

RegionQuery::RegionQuery()
{
}
void RegionQuery::shutDown()
{
}
void RegionQuery::insertObject(IShapePtr)
{
}
void RegionQuery::removeObject(IShapePtr)
{
}
void RegionQuery::clear()
{
}
std::vector<IShapePtr> RegionQuery::getShapeUnderPos(QPoint const &) const
{
    return {};
}

void ServiceManager::shutDown()
{
}
//! Mocking

/*!
    \brief UT_load_save_commands is entry point for unit testing load_save_commands .

    This unit test covers following steps:

    \list
        \li step1
        \li step2
    \endlist
*/
bool UT_shape_creation_directive_commands()
{
    // Expecting!: dicmdCreateObj to be created
    auto& dm = DesignManager::getInstance();
    dm.closeDesign(0);
    dm.createDesign(0);
    dm.setActiveDesign(0);
    dicmdCreateObj<RECTANGLE> rect_cmd;

    // Expecting!: adding arguments and no impact on working set
    rect_cmd.set_arg("-points", "{(0,0;100,100)}");
    assert("SHAPES COUNT IN WS BEFORE EXECUTING" && dm.getActiveDesign()->getObjects().size() == 0);

    // Expecting!: executing command and working set should be added with 1 shape
    rect_cmd.execute();
    assert("SHAPES COUNT IN WS AFTER EXECUTING FIRST TIME" &&
           dm.getActiveDesign()->getObjects().size() == 1);

    // Expecting!: executing command and working set should have addional shapes
    rect_cmd.execute();
    assert("SHAPES COUNT IN WS AFTER EXECUTING SECOND TIME" &&
           dm.getActiveDesign()->getObjects().size() == 2);
    return true;
}

int main()
{
    UT_shape_creation_directive_commands();
}

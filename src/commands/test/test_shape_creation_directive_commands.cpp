#include "../shape_creation_directive_commands.hpp"
#include "../core/iobject_pool.hpp"
#include "../core/ishape.hpp"

#include <cassert>
#include <string>
/*! \file */ 
//IMocking!: Workingset
class MockWorkingSet : public IObjectPool
{

public:
	virtual void clear() override {} 
	virtual std::vector<IShape*> getObjects() const override {}
    IShape* addObject(IShape* s) override { m_shapes_count++; return s;}
	virtual std::string getName() override {}
	virtual void dumpToFile(const std::string&) {}
	virtual void removeObject(IShape*) {}
	virtual ~MockWorkingSet() {}    
	int m_shapes_count = 0;
};

//IMocking!: Shape
class MockShape : public IShape
{
	virtual void reset() { }
	virtual void addPoint(const QPoint&) { }
	virtual void updateProperties(ShapeProperties b) {}
	virtual bool is_draw_mode() {return false;}
	virtual void movePoint(const QPoint&) {}
	virtual IShape* clone() { return new MockShape; }
	virtual void draw(QPainter*) { }
    virtual std::vector<QPoint> getPoints() { }
    virtual ObjectType getType() const { } ;
};

//Mocking!: Implementations
ShapeCreator::ShapeCreator() {}
ShapeCreator::~ShapeCreator() {}
IShape* ShapeCreator::create(ObjectType) {return new MockShape;}
    
void Messenger::expose_msg(const LogMsgSeverity& s, const std::string&, bool) {}
void Messenger::log_command(const std::string&, bool) {}

RegionQuery::RegionQuery() {}
void RegionQuery::shutDown() {}
void RegionQuery::insertObject(IShape*) {}

void ServiceManager::shutDown(){}
//!Mocking

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
    
    //Expecting!: dicmdCreateObj to be created
    IObjectPoolPtr ws = std::shared_ptr<MockWorkingSet>(new MockWorkingSet);
    dicmdCreateObj<RECTANGLE> rect_cmd(ws);
    
    //Expecting!: adding arguments and no impact on working set
    rect_cmd.set_arg("-points","{(0,0;100,100)}");
    assert("SHAPES COUNT IN WS BEFORE EXECUTING"&&dynamic_cast<MockWorkingSet*>(ws.get())->m_shapes_count == 0);

    //Expecting!: executing command and working set should be added with 1 shape
    rect_cmd.execute();
    assert("SHAPES COUNT IN WS AFTER EXECUTING FIRST TIME"&&dynamic_cast<MockWorkingSet*>(ws.get())->m_shapes_count == 1);    

    //Expecting!: executing command and working set should have addional shapes
    rect_cmd.execute();
    assert("SHAPES COUNT IN WS AFTER EXECUTING SECOND TIME"&&dynamic_cast<MockWorkingSet*>(ws.get())->m_shapes_count == 2);    
}


int main() 
{
    UT_shape_creation_directive_commands();
}

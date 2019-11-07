#include "../shape_creation_directive_commands.hpp"
#include "../core/iobject_pool.hpp"
#include "../core/ishape.hpp"

#include <cassert>
#include <string>

//!Mocking: Workingset
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
//Mocking!

//!Mocking: Shape
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
//Mocking!

//!Mocking: ShapeCreator
ShapeCreator::ShapeCreator() {}
ShapeCreator::~ShapeCreator() {}
IShape* ShapeCreator::create(ObjectType) {return new MockShape;}
//Mocking!
    
//!Mocking: Messenger
void Messenger::expose_msg(const LogMsgSeverity& s, const std::string&, bool) {}
void Messenger::log_command(const std::string&, bool) {}
//Mocking!

//!Mocking: RegionQuery
RegionQuery::RegionQuery() {}
void RegionQuery::shutDown() {}
void RegionQuery::insertObject(IShape*) {}
//Mocking!

//!Mocking: ServiceManager
void ServiceManager::shutDown(){}
//Mocking!

/*
bool
{
    IObjectPoolPtr ws;
    dicmdCreateObj<RECTANGLE> rect_cmd(ws);
    rect_cmd.set_arg("-points","{(0,0;100,100)}");
    
}

bool tst_all() 
{
    tst_obj_replying_creation<RECTANGLE>();
    tst_obj_replying_creation<ELIPSE>();
    tst_obj_replying_creation<LINE();
    tst_obj_replying_creation<>();
    tst_obj_replying_creation<RECTANGLE>();
}
*/

int main()
{
    IObjectPoolPtr ws = std::shared_ptr<MockWorkingSet>(new MockWorkingSet);
    dicmdCreateObj<RECTANGLE> rect_cmd(ws);
    rect_cmd.set_arg("-points","{(0,0;100,100)}");
    assert("SHAPES COUNT IN WS BEFORE EXECUTING"&&dynamic_cast<MockWorkingSet*>(ws.get())->m_shapes_count == 0);
    rect_cmd.execute();
    assert("SHAPES COUNT IN WS AFTER EXECUTING FIRST TIME"&&dynamic_cast<MockWorkingSet*>(ws.get())->m_shapes_count == 1);    
    rect_cmd.execute();
    assert("SHAPES COUNT IN WS AFTER EXECUTING SECOND TIME"&&dynamic_cast<MockWorkingSet*>(ws.get())->m_shapes_count == 2);    
}

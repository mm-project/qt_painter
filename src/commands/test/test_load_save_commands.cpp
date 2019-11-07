#include "../load_save_commands.hpp"
#include "../core/iobject_pool.hpp"
#include "../core/ishape.hpp"
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


int main()
{
    IObjectPoolPtr ws = std::shared_ptr<MockWorkingSet>(new MockWorkingSet);
    dicmdDesignSave cmd(ws);
    cmd.set_arg("-filename","morqur");
    cmd.execute();
}

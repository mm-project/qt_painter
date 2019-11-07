#include "../load_save_commands.hpp"
#include "../core/iobject_pool.hpp"
#include "../core/ishape.hpp"

//IMocking!: MockWorkingSet=>IObjectPool
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

//IMocking!: MockShape=>IShape
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

//Mocking!: 
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

int main()
{
    //Expecting!: dicmdDesignSave command to be properly created
    IObjectPoolPtr ws = std::shared_ptr<MockWorkingSet>(new MockWorkingSet);
    dicmdDesignSave cmd(ws);

    //Expecting!: should be called properly with called arguments
    cmd.set_arg("-filename","morqur");
    cmd.execute();
}

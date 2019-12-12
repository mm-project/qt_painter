#include "../load_save_commands.hpp"
#include "../core/iobject_pool.hpp"
#include "../core/ishape.hpp"
/*! \file */ 
//IMocking!: MockWorkingSet=>IObjectPool
class MockWorkingSet : public IObjectPool
{

public:
	virtual void clear() override {} 
	virtual std::vector<IShapePtr> getObjects() const override {}
    IShapePtr addObject(IShapePtr s) override { m_shapes_count++; return s;}
	virtual std::string getName() override {}
	virtual void dumpToFile(const std::string&) {}
	virtual void removeObject(IShapePtr) {}
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
    virtual void moveCenterToPoint(QPoint&) {}
};

//Mocking!: 
ShapeCreator::ShapeCreator() {}
ShapeCreator::~ShapeCreator() {}
IShapePtr ShapeCreator::create(ObjectType) {std::shared_ptr<IShape>(return new MockShape);}
    
void Messenger::expose_msg(const LogMsgSeverity& s, const std::string&, bool) {}
void Messenger::log_command(const std::string&, bool) {}

RegionQuery::RegionQuery() {}
void RegionQuery::shutDown() {}
void RegionQuery::insertObject(IShape*) {}
void RegionQuery::removeObject(IShape*) {}

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
bool UT_load_save_commands()
{
    //Expecting!: dicmdDesignSave command to be properly created
    IObjectPoolPtr ws = std::shared_ptr<MockWorkingSet>(new MockWorkingSet);
    dicmdDesignSave cmd(ws);

    //Expecting!: should be called properly with called arguments
    cmd.set_arg("-filename","morqur");
    cmd.execute();
}


int main()
{
    //UT_load_save_commands();
    return 0;
}

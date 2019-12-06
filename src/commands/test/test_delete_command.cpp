#include "../delete_command.hpp"
#include "../core/iobject_pool.hpp"
#include "../core/ishape.hpp"


#include <cassert>
#include <string>
/*! \file */ 

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
void Messenger::expose_msg(const LogMsgSeverity& s, const std::string&, bool) {}
void Messenger::log_command(const std::string&, bool) {}

RegionQuery::RegionQuery() {}
void RegionQuery::shutDown() {}
void RegionQuery::insertObject(IShape*) {}
void RegionQuery::clear() {}
IShape* RegionQuery::getShapeUnderPos(QPoint const&) const { return nullptr; }

void ObjectPoolBase::clear() noexcept {}
std::vector<IShapePtr> ObjectPoolBase::getObjects() const noexcept { std::vector<IShapePtr> res; return res; }
IShapePtr ObjectPoolBase::addObject(IShapePtr s) { return s;}
std::string Design::getName() const noexcept {}
void Design::dumpToFile(const std::string&) const {}
void ObjectPoolBase::removeObject(IShapePtr) noexcept {}

void ServiceManager::shutDown(){}
//!Mocking

/*!
    \brief UT_delete_command is entry point for unit testing delete_command module .

    This unit test covers following steps:

    \list 
        \li step1
        \li step2
    \endlist
*/
bool UT_delete_command()
{
    
    //Expecting!: dicmdDeleteObj command to be created
	ObjectPoolPtr ws = std::shared_ptr<Design>(new Design);
    dicmdDeleteObj cmd(ws);

    //Expecting!: dicmdCreateObj to be executed on point 0,0 without issues
    cmd.set_arg("-point","(0,0)");
    cmd.execute();

    //Expecting!: command should throw exception when argument is invalid
    cmd.set_arg("-p1int","(0,0)");
    cmd.execute();
}


int main() 
{
    //return 0;
    UT_delete_command();
}

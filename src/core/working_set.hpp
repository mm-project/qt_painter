#ifndef WORKING_SET_HPP
#define WORKING_SET_HPP

#include "ishape.hpp"
#include "iobject_pool.hpp"

#include <vector>
#include <QObject>

class WorkingSet : public IObjectPool
{
public:
	//
	//	Interface
	//
	virtual void clear() override;
	virtual std::vector<IShape*> getObjects() const override;
    IShape* addObject(IShape*) override;
	virtual ~WorkingSet() {}
	virtual std::string getName() override;
	virtual void dumpToFile(const std::string&);
	void removeObject(IShape*);

private:
	//
	//	Methods
	// 
	std::vector<IShape*> m_shapes;
};

using WorkingSetPtr = std::shared_ptr<WorkingSet>;

#endif

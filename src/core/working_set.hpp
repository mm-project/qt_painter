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
	void clear() override;
	std::vector<IShape*> getObjects() const override;
	IShape* addObject(IShape*) override;
	virtual ~WorkingSet() {}
	std::string getName() override;
	void dumpToFile(const std::string&);
	void removeObject(IShape*) override;

private:
	//
	//	Methods
	// 
	std::vector<IShape*> m_shapes;
};

using WorkingSetPtr = std::shared_ptr<WorkingSet>;

#endif

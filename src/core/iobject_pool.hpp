#ifndef I_OBJECT_POOL_HPP
#define I_OBJECT_POOL_HPP

///////////////////////////////////////////////////////////////////////////////
//
//	Inludes	
//
#include "ishape.hpp"

//
//Stl
//
#include <vector>
#include <memory>

///////////////////////////////////////////////////////////////////////////////
//
//	Object pool interface
//
class IObjectPool
{
public:
	//
	//	Interface
	//
	virtual ~IObjectPool() = default;
	virtual void clear() = 0;
	virtual std::vector<IShape*> getObjects() const = 0;
	virtual IShape* addObject(IShape*) = 0;
	virtual std::string getName() = 0;
	virtual void removeObject(IShape*) {}
};

using IObjectPoolPtr = std::shared_ptr<IObjectPool>;

#endif

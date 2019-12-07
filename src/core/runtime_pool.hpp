#ifndef RUNTIME_POOL_HPP
#define RUNTIME_POOL_HPP

#include "ishape.hpp"
#include "iobject_pool.hpp"

#include <vector>

class RuntimePool;
using RuntimePoolPtr = std::shared_ptr<RuntimePool>;

class RuntimePoolManager;
using RuntimePoolManagerPtr = std::shared_ptr<RuntimePoolManager>;

class RuntimePool : public ObjectPoolBase
{
public:
	RuntimePool(RuntimePoolManagerPtr = 0);

	//	Inherited interface
	IShapePtr addObject(IShapePtr shape) override;
	std::string getName() const noexcept override;

	// Own interface
	void addPoint(QPoint);
	void movePoint(QPoint);

	void changeBasicProperties(const ShapeProperties&);

	RuntimePoolManagerPtr m_parent;
};

//	not singleton
class RuntimePoolManager  
{
public:
	RuntimePoolManager();
	void addChild(RuntimePoolPtr, const std::string&);
	RuntimePoolPtr getChild(const std::string&) const;

	std::map<std::string, RuntimePoolPtr> getChildren() const noexcept;

private:
	std::map<std::string, RuntimePoolPtr> m_children;
};

#endif

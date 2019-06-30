#ifndef RUNTIME_ENVIRONMENT_HPP
#define RUNTIME_ENVIRONMENT_HPP

#include "basic_shape.hpp"
#include "iobject_pool.hpp"

#include <vector>

class ObjectSandbox;
using ObjectSandboxPtr = std::shared_ptr<ObjectSandbox>;

class ObjectPoolSandbox;
using ObjectPoolSandboxPtr = std::shared_ptr<ObjectPoolSandbox>;

class ObjectSandbox
{
public:
	ObjectSandbox(ObjectPoolSandboxPtr = 0);
	IObjectPoolPtr getPool() const;
	void clear();
	void addPoint(QPoint);

	void movePoint(QPoint);

	void addObject(IShape*);
	void changeBasicProperties(const ShapeProperties&);

private:
	IObjectPoolPtr m_pool;
	ObjectPoolSandboxPtr m_parent;
};

class ObjectPoolSandbox
{
public:
	ObjectPoolSandbox();
	std::vector<ObjectSandboxPtr> getChildren() const;
	void addChildren(ObjectSandboxPtr);
	
private:
	std::vector<ObjectSandboxPtr> m_children;
};

#endif

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
	void addObject(IShape*);
	/*void reset();

	void change_object_type(ObjectType);

	IShape* get_runtime_object() const;

	void set_pos1(const QPoint&);
	void set_pos2(const QPoint&);

	void draw_runtime(QPainter* p);

	void change_basic_properties(ShapeProperties);*/
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

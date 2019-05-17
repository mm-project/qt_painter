#ifndef SHAPE_CREATOR_HPP
#define SHAPE_CREATOR_HPP

#include <memory>

#include "basic_shape.hpp"

//
//	class ShapeCreator
//	based on AbstractFactoryIdea
//
class ShapeCreator;
using ShapeCreatorPtr = std::shared_ptr<ShapeCreator>;

class ShapeCreator
{
public:
	~ShapeCreator();

public:
	static ShapeCreatorPtr getInstance();
	IShape* create(ObjectType);

private:
	ShapeCreator();
	ShapeCreator(const ShapeCreator&) = default;
	ShapeCreator& operator=(const ShapeCreator&) = default;

private:
	static ShapeCreatorPtr m_instance;
	IShape* m_line;
	IShape* m_rect;
	IShape* m_ellipse;
	IShape* m_polygon;
};
#endif
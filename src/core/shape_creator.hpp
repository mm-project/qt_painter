#ifndef SHAPE_CREATOR_HPP
#define SHAPE_CREATOR_HPP

#include <memory>

#include "ishape.hpp"
#include "service.hpp"

//
//	class ShapeCreator
//	based on AbstractFactoryIdea
//
class ShapeCreator : public Service<ShapeCreator>
{
public:
	ShapeCreator();
	virtual ~ShapeCreator();

	IShape* create(ObjectType);

private:
	IShape* m_line;
	IShape* m_rect;
	IShape* m_ellipse;
	IShape* m_polygon;
};
#endif

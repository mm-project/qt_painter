#ifndef SHAPE_CREATOR_HPP
#define SHAPE_CREATOR_HPP

#include <memory>

#include "../ishape.hpp"
#include "../service.hpp"

//
//	class ShapeCreator
//	based on AbstractFactoryIdea
//
class ShapeCreator : public Service<ShapeCreator>
{
public:
	ShapeCreator();
	virtual ~ShapeCreator();

	IShapePtr create(ObjectType);

private:
	IShapePtr m_line;
	IShapePtr m_rect;
	IShapePtr m_ellipse;
	IShapePtr m_polygon;
};
#endif

#include "shape_creator.hpp"
#include "shapes.hpp"

ShapeCreator::ShapeCreator()
{
	m_line = std::shared_ptr<IShape>(new Line);
	m_rect = std::shared_ptr<IShape>(new Rectangle);
	m_ellipse = std::shared_ptr<IShape>(new Ellipse);
	m_polygon = std::shared_ptr<IShape>(new Polygon);
}

ShapeCreator::~ShapeCreator()
{}

IShapePtr ShapeCreator::create(ObjectType t)
{
	switch (t)
	{
	case LINE:
		return std::shared_ptr<IShape>(m_line->clone());
	case RECTANGLE:
		return std::shared_ptr<IShape>(m_rect->clone());
	case ELLIPSE:
		return std::shared_ptr<IShape>(m_ellipse->clone());
	case POLYGON:
		return std::shared_ptr<IShape>(m_polygon->clone());
	}
	return nullptr;
}

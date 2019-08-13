#include "shape_creator.hpp"
#include "shapes.hpp"

ShapeCreatorPtr ShapeCreator::m_instance = 0;

ShapeCreator::ShapeCreator()
{
	m_line = new line;
	m_rect = new rectangle;
	m_ellipse = new ellipse;
	m_polygon = new polygon;
}

ShapeCreator::~ShapeCreator()
{
	delete m_line;
	delete m_rect;
	delete m_ellipse;
	delete m_polygon;
}

ShapeCreatorPtr ShapeCreator::getInstance()
{
	if (m_instance == nullptr)
		m_instance = std::shared_ptr<ShapeCreator>(new ShapeCreator);
	return m_instance;
}

IShape* ShapeCreator::create(ObjectType t)
{
	switch (t)
	{
	case LINE:
		return m_line->clone();
	case RECTANGLE:
		return m_rect->clone();
	case ELLIPSE:
		return m_ellipse->clone();
	case POLYGON:
		return m_polygon->clone();
	}
	return nullptr;
}

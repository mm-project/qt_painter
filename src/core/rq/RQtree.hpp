#ifndef RQ_PLUGIN_HPP
#define RQ_PLUGIN_HPP

// Includes
#include "node.hpp"
// Stl
#include <vector>
#include <queue>
#include <iostream>
#include <iomanip>

namespace rq {
//
//	class RQtree
//	Implemets kd-tree
//	A tree can be contructed from a set of data of RQobjects
//
template <typename T>
class RQtree
{
public:
	//
	//	Methods
	//

	//	Constructors
	RQtree() = default;
	explicit RQtree(const std::vector<RQobjectPtr>&);

	//	Destructor
	virtual ~RQtree()
	{
	}

public:
	//	Insert point into the kd-tree
	void insert(const RQobjectPtr&);

	//	Removes point from the kd-tree
	void remove(const RQobjectPtr&);

	//	Checks if the point already exists in the kd-tree
	bool search(const RQobjectPtr&) const;

	void print() const;

	bool empty()  const;

	RQobjectPtr getObject(const CPoint& p) const;

	std::vector<RQobjectPtr> getObjects(const QRect& oRect) const;

	//CPoint<T> nearest_point(const CPoint<T>&);

	//	Returns 2 left and right childs
	std::vector<RQobjectPtr> nearest_points(const RQobjectPtr&) const;

	void clear();

private:
	//
	//	Helpers
	//

	void build_tree(const std::vector<RQobjectPtr>&);
	CNodePtr<T> _insert(CNodePtr<T>&, const RQobjectPtr&, int);
	bool _search(CNodePtr<T>, const RQobjectPtr&, int) const;

	void print_postorder(CNodePtr<T>, int = 0) const;
	std::vector<RQobjectPtr> _nearest_points(CNodePtr<T>, const RQobjectPtr&, int) const;
	RQobjectPtr _getObject(CNodePtr<T>, const CPoint& p, int) const;
	void _getObjects(CNodePtr<T>, const QRect&, int, std::vector<RQobjectPtr>&) const;
	CNodePtr<T> _remove(CNodePtr<T>&, const RQobjectPtr&, int);
	void removeChild(CNodePtr<T>&, CNodePtr<T>&, int);
	CNodePtr<T> findMin(CNodePtr<T>&, int, int) const;
	CNodePtr<T> findMax(CNodePtr<T>&, int, int) const;
private:
	//
	//	Contents
	//
	CNodePtr<T>	m_root;
};

template <typename T>
using RQtreePtr = std::shared_ptr<RQtree<T>>;

template <typename T>
RQtree<T>::RQtree(const std::vector<RQobjectPtr>& points)
{
	build_tree(points);
}

template <typename T>
void RQtree<T>::build_tree(const std::vector<RQobjectPtr>& points)
{
	for (auto point : points)
		insert(point);
}

template <typename T>
void RQtree<T>::insert(const RQobjectPtr& point)
{
	_insert(m_root, point, 0);
}

template <typename T>
void RQtree<T>::remove(const RQobjectPtr& object)
{
	_remove(m_root, object, 0);
}

template <typename T>
CNodePtr<T> RQtree<T>::_insert(CNodePtr<T>& root, const RQobjectPtr& point, int depth)
{
	// make the root
	if (root == nullptr)
	{
		root = std::shared_ptr<CNode<T>>(new CNode<T>(point));
		return root;
	}

	int cd = depth % 2; //change it , 2 means point size(x, y) :D

	if (point->at(cd) < root->m_object->at(cd))
		root->m_left_ptr = _insert(root->m_left_ptr, point, depth + 1);
	else
		root->m_right_ptr = _insert(root->m_right_ptr, point, depth + 1);

	return root;
}

template <typename T>
void RQtree<T>::print() const
{
	print_postorder(m_root);
}

template <typename  T>
void RQtree<T>::print_postorder(CNodePtr<T> node, int indent) const
{
	if (node != nullptr)
	{
		std::cout << "( " << node->m_point.x() << " , " << node->m_point.y() << " )" << "\n";
		if (indent)
			std::cout << std::setw(indent) << ' ';

		if (node->m_right_ptr != nullptr)
			print_postorder(node->m_right_ptr, indent + 4);

		if (indent)
			std::cout << std::setw(indent) << ' ';

		if (node->m_left_ptr != nullptr)
			print_postorder(node->m_left_ptr, indent + 4);
	}
}

template <typename T>
bool RQtree<T>::search(const RQobjectPtr& point) const
{
	return _search(m_root, point, 0);
}

template <typename T>
bool RQtree<T>::_search(CNodePtr<T> node, const RQobjectPtr& point, int depth) const
{
	if (node == nullptr)
		return false;

	if (node->m_object == point)
		return true;

	int cd = depth % 2;

	if (point->at(cd) < node->m_object->at(cd))
		return _search(node->m_left_ptr, point, depth + 1);

	return _search(node->m_right_ptr, point, depth + 1);
}

template <typename T>
bool RQtree<T>::empty() const
{
	return m_root == nullptr;
}

template <typename T>
std::vector<RQobjectPtr> RQtree<T>::nearest_points(const RQobjectPtr& point) const
{
	return _nearest_points(m_root, point, 0);
}

template <typename T>
std::vector<RQobjectPtr> RQtree<T>::_nearest_points(CNodePtr<T> node, const RQobjectPtr& point, int depth) const
{
	if (node == nullptr)
		return std::vector<RQobjectPtr>();

	if (node->m_point == point)
	{
		std::vector<RQobjectPtr> nearests;

		/*if (node->m_left_ptr != nullptr)
			nearests.push_back(node->m_left_ptr->m_point);

		if (node->m_right_ptr != nullptr)
			nearests.push_back(node->m_right_ptr->m_point);*/

		return nearests;
	}

	int cd = depth % 2;

	if (point->at(cd) < node->m_object->at(cd))
		return _nearest_points(node->m_left_ptr, point, depth + 1);

	return _nearest_points(node->m_right_ptr, point, depth + 1);
}

template <typename T>
RQobjectPtr RQtree<T>::getObject(const CPoint& p) const
{
	return _getObject(m_root, p, 0);
}

template <typename T>
RQobjectPtr RQtree<T>::_getObject(CNodePtr<T> node, const CPoint& point, int depth) const
{
	if (node == nullptr)
		return nullptr;

	if (node->m_object->contains(point))
		return node->m_object;

	int cd = depth % 2;

	if (point < node->m_object->at(cd))
		return _getObject(node->m_left_ptr, point, depth + 1);

	return _getObject(node->m_right_ptr, point, depth + 1);
}

template <typename T>
std::vector<RQobjectPtr> RQtree<T>::getObjects(const QRect& oRect) const
{
	std::vector<RQobjectPtr> shapes;
	_getObjects(m_root, oRect, 0, shapes);
	return shapes;
}

template <typename T>
void  RQtree<T>::_getObjects(CNodePtr<T> node, const QRect& oRect, int depth, std::vector<RQobjectPtr>& shapes) const
{
	if (node == nullptr)
		return;

	if (node->m_object->intersects(oRect))
		shapes.push_back(node->m_object);

	int cd = depth % 2;

	CPoint point = CPoint(oRect.bottomRight());
	if (cd == 0)
		point = CPoint(oRect.topLeft());

	//if (point < node->m_object->at(cd))
		_getObjects(node->m_left_ptr, oRect, depth + 1, shapes);

	return _getObjects(node->m_right_ptr, oRect, depth + 1, shapes);
}

template <typename T>
void RQtree<T>::clear()
{
	m_root = nullptr;
}

template <typename T>
CNodePtr<T> RQtree<T>::_remove(CNodePtr<T>& node, const RQobjectPtr& object, int depth)
{
	if (node == nullptr)
		return nullptr;

	int cd = depth % 2;

	//temporary solution, you can improve it
	if (node->m_right_ptr != nullptr)
	{
		if (node->m_right_ptr->m_object->getObject() == object->getObject())
		{
			auto ptr = node->m_right_ptr;
			node->m_right_ptr = nullptr;
			std::queue<CNodePtr<T>> nodes;
			if (ptr->m_left_ptr != nullptr)
				nodes.push(ptr->m_left_ptr);
			if (ptr->m_right_ptr != nullptr)
				nodes.push(ptr->m_right_ptr);
			while (!nodes.empty())
			{
				auto p = nodes.front();
				nodes.pop();
				insert(p->m_object);
				if (p->m_left_ptr != nullptr)
					nodes.push(p->m_left_ptr);
				if (p->m_right_ptr != nullptr)
					nodes.push(p->m_right_ptr);
			}
			return nullptr;
		}
	}
	else if (node->m_left_ptr != nullptr)
	{
		if (node->m_left_ptr->m_object->getObject() == object->getObject())
		{
			auto ptr = node->m_left_ptr;
			node->m_left_ptr = nullptr;
			std::queue<CNodePtr<T>> nodes;
			if (ptr->m_left_ptr != nullptr)
				nodes.push(ptr->m_left_ptr);
			if (ptr->m_right_ptr != nullptr)
				nodes.push(ptr->m_right_ptr);
			while (!nodes.empty())
			{
				auto p = nodes.front();
				nodes.pop();
				if (p->m_left_ptr != nullptr)
					nodes.push(p->m_left_ptr);
				if (p->m_right_ptr != nullptr)
					nodes.push(p->m_right_ptr);
			}
			return nullptr;
		}
	}

	// case wirh root
	if (m_root->m_object->getObject() == object->getObject())
	{
			auto ptr = m_root;
			m_root = nullptr;
			std::queue<CNodePtr<T>> nodes;
			if (ptr->m_left_ptr != nullptr)
				nodes.push(ptr->m_left_ptr);
			if (ptr->m_right_ptr != nullptr)
				nodes.push(ptr->m_right_ptr);
			while (!nodes.empty())
			{
				auto p = nodes.front();
				nodes.pop();
				insert(p->m_object);
				if (p->m_left_ptr != nullptr)
					nodes.push(p->m_left_ptr);
				if (p->m_right_ptr != nullptr)
					nodes.push(p->m_right_ptr);
			}
			return nullptr;
		
	}


	//if (node->m_object->getObject() == object->getObject() || node->m_object == object)
	//{
	//	// delete node
	//	if (node->m_right_ptr != nullptr)
	//	{
	//		auto min = findMin(node->m_right_ptr, cd);
	//		removeChild(m_root, min, cd);
	//		return node->m_right_ptr = _remove(node->m_right_ptr, min->m_object, depth + 1);
	//	}
	//	else if (node->m_left_ptr != nullptr)
	//	{
	//		auto min = findMax(node->m_left_ptr, cd);
	//		removeChild(m_root, min, cd);
	//		return node->m_left_ptr = _remove(node->m_left_ptr, min->m_object, depth + 1);
	//	}
	//	else
	//	{
	//		//delete node;
	//		return nullptr;
	//	}

	//}

	if (object->at(cd) < node->m_object->at(cd))
		return _remove(node->m_left_ptr, object, depth + 1);

	return _remove(node->m_right_ptr, object, depth + 1);
}

template <typename T>
CNodePtr<T> RQtree<T>::findMin(CNodePtr<T>& node, int depth, int d) const
{
	if (node == nullptr)
		return nullptr;

	int cd = depth % 2;
	
	if (cd == d) {
		if (node->m_left_ptr == nullptr)
			return node;
		return findMin(node->m_left_ptr, depth + 1, d);
	}

	return findMin(node->m_left_ptr, depth + 1);
}

template <typename T>
CNodePtr<T> RQtree<T>::findMax(CNodePtr<T>& node, int depth, int d) const
{
	if (node == nullptr)
		return nullptr;

	if (node->m_right_ptr == nullptr)
		return node;

	return findMin(node->m_right_ptr, depth + 1);
}

template <typename T>
void RQtree<T>::removeChild(CNodePtr<T>& root, CNodePtr<T>& object, int depth)
{
	if (object == nullptr || root == nullptr)
		return;

	if (root->m_right_ptr != nullptr && root->m_right_ptr == object)
		root->m_right_ptr = nullptr;

	if (root->m_left_ptr != nullptr && root->m_left_ptr == object)
		root->m_left_ptr = nullptr;

	int cd = depth % 2;
	if (object->m_object->at(cd) < root->m_object->at(cd))
		return removeChild(root->m_left_ptr, object, depth + 1);

	return removeChild(root->m_right_ptr, object, depth + 1);
}
}
#endif

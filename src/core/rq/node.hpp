#ifndef node_hpp
#define node_hpp

#include "rq_object.hpp"

#include <memory>

namespace rq {

template <typename T>
class CNode;

template <typename T>
using CNodePtr = std::shared_ptr<CNode<T>>;

template <typename T>
class CNode
{
public:
	CNode(const RQobjectPtr& = 0, CNodePtr<T> = 0, CNodePtr<T> = 0);
	~CNode() = default;

	RQobjectPtr m_object;
	CNodePtr<T> m_left_ptr;
	CNodePtr<T> m_right_ptr;
};

template <typename T>
CNode<T>::CNode(const RQobjectPtr& p, CNodePtr<T> left, CNodePtr<T> right)
	: m_object(p)
	, m_left_ptr(left)
	, m_right_ptr(right)
{}

}

#endif

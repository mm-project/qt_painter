#pragma once

#include <memory>
#include <vector>
#include <stack>

// TODO: change this
#include <QPoint>
#include <QRect>

template <typename T> 
class KDnode;
template <typename T> 
using KDnodePtr = std::shared_ptr< KDnode < T> >;

template <typename T>
class KDtree;
template <typename T>
using KDtreePtr =  std::shared_ptr< KDtree < T > >; 

template <typename T>
struct KDnode
{
    /* data */
    KDnodePtr<T>    pLeft;
    KDnodePtr<T>    pRight;
    T               object; // IShape : should provide functions centroid and boundary
};

template <typename T>
bool compare( const T& obj1, const T& obj2, int nDimension )
{
    auto oCenter1 = obj1->center();
    auto oCenter2 = obj2->center();
    if ( nDimension == 0 )
        return oCenter1.x() < oCenter2.x();
    return oCenter1.y() < oCenter2.y();
}

template <typename T>
bool compare( const QPoint& point, const T& obj, int nDimension )
{
    auto oCenter = obj->center();
    if ( nDimension == 0 )
        return point.x() < oCenter.x();
    return point.y() < oCenter.y();
}

template <typename T>
bool compare( const QRect& oRect, const T& obj, int nDimension )
{
    auto oCenter1 = oRect.center();
    auto oCenter2 = obj->center();
    if ( nDimension == 0 )
        return oCenter1.x() < oCenter2.x();
    return oCenter1.y() < oCenter2.y();
}

template <typename T>
bool compare( KDnodePtr<T> pLeft, KDnodePtr<T> pRight, int nDimension )
{
    return compare( pLeft->object, pRight->object, nDimension );
}

template <typename T>
class KDtree
{
public:
    KDtree() = default;
    explicit KDtree( const std::vector<T> arrObjects );

public:
    void insert( const std::vector<T> arrObjects );
    void insert( const T& object );
    void clear();

    void remove( const T& object );

    std::vector<T> query( const QPoint& ) const;
    std::vector<T> query( const QRect& ) const;

private:
    void initRoot();

private:
    KDnodePtr<T>    m_pRoot;
};

template <typename T>
KDtree<T>::KDtree( const std::vector<T> arrObjecrs )
{
    insert( arrObjecrs );
}

template <typename T>
void KDtree<T>::initRoot()
{
    if ( m_pRoot != nullptr )
        return;

    m_pRoot = std::make_shared<KDnode <T> >();
}

template <typename T>
void KDtree<T>::insert( const std::vector<T> arrObjecrs )
{
    initRoot();
    for ( const auto& object : arrObjecrs )
    {
        insert( object );
    }
}

template <typename T>
void KDtree<T>::insert( const T& object )
{
    initRoot();
    if ( m_pRoot->object == nullptr )
    {
        m_pRoot->object = object;
        return;
    }

    auto pNode = m_pRoot;
    auto pNodePrev = m_pRoot;
    bool bLeft = true;
    auto nDepth = 0;
    while ( pNode != nullptr )
    {
        pNodePrev = pNode;
        const auto dimension = nDepth % 2;
        ++nDepth;
        if ( compare( object, pNode->object, dimension ) )
        {
            bLeft = true;
            pNode = pNode->pLeft;
        }
        else
        {
            bLeft = false;
            pNode = pNode->pRight;
        }
    }
    pNode = std::make_shared< KDnode< T > >();
    pNode->object = object;
    if ( bLeft )
    {
        pNodePrev->pLeft = pNode;
    }
    else
    {
        pNodePrev->pRight = pNode;
    }
}

template <typename T>
void KDtree<T>::clear()
{
    m_pRoot = nullptr;
}

template <typename T>
std::vector<T> KDtree<T>::query( const QPoint& oPoint ) const
{
    std::vector<T> result;
    auto pNode = m_pRoot;
    auto nDepth = 0;
    while ( pNode != nullptr )
    {
        if ( pNode->object->contains( oPoint ) )
            result.emplace_back( pNode->object );

        const auto dimension = nDepth % 2;
        ++nDepth;
        if ( compare( oPoint, pNode->object, dimension ) )
            pNode = pNode->pLeft;
        else
            pNode = pNode->pRight;
    }
    return result;
}

template <typename T>
std::vector<T> KDtree<T>::query( const QRect& oRect ) const
{
    std::vector<T> result;
    std::stack< KDnodePtr<T> > nodes;
    nodes.push( m_pRoot );
    while ( !nodes.empty() )
    {
        auto pNode = nodes.top();
        nodes.pop();
        if ( pNode == nullptr )
            continue;

        //if ( pNode->object->isDisjointFrom( oRect ) )
        //    continue;

        if ( pNode->object->intersects( oRect ) )
            result.emplace_back( pNode->object );

        nodes.push( pNode->pLeft );
        nodes.push( pNode->pRight );
    }
    return result;
}

template <typename T>
void KDtree<T>::remove( const T& object )
{
    // TODO: make it better
    std::vector<T> shapes_to_insert;
    KDnodePtr<T> pShapeToDelete;
    std::stack< KDnodePtr<T> > nodes;
    nodes.push( m_pRoot );
    while ( !nodes.empty() )
    {
        auto pNode = nodes.top();
        nodes.pop();
        if ( pNode == nullptr )
            continue;

        if ( pNode->pLeft != nullptr && pNode->pLeft->object == object )
        {
            pShapeToDelete = pNode->pLeft;
            pNode->pLeft = nullptr; // remove the link
            break;
        }

        if ( pNode->pRight != nullptr && pNode->pRight->object == object )
        {
            pShapeToDelete = pNode->pRight;
            pNode->pRight = nullptr; // remove the link
            break;
        }

        if ( pNode->object == object )
        {
            m_pRoot = nullptr;
            pShapeToDelete = pNode;
            break;
        }
        nodes.push( pNode->pLeft );
        nodes.push( pNode->pRight );
    }
           
    nodes = {};

    nodes.push( pShapeToDelete );
    while ( !nodes.empty() )
    {
        auto pNode = nodes.top();
        nodes.pop();
        if ( pNode == nullptr )
            continue;

        nodes.push( pNode->pLeft );
        nodes.push( pNode->pRight );
        shapes_to_insert.emplace_back( pNode->object );
    }

    pShapeToDelete = nullptr;
    // skip the first shape
    for ( size_t i = 1; i < shapes_to_insert.size(); ++i )
    {
        insert( shapes_to_insert[i] );
    }
}
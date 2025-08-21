#pragma once

#include "rqtree_interface.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <functional>
#include <limits>
#include <memory>
#include <optional>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace ds {

template <class Scalar>
struct AABB {
    static_assert(std::is_floating_point<Scalar>::value, "Scalar must be floating point");
    Scalar minx{}, miny{}, maxx{}, maxy{}; // closed box semantics

    Scalar width()  const { return maxx - minx; }
    Scalar height() const { return maxy - miny; }

    std::pair<Scalar,Scalar> center() const { return { (minx+maxx)/Scalar(2), (miny+maxy)/Scalar(2) }; }

    bool containsPoint(Scalar x, Scalar y, Scalar eps = Scalar(0)) const {
        return (x >= minx - eps && x <= maxx + eps && y >= miny - eps && y <= maxy + eps);
    }
    bool containsBox(const AABB& b) const {
        return b.minx >= minx && b.maxx <= maxx && b.miny >= miny && b.maxy <= maxy;
    }
    bool intersects(const AABB& b) const {
        return !(b.minx > maxx || b.maxx < minx || b.miny > maxy || b.maxy < miny);
    }
};

// Distance from point to AABB (squared)
template <class Scalar>
inline Scalar dist2_point_aabb(Scalar x, Scalar y, const AABB<Scalar>& r) {
    Scalar dx = (x < r.minx ? r.minx - x : (x > r.maxx ? x - r.maxx : Scalar(0)));
    Scalar dy = (y < r.miny ? r.miny - y : (y > r.maxy ? y - r.maxy : Scalar(0)));
    return dx*dx + dy*dy;
}

// -----------------------------
// Shape Traits
// -----------------------------
template <class T, class Scalar>
struct DefaultShapeTraits {
    static AABB<Scalar> aabb(const T& t) {
        return { t.minx(), t.miny(), t.maxx(), t.maxy() };
    }
    static bool contains_point(const T& t, Scalar x, Scalar y) {
        auto b = aabb(t);
        return b.containsPoint(x,y);
    }
    static std::pair<Scalar,Scalar> anchor(const T& t) {
        auto b = aabb(t);
        return b.center();
    }
};

// Specialization for IShapePtr (std::shared_ptr<IShape>)
// In case QRectF is not covering all the cases specialize this template
template <class Scalar>
struct DefaultShapeTraits<std::shared_ptr<IShape>, Scalar> {
    using T = std::shared_ptr<IShape>;
    
    static AABB<Scalar> aabb(const T& shape) {
        auto bbox = shape->getBBox();
        return { 
            static_cast<Scalar>(bbox.left()), 
            static_cast<Scalar>(bbox.top()),
            static_cast<Scalar>(bbox.right()), 
            static_cast<Scalar>(bbox.bottom()) 
        };
    }
    
    static bool contains_point(const T& shape, Scalar x, Scalar y) {
        QPoint pt(static_cast<int>(x), static_cast<int>(y));
        return shape->contains(pt);
    }
    
    static std::pair<Scalar,Scalar> anchor(const T& shape) {
        auto center = shape->center();
        return { static_cast<Scalar>(center.x()), static_cast<Scalar>(center.y()) };
    }
};

// -----------------------------
// Dynamic Quadtree for SHAPES
// -----------------------------
template <class T, class Scalar = double, class Traits = DefaultShapeTraits<T,Scalar>>
class Quadtree {
public:
    using Box = AABB<Scalar>;

    explicit Quadtree(Box world,
                      std::size_t capacity = 8,
                      std::size_t maxDepth = 16,
                      std::size_t mergeThreshold = 3)
        : world_(std::move(world)), capacity_(capacity), maxDepth_(maxDepth), mergeThreshold_(mergeThreshold) {
        root_ = std::make_unique<Node>(nullptr, world_, 0);
    }

    // ------------ Basic ops ------------
    void clear() {
        root_.reset();
        root_ = std::make_unique<Node>(nullptr, world_, 0);
        size_ = 0;
    }

    void insert(const T& obj) { insertOne(obj); }

    void insert(const std::vector<T>& objs) {
        for (const auto& o : objs) insertOne(o);
    }

    void remove(const T& object) {
        removeFromNode(root_.get(), object);
    }

    // ------------ Queries ------------
    template <class F>
    void queryRange(const Box& range, F&& cb) const {
        queryRangeNode(root_.get(), range, std::forward<F>(cb));
    }

    template <class F>
    void queryPoint(Scalar x, Scalar y, F&& cb) const {
        queryPointNode(root_.get(), x, y, std::forward<F>(cb));
    }

    std::size_t size() const noexcept { return size_; }
    const Box& bounds() const { return world_; }

private:
    struct Node {
        Node* parent;
        Box box;
        std::size_t depth;
        std::array<std::unique_ptr<Node>, 4> child{}; // NW NE SW SE
        std::vector<T> items; // shapes stored here when they straddle boundaries or node is leaf
        std::size_t subtreeCount{0};
        Node(Node* p, const Box& b, std::size_t d) : parent(p), box(b), depth(d) {}
        bool isLeaf() const { return child[0] == nullptr; }
    };

    std::unique_ptr<Node> root_;
    Box world_;
    std::size_t capacity_;
    std::size_t maxDepth_;
    std::size_t mergeThreshold_;
    std::size_t size_ = 0;

    int childIndexForShape(const Node* n, const T& obj) const {
        auto c = n->box.center();
        const Scalar midx = c.first;
        const Scalar midy = c.second;
        const Box bb = Traits::aabb(obj);

        Box nw{ n->box.minx, n->box.miny, midx,        midy };
        Box ne{ midx,        n->box.miny, n->box.maxx, midy };
        Box sw{ n->box.minx, midy,        midx,        n->box.maxy };
        Box se{ midx,        midy,        n->box.maxx, n->box.maxy };
        if (nw.containsBox(bb)) return 0;
        if (ne.containsBox(bb)) return 1;
        if (sw.containsBox(bb)) return 2;
        if (se.containsBox(bb)) return 3;
        return -1;
    }

    void subdivide(Node* n) {
        if (!n->isLeaf()) return;
        auto c = n->box.center();
        Scalar midx = c.first;
        Scalar midy = c.second;
        n->child[0] = std::make_unique<Node>(n, Box{ n->box.minx, n->box.miny, midx,        midy }, n->depth+1);
        n->child[1] = std::make_unique<Node>(n, Box{ midx,        n->box.miny, n->box.maxx, midy }, n->depth+1);
        n->child[2] = std::make_unique<Node>(n, Box{ n->box.minx, midy,        midx,        n->box.maxy }, n->depth+1);
        n->child[3] = std::make_unique<Node>(n, Box{ midx,        midy,        n->box.maxx, n->box.maxy }, n->depth+1);

        std::vector<T> remaining;
        remaining.reserve(n->items.size());
        for (const auto& it : n->items) {
            int idx = childIndexForShape(n, it);
            if (idx < 0) { remaining.push_back(it); continue; }
            n->child[std::size_t(idx)]->items.push_back(it);
            ++n->child[std::size_t(idx)]->subtreeCount;
        }
        n->items.swap(remaining);
    }

    void tryMerge(Node* n) {
        if (!n || n->isLeaf()) return;
        bool childrenAreLeaves = true;
        std::size_t total = n->items.size();
        for (int i=0;i<4;++i) {
            Node* ch = n->child[std::size_t(i)].get();
            if (!ch->isLeaf()) { childrenAreLeaves = false; break; }
            total += ch->items.size();
        }
        if (childrenAreLeaves && (total <= mergeThreshold_ || total <= capacity_)) {
            for (int i=0;i<4;++i) {
                Node* ch = n->child[std::size_t(i)].get();
                for (auto& it : ch->items) n->items.push_back(std::move(it));
                n->child[std::size_t(i)].reset();
            }
            n->subtreeCount = n->items.size();
        }
    }

    void insertIntoNode(Node* n, const T& obj) {
        if (!n->isLeaf()) {
            int idx = childIndexForShape(n, obj);
            if (idx >= 0) {
                insertIntoNode(n->child[std::size_t(idx)].get(), obj);
                return;
            }
        }
        if (n->isLeaf() && (n->items.size() >= capacity_) && (n->depth < maxDepth_)) {
            subdivide(n);
            insertIntoNode(n, obj);
            return;
        }
        n->items.push_back(obj);
        for (Node* p = n; p; p = p->parent) ++p->subtreeCount;
        ++size_;
    }

    void insertOne(const T& obj) {
        insertIntoNode(root_.get(), obj);
    }

    bool removeFromNode(Node* n, const T& object) {
        auto& v = n->items;
        auto it = std::find(v.begin(), v.end(), object);
        if (it != v.end()) {
            v.erase(it);
            for (Node* p = n; p; p = p->parent) { --p->subtreeCount; }
            if (size_) --size_;
            for (Node* p = n; p; p = p->parent) tryMerge(p);
            return true;
        }
        if (n->isLeaf()) return false;
        for (int i=0;i<4;++i) if (removeFromNode(n->child[std::size_t(i)].get(), object)) return true;
        return false;
    }

    template <class F>
    void queryRangeNode(const Node* n, const Box& range, F&& cb) const {
        if (!n->box.intersects(range)) return;
        for (const auto& it : n->items) {
            if (range.intersects(Traits::aabb(it))) cb(it);
        }
        if (n->isLeaf()) return;
        for (int i=0;i<4;++i) queryRangeNode(n->child[std::size_t(i)].get(), range, cb);
    }

    template <class F>
    void queryPointNode(const Node* n, Scalar x, Scalar y, F&& cb) const {
        if (!n->box.containsPoint(x,y)) return;
        for (const auto& it : n->items) {
            auto bb = Traits::aabb(it);
            if (!bb.containsPoint(x,y)) continue;
            if (Traits::contains_point(it, x, y)) cb(it);
        }
        if (n->isLeaf()) return;
        for (int i=0;i<4;++i) queryPointNode(n->child[std::size_t(i)].get(), x, y, cb);
    }
};

} // namespace ds

// Qt adapter
template <class T, class Scalar = double>
class QtShapeQuadtree : public IRQtree<T> {
public:
    using Core = ds::Quadtree<T, Scalar, ds::DefaultShapeTraits<T, Scalar>>;
    using Box  = ds::AABB<Scalar>;

    explicit QtShapeQuadtree(Box world = Box{-10000.0, -10000.0, 10000.0, 10000.0},
                             std::size_t capacity = 8,
                             std::size_t maxDepth = 16,
                             std::size_t mergeThreshold = 3)
        : core_(world, capacity, maxDepth, mergeThreshold) {}

    void insert(const std::vector<T>& arrObjects) override { 
        for (const auto& o : arrObjects) core_.insert(o); 
    }
    
    void insert(const T& object) override { 
        core_.insert(object); 
    }

    void clear() override { 
        core_.clear(); 
    }

    void remove(const T& object) override { 
        core_.remove(object); 
    }

    std::vector<T> query(const QPoint& point) const noexcept override {
        std::vector<T> out;
        Scalar x = static_cast<Scalar>(point.x());
        Scalar y = static_cast<Scalar>(point.y());
        core_.queryPoint(x, y, [&](const T& t){ out.push_back(t); });
        return out;
    }

    std::vector<T> query(const QRect& rect) const noexcept override {
        std::vector<T> out;
        const Scalar l = static_cast<Scalar>(rect.left());
        const Scalar t = static_cast<Scalar>(rect.top());
        const Scalar r = static_cast<Scalar>(rect.right());
        const Scalar b = static_cast<Scalar>(rect.bottom());
        core_.queryRange(Box{l,t,r,b}, [&](const T& t){ out.push_back(t); });
        return out;
    }

    std::size_t getSize() const noexcept override { 
        return core_.size(); 
    }

private:
    Core core_;
};

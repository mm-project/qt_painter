#pragma once

#include <vector>

// TODO: change this
#include <QPoint>
#include <QRect>

template <typename T> class IRQtree {
  public:
    virtual void insert(const std::vector<T> &arrObjects) = 0;
    virtual void insert(const T &object) = 0;

    virtual void clear() = 0;
    virtual void remove(const T &object) = 0;

    virtual std::vector<T> query(const QPoint &) const noexcept = 0;
    virtual std::vector<T> query(const QRect &) const noexcept = 0;

    virtual size_t getSize() const noexcept = 0;
};

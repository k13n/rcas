#pragma once
#ifndef CASXML_ALLOCATOR_H_
#define CASXML_ALLOCATOR_H_

#include <atomic>
#include <memory>
#include <iostream>

namespace casxml {

extern std::atomic_size_t btree_memory_used;

// thanks to
// https://medium.com/@vgasparyan1995/what-is-an-allocator-c8df15a93ed

template<typename T>
class Allocator : public std::allocator<T> {
private:
  using Base     = std::allocator<T>;
  using Pointer  = typename std::allocator_traits<Base>::pointer;
  using SizeType = typename std::allocator_traits<Base>::size_type;

public:
  Allocator() = default;

  template<typename U>
  Allocator(const Allocator<U>& other)
      : Base(other)
  {}

  template<typename U>
  struct rebind {
    using other = Allocator<U>;
  };

  Pointer allocate(SizeType n) {
    btree_memory_used.fetch_add(n * sizeof(T));
    return Base::allocate(n);
  }

  void deallocate(Pointer p, SizeType n) {
    btree_memory_used.fetch_sub(n * sizeof(T));
    Base::deallocate(p, n);
  }

};


}; // namespace xml

#endif // CASXML_ALLOCATOR_H_

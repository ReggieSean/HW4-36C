#ifndef PQUEUE_H_
#define PQUEUE_H_

#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>
#include <utility>

template <typename T, typename C = std::less<T>>
class PQueue {
 public:
  // Constructor
  PQueue() {}
  // Return number of items in priority queue
  size_t Size();
  // Return top of priority queue
  T& Top();
  // Remove top of priority queue
  void Pop();
  // Insert item and sort priority queue
  void Push(const T &item);

 private:
  std::vector<T> items;
  size_t cur_size = 0;
  C cmp;

  // Helper methods for indices
  size_t Root() {
    return 0;
  }
  size_t Parent(size_t n) {
    return (n - 1) / 2;
  }
  size_t LeftChild(size_t n) {
    return 2 * n + 1;
  }
  size_t RightChild(size_t n) {
    return 2 * n + 2;
  }

  // Helper methods for node testing
  bool HasParent(size_t n) {
    return n != Root();
  }
  bool IsNode(size_t n) {
    return n < cur_size;
  }

  // Helper methods for restructuring
  void PercolateUp(size_t n);
  void PercolateDown(size_t n);

  // Node comparison
  bool CompareNodes(size_t i, size_t j);
};

template<typename T, typename C>
size_t PQueue<T, C>::Size() {
  return cur_size;
}

template<typename T, typename C>
T &PQueue<T, C>::Top() {
  return items[Root()];
}

template<typename T, typename C>
void PQueue<T, C>::Pop() {
  if (cur_size == 0)
    return;
  std::swap(items[0], items[cur_size - 1]);
  auto it = items.end();
  items.erase(it - 1);
  cur_size--;
  PercolateDown(Root());
}

template<typename T, typename C>
void PQueue<T, C>::Push(const T &item) {
  items.emplace_back(item);
  cur_size++;
  PercolateUp(cur_size - 1);
}

template<typename T, typename C>
void PQueue<T, C>::PercolateUp(size_t n) {
  if (HasParent(n)) {
    if (CompareNodes(n, Parent(n))) {
      std::swap(items[n], items[Parent(n)]);
      PercolateUp(Parent(n));
    } else {
      // the node is at the right place
      return;
    }
  } else {
    // the only node doesn't need to percolate up
    return;
  }
}

template<typename T, typename C>
void PQueue<T, C>::PercolateDown(size_t n) {
  if (IsNode(RightChild(n)) && IsNode(LeftChild(n))) {
    // both nodes can be compared
    if (CompareNodes(LeftChild(n), n) && CompareNodes(RightChild(n), n)) {
      // both child nodes are less than the parent
      if (CompareNodes(RightChild(n), LeftChild(n))) {
        std::swap(items[n], items[RightChild(n)]);
        PercolateDown(RightChild(n));
      } else {
        std::swap(items[n], items[LeftChild(n)]);
        PercolateDown(LeftChild(n));
      }
    } else if (CompareNodes(LeftChild(n), n)) {
      // exchange parent and left child
      std::swap(items[n], items[LeftChild(n)]);
      PercolateDown(LeftChild(n));
    } else if (CompareNodes(RightChild(n), n)) {
      // exchange parent and right child
      std::swap(items[n], items[RightChild(n)]);
      PercolateDown(RightChild(n));
    } else {
      // at right pos
      return;
    }
  } else if (IsNode(LeftChild(n))) {
    // only left child can be compared
    if (CompareNodes(LeftChild(n), n)) {
      std::swap(items[n], items[LeftChild(n)]);
      PercolateDown(LeftChild(n));
    } else {
      // at right pos
      return;
    }
  } else if (IsNode(RightChild(n))) {
    // only right child can be compared
    if (CompareNodes(RightChild(n), n)) {
      std::swap(items[n], items[RightChild(n)]);
      PercolateDown(RightChild(n));
    } else {
      // at right pos
      return;
    }
  } else {
    // one node
    return;
  }
}

template<typename T, typename C>
bool PQueue<T, C>::CompareNodes(size_t i, size_t j) {
  return cmp(items[i], items[j]);
}

// To be completed below

#endif  // PQUEUE_H_

#include <gtest/gtest.h>
#include <functional>

#include "pqueue.h"
TEST(PQueue, less) {
  PQueue<int> pq;

  pq.Push(42);
  pq.Push(23);
  pq.Push(2);
  pq.Push(34);

  EXPECT_EQ(pq.Top(), 2);
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 23);
}

TEST(PQueue, lessforallnode) {
  PQueue<int> pq;
  // unsorted push
  pq.Push(42);
  pq.Push(23);
  pq.Push(2);
  pq.Push(10);
  pq.Push(34);
  pq.Push(1);
  pq.Push(30);

  EXPECT_EQ(pq.Top(), 1);
  EXPECT_EQ(pq.Size(), 7);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 2);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 10);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 23);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 30);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 34);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 42);
  pq.Pop();
  EXPECT_EQ(pq.Size(), 0);
  // Each pop will give the lowest number--highest priority
}

TEST(PQueue, great) {
  PQueue<int, std::greater<int>> pq;

  pq.Push(42);
  pq.Push(23);
  pq.Push(2);
  pq.Push(34);

  EXPECT_EQ(pq.Top(), 42);
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 34);
}

TEST(PQueue, greatforallnode) {
  PQueue<int, std::greater<int>> pq;
  // unsorted push
  pq.Push(42);
  pq.Push(23);
  pq.Push(2);
  pq.Push(10);
  pq.Push(34);
  pq.Push(1);
  pq.Push(30);

  EXPECT_EQ(pq.Top(), 42);
  EXPECT_EQ(pq.Size(), 7);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 34);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 30);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 23);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 10);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 2);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 1);
  pq.Pop();
  EXPECT_EQ(pq.Size(), 0);
  // Each pop will give the highest number--highest priority
}

class MyClass {
 public:
  explicit MyClass(int n) : n_(n) {}
  bool operator < (const MyClass &mc) const { return n_ < mc.n_; }
  int n() { return n_; }
 private:
  int n_;
};

TEST(PQueue, custom_class) {
  std::vector<MyClass> vec{MyClass(42), MyClass(23), MyClass(2), MyClass(34)};

  PQueue<MyClass> pq;
  // unsorted push
  pq.Push(vec[0]);
  pq.Push(vec[1]);
  pq.Push(vec[2]);
  pq.Push(vec[3]);

  EXPECT_EQ(pq.Top().n(), vec[2].n());
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top().n(), vec[1].n());
}

template <typename T>
class MinPointerComparator {
 public:
  bool operator () (const T &lhs, const T &rhs) const {
    return *lhs < *rhs;
    // use the objects
  }
};

TEST(PQueue, custom_class_pointer) {
  std::vector<MyClass*> vec{new MyClass(42), new MyClass(23),
                            new MyClass(2), new MyClass(34)};

  PQueue<MyClass*, MinPointerComparator<MyClass*>> pq;
  // custom comparator on pointer
  pq.Push(vec[0]);
  pq.Push(vec[1]);
  pq.Push(vec[2]);
  pq.Push(vec[3]);

  EXPECT_EQ(pq.Top(), vec[2]);
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top(), vec[1]);
}


int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

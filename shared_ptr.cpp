#include <iostream>
#include <map>
#include <mutex>
#include <shared_mutex>

// 通过shared_ptr的实现，可以考察候选人对默认构造函数，复制构造函数，移动构造函数和析构函数
// 模板，静态成员数据，线程安全的理解和运用，当然还包括对智能指针本身的运用
// 完整实现shared_ptr需要花时间较多，这里其实也只是一个简略的版本，考虑到面试时间有限，
// 建议让候选人边讲实现思路边写代码，从一开始如何实现引用计数，到各个构造、析构方法
// 并考察候选人能否意识到线程安全的问题
// 如果不能给出基本的实现，至少应达到讲出下面测试用例的预期输出和解释
template <typename T> class shared_ptr {
  static std::map<T *, unsigned int> counts;
  static std::shared_mutex counts_locker;
  T *ptr = nullptr;

public:
  shared_ptr(T *p = nullptr) : ptr(p) {
    if (p != nullptr) {
      std::unique_lock lock(counts_locker);
      counts[ptr] = 1;
    }
  }

  ~shared_ptr() { reset(); }

  shared_ptr<T>(const shared_ptr<T> &p) : ptr(p.ptr) {
    if (ptr != nullptr) {
      std::unique_lock lock(counts_locker);
      ++(counts[ptr]);
    }
  }

  shared_ptr<T>(shared_ptr<T> &&p) : ptr(p.ptr) { p.ptr = nullptr; }

  shared_ptr<T> &operator=(const shared_ptr<T> &p) {
    this->reset();
    this->ptr = p.ptr;
    if (ptr != nullptr) {
      std::unique_lock lock(counts_locker);
      ++(counts[ptr]);
    }
    return *this;
  }

  shared_ptr<T> &operator=(shared_ptr<T> &&p) {
    if (this->ptr == p.ptr) {
      p.reset();
    } else {
      this->reset();
      this->ptr = p.ptr;
      p.ptr = nullptr;
    }

    return *this;
  }

  unsigned count() const {
    if (ptr == nullptr) {
      return 0;
    } else {
      std::shared_lock lock(counts_locker);
      return counts[ptr];
    }
  }

  void reset() {
    if (ptr != nullptr) {
      std::unique_lock lock(counts_locker);
      --counts[ptr];
      if (counts[ptr] == 0) {
        counts.erase(ptr);
        delete ptr;
      }
      ptr = nullptr;
    }
  }
};

template <typename T> std::map<T *, unsigned int> shared_ptr<T>::counts;
template <typename T> std::shared_mutex shared_ptr<T>::counts_locker;

int main() {

  struct test_type {
    test_type(int _id) : id(_id) {}
    ~test_type() {
      std::cout << "test data " << id << " is deleted." << std::endl;
    }

    int id = 0;
  };

  shared_ptr<test_type> p1;
  shared_ptr<test_type> p2 = new test_type(1);

  std::cout << "p1 count: " << p1.count() << std::endl;
  std::cout << "p2 count: " << p2.count() << std::endl;

  p1 = p2;
  std::cout << "After p1 = p2" << std::endl;
  std::cout << "p1 count: " << p1.count() << std::endl;
  std::cout << "p2 count: " << p2.count() << std::endl;

  {
    shared_ptr<test_type> p3(p1);
    std::cout << "After shared_ptr<test_type> p3(p1)" << std::endl;
    std::cout << "p1 count: " << p1.count() << std::endl;
    std::cout << "p2 count: " << p2.count() << std::endl;
    std::cout << "p3 count: " << p3.count() << std::endl;
  }

  std::cout << "After p3 out of scope" << std::endl;
  std::cout << "p1 count: " << p1.count() << std::endl;
  std::cout << "p2 count: " << p2.count() << std::endl;

  shared_ptr<test_type> p4(std::move(p1));
  std::cout << "After p4(std::move(p1))" << std::endl;
  std::cout << "p1 count: " << p1.count() << std::endl;
  std::cout << "p2 count: " << p2.count() << std::endl;
  std::cout << "p4 count: " << p4.count() << std::endl;

  p2.reset();
  std::cout << "After p2.reset()" << std::endl;
  std::cout << "p1 count: " << p1.count() << std::endl;
  std::cout << "p2 count: " << p2.count() << std::endl;
  std::cout << "p4 count: " << p4.count() << std::endl;

  shared_ptr<test_type> p5 = new test_type(5);
  std::cout << "After p5 = new test_type(5)" << std::endl;
  std::cout << "p5 count: " << p5.count() << std::endl;

  std::cout << "After p5 = p4" << std::endl;
  p5 = p4;
  std::cout << "p4 count: " << p4.count() << std::endl;
  std::cout << "p5 count: " << p5.count() << std::endl;

  std::cout << "After p5 = std::move(p4)" << std::endl;
  p5 = std::move(p4);
  std::cout << "p4 count: " << p4.count() << std::endl;
  std::cout << "p5 count: " << p5.count() << std::endl;

  shared_ptr<test_type> p6 = new test_type(6);
  std::cout << "After p6 = new test_type(6)" << std::endl;
  std::cout << "p6 count: " << p6.count() << std::endl;

  std::cout << "After p6 = std::move(p5)" << std::endl;
  p6 = std::move(p5);
  std::cout << "p5 count: " << p5.count() << std::endl;
  std::cout << "p6 count: " << p6.count() << std::endl;

  std::cout << "After main return, all test data be delete:" << std::endl;
  return 0;
}
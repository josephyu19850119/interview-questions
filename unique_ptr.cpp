#include <iostream>

// 通过unique_ptr的实现，可以考察候选人对移动构造函数，移动赋值操作符，析构函数，
// 模板的理解和运用，还包括用delete关键字而不是private来禁用复制构造函数、赋值操作符
// 完整实现unique_ptr需要花时间较多，这里其实也只是一个简略的版本，考虑到面试时间有限，
// 建议让候选人边讲实现思路边写代码
template <typename T> class unique_ptr {
  T *ptr = nullptr;

public:
  unique_ptr(T *p = nullptr) : ptr(p) {}

  unique_ptr(const unique_ptr &p) = delete;
  unique_ptr(unique_ptr &&p) : ptr(p.ptr) { p.ptr = nullptr; }

  ~unique_ptr() { delete ptr; }

  unique_ptr &operator=(const unique_ptr &p) = delete;
  unique_ptr &operator=(unique_ptr &&p) {
    reset();
    this->ptr = p.ptr;
    p.ptr = nullptr;
    return *this;
  }
  void reset() {
    if (this->ptr != nullptr) {
      delete this->ptr;
      this->ptr = nullptr;
    }
  }
  unsigned count() const { return ptr == nullptr ? 0 : 1; }
};

int main() {
  struct test_data {
    test_data(int _id) : id(_id) {}
    ~test_data() {
      std::cout << "test_data " << id << " is deleted" << std::endl;
    }
    int id = 0;
  };

  unique_ptr<test_data> p0;
  unique_ptr<test_data> p1 = new test_data(1);
  unique_ptr<test_data> p2 = new test_data(2);
  std::cout << "p0.count: " << p0.count() << std::endl;
  std::cout << "p1.count: " << p1.count() << std::endl;
  std::cout << "p2.count: " << p2.count() << std::endl << std::endl;

  std::cout << "After p1 = std::move(p2)" << std::endl;
  p1 = std::move(p2);
  std::cout << "p1.count: " << p1.count() << std::endl;
  std::cout << "p2.count: " << p2.count() << std::endl << std::endl;

  p0 = std::move(p1);
  std::cout << "After p0 = std::move(p1)" << std::endl;
  std::cout << "p0.count: " << p0.count() << std::endl;
  std::cout << "p1.count: " << p1.count() << std::endl << std::endl;

  unique_ptr<test_data> p3(std::move(p0));
  std::cout << "After p3(std::move(p0))" << std::endl;
  std::cout << "p0.count: " << p0.count() << std::endl;
  std::cout << "p3.count: " << p3.count() << std::endl << std::endl;

  {
    unique_ptr<test_data> p4 = new test_data(4);
    unique_ptr<test_data> p5 = new test_data(5);
    std::cout << "p4.count: " << p4.count() << std::endl;
    std::cout << "p5.count: " << p5.count() << std::endl;

    std::cout << "After p4.reset()" << std::endl;
    p4.reset();
    std::cout << "p4.count: " << p4.count() << std::endl;

    std::cout << "After p5 out of scope" << std::endl;
  }

  // 👇无法编译
  // p1 = p3;

  std::cout << "After main return, all test data be delete:" << std::endl;
  return 0;
}
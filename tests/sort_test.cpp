#include <algorithm>
#include <memory>
#include <vector>
#include <random>
#include <gtest/gtest.h>
#include "cpputils/sort_util.h"

struct obj0 {
  int a;
  int b;
  int c;

  int sum() const { return a + b; }
  int diff() const { return a - b; }

  obj0(int a, int b, int c) : a(a), b(b), c(c) { }
};

struct obj1 {
  int a;
  int b;
  int c;
  int d;
  int e;

  obj1(int a, int b, int c, int d, int e) : a(a), b(b), c(c), d(d), e(e) { }
};

int rand() {
  static std::random_device                                       dev;
  static std::mt19937                                             rng(dev());
  static std::uniform_int_distribution<std::mt19937::result_type> dist(1, 20);
  return dist(rng);
}

TEST(SortTest, CMP) {
  for (int i = 0; i < 10; i++) {
    std::vector<std::pair<int, int>> v0;
    for (int j = 0; j < 100; j++) {
      v0.emplace_back(rand(), rand());
    }
    std::sort(v0.begin(), v0.end(), CMP(first, <, second, >));
    bool ok0 = std::is_sorted(v0.begin(), v0.end(), [](const auto &a, const auto &b) {
      return a.first < b.first || (a.first == b.first && a.second > b.second);
    });
    EXPECT_TRUE(ok0);

    std::vector<obj0> v1;
    for (int j = 0; j < 100; j++) {
      v1.emplace_back(obj0(rand(), rand(), rand()));
    }
    std::sort(v1.begin(), v1.end(), CMP(a, >, b, >, c, >));
    bool ok1 = std::is_sorted(v1.begin(), v1.end(), [](const auto &x, const auto &y) {
      return x.a > y.a || (x.a == y.a && x.b > y.b) || (x.a == y.b && x.b == y.b && x.c > y.c);
    });
    EXPECT_TRUE(ok1);

    std::sort(v1.begin(), v1.end(), CMP(sum(), >, diff(), >));
    ok1 = std::is_sorted(v1.begin(), v1.end(), [](const auto &x, const auto &y) {
      return x.sum() > y.sum() || (x.sum() == y.sum() && x.diff() > y.diff());
    });
    EXPECT_TRUE(ok1);

    std::vector<obj1> v2;
    for (int j = 0; j < 100; j++) {
      v2.emplace_back(obj1(rand(), rand(), rand(), rand(), rand()));
    }
    std::sort(v2.begin(), v2.end(), CMP(a, >, b, >, c, >, d, >, e, >));
    bool ok2 = std::is_sorted(v2.begin(), v2.end(), [](const auto &x, const auto &y) {
      return x.a > y.a || (x.a == y.a && x.b > y.b) || (x.a == y.b && x.b == y.b && x.c > y.c) ||
             (x.a == y.b && x.b == y.b && x.c == y.c && x.d > y.d) ||
             (x.a == y.b && x.b == y.b && x.c == y.c && x.d == y.d && x.e > y.e);
    });
    EXPECT_TRUE(ok2);
  }
}

TEST(SortTest, CMP_P) {
  for (int i = 0; i < 10; i++) {
    std::vector<std::shared_ptr<std::pair<int, int>>> v0;
    for (int j = 0; j < 100; j++) {
      v0.emplace_back(std::make_shared<std::pair<int, int>>(rand(), rand()));
    }
    std::sort(v0.begin(), v0.end(), CMP_P(first, <, second, >));
    bool ok0 = std::is_sorted(v0.begin(), v0.end(), [](const auto &a, const auto &b) {
      return a->first < b->first || (a->first == b->first && a->second > b->second);
    });
    EXPECT_TRUE(ok0);

    std::vector<std::shared_ptr<obj0>> v1;
    for (int j = 0; j < 100; j++) {
      v1.emplace_back(std::make_shared<obj0>(rand(), rand(), rand()));
    }
    std::sort(v1.begin(), v1.end(), CMP_P(a, >, b, >, c, >));
    bool ok1 = std::is_sorted(v1.begin(), v1.end(), [](const auto &x, const auto &y) {
      return x->a > y->a || (x->a == y->a && x->b > y->b) ||
             (x->a == y->b && x->b == y->b && x->c > y->c);
    });
    EXPECT_TRUE(ok1);

    std::sort(v1.begin(), v1.end(), CMP_P(sum(), >, diff(), >));
    ok1 = std::is_sorted(v1.begin(), v1.end(), [](const auto &x, const auto &y) {
      return x->sum() > y->sum() || (x->sum() == y->sum() && x->diff() > y->diff());
    });
    EXPECT_TRUE(ok1);

    std::vector<std::shared_ptr<obj1>> v2;
    for (int j = 0; j < 100; j++) {
      v2.emplace_back(std::make_shared<obj1>(rand(), rand(), rand(), rand(), rand()));
    }
    std::sort(v2.begin(), v2.end(), CMP_P(a, >, b, >, c, >, d, >, e, >));
    bool ok2 = std::is_sorted(v2.begin(), v2.end(), [](const auto &x, const auto &y) {
      return x->a > y->a || (x->a == y->a && x->b > y->b) ||
             (x->a == y->b && x->b == y->b && x->c > y->c) ||
             (x->a == y->b && x->b == y->b && x->c == y->c && x->d > y->d) ||
             (x->a == y->b && x->b == y->b && x->c == y->c && x->d == y->d && x->e > y->e);
    });
    EXPECT_TRUE(ok2);
  }
}

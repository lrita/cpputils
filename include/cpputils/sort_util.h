#pragma once
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/arithmetic/mod.hpp>
#include <boost/preprocessor/arithmetic/div.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/arithmetic/mul.hpp>
#include <boost/preprocessor/variadic/size.hpp>
#include <boost/preprocessor/variadic/to_tuple.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/size.hpp>
#include <boost/preprocessor/repeat.hpp>

// clang-format off
#define __CMP_EMPTY__(...)
#define __CMP_INPUT_EMPTY__()   static_assert(false, "input param is empty");
#define __CMP_INPUT_INVALID__() static_assert(false, "input param size must be even");
#define __CMP_CHECK__(...)                                                                                                     \
  BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), 0), __CMP_INPUT_EMPTY__, __CMP_EMPTY__)()                    \
  BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_MOD(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), 2), 0), __CMP_EMPTY__, __CMP_INPUT_INVALID__)()
#define __CMP_F_H_(d, f, o) if (__x_ d f o __y_ d f) return true; if (__y_ d f o __x_ d f) return false;
#define __CMP_T_H_(d, f, o) return __x_ d f o __y_ d f;
#define __CMP_REP_OBJ__(Z, N, TUPLE) BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_TUPLE_SIZE(TUPLE), BOOST_PP_MUL(BOOST_PP_INC(N), 2)), __CMP_REP_OBJ_T_, __CMP_REP_OBJ_F_)(N, TUPLE)
#define __CMP_REP_OBJ_T_(N, TUPLE) __CMP_T_H_(., BOOST_PP_TUPLE_ELEM(BOOST_PP_MUL(N, 2), TUPLE), BOOST_PP_TUPLE_ELEM(BOOST_PP_INC(BOOST_PP_MUL(N, 2)), TUPLE))
#define __CMP_REP_OBJ_F_(N, TUPLE) __CMP_F_H_(., BOOST_PP_TUPLE_ELEM(BOOST_PP_MUL(N, 2), TUPLE), BOOST_PP_TUPLE_ELEM(BOOST_PP_INC(BOOST_PP_MUL(N, 2)), TUPLE))
#define __CMP_REP_PTR__(Z, N, TUPLE) BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_TUPLE_SIZE(TUPLE), BOOST_PP_MUL(BOOST_PP_INC(N), 2)), __CMP_REP_PTR_T_, __CMP_REP_PTR_F_)(N, TUPLE)
#define __CMP_REP_PTR_T_(N, TUPLE) __CMP_T_H_(->, BOOST_PP_TUPLE_ELEM(BOOST_PP_MUL(N, 2), TUPLE), BOOST_PP_TUPLE_ELEM(BOOST_PP_INC(BOOST_PP_MUL(N, 2)), TUPLE))
#define __CMP_REP_PTR_F_(N, TUPLE) __CMP_F_H_(->, BOOST_PP_TUPLE_ELEM(BOOST_PP_MUL(N, 2), TUPLE), BOOST_PP_TUPLE_ELEM(BOOST_PP_INC(BOOST_PP_MUL(N, 2)), TUPLE))

// CMP(...) 生成按 class / struct 对象成员比较关系的表达式，CMP(...)参数必须是偶数个，
// 每一对第一个是字段名，第二个是比较逻辑符号，比如'<' / '>'，例如：
// 有 struct obj{ int a; int b; }; `std::sort(vec, CMP(a, <, b, >))`标识先按 a 升序，再按 b 降序
// 有 std::pair<int, int>; `std::sort(vec, CMP(first, <, second, >))`标识先按 pair 的 first 升序，再按 second 降序
#define CMP(...)                                                                                                                     \
  [](const auto &__x_, const auto &__y_) -> bool {                                                                                   \
    __CMP_CHECK__(__VA_ARGS__)                                                                                                       \
    BOOST_PP_REPEAT(BOOST_PP_DIV(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), 2), __CMP_REP_OBJ__, BOOST_PP_VARIADIC_TO_TUPLE(__VA_ARGS__))  \
  }
// CMP_P(...) 与 CMP(...) 逻辑一致，在容器元素为指针时使用
#define CMP_P(...)                                                                                                                   \
  [](const auto& __x_, const auto& __y_) -> bool {                                                                                   \
    __CMP_CHECK__(__VA_ARGS__)                                                                                                       \
    BOOST_PP_REPEAT(BOOST_PP_DIV(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), 2), __CMP_REP_PTR__, BOOST_PP_VARIADIC_TO_TUPLE(__VA_ARGS__))  \
  }
// clang-format on

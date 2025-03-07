# cpputils

一些方便测试使用的c++辅助功能，包含

- 从[unordered\_dense](https://github.com/martinus/unordered_dense/blob/d911053e390816ecc5dedd5a9d6b4bb5ed92b4c9/test/app/counter.h)提取的用来测试对象构造、拷贝、移动的辅助类

# 编译
```sh
export fmt_DIR=.../fmt GTest_DIR=.../googletest
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=.../cpputils
```

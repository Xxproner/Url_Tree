# Introdution
This repo is a C++ implementation of URL tree (similar to [w3 library](https://www.w3.org/Library/src/HTUTree.html))
It is possible to storage endpoint's arbitrary data in property_tree data structure (boost [basic_ptree](https://www.boost.org/doc/libs/1_86_0/doc/html/doxygen/classboost_1_1property__tree_1_1basic__ptree.html)). 
# Interface.
```cpp
template <typename EndpointData_T,
  typename URLChar_T
>
class Router
{
  using key_type = std::basic_string<URLChar_T, std::char_traits<URLChar_T>>;
  Router(const key_type& host, const key_type& scheme, uint16_t port = 80);

  // ...
}
```
Always children are sorted by native __std::basic_string<...>::operator<(...)__. Search, removal, and insertion operations have complexity O(logN * D), 
where _N_ is number children of a node and _D_ is tree depth. 
Router meets the requirements of [Container](https://en.cppreference.com/w/cpp/named_req/Container). 

# Requirements
  - c++17 or earlier (we are on it)
  - boost 1.83.0
# Benchmark
TODO:
# Appendix
If you good project, we would be glad to listen some criticism.
Contact:
  email:  xxproner@mail.ru.

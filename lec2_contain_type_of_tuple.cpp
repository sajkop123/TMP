#include <string>
#include <iostream>
#include <atomic>
#include <vector>
#include <memory>

template<typename T>
void func() {
  puts(__PRETTY_FUNCTION__);
}

template<bool cond, typename THEN, typename ELSE>
struct if_;

template<typename THEN, typename ELSE>
struct if_<true, THEN, ELSE> {
  using type = THEN;
};

template<typename THEN, typename ELSE>
struct if_<false, THEN, ELSE> {
  using type = ELSE;
};

template<typename SEARCH, typename TUPLE, size_t start_from = 0>
struct contain_type :
  if_< // IF
    std::is_same_v<SEARCH, std::tuple_element_t<start_from, TUPLE>>,
    // THEN
    std::true_type,
    //ELSE
    typename if_ < // IF
      (start_from == std::tuple_size_v<TUPLE> - 1),
      // THEN
      std::false_type,
      // ELSE
      contain_type<SEARCH, TUPLE, start_from+1>
    >::type
  >::type {};

// specilization for empty tuple
template<typename SEARCH>
struct contain_type<SEARCH, std::tuple<>, 0> : std::false_type {};

//  normal implementation
bool contain(const std::string& search, const std::vector<std::string>& v, size_t start_from = 0) {
  if (v[start_from] == search) {
    return true;
  } else {
    if (start_from == v.size() - 1) {
      return false;
    } else {
      return contain(search, v, start_from+1);
    }
  }
}

int main() {
  std::tuple<int, bool, float> tuple;
  std::cout << std::boolalpha << contain_type<bool, decltype(tuple)>::value << "\n";
  std::cout << std::boolalpha << contain_type<double, decltype(tuple)>::value << "\n";
  std::cout << std::boolalpha << contain_type<int, std::tuple<>>::value << "\n";
}

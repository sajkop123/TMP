#include <string>
#include <iostream>
#include <atomic>
#include <vector>
#include <memory>

template<typename T>
void func() {
  puts(__PRETTY_FUNCTION__);
}

// variadic template

// handle corner case for only one type
template<typename T_Last>
void printn(T_Last t_last) {
  std::cout << t_last << "\n";
}

// print n variables with corresponding type
template<typename T0, typename... T1toTn>  // `...` in front of name : variable number of elements (here typenames)
void printn(T0 t0, T1toTn... args) {  // `...` between names : define second name as list of params given by first
  std::cout << t0 << ", ";
  printn(args...);  // `...` after name : expand name to a list of all elements it represents
}

// ********************** trick to print tuple **********************
// the second input does not have name becuase we don't use it,
// compiler knows which overload to pick. 
// this principle of taking a parameter just for the type and
// not for its data is called "tag dispatch".
template<typename TUPLE, size_t ... indices>
void print_tuple_impl(TUPLE t, std::index_sequence<indices...>) {
  printn(std::get<indices>(t)...);  // printn(std::get<0>(t), std::get<1>(t), ...);
}

template<typename TUPLE>
void print_tuple(TUPLE t) {
  // make_index_sequence<0, 1, 2, 3, ..., tuple_size>
  print_tuple_impl(t, std::make_index_sequence<std::tuple_size<TUPLE>::value>());
}

// ******************************************************************

// ********************** ADVANCE : perfect forwarding **********************
// handle corner case for only one type
template<typename T_Last>
void printn_nocopy(T_Last&& t_last) {
  std::cout << std::forward<T_Last>(t_last) << "\n";
}

// print n variables with corresponding type
template<typename T0, typename... T1toTn> 
void printn_nocopy(T0&& t0, T1toTn&&... args) {
  std::cout << std::forward<T0>(t0) << ", ";
  printn_nocopy(std::forward<T1toTn>(args)...);  // std::forward<T1>(args1), std::forward<T1>(args1), ...
}

template<typename TUPLE, size_t ... indices>
void print_tuple_impl_nocopy(TUPLE &&t, std::index_sequence<indices...>) {
  printn(std::get<indices>(std::forward<TUPLE>(t))...);  // printn(std::get<0>(t), std::get<1>(t), ...);
}

template<typename TUPLE>
void print_tuple_nocopy(TUPLE&& t) {
  print_tuple_impl_nocopy(
      std::forward<TUPLE>(t),
      std::make_index_sequence<std::tuple_size<std::remove_reference_t<TUPLE>>::value>());
}
// ******************************************************************


int main() {
  std::cout << std::boolalpha;
  printn(100, "hellow", 8.5, true);

  auto tuple = std::make_tuple(100, "hellow", 8.5, true);
  print_tuple(tuple);
  print_tuple_nocopy(tuple);
}

#include <string>
#include <iostream>
#include <atomic>
#include <vector>
#include <memory>
#include <list>

template<typename T>
void func() {
  puts(__PRETTY_FUNCTION__);
}

// variadic template + list-liked self-defined container

// general meta function
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

// meta function for our list-liked container
template<typename...>
struct type_list{};

template<typename LIST>
struct empty : std::false_type {};

template<>
struct empty<type_list<>> : std::true_type {};

static_assert(empty<type_list<>>::value);  // static assert to test if empty works as expected.
static_assert(empty<type_list<bool>>::value == false);  

template<typename LIST>
struct front {
};

template<typename T0, typename... T1toN>
struct front<type_list<T0, T1toN...>> {
  using type = T0;
};

static_assert(std::is_same_v<front<type_list<bool, int>>::type, bool>);  // check front works as expected.

template<typename LIST>
struct pop_front {};

template<typename T0, typename ...T1toN>
struct pop_front<type_list<T0, T1toN...>> {
  using type = type_list<T1toN...>;
};

static_assert(std::is_same_v<pop_front<type_list<bool, int, double>>::type,
              type_list<int, double>>);


template<typename LIST>
using front_t = typename front<LIST>::type;
template<typename LIST>
using pop_front_t = typename pop_front<LIST>::type;
template<typename LIST>
static constexpr bool empty_v = empty<LIST>::value;

// add forward declaration
template<typename SEARCH, typename LIST>
struct contains_type;

template<typename SEARCH, typename LIST>
struct non_empty_contains_type :
  if_<
    // IF
    std::is_same_v<SEARCH, front_t<LIST>>,
    // THEN
    std::true_type,
    // ELSE
    contains_type<SEARCH, pop_front_t<LIST>>
  >::type {};

template<typename SEARCH, typename LIST>
struct contains_type :
  if_< 
    // IF
    empty_v<LIST>,
    // THEN
    std::false_type,
    // ELSE
    non_empty_contains_type<SEARCH, LIST>
  >::type {};


// template<typename SEARCH>
// struct contains_type<SEARCH, type_list<>> : std::false_type {};

// runtime function, same as lec2 in functionality, but more readable
bool contains(const std::string& search, std::list<std::string>& l) {
  if (l.empty()) {
    return false;
  } else {
    if (search == l.front()) {
      return true;
    } else {
      l.pop_front();
      return contains(search, l);
    }
  }
}

int main() {
  std::list<std::string> list{"int", "double", "bool"};
  std::cout << std::boolalpha;
  std::cout << contains("bool", list) << "\n";
  std::cout << contains("float", list) << "\n";

  type_list<int, double, bool> types;
  std::cout << contains_type<bool, decltype(types)>::value << "\n";
  std::cout << contains_type<float, decltype(types)>::value << "\n";
  std::cout << contains_type<bool, type_list<>>::value << "\n";
}

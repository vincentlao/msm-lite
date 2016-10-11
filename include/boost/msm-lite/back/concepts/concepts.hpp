#ifndef CONCEPTS_ID6ZF1PC
#define CONCEPTS_ID6ZF1PC

#include "boost/msm-lite/aux_/type_traits.hpp"

namespace detail {
struct on_entry;
struct on_exit;
struct terminate_state;
}

namespace concepts {
template <class T>
decltype(aux::declval<T>()()) configurable_impl(int);
template <class>
void configurable_impl(...);
template <class T>
struct configurable : aux::is_pool<decltype(configurable_impl<T>(0))> {};
struct callable_fallback {
  void operator()();
};
template <class T>
aux::false_type test_callable(aux::non_type<void (callable_fallback::*)(), &T::operator()> *);
template <class>
aux::true_type test_callable(...);
template <class, class>
struct callable_impl : aux::false_type {};
template <class R>
struct callable_impl<R, aux::true_type> : aux::true_type {};
template <class R, class T>
using callable = callable_impl<R, decltype(test_callable<aux::inherit<T, callable_fallback>>(0))>;
template <class...>
struct is_valid_transition : aux::true_type {};
template <class S1, class S2, class... Ts>
struct is_valid_transition<S1, S2, detail::on_entry, Ts...> : aux::is_same<S1, S2> {};
template <class S1, class S2, class... Ts>
struct is_valid_transition<S1, S2, detail::on_exit, Ts...> : aux::is_same<S1, S2> {};
template <class... Ts>
struct is_valid_transition<detail::terminate_state, Ts...> {};
aux::false_type transitional_impl(...);
template <class T>
auto transitional_impl(T &&t) -> is_valid_transition<typename T::src_state, typename T::dst_state, typename T::event,
                                                     typename T::deps, decltype(T::initial), decltype(T::history)>;
template <class T>
struct transitional : decltype(transitional_impl(aux::declval<T>())) {};
template <class>
aux::false_type dispatchable_impl(...);
template <class...>
struct is_valid_event : aux::true_type {};
template <class, class TEvent>
auto dispatchable_impl(TEvent &&) -> is_valid_event<decltype(TEvent::id), decltype(TEvent())>;
template <class T, class TEvent>
auto dispatchable_impl(TEvent &&) -> is_valid_event<decltype(TEvent::id), decltype(TEvent(aux::declval<T>()))>;
template <class, class>
struct dispatchable;
template <class T, class... TEvents>
struct dispatchable<T, aux::type_list<TEvents...>>
    : aux::is_same<aux::bool_list<aux::always<TEvents>::value...>,
                   aux::bool_list<decltype(dispatchable_impl<T>(aux::declval<TEvents>()))::value...>> {};

template <class T, class = decltype(T::c_str())>
aux::true_type test_stringable(const T &);
aux::false_type test_stringable(...);
template <class T, class = void>
struct stringable : aux::false_type {};
template <class T>
struct stringable<T, decltype(void(sizeof(T)))> : decltype(test_stringable(aux::declval<T>())) {};

}  // concepts

#endif

//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once
#if (__cplusplus < 201305L && _MSC_VER < 1900)
#error "Boost.MSM-lite requires C++14 support (Clang-3.4+, GCC-5.1+, MSVC-2015+)"
#else
#define BOOST_MSM_LITE_VERSION 1'0'1
#if !defined(BOOST_MSM_LITE_LOG)
#define BOOST_MSM_LITE_LOG(...)
#else
#define BOOST_MSM_LITE_LOG_ENABLED
#endif
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
#define BOOST_MSM_LITE_NOEXCEPT noexcept
#define BOOST_MSM_LITE_NOEXCEPT_IF(...) noexcept(__VA_ARGS__)
#else
#define BOOST_MSM_LITE_NOEXCEPT
#define BOOST_MSM_LITE_NOEXCEPT_IF(...)
#endif
#if !defined(__has_builtin)
#define __has_builtin(...) 0
#endif
#if !defined(__has_extension)
#define __has_extension(...) 0
#endif
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-string-literal-operator-template"
#pragma clang diagnostic ignored "-Wzero-length-array"
#endif
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
namespace boost {
namespace msm {
namespace lite {
inline namespace v_1_0_1 {
namespace aux {
using byte = unsigned char;
struct none_type {};
template <char... Chrs>
struct string {
  static auto c_str() BOOST_MSM_LITE_NOEXCEPT {
    static constexpr char str[] = {Chrs..., 0};
    return str;
  }
};
template <class...>
struct type {};
template <class T, T>
struct non_type {};
template <class, class>
struct pair {};
template <class...>
struct type_list {
  using type = type_list;
};
template <bool...>
struct bool_list {
  using type = bool_list;
};
template <class... Ts>
struct inherit : Ts... {
  using type = inherit;
};
template <class T>
T &&declval() BOOST_MSM_LITE_NOEXCEPT;
template <class T, T V>
struct integral_constant {
  using type = integral_constant;
  static constexpr T value = V;
};
using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;
template <class...>
using void_t = void;
template <class>
struct always : aux::true_type {};
template <bool B, class T, class F>
struct conditional {
  using type = T;
};
template <class T, class F>
struct conditional<false, T, F> {
  using type = F;
};
template <bool B, class T, class F>
using conditional_t = typename conditional<B, T, F>::type;
template <bool B, class T = void>
struct enable_if {};
template <class T>
struct enable_if<true, T> {
  using type = T;
};
template <bool B, class T = void>
using enable_if_t = typename enable_if<B, T>::type;
template <class, class>
struct is_same : false_type {};
template <class T>
struct is_same<T, T> : true_type {};
template <class T, class U>
using is_base_of = integral_constant<bool, __is_base_of(T, U)>;
#if __has_extension(is_constructible)
template <class T, class... TArgs>
using is_constructible = integral_constant<bool, __is_constructible(T, TArgs...)>;
#else
template <class T, class... TArgs>
decltype(void(T(declval<TArgs>()...)), true_type{}) test_is_constructible(int);
template <class, class...>
false_type test_is_constructible(...);
template <class T, class... TArgs>
using is_constructible = decltype(test_is_constructible<T, TArgs...>(0));
#endif
template <class T>
struct remove_reference {
  using type = T;
};
template <class T>
struct remove_reference<T &> {
  using type = T;
};
template <class T>
struct remove_reference<const T &> {
  using type = T;
};
template <class T>
struct remove_reference<T &&> {
  using type = T;
};
template <class T>
using remove_reference_t = typename remove_reference<T>::type;
template <class>
struct function_traits;
template <class R, class... TArgs>
struct function_traits<R (*)(TArgs...)> {
  using args = type_list<TArgs...>;
};
template <class R, class... TArgs>
struct function_traits<R(TArgs...)> {
  using args = type_list<TArgs...>;
};
template <class R, class T, class... TArgs>
struct function_traits<R (T::*)(TArgs...)> {
  using args = type_list<TArgs...>;
};
template <class R, class T, class... TArgs>
struct function_traits<R (T::*)(TArgs...) const> {
  using args = type_list<TArgs...>;
};
template <class T>
using function_traits_t = typename function_traits<T>::args;
template <int...>
struct index_sequence {
  using type = index_sequence;
};
#if __has_builtin(__make_integer_seq)
template <class T, T...>
struct integer_sequence;
template <int... Ns>
struct integer_sequence<int, Ns...> {
  using type = index_sequence<Ns...>;
};
template <int N>
struct make_index_sequence_impl {
  using type = typename __make_integer_seq<integer_sequence, int, N>::type;
};
#else
template <class, class>
struct concat;
template <int... I1, int... I2>
struct concat<index_sequence<I1...>, index_sequence<I2...>> : index_sequence<I1..., (sizeof...(I1) + I2)...> {};
template <int N>
struct make_index_sequence_impl
    : concat<typename make_index_sequence_impl<N / 2>::type, typename make_index_sequence_impl<N - N / 2>::type>::type {};
template <>
struct make_index_sequence_impl<0> : index_sequence<> {};
template <>
struct make_index_sequence_impl<1> : index_sequence<0> {};
#endif
template <int N>
using make_index_sequence = typename make_index_sequence_impl<N>::type;
template <class... Ts>
struct join {
  using type = type_list<>;
};
template <class T>
struct join<T> {
  using type = T;
};
template <class... T1s, class... T2s, class... Ts>
struct join<type_list<T1s...>, type_list<T2s...>, Ts...> : join<type_list<T1s..., T2s...>, Ts...> {};
template <class... Ts, class... T1s, class... T2s, class... T3s, class... T4s, class... T5s, class... T6s, class... T7s,
          class... T8s, class... T9s, class... T10s, class... T11s, class... T12s, class... T13s, class... T14s, class... T15s,
          class... T16s, class... Us>
struct join<type_list<Ts...>, type_list<T1s...>, type_list<T2s...>, type_list<T3s...>, type_list<T4s...>, type_list<T5s...>,
            type_list<T6s...>, type_list<T7s...>, type_list<T8s...>, type_list<T9s...>, type_list<T10s...>, type_list<T11s...>,
            type_list<T12s...>, type_list<T13s...>, type_list<T14s...>, type_list<T15s...>, type_list<T16s...>, Us...>
    : join<type_list<Ts..., T1s..., T2s..., T3s..., T4s..., T5s..., T6s..., T7s..., T8s..., T9s..., T10s..., T11s..., T12s...,
                     T13s..., T14s..., T15s..., T16s...>,
           Us...> {};
template <class... TArgs>
using join_t = typename join<TArgs...>::type;
template <class, class...>
struct unique_impl;
template <class T1, class T2, class... Rs, class... Ts>
struct unique_impl<type<T1, Rs...>, T2, Ts...>
    : conditional_t<is_base_of<type<T2>, T1>::value, unique_impl<type<inherit<T1>, Rs...>, Ts...>,
                    unique_impl<type<inherit<T1, type<T2>>, Rs..., T2>, Ts...>> {};
template <class T1, class... Rs>
struct unique_impl<type<T1, Rs...>> : type_list<Rs...> {};
template <class... Ts>
struct unique : unique_impl<type<none_type>, Ts...> {};
template <class T>
struct unique<T> : type_list<T> {};
template <class... Ts>
using unique_t = typename unique<Ts...>::type;
template <template <class...> class, class>
struct apply;
template <template <class...> class T, template <class...> class U, class... Ts>
struct apply<T, U<Ts...>> {
  using type = T<Ts...>;
};
template <template <class...> class T, class D>
using apply_t = typename apply<T, D>::type;
template <int, class T>
struct tuple_type {
  T value;
};
template <class, class...>
struct tuple_impl;
template <int... Ns, class... Ts>
struct tuple_impl<index_sequence<Ns...>, Ts...> : tuple_type<Ns, Ts>... {
  using boost_di_inject__ = aux::type_list<Ts...>;
  explicit tuple_impl(Ts... ts) BOOST_MSM_LITE_NOEXCEPT : tuple_type<Ns, Ts>{ts}... {}
};
template <>
struct tuple_impl<index_sequence<0>> {
  aux::byte _[0];
};
template <class... Ts>
using tuple = tuple_impl<make_index_sequence<sizeof...(Ts)>, Ts...>;
template <int N, class T>
auto &get_by_id_impl(tuple_type<N, T> *object) BOOST_MSM_LITE_NOEXCEPT {
  return static_cast<tuple_type<N, T> &>(*object).value;
}
template <int N, class Tuple>
auto &get_by_id(Tuple &t) BOOST_MSM_LITE_NOEXCEPT {
  return get_by_id_impl<N>(&t);
}
template <class T>
struct pool_type {
  T value;
};
struct init {};
template <class T>
auto try_get(...) BOOST_MSM_LITE_NOEXCEPT {
  return aux::remove_reference_t<T>{};
}
template <class T>
auto &try_get(pool_type<T> *object) BOOST_MSM_LITE_NOEXCEPT {
  return static_cast<pool_type<T> &>(*object).value;
}
template <class T>
auto &try_get(pool_type<T &> *object) BOOST_MSM_LITE_NOEXCEPT {
  return static_cast<pool_type<T &> &>(*object).value;
}
template <class T, class TPool>
decltype(auto) get(TPool &p) BOOST_MSM_LITE_NOEXCEPT {
  return static_cast<pool_type<T> &>(p).value;
}
template <class... Ts>
struct pool : pool_type<Ts>... {
  using boost_di_inject__ = aux::type_list<Ts...>;
  explicit pool(Ts... ts) BOOST_MSM_LITE_NOEXCEPT : pool_type<Ts>{ts}... {}
  template <class... TArgs>
  pool(init &&, pool<TArgs...> &&p) BOOST_MSM_LITE_NOEXCEPT : pool_type<Ts>{aux::try_get<Ts>(&p)}... {}
  pool(){}
};
template <>
struct pool<> {
  explicit pool(...) BOOST_MSM_LITE_NOEXCEPT {}
  aux::byte _[0];
};
template <class>
struct is_pool : aux::false_type {};
template <class... Ts>
struct is_pool<pool<Ts...>> : aux::true_type {};
template <int, class T>
struct type_id_type {};
template <class, class...>
struct type_id_impl;
template <int... Ns, class... Ts>
struct type_id_impl<index_sequence<Ns...>, Ts...> : type_id_type<Ns, Ts>... {};
template <class... Ts>
struct type_id : type_id_impl<make_index_sequence<sizeof...(Ts)>, Ts...> {};
template <class T, int, int N>
constexpr auto get_id_impl(type_id_type<N, T> *) BOOST_MSM_LITE_NOEXCEPT {
  return N;
}
template <class T, int D>
constexpr auto get_id_impl(...) BOOST_MSM_LITE_NOEXCEPT {
  return D;
}
template <class TIds, int D, class T>
constexpr auto get_id() BOOST_MSM_LITE_NOEXCEPT {
  return get_id_impl<T, D>((TIds *)0);
}
template <class>
struct size;
template <template <class...> class T, class... Ts>
struct size<T<Ts...>> {
  static constexpr auto value = sizeof...(Ts);
};
template <int... Ts>
constexpr auto max() noexcept {
  auto max = 0;
  int _[]{0, (Ts > max ? max = Ts : max)...};
  (void)_;
  return max;
}
template <class... Ts>
class variant {
  using ids_t = type_id<Ts...>;
  alignas(max<alignof(Ts)...>()) byte data[max<sizeof(Ts)...>()];

 public:
  template <class T>
  variant(T object) noexcept {  // non explicit
    id = get_id<ids_t, -1, T>();
    new (&data) T(static_cast<T &&>(object));
  }

  template <class R, class T>
  auto apply(const T &expr) const {
    return apply_impl<R>(expr, type_list<Ts...>{});
  }

  template <class R, class T, class... Tx>
  auto apply_impl(T expr, type_list<Tx...>) const {
    static R (*dispatch[])(T, const decltype(data) &) = {&variant::call<R, T, Tx>...};
    return dispatch[id](expr, data);
  }

  template <class R, class X, class T>
  static R inline call(X x, const decltype(data) &d) {
    return x(*((T *)d));
  }

 private:
  int id = -1;
};
template <class TExpr, class = void>
struct zero_wrapper : TExpr {
  explicit zero_wrapper(const TExpr &expr) : TExpr(expr) {}
};

template <class, class>
struct zero_wrapper_impl;

template <class TExpr, class... TArgs>
struct zero_wrapper_impl<TExpr, aux::type_list<TArgs...>> {
  auto operator()(TArgs... args) const { return reinterpret_cast<const TExpr &>(*this)(args...); }
  aux::byte _[0];
};

template <class TExpr>
struct zero_wrapper<TExpr, void_t<decltype(+declval<TExpr>())>>
    : zero_wrapper_impl<TExpr, aux::function_traits_t<decltype(&TExpr::operator())>> {
  explicit zero_wrapper(...) {}
};
}  // aux
namespace detail {
struct on_entry;
struct on_exit;
struct terminate_state;
}  // detail
namespace concepts {
#define BOOST_MSM_LITE_REQUIRES(...) typename aux::enable_if<__VA_ARGS__, int>::type = 0
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
namespace detail {
struct no_policy {
  using type = no_policy;
  template <class>
  using rebind = no_policy;
  aux::byte _[0];
};
template <class...>
struct transition;
template <class, class>
struct transition_sg;
template <class, class>
struct transition_sa;
template <class, class>
struct transition_eg;
template <class, class>
struct transition_ea;
template <class>
class sm;
template <class>
class sm_impl;
struct defer {
  template <class SM, class T>
  void operator()(sm<SM> &, const T &) BOOST_MSM_LITE_NOEXCEPT {
    // static_assert(aux::is_same<typename sm<SM>::template defer_queue_t<T>, no_policy>::value, "");
  }
};
struct process_event {
  template <class TEvent>
  struct process_event_impl {
    template <class SM, class T>
    void operator()(sm<SM> &sm, const T &) BOOST_MSM_LITE_NOEXCEPT {
      sm.process_event(event);
    }

    TEvent event;
  };

  template <class TEvent>
  auto operator()(const TEvent &event) BOOST_MSM_LITE_NOEXCEPT {
    return process_event_impl<TEvent>{event};
  }
};
enum class status { HANDLED, NOT_HANDLED, DEFFERED };
template <class T>
inline status ret_status() {
  return status::HANDLED;
}
template <>
inline status ret_status<aux::zero_wrapper<defer>>() {
  return status::DEFFERED;
}
struct _ {};
struct thread_safety_policy {};
struct exception_safe_policy {};
struct defer_queue_policy {};
struct operator_base {};
struct internal_event {
  static auto c_str() BOOST_MSM_LITE_NOEXCEPT { return "internal_event"; }
};
struct anonymous : internal_event {
  static auto c_str() BOOST_MSM_LITE_NOEXCEPT { return "anonymous"; }
};
struct on_entry : internal_event {
  static auto c_str() BOOST_MSM_LITE_NOEXCEPT { return "on_entry"; }
};
struct on_exit : internal_event {
  static auto c_str() BOOST_MSM_LITE_NOEXCEPT { return "on_exit"; }
};
struct always {
  status operator()() const BOOST_MSM_LITE_NOEXCEPT { return status::HANDLED; }
  aux::byte _[0];
};
struct none {
  void operator()() BOOST_MSM_LITE_NOEXCEPT {}
  aux::byte _[0];
};
struct fsm {
  using sm = fsm;
  using thread_safety_policy = no_policy;
  using defer_queue_policy = no_policy;
  auto operator()() BOOST_MSM_LITE_NOEXCEPT { return aux::pool<>{}; }
};
template <class>
struct event {
  template <class T, BOOST_MSM_LITE_REQUIRES(concepts::callable<bool, T>::value)>
  auto operator[](const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition_eg<event, aux::zero_wrapper<T>>{*this, aux::zero_wrapper<T>{t}};
  }

  template <class T, BOOST_MSM_LITE_REQUIRES(concepts::callable<void, T>::value)>
  auto operator/(const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition_ea<event, aux::zero_wrapper<T>>{*this, aux::zero_wrapper<T>{t}};
  }
};
template <class T>
struct exception {
  using type = T;
  T exception;
};
template <class TEvent>
struct unexpected_event {
  using type = TEvent;
  TEvent event;
};
struct initial_state {};
struct terminate_state {};
struct history_state {};
template <class>
struct state;
template <class S>
class stringable {};
template <class TState>
struct stringable<state<TState>> {
  static constexpr bool value = concepts::stringable<TState>::value;
};
template <class S, bool is_stringable = stringable<S>::value>
struct state_str {
  static auto c_str() BOOST_MSM_LITE_NOEXCEPT { return S::type::c_str(); }
};
template <class S>
struct state_str<S, false> {
  static auto c_str() BOOST_MSM_LITE_NOEXCEPT { return __PRETTY_FUNCTION__; }
};
template <>
struct state_str<state<terminate_state>> {
  static auto c_str() BOOST_MSM_LITE_NOEXCEPT { return "terminate"; }
};
template <char... Chrs>
struct state_str<state<aux::string<Chrs...>>, false> : aux::string<Chrs...> {};
template <char... Chrs, class T>
struct state_str<state<aux::string<Chrs...>(T)>, false> : state_str<state<aux::string<Chrs...>>> {};
template <class TState>
struct state_impl : state_str<TState> {
  using explicit_states = aux::type_list<>;
  template <class T>
  auto operator<=(const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<TState, T>{static_cast<const TState &>(*this), t};
  }
  template <class T>
  auto operator+(const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<TState, T>{static_cast<const TState &>(*this), t};
  }
  template <class T, BOOST_MSM_LITE_REQUIRES(concepts::callable<bool, T>::value)>
  auto operator[](const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition_sg<TState, aux::zero_wrapper<T>>{static_cast<const TState &>(*this), aux::zero_wrapper<T>{t}};
  }
  template <class T, BOOST_MSM_LITE_REQUIRES(concepts::callable<void, T>::value)>
  auto operator/(const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition_sa<TState, aux::zero_wrapper<T>>{static_cast<const TState &>(*this), aux::zero_wrapper<T>{t}};
  }
};
template <class TState>
struct state : state_impl<state<TState>> {
  using type = TState;
  static constexpr auto initial = false;
  static constexpr auto history = false;
  auto operator*() const BOOST_MSM_LITE_NOEXCEPT { return state<TState(initial_state)>{}; }
  auto operator()(const initial_state &) const BOOST_MSM_LITE_NOEXCEPT { return state<TState(initial_state)>{}; }
  auto operator()(const history_state &) const BOOST_MSM_LITE_NOEXCEPT { return state<TState(history_state)>{}; }
  template <class... Ts>
  auto operator()(const state<Ts> &...) const BOOST_MSM_LITE_NOEXCEPT {
    return state<TState(Ts...)>{};
  }
  template <class T>
  auto operator=(const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<T, state>{t, *this};
  }
};
template <class TState>
struct state<TState(initial_state)> : state_impl<state<TState(initial_state)>> {
  using type = TState;
  static constexpr auto initial = true;
  static constexpr auto history = false;
  template <class T>
  auto operator=(const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<T, state>{t, *this};
  }
};
template <class TState>
struct state<TState(history_state)> : state_impl<state<TState(history_state)>> {
  using type = TState;
  static constexpr auto initial = true;
  static constexpr auto history = true;
  template <class T>
  auto operator=(const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<T, state>{t, *this};
  }
};
template <class TState, class... TExplicitStates>
struct state<TState(TExplicitStates...)> : state_impl<state<TState(TExplicitStates...)>> {
  using type = TState;
  using explicit_states = aux::type_list<TExplicitStates...>;
  static constexpr auto initial = false;
  static constexpr auto history = false;
};
template <class, class>
aux::type_list<> args_impl__(...);
template <class T, class>
auto args_impl__(int) -> aux::function_traits_t<T>;
template <class T, class E>
auto args_impl__(int) -> aux::function_traits_t<decltype(&T::template operator() < E >)>;
template <class T, class>
auto args_impl__(int) -> aux::function_traits_t<decltype(&T::operator())>;
template <class T, class E>
auto args__(...) -> decltype(args_impl__<T, E>(0));
template <class T, class E>
auto args__(int) -> aux::function_traits_t<decltype(&T::template operator() < fsm, E >)>;
template <class T, class E>
using args_t = decltype(args__<T, E>(0));
template <class, class>
struct ignore;
template <class E, class... Ts>
struct ignore<E, aux::type_list<Ts...>> {
  using type = aux::join_t<aux::conditional_t<aux::is_same<E, aux::remove_reference_t<Ts>>::value ||
                                                  aux::is_same<sm<fsm>, aux::remove_reference_t<Ts>>::value,
                                              aux::type_list<>, aux::type_list<Ts>>...>;
};
template <class T, class E, class = void>
struct get_deps {
  using type = typename ignore<E, args_t<T, E>>::type;
};
template <class T, class E>
using get_deps_t = typename get_deps<T, E>::type;
template <template <class...> class T, class... Ts, class E>
struct get_deps<T<Ts...>, E, aux::enable_if_t<aux::is_base_of<operator_base, T<Ts...>>::value>> {
  using type = aux::join_t<get_deps_t<Ts, E>...>;
};
template <class T, class TEvent, class TDeps, class SM,
          aux::enable_if_t<!aux::is_same<TEvent, aux::remove_reference_t<T>>::value &&
                               !aux::is_same<sm<fsm>, aux::remove_reference_t<T>>::value,
                           int> = 0>
decltype(auto) get_arg(const TEvent &, TDeps &deps, SM &) BOOST_MSM_LITE_NOEXCEPT {
  return aux::get<T>(deps);
}
template <class T, class TEvent, class TDeps, class SM,
          aux::enable_if_t<aux::is_same<TEvent, aux::remove_reference_t<T>>::value, int> = 0>
decltype(auto) get_arg(const TEvent &event, TDeps &, SM &) BOOST_MSM_LITE_NOEXCEPT {
  return event;
}
template <class T, class TEvent, class TDeps, class SM,
          aux::enable_if_t<aux::is_same<sm<fsm>, aux::remove_reference_t<T>>::value, int> = 0>
decltype(auto) get_arg(const TEvent &, TDeps &, SM &sm) BOOST_MSM_LITE_NOEXCEPT {
  return sm;
}
#if defined(BOOST_MSM_LITE_LOG_ENABLED)
template <class... Ts, class T, class TEvent, class TDeps, class SM>
auto call_impl(const aux::type<void> &, const aux::type_list<Ts...> &, T object, const TEvent &event, TDeps &deps, sm_impl<SM> &self)
    /*BOOST_MSM_LITE_NOEXCEPT_IF(sm<SM>::is_noexcept)*/ {
  object(get_arg<Ts>(event, deps, self)...);
  BOOST_MSM_LITE_LOG(action, SM, object, event);
}
template <class... Ts, class T, class TEvent, class TDeps, class SM>
auto call_impl(const aux::type<bool> &, const aux::type_list<Ts...> &, T object, const TEvent &event, TDeps &deps, sm_impl<SM> &self)
    /*BOOST_MSM_LITE_NOEXCEPT_IF(sm<SM>::is_noexcept)*/ {
  auto result = object(get_arg<Ts>(event, deps, self)...);
  BOOST_MSM_LITE_LOG(guard, SM, object, event, result);
  return result;
}
#endif
template <class... Ts, class T, class TEvent, class TDeps, class SM,
          aux::enable_if_t<!aux::is_base_of<operator_base, T>::value, int> = 0>
auto call_impl(const aux::type_list<Ts...> &args, T object, const TEvent &event, TDeps &deps, sm_impl<SM> &self)
    /*BOOST_MSM_LITE_NOEXCEPT_IF(sm<SM>::is_noexcept)*/ {
#if defined(BOOST_MSM_LITE_LOG_ENABLED)
  using result_type = decltype(object(get_arg<Ts>(event, deps, self)...));
  return call_impl(aux::type<result_type>{}, args, object, event, deps, self);
#else
  (void)args;
  return object(get_arg<Ts>(event, deps, self)...);
#endif
}
template <class... Ts, class T, class TEvent, class TDeps, class SM,
          aux::enable_if_t<aux::is_base_of<operator_base, T>::value, int> = 0>
auto call_impl(const aux::type_list<Ts...> &, T object, const TEvent &event, TDeps &deps, sm_impl<SM> &self)
    /*BOOST_MSM_LITE_NOEXCEPT_IF(sm<SM>::is_noexcept)*/ {
  return object(event, deps, self);
}
template <class T, class TEvent, class TDeps, class SM>
auto call(T object, const TEvent &event, TDeps &deps, sm_impl<SM> &self) /*BOOST_MSM_LITE_NOEXCEPT_IF(sm<SM>::is_noexcept)*/ {
  return call_impl(args_t<T, TEvent>{}, object, event, deps, self);
}
template <class... Ts>
class seq_ : operator_base {
 public:
  explicit seq_(Ts... ts) BOOST_MSM_LITE_NOEXCEPT : a(ts...) {}

  template <class TEvent, class TDeps, class SM>
  void operator()(const TEvent &event, TDeps &deps, SM &self) /*BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept)*/ {
    for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, deps, self);
  }

 private:
  template <int... Ns, class TEvent, class TDeps, class SM>
  void for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TDeps &deps, SM &self)
      /*BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept)*/ {
    int _[]{0, (call(aux::get_by_id<Ns>(a), event, deps, self), 0)...};
    (void)_;
  }

  aux::tuple<Ts...> a;
};
template <class... Ts>
class and_ : operator_base {
 public:
  explicit and_(Ts... ts) BOOST_MSM_LITE_NOEXCEPT : g(ts...) {}

  template <class TEvent, class TDeps, class SM>
  auto operator()(const TEvent &event, TDeps &deps, SM &self) /*BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept)*/ {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, deps, self);
  }

 private:
  template <int... Ns, class TEvent, class TDeps, class SM>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TDeps &deps, SM &self)
      /*BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept)*/ {
    auto result = true;
    int _[]{0, (call(aux::get_by_id<Ns>(g), event, deps, self) ? result : result = false)...};
    (void)_;
    return result;
  }

  aux::tuple<Ts...> g;
};
template <class... Ts>
class or_ : operator_base {
 public:
  explicit or_(Ts... ts) BOOST_MSM_LITE_NOEXCEPT : g(ts...) {}

  template <class TEvent, class TDeps, class SM>
  auto operator()(const TEvent &event, TDeps &deps, SM &self) /*BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept)*/ {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, deps, self);
  }

 private:
  template <int... Ns, class TEvent, class TDeps, class SM>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TDeps &deps, SM &self)
      /*BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept)*/ {
    auto result = false;
    int _[]{0, (call(aux::get_by_id<Ns>(g), event, deps, self) ? result = true : result)...};
    (void)_;
    return result;
  }

  aux::tuple<Ts...> g;
};
template <class T>
class not_ : operator_base {
 public:
  explicit not_(T t) BOOST_MSM_LITE_NOEXCEPT : g(t) {}

  template <class TEvent, class TDeps, class SM>
  auto operator()(const TEvent &event, TDeps &deps, SM &self) /*BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept)*/ {
    return !call(g, event, deps, self);
  }

 private:
  T g;
};
template <class E, class G>
struct transition<event<E>, G> {
  template <class T>
  auto operator/(const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<event<E>, G, aux::zero_wrapper<T>>{e, g, aux::zero_wrapper<T>{t}};
  }
  event<E> e;
  G g;
};
template <class E, class G, class A>
struct transition<event<E>, G, A> {
  event<E> e;
  G g;
  A a;
};
template <class S2, class G, class A>
struct transition<state<S2>, G, A> : transition<state<S2>, state<S2>, event<anonymous>, G, A> {
  using transition<state<S2>, state<S2>, event<anonymous>, G, A>::g;
  using transition<state<S2>, state<S2>, event<anonymous>, G, A>::a;
  transition(const G &g, const A &a) : transition<state<S2>, state<S2>, event<anonymous>, G, A>{g, a} {}
  template <class T>
  auto operator=(const T &) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<T, state<S2>, event<anonymous>, G, A>{g, a};
  }
};
template <class S1, class S2>
struct transition<state<S1>, state<S2>> : transition<state<S1>, state<S2>, event<anonymous>, always, none> {
  transition(const state<S1> &, const state<S2> &)
      : transition<state<S1>, state<S2>, event<anonymous>, always, none>{always{}, none{}} {}
};
template <class S2, class G>
struct transition_sg<state<S2>, G> : transition<state<S2>, state<S2>, event<anonymous>, G, none> {
  using transition<state<S2>, state<S2>, event<anonymous>, G, none>::g;
  transition_sg(const state<S2> &, const G &g) : transition<state<S2>, state<S2>, event<anonymous>, G, none>{g, none{}} {}
  template <class T>
  auto operator/(const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<state<S2>, G, aux::zero_wrapper<T>>{g, aux::zero_wrapper<T>{t}};
  }
  template <class T>
  auto operator=(const T &) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<T, state<S2>, event<anonymous>, G, none>{g, none{}};
  }
};
template <class S2, class A>
struct transition_sa<state<S2>, A> : transition<state<S2>, state<S2>, event<anonymous>, always, A> {
  using transition<state<S2>, state<S2>, event<anonymous>, always, A>::a;
  transition_sa(const state<S2> &, const A &a) : transition<state<S2>, state<S2>, event<anonymous>, always, A>{always{}, a} {}
  template <class T>
  auto operator=(const T &) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<T, state<S2>, event<anonymous>, always, A>{always{}, a};
  }
};
template <class S2, class E>
struct transition<state<S2>, event<E>> {
  template <class T>
  auto operator=(const T &) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<T, state<S2>, event<E>, always, none>{always{}, none{}};
  }
  const state<S2> &s2;
  event<E> e;
};
template <class E, class G>
struct transition_eg<event<E>, G> {
  template <class T>
  auto operator/(const T &t) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<event<E>, G, aux::zero_wrapper<T>>{e, g, aux::zero_wrapper<T>{t}};
  }
  event<E> e;
  G g;
};
template <class E, class A>
struct transition_ea<event<E>, A> {
  event<E> e;
  A a;
};
template <class S1, class S2, class G, class A>
struct transition<state<S1>, transition<state<S2>, G, A>> : transition<state<S1>, state<S2>, event<anonymous>, G, A> {
  transition(const state<S1> &, const transition<state<S2>, G, A> &t)
      : transition<state<S1>, state<S2>, event<anonymous>, G, A>{t.g, t.a} {}
};
template <class S1, class E, class G, class A>
struct transition<state<S1>, transition<event<E>, G, A>> : transition<state<S1>, state<S1>, event<E>, G, A> {
  using transition<state<S1>, state<S1>, event<E>, G, A>::g;
  using transition<state<S1>, state<S1>, event<E>, G, A>::a;
  transition(const state<S1> &, const transition<event<E>, G, A> &t)
      : transition<state<S1>, state<S1>, event<E>, G, A>{t.g, t.a} {}
  template <class T>
  auto operator=(const T &) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<T, state<S1>, event<E>, G, A>{g, a};
  }
};
template <class S1, class S2, class E>
struct transition<state<S1>, transition<state<S2>, event<E>>> : transition<state<S1>, state<S2>, event<E>, always, none> {
  transition(const state<S1> &, const transition<state<S2>, event<E>> &)
      : transition<state<S1>, state<S2>, event<E>, always, none>{always{}, none{}} {}
};
template <class S1, class S2, class G>
struct transition<state<S1>, transition_sg<state<S2>, G>> : transition<state<S1>, state<S2>, event<anonymous>, G, none> {
  transition(const state<S1> &, const transition_sg<state<S2>, G> &t)
      : transition<state<S1>, state<S2>, event<anonymous>, G, none>{t.g, none{}} {}
};
template <class S1, class S2, class A>
struct transition<state<S1>, transition_sa<state<S2>, A>> : transition<state<S1>, state<S2>, event<anonymous>, always, A> {
  transition(const state<S1> &, const transition_sa<state<S2>, A> &t)
      : transition<state<S1>, state<S2>, event<anonymous>, always, A>{always{}, t.a} {}
};
template <class S2, class E, class G>
struct transition<state<S2>, transition_eg<event<E>, G>> : transition<state<S2>, state<S2>, event<E>, G, none> {
  using transition<state<S2>, state<S2>, event<E>, G, none>::g;
  transition(const state<S2> &, const transition_eg<event<E>, G> &t)
      : transition<state<S2>, state<S2>, event<E>, G, none>{t.g, none{}} {}
  template <class T>
  auto operator=(const T &) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<T, state<S2>, event<E>, G, none>{g, none{}};
  }
};
template <class S1, class S2, class E, class G>
struct transition<state<S1>, transition<state<S2>, transition_eg<event<E>, G>>>
    : transition<state<S1>, state<S2>, event<E>, G, none> {
  transition(const state<S1> &, const transition<state<S2>, transition_eg<event<E>, G>> &t)
      : transition<state<S1>, state<S2>, event<E>, G, none>{t.g, none{}} {}
};
template <class S2, class E, class A>
struct transition<state<S2>, transition_ea<event<E>, A>> : transition<state<S2>, state<S2>, event<E>, always, A> {
  using transition<state<S2>, state<S2>, event<E>, always, A>::a;
  transition(const state<S2> &, const transition_ea<event<E>, A> &t)
      : transition<state<S2>, state<S2>, event<E>, always, A>{always{}, t.a} {}
  template <class T>
  auto operator=(const T &) const BOOST_MSM_LITE_NOEXCEPT {
    return transition<T, state<S2>, event<E>, always, A>{always{}, a};
  }
};
template <class S1, class S2, class E, class A>
struct transition<state<S1>, transition<state<S2>, transition_ea<event<E>, A>>>
    : transition<state<S1>, state<S2>, event<E>, always, A> {
  transition(const state<S1> &, const transition<state<S2>, transition_ea<event<E>, A>> &t)
      : transition<state<S1>, state<S2>, event<E>, always, A>{always{}, t.a} {}
};
template <class S1, class S2, class E, class G, class A>
struct transition<state<S1>, transition<state<S2>, transition<event<E>, G, A>>>
    : transition<state<S1>, state<S2>, event<E>, G, A> {
  transition(const state<S1> &, const transition<state<S2>, transition<event<E>, G, A>> &t)
      : transition<state<S1>, state<S2>, event<E>, G, A>{t.g, t.a} {}
};
template <class S1, class S2, class E, class G, class A>
struct transition<state<S1>, state<S2>, event<E>, G, A> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = G;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, aux::join_t<get_deps_t<G, E>, get_deps_t<A, E>>>;

  transition(const G &g, const A &a) : g(g), a(a) {}

  template <class SM>
  status execute(SM &self, const E &event, aux::byte &current_state) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    if (call(g, event, self.deps_, self.me_)) {
      call(a, event, self.deps_, self.me_);
      self.me_.template update_current_state<typename state<S1>::explicit_states>(
          current_state, aux::get_id<typename SM::states_ids_t, -1, dst_state>(), state<src_state>{}, state<dst_state>{});
      return ret_status<A>();
    }
    return status::NOT_HANDLED;
  }

  G g;
  A a;
};
template <class S1, class S2, class E, class A>
struct transition<state<S1>, state<S2>, event<E>, always, A> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = always;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<A, E>>;

  transition(const always &, const A &a) : a(a) {}

  template <class SM>
  status execute(SM &self, const E &event, aux::byte &current_state) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    call(a, event, self.deps_, self.me_);
    self.me_.template update_current_state<typename state<S1>::explicit_states>(
        current_state, aux::get_id<typename SM::type::states_ids_t, -1, dst_state>(), state<src_state>{}, state<dst_state>{});
    return ret_status<A>();
  }

  A a;
};
template <class S1, class S2, class E, class G>
struct transition<state<S1>, state<S2>, event<E>, G, none> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = G;
  using action = none;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<G, E>>;

  transition(const G &g, const none &) : g(g) {}

  template <class SM>
  status execute(SM &self, const E &event, aux::byte &current_state) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    if (call(g, event, self.deps_, self.me_)) {
      self.me_.template update_current_state<typename state<S1>::explicit_states>(
          current_state, aux::get_id<typename SM::type::states_ids_t, -1, dst_state>(), state<src_state>{}, state<dst_state>{});
      return status::HANDLED;
    }
    return status::NOT_HANDLED;
  }

  G g;
};
template <class S1, class S2, class E>
struct transition<state<S1>, state<S2>, event<E>, always, none> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = always;
  using action = none;
  using deps = aux::type_list<>;

  transition(const always &, const none &) {}

  template <class SM>
  status execute(SM &self, const E &, aux::byte &current_state) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    self.me_.template update_current_state<typename state<S1>::explicit_states>(
        current_state, aux::get_id<typename SM::type::states_ids_t, -1, dst_state>(), state<src_state>{}, state<dst_state>{});
    return status::HANDLED;
  }
  aux::byte _[0];
};
template <class...>
struct transition_impl;
template <class T, class... Ts>
struct transition_impl<T, Ts...> {
  template <class SM, class TEvent>
  static status execute(SM &self, const TEvent &event, aux::byte &current_state) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    if (aux::get<T>(self.me_transitions_).execute(self, event, current_state) != status::NOT_HANDLED) {
      return status::HANDLED;
    }
    return transition_impl<Ts...>::execute(self, event, current_state);
  }
};
template <class T>
struct transition_impl<T> {
  template <class SM, class TEvent>
  static status execute(SM &self, const TEvent &event, aux::byte &current_state) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    return aux::get<T>(self.me_.transitions_).execute(self, event, current_state);
  }

  template <class SM, class>
  static status execute(SM &self, const on_entry &event, aux::byte &current_state) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    aux::get<T>(self.me_.transitions_).execute(self, event, current_state);
    return status::NOT_HANDLED;  // let the top sm process on_entry event
  }

  template <class SM, class>
  static status execute(SM &self, const on_exit &event, aux::byte &current_state) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    aux::get<T>(self.me_.transitions_).execute(self, event, current_state);
    return status::NOT_HANDLED;  // let the top sm process on_exit event
  }
};
template <>
struct transition_impl<> {
  template <class SM, class TEvent>
  static status execute(SM &, const TEvent &, aux::byte &) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    return status::NOT_HANDLED;
  }
};
template <class...>
struct transition_sub_impl;
template <class TSM, class T, class... Ts>
struct transition_sub_impl<TSM, T, Ts...> {
  template <class SM, class TEvent>
  static status execute(SM &self, const TEvent &event, aux::byte &current_state) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    const auto handled = aux::try_get<TSM>(&self.sub_sms_).process_event(event);
    return handled != status::NOT_HANDLED ? handled : transition_impl<T, Ts...>::execute(self, event, current_state);
  }
};
template <class TSM>
struct transition_sub_impl<TSM> {
  template <class SM, class TEvent>
  static status execute(SM &self, const TEvent &event, aux::byte &) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    return aux::try_get<TSM>(&self.sub_sms_).process_event(event);
  }
};
template <class, class>
struct state_mappings;
template <class S, class... Ts>
struct state_mappings<S, aux::type_list<Ts...>> {
  using element_type = state<S>;
  using types = aux::type_list<Ts...>;
};
template <class, class>
struct event_mappings;
template <class E, class... Ts>
struct event_mappings<E, aux::inherit<Ts...>> {
  using element_type = event<E>;
  using types = aux::type_list<Ts...>;
};
template <class...>
struct unique_mappings;
template <class, class...>
struct unique_mappings_impl;
template <class... Ts>
using unique_mappings_t = typename unique_mappings<Ts...>::type;
template <class, class, class, class R>
struct get_mapping : aux::type_list<R> {};
template <class E, class T, class R>
struct get_mapping<event<E>, event<E>, T, R>
    : aux::type_list<event_mappings<E, aux::apply_t<unique_mappings_t, aux::join_t<typename R::types, typename T::types>>>> {};
template <class S, class T, class R>
struct get_mapping<state<S>, state<S>, T, R>
    : aux::type_list<state_mappings<S, aux::join_t<typename R::types, typename T::types>>> {};
template <class T, class... Ts>
struct extend_mapping : aux::join_t<typename get_mapping<typename T::element_type, typename Ts::element_type, T, Ts>::type...> {
};
template <class T, class... Ts>
using extend_mapping_t = aux::apply_t<aux::inherit, typename extend_mapping<T, Ts...>::type>;
template <bool, class, class...>
struct conditional_mapping;
template <class T1, class T2, class... Rs, class... Ts>
struct conditional_mapping<true, aux::type<T1, aux::inherit<Rs...>>, T2, Ts...> {
  using type = unique_mappings_impl<aux::type<aux::inherit<T1>, extend_mapping_t<T2, Rs...>>, Ts...>;
};
template <class T1, class T2, class... Rs, class... Ts>
struct conditional_mapping<false, aux::type<T1, aux::inherit<Rs...>>, T2, Ts...> {
  using type =
      unique_mappings_impl<aux::type<aux::inherit<T1, aux::type<typename T2::element_type>>, aux::inherit<T2, Rs...>>, Ts...>;
};
template <class T1, class T2, class... Rs, class... Ts>
struct unique_mappings_impl<aux::type<T1, aux::inherit<Rs...>>, T2, Ts...>
    : conditional_mapping<aux::is_base_of<aux::type<typename T2::element_type>, T1>::value, aux::type<T1, aux::inherit<Rs...>>,
                          T2, Ts...>::type {};
template <class T1, class Rs>
struct unique_mappings_impl<aux::type<T1, Rs>> : aux::apply_t<aux::inherit, Rs> {};
template <class... Ts>
struct unique_mappings : unique_mappings_impl<aux::type<aux::none_type, aux::inherit<>>, Ts...> {};
template <class T>
struct unique_mappings<T> : aux::inherit<T> {};
template <class, class...>
struct mappings;
template <class... Ts>
struct mappings<aux::pool<Ts...>>
    : unique_mappings_t<
          event_mappings<typename Ts::event, aux::inherit<state_mappings<typename Ts::src_state, aux::type_list<Ts>>>>...> {};
template <class T>
using mappings_t = typename mappings<T>::type;
template <class>
transition_impl<> get_state_mapping_impl(...);
template <class T, class... Ts>
transition_impl<Ts...> get_state_mapping_impl(state_mappings<T, aux::type_list<Ts...>> *);
template <class S>
transition_sub_impl<S> get_sub_state_mapping_impl(...);
template <class T, class... Ts>
transition_sub_impl<T, Ts...> get_sub_state_mapping_impl(state_mappings<T, aux::type_list<Ts...>> *);
template <class T, class U>
struct get_state_mapping {
  using type = decltype(get_state_mapping_impl<T>((U *)0));
};
template <class T, class U>
struct get_state_mapping<sm<T>, U> {
  using type = decltype(get_sub_state_mapping_impl<sm<T>>((U *)0));
};
template <class T, class U>
using get_state_mapping_t = typename get_state_mapping<T, U>::type;
template <class>
transition_impl<> get_event_mapping_impl(...);
template <class T, class TMappings>
TMappings get_event_mapping_impl(event_mappings<T, TMappings> *);
template <class T, class U>
using get_event_mapping_t = decltype(get_event_mapping_impl<T>((U *)0));
template <class, class TEvent>
struct get_all_events_impl {
  using type = aux::conditional_t<aux::is_base_of<internal_event, TEvent>::value, aux::type_list<>, aux::type_list<TEvent>>;
};
template <class T, class TEvent>
struct get_all_events_impl<sm<T>, TEvent> {
  using type = aux::join_t<aux::type_list<TEvent>, typename sm<T>::events>;
};
template <class... Ts>
using get_all_events = aux::join_t<typename get_all_events_impl<typename Ts::src_state, typename Ts::event>::type...>;
template <class... Ts>
using get_events = aux::type_list<typename Ts::event...>;
template <class T>
struct get_exception : aux::type_list<> {};
template <class T>
struct get_exception<exception<T>> : aux::type_list<exception<T>> {};
template <class... Ts>
using get_exceptions = aux::join_t<typename get_exception<Ts>::type...>;
template <class... Ts>
using get_states = aux::join_t<aux::type_list<typename Ts::src_state, typename Ts::dst_state>...>;
template <class... Ts>
using get_initial_states =
    aux::join_t<aux::conditional_t<Ts::initial, aux::type_list<typename Ts::src_state>, aux::type_list<>>...>;
template <class... Ts>
using get_history_states =
    aux::join_t<aux::conditional_t<!Ts::history && Ts::initial, aux::type_list<typename Ts::src_state>, aux::type_list<>>...>;
template <class>
no_policy get_policy(...);
template <class T, class TPolicy>
TPolicy get_policy(aux::pair<T, TPolicy> *);
template<template<class> class, class T>
struct sm_inject {
  using sm = T;
  using all = aux::type_list<T>;
};
template<template<class> class TRebind, class T, class... Ts>
struct sm_inject<TRebind, T(Ts...)> {
  using sm = T;
  using all = aux::join_t<aux::type_list<T, Ts...>, typename TRebind<Ts>::deps...>;
};
template <class SM, class... TPolicies>
struct sm_policy {
  using sm = SM;
  using thread_safety_policy = decltype(get_policy<detail::thread_safety_policy>((aux::inherit<TPolicies...> *)0));
  using exception_safe_policy = decltype(get_policy<detail::exception_safe_policy>((aux::inherit<TPolicies...> *)0));
  using defer_queue_policy = decltype(get_policy<defer_queue_policy>((aux::inherit<TPolicies...> *)0));
};
template <class>
struct get_sub_sm : aux::type_list<> {};
template <class T>
struct get_sub_sm<sm<T>> : aux::type_list<T> {};
template <class... Ts>
using get_sub_sms = aux::join_t<typename get_sub_sm<Ts>::type...>;
template <class... Ts>
using merge_deps = aux::join_t<typename Ts::deps...>;
template<class T, class TDeps, class = void>
struct is_required : aux::false_type {
  static_assert(aux::is_same<T, void>::value, "");
};
template<class T, class TDeps>
struct is_required<T, TDeps, aux::enable_if_t<aux::is_base_of<aux::pool_type<T>, TDeps>::value>> : aux::true_type {};
template <class TSM>
class sm_impl {
  template <class>
  friend class sm;
  template <class>
  friend struct state;
  template <class...>
  friend struct transition;
  template <class...>
  friend struct transition_impl;
  template <class...>
  friend struct transition_sub_impl;
  template<template<class> class, class>
  friend struct sm_inject;

public:
  template<class T>
  using rebind = sm<sm_policy<T>>;
  using sm_t = typename sm_inject<rebind, typename TSM::sm>::sm;
  using sm_raw_t = aux::remove_reference_t<sm_t>;
  using thread_safety_t = typename TSM::thread_safety_policy::type;
  template <class T> using defer_queue_t = typename TSM::defer_queue_policy::template rebind<T>;
  using transitions_t = decltype(aux::declval<sm_raw_t>()());
  using mappings_t = detail::mappings_t<transitions_t>;
  using states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_states, transitions_t>>;
  using states_ids_t = aux::apply_t<aux::type_id, states_t>;
  using initial_states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_initial_states, transitions_t>>;
  using initial_states_ids_t = aux::apply_t<aux::type_id, initial_states_t>;
  using initial_but_not_history_states_t = aux::apply_t<get_history_states, transitions_t>;
  using has_history_states =
      aux::integral_constant<bool, aux::size<initial_states_t>::value != aux::size<initial_but_not_history_states_t>::value>;
  using events_t = aux::apply_t<aux::unique_t, aux::apply_t<get_events, transitions_t>>;
  using events_ids_t = aux::apply_t<aux::pool, events_t>;
  using deps = aux::apply_t<aux::unique_t, aux::join_t<typename sm_inject<rebind, typename TSM::sm>::all, aux::apply_t<detail::merge_deps, transitions_t>>>;
  static constexpr auto regions = aux::size<initial_states_t>::value > 0 ? aux::size<initial_states_t>::value : 1;
  static_assert(regions > 0, "At least one initial state is required");

  template <class... TStates>
  using get_ids = aux::index_sequence<aux::get_id<states_ids_t, -1, TStates>()...>;

 public:
  using states = states_t;
  using events = aux::apply_t<aux::unique_t, aux::apply_t<get_all_events, transitions_t>>;
  using transitions = aux::apply_t<aux::type_list, transitions_t>;

  //sm_impl() BOOST_MSM_LITE_NOEXCEPT : transitions_(aux::try_get<sm_t>(&static_cast<CRTP*>(this)->sub_sms_)()) {
  sm_impl() BOOST_MSM_LITE_NOEXCEPT : transitions_(sm_t{}()) {
    initialize(initial_states_t{});
  }

//#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
  //using exceptions = aux::apply_t<aux::unique_t, aux::apply_t<get_exceptions, events_t>>;
  //static constexpr auto is_noexcept = BOOST_MSM_LITE_NOEXCEPT_IF(aux::declval<sm_raw_t>()());
//#endif

  template <class TEvent, class TDeps, class TSub>
  status process_event(const TEvent &event, TDeps& deps, TSub& sub) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    BOOST_MSM_LITE_LOG(process_event, sm_raw_t, event);
//#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    //const auto handled = process_event_noexcept(event, deps_, aux::integral_constant<bool, is_noexcept>{});
//#else
    const auto handled =
        process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, deps, sub, states_t{}, aux::make_index_sequence<regions>{});
//#endif
    //process_internal_event(anonymous{});
    //process_defer_events(handled, event, deps_, aux::type<defer_queue_t<TEvent>>{});

    return handled;
  }

  template <class TVisitor, BOOST_MSM_LITE_REQUIRES(concepts::callable<void, TVisitor>::value)>
  void visit_current_states(const TVisitor &visitor) const
      BOOST_MSM_LITE_NOEXCEPT_IF(BOOST_MSM_LITE_NOEXCEPT_IF(visitor(state<initial_state>{}))) {
    visit_current_states_impl(visitor, states_t{}, aux::make_index_sequence<regions>{});
  }

  template <class TState>
  bool is(const state<TState> &) const BOOST_MSM_LITE_NOEXCEPT {
    auto result = false;
    visit_current_states([&](auto state) { result |= aux::is_same<TState, typename decltype(state)::type>::value; });
    return result;
  }

  template <class... TStates, BOOST_MSM_LITE_REQUIRES(sizeof...(TStates) == regions)>
  bool is(const state<TStates> &...) const BOOST_MSM_LITE_NOEXCEPT {
    auto result = true;
    auto i = 0;
    int state_ids[] = {aux::get_id<states_ids_t, 0, TStates>()...};
    visit_current_states(
        [&](auto state) { result &= (aux::get_id<states_ids_t, 0, typename decltype(state)::type>() == state_ids[i++]); });
    return result;
  }

 private:
  template <class... TStates>
  void initialize(const aux::type_list<TStates...> &) BOOST_MSM_LITE_NOEXCEPT {
    auto region = 0, i = region;
    int _[]{0, (region = i, current_state_[region] = aux::get_id<states_ids_t, 0, TStates>(), ++i, 0)...};
    (void)_;
    //process_internal_event(anonymous{});
    //process_internal_event(on_entry{});
  }

  void initialize(const aux::type_list<> &) BOOST_MSM_LITE_NOEXCEPT {}

  //template <class TEvent>
  //status process_event_no_deffer(const TEvent &event) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    //BOOST_MSM_LITE_LOG(process_event, sm_raw_t, event);
//#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    //return process_event_noexcept(event, aux::integral_constant<bool, is_noexcept>{});
//#else
    //return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, states_t{}, aux::make_index_sequence<regions>{});
//#endif
    //process_internal_event(anonymous{});
  //}

  //status process_internal_event(...) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) { return status::NOT_HANDLED; }

  //template <class TEvent, BOOST_MSM_LITE_REQUIRES(aux::is_base_of<aux::pool_type<TEvent>, events_ids_t>::value)>
  //status process_internal_event(const TEvent &event) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    //BOOST_MSM_LITE_LOG(process_event, sm_raw_t, event);
//#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    //return process_event_noexcept(event, aux::integral_constant<bool, is_noexcept>{});
//#else
    //return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, states_t{}, aux::make_index_sequence<regions>{});
//#endif
  //}

  //template <class TEvent, BOOST_MSM_LITE_REQUIRES(aux::is_base_of<aux::pool_type<TEvent>, events_ids_t>::value)>
  //status process_internal_event(const TEvent &event, aux::byte &current_state) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    //BOOST_MSM_LITE_LOG(process_event, sm_raw_t, event);
//#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    //return process_event_noexcept(event, current_state, aux::integral_constant<bool, is_noexcept>{});
//#else
    //return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, states_t{}, current_state);
//#endif
  //}

  template <class TMappings, class TEvent, class TDeps, class TSub, class... TStates>
  status process_event_impl(const TEvent &event, TDeps& deps, TSub& sub, const aux::type_list<TStates...> &, const aux::index_sequence<0> &)
      BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    struct self {
      using type = sm_impl;
      TDeps& deps_;
      sm_impl& me_;
      TSub& sub_sms_;
    } self_{deps, *this, sub};

    static status (*dispatch_table[sizeof...(TStates)])(
        self &, const TEvent &, aux::byte &) = {&get_state_mapping_t<TStates, TMappings>::template execute<self, TEvent>...};
    const auto lock = create_lock(aux::type<thread_safety_t>{});
    (void)lock;
    return dispatch_table[current_state_[0]](self_, event, current_state_[0]);
  }

/*  template <class TMappings, class TEvent, class TDeps, class... TStates, int... Ns>*/
  //status process_event_impl(const TEvent &event, TDeps& deps, const aux::type_list<TStates...> &, const aux::index_sequence<Ns...> &)
      //BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    //static status (*dispatch_table[sizeof...(TStates)])(
        //sm_impl &, const TEvent &, aux::byte &) = {&get_state_mapping_t<TStates, TMappings>::template execute<sm_impl, TEvent>...};
    //auto handled = false;
    //const auto lock = create_lock(aux::type<thread_safety_t>{});
    //(void)lock;
    //int _[]{0, (handled |= dispatch_table[current_state_[Ns]](*this, event, current_state_[Ns]) != status::NOT_HANDLED, 0)...};
    //(void)_;
    //return handled ? status::HANDLED : status::NOT_HANDLED;
  /*}*/

/*  template <class TMappings, class TEvent, class TDeps, class... TStates>*/
  //status process_event_impl(const TEvent &event, TDeps& deps, const aux::type_list<TStates...> &, aux::byte &current_state)
      //BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    //static status (*dispatch_table[sizeof...(TStates)])(
        //sm_impl &, const TEvent &, aux::byte &) = {&get_state_mapping_t<TStates, TMappings>::template execute<sm_impl, TEvent>...};
    //const auto lock = create_lock(aux::type<thread_safety_t>{});
    //(void)lock;
    //return dispatch_table[current_state](*this, event, current_state);
  /*}*/


//#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
  //template <class TEvent>
  //status process_event_noexcept(const TEvent &event, const aux::true_type &) noexcept {
    //return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, states_t{}, aux::make_index_sequence<regions>{});
  //}

  //template <class TEvent>
  //status process_event_noexcept(const TEvent &event, aux::byte &current_state, const aux::true_type &) noexcept {
    //return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, states_t{}, current_state);
  //}

  //template <class TEvent>
  //status process_event_noexcept(const TEvent &event, const aux::false_type &) {
    //try {
      //return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, states_t{},
                                                                         //aux::make_index_sequence<regions>{});
    //} catch (...) {
      //return process_exception(exceptions{});
    //}
  //}

  //status process_exception(const aux::type_list<> &) { return process_event(exception<_>{}); }

  //template <class E, class... Es>
  //status process_exception(const aux::type_list<E, Es...> &) {
    //try {
      //throw;
    //} catch (const typename E::type &e) {
      //return process_event(E{e});
    //} catch (...) {
      //return process_exception(aux::type_list<Es...>{});
    //}
  //}
/*#endif*/

/*  template <class TEvent>*/
  //void process_defer_events(const status &, const TEvent &, const aux::type<detail::no_policy> &)
      /*BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {}*/

/*  template <class TEvent, class T>*/
  //void process_defer_events(const status &handled, const TEvent &event, const aux::type<T> &)
      //BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    //if (handled == status::DEFFERED) {
      //defer_.push(event);
    //} else {
      //while (!defer_.empty() && defer_.front().template apply<detail::status>([this](const auto &event) {
        //return process_event_no_deffer(event);
      //}) == status::HANDLED) {
        //defer_.pop();
      //}
    //}
  /*}*/

  template <class TVisitor, class... TStates>
  void visit_current_states_impl(const TVisitor &visitor, const aux::type_list<TStates...> &,
                                 const aux::index_sequence<0> &) const BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    static void (*dispatch_table[sizeof...(TStates)])(const TVisitor &) = {&sm_impl::visit_state<TVisitor, TStates>...};
    dispatch_table[current_state_[0]](visitor);
  }

  template <class TVisitor, class... TStates, int... Ns>
  void visit_current_states_impl(const TVisitor &visitor, const aux::type_list<TStates...> &,
                                 const aux::index_sequence<Ns...> &) const BOOST_MSM_LITE_NOEXCEPT {
    static void (*dispatch_table[sizeof...(TStates)])(const TVisitor &) = {&sm_impl::visit_state<TVisitor, TStates>...};
    int _[]{0, (dispatch_table[current_state_[Ns]](visitor), 0)...};
    (void)_;
  }

  template <class TVisitor, class TState>
  static void visit_state(const TVisitor &visitor) BOOST_MSM_LITE_NOEXCEPT {
    visitor(state<TState>{});
  }

  template <class, class TState>
  void update_current_state(aux::byte &, const aux::byte &, const TState &, const TState &)
      BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {}

  template <class TExplicit, class TSrcState, class TDstState>
  void update_current_state(aux::byte &current_state, const aux::byte &new_state, const TSrcState &src_state,
                            const TDstState &dst_state) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    update_current_state_impl<TExplicit>(current_state, new_state, src_state, dst_state);
  }

  template <class, class TSrcState, class TDstState>
  void update_current_state_impl(aux::byte &current_state, const aux::byte &new_state, const TSrcState &src_state,
                                 const TDstState &dst_state) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    //process_internal_event(on_exit{}, current_state);
    BOOST_MSM_LITE_LOG(state_change, sm_raw_t, src_state, dst_state);
    (void)src_state;
    (void)dst_state;
    current_state = new_state;
    //process_internal_event(on_entry{}, current_state);
  }

  template <class TExplicit, class TSrcState, class T>
  void update_current_state_impl(aux::byte &current_state, const aux::byte &new_state, const TSrcState &src_state,
                                 const state<sm<T>> &dst_state) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    //process_internal_event(on_exit{}, current_state);
    BOOST_MSM_LITE_LOG(state_change, sm_raw_t, src_state, dst_state);
    (void)src_state;
    (void)dst_state;
    current_state = new_state;
    //process_internal_event(on_entry{}, current_state);
    //update_composite_states<T>(TExplicit{}, typename sm<T>::has_history_states{},
                                   //typename sm<T>::initial_but_not_history_states_t{});
  }

  template <class T, class... Ts>  // explicit
  void update_composite_states(const aux::type_list<Ts...> &, ...) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    auto &sm = aux::try_get<T>(&this->sub_sms_);
    int _[]{0, (sm.current_state_[sm.template get_region<Ts>()] = aux::get_id<typename T::states_ids_t, -1, Ts>(), 0)...};
    (void)_;
    // TODO
  }

  template <class T, class... Ts, class... THs>  // history states, no explicit
  void update_composite_states(const aux::type_list<> &, const aux::true_type &, const aux::type_list<THs...> &)
      BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    auto &sm = aux::try_get<T>(&this->sub_sms_);
    int _[]{0, (sm.current_state_[aux::get_id<typename T::initial_states_ids_t, -1, THs>()] =
                    aux::get_id<typename T::states_ids_t, -1, THs>(),
                0)...};
    (void)_;
  }

  template <class T>  // just initials, no explicit
  void update_composite_states(const aux::type_list<> &, const aux::false_type &, ...) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    aux::try_get<T>(&this->sub_sms_).initialize(typename T::initial_states_t{});
  }

  template <class TState>
  static constexpr auto get_region() noexcept {
    return get_region_impl(aux::get_id<states_ids_t, -1, TState>(), aux::apply_t<get_ids, initial_states_t>{});
  }

  template <int... Ids>
  static constexpr auto get_region_impl(int id, const aux::index_sequence<Ids...> &) noexcept {
    auto region = 0, i = 0;
    int _[]{0, (id < Ids ? region : region = i, ++i)...};
    (void)_;
    return region;
  }

  auto create_lock(const aux::type<detail::no_policy> &) { return detail::no_policy{}; }

  template <class T>
  auto create_lock(const aux::type<T> &) {
    struct lock_guard {
      explicit lock_guard(thread_safety_t &synch) : thread_safety_(synch) { thread_safety_.lock(); }

      ~lock_guard() { thread_safety_.unlock(); }

      thread_safety_t &thread_safety_;
    };
    return lock_guard{thread_safety_};
  }

  transitions_t transitions_;

 protected:
  aux::conditional_t<(aux::size<states_t>::value > 0xFF), unsigned short, aux::byte> current_state_[regions];

 private:
  thread_safety_t thread_safety_;
  defer_queue_t<aux::apply_t<aux::variant, events_t>> defer_;
};
template <class TSM>
class sm : public sm_impl<TSM> {
public:
  using type = TSM;
  template<template<class> class, class> friend struct sm_inject;
  template<class T>
  using rebind = sm<sm_policy<T>>;
  using sm_t = typename sm_inject<rebind, typename TSM::sm>::sm;
  using sm_raw_t = aux::remove_reference_t<sm_t>;
  using thread_safety_t = typename TSM::thread_safety_policy::type;
  template <class T> using defer_queue_t = typename TSM::defer_queue_policy::template rebind<T>;
  using transitions_t = decltype(aux::declval<sm_raw_t>()());
  using states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_states, transitions_t>>;
  template<class>
  struct convert;

  template<class... Ts>
  struct convert<aux::type_list<Ts...>> {
    using type = aux::type_list<sm_impl<Ts>...>;
  };
  using sub_sms_t = aux::apply_t<aux::pool, typename convert<aux::apply_t<get_sub_sms, states_t>>::type>;
  using deps = aux::apply_t<aux::unique_t, aux::join_t<typename sm_inject<rebind, typename TSM::sm>::all, aux::apply_t<detail::merge_deps, transitions_t>>>;
  using deps_t = aux::apply_t<aux::pool, deps>;

  template <class... TDeps>
  using dependable = aux::is_same<aux::bool_list<aux::always<TDeps>::value...>,
                                  aux::bool_list<is_required<TDeps, deps_t>::value...>>;

 public:
  sm(sm &&) = default;
  sm(const sm &) = delete;
  sm &operator=(const sm &) = delete;

  template <class... TDeps>//, BOOST_MSM_LITE_REQUIRES(dependable<TDeps...>::value)>
  explicit sm(TDeps &&... deps) BOOST_MSM_LITE_NOEXCEPT : deps_{aux::init{}, aux::pool<TDeps...>{deps...}}//, sub_sms_{deps_}
  { }

  explicit sm(deps_t &deps) BOOST_MSM_LITE_NOEXCEPT : deps_(deps)//, sub_sms_{deps_}
  { }

  template <class TEvent>
  status process_event(const TEvent &event) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    return sm_impl<TSM>::process_event(event, deps_, sub_sms_);
  }

  template <class TEvent>
  status process_event(const event<TEvent> &) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    return process_event(TEvent{});
  }

 private:
  deps_t deps_;
  sub_sms_t sub_sms_;
};
template <class TEvent = void>
struct dispatch_event_impl {
  template <class SM, class T>
  static bool execute(SM &sm, const T &data) BOOST_MSM_LITE_NOEXCEPT {
    return execute_impl(sm, data, aux::is_constructible<TEvent, T>{});
  }

  template <class SM, class T>
  static bool execute_impl(SM &sm, const T &data, const aux::true_type &) BOOST_MSM_LITE_NOEXCEPT {
    return sm.process_event(TEvent(data)) != detail::status::NOT_HANDLED;
  }

  template <class SM, class T>
  static bool execute_impl(SM &sm, const T &, const aux::false_type &) BOOST_MSM_LITE_NOEXCEPT {
    return sm.process_event(TEvent()) != detail::status::NOT_HANDLED;
  }
};
template <>
struct dispatch_event_impl<void> {
  template <class SM, class T>
  static bool execute(SM &, const T &) BOOST_MSM_LITE_NOEXCEPT {
    return false;
  }
};
template <int, class>
struct events_ids_impl {};
template <class... Ts>
struct event_id : events_ids_impl<Ts::id, Ts>... {};
template <int>
dispatch_event_impl<> get_event_impl(...);
template <int N, class T>
dispatch_event_impl<T> get_event_impl(events_ids_impl<N, T> *);
template <int N, class T>
struct get_event {
  using type = decltype(get_event_impl<N>((T *)0));
};
template <int N, class T>
using get_event_t = typename get_event<N, T>::type;
template <class TEvent, int EventRangeBegin, class SM, int... Ns>
auto make_dispatch_table(sm<SM> &fsm, const aux::index_sequence<Ns...> &) BOOST_MSM_LITE_NOEXCEPT {
  using events_ids = aux::apply_t<event_id, typename sm<SM>::events>;
  return [&](const TEvent &event, int id) {
    static bool (*dispatch_table[])(
        sm<SM> &, const TEvent &) = {&get_event_t<Ns + EventRangeBegin, events_ids>::template execute<sm<SM>, TEvent>...};
    return dispatch_table[id - EventRangeBegin](fsm, event);
  };
}
}  // detail
namespace testing {
template <class T, class... TPolicies>
class sm : public detail::sm<detail::sm_policy<T, TPolicies...>> {
  using states_ids_t = aux::apply_t<aux::type_id, typename detail::sm<detail::sm_policy<T, TPolicies...>>::states>;

 public:
  using detail::sm<detail::sm_policy<T, TPolicies...>>::sm;

  template <class... TStates>
  void set_current_states(const detail::state<TStates> &...) BOOST_MSM_LITE_NOEXCEPT {
    auto region = 0, i = region;
    int _[]{0, (region = i, detail::sm<detail::sm_policy<T, TPolicies...>>::current_state_[region] =
                                aux::get_id<states_ids_t, 0, TStates>(),
                ++i, 0)...};
    (void)_;
  }
};
}  // testing
template <class T, BOOST_MSM_LITE_REQUIRES(concepts::callable<bool, T>::value)>
auto operator!(const T &t)BOOST_MSM_LITE_NOEXCEPT {
  return detail::not_<aux::zero_wrapper<T>>(aux::zero_wrapper<T>{t});
}
template <class T1, class T2,
          BOOST_MSM_LITE_REQUIRES(concepts::callable<bool, T1>::value &&concepts::callable<bool, T2>::value)>
auto operator&&(const T1 &t1, const T2 &t2) BOOST_MSM_LITE_NOEXCEPT {
  return detail::and_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
template <class T1, class T2,
          BOOST_MSM_LITE_REQUIRES(concepts::callable<bool, T1>::value &&concepts::callable<bool, T2>::value)>
auto operator||(const T1 &t1, const T2 &t2) BOOST_MSM_LITE_NOEXCEPT {
  return detail::or_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
template <class T1, class T2,
          BOOST_MSM_LITE_REQUIRES(concepts::callable<void, T1>::value &&concepts::callable<void, T2>::value)>
auto operator,(const T1 &t1, const T2 &t2) BOOST_MSM_LITE_NOEXCEPT {
  return detail::seq_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
template <class TEvent>
detail::event<TEvent> event{};
__attribute__((unused)) static const auto on_entry = event<detail::on_entry>;
__attribute__((unused)) static const auto on_exit = event<detail::on_exit>;
template <class T = detail::_>
detail::event<detail::exception<T>> exception{};
template<class T, class = void>
struct state2 {
  using type = detail::state<T>;
};
template<class T>
struct state2<T, aux::enable_if_t<concepts::configurable<T>::value>> {
  using type = detail::state<detail::sm<detail::sm_policy<T>>>;
};
template <class T>
typename state2<T>::type state{};
#if !defined(_MSC_VER)
template <class T, T... Chrs>
auto operator""_s() BOOST_MSM_LITE_NOEXCEPT {
  return detail::state<aux::string<Chrs...>>{};
}
template <class T, T... Chrs>
auto operator""_t() BOOST_MSM_LITE_NOEXCEPT {
  return event<aux::string<Chrs...>>;
}
#endif
template <class T>
struct thread_safe : aux::pair<detail::thread_safety_policy, thread_safe<T>> {
  using type = T;
};
struct exception_safe : aux::pair<detail::exception_safe_policy, exception_safe> {};
template <template <class...> class T>
struct defer_queue : aux::pair<detail::defer_queue_policy, defer_queue<T>> {
  template <class U>
  using rebind = T<U>;
};
__attribute__((unused)) static detail::state<detail::terminate_state> X;
__attribute__((unused)) static detail::history_state H;
__attribute__((unused)) static detail::process_event process_event;
__attribute__((unused)) static detail::defer defer;
template <class... Ts, BOOST_MSM_LITE_REQUIRES(aux::is_same<aux::bool_list<aux::always<Ts>::value...>,
                                                            aux::bool_list<concepts::transitional<Ts>::value...>>::value)>
auto make_transition_table(Ts... ts) BOOST_MSM_LITE_NOEXCEPT {
  return aux::pool<Ts...>{ts...};
}
template <class T, class... TPolicies> /*, BOOST_MSM_LITE_REQUIRES(concepts::configurable<T>::value)*/
using sm = detail::sm<detail::sm_policy<T, TPolicies...>>;
template <class TEvent, int EventRangeBegin, int EventRangeEnd, class SM,
          BOOST_MSM_LITE_REQUIRES(concepts::dispatchable<TEvent, typename sm<SM>::events>::value)>
auto make_dispatch_table(sm<SM> &fsm) BOOST_MSM_LITE_NOEXCEPT {
  static_assert(EventRangeEnd - EventRangeBegin > 0, "Event ids range difference has to be greater than 0");
  return detail::make_dispatch_table<TEvent, EventRangeBegin>(fsm,
                                                              aux::make_index_sequence<EventRangeEnd - EventRangeBegin + 1>{});
}
}  // v_1_0_1
}  // lite
}  // msm
}  // boost
#endif
#if defined(__CLANG__)
#pragma clang diagnostic pop
#elif defined(__GCC__)
#pragma GCC diagnostic pop
#endif

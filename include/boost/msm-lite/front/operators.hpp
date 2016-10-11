#ifndef OPERATORS_AUOQ1Q6
#define OPERATORS_AUOQ1Q6

namespace detail {

struct operator_base {};

template <class, class>
aux::type_list<> args_impl__(...);
template <class T, class>
auto args_impl__(int) -> aux::function_traits_t<T>;
template <class T, class E>
auto args_impl__(int) -> aux::function_traits_t<decltype(&T::template operator() < E >)>;
template <class T, class>
auto args_impl__(int) -> aux::function_traits_t<decltype(&T::operator())>;
template <class T, class E>
using args_t = decltype(args_impl__<T, E>(0));
template <class, class>
struct ignore;
template <class E, class... Ts>
struct ignore<E, aux::type_list<Ts...>> {
  using type = aux::join_t<aux::conditional_t<aux::is_same<E, aux::remove_reference_t<Ts>>::value, aux::type_list<>, aux::type_list<Ts>>...>;
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
          aux::enable_if_t<!aux::is_same<TEvent, aux::remove_reference_t<T>>::value, int> = 0>
decltype(auto) get_arg(const TEvent &, TDeps &deps, SM &) {
  return aux::get<T>(deps);
}
template <class T, class TEvent, class TDeps, class SM,
          aux::enable_if_t<aux::is_same<TEvent, aux::remove_reference_t<T>>::value, int> = 0>
decltype(auto) get_arg(const TEvent &event, TDeps &, SM &) {
  return event;
}
#if defined(BOOST_MSM_LITE_LOG_ENABLED)
template <class... Ts, class T, class TEvent, class TDeps, class SM>
auto call_impl(const aux::type<void> &, const aux::type_list<Ts...> &, T object, const TEvent &event, TDeps &deps,
               sm_impl<SM> &self) {
  object(get_arg<Ts>(event, deps, self)...);
  BOOST_MSM_LITE_LOG(action, SM, object, event);
}
template <class... Ts, class T, class TEvent, class TDeps, class SM>
auto call_impl(const aux::type<bool> &, const aux::type_list<Ts...> &, T object, const TEvent &event, TDeps &deps,
               sm_impl<SM> &self) {
  auto result = object(get_arg<Ts>(event, deps, self)...);
  BOOST_MSM_LITE_LOG(guard, SM, object, event, result);
  return result;
}
#endif
template <class... Ts, class T, class TEvent, class TDeps, class SM,
          aux::enable_if_t<!aux::is_base_of<operator_base, T>::value, int> = 0>
auto call_impl(const aux::type_list<Ts...> &args, T object, const TEvent &event, TDeps &deps, sm_impl<SM> &self) {
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
auto call_impl(const aux::type_list<Ts...> &, T object, const TEvent &event, TDeps &deps, sm_impl<SM> &self) {
  return object(event, deps, self);
}
template <class T, class TEvent, class TDeps, class SM>
auto call(T object, const TEvent &event, TDeps &deps, sm_impl<SM> &self) {
  return call_impl(args_t<T, TEvent>{}, object, event, deps, self);
}
template <class... Ts>
class seq_ : operator_base {
 public:
  explicit seq_(Ts... ts) : a(ts...) {}

  template <class TEvent, class TDeps, class SM>
  void operator()(const TEvent &event, TDeps &deps, SM &self) {
    for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, deps, self);
  }

 private:
  template <int... Ns, class TEvent, class TDeps, class SM>
  void for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TDeps &deps, SM &self) {
    int _[]{0, (call(aux::get_by_id<Ns>(a), event, deps, self), 0)...};
    (void)_;
  }

  aux::tuple<Ts...> a;
};
template <class... Ts>
class and_ : operator_base {
 public:
  explicit and_(Ts... ts) : g(ts...) {}

  template <class TEvent, class TDeps, class SM>
  auto operator()(const TEvent &event, TDeps &deps, SM &self) {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, deps, self);
  }

 private:
  template <int... Ns, class TEvent, class TDeps, class SM>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TDeps &deps, SM &self) {
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
  explicit or_(Ts... ts) : g(ts...) {}

  template <class TEvent, class TDeps, class SM>
  auto operator()(const TEvent &event, TDeps &deps, SM &self) {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, deps, self);
  }

 private:
  template <int... Ns, class TEvent, class TDeps, class SM>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TDeps &deps, SM &self) {
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
  explicit not_(T t) : g(t) {}

  template <class TEvent, class TDeps, class SM>
  auto operator()(const TEvent &event, TDeps &deps, SM &self) {
    return !call(g, event, deps, self);
  }

 private:
  T g;
};

} // detail

template <class T, BOOST_MSM_LITE_REQUIRES(concepts::callable<bool, T>::value)>
auto operator!(const T &t) {
  return detail::not_<aux::zero_wrapper<T>>(aux::zero_wrapper<T>{t});
}
template <class T1, class T2,
          BOOST_MSM_LITE_REQUIRES(concepts::callable<bool, T1>::value &&concepts::callable<bool, T2>::value)>
auto operator&&(const T1 &t1, const T2 &t2) {
  return detail::and_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
template <class T1, class T2,
          BOOST_MSM_LITE_REQUIRES(concepts::callable<bool, T1>::value &&concepts::callable<bool, T2>::value)>
auto operator||(const T1 &t1, const T2 &t2) {
  return detail::or_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
template <class T1, class T2,
          BOOST_MSM_LITE_REQUIRES(concepts::callable<void, T1>::value &&concepts::callable<void, T2>::value)>
auto operator,(const T1 &t1, const T2 &t2) {
  return detail::seq_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}

#endif /* end of include guard: OPERATORS_AUOQ1Q6 */

#ifndef FRONT_TX0KG5V
#define FRONT_TX0KG5V

namespace detail {
struct on_entry;
struct on_exit;
struct terminate_state;

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
template <class>
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
    BOOST_MSM_LITE_NOEXCEPT_IF(sm<SM>::is_noexcept) {
  object(get_arg<Ts>(event, deps, self)...);
  BOOST_MSM_LITE_LOG(action, SM, object, event);
}
template <class... Ts, class T, class TEvent, class TDeps, class SM>
auto call_impl(const aux::type<bool> &, const aux::type_list<Ts...> &, T object, const TEvent &event, TDeps &deps, sm_impl<SM> &self)
    BOOST_MSM_LITE_NOEXCEPT_IF(sm<SM>::is_noexcept) {
  auto result = object(get_arg<Ts>(event, deps, self)...);
  BOOST_MSM_LITE_LOG(guard, SM, object, event, result);
  return result;
}
#endif
template <class... Ts, class T, class TEvent, class TDeps, class SM,
          aux::enable_if_t<!aux::is_base_of<operator_base, T>::value, int> = 0>
auto call_impl(const aux::type_list<Ts...> &args, T object, const TEvent &event, TDeps &deps, sm_impl<SM> &self)
    BOOST_MSM_LITE_NOEXCEPT_IF(sm<SM>::is_noexcept) {
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
    BOOST_MSM_LITE_NOEXCEPT_IF(sm<SM>::is_noexcept) {
  return object(event, deps, self);
}
template <class T, class TEvent, class TDeps, class SM>
auto call(T object, const TEvent &event, TDeps &deps, sm_impl<SM> &self) BOOST_MSM_LITE_NOEXCEPT_IF(sm<SM>::is_noexcept) {
  return call_impl(args_t<T, TEvent>{}, object, event, deps, self);
}
template <class... Ts>
class seq_ : operator_base {
 public:
  explicit seq_(Ts... ts) BOOST_MSM_LITE_NOEXCEPT : a(ts...) {}

  template <class TEvent, class TDeps, class SM>
  void operator()(const TEvent &event, TDeps &deps, SM &self) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, deps, self);
  }

 private:
  template <int... Ns, class TEvent, class TDeps, class SM>
  void for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TDeps &deps, SM &self)
      BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
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
  auto operator()(const TEvent &event, TDeps &deps, SM &self) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, deps, self);
  }

 private:
  template <int... Ns, class TEvent, class TDeps, class SM>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TDeps &deps, SM &self)
      BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
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
  auto operator()(const TEvent &event, TDeps &deps, SM &self) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, deps, self);
  }

 private:
  template <int... Ns, class TEvent, class TDeps, class SM>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TDeps &deps, SM &self)
      BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
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
  auto operator()(const TEvent &event, TDeps &deps, SM &self) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
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
      self.me_.template update_current_state<typename state<S1>::explicit_states>(self,
          current_state, aux::get_id<typename SM::type::states_ids_t, -1, dst_state>(), state<src_state>{}, state<dst_state>{});
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
    self.me_.template update_current_state<typename state<S1>::explicit_states>(self,
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
      self.me_.template update_current_state<typename state<S1>::explicit_states>(self,
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
    self.me_.template update_current_state<typename state<S1>::explicit_states>(self,
        current_state, aux::get_id<typename SM::type::states_ids_t, -1, dst_state>(), state<src_state>{}, state<dst_state>{});
    return status::HANDLED;
  }
  aux::byte _[0];
};

}  // detail
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

#endif

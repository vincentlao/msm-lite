#ifndef BACK_E56KIYAC
#define BACK_E56KIYAC

namespace detail {

template <class...>
struct transition_impl;
template <class T, class... Ts>
struct transition_impl<T, Ts...> {
  template <class SM, class TEvent>
  static status execute(SM &self, const TEvent &event, aux::byte &current_state) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    if (aux::get<T>(self.me_.transitions_).execute(self, event, current_state) != status::NOT_HANDLED) {
      return status::HANDLED;
    }
    return transition_impl<Ts...>::execute(self, event, current_state);
  }
};
template<class...> struct q;
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
struct transition_sub_impl<sm<TSM>, T, Ts...> {
  template <class SM, class TEvent>
  static status execute(SM &self, const TEvent &event, aux::byte &current_state) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    const auto handled = aux::try_get<sm_impl<TSM>>(&self.sub_sms_).process_event(event, self.deps_, self.sub_sms_);
    return handled != status::NOT_HANDLED ? handled : transition_impl<T, Ts...>::execute(self, event, current_state);
  }
};
template <class TSM>
struct transition_sub_impl<sm<TSM>> {
  template <class SM, class TEvent>
  static status execute(SM &self, const TEvent &event, aux::byte &) BOOST_MSM_LITE_NOEXCEPT_IF(SM::is_noexcept) {
    return aux::try_get<sm_impl<TSM>>(&self.sub_sms_).process_event(event, self.deps_, self.sub_sms_);
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
  using type = aux::join_t<aux::type_list<TEvent>, typename sm_impl<T>::events>;
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
struct get_sub_sm<sm<T>> : aux::join_t<aux::type_list<T>, typename sm<T>::sub_sms> {};
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
  template<class>
  friend class sm_impl;
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
  using deps = aux::apply_t<merge_deps, transitions_t>;
  static constexpr auto regions = aux::size<initial_states_t>::value > 0 ? aux::size<initial_states_t>::value : 1;
  static_assert(regions > 0, "At least one initial state is required");

  template <class... TStates>
  using get_ids = aux::index_sequence<aux::get_id<states_ids_t, -1, TStates>()...>;

 public:
  using states = states_t;
  using events = aux::apply_t<aux::unique_t, aux::apply_t<get_all_events, transitions_t>>;
  using transitions = aux::apply_t<aux::type_list, transitions_t>;

  sm_impl(const aux::pool_type<sm_raw_t&>* t) BOOST_MSM_LITE_NOEXCEPT : transitions_((t->value)()) {
    initialize(typename sm_impl<TSM>::initial_states_t{});
  }

  sm_impl(...) BOOST_MSM_LITE_NOEXCEPT : transitions_(sm_t{}()) {
    initialize(typename sm_impl<TSM>::initial_states_t{});
  }

#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
  using exceptions = aux::apply_t<aux::unique_t, aux::apply_t<get_exceptions, events_t>>;
  static constexpr auto is_noexcept = BOOST_MSM_LITE_NOEXCEPT_IF(aux::declval<sm_raw_t>()());
#endif

  template <class TEvent, class TDeps, class TSub>
  status process_event(const TEvent &event, TDeps& deps, TSub& sub) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    BOOST_MSM_LITE_LOG(process_event, sm_raw_t, event);

    struct self {
      using type __attribute__((unused)) = sm_impl;
      TDeps& deps_;
      sm_impl& me_;
      TSub& sub_sms_;
    } self_{deps, *this, sub};

#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    const auto handled = process_event_noexcept(event, self_, aux::integral_constant<bool, is_noexcept>{});
#else
    const auto handled =
        process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self_, states_t{}, aux::make_index_sequence<regions>{});
#endif
    process_internal_event(self_, anonymous{});
    process_defer_events(handled, event, aux::type<defer_queue_t<TEvent>>{});

    return handled;
  }

  template <class TDeps, class TSub>
  void start(TDeps& deps, TSub& sub) {
    struct self {
      using type __attribute__((unused)) = sm_impl;
      TDeps& deps_;
      sm_impl& me_;
      TSub& sub_sms_;
    } self_{deps, *this, sub};

    process_internal_event(self_, anonymous{});
    process_internal_event(self_, on_entry{});
  }

 private:
  void initialize(const aux::type_list<> &) BOOST_MSM_LITE_NOEXCEPT {}

  template <class... TStates>
  void initialize(const aux::type_list<TStates...> &) BOOST_MSM_LITE_NOEXCEPT {
    auto region = 0, i = region;
    int _[]{0, (region = i, current_state_[region] = aux::get_id<states_ids_t, 0, TStates>(), ++i, 0)...};
    (void)_;
  }

  template <class TEvent>
  status process_event_no_deffer(const TEvent &event) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    BOOST_MSM_LITE_LOG(process_event, sm_raw_t, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    return process_event_noexcept(event, aux::integral_constant<bool, is_noexcept>{});
#else
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, states_t{}, aux::make_index_sequence<regions>{});
#endif
    //process_internal_event(anonymous{});
  }

  status process_internal_event(...) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) { return status::NOT_HANDLED; }

  template <class TSelf, class TEvent, BOOST_MSM_LITE_REQUIRES(aux::is_base_of<aux::pool_type<TEvent>, events_ids_t>::value)>
  status process_internal_event(TSelf& self, const TEvent &event) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    BOOST_MSM_LITE_LOG(process_event, sm_raw_t, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    return process_event_noexcept(event, aux::integral_constant<bool, is_noexcept>{});
#else
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self, states_t{}, aux::make_index_sequence<regions>{});
#endif
  }

  template <class TSelf, class TEvent, BOOST_MSM_LITE_REQUIRES(aux::is_base_of<aux::pool_type<TEvent>, events_ids_t>::value)>
  status process_internal_event(TSelf& self, const TEvent &event, aux::byte &current_state) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    BOOST_MSM_LITE_LOG(process_event, sm_raw_t, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    return process_event_noexcept(event, current_state, aux::integral_constant<bool, is_noexcept>{});
#else
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self, states_t{}, current_state);
#endif
  }

  template <class TMappings, class TEvent, class TSelf, class... TStates>
  status process_event_impl(const TEvent &event, TSelf& self, const aux::type_list<TStates...> &, const aux::index_sequence<0> &)
      BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    static status (*dispatch_table[sizeof...(TStates)])(
        TSelf &, const TEvent &, aux::byte &) = {&get_state_mapping_t<TStates, TMappings>::template execute<TSelf, TEvent>...};
    const auto lock = create_lock(aux::type<thread_safety_t>{});
    (void)lock;
    return dispatch_table[current_state_[0]](self, event, current_state_[0]);
  }

  template <class TMappings, class TEvent, class TSelf, class... TStates, int... Ns>
  status process_event_impl(const TEvent &event, TSelf& self, const aux::type_list<TStates...> &, const aux::index_sequence<Ns...> &)
      BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    static status (*dispatch_table[sizeof...(TStates)])(
        TSelf &, const TEvent &, aux::byte &) = {&get_state_mapping_t<TStates, TMappings>::template execute<TSelf, TEvent>...};
    auto handled = false;
    const auto lock = create_lock(aux::type<thread_safety_t>{});
    (void)lock;
    int _[]{0, (handled |= dispatch_table[current_state_[Ns]](self, event, current_state_[Ns]) != status::NOT_HANDLED, 0)...};
    (void)_;
    return handled ? status::HANDLED : status::NOT_HANDLED;
  }

 template <class TMappings, class TEvent, class TSelf, class... TStates>
  status process_event_impl(const TEvent &event, TSelf& self, const aux::type_list<TStates...> &, aux::byte &current_state)
      BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    static status (*dispatch_table[sizeof...(TStates)])(
        TSelf &, const TEvent &, aux::byte &) = {&get_state_mapping_t<TStates, TMappings>::template execute<TSelf, TEvent>...};
    const auto lock = create_lock(aux::type<thread_safety_t>{});
    (void)lock;
    return dispatch_table[current_state](self, event, current_state);
  }


#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
  template <class TEvent>
  status process_event_noexcept(const TEvent &event, const aux::true_type &) noexcept {
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, states_t{}, aux::make_index_sequence<regions>{});
  }

  template <class TEvent>
  status process_event_noexcept(const TEvent &event, aux::byte &current_state, const aux::true_type &) noexcept {
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, states_t{}, current_state);
  }

  template <class TEvent>
  status process_event_noexcept(const TEvent &event, const aux::false_type &) {
    try {
      return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, states_t{},
                                                                         aux::make_index_sequence<regions>{});
    } catch (...) {
      return process_exception(exceptions{});
    }
  }

  status process_exception(const aux::type_list<> &) { return process_event(exception<_>{}); }

  template <class E, class... Es>
  status process_exception(const aux::type_list<E, Es...> &) {
    try {
      throw;
    } catch (const typename E::type &e) {
      return process_event(E{e});
    } catch (...) {
      return process_exception(aux::type_list<Es...>{});
    }
  }
#endif

 template <class TEvent>
  void process_defer_events(const status &, const TEvent &, const aux::type<detail::no_policy> &)
      BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {}

 template <class TEvent, class T>
  void process_defer_events(const status &handled, const TEvent &event, const aux::type<T> &)
      BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    if (handled == status::DEFFERED) {
      defer_.push(event);
    } else {
      while (!defer_.empty() && defer_.front().template apply<detail::status>([this](const auto &event) {
        return process_event_no_deffer(event);
      }) == status::HANDLED) {
        defer_.pop();
      }
    }
  }

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

  template <class, class TSelf, class TState>
  void update_current_state(TSelf&, aux::byte &, const aux::byte &, const TState &, const TState &)
      BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {}

  template <class TExplicit, class TSelf, class TSrcState, class TDstState>
  void update_current_state(TSelf& self, aux::byte &current_state, const aux::byte &new_state, const TSrcState &src_state,
                            const TDstState &dst_state) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    update_current_state_impl<TExplicit>(self, current_state, new_state, src_state, dst_state);
  }

  template <class, class TSelf, class TSrcState, class TDstState>
  void update_current_state_impl(TSelf& self, aux::byte &current_state, const aux::byte &new_state, const TSrcState &src_state,
                                 const TDstState &dst_state) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    process_internal_event(self, on_exit{}, current_state);
    BOOST_MSM_LITE_LOG(state_change, sm_raw_t, src_state, dst_state);
    (void)src_state;
    (void)dst_state;
    current_state = new_state;
    process_internal_event(self, on_entry{}, current_state);
  }

  template <class TExplicit, class TSelf, class TSrcState, class T>
  void update_current_state_impl(TSelf& self, aux::byte &current_state, const aux::byte &new_state, const TSrcState &src_state,
                                 const state<sm<T>> &dst_state) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    process_internal_event(self, on_exit{}, current_state);
    BOOST_MSM_LITE_LOG(state_change, sm_raw_t, src_state, dst_state);
    (void)src_state;
    (void)dst_state;
    current_state = new_state;
    process_internal_event(self, on_entry{}, current_state);
    update_composite_states<sm_impl<T>>(self, TExplicit{}, typename sm_impl<T>::has_history_states{},
                                   typename sm_impl<T>::initial_but_not_history_states_t{});
  }

  template <class T, class TSelf, class... Ts>  // explicit
  void update_composite_states(TSelf& self, const aux::type_list<Ts...> &, ...) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    auto &sm = aux::try_get<T>(&self.sub_sms_);
    int _[]{0, (sm.current_state_[sm.template get_region<Ts>()] = aux::get_id<typename T::states_ids_t, -1, Ts>(), 0)...};
    (void)_;
    // TODO
  }

  template <class T, class TSelf, class... Ts, class... THs>  // history states, no explicit
  void update_composite_states(TSelf& self, const aux::type_list<> &, const aux::true_type &, const aux::type_list<THs...> &)
      BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    auto &sm = aux::try_get<T>(&self.sub_sms_);
    int _[]{0, (sm.current_state_[aux::get_id<typename T::initial_states_ids_t, -1, THs>()] =
                    aux::get_id<typename T::states_ids_t, -1, THs>(),
                0)...};
    (void)_;
  }

  template <class T, class TSelf>  // just initials, no explicit
  void update_composite_states(TSelf& self, const aux::type_list<> &, const aux::false_type &, ...) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    aux::try_get<T>(&self.sub_sms_).initialize(typename T::initial_states_t{});
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
class sm {
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
  using sub_sms = aux::apply_t<get_sub_sms, states_t>;
  using sub_sms_t = aux::apply_t<aux::pool, typename convert<aux::join_t<aux::type_list<TSM>, aux::apply_t<get_sub_sms, states_t>>>::type>;
  using deps = aux::apply_t<merge_deps, transitions_t>;
  using deps_t = aux::apply_t<aux::pool, aux::apply_t<aux::unique_t, aux::join_t<deps, aux::apply_t<merge_deps, sub_sms_t>>>>;

  template <class... TDeps>
  using dependable = aux::is_same<aux::bool_list<aux::always<TDeps>::value...>,
                                  aux::bool_list<is_required<TDeps, deps_t>::value...>>;

 public:
  sm(sm &&) = default;
  sm(const sm &) = delete;
  sm &operator=(const sm &) = delete;

  template <class... TDeps>//, BOOST_MSM_LITE_REQUIRES(dependable<TDeps...>::value)>
  explicit sm(TDeps &&... deps) BOOST_MSM_LITE_NOEXCEPT : deps_{aux::init{}, aux::pool<TDeps...>{deps...}}, sub_sms_{aux::pool<TDeps...>{deps...}} {
    start(aux::type<sub_sms_t>{});
  }

  //TODO
  //explicit sm(deps_t &deps) BOOST_MSM_LITE_NOEXCEPT : deps_(deps), sub_sms_{deps}
  //{ }

  template <class TEvent>
  status process_event(const TEvent &event) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    return static_cast<aux::pool_type<sm_impl<TSM>>&>(sub_sms_).value.process_event(event, deps_, sub_sms_);
  }

  template <class TEvent>
  status process_event(const event<TEvent> &) BOOST_MSM_LITE_NOEXCEPT_IF(is_noexcept) {
    return process_event(TEvent{});
  }

  template <class TVisitor, BOOST_MSM_LITE_REQUIRES(concepts::callable<void, TVisitor>::value)>
  void visit_current_states(const TVisitor &visitor) const
      BOOST_MSM_LITE_NOEXCEPT_IF(BOOST_MSM_LITE_NOEXCEPT_IF(visitor(state<initial_state>{}))) {
    using states_t = typename sm_impl<TSM>::states_t;
    constexpr auto regions = sm_impl<TSM>::regions;
    static_cast<const aux::pool_type<sm_impl<TSM>>&>(sub_sms_).value.visit_current_states_impl(visitor, states_t{}, aux::make_index_sequence<regions>{});
  }

  template <class TState>
  bool is(const state<TState> &) const BOOST_MSM_LITE_NOEXCEPT {
    auto result = false;
    visit_current_states([&](auto state) { result |= aux::is_same<TState, typename decltype(state)::type>::value; });
    return result;
  }

  template <class... TStates, BOOST_MSM_LITE_REQUIRES(sizeof...(TStates) == sm_impl<TSM>::regions)>
  bool is(const state<TStates> &...) const BOOST_MSM_LITE_NOEXCEPT {
    auto result = true;
    auto i = 0;
    using states_ids_t = typename sm_impl<TSM>::states_ids_t;
    int state_ids[] = {aux::get_id<states_ids_t, 0, TStates>()...};
    visit_current_states(
        [&](auto state) { result &= (aux::get_id<states_ids_t, 0, typename decltype(state)::type>() == state_ids[i++]); });
    return result;
  }

 private:
  template<class... Ts>
  void start(const aux::type<aux::pool<Ts...>>&) {
    int _[]{0, (static_cast<aux::pool_type<Ts>&>(sub_sms_).value.start(deps_, sub_sms_), 0)...};
    (void)_;
  }

  deps_t deps_;
  sub_sms_t sub_sms_;
};

} // detail

#endif
#ifndef BACK_E56KIYAC
#define BACK_E56KIYAC

#include "boost/msm-lite/aux_/type_traits.hpp"
#include "boost/msm-lite/aux_/utility.hpp"
#include "boost/msm-lite/back/concepts/callable.hpp"
#include "boost/msm-lite/back/concepts/configurable.hpp"
#include "boost/msm-lite/back/concepts/stringable.hpp"
#include "boost/msm-lite/back/concepts/transitional.hpp"
#include "boost/msm-lite/back/mappings.hpp"
#include "boost/msm-lite/back/policies.hpp"
#include "boost/msm-lite/back/transitions.hpp"

namespace detail {

template <class T>
struct exception {
  using type = T;
  T exception;
};

struct _ {};

struct internal_event {
  static auto c_str() { return "internal_event"; }
};
struct anonymous : internal_event {
  static auto c_str() { return "anonymous"; }
};
struct on_entry : internal_event {
  static auto c_str() { return "on_entry"; }
};
struct on_exit : internal_event {
  static auto c_str() { return "on_exit"; }
};

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
template <template <class> class, class T>
struct sm_inject {
  using sm = T;
};
template <template <class> class TRebind, class T, class... Ts>
struct sm_inject<TRebind, T(Ts...)> {
  using sm = T;
};
template <class SM, class... TPolicies>
struct sm_policy {
  using sm = SM;
  using thread_safety_policy = decltype(get_policy<detail::thread_safety_policy__>((aux::inherit<TPolicies...> *)0));
  using defer_queue_policy = decltype(get_policy<defer_queue_policy__>((aux::inherit<TPolicies...> *)0));
  using logger_policy = decltype(get_policy<logger_policy__>((aux::inherit<TPolicies...> *)0));
};
template <class>
struct get_sub_sm : aux::type_list<> {};
template <class T>
struct get_sub_sm<sm<T>> : aux::join_t<aux::type_list<T>, typename sm<T>::sub_sms> {};
template <class... Ts>
using get_sub_sms = aux::join_t<typename get_sub_sm<Ts>::type...>;
template <class... Ts>
using get_sm_t = aux::type_list<typename Ts::sm...>;
template <class... Ts>
using merge_deps = aux::join_t<typename Ts::deps...>;

template <class TSM>
class sm_impl {
  template <class>
  friend class sm_impl;
  template <class>
  friend class sm;
  template <class>
  friend struct state;
  template <class...>
  friend struct transition;
  template <class...>
  friend struct transitions;
  template <class...>
  friend struct transitions_sub;
  template <template <class> class, class>
  friend struct sm_inject;

 public:
  template <class T>
  using rebind = sm<sm_policy<T>>;
  using sm_t = typename sm_inject<rebind, typename TSM::sm>::sm;
  using sm_raw_t = aux::remove_reference_t<sm_t>;
  using thread_safety_t = typename TSM::thread_safety_policy::type;
  template <class T>
  using defer_queue_t = typename TSM::defer_queue_policy::template rebind<T>;
  using logger_t = typename TSM::logger_policy::type;
  using has_logger = aux::integral_constant<bool, !aux::is_same<logger_t, no_policy>::value>;
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
  sm_impl(const aux::pool_type<sm_raw_t &> *t) : transitions_((t->value)()) {
    initialize(typename sm_impl<TSM>::initial_states_t{});
  }

  sm_impl(...) : transitions_(sm_t{}()) { initialize(typename sm_impl<TSM>::initial_states_t{}); }

#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)  // __pph__
  using exceptions = aux::apply_t<aux::unique_t, aux::apply_t<get_exceptions, events_t>>;
  using has_exceptions = aux::integral_constant<bool, (aux::size<exceptions>::value > 0)>;
#endif  // __pph__

  template <class TEvent, class TDeps, class TSub>
  status process_event(const TEvent &event, TDeps &deps, TSub &sub) {
    log_process_event<logger_t, sm_raw_t>(has_logger{}, deps, event);

    struct self {
      using type __attribute__((unused)) = sm_impl;
      TDeps &deps_;
      sm_impl &me_;
      TSub &sub_sms_;
    } self_{deps, *this, sub};

#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)  // __pph__
    const auto handled = process_event_noexcept(event, self_, has_exceptions{});
#else   // __pph__
    const auto handled = process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self_, states_t{},
                                                                                     aux::make_index_sequence<regions>{});
#endif  // __pph__
    process_internal_event(self_, anonymous{});
    process_defer_events(self_, handled, event, aux::type<defer_queue_t<TEvent>>{});

    return handled;
  }

  template <class TDeps, class TSub>
  void start(TDeps &deps, TSub &sub) {
    struct self {
      using type __attribute__((unused)) = sm_impl;
      TDeps &deps_;
      sm_impl &me_;
      TSub &sub_sms_;
    } self_{deps, *this, sub};

    process_internal_event(self_, anonymous{});
    process_internal_event(self_, on_entry{});
  }

 private:
  void initialize(const aux::type_list<> &) {}

  template <class... TStates>
  void initialize(const aux::type_list<TStates...> &) {
    auto region = 0, i = region;
    int _[]{0, (region = i, current_state_[region] = aux::get_id<states_ids_t, 0, TStates>(), ++i, 0)...};
    (void)_;
  }

  template <class TSelf, class TEvent>
  status process_event_no_deffer(TSelf &self, const TEvent &event) {
    log_process_event<logger_t, sm_raw_t>(has_logger{}, self.deps_, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)  // __pph__
    return process_event_noexcept(event, self, has_exceptions{});
#else   // __pph__
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self, states_t{},
                                                                       aux::make_index_sequence<regions>{});
#endif  // __pph__
    process_internal_event(self, anonymous{});
  }

  status process_internal_event(...) { return status::NOT_HANDLED; }

  template <class TSelf, class TEvent, BOOST_MSM_LITE_REQUIRES(aux::is_base_of<aux::pool_type<TEvent>, events_ids_t>::value)>
  status process_internal_event(TSelf &self, const TEvent &event) {
    log_process_event<logger_t, sm_raw_t>(has_logger{}, self.deps_, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)  // __pph__
    return process_event_noexcept(event, self, has_exceptions{});
#else   // __pph__
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self, states_t{},
                                                                       aux::make_index_sequence<regions>{});
#endif  // __pph__
  }

  template <class TSelf, class TEvent, BOOST_MSM_LITE_REQUIRES(aux::is_base_of<aux::pool_type<TEvent>, events_ids_t>::value)>
  status process_internal_event(TSelf &self, const TEvent &event, aux::byte &current_state) {
    log_process_event<logger_t, sm_raw_t>(has_logger{}, self.deps_, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)  // __pph__
    return process_event_noexcept(event, self, current_state, has_exceptions{});
#else  // __pph__
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self, states_t{}, current_state);
#endif  // __pph__
  }

  template <class TMappings, class TEvent, class TSelf, class... TStates>
  status process_event_impl(const TEvent &event, TSelf &self, const aux::type_list<TStates...> &,
                            const aux::index_sequence<0> &) {
    static status (*dispatch_table[sizeof...(TStates)])(
        TSelf &, const TEvent &, aux::byte &) = {&get_state_mapping_t<TStates, TMappings>::template execute<TSelf, TEvent>...};
    const auto lock = create_lock(aux::type<thread_safety_t>{});
    (void)lock;
    return dispatch_table[current_state_[0]](self, event, current_state_[0]);
  }

  template <class TMappings, class TEvent, class TSelf, class... TStates, int... Ns>
  status process_event_impl(const TEvent &event, TSelf &self, const aux::type_list<TStates...> &,
                            const aux::index_sequence<Ns...> &) {
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
  status process_event_impl(const TEvent &event, TSelf &self, const aux::type_list<TStates...> &, aux::byte &current_state) {
    static status (*dispatch_table[sizeof...(TStates)])(
        TSelf &, const TEvent &, aux::byte &) = {&get_state_mapping_t<TStates, TMappings>::template execute<TSelf, TEvent>...};
    const auto lock = create_lock(aux::type<thread_safety_t>{});
    (void)lock;
    return dispatch_table[current_state](self, event, current_state);
  }

#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)  // __pph__
  template <class TEvent, class TSelf>
  status process_event_noexcept(const TEvent &event, TSelf &self, const aux::false_type &) noexcept {
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self, states_t{},
                                                                       aux::make_index_sequence<regions>{});
  }

  template <class TEvent, class TSelf>
  status process_event_noexcept(const TEvent &event, TSelf &self, aux::byte &current_state, const aux::false_type &) noexcept {
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self, states_t{}, current_state);
  }

  template <class TEvent, class TSelf>
  status process_event_noexcept(const TEvent &event, TSelf &self, const aux::true_type &) {
    try {
      return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self, states_t{},
                                                                         aux::make_index_sequence<regions>{});
    } catch (...) {
      return process_exception(self, exceptions{});
    }
  }

  template <class TSelf>
  status process_exception(TSelf &self, const aux::type_list<> &) {
    return process_event(exception<_>{}, self.deps_, self.sub_sms_);
  }

  template <class TSelf, class E, class... Es>
  status process_exception(TSelf &self, const aux::type_list<E, Es...> &) {
    try {
      throw;
    } catch (const typename E::type &e) {
      return process_event(E{e}, self.deps_, self.sub_sms_);
    } catch (...) {
      return process_exception(self, aux::type_list<Es...>{});
    }
  }
#endif  // __pph__

  template <class TSelf, class TEvent>
  void process_defer_events(TSelf &, const status &, const TEvent &, const aux::type<detail::no_policy> &) {}

  template <class TSelf, class TEvent, class T>
  void process_defer_events(TSelf &self, const status &handled, const TEvent &event, const aux::type<T> &) {
    if (handled == status::DEFFERED) {
      defer_.push(event);
    } else {
      while (!defer_.empty() && defer_.front().template apply<detail::status>([this, &self](const auto &event) {
        return process_event_no_deffer(self, event);
      }) == status::HANDLED) {
        defer_.pop();
      }
    }
  }

  template <class TVisitor, class... TStates>
  void visit_current_states_impl(const TVisitor &visitor, const aux::type_list<TStates...> &,
                                 const aux::index_sequence<0> &) const {
    static void (*dispatch_table[sizeof...(TStates)])(const TVisitor &) = {&sm_impl::visit_state<TVisitor, TStates>...};
    dispatch_table[current_state_[0]](visitor);
  }

  template <class TVisitor, class... TStates, int... Ns>
  void visit_current_states_impl(const TVisitor &visitor, const aux::type_list<TStates...> &,
                                 const aux::index_sequence<Ns...> &) const {
    static void (*dispatch_table[sizeof...(TStates)])(const TVisitor &) = {&sm_impl::visit_state<TVisitor, TStates>...};
    int _[]{0, (dispatch_table[current_state_[Ns]](visitor), 0)...};
    (void)_;
  }

  template <class TVisitor, class TState>
  static void visit_state(const TVisitor &visitor) {
    visitor(state<TState>{});
  }

  template <class, class TSelf, class TState>
  void update_current_state(TSelf &, aux::byte &, const aux::byte &, const TState &, const TState &) {}

  template <class TExplicit, class TSelf, class TSrcState, class TDstState>
  void update_current_state(TSelf &self, aux::byte &current_state, const aux::byte &new_state, const TSrcState &src_state,
                            const TDstState &dst_state) {
    update_current_state_impl<TExplicit>(self, current_state, new_state, src_state, dst_state);
  }

  template <class, class TSelf, class TSrcState, class TDstState>
  void update_current_state_impl(TSelf &self, aux::byte &current_state, const aux::byte &new_state, const TSrcState &src_state,
                                 const TDstState &dst_state) {
    process_internal_event(self, on_exit{}, current_state);
    log_state_change<logger_t, sm_raw_t>(has_logger{}, self.deps_, src_state, dst_state);
    (void)src_state;
    (void)dst_state;
    current_state = new_state;
    process_internal_event(self, on_entry{}, current_state);
  }

  template <class TExplicit, class TSelf, class TSrcState, class T>
  void update_current_state_impl(TSelf &self, aux::byte &current_state, const aux::byte &new_state, const TSrcState &src_state,
                                 const state<sm<T>> &dst_state) {
    process_internal_event(self, on_exit{}, current_state);
    log_state_change<logger_t, sm_raw_t>(has_logger{}, self.deps_, src_state, dst_state);
    (void)src_state;
    (void)dst_state;
    current_state = new_state;
    process_internal_event(self, on_entry{}, current_state);
    update_composite_states<sm_impl<T>>(self, TExplicit{}, typename sm_impl<T>::has_history_states{},
                                        typename sm_impl<T>::initial_but_not_history_states_t{});
  }

  template <class T, class TSelf, class... Ts>  // explicit
  void update_composite_states(TSelf &self, const aux::type_list<Ts...> &, ...) {
    auto &sm = aux::try_get<T>(&self.sub_sms_);
    int _[]{0, (sm.current_state_[sm.template get_region<Ts>()] = aux::get_id<typename T::states_ids_t, -1, Ts>(), 0)...};
    (void)_;
    // TODO
  }

  template <class T, class TSelf, class... Ts, class... THs>  // history states, no explicit
  void update_composite_states(TSelf &self, const aux::type_list<> &, const aux::true_type &, const aux::type_list<THs...> &) {
    auto &sm = aux::try_get<T>(&self.sub_sms_);
    int _[]{0, (sm.current_state_[aux::get_id<typename T::initial_states_ids_t, -1, THs>()] =
                    aux::get_id<typename T::states_ids_t, -1, THs>(),
                0)...};
    (void)_;
  }

  template <class T, class TSelf>  // just initials, no explicit
  void update_composite_states(TSelf &self, const aux::type_list<> &, const aux::false_type &, ...) {
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

 public:
  aux::conditional_t<(aux::size<states_t>::value > 0xFF), unsigned short, aux::byte> current_state_[regions];

 private:
  thread_safety_t thread_safety_;
  defer_queue_t<aux::apply_t<aux::variant, events_t>> defer_;
};
template <class TSM>
class sm {
 public:
  using type = TSM;
  template <template <class> class, class>
  friend struct sm_inject;
  template <class T>
  using rebind = sm<sm_policy<T>>;
  using sm_t = typename sm_inject<rebind, typename TSM::sm>::sm;
  using sm_raw_t = aux::remove_reference_t<sm_t>;
  using thread_safety_t = typename TSM::thread_safety_policy::type;
  template <class T>
  using defer_queue_t = typename TSM::defer_queue_policy::template rebind<T>;
  using logger_t = typename TSM::logger_policy::type;
  using transitions_t = decltype(aux::declval<sm_raw_t>()());
  using states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_states, transitions_t>>;
  template <class>
  struct convert;

  template <class... Ts>
  struct convert<aux::type_list<Ts...>> {
    using type = aux::type_list<sm_impl<Ts>...>;
  };
  using sub_sms = aux::apply_t<get_sub_sms, states_t>;
  using sm_all_t = aux::apply_t<aux::inherit, aux::join_t<aux::type_list<sm_raw_t>, aux::apply_t<get_sm_t, sub_sms>>>;
  using sub_sms_t =
      aux::apply_t<aux::pool, typename convert<aux::join_t<aux::type_list<TSM>, aux::apply_t<get_sub_sms, states_t>>>::type>;
  using deps = aux::apply_t<merge_deps, transitions_t>;
  using deps_t = aux::apply_t<
      aux::pool, aux::apply_t<aux::unique_t, aux::join_t<deps, aux::type_list<logger_t>, aux::apply_t<merge_deps, sub_sms_t>>>>;

  template <class... TDeps>
  using dependable = aux::is_same<aux::bool_list<aux::always<TDeps>::value...>,
                                  aux::bool_list<aux::is_base_of<aux::remove_reference_t<TDeps>, sm_all_t>::value ||
                                                 aux::is_base_of<aux::pool_type<TDeps>, deps_t>::value...>>;

 public:
  using states = typename sm_impl<TSM>::states_t;
  using events = aux::apply_t<aux::unique_t, aux::apply_t<get_all_events, transitions_t>>;
  using transitions = aux::apply_t<aux::type_list, transitions_t>;

  sm(sm &&) = default;
  sm(const sm &) = delete;
  sm &operator=(const sm &) = delete;

  template <class... TDeps, BOOST_MSM_LITE_REQUIRES(dependable<TDeps...>::value)>
  explicit sm(TDeps &&... deps) : deps_{aux::init{}, aux::pool<TDeps...>{deps...}}, sub_sms_{aux::pool<TDeps...>{deps...}} {
    start(aux::type<sub_sms_t>{});
  }

  explicit sm(deps_t &deps) : deps_(deps), sub_sms_{deps} { start(aux::type<sub_sms_t>{}); }

  template <class TEvent>
  void process_event(const TEvent &event) {
    static_cast<aux::pool_type<sm_impl<TSM>> &>(sub_sms_).value.process_event(event, deps_, sub_sms_);
  }

  template <class TEvent>
  void process_event(const event<TEvent> &) {
    process_event(TEvent{});
  }

  template <class TVisitor, BOOST_MSM_LITE_REQUIRES(concepts::callable<void, TVisitor>::value)>
  void visit_current_states(const TVisitor &visitor) const {
    using states_t = typename sm_impl<TSM>::states_t;
    constexpr auto regions = sm_impl<TSM>::regions;
    static_cast<const aux::pool_type<sm_impl<TSM>> &>(sub_sms_).value.visit_current_states_impl(
        visitor, states_t{}, aux::make_index_sequence<regions>{});
  }

  template <class TState>
  bool is(const state<TState> &) const {
    auto result = false;
    visit_current_states([&](auto state) { result |= aux::is_same<TState, typename decltype(state)::type>::value; });
    return result;
  }

  template <class... TStates, BOOST_MSM_LITE_REQUIRES(sizeof...(TStates) == sm_impl<TSM>::regions)>
  bool is(const state<TStates> &...) const {
    auto result = true;
    auto i = 0;
    using states_ids_t = typename sm_impl<TSM>::states_ids_t;
    int state_ids[] = {aux::get_id<states_ids_t, 0, TStates>()...};
    visit_current_states(
        [&](auto state) { result &= (aux::get_id<states_ids_t, 0, typename decltype(state)::type>() == state_ids[i++]); });
    return result;
  }

 private:
  template <class... Ts>
  void start(const aux::type<aux::pool<Ts...>> &) {
    int _[]{0, (static_cast<aux::pool_type<Ts> &>(sub_sms_).value.start(deps_, sub_sms_), 0)...};
    (void)_;
  }

 protected:
  deps_t deps_;
  sub_sms_t sub_sms_;
};

}  // detail

#endif

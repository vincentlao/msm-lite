#ifndef DISPATCH_TABLE_QUT9E60B
#define DISPATCH_TABLE_QUT9E60B

namespace detail {
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
}
template <class TEvent, int EventRangeBegin, int EventRangeEnd, class SM,
          BOOST_MSM_LITE_REQUIRES(concepts::dispatchable<TEvent, typename sm<SM>::events>::value)>
auto make_dispatch_table(sm<SM> &fsm) BOOST_MSM_LITE_NOEXCEPT {
  static_assert(EventRangeEnd - EventRangeBegin > 0, "Event ids range difference has to be greater than 0");
  return detail::make_dispatch_table<TEvent, EventRangeBegin>(fsm,
                                                              aux::make_index_sequence<EventRangeEnd - EventRangeBegin + 1>{});

#endif

#ifndef TRANSITIONS_IOIT6WH
#define TRANSITIONS_IOIT6WH

namespace detail {

enum class status { HANDLED, NOT_HANDLED, DEFFERED };

template <class...>
struct transition_impl;

template <class...>
struct transition_sub_impl;

template <class T, class... Ts>
struct transition_impl<T, Ts...> {
  template <class SM, class TEvent>
  static status execute(SM &self, const TEvent &event, aux::byte &current_state) {
    if (aux::get<T>(self.me_.transitions_).execute(self, event, current_state) != status::NOT_HANDLED) {
      return status::HANDLED;
    }
    return transition_impl<Ts...>::execute(self, event, current_state);
  }
};
template <class T>
struct transition_impl<T> {
  template <class SM, class TEvent>
  static status execute(SM &self, const TEvent &event, aux::byte &current_state) {
    return aux::get<T>(self.me_.transitions_).execute(self, event, current_state);
  }

  template <class SM, class>
  static status execute(SM &self, const on_entry &event, aux::byte &current_state) {
    aux::get<T>(self.me_.transitions_).execute(self, event, current_state);
    return status::NOT_HANDLED;  // let the top sm process on_entry event
  }

  template <class SM, class>
  static status execute(SM &self, const on_exit &event, aux::byte &current_state) {
    aux::get<T>(self.me_.transitions_).execute(self, event, current_state);
    return status::NOT_HANDLED;  // let the top sm process on_exit event
  }
};
template <>
struct transition_impl<> {
  template <class SM, class TEvent>
  static status execute(SM &, const TEvent &, aux::byte &) {
    return status::NOT_HANDLED;
  }
};
template <class TSM, class T, class... Ts>
struct transition_sub_impl<sm<TSM>, T, Ts...> {
  template <class SM, class TEvent>
  static status execute(SM &self, const TEvent &event, aux::byte &current_state) {
    const auto handled = aux::try_get<sm_impl<TSM>>(&self.sub_sms_).process_event(event, self.deps_, self.sub_sms_);
    return handled != status::NOT_HANDLED ? handled : transition_impl<T, Ts...>::execute(self, event, current_state);
  }
};
template <class TSM>
struct transition_sub_impl<sm<TSM>> {
  template <class SM, class TEvent>
  static status execute(SM &self, const TEvent &event, aux::byte &) {
    return aux::try_get<sm_impl<TSM>>(&self.sub_sms_).process_event(event, self.deps_, self.sub_sms_);
  }
};

} // detail
#endif

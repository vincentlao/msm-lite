#ifndef TRANSITIONS_IOIT6WH
#define TRANSITIONS_IOIT6WH

namespace detail {

enum class status { HANDLED, NOT_HANDLED, DEFFERED };

template <class...>
struct transitions;

template <class...>
struct transitions_sub;

template <class T, class... Ts>
struct transitions<T, Ts...> {
  template <class SM, class TEvent>
  static status execute(SM &self, const TEvent &event, aux::byte &current_state) {
    if (aux::get<T>(self.me_.transitions_).execute(self, event, current_state) != status::NOT_HANDLED) {
      return status::HANDLED;
    }
    return transitions<Ts...>::execute(self, event, current_state);
  }
};
template <class T>
struct transitions<T> {
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
struct transitions<> {
  template <class SM, class TEvent>
  static status execute(SM &, const TEvent &, aux::byte &) {
    return status::NOT_HANDLED;
  }
};
template <class TSM, class T, class... Ts>
struct transitions_sub<sm<TSM>, T, Ts...> {
  template <class SM, class TEvent>
  static status execute(SM &self, const TEvent &event, aux::byte &current_state) {
    const auto handled = aux::try_get<sm_impl<TSM>>(&self.sub_sms_).process_event(event, self.deps_, self.sub_sms_);
    return handled != status::NOT_HANDLED ? handled : transitions<T, Ts...>::execute(self, event, current_state);
  }
};
template <class TSM>
struct transitions_sub<sm<TSM>> {
  template <class SM, class TEvent>
  static status execute(SM &self, const TEvent &event, aux::byte &) {
    return aux::try_get<sm_impl<TSM>>(&self.sub_sms_).process_event(event, self.deps_, self.sub_sms_);
  }
};

}  // detail
#endif

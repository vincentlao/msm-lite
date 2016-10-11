#ifndef PROCESS_EVENT_JOH07846
#define PROCESS_EVENT_JOH07846

namespace detail {

struct process_event {
  template <class TEvent>
  struct process_event_impl {
    template <class SM, class T>
    void operator()(sm<SM> &sm, const T &) {
      sm.process_event(event);
    }

    TEvent event;
  };

  template <class TEvent>
  auto operator()(const TEvent &event) {
    return process_event_impl<TEvent>{event};
  }
};

} // detail

#endif

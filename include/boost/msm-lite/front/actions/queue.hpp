#ifndef QUEUE_Q3QVBZJG
#define QUEUE_Q3QVBZJG

namespace detail {

struct queue {
  template <class TEvent>
  struct queue_impl {
    template <class T>
    void operator()(const T &) {
      // sm.process_event(event);
    }

    TEvent event;
  };

  template <class TEvent>
  auto operator()(const TEvent &event) {
    return queue_impl<TEvent>{event};
  }
};

}  // detail

#endif

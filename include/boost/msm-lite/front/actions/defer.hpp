#ifndef DEFER_W9M9BM8H
#define DEFER_W9M9BM8H

namespace detail {

struct defer {
  template <class SM, class T>
  void operator()(sm<SM> &, const T &) {
    // static_assert(aux::is_same<typename sm<SM>::template defer_queue_t<T>, no_policy>::value, "");
  }
};

} // detail
#endif

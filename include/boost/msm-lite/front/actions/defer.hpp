#ifndef DEFER_W9M9BM8H
#define DEFER_W9M9BM8H

namespace detail {

struct defer {
  template <class TEvent>
  void operator()(const TEvent &) {
  }
};

}  // detail
#endif

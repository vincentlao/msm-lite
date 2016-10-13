#ifndef DEFER_W9M9BM8H
#define DEFER_W9M9BM8H

namespace detail {

struct action_base {};

struct defer : action_base {
  template <class TSelf, class TEvent>
  void operator()(TSelf&, const TEvent &) {}
};

}  // detail
#endif

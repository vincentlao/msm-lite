#ifndef POLICIES_Y0QZTJTY
#define POLICIES_Y0QZTJTY

namespace detail {

struct thread_safety_policy {};
struct exception_safe_policy {};
struct defer_queue_policy__ {};
struct logging_policy {};

struct no_policy {
  using type = no_policy;
  template <class>
  using rebind = no_policy;
  aux::byte _[0];
};

}  // detail

#endif

#ifndef SM_VEYB5F0H
#define SM_VEYB5F0H

namespace testing {
template <class T, class... TPolicies>
class sm : public detail::sm<detail::sm_policy<T, TPolicies...>> {
  using states_ids_t = aux::apply_t<aux::type_id, typename detail::sm<detail::sm_policy<T, TPolicies...>>::states>;

 public:
  using detail::sm<detail::sm_policy<T, TPolicies...>>::sm;

  template <class... TStates>
  void set_current_states(const detail::state<TStates> &...) {
    auto region = 0, i = region;
    int _[]{0, (region = i, detail::sm<detail::sm_policy<T, TPolicies...>>::current_state_[region] =
                                aux::get_id<states_ids_t, 0, TStates>(),
                ++i, 0)...};
    (void)_;
  }
};
}  // testing

#endif

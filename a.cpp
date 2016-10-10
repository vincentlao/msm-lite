#include <iostream>
#include <cstdio>
#include <typeinfo>

template <class SM, class TEvent>
void log_process_event(const TEvent&) {
  printf("[%s][process_event] %s\n", typeid(SM).name(), typeid(TEvent).name());
}

template <class SM, class TGuard, class TEvent>
void log_guard(const TGuard&, const TEvent&, bool result) {
  printf("[%s][guard] %s %s %s\n", typeid(SM).name(), typeid(TGuard).name(), typeid(TEvent).name(),
         (result ? "[OK]" : "[Reject]"));
}

template <class SM, class TAction, class TEvent>
void log_action(const TAction&, const TEvent&) {
  printf("[%s][action] %s %s\n", typeid(SM).name(), typeid(TAction).name(), typeid(TEvent).name());
}

template <class SM, class TSrcState, class TDstState>
void log_state_change(const TSrcState& src, const TDstState& dst) {
  printf("[%s][transition] %s -> %s\n", typeid(SM).name(), src.c_str(), dst.c_str());
}

//#define BOOST_MSM_LITE_LOG(T, SM, ...) log_##T<SM>(__VA_ARGS__)
#include <boost/msm-lite.hpp>

namespace msm = boost::msm::lite;

struct e1{};
struct e2{};
struct e3{};
struct e4{};
struct e5{};
struct e6{};
struct e7{};

struct sub2 {
  auto operator()() {
    using namespace msm;
    return make_transition_table(
      *"s1"_s + event<e3> / [](double&) { std::cout << "sub sub action" << std::endl; } = "s2"_s,
      "s2"_s + event<e4> / [] { std::cout << "exit sub sub action" << std::endl; }
    );
  }
};

struct sub {
  auto operator()() {
    using namespace msm;
    return make_transition_table(
      *"s1"_s + event<e2> / [](double&) { std::cout << "sub action" << std::endl; } = state<sub2>,
       state<sub2> + event<e5> / [] { std::cout << "exit sub action" << std::endl; }
    );
  }
};

struct c {
  auto operator()() {
    using namespace msm;
    return make_transition_table(
      *"idle"_s + event<e1> / [](int&) { std::cout << "action" << std::endl; } = state<sub>,
      *state<sub> + event<e6> / [] { std::cout << "exit" << std::endl; } = X
    );
  }
};


template<class> struct q;

int main() {
  c c_;
  sub s_;
  int i = 2;
  double d = 2.0;
  //msm::sm<c&(sub&)> sm{c_, s_, d, i};

  //q<typename msm::sm<c>::states_ids_t>{};

  msm::sm<c> sm{d, i};
  sm.process_event(e1{});
  sm.process_event(e2{});
  sm.process_event(e3{});
  sm.process_event(e4{});
  sm.process_event(e5{});
  sm.process_event(e6{});
}

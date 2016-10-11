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
  sub2(bool, bool){}
  auto operator()() {
    using namespace msm;
    return make_transition_table(
      *"s1"_s + event<e3> / [this](double&) { blah = true; std::cout << "sub sub action" << std::endl; } = "s2"_s,
      "s2"_s + event<e4> / [] { std::cout << "exit sub sub action" << std::endl; }
    );
  }

  bool blah = false;
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

/*struct c {*/
  //c(int){}
  //auto operator()() {
    //using namespace msm;
    //return make_transition_table(
      //*"idle"_s + event<e1> / [](int&) { std::cout << "action" << std::endl; } = state<sub>,
      //state<sub> + event<e6> / [] { std::cout << "exit" << std::endl; } = X
    //);
  //}
/*};*/


template<class> struct q;

  struct sub_1 {
    auto operator()() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
        *"ls1_1"_s + event<e1> = "ls1_2"_s
       , "ls1_2"_s + event<e2> = "ls1_1"_s
       , "ls1_1"_s + msm::on_entry / [this] { ls1_1_entry = true; }
       , "ls1_1"_s + msm::on_exit  / [this] { ls1_1_exit = true; }
       , "ls1_2"_s + msm::on_entry / [this] { ls1_2_entry = true; }
       , "ls1_2"_s + msm::on_exit  / [this] { ls1_2_exit = true; }
      );
      // clang-format on
    }

    bool ls1_1_entry = false;
    bool ls1_1_exit = false;
    bool ls1_2_entry = false;
    bool ls1_2_exit = false;
  };

  struct sub_2 {
    auto operator()() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
        *"ls2_1"_s + event<e1> = "ls2_2"_s
       , "ls2_2"_s + event<e2> = "ls2_1"_s
       , "ls2_1"_s + msm::on_entry / [this] { ls2_1_entry = true; }
       , "ls2_1"_s + msm::on_exit  / [this] { ls2_1_exit = true; }
       , "ls2_2"_s + msm::on_entry / [this] { ls2_2_entry = true; }
       , "ls2_2"_s + msm::on_exit  / [this] { ls2_2_exit = true; }
      );
      // clang-format on
    }

    bool ls2_1_entry = false;
    bool ls2_1_exit = false;
    bool ls2_2_entry = false;
    bool ls2_2_exit = false;
  };

  struct c {
    auto operator()() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
        *state<sub_1> + event<e3> = state<sub_2>
       , state<sub_2> + event<e4> = state<sub_1>
       , state<sub_1> + msm::on_entry / [this] { sub1_entry = true; }
       , state<sub_1> + msm::on_exit  / [this] { sub1_exit = true; }
       , state<sub_2> + msm::on_entry / [this] { sub2_entry = true; }
       , state<sub_2> + msm::on_exit  / [this] { sub2_exit = true; }
      );
      // clang-format on
    }

    bool sub1_entry = false;
    bool sub1_exit = false;
    bool sub2_entry = false;
    bool sub2_exit = false;
  };


int main() {

  c c_;
  sub_1 s1;
  sub_2 s2;
  msm::sm<c> sm{c_, s1, s2};
/*  c c_{42};*/
  //sub s_;
  //int i = 2;
  //sub2 s2{false, false};
  //double d = 2.0;
  ////msm::sm<c&(sub&)> sm{c_, s_, d, i};

  ////q<typename msm::sm<c>::states_ids_t>{};

  //msm::sm<c> sm{d, i, c_, s2};
  //std::cout << s2.blah << std::endl;
  //sm.process_event(e1{});
  //sm.process_event(e2{});
  //sm.process_event(e3{});
  //sm.process_event(e4{});
  //sm.process_event(e5{});
  //sm.process_event(e6{});
  /*std::cout << s2.blah << std::endl;*/
}

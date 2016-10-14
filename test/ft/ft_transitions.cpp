//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <string>
#include <utility>
#include "boost/msm-lite.hpp"

namespace msm = boost::msm::lite;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};
struct e6 {};

const auto idle = msm::state<class idle>;
const auto s1 = msm::state<class s1>;
const auto s2 = msm::state<class s2>;
const auto s3 = msm::state<class s3>;
const auto s4 = msm::state<class s4>;
const auto s5 = msm::state<class s5>;
const auto end = msm::state<class end>;

test transition = [] {
  using namespace msm;
  struct c {
    auto operator()() noexcept { return make_transition_table(*idle + event<e1> = s1); }
  };

  msm::sm<c> sm;
  expect(sm.is(idle));
  sm.process_event(e1{});
  expect(sm.is(s1));
};

test internal_transition = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> / [] {}
        , s1 + event<e3> = s2
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  expect(sm.is(idle));
  sm.process_event(e1{});
  expect(sm.is(s1));
  sm.process_event(e2{});
  expect(sm.is(s1));
  sm.process_event(e3{});
  expect(sm.is(s2));
};

test anonymous_transition = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
       *idle / [this] { a_called = true; } = s1
      );
      // clang-format on
    }
    bool a_called = false;
  };

  c c_;
  msm::sm<c> sm{c_};
  expect(sm.is(s1));
  expect(c_.a_called);
};

test no_transition = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      return make_transition_table(*idle + event<e1> = s1);
    }
  };

  msm::sm<c> sm;
  expect(sm.is(idle));
  sm.process_event(e2{});
  expect(sm.is(idle));
  sm.process_event(int{});
  expect(sm.is(idle));
};

test transition_with_action_with_event = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      auto action = [this](const e1 &) { called = true; };
      return make_transition_table(*idle + event<e1> / action = s1);
    }

    bool called = false;
  };

  c c_;
  msm::sm<c> sm{c_};
  expect(sm.is(idle));
  sm.process_event(e1{});
  expect(c_.called);
  expect(sm.is(s1));
};

test transition_with_action_with_parameter = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      auto action = [this](int i) {
        called = true;
        expect(i == 42);
      };
      return make_transition_table(*idle + event<e1> / action = s1);
    }

    bool called = false;
  };

  c c_;
  msm::sm<c> sm{c_, 42};
  sm.process_event(e1{});
  expect(c_.called);
  expect(sm.is(s1));
};

test transition_with_action_and_guad_with_parameter = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;

      auto guard = [this](double d) {
        g_called = true;
        expect(d == 87.0);
        return true;
      };

      auto action = [this](int i) {
        a_called = true;
        expect(i == 42);
      };

      return make_transition_table(*idle + event<e1>[guard] / action = s1);
    }

    bool a_called = false;
    bool g_called = false;
  };

  c c_;
  msm::sm<c> sm{c_, 87.0, 42};
  sm.process_event(e1{});
  expect(c_.g_called);
  expect(c_.a_called);
  expect(sm.is(s1));
};

test transition_with_action_and_guad_with_parameters_and_event = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;

      auto guard = [this](int i, auto e, double d) {
        g_called = true;
        static_expect(msm::aux::is_same<decltype(e), e1>::value);
        expect(i == 42);
        expect(d == 87.0);
        return true;
      };

      auto action = [this](int i, float &f) {
        a_called = true;
        expect(i == 42);
        expect(f == 12.f);
      };

      return make_transition_table(*idle + event<e1>[guard] / action = s1);
    }

    bool a_called = false;
    bool g_called = false;
  };

  c c_;
  auto f = 12.f;
  msm::sm<c> sm{c_, 42, 87.0, f};
  sm.process_event(e1{});
  expect(c_.g_called);
  expect(c_.a_called);
  expect(sm.is(s1));
};

test transitions = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      auto yes = [] { return true; };
      auto no = [] { return false; };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> = s2
        , s2 + event<e3> [no] = s3
        , s2 + event<e3> [yes] = s4
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  sm.process_event(e1{});
  sm.process_event(e2{});
  sm.process_event(e3{});
  expect(sm.is(s4));
};

test transitions_dsl = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      auto yes = [] { return true; };
      auto no = [] { return false; };

      // clang-format off
      return make_transition_table(
          s1 <= *idle + event<e1>
        , s2 <= s1 + event<e2>
        , s3 <= s2 + event<e3> [no]
        , s4 <= s2 + event<e3> [yes]
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  sm.process_event(e1{});
  sm.process_event(e2{});
  sm.process_event(e3{});
  expect(sm.is(s4));
};

test transitions_dsl_mix = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      auto yes = [] { return true; };
      auto no = [] { return false; };

      // clang-format off
      return make_transition_table(
          s1 <= *idle + event<e1>
        , s1 + event<e2> = s2
        , s3 <= s2 + event<e3> [no]
        , s2 + event<e3> [yes] = s4
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  sm.process_event(e1{});
  sm.process_event(e2{});
  sm.process_event(e3{});
  expect(sm.is(s4));
};

test transition_loop = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> = s2
        , s2 + event<e3> = idle
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  expect(sm.is(idle));
  sm.process_event(e1{});
  expect(sm.is(s1));
  sm.process_event(e2{});
  expect(sm.is(s2));
  sm.process_event(e3{});
  expect(sm.is(idle));
  sm.process_event(e1{});
  expect(sm.is(s1));
};

test no_transitions = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      auto yes = [] { return true; };
      auto no = [] { return false; };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> [no] = s2
        , s1 + event<e2> [yes] = s2
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  sm.process_event(e1{});
  sm.process_event(e3{});
  sm.process_event(e2{});
  sm.process_event(e1{});
  expect(sm.is(s2));
};

test transitions_states = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      auto yes = [] { return true; };
      auto no = [] { return false; };

      // clang-format off
      return make_transition_table(
         *"idle"_s + event<e1> = "s1"_s
        , "s1"_s + event<e2> = "s2"_s
        , "s2"_s + event<e3> [no] = "s3"_s
        , "s2"_s + event<e3> [yes] = "s4"_s
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  sm.process_event(e1{});
  sm.process_event(e2{});
  sm.process_event(e3{});
  using namespace msm;
  expect(sm.is("s4"_s));
};

test transition_overload = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<int> = s2
        , s1 + event<float> = s3
      );
      // clang-format on
    }
  };

  {
    msm::sm<c> sm;
    sm.process_event(e1{});
    sm.process_event(42);
    expect(sm.is(s2));
  }

  {
    msm::sm<c> sm;
    sm.process_event(e1{});
    sm.process_event(42.f);
    expect(sm.is(s3));
  }
};

test initial_transition_overload = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , idle + event<e2> = s2
      );
      // clang-format on
    }
  };

  {
    msm::sm<c> sm;
    sm.process_event(e1{});
    expect(sm.is(s1));
  }

  {
    msm::sm<c> sm;
    sm.process_event(e2{});
    expect(sm.is(s2));
  }
};

test transition_entry_exit_actions = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      auto entry_action = [this] { a_entry_action++; };
      auto exit_action = [this] { a_exit_action++; };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + msm::on_entry / entry_action
        , s1 + msm::on_exit / exit_action
        , s1 + event<e2> = s2
      );
      // clang-format on
    }

    int a_entry_action = 0;
    int a_exit_action = 0;
  };

  c c_;
  msm::sm<c> sm{c_};
  expect(!c_.a_entry_action);
  expect(sm.is(idle));
  sm.process_event(e1{});
  expect(1 == c_.a_entry_action);
  expect(sm.is(s1));
  expect(0 == c_.a_exit_action);
  sm.process_event(e2{});
  expect(1 == c_.a_entry_action);
  expect(1 == c_.a_exit_action);
  expect(sm.is(s2));
};

struct c_guard {
  template <class T>
  bool operator()(const T &) const noexcept {
    return true;
  }
};

struct c_action {
  explicit c_action(int) {}
  template <class T>
  void operator()(const T &) noexcept {}
};

test transition_types = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      auto guard1 = [] { return true; };
      auto guard2 = [](auto) { return false; };
      auto guard3 = [=](int v) { return [=] { return guard2(v); }; };
      auto action1 = [] {};
      auto action2 = [](int, auto, float &) {};

      // clang-format off
      return make_transition_table(
         *idle + event<e1> [guard1] / (action1, []{}) = s1
        , s1 + event<e2> / ([] { }) = s2
        , s2 + event<e4> / defer
        , s1 / [] {}
        , s1 [guard1 && guard3(42)]
        , s1 [guard1] / action1
        , s1 + event<e3> / action1
        , idle = s1
        , s3 + event<e4> / [] { }
        , s3 + event<e5> [guard1] / action1
        , s1 [guard1 && guard1 && [] { return true; }] = s2
        , (*s3) [guard1] = s2
        , s2 [guard1 && !guard2] = s3
        , s3 [guard1] / action1 = s4
        , s4 / action1 = s5
        , s5 / (action1, action2) = end
        , idle(H) + event<e1> = s1
        , idle(H) + event<e1> [c_guard{} && guard1] = s1
        ,*idle + event<e1> [guard1] / action1 = s1
        , idle + event<e1> / action1 = s1
        , idle + event<e1> / (action1, c_action{1}) = s1
        , idle + event<e1> [guard1 && guard2] / (action1, action2) = s1
        , idle + event<e1> [guard1 && guard2] / (action1, action2, []{}) = s1
        , idle + event<e1> [guard1 || !guard2] / (action1, action2, []{}, [](auto){}) = s1
        , idle + event<e2> [guard1 || guard2] / (action1, action2, []{}, [](int, auto, float){}) = s1
        , idle + event<e1> [guard1 && guard2 && [] { return true; } ] / (action1, action2, []{}, [](int, auto, float){}) = X
        , idle + event<e1> [guard1 && guard2 && [] { return true; } && [] (auto) { return false; } ] / (action1, action2, []{}, [](int, auto, double){}) = X
        //
        , s1 <= *idle + event<e1> [guard1] / (action1, []{})
        , s2 <= s1 + event<e2> / ([] { })
        , s1 <= s2 + event<e4> / defer
        , s2 <= s1 [guard1 && guard1 && [] { return true; }]
        , s2 <= (*s3) [guard1]
        , s2 <= s3 [guard1 && !guard2]
        , s3 <= s4 [guard1] / action1
        , s4 <= s5 / action1
        , end <= s5 / (action1, action2)
        , s1 <= idle(H) + event<e1>
        , s1 <= idle(H) + event<e1> [c_guard{} && guard1]
        ,*s1 <= idle + event<e1> [guard1] / action1
        , s1 <= idle + event<e1> / action1
        , s1 <= idle + event<e1> / (action1, c_action{1}, c_action{2})
        , s1 <= idle + event<e1> [guard1 && guard2] / (action1, action2)
        , s1 <= idle + event<e1> [guard1 && guard2] / (action1, action2, []{})
        , s1 <= idle + event<e1> [guard1 || !guard2] / (action1, action2, []{}, [](auto){})
        , s1 <= idle + event<e2> [guard1 || guard2] / (action1, action2, []{}, [](int, auto, float){})
        , X <= idle + event<e1> [guard1 && guard2 && [] { return true; } ] / (action1, action2, []{}, [](int, auto, float){})
        , X <= idle + event<e1> [guard1 && guard2 && [] { return true; } && [] (auto) { return false; } ] / (action1, action2, []{}, [](int, auto, double){})
      );
      // clang-format on
    }
  };

  float f = 12.0;
  msm::sm<c> sm{f, 42, 87.0, 0.0f};
};

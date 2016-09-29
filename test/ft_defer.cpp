//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <queue>
#include <vector>
#include "boost/msm-lite.hpp"

namespace msm = boost::msm::lite;

struct event1 {};
struct event2 {};

test defer_and_transitions = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
       *"state1"_s + event<event1> / defer,
        "state1"_s + on_entry / [this] { entries.emplace_back(1); },
        "state2"_s + event<event1> / defer,
        "state2"_s + on_entry / [this] { entries.emplace_back(2); },
        "state1"_s + event<event2> = "state2"_s,
        "state2"_s + event<event2> = "state3"_s,
        "state3"_s + event<event1> = "state4"_s,
        "state3"_s + on_entry / [this] { entries.emplace_back(3); },
        "state4"_s + event<event1> = "state5"_s,
        "state4"_s + on_entry / [this] { entries.emplace_back(4); },
        "state5"_s + event<event1> = "state6"_s,
        "state5"_s + on_entry / [this] { entries.emplace_back(5); },
        "state6"_s + on_entry / [this] { entries.emplace_back(6); }
      );
      // clang-format on
    }

    std::vector<int> entries;
  };

  msm::sm<c, msm::defer_queue<std::queue>> sm;
  sm.process_event(event1());
  sm.process_event(event1());
  sm.process_event(event2());
  sm.process_event(event2());

  // state1::on_entry()
  // state2::on_entry()
  // state3::on_entry()
  // state4::on_entry()
  // state5::on_entry()
};

test defer_and_anonymous = [] {
  struct c {
    auto configure() const noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
       *"state1"_s + event<event1> / defer,
        "state1"_s + on_entry / [] {std::cout << "state1 on entry" << std::endl; },
        "state1"_s + event<event2> = "state2"_s,
        "state2"_s  = "state3"_s,
        "state2"_s + on_entry / [] {std::cout << "state2 on entry" << std::endl; },
        "state3"_s + event<event1> = "state4"_s,
        "state4"_s + on_entry / [] {std::cout << "state4 on entry" << std::endl; },
        "state3"_s + on_entry / [] {std::cout << "state3 on entry" << std::endl; },
        "state2"_s + event<event1> = "state5"_s,
        "state5"_s + on_entry / [] {std::cout << "state5 on entry" << std::endl; }
      );
      // clang-format on
    }
  };

  sm<sub&, exeception>{sub, 42};
  msm::sm<c, msm::defer_queue<std::queue>> sm;
  sm.process_event(event1());
  sm.process_event(event2());

  // state1::on_entry()
  // state2::on_entry()
  // state3::on_entry()
  // state4::on_entry()
};

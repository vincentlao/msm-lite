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

test events = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
          *"idle"_s + "e1"_t = "s1"_s
        , "s1"_s + "e2"_t = "s2"_s
        , "s2"_s + "e3"_t / [](auto) { } = X
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  using namespace msm;
  sm.process_event("e1"_t);
  expect(sm.is("s1"_s));
  sm.process_event("e2"_t);
  expect(sm.is("s2"_s));
  sm.process_event("e3"_t);
  expect(sm.is(X));
};


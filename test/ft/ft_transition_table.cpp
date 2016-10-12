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

test empty = [] {
  struct c {
    auto operator()() noexcept { return msm::make_transition_table(); }
  };

  msm::sm<c> sm;
};

test ctor = [] {
  struct c {
    auto operator()() noexcept { return msm::make_transition_table(); }
  };

  msm::sm<c> sm;
  msm::sm<c> sm_{std::move(sm)};
  (void)sm_;
};

test minimal = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      return make_transition_table(*idle + event<e1> / [] {});
    }
  };

  msm::sm<c> sm;
  expect(sm.is(idle));
  sm.process_event(e1{});
  expect(sm.is(idle));
};

test operators = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      auto yes = [] { return true; };
      auto no = [] { return false; };
      auto action = [](int &i) { i++; };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> [ yes || no ] / (action, action) = s1
        , s1 + event<e1> [ !no && yes ] / action = s2
        , s2 + event<e1> [ no && yes ] / (action, [](int&i) {i++;}) = s3
        , s2 + event<e2> [ yes && [] { return true; } ] / (action, [](int&i) {i++;}) = s3
      );
      // clang-format on
    }
  };

  int i = 0;
  msm::sm<c> sm{i};

  {
    i = 0;
    sm.process_event(e1{});
    expect(sm.is(s1));
    expect(2 == i);
  }

  {
    i = 0;
    sm.process_event(e1{});
    expect(sm.is(s2));
    expect(1 == i);
  }

  {
    i = 0;
    sm.process_event(e1{});
    expect(sm.is(s2));
    expect(0 == i);
  }

  {
    i = 0;
    sm.process_event(e2{});
    expect(sm.is(s3));
    expect(2 == i);
  }
};

test uml_notation = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> = X
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  sm.process_event(e1{});
  expect(sm.is(s1));
  sm.process_event(e2{});
  expect(sm.is(msm::X));
};

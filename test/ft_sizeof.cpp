//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/msm-lite.hpp"

namespace msm = boost::msm::lite;

test transition_sizeof = [] {
  using namespace msm;
  constexpr auto i = 0;

  {
    auto t = "state"_s + "event"_t[([] {})];
    static_expect(0 == sizeof(t));
  }

  {
    auto t = "state"_s + "event"_t / [] {};
    static_expect(0 == sizeof(t));
  }

  {
    auto t = "state"_s + "event"_t[([] {})] / [] {};
    static_expect(0 == sizeof(t));
  }

  {
    auto t = "state"_s + "event"_t[([](int) {})] / [] {};
    static_expect(0 == sizeof(t));
  }

  {
    auto t = "state"_s + "event"_t[([] {})] / [](int) {};
    static_expect(0 == sizeof(t));
  }

  {
    auto t = "state"_s + "event"_t[([](int) {})] / [](int) {};
    static_expect(0 == sizeof(t));
  }

  {
    auto t = "state"_s + "event"_t[([](int, float) {})] / [](double, const int&) {};
    static_expect(0 == sizeof(t));
  }

  {
    auto t = "state"_s + "event"_t[([i] {})] / [] {};
    static_expect(sizeof(i) == sizeof(t));
  }

  {
    auto t = "state"_s + "event"_t[([] {})] / [i] {};
    static_expect(sizeof(i) == sizeof(t));
  }

  {
    auto t = "state"_s + "event"_t[([] {})] / [&i] {};
    static_expect(sizeof(&i) == sizeof(t));
  }
};

test sm_sizeof_empty = [] {
  struct empty {
    auto configure() noexcept {
      using namespace msm;
      return make_transition_table();
    }
  };

  static_expect(2 /*deps=1, current_state=1*/ == sizeof(msm::sm<empty>{}));
};

test sm_sizeof_default_guard_action = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
        *"idle"_s + "event1"_t = "s1"_s,
        "s1"_s + "event2"_t = X,
        "s2"_s + "event2"_t [([]{})] = X,
        "s3"_s + "event3"_t / []{} = X,
        "s4"_s + "event4"_t [([]{})] / []{} = X,
        "s5"_s + "event5"_t = X
      );
      // clang-format on
    }
  };

  static_expect(2 /*deps=1, current_state=1*/ == sizeof(msm::sm<c>{}));
};

test sm_sizeof_no_capture = [] {
  struct no_capture_transition {
    auto configure() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
        *"idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{}
      );
      // clang-format on
    }
  };
  static_expect(2 /*deps=1, current_state=1*/ == sizeof(msm::sm<no_capture_transition>));
};

test sm_sizeof_more_than_256_transitions = [] {
  struct c {
    auto configure() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
        *"s1"_s + "event1"_t [([]{})] / []{} = "s2"_s,
        "s2"_s + "event2"_t [([]{})] / []{} = "s3"_s,
        "s3"_s + "event3"_t [([]{})] / []{} = "s4"_s,
        "s4"_s + "event4"_t [([]{})] / []{} = "s5"_s,
        "s5"_s + "event5"_t [([]{})] / []{} = "s6"_s,
        "s6"_s + "event6"_t [([]{})] / []{} = "s7"_s,
        "s7"_s + "event7"_t [([]{})] / []{} = "s8"_s,
        "s8"_s + "event8"_t [([]{})] / []{} = "s9"_s,
        "s9"_s + "event9"_t [([]{})] / []{} = "s10"_s,
        "s10"_s + "event10"_t [([]{})] / []{} = "s11"_s,
        "s11"_s + "event11"_t [([]{})] / []{} = "s12"_s,
        "s12"_s + "event12"_t [([]{})] / []{} = "s13"_s,
        "s13"_s + "event13"_t [([]{})] / []{} = "s14"_s,
        "s14"_s + "event14"_t [([]{})] / []{} = "s15"_s,
        "s15"_s + "event15"_t [([]{})] / []{} = "s16"_s,
        "s16"_s + "event16"_t [([]{})] / []{} = "s17"_s,
        "s17"_s + "event17"_t [([]{})] / []{} = "s18"_s,
        "s18"_s + "event18"_t [([]{})] / []{} = "s19"_s,
        "s19"_s + "event19"_t [([]{})] / []{} = "s20"_s,
        "s20"_s + "event20"_t [([]{})] / []{} = "s21"_s,
        "s21"_s + "event21"_t [([]{})] / []{} = "s22"_s,
        "s22"_s + "event22"_t [([]{})] / []{} = "s23"_s,
        "s23"_s + "event23"_t [([]{})] / []{} = "s24"_s,
        "s24"_s + "event24"_t [([]{})] / []{} = "s25"_s,
        "s25"_s + "event25"_t [([]{})] / []{} = "s26"_s,
        "s26"_s + "event26"_t [([]{})] / []{} = "s27"_s,
        "s27"_s + "event27"_t [([]{})] / []{} = "s28"_s,
        "s28"_s + "event28"_t [([]{})] / []{} = "s29"_s,
        "s29"_s + "event29"_t [([]{})] / []{} = "s30"_s,
        "s30"_s + "event30"_t [([]{})] / []{} = "s31"_s,
        "s31"_s + "event31"_t [([]{})] / []{} = "s32"_s,
        "s32"_s + "event32"_t [([]{})] / []{} = "s33"_s,
        "s33"_s + "event33"_t [([]{})] / []{} = "s34"_s,
        "s34"_s + "event34"_t [([]{})] / []{} = "s35"_s,
        "s35"_s + "event35"_t [([]{})] / []{} = "s36"_s,
        "s36"_s + "event36"_t [([]{})] / []{} = "s37"_s,
        "s37"_s + "event37"_t [([]{})] / []{} = "s38"_s,
        "s38"_s + "event38"_t [([]{})] / []{} = "s39"_s,
        "s39"_s + "event39"_t [([]{})] / []{} = "s40"_s,
        "s40"_s + "event40"_t [([]{})] / []{} = "s41"_s,
        "s41"_s + "event41"_t [([]{})] / []{} = "s42"_s,
        "s42"_s + "event42"_t [([]{})] / []{} = "s43"_s,
        "s43"_s + "event43"_t [([]{})] / []{} = "s44"_s,
        "s44"_s + "event44"_t [([]{})] / []{} = "s45"_s,
        "s45"_s + "event45"_t [([]{})] / []{} = "s46"_s,
        "s46"_s + "event46"_t [([]{})] / []{} = "s47"_s,
        "s47"_s + "event47"_t [([]{})] / []{} = "s48"_s,
        "s48"_s + "event48"_t [([]{})] / []{} = "s49"_s,
        "s49"_s + "event49"_t [([]{})] / []{} = "s50"_s,
        "s50"_s + "event50"_t [([]{})] / []{} = "s51"_s,
        "s51"_s + "event51"_t [([]{})] / []{} = "s52"_s,
        "s52"_s + "event52"_t [([]{})] / []{} = "s53"_s,
        "s53"_s + "event53"_t [([]{})] / []{} = "s54"_s,
        "s54"_s + "event54"_t [([]{})] / []{} = "s55"_s,
        "s55"_s + "event55"_t [([]{})] / []{} = "s56"_s,
        "s56"_s + "event56"_t [([]{})] / []{} = "s57"_s,
        "s57"_s + "event57"_t [([]{})] / []{} = "s58"_s,
        "s58"_s + "event58"_t [([]{})] / []{} = "s59"_s,
        "s59"_s + "event59"_t [([]{})] / []{} = "s60"_s,
        "s60"_s + "event60"_t [([]{})] / []{} = "s61"_s,
        "s61"_s + "event61"_t [([]{})] / []{} = "s62"_s,
        "s62"_s + "event62"_t [([]{})] / []{} = "s63"_s,
        "s63"_s + "event63"_t [([]{})] / []{} = "s64"_s,
        "s64"_s + "event64"_t [([]{})] / []{} = "s65"_s,
        "s65"_s + "event65"_t [([]{})] / []{} = "s66"_s,
        "s66"_s + "event66"_t [([]{})] / []{} = "s67"_s,
        "s67"_s + "event67"_t [([]{})] / []{} = "s68"_s,
        "s68"_s + "event68"_t [([]{})] / []{} = "s69"_s,
        "s69"_s + "event69"_t [([]{})] / []{} = "s70"_s,
        "s70"_s + "event70"_t [([]{})] / []{} = "s71"_s,
        "s71"_s + "event71"_t [([]{})] / []{} = "s72"_s,
        "s72"_s + "event72"_t [([]{})] / []{} = "s73"_s,
        "s73"_s + "event73"_t [([]{})] / []{} = "s74"_s,
        "s74"_s + "event74"_t [([]{})] / []{} = "s75"_s,
        "s75"_s + "event75"_t [([]{})] / []{} = "s76"_s,
        "s76"_s + "event76"_t [([]{})] / []{} = "s77"_s,
        "s77"_s + "event77"_t [([]{})] / []{} = "s78"_s,
        "s78"_s + "event78"_t [([]{})] / []{} = "s79"_s,
        "s79"_s + "event79"_t [([]{})] / []{} = "s80"_s,
        "s80"_s + "event80"_t [([]{})] / []{} = "s81"_s,
        "s81"_s + "event81"_t [([]{})] / []{} = "s82"_s,
        "s82"_s + "event82"_t [([]{})] / []{} = "s83"_s,
        "s83"_s + "event83"_t [([]{})] / []{} = "s84"_s,
        "s84"_s + "event84"_t [([]{})] / []{} = "s85"_s,
        "s85"_s + "event85"_t [([]{})] / []{} = "s86"_s,
        "s86"_s + "event86"_t [([]{})] / []{} = "s87"_s,
        "s87"_s + "event87"_t [([]{})] / []{} = "s88"_s,
        "s88"_s + "event88"_t [([]{})] / []{} = "s89"_s,
        "s89"_s + "event89"_t [([]{})] / []{} = "s90"_s,
        "s90"_s + "event90"_t [([]{})] / []{} = "s91"_s,
        "s91"_s + "event91"_t [([]{})] / []{} = "s92"_s,
        "s92"_s + "event92"_t [([]{})] / []{} = "s93"_s,
        "s93"_s + "event93"_t [([]{})] / []{} = "s94"_s,
        "s94"_s + "event94"_t [([]{})] / []{} = "s95"_s,
        "s95"_s + "event95"_t [([]{})] / []{} = "s96"_s,
        "s96"_s + "event96"_t [([]{})] / []{} = "s97"_s,
        "s97"_s + "event97"_t [([]{})] / []{} = "s98"_s,
        "s98"_s + "event98"_t [([]{})] / []{} = "s99"_s,
        "s99"_s + "event99"_t [([]{})] / []{} = "s100"_s,
        "s100"_s + "event100"_t [([]{})] / []{} = "s101"_s,
        "s101"_s + "event101"_t [([]{})] / []{} = "s102"_s,
        "s102"_s + "event102"_t [([]{})] / []{} = "s103"_s,
        "s103"_s + "event103"_t [([]{})] / []{} = "s104"_s,
        "s104"_s + "event104"_t [([]{})] / []{} = "s105"_s,
        "s105"_s + "event105"_t [([]{})] / []{} = "s106"_s,
        "s106"_s + "event106"_t [([]{})] / []{} = "s107"_s,
        "s107"_s + "event107"_t [([]{})] / []{} = "s108"_s,
        "s108"_s + "event108"_t [([]{})] / []{} = "s109"_s,
        "s109"_s + "event109"_t [([]{})] / []{} = "s110"_s,
        "s110"_s + "event110"_t [([]{})] / []{} = "s111"_s,
        "s111"_s + "event111"_t [([]{})] / []{} = "s112"_s,
        "s112"_s + "event112"_t [([]{})] / []{} = "s113"_s,
        "s113"_s + "event113"_t [([]{})] / []{} = "s114"_s,
        "s114"_s + "event114"_t [([]{})] / []{} = "s115"_s,
        "s115"_s + "event115"_t [([]{})] / []{} = "s116"_s,
        "s116"_s + "event116"_t [([]{})] / []{} = "s117"_s,
        "s117"_s + "event117"_t [([]{})] / []{} = "s118"_s,
        "s118"_s + "event118"_t [([]{})] / []{} = "s119"_s,
        "s119"_s + "event119"_t [([]{})] / []{} = "s120"_s,
        "s120"_s + "event120"_t [([]{})] / []{} = "s121"_s,
        "s121"_s + "event121"_t [([]{})] / []{} = "s122"_s,
        "s122"_s + "event122"_t [([]{})] / []{} = "s123"_s,
        "s123"_s + "event123"_t [([]{})] / []{} = "s124"_s,
        "s124"_s + "event124"_t [([]{})] / []{} = "s125"_s,
        "s125"_s + "event125"_t [([]{})] / []{} = "s126"_s,
        "s126"_s + "event126"_t [([]{})] / []{} = "s127"_s,
        "s127"_s + "event127"_t [([]{})] / []{} = "s128"_s,
        "s128"_s + "event128"_t [([]{})] / []{} = "s129"_s,
        "s129"_s + "event129"_t [([]{})] / []{} = "s130"_s,
        "s130"_s + "event130"_t [([]{})] / []{} = "s131"_s,
        "s131"_s + "event131"_t [([]{})] / []{} = "s132"_s,
        "s132"_s + "event132"_t [([]{})] / []{} = "s133"_s,
        "s133"_s + "event133"_t [([]{})] / []{} = "s134"_s,
        "s134"_s + "event134"_t [([]{})] / []{} = "s135"_s,
        "s135"_s + "event135"_t [([]{})] / []{} = "s136"_s,
        "s136"_s + "event136"_t [([]{})] / []{} = "s137"_s,
        "s137"_s + "event137"_t [([]{})] / []{} = "s138"_s,
        "s138"_s + "event138"_t [([]{})] / []{} = "s139"_s,
        "s139"_s + "event139"_t [([]{})] / []{} = "s140"_s,
        "s140"_s + "event140"_t [([]{})] / []{} = "s141"_s,
        "s141"_s + "event141"_t [([]{})] / []{} = "s142"_s,
        "s142"_s + "event142"_t [([]{})] / []{} = "s143"_s,
        "s143"_s + "event143"_t [([]{})] / []{} = "s144"_s,
        "s144"_s + "event144"_t [([]{})] / []{} = "s145"_s,
        "s145"_s + "event145"_t [([]{})] / []{} = "s146"_s,
        "s146"_s + "event146"_t [([]{})] / []{} = "s147"_s,
        "s147"_s + "event147"_t [([]{})] / []{} = "s148"_s,
        "s148"_s + "event148"_t [([]{})] / []{} = "s149"_s,
        "s149"_s + "event149"_t [([]{})] / []{} = "s150"_s,
        "s150"_s + "event150"_t [([]{})] / []{} = "s151"_s,
        "s151"_s + "event151"_t [([]{})] / []{} = "s152"_s,
        "s152"_s + "event152"_t [([]{})] / []{} = "s153"_s,
        "s153"_s + "event153"_t [([]{})] / []{} = "s154"_s,
        "s154"_s + "event154"_t [([]{})] / []{} = "s155"_s,
        "s155"_s + "event155"_t [([]{})] / []{} = "s156"_s,
        "s156"_s + "event156"_t [([]{})] / []{} = "s157"_s,
        "s157"_s + "event157"_t [([]{})] / []{} = "s158"_s,
        "s158"_s + "event158"_t [([]{})] / []{} = "s159"_s,
        "s159"_s + "event159"_t [([]{})] / []{} = "s160"_s,
        "s160"_s + "event160"_t [([]{})] / []{} = "s161"_s,
        "s161"_s + "event161"_t [([]{})] / []{} = "s162"_s,
        "s162"_s + "event162"_t [([]{})] / []{} = "s163"_s,
        "s163"_s + "event163"_t [([]{})] / []{} = "s164"_s,
        "s164"_s + "event164"_t [([]{})] / []{} = "s165"_s,
        "s165"_s + "event165"_t [([]{})] / []{} = "s166"_s,
        "s166"_s + "event166"_t [([]{})] / []{} = "s167"_s,
        "s167"_s + "event167"_t [([]{})] / []{} = "s168"_s,
        "s168"_s + "event168"_t [([]{})] / []{} = "s169"_s,
        "s169"_s + "event169"_t [([]{})] / []{} = "s170"_s,
        "s170"_s + "event170"_t [([]{})] / []{} = "s171"_s,
        "s171"_s + "event171"_t [([]{})] / []{} = "s172"_s,
        "s172"_s + "event172"_t [([]{})] / []{} = "s173"_s,
        "s173"_s + "event173"_t [([]{})] / []{} = "s174"_s,
        "s174"_s + "event174"_t [([]{})] / []{} = "s175"_s,
        "s175"_s + "event175"_t [([]{})] / []{} = "s176"_s,
        "s176"_s + "event176"_t [([]{})] / []{} = "s177"_s,
        "s177"_s + "event177"_t [([]{})] / []{} = "s178"_s,
        "s178"_s + "event178"_t [([]{})] / []{} = "s179"_s,
        "s179"_s + "event179"_t [([]{})] / []{} = "s180"_s,
        "s180"_s + "event180"_t [([]{})] / []{} = "s181"_s,
        "s181"_s + "event181"_t [([]{})] / []{} = "s182"_s,
        "s182"_s + "event182"_t [([]{})] / []{} = "s183"_s,
        "s183"_s + "event183"_t [([]{})] / []{} = "s184"_s,
        "s184"_s + "event184"_t [([]{})] / []{} = "s185"_s,
        "s185"_s + "event185"_t [([]{})] / []{} = "s186"_s,
        "s186"_s + "event186"_t [([]{})] / []{} = "s187"_s,
        "s187"_s + "event187"_t [([]{})] / []{} = "s188"_s,
        "s188"_s + "event188"_t [([]{})] / []{} = "s189"_s,
        "s189"_s + "event189"_t [([]{})] / []{} = "s190"_s,
        "s190"_s + "event190"_t [([]{})] / []{} = "s191"_s,
        "s191"_s + "event191"_t [([]{})] / []{} = "s192"_s,
        "s192"_s + "event192"_t [([]{})] / []{} = "s193"_s,
        "s193"_s + "event193"_t [([]{})] / []{} = "s194"_s,
        "s194"_s + "event194"_t [([]{})] / []{} = "s195"_s,
        "s195"_s + "event195"_t [([]{})] / []{} = "s196"_s,
        "s196"_s + "event196"_t [([]{})] / []{} = "s197"_s,
        "s197"_s + "event197"_t [([]{})] / []{} = "s198"_s,
        "s198"_s + "event198"_t [([]{})] / []{} = "s199"_s,
        "s199"_s + "event199"_t [([]{})] / []{} = "s200"_s,
        "s200"_s + "event200"_t [([]{})] / []{} = "s201"_s,
        "s201"_s + "event201"_t [([]{})] / []{} = "s202"_s,
        "s202"_s + "event202"_t [([]{})] / []{} = "s203"_s,
        "s203"_s + "event203"_t [([]{})] / []{} = "s204"_s,
        "s204"_s + "event204"_t [([]{})] / []{} = "s205"_s,
        "s205"_s + "event205"_t [([]{})] / []{} = "s206"_s,
        "s206"_s + "event206"_t [([]{})] / []{} = "s207"_s,
        "s207"_s + "event207"_t [([]{})] / []{} = "s208"_s,
        "s208"_s + "event208"_t [([]{})] / []{} = "s209"_s,
        "s209"_s + "event209"_t [([]{})] / []{} = "s210"_s,
        "s210"_s + "event210"_t [([]{})] / []{} = "s211"_s,
        "s211"_s + "event211"_t [([]{})] / []{} = "s212"_s,
        "s212"_s + "event212"_t [([]{})] / []{} = "s213"_s,
        "s213"_s + "event213"_t [([]{})] / []{} = "s214"_s,
        "s214"_s + "event214"_t [([]{})] / []{} = "s215"_s,
        "s215"_s + "event215"_t [([]{})] / []{} = "s216"_s,
        "s216"_s + "event216"_t [([]{})] / []{} = "s217"_s,
        "s217"_s + "event217"_t [([]{})] / []{} = "s218"_s,
        "s218"_s + "event218"_t [([]{})] / []{} = "s219"_s,
        "s219"_s + "event219"_t [([]{})] / []{} = "s220"_s,
        "s220"_s + "event220"_t [([]{})] / []{} = "s221"_s,
        "s221"_s + "event221"_t [([]{})] / []{} = "s222"_s,
        "s222"_s + "event222"_t [([]{})] / []{} = "s223"_s,
        "s223"_s + "event223"_t [([]{})] / []{} = "s224"_s,
        "s224"_s + "event224"_t [([]{})] / []{} = "s225"_s,
        "s225"_s + "event225"_t [([]{})] / []{} = "s226"_s,
        "s226"_s + "event226"_t [([]{})] / []{} = "s227"_s,
        "s227"_s + "event227"_t [([]{})] / []{} = "s228"_s,
        "s228"_s + "event228"_t [([]{})] / []{} = "s229"_s,
        "s229"_s + "event229"_t [([]{})] / []{} = "s230"_s,
        "s230"_s + "event230"_t [([]{})] / []{} = "s231"_s,
        "s231"_s + "event231"_t [([]{})] / []{} = "s232"_s,
        "s232"_s + "event232"_t [([]{})] / []{} = "s233"_s,
        "s233"_s + "event233"_t [([]{})] / []{} = "s234"_s,
        "s234"_s + "event234"_t [([]{})] / []{} = "s235"_s,
        "s235"_s + "event235"_t [([]{})] / []{} = "s236"_s,
        "s236"_s + "event236"_t [([]{})] / []{} = "s237"_s,
        "s237"_s + "event237"_t [([]{})] / []{} = "s238"_s,
        "s238"_s + "event238"_t [([]{})] / []{} = "s239"_s,
        "s239"_s + "event239"_t [([]{})] / []{} = "s240"_s,
        "s240"_s + "event240"_t [([]{})] / []{} = "s241"_s,
        "s241"_s + "event241"_t [([]{})] / []{} = "s242"_s,
        "s242"_s + "event242"_t [([]{})] / []{} = "s243"_s,
        "s243"_s + "event243"_t [([]{})] / []{} = "s244"_s,
        "s244"_s + "event244"_t [([]{})] / []{} = "s245"_s,
        "s245"_s + "event245"_t [([]{})] / []{} = "s246"_s,
        "s246"_s + "event246"_t [([]{})] / []{} = "s247"_s,
        "s247"_s + "event247"_t [([]{})] / []{} = "s248"_s,
        "s248"_s + "event248"_t [([]{})] / []{} = "s249"_s,
        "s249"_s + "event249"_t [([]{})] / []{} = "s250"_s,
        "s250"_s + "event250"_t [([]{})] / []{} = "s251"_s,
        "s251"_s + "event251"_t [([]{})] / []{} = "s252"_s,
        "s252"_s + "event252"_t [([]{})] / []{} = "s253"_s,
        "s253"_s + "event253"_t [([]{})] / []{} = "s254"_s,
        "s254"_s + "event254"_t [([]{})] / []{} = "s255"_s,
        "s255"_s + "event255"_t [([]{})] / []{} = "s256"_s,
        "s256"_s + "event256"_t [([]{})] / []{} = "s257"_s,
        "s257"_s + "event257"_t [([]{})] / []{} = "s258"_s
      );
      // clang-format on
    }
  };
  static_expect(4 /*deps=1, current_state=2, alignment = 1*/ == sizeof(msm::sm<c>));
};

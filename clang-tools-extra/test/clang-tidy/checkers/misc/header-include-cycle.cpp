// RUN: rm -rf %t.dir/misc-header-include-cycle-headers
// RUN: mkdir -p %t.dir/misc-header-include-cycle-headers
// RUN: cp -r %S/Inputs/header-include-cycle* %t.dir/misc-header-include-cycle-headers/
// RUN: mkdir %t.dir/misc-header-include-cycle-headers/system
// RUN: cp -r %S/Inputs/system/header-include-cycle* %t.dir/misc-header-include-cycle-headers/system
// RUN: cp %s %t.dir/header-include-cycle.cpp
// RUN: clang-tidy %t.dir%{fs-sep}header-include-cycle.cpp -checks='-*,misc-header-include-cycle' -header-filter=.* \
// RUN: -config="{CheckOptions: {misc-header-include-cycle.IgnoredFilesList: 'header-include-cycle.self-e.hpp'}}" \
// RUN: -- -I%t.dir%{fs-sep}misc-header-include-cycle-headers -isystem %t.dir%{fs-sep}misc-header-include-cycle-headers%{fs-sep}system \
// RUN: --include %t.dir%{fs-sep}misc-header-include-cycle-headers%{fs-sep}header-include-cycle.self-i.hpp | FileCheck %s \
// RUN: -check-prefix=CHECK-MESSAGES "-implicit-check-not={{note|warning|error}}:" --dump-input=fail
// RUN: rm -rf %t.dir/misc-header-include-cycle-headers

#ifndef MAIN_GUARD
#define MAIN_GUARD

#include "header-include-cycle.cpp"
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: direct self-inclusion of header file 'header-include-cycle.cpp' [misc-header-include-cycle]

#include <header-include-cycle.first-d.hpp>
// CHECK-MESSAGES: header-include-cycle.fourth-d.hpp:3:10: warning: circular header file dependency detected while including 'header-include-cycle.first-d.hpp', please check the include path [misc-header-include-cycle]
// CHECK-MESSAGES: header-include-cycle.third-d.hpp:3:10: note: 'header-include-cycle.fourth-d.hpp' included from here
// CHECK-MESSAGES: header-include-cycle.second-d.hpp:3:10: note: 'header-include-cycle.third-d.hpp' included from here
// CHECK-MESSAGES: header-include-cycle.first-d.hpp:3:10: note: 'header-include-cycle.second-d.hpp' included from here
// CHECK-MESSAGES: :[[@LINE-5]]:10: note: 'header-include-cycle.first-d.hpp' included from here

#include <header-include-cycle.first.hpp>
// CHECK-MESSAGES: header-include-cycle.fourth.hpp:2:10: warning: circular header file dependency detected while including 'header-include-cycle.first.hpp', please check the include path [misc-header-include-cycle]
// CHECK-MESSAGES: header-include-cycle.third.hpp:2:10: note: 'header-include-cycle.fourth.hpp' included from here
// CHECK-MESSAGES: header-include-cycle.second.hpp:2:10: note: 'header-include-cycle.third.hpp' included from here
// CHECK-MESSAGES: header-include-cycle.first.hpp:2:10: note: 'header-include-cycle.second.hpp' included from here
// CHECK-MESSAGES: :[[@LINE-5]]:10: note: 'header-include-cycle.first.hpp' included from here

#include <header-include-cycle.self-d.hpp>
// CHECK-MESSAGES: header-include-cycle.self-d.hpp:3:10: warning: direct self-inclusion of header file 'header-include-cycle.self-d.hpp' [misc-header-include-cycle]

// CHECK-MESSAGES: header-include-cycle.self-i.hpp:2:10: warning: direct self-inclusion of header file 'header-include-cycle.self-i.hpp' [misc-header-include-cycle]

#include <header-include-cycle.self-o.hpp>
// CHECK-MESSAGES: header-include-cycle.self-n.hpp:2:10: warning: direct self-inclusion of header file 'header-include-cycle.self-n.hpp' [misc-header-include-cycle]

#include <header-include-cycle.self.hpp>
// CHECK-MESSAGES: header-include-cycle.self.hpp:2:10: warning: direct self-inclusion of header file 'header-include-cycle.self.hpp' [misc-header-include-cycle]

// Should not warn about second include of guarded headers:
#include <header-include-cycle.first.hpp>
#include <header-include-cycle.first-d.hpp>
#include <header-include-cycle.self.hpp>
#include <header-include-cycle.self-d.hpp>
#include <header-include-cycle.self-o.hpp>
#include <header-include-cycle.self-n.hpp>

// Should not warn about system includes
#include <header-include-cycle.first-s.hpp>
#include <header-include-cycle.self-s.hpp>

// Should not warn about this excluded header
#include <header-include-cycle.self-e.hpp>
#endif

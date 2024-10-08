// RUN: rm -rf %t
// RUN: mkdir -p %t
// RUN: split-file %s %t

// RUN: %clang_cc1 -std=c++20 -emit-module-interface %t/std10-3-ex1-tu1.cpp \
// RUN:  -o %t/M_PartImpl.pcm

// RUN: %clang_cc1 -std=c++20 -emit-module-interface %t/std10-3-ex1-tu2.cpp \
// RUN:  -fmodule-file=M:PartImpl=%t/M_PartImpl.pcm -o %t/M.pcm -verify

// RUN: %clang_cc1 -std=c++20 -emit-module-interface %t/std10-3-ex1-tu3.cpp \
// RUN:  -o %t/M_Part.pcm

// RUN: %clang_cc1 -std=c++20 -emit-module-interface %t/std10-3-ex1-tu4.cpp \
// RUN:  -fmodule-file=M:Part=%t/M_Part.pcm -o %t/M.pcm

// Test again with reduced BMI.
// RUN: rm %t/M_PartImpl.pcm %t/M.pcm %t/M_Part.pcm
// RUN: %clang_cc1 -std=c++20 -emit-reduced-module-interface %t/std10-3-ex1-tu1.cpp \
// RUN:  -o %t/M_PartImpl.pcm

// RUN: %clang_cc1 -std=c++20 -emit-reduced-module-interface %t/std10-3-ex1-tu2.cpp \
// RUN:  -fmodule-file=M:PartImpl=%t/M_PartImpl.pcm -o %t/M.pcm -verify

// RUN: %clang_cc1 -std=c++20 -emit-reduced-module-interface %t/std10-3-ex1-tu3.cpp \
// RUN:  -o %t/M_Part.pcm

// RUN: %clang_cc1 -std=c++20 -emit-reduced-module-interface %t/std10-3-ex1-tu4.cpp \
// RUN:  -fmodule-file=M:Part=%t/M_Part.pcm -o %t/M.pcm

//--- std10-3-ex1-tu1.cpp
module M:PartImpl;

// expected-no-diagnostics

//--- std10-3-ex1-tu2.cpp
export module M;
                     // error: exported partition :Part is an implementation unit
export import :PartImpl; // expected-error {{module partition implementations cannot be exported}}
                         // expected-warning@-1 {{importing an implementation partition unit in a module interface is not recommended.}}

//--- std10-3-ex1-tu3.cpp
export module M:Part;

// expected-no-diagnostics

//--- std10-3-ex1-tu4.cpp
export module M;
export import :Part;

// expected-no-diagnostics

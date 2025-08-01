// RUN: %clang_min_runtime   -fsanitize=pointer-overflow %s -o %t && %run %t 2>&1 | FileCheck %s --implicit-check-not="pointer-overflow"
// RUN: %clangxx_min_runtime -x c++ -fsanitize=pointer-overflow %s -o %t && %run %t 2>&1 | FileCheck %s --implicit-check-not="pointer-overflow"

#include <stdlib.h>

int main(int argc, char *argv[]) {
  char *base, *result;

  base = (char *)0;
  result = base + 0;
  // CHECK-NOT: pointer-overflow

  base = (char *)0;
  result = base + 1;
  // CHECK: pointer-overflow by 0x{{[[:xdigit:]]+$}}

  base = (char *)1;
  result = base - 1;
  // CHECK: pointer-overflow by 0x{{[[:xdigit:]]+$}}

  return 0;
}

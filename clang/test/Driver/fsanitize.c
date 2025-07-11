// * Test -fsanitize-trap */

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-TRAP
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-trap=signed-integer-overflow %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-TRAP2
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-undefined-trap-on-error %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-TRAP
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined-trap -fsanitize-undefined-trap-on-error %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-TRAP
// RUN: %clang --target=x86_64-linux-gnu -fsanitize-undefined-trap-on-error -fsanitize=undefined-trap %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-TRAP
// RUN: %clang --target=x86_64-linux-gnu -fsanitize-trap -fsanitize=undefined-trap %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-TRAP
// CHECK-UNDEFINED-TRAP-NOT: -fsanitize-recover
// CHECK-UNDEFINED-TRAP: "-fsanitize={{((signed-integer-overflow|integer-divide-by-zero|shift-base|shift-exponent|unreachable|return|vla-bound|alignment|null|pointer-overflow|float-cast-overflow|array-bounds|enum|bool|builtin|returns-nonnull-attribute|nonnull-attribute|function),?){18}"}}
// CHECK-UNDEFINED-TRAP: "-fsanitize-trap=alignment,array-bounds,bool,builtin,enum,float-cast-overflow,function,integer-divide-by-zero,nonnull-attribute,null,pointer-overflow,return,returns-nonnull-attribute,shift-base,shift-exponent,signed-integer-overflow,unreachable,vla-bound"
// CHECK-UNDEFINED-TRAP2: "-fsanitize-trap=alignment,array-bounds,bool,builtin,enum,float-cast-overflow,function,integer-divide-by-zero,nonnull-attribute,null,pointer-overflow,return,returns-nonnull-attribute,shift-base,shift-exponent,unreachable,vla-bound"


// * Test -fsanitize-merge *

// The trailing -fsanitize-merge takes precedence
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                                                                              %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                             -fsanitize-merge                                 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                             -fsanitize-merge=undefined                       %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-merge                         -fsanitize-merge                                 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-merge                         -fsanitize-merge=undefined                       %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-merge=undefined               -fsanitize-merge                                 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-merge=undefined               -fsanitize-merge=undefined                       %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-merge=signed-integer-overflow -fsanitize-merge                                 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fsanitize-merge=bool                       -fsanitize-merge=undefined                       %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                             -fsanitize-merge=undefined -fsanitize-merge=bool %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE
// CHECK-UNDEFINED-MERGE: "-fsanitize-merge=alignment,array-bounds,bool,builtin,enum,float-cast-overflow,function,integer-divide-by-zero,nonnull-attribute,null,pointer-overflow,return,returns-nonnull-attribute,shift-base,shift-exponent,signed-integer-overflow,unreachable,vla-bound"

// The trailing arguments (-fsanitize-merge -fno-sanitize-merge=signed-integer-overflow) take precedence
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                                                        -fno-sanitize-merge=signed-integer-overflow %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE2
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                             -fsanitize-merge           -fno-sanitize-merge=signed-integer-overflow %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE2
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                             -fsanitize-merge=undefined -fno-sanitize-merge=signed-integer-overflow %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE2
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-merge                         -fsanitize-merge           -fno-sanitize-merge=signed-integer-overflow %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE2
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-merge                         -fsanitize-merge=undefined -fno-sanitize-merge=signed-integer-overflow %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE2
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-merge=signed-integer-overflow -fsanitize-merge           -fno-sanitize-merge=signed-integer-overflow %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE2
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-merge=signed-integer-overflow -fsanitize-merge=undefined -fno-sanitize-merge=signed-integer-overflow %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE2
// CHECK-UNDEFINED-MERGE2: "-fsanitize-merge=alignment,array-bounds,bool,builtin,enum,float-cast-overflow,function,integer-divide-by-zero,nonnull-attribute,null,pointer-overflow,return,returns-nonnull-attribute,shift-base,shift-exponent,unreachable,vla-bound"

// The trailing -fno-sanitize-merge takes precedence
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                            -fno-sanitize-merge                                    %s -### 2>&1 | not FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE3
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                            -fno-sanitize-merge=undefined                          %s -### 2>&1 | not FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE3
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                            -fno-sanitize-merge           -fno-sanitize-merge=bool %s -### 2>&1 | not FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE3
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                            -fno-sanitize-merge=undefined -fno-sanitize-merge=bool %s -### 2>&1 | not FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE3
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fsanitize-merge           -fno-sanitize-merge                                    %s -### 2>&1 | not FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE3
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fsanitize-merge           -fno-sanitize-merge=undefined                          %s -### 2>&1 | not FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE3
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fsanitize-merge=undefined -fno-sanitize-merge                                    %s -### 2>&1 | not FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE3
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fsanitize-merge=undefined -fno-sanitize-merge=undefined                          %s -### 2>&1 | not FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE3
// CHECK-UNDEFINED-MERGE3: "-fsanitize-merge"

// The trailing arguments (-fsanitize-merge -fno-sanitize-merge=alignment,null) take precedence
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                                                        -fno-sanitize-merge=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE4
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                             -fsanitize-merge           -fno-sanitize-merge=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE4
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                             -fsanitize-merge=undefined -fno-sanitize-merge=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE4
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-merge                         -fsanitize-merge           -fno-sanitize-merge=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE4
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-merge                         -fsanitize-merge=undefined -fno-sanitize-merge=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE4
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-merge=signed-integer-overflow -fsanitize-merge           -fno-sanitize-merge=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE4
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-merge=signed-integer-overflow -fsanitize-merge=undefined -fno-sanitize-merge=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE4
// CHECK-UNDEFINED-MERGE4: "-fsanitize-merge=array-bounds,bool,builtin,enum,float-cast-overflow,function,integer-divide-by-zero,nonnull-attribute,pointer-overflow,return,returns-nonnull-attribute,shift-base,shift-exponent,signed-integer-overflow,unreachable,vla-bound"

// The trailing arguments (-fno-sanitize-merge -fsanitize-merge=alignment,null) take precedence
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                          -fno-sanitize-merge           -fsanitize-merge=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE5
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                          -fno-sanitize-merge=undefined -fsanitize-merge=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE5
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fsanitize-merge                         -fno-sanitize-merge           -fsanitize-merge=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE5
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fsanitize-merge                         -fno-sanitize-merge=undefined -fsanitize-merge=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE5
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fsanitize-merge=signed-integer-overflow -fno-sanitize-merge           -fsanitize-merge=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE5
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fsanitize-merge=signed-integer-overflow -fno-sanitize-merge=undefined -fsanitize-merge=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-MERGE5
// CHECK-UNDEFINED-MERGE5: "-fsanitize-merge=alignment,null"


// * Test -fsanitize-annotate-debug-info *

// The trailing -fsanitize-annotate-debug-info takes precedence
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                                            -fsanitize-annotate-debug-info                                 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                                            -fsanitize-annotate-debug-info=undefined                       %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-annotate-debug-info                         -fsanitize-annotate-debug-info                                 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-annotate-debug-info                         -fsanitize-annotate-debug-info=undefined                       %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-annotate-debug-info=undefined               -fsanitize-annotate-debug-info                                 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-annotate-debug-info=undefined               -fsanitize-annotate-debug-info=undefined                       %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-annotate-debug-info=signed-integer-overflow -fsanitize-annotate-debug-info                                 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fsanitize-annotate-debug-info=bool                       -fsanitize-annotate-debug-info=undefined                       %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                                            -fsanitize-annotate-debug-info=undefined -fsanitize-annotate-debug-info=bool %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO
// CHECK-UNDEFINED-PSEUDO: "-fsanitize-annotate-debug-info=alignment,array-bounds,bool,builtin,enum,float-cast-overflow,function,integer-divide-by-zero,nonnull-attribute,null,pointer-overflow,return,returns-nonnull-attribute,shift-base,shift-exponent,signed-integer-overflow,unreachable,vla-bound"

// The trailing arguments (-fsanitize-annotate-debug-info -fno-sanitize-annotate-debug-info=signed-integer-overflow) take precedence
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                                            -fsanitize-annotate-debug-info           -fno-sanitize-annotate-debug-info=signed-integer-overflow %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO2
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                                            -fsanitize-annotate-debug-info=undefined -fno-sanitize-annotate-debug-info=signed-integer-overflow %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO2
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-annotate-debug-info                         -fsanitize-annotate-debug-info           -fno-sanitize-annotate-debug-info=signed-integer-overflow %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO2
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-annotate-debug-info                         -fsanitize-annotate-debug-info=undefined -fno-sanitize-annotate-debug-info=signed-integer-overflow %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO2
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-annotate-debug-info=signed-integer-overflow -fsanitize-annotate-debug-info           -fno-sanitize-annotate-debug-info=signed-integer-overflow %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO2
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-annotate-debug-info=signed-integer-overflow -fsanitize-annotate-debug-info=undefined -fno-sanitize-annotate-debug-info=signed-integer-overflow %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO2
// CHECK-UNDEFINED-PSEUDO2: "-fsanitize-annotate-debug-info=alignment,array-bounds,bool,builtin,enum,float-cast-overflow,function,integer-divide-by-zero,nonnull-attribute,null,pointer-overflow,return,returns-nonnull-attribute,shift-base,shift-exponent,unreachable,vla-bound"

// The trailing -fno-sanitize-annotate-debug-info takes precedence
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                           -fno-sanitize-annotate-debug-info                                    %s -### 2>&1 | not FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO3
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                           -fno-sanitize-annotate-debug-info=undefined                          %s -### 2>&1 | not FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO3
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                           -fno-sanitize-annotate-debug-info           -fno-sanitize-annotate-debug-info=bool %s -### 2>&1 | not FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO3
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                           -fno-sanitize-annotate-debug-info=undefined -fno-sanitize-annotate-debug-info=bool %s -### 2>&1 | not FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO3
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fsanitize-annotate-debug-info           -fno-sanitize-annotate-debug-info                                    %s -### 2>&1 | not FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO3
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fsanitize-annotate-debug-info           -fno-sanitize-annotate-debug-info=undefined                          %s -### 2>&1 | not FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO3
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fsanitize-annotate-debug-info=undefined -fno-sanitize-annotate-debug-info                                    %s -### 2>&1 | not FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO3
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fsanitize-annotate-debug-info=undefined -fno-sanitize-annotate-debug-info=undefined                          %s -### 2>&1 | not FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO3
// CHECK-UNDEFINED-PSEUDO3: "-fsanitize-annotate-debug-info"

// The trailing arguments (-fsanitize-annotate-debug-info -fno-sanitize-annotate-debug-info=alignment,null) take precedence
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                                            -fsanitize-annotate-debug-info           -fno-sanitize-annotate-debug-info=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO4
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                                            -fsanitize-annotate-debug-info=undefined -fno-sanitize-annotate-debug-info=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO4
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-annotate-debug-info                         -fsanitize-annotate-debug-info           -fno-sanitize-annotate-debug-info=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO4
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-annotate-debug-info                         -fsanitize-annotate-debug-info=undefined -fno-sanitize-annotate-debug-info=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO4
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-annotate-debug-info=signed-integer-overflow -fsanitize-annotate-debug-info           -fno-sanitize-annotate-debug-info=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO4
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fno-sanitize-annotate-debug-info=signed-integer-overflow -fsanitize-annotate-debug-info=undefined -fno-sanitize-annotate-debug-info=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO4
// CHECK-UNDEFINED-PSEUDO4: "-fsanitize-annotate-debug-info=array-bounds,bool,builtin,enum,float-cast-overflow,function,integer-divide-by-zero,nonnull-attribute,pointer-overflow,return,returns-nonnull-attribute,shift-base,shift-exponent,signed-integer-overflow,unreachable,vla-bound"

// The trailing arguments (-fno-sanitize-annotate-debug-info -fsanitize-annotate-debug-info=alignment,null) take precedence
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined                                                         -fno-sanitize-annotate-debug-info=undefined -fsanitize-annotate-debug-info=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO5
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fsanitize-annotate-debug-info                         -fno-sanitize-annotate-debug-info           -fsanitize-annotate-debug-info=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO5
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fsanitize-annotate-debug-info                         -fno-sanitize-annotate-debug-info=undefined -fsanitize-annotate-debug-info=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO5
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fsanitize-annotate-debug-info=signed-integer-overflow -fno-sanitize-annotate-debug-info           -fsanitize-annotate-debug-info=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO5
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-trap=undefined -fsanitize-annotate-debug-info=signed-integer-overflow -fno-sanitize-annotate-debug-info=undefined -fsanitize-annotate-debug-info=alignment,null %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-PSEUDO5
// CHECK-UNDEFINED-PSEUDO5: "-fsanitize-annotate-debug-info=alignment,null"


// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED
// CHECK-UNDEFINED: "-fsanitize={{((signed-integer-overflow|integer-divide-by-zero|function|shift-base|shift-exponent|unreachable|return|vla-bound|alignment|null|pointer-overflow|float-cast-overflow|array-bounds|enum|bool|builtin|returns-nonnull-attribute|nonnull-attribute),?){18}"}}

// RUN: %clang --target=x86_64-apple-darwin10 -fsanitize=undefined %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-DARWIN
// CHECK-UNDEFINED-DARWIN: "-fsanitize={{((signed-integer-overflow|integer-divide-by-zero|function|shift-base|shift-exponent|unreachable|return|vla-bound|alignment|null|pointer-overflow|float-cast-overflow|array-bounds|enum|bool|builtin|returns-nonnull-attribute|nonnull-attribute),?){18}"}}

// RUN: %clang --target=i386-pc-win32 -fsanitize=undefined %s -### 2>&1 | FileCheck %s --check-prefixes=CHECK-UNDEFINED-WIN32,CHECK-UNDEFINED-MSVC
// RUN: %clang --target=i386-pc-win32 -fsanitize=undefined -x c++ %s -### 2>&1 | FileCheck %s --check-prefixes=CHECK-UNDEFINED-WIN32,CHECK-UNDEFINED-WIN-CXX,CHECK-UNDEFINED-MSVC
// RUN: %clang --target=x86_64-pc-win32 -fsanitize=undefined %s -### 2>&1 | FileCheck %s --check-prefixes=CHECK-UNDEFINED-WIN64,CHECK-UNDEFINED-MSVC
// RUN: %clang --target=x86_64-w64-mingw32 -fsanitize=undefined %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-WIN64-MINGW
// RUN: %clang --target=x86_64-pc-win32 -fsanitize=undefined -x c++ %s -### 2>&1 | FileCheck %s --check-prefixes=CHECK-UNDEFINED-WIN64,CHECK-UNDEFINED-WIN-CXX,CHECK-UNDEFINED-MSVC
// CHECK-UNDEFINED-WIN32: "--dependent-lib={{[^"]*}}ubsan_standalone{{(-i386)?}}.lib"
// CHECK-UNDEFINED-WIN64: "--dependent-lib={{[^"]*}}ubsan_standalone{{(-x86_64)?}}.lib"
// CHECK-UNDEFINED-WIN64-MINGW: "--dependent-lib={{[^"]*}}libclang_rt.ubsan_standalone{{(-x86_64)?}}.a"
// CHECK-UNDEFINED-WIN-CXX: "--dependent-lib={{[^"]*}}ubsan_standalone_cxx{{[^"]*}}.lib"
// CHECK-UNDEFINED-MSVC-SAME: "-fsanitize={{((signed-integer-overflow|integer-divide-by-zero|shift-base|shift-exponent|unreachable|return|vla-bound|alignment|null|pointer-overflow|float-cast-overflow|array-bounds|enum|bool|builtin|returns-nonnull-attribute|nonnull-attribute|function),?){18}"}}
// CHECK-UNDEFINED-WIN64-MINGW-SAME: "-fsanitize={{((signed-integer-overflow|integer-divide-by-zero|shift-base|shift-exponent|unreachable|return|vla-bound|alignment|null|pointer-overflow|float-cast-overflow|array-bounds|enum|bool|builtin|returns-nonnull-attribute|nonnull-attribute|function),?){18}"}}

// RUN: %clang --target=i386-pc-win32 -fsanitize-coverage=bb %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-COVERAGE-WIN32
// CHECK-COVERAGE-WIN32: "--dependent-lib={{[^"]*}}ubsan_standalone{{(-i386)?}}.lib"
// RUN: %clang --target=x86_64-pc-win32 -fsanitize-coverage=bb %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-COVERAGE-WIN64
// CHECK-COVERAGE-WIN64: "--dependent-lib={{[^"]*}}ubsan_standalone{{(-x86_64)?}}.lib"

// RUN: %clang --target=%itanium_abi_triple -fsanitize=integer %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-INTEGER -implicit-check-not="-fsanitize-address-use-after-scope"
// CHECK-INTEGER: "-fsanitize={{((signed-integer-overflow|unsigned-integer-overflow|integer-divide-by-zero|shift-base|shift-exponent|implicit-unsigned-integer-truncation|implicit-signed-integer-truncation|implicit-integer-sign-change|unsigned-shift-base),?){9}"}}

// RUN: %clang -fsanitize=implicit-integer-conversion %s -### 2>&1 | FileCheck %s --check-prefixes=CHECK-implicit-integer-conversion,CHECK-implicit-integer-conversion-RECOVER
// RUN: %clang -fsanitize=implicit-integer-conversion -fsanitize-recover=implicit-integer-conversion %s -### 2>&1 | FileCheck %s --check-prefixes=CHECK-implicit-integer-conversion,CHECK-implicit-integer-conversion-RECOVER
// RUN: %clang -fsanitize=implicit-integer-conversion -fno-sanitize-recover=implicit-integer-conversion %s -### 2>&1 | FileCheck %s --check-prefixes=CHECK-implicit-integer-conversion,CHECK-implicit-integer-conversion-NORECOVER
// RUN: %clang -fsanitize=implicit-integer-conversion -fsanitize-trap=implicit-integer-conversion %s -### 2>&1 | FileCheck %s --check-prefixes=CHECK-implicit-integer-conversion,CHECK-implicit-integer-conversion-TRAP
// CHECK-implicit-integer-conversion: "-fsanitize={{((implicit-unsigned-integer-truncation|implicit-signed-integer-truncation|implicit-integer-sign-change),?){3}"}}
// CHECK-implicit-integer-conversion-RECOVER: "-fsanitize-recover={{((implicit-unsigned-integer-truncation|implicit-signed-integer-truncation|implicit-integer-sign-change),?){3}"}}
// CHECK-implicit-integer-conversion-RECOVER-NOT: "-fno-sanitize-recover={{((implicit-unsigned-integer-truncation|implicit-signed-integer-truncation|implicit-integer-sign-change),?){3}"}}
// CHECK-implicit-integer-conversion-RECOVER-NOT: "-fsanitize-trap={{((implicit-unsigned-integer-truncation|implicit-signed-integer-truncation|implicit-integer-sign-change),?){3}"}}
// CHECK-implicit-integer-conversion-NORECOVER-NOT: "-fno-sanitize-recover={{((implicit-unsigned-integer-truncation|implicit-signed-integer-truncation|implicit-integer-sign-change),?){3}"}} // ???
// CHECK-implicit-integer-conversion-NORECOVER-NOT: "-fsanitize-recover={{((implicit-unsigned-integer-truncation|implicit-signed-integer-truncation|implicit-integer-sign-change),?){3}"}}
// CHECK-implicit-integer-conversion-NORECOVER-NOT: "-fsanitize-trap={{((implicit-unsigned-integer-truncation|implicit-signed-integer-truncation|implicit-integer-sign-change),?){3}"}}
// CHECK-implicit-integer-conversion-TRAP: "-fsanitize-trap={{((implicit-unsigned-integer-truncation|implicit-signed-integer-truncation|implicit-integer-sign-change),?){3}"}}
// CHECK-implicit-integer-conversion-TRAP-NOT: "-fsanitize-recover={{((implicit-unsigned-integer-truncation|implicit-signed-integer-truncation|implicit-integer-sign-change),?){3}"}}
// CHECK-implicit-integer-conversion-TRAP-NOT: "-fno-sanitize-recover={{((implicit-unsigned-integer-truncation|implicit-signed-integer-truncation|implicit-integer-sign-change),?){3}"}}

// RUN: %clang -fsanitize=implicit-integer-arithmetic-value-change %s -### 2>&1 | FileCheck %s --check-prefixes=CHECK-implicit-integer-arithmetic-value-change,CHECK-implicit-integer-arithmetic-value-change-RECOVER
// RUN: %clang -fsanitize=implicit-integer-arithmetic-value-change -fsanitize-recover=implicit-integer-arithmetic-value-change %s -### 2>&1 | FileCheck %s --check-prefixes=CHECK-implicit-integer-arithmetic-value-change,CHECK-implicit-integer-arithmetic-value-change-RECOVER
// RUN: %clang -fsanitize=implicit-integer-arithmetic-value-change -fno-sanitize-recover=implicit-integer-arithmetic-value-change %s -### 2>&1 | FileCheck %s --check-prefixes=CHECK-implicit-integer-arithmetic-value-change,CHECK-implicit-integer-arithmetic-value-change-NORECOVER
// RUN: %clang -fsanitize=implicit-integer-arithmetic-value-change -fsanitize-trap=implicit-integer-arithmetic-value-change %s -### 2>&1 | FileCheck %s --check-prefixes=CHECK-implicit-integer-arithmetic-value-change,CHECK-implicit-integer-arithmetic-value-change-TRAP
// CHECK-implicit-integer-arithmetic-value-change: "-fsanitize={{((implicit-signed-integer-truncation|implicit-integer-sign-change),?){2}"}}
// CHECK-implicit-integer-arithmetic-value-change-RECOVER: "-fsanitize-recover={{((implicit-signed-integer-truncation|implicit-integer-sign-change),?){2}"}}
// CHECK-implicit-integer-arithmetic-value-change-RECOVER-NOT: "-fno-sanitize-recover={{((implicit-signed-integer-truncation|implicit-integer-sign-change),?){2}"}}
// CHECK-implicit-integer-arithmetic-value-change-RECOVER-NOT: "-fsanitize-trap={{((implicit-signed-integer-truncation|implicit-integer-sign-change),?){2}"}}
// CHECK-implicit-integer-arithmetic-value-change-NORECOVER-NOT: "-fno-sanitize-recover={{((implicit-signed-integer-truncation|implicit-integer-sign-change),?){2}"}} // ???
// CHECK-implicit-integer-arithmetic-value-change-NORECOVER-NOT: "-fsanitize-recover={{((implicit-signed-integer-truncation|implicit-integer-sign-change),?){2}"}}
// CHECK-implicit-integer-arithmetic-value-change-NORECOVER-NOT: "-fsanitize-trap={{((implicit-signed-integer-truncation|implicit-integer-sign-change),?){2}"}}
// CHECK-implicit-integer-arithmetic-value-change-TRAP: "-fsanitize-trap={{((implicit-signed-integer-truncation|implicit-integer-sign-change),?){2}"}}
// CHECK-implicit-integer-arithmetic-value-change-TRAP-NOT: "-fsanitize-recover={{((implicit-signed-integer-truncation|implicit-integer-sign-change),?){2}"}}
// CHECK-implicit-integer-arithmetic-value-change-TRAP-NOT: "-fno-sanitize-recover={{((implicit-signed-integer-truncation|implicit-integer-sign-change),?){2}"}}

// RUN: %clang -fsanitize=implicit-integer-truncation %s -### 2>&1 | FileCheck %s --check-prefixes=CHECK-implicit-integer-truncation,CHECK-implicit-integer-truncation-RECOVER
// RUN: %clang -fsanitize=implicit-integer-truncation -fsanitize-recover=implicit-integer-truncation %s -### 2>&1 | FileCheck %s --check-prefixes=CHECK-implicit-integer-truncation,CHECK-implicit-integer-truncation-RECOVER
// RUN: %clang -fsanitize=implicit-integer-truncation -fno-sanitize-recover=implicit-integer-truncation %s -### 2>&1 | FileCheck %s --check-prefixes=CHECK-implicit-integer-truncation,CHECK-implicit-integer-truncation-NORECOVER
// RUN: %clang -fsanitize=implicit-integer-truncation -fsanitize-trap=implicit-integer-truncation %s -### 2>&1 | FileCheck %s --check-prefixes=CHECK-implicit-integer-truncation,CHECK-implicit-integer-truncation-TRAP
// CHECK-implicit-integer-truncation: "-fsanitize={{((implicit-unsigned-integer-truncation|implicit-signed-integer-truncation),?){2}"}}
// CHECK-implicit-integer-truncation-RECOVER: "-fsanitize-recover={{((implicit-unsigned-integer-truncation|implicit-signed-integer-truncation),?){2}"}}
// CHECK-implicit-integer-truncation-RECOVER-NOT: "-fno-sanitize-recover={{((implicit-unsigned-integer-truncation|implicit-signed-integer-truncation),?){2}"}}
// CHECK-implicit-integer-truncation-RECOVER-NOT: "-fsanitize-trap={{((implicit-unsigned-integer-truncation|implicit-signed-integer-truncation),?){2}"}}
// CHECK-implicit-integer-truncation-NORECOVER-NOT: "-fno-sanitize-recover={{((implicit-unsigned-integer-truncation|implicit-signed-integer-truncation),?){2}"}} // ???
// CHECK-implicit-integer-truncation-NORECOVER-NOT: "-fsanitize-recover={{((implicit-unsigned-integer-truncation|implicit-signed-integer-truncation),?){2}"}}
// CHECK-implicit-integer-truncation-NORECOVER-NOT: "-fsanitize-trap={{((implicit-unsigned-integer-truncation|implicit-signed-integer-truncation),?){2}"}}
// CHECK-implicit-integer-truncation-TRAP: "-fsanitize-trap={{((implicit-unsigned-integer-truncation|implicit-signed-integer-truncation),?){2}"}}
// CHECK-implicit-integer-truncation-TRAP-NOT: "-fsanitize-recover={{((implicit-unsigned-integer-truncation|implicit-signed-integer-truncation),?){2}"}}
// CHECK-implicit-integer-truncation-TRAP-NOT: "-fno-sanitize-recover={{((implicit-unsigned-integer-truncation|implicit-signed-integer-truncation),?){2}"}}

// RUN: %clang -fsanitize=bounds -### -fsyntax-only %s 2>&1 | FileCheck %s --check-prefix=CHECK-BOUNDS
// CHECK-BOUNDS: "-fsanitize={{((array-bounds|local-bounds),?){2}"}}

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=all %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-FSANITIZE-ALL
// CHECK-FSANITIZE-ALL: error: unsupported argument 'all' to option '-fsanitize='

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address,undefined -fno-sanitize=all -fsanitize=thread %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-FNO-SANITIZE-ALL
// CHECK-FNO-SANITIZE-ALL: "-fsanitize=thread"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=thread,undefined -fno-sanitize=thread -fno-sanitize=float-cast-overflow,vptr,bool,builtin,enum %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-PARTIAL-UNDEFINED
// CHECK-PARTIAL-UNDEFINED: "-fsanitize={{((signed-integer-overflow|integer-divide-by-zero|function|shift-base|shift-exponent|unreachable|return|vla-bound|alignment|null|pointer-overflow|array-bounds|returns-nonnull-attribute|nonnull-attribute),?){14}"}}

// RUN: %clang -fsanitize=shift -fno-sanitize=shift-base %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-FSANITIZE-SHIFT-PARTIAL
// CHECK-FSANITIZE-SHIFT-PARTIAL: "-fsanitize=shift-exponent"

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=vptr -fsanitize-trap=vptr %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-VPTR-TRAP-UNDEF
// CHECK-VPTR-TRAP-UNDEF: error: invalid argument '-fsanitize=vptr' not allowed with '-fsanitize-trap=undefined'

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=vptr -fsanitize-undefined-trap-on-error %s -###

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=vptr -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-VPTR-NO-RTTI
// CHECK-VPTR-NO-RTTI: '-fsanitize=vptr' not allowed with '-fno-rtti'

// RUN: %clang -fsanitize=undefined -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UNDEFINED-NO-RTTI
// CHECK-UNDEFINED-NO-RTTI-NOT: vptr

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=address,thread -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANA-SANT
// CHECK-SANA-SANT: '-fsanitize=address' not allowed with '-fsanitize=thread'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=address,memory -pie -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANA-SANM
// CHECK-SANA-SANM: '-fsanitize=address' not allowed with '-fsanitize=memory'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=thread,memory -pie -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANT-SANM
// CHECK-SANT-SANM: '-fsanitize=thread' not allowed with '-fsanitize=memory'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=memory,thread -pie -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANM-SANT
// CHECK-SANM-SANT: '-fsanitize=thread' not allowed with '-fsanitize=memory'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=leak,thread -pie -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANL-SANT
// CHECK-SANL-SANT: '-fsanitize=leak' not allowed with '-fsanitize=thread'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=leak,memory -pie -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANL-SANM
// CHECK-SANL-SANM: '-fsanitize=leak' not allowed with '-fsanitize=memory'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=kernel-memory,address -pie -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-KMSAN-ASAN
// CHECK-KMSAN-ASAN: '-fsanitize=kernel-memory' not allowed with '-fsanitize=address'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=kernel-memory,hwaddress -pie -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-KMSAN-HWASAN
// CHECK-KMSAN-HWASAN: '-fsanitize=kernel-memory' not allowed with '-fsanitize=hwaddress'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=kernel-memory,leak -pie -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-KMSAN-LSAN
// CHECK-KMSAN-LSAN: '-fsanitize=kernel-memory' not allowed with '-fsanitize=leak'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=kernel-memory,thread -pie -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-KMSAN-TSAN
// CHECK-KMSAN-TSAN: '-fsanitize=kernel-memory' not allowed with '-fsanitize=thread'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=kernel-memory,kernel-address -pie -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-KMSAN-KASAN
// CHECK-KMSAN-KASAN: '-fsanitize=kernel-memory' not allowed with '-fsanitize=kernel-address'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=kernel-memory,memory -pie -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-KMSAN-MSAN
// CHECK-KMSAN-MSAN: '-fsanitize=kernel-memory' not allowed with '-fsanitize=memory'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=kernel-memory,safe-stack -pie -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-KMSAN-SAFESTACK
// CHECK-KMSAN-SAFESTACK: '-fsanitize=kernel-memory' not allowed with '-fsanitize=safe-stack'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=kernel-address,thread -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANKA-SANT
// CHECK-SANKA-SANT: '-fsanitize=kernel-address' not allowed with '-fsanitize=thread'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=kernel-address,memory -pie -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANKA-SANM
// CHECK-SANKA-SANM: '-fsanitize=kernel-address' not allowed with '-fsanitize=memory'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=kernel-address,address -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANKA-SANA
// CHECK-SANKA-SANA: '-fsanitize=kernel-address' not allowed with '-fsanitize=address'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=kernel-address,leak -pie -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANKA-SANL
// CHECK-SANKA-SANL: '-fsanitize=kernel-address' not allowed with '-fsanitize=leak'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=kernel-hwaddress,thread -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANKHA-SANT
// CHECK-SANKHA-SANT: '-fsanitize=kernel-hwaddress' not allowed with '-fsanitize=thread'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=kernel-hwaddress,memory -pie -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANKHA-SANM
// CHECK-SANKHA-SANM: '-fsanitize=kernel-hwaddress' not allowed with '-fsanitize=memory'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=kernel-hwaddress,address -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANKHA-SANA
// CHECK-SANKHA-SANA: '-fsanitize=kernel-hwaddress' not allowed with '-fsanitize=address'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=kernel-hwaddress,leak -pie -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANKHA-SANL
// CHECK-SANKHA-SANL: '-fsanitize=kernel-hwaddress' not allowed with '-fsanitize=leak'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=kernel-hwaddress,hwaddress -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANKHA-SANHA
// CHECK-SANKHA-SANHA: '-fsanitize=kernel-hwaddress' not allowed with '-fsanitize=hwaddress'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=kernel-hwaddress,kernel-address -pie -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANKHA-SANKA
// CHECK-SANKHA-SANKA: '-fsanitize=kernel-hwaddress' not allowed with '-fsanitize=kernel-address'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=hwaddress,thread -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANHA-SANT
// CHECK-SANHA-SANT: '-fsanitize=hwaddress' not allowed with '-fsanitize=thread'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=hwaddress,memory -pie -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANHA-SANM
// CHECK-SANHA-SANM: '-fsanitize=hwaddress' not allowed with '-fsanitize=memory'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=hwaddress,address -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANHA-SANA
// CHECK-SANHA-SANA: '-fsanitize=hwaddress' not allowed with '-fsanitize=address'

// RUN: not %clang --target=aarch64-linux-android -fsanitize=memtag,address -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANMT-SANA
// CHECK-SANMT-SANA: '-fsanitize=memtag' not allowed with '-fsanitize=address'

// RUN: not %clang --target=aarch64-linux-android -fsanitize=memtag,hwaddress -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANMT-SANHA
// CHECK-SANMT-SANHA: '-fsanitize=memtag' not allowed with '-fsanitize=hwaddress'

// RUN: not %clang --target=i386-linux-android -fsanitize=memtag -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANMT-BAD-ARCH
// RUN: not %clang --target=x86_64-linux-android -fsanitize=memtag -fno-rtti %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANMT-BAD-ARCH
// CHECK-SANMT-BAD-ARCH: unsupported option '-fsanitize=memtag' for target

// RUN: %clang --target=aarch64-linux-android31 -fsanitize=memtag -march=armv8-a+memtag %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANMT-MT
// CHECK-SANMT-MT: "-target-feature" "+mte"
// CHECK-SANMT-MT-SAME: "-fsanitize=memtag-stack,memtag-heap,memtag-globals"

// RUN: not %clang --target=aarch64-linux -fsanitize=memtag -Xclang -target-feature -Xclang +mte %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANMT-MT

// RUN: not %clang --target=aarch64-linux -fsanitize=memtag %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANMT-NOMT-0
// CHECK-SANMT-NOMT-0: '-fsanitize=memtag-stack' requires hardware support (+memtag)

// RUN: not %clang --target=aarch64-linux -fsanitize=memtag -I +mte %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANMT-NOMT-1
// CHECK-SANMT-NOMT-1: '-fsanitize=memtag-stack' requires hardware support (+memtag)

// RUN: not %clang --target=aarch64-linux-android31 -fsanitize-trap=memtag -march=armv8-a+memtag -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANMT-TRAP
// CHECK-SANMT-TRAP: error: unsupported argument 'memtag' to option '-fsanitize-trap='

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address -fsanitize-address-use-after-scope %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-USE-AFTER-SCOPE
// RUN: %clang_cl --target=x86_64-windows -fsanitize=address -fsanitize-address-use-after-scope -### -- %s 2>&1 | FileCheck %s --check-prefix=CHECK-USE-AFTER-SCOPE
// CHECK-USE-AFTER-SCOPE: -cc1{{.*}}-fsanitize-address-use-after-scope

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address -fno-sanitize-address-use-after-scope %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-USE-AFTER-SCOPE-OFF
// RUN: %clang_cl --target=x86_64-windows -fsanitize=address -fno-sanitize-address-use-after-scope -### -- %s 2>&1 | FileCheck %s --check-prefix=CHECK-USE-AFTER-SCOPE-OFF
// CHECK-USE-AFTER-SCOPE-OFF-NOT: -cc1{{.*}}address-use-after-scope

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address -fno-sanitize-address-use-after-scope -fsanitize-address-use-after-scope %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-USE-AFTER-SCOPE-BOTH
// RUN: %clang_cl --target=x86_64-windows -fsanitize=address -fno-sanitize-address-use-after-scope -fsanitize-address-use-after-scope -### -- %s 2>&1 | FileCheck %s --check-prefix=CHECK-USE-AFTER-SCOPE-BOTH
// CHECK-USE-AFTER-SCOPE-BOTH: -cc1{{.*}}-fsanitize-address-use-after-scope

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address -fsanitize-address-use-after-scope -fno-sanitize-address-use-after-scope %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-USE-AFTER-SCOPE-BOTH-OFF
// CHECK-USE-AFTER-SCOPE-BOTH-OFF-NOT: -cc1{{.*}}address-use-after-scope

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-WITHOUT-USE-AFTER-SCOPE
// CHECK-ASAN-WITHOUT-USE-AFTER-SCOPE: -cc1{{.*}}address-use-after-scope

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address -fsanitize-address-poison-custom-array-cookie %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-POISON-CUSTOM-ARRAY-NEW-COOKIE
// RUN: %clang_cl --target=x86_64-windows -fsanitize=address -fsanitize-address-poison-custom-array-cookie -### -- %s 2>&1 | FileCheck %s --check-prefix=CHECK-POISON-CUSTOM-ARRAY-NEW-COOKIE
// CHECK-POISON-CUSTOM-ARRAY-NEW-COOKIE: -cc1{{.*}}-fsanitize-address-poison-custom-array-cookie

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address -fno-sanitize-address-poison-custom-array-cookie %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-POISON-CUSTOM-ARRAY-NEW-COOKIE-OFF
// RUN: %clang_cl --target=x86_64-windows -fsanitize=address -fno-sanitize-address-poison-custom-array-cookie -### -- %s 2>&1 | FileCheck %s --check-prefix=CHECK-POISON-CUSTOM-ARRAY-NEW-COOKIE-OFF
// CHECK-POISON-CUSTOM-ARRAY-NEW-COOKIE-OFF-NOT: -cc1{{.*}}address-poison-custom-array-cookie

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address -fno-sanitize-address-poison-custom-array-cookie -fsanitize-address-poison-custom-array-cookie %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-POISON-CUSTOM-ARRAY-NEW-COOKIE-BOTH
// RUN: %clang_cl --target=x86_64-windows -fsanitize=address -fno-sanitize-address-poison-custom-array-cookie -fsanitize-address-poison-custom-array-cookie -### -- %s 2>&1 | FileCheck %s --check-prefix=CHECK-POISON-CUSTOM-ARRAY-NEW-COOKIE-BOTH
// CHECK-POISON-CUSTOM-ARRAY-NEW-COOKIE-BOTH: -cc1{{.*}}-fsanitize-address-poison-custom-array-cookie

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address -fsanitize-address-poison-custom-array-cookie -fno-sanitize-address-poison-custom-array-cookie %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-POISON-CUSTOM-ARRAY-NEW-COOKIE-BOTH-OFF
// CHECK-POISON-CUSTOM-ARRAY-NEW-COOKIE-BOTH-OFF-NOT: -cc1{{.*}}address-poison-custom-array-cookie

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-WITHOUT-POISON-CUSTOM-ARRAY-NEW-COOKIE
// CHECK-ASAN-WITHOUT-POISON-CUSTOM-ARRAY-NEW-COOKIE-NOT: -cc1{{.*}}address-poison-custom-array-cookie

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address -fsanitize-address-globals-dead-stripping %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-GLOBALS
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-GLOBALS
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address -fsanitize-address-globals-dead-stripping -fno-sanitize-address-globals-dead-stripping %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-NO-ASAN-GLOBALS
// RUN: %clang_cl --target=x86_64-windows-msvc -fsanitize=address -fsanitize-address-globals-dead-stripping -### -- %s 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-GLOBALS
// RUN: %clang_cl --target=x86_64-windows-msvc -fsanitize=address -### -- %s 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-GLOBALS
// RUN: %clang --target=x86_64-scei-ps4  -fsanitize=address %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-GLOBALS
// RUN: %clang --target=x86_64-sie-ps5  -fsanitize=address %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-GLOBALS
// CHECK-ASAN-GLOBALS: -cc1{{.*}}-fsanitize-address-globals-dead-stripping
// CHECK-NO-ASAN-GLOBALS-NOT: -cc1{{.*}}-fsanitize-address-globals-dead-stripping

// RUN: %clang --target=x86_64-linux-gnu -fsanitize-address-outline-instrumentation %s -### 2>&1 | \
// RUN:     FileCheck %s --check-prefix=CHECK-ASAN-OUTLINE-WARN
// CHECK-ASAN-OUTLINE-WARN: warning: argument unused during compilation: '-fsanitize-address-outline-instrumentation'
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address -fsanitize-address-outline-instrumentation %s -### 2>&1 | \
// RUN:     FileCheck %s --check-prefix=CHECK-ASAN-OUTLINE-OK
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address -fno-sanitize-address-outline-instrumentation -fsanitize-address-outline-instrumentation %s -### 2>&1 | \
// RUN:     FileCheck %s --check-prefix=CHECK-ASAN-OUTLINE-OK
// CHECK-ASAN-OUTLINE-OK: "-mllvm" "-asan-instrumentation-with-call-threshold=0"
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address -fno-sanitize-address-outline-instrumentation %s -### 2>&1 | \
// RUN:     FileCheck %s --check-prefix=CHECK-NO-CHECK-ASAN-CALLBACK
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address -fsanitize-address-outline-instrumentation -fno-sanitize-address-outline-instrumentation %s -### 2>&1 | \
// RUN:     FileCheck %s --check-prefix=CHECK-NO-CHECK-ASAN-CALLBACK
// CHECK-NO-CHECK-ASAN-CALLBACK-NOT: "-mllvm" "-asan-instrumentation-with-call-threshold=0"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-ODR-INDICATOR
// RUN: %clang_cl --target=x86_64-windows -fsanitize=address -### -- %s 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-ODR-INDICATOR-OFF
// CHECK-ASAN-ODR-INDICATOR-NOT: "-fsanitize-address-use-odr-indicator"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address -fno-sanitize-address-use-odr-indicator %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-ODR-INDICATOR-OFF
// RUN: %clang_cl --target=x86_64-windows -fsanitize=address -fno-sanitize-address-use-odr-indicator -### -- %s 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-ODR-INDICATOR-OFF
// CHECK-ASAN-ODR-INDICATOR-OFF: "-cc1" {{.*}} "-fno-sanitize-address-use-odr-indicator"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address -fno-sanitize-address-use-odr-indicator -fsanitize-address-use-odr-indicator %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-ODR-INDICATOR
// RUN: %clang_cl --target=x86_64-windows -fsanitize=address -fno-sanitize-address-use-odr-indicator -fsanitize-address-use-odr-indicator -### -- %s 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-ODR-INDICATOR

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address -fsanitize-address-use-odr-indicator -fno-sanitize-address-use-odr-indicator %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-ODR-INDICATOR-OFF

// RUN: %clang --target=x86_64-linux-gnu -fsanitize-memory-track-origins -pie %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ONLY-TRACK-ORIGINS
// CHECK-ONLY-TRACK-ORIGINS: warning: argument unused during compilation: '-fsanitize-memory-track-origins'

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory -fno-sanitize=memory -fsanitize-memory-track-origins -pie %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TRACK-ORIGINS-DISABLED-MSAN
// CHECK-TRACK-ORIGINS-DISABLED-MSAN-NOT: warning: argument unused

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-NO-EXTRA-TRACK-ORIGINS
// CHECK-NO-EXTRA-TRACK-ORIGINS-NOT: "-fsanitize-memory-track-origins"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address -fsanitize=alignment -fsanitize=vptr -fno-sanitize=vptr %s -### 2>&1
// OK

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory -pie %s -### 2>&1
// OK

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory -fsanitize-memory-track-origins -pie %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TRACK-ORIGINS-2
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory -fsanitize-memory-track-origins=1 -pie %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TRACK-ORIGINS-1
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory -fsanitize-memory-track-origins=1 -fsanitize-memory-track-origins -pie %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TRACK-ORIGINS-2
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory -fsanitize-memory-track-origins=2 -fsanitize-memory-track-origins -pie %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TRACK-ORIGINS-2
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory -fno-sanitize-memory-track-origins -fsanitize-memory-track-origins -pie %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TRACK-ORIGINS-2
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory -fsanitize-memory-track-origins=0 -fsanitize-memory-track-origins=1 -pie %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TRACK-ORIGINS-1
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory -fsanitize-memory-track-origins=0 -fsanitize-memory-track-origins -pie %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TRACK-ORIGINS-2

// CHECK-TRACK-ORIGINS-1: -fsanitize-memory-track-origins=1

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory -fno-sanitize-memory-track-origins -pie %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-NO-TRACK-ORIGINS
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory -fsanitize-memory-track-origins=0 -pie %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-NO-TRACK-ORIGINS
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory -fsanitize-memory-track-origins -fno-sanitize-memory-track-origins -pie %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-NO-TRACK-ORIGINS
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory -fsanitize-memory-track-origins -fsanitize-memory-track-origins=0 -pie %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-NO-TRACK-ORIGINS
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory -fsanitize-memory-track-origins=2 -fno-sanitize-memory-track-origins -pie %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-NO-TRACK-ORIGINS
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory -fsanitize-memory-track-origins=2 -fsanitize-memory-track-origins=0 -pie %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-NO-TRACK-ORIGINS
// CHECK-NO-TRACK-ORIGINS-NOT: sanitize-memory-track-origins

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory -fsanitize-memory-track-origins=2 -pie %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TRACK-ORIGINS-2
// CHECK-TRACK-ORIGINS-2: -fsanitize-memory-track-origins=2

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=memory -fsanitize-memory-track-origins=3 -pie %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TRACK-ORIGINS-3
// CHECK-TRACK-ORIGINS-3: error: invalid value '3' in '-fsanitize-memory-track-origins=3'

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory -fsanitize-memory-use-after-dtor %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-USE-AFTER-DTOR
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory -fno-sanitize-memory-use-after-dtor -fsanitize-memory-use-after-dtor %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-USE-AFTER-DTOR
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-USE-AFTER-DTOR
// CHECK-USE-AFTER-DTOR: -cc1{{.*}}-fsanitize-memory-use-after-dtor

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory -fno-sanitize-memory-use-after-dtor %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-USE-AFTER-DTOR-OFF
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory -fsanitize-memory-use-after-dtor -fno-sanitize-memory-use-after-dtor %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-USE-AFTER-DTOR-OFF
// CHECK-USE-AFTER-DTOR-OFF-NOT: -cc1{{.*}}memory-use-after-dtor

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address -fsanitize-address-field-padding=0 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-FIELD-PADDING-0
// CHECK-ASAN-FIELD-PADDING-0-NOT: -fsanitize-address-field-padding
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address -fsanitize-address-field-padding=1 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-FIELD-PADDING-1
// CHECK-ASAN-FIELD-PADDING-1: -fsanitize-address-field-padding=1
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address -fsanitize-address-field-padding=2 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-FIELD-PADDING-2
// CHECK-ASAN-FIELD-PADDING-2: -fsanitize-address-field-padding=2
// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=address -fsanitize-address-field-padding=3 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-FIELD-PADDING-3
// CHECK-ASAN-FIELD-PADDING-3: error: invalid value '3' in '-fsanitize-address-field-padding=3'
// RUN: %clang --target=x86_64-linux-gnu -fsanitize-address-field-padding=2 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-FIELD-PADDING-NO-ASAN
// CHECK-ASAN-FIELD-PADDING-NO-ASAN: warning: argument unused during compilation: '-fsanitize-address-field-padding=2'
// RUN: %clang --target=x86_64-linux-gnu -fsanitize-address-field-padding=2 -fsanitize=address -fno-sanitize=address %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-FIELD-PADDING-DISABLED-ASAN
// CHECK-ASAN-FIELD-PADDING-DISABLED-ASAN-NOT: warning: argument unused


// RUN: %clang --target=x86_64-linux-gnu -fsanitize=vptr -fno-sanitize=vptr -fsanitize=undefined,address %s -### 2>&1
// OK

// RUN: %clang --target=arm-linux-androideabi %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ANDROID-NO-ASAN
// CHECK-ANDROID-NO-ASAN: "-mrelocation-model" "pic"

// RUN: not %clang --target=aarch64-linux-android -fsanitize=memory %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-MSAN-ANDROID
// RUN: not %clang --target=i386-linux-android -fsanitize=memory %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-MSAN-ANDROID
// RUN: not %clang --target=x86_64-linux-android -fsanitize=memory %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-MSAN-ANDROID
// CHECK-MSAN-ANDROID: unsupported option '-fsanitize=memory' for target

// RUN: %clang --target=x86_64-linux-gnu %s -fsanitize=undefined -### 2>&1 | FileCheck %s --check-prefix=CHECK-RECOVER-UBSAN
// RUN: %clang --target=x86_64-linux-gnu %s -fsanitize=undefined -fsanitize-recover -### 2>&1 | FileCheck %s --check-prefix=CHECK-RECOVER-UBSAN
// RUN: %clang --target=x86_64-linux-gnu %s -fsanitize=undefined -fsanitize-recover=all -### 2>&1 | FileCheck %s --check-prefix=CHECK-RECOVER-UBSAN
// RUN: %clang --target=x86_64-linux-gnu %s -fsanitize=undefined -fno-sanitize-recover -fsanitize-recover=undefined -### 2>&1 | FileCheck %s --check-prefix=CHECK-RECOVER-UBSAN
// RUN: %clang --target=x86_64-linux-gnu %s -fsanitize=undefined -fno-sanitize-recover=undefined -### 2>&1 | FileCheck %s --check-prefix=CHECK-NO-RECOVER-UBSAN
// RUN: %clang --target=x86_64-linux-gnu %s -fsanitize=undefined -fno-sanitize-recover=all -fsanitize-recover=thread -### 2>&1 | FileCheck %s --check-prefix=CHECK-NO-RECOVER-UBSAN
// RUN: %clang --target=x86_64-linux-gnu %s -fsanitize=undefined -fsanitize-recover=all -fno-sanitize-recover=undefined -### 2>&1 | FileCheck %s --check-prefix=CHECK-NO-RECOVER-UBSAN
// CHECK-RECOVER-UBSAN: "-fsanitize-recover={{((signed-integer-overflow|integer-divide-by-zero|function|shift-base|shift-exponent|vla-bound|alignment|null|pointer-overflow|float-cast-overflow|array-bounds|enum|bool|builtin|returns-nonnull-attribute|nonnull-attribute),?){16}"}}
// CHECK-NO-RECOVER-UBSAN-NOT: sanitize-recover

// RUN: %clang --target=x86_64-linux-gnu %s -fsanitize=undefined -fno-sanitize-recover=all -fsanitize-recover=object-size,shift-base -### 2>&1 | FileCheck %s --check-prefix=CHECK-PARTIAL-RECOVER
// CHECK-PARTIAL-RECOVER: "-fsanitize-recover={{((shift-base),?){1}"}}

// RUN: %clang --target=x86_64-linux-gnu %s -fsanitize=address -fsanitize-recover=all -### 2>&1 | FileCheck %s --check-prefix=CHECK-RECOVER-ASAN
// RUN: %clang --target=x86_64-linux-gnu %s -fsanitize=address -fsanitize-recover -### 2>&1 | FileCheck %s --check-prefix=CHECK-RECOVER-ASAN
// CHECK-RECOVER-ASAN: "-fsanitize-recover=address"

// RUN: not %clang --target=x86_64-linux-gnu %s -fsanitize=undefined -fsanitize-recover=foobar,object-size,unreachable -### 2>&1 | FileCheck %s --check-prefix=CHECK-DIAG-RECOVER
// CHECK-DIAG-RECOVER: unsupported argument 'foobar' to option '-fsanitize-recover='
// CHECK-DIAG-RECOVER: unsupported argument 'unreachable' to option '-fsanitize-recover='

// RUN: %clang --target=x86_64-linux-gnu %s -fsanitize=undefined -fsanitize-recover -fno-sanitize-recover -### 2>&1 | FileCheck %s --check-prefix=CHECK-DEPRECATED-RECOVER
// CHECK-DEPRECATED-RECOVER-NOT: is deprecated

// RUN: not %clang --target=x86_64-linux-gnu %s -fsanitize=kernel-address -fno-sanitize-recover=kernel-address -### 2>&1 | FileCheck %s --check-prefix=CHECK-NO-RECOVER-KASAN
// RUN: not %clang --target=x86_64-linux-gnu %s -fsanitize=kernel-hwaddress -fno-sanitize-recover=kernel-hwaddress -### 2>&1 | FileCheck %s --check-prefix=CHECK-NO-RECOVER-KHWASAN
// CHECK-NO-RECOVER-KASAN: unsupported argument 'kernel-address' to option '-fno-sanitize-recover='
// CHECK-NO-RECOVER-KHWASAN: unsupported argument 'kernel-hwaddress' to option '-fno-sanitize-recover='

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANL
// CHECK-SANL: "-fsanitize=leak"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address,leak -fno-sanitize=address %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANA-SANL-NO-SANA
// CHECK-SANA-SANL-NO-SANA: "-fsanitize=leak"

// RUN: %clang --target=i686-linux-gnu -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANL-X86
// CHECK-SANL-X86: "-fsanitize=leak"

// RUN: %clang --target=i686-linux-gnu -fsanitize=address,leak -fno-sanitize=address %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANA-SANL-NO-SANA-X86
// CHECK-SANA-SANL-NO-SANA-X86: "-fsanitize=leak"

// RUN: %clang --target=arm-linux-gnu -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANL-ARM
// CHECK-SANL-ARM: "-fsanitize=leak"

// RUN: %clang --target=arm-linux-gnu -fsanitize=address,leak -fno-sanitize=address %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANA-SANL-NO-SANA-ARM
// CHECK-SANA-SANL-NO-SANA-ARM: "-fsanitize=leak"

// RUN: %clang --target=thumb-linux -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANL-THUMB
// CHECK-SANL-THUMB: "-fsanitize=leak"

// RUN: %clang --target=thumb-linux -fsanitize=address,leak -fno-sanitize=address %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANA-SANL-NO-SANA-THUMB
// CHECK-SANA-SANL-NO-SANA-THUMB: "-fsanitize=leak"

// RUN: %clang --target=armeb-linux-gnu -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANL-ARMEB
// CHECK-SANL-ARMEB: "-fsanitize=leak"

// RUN: %clang --target=armeb-linux-gnu -fsanitize=address,leak -fno-sanitize=address %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANA-SANL-NO-SANA-ARMEB
// CHECK-SANA-SANL-NO-SANA-ARMEB: "-fsanitize=leak"

// RUN: %clang --target=thumbeb-linux -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANL-THUMBEB
// CHECK-SANL-THUMBEB: "-fsanitize=leak"

// RUN: %clang --target=thumbeb-linux -fsanitize=address,leak -fno-sanitize=address %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANA-SANL-NO-SANA-THUMBEB
// CHECK-SANA-SANL-NO-SANA-THUMBEB: "-fsanitize=leak"

// RUN: not %clang --target=mips-unknown-linux -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANL-MIPS
// CHECK-SANL-MIPS: unsupported option '-fsanitize=leak' for target 'mips-unknown-linux'

// RUN: not %clang --target=mips-unknown-freebsd -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANL-MIPS-FREEBSD
// CHECK-SANL-MIPS-FREEBSD: unsupported option '-fsanitize=leak' for target 'mips-unknown-freebsd'

// RUN: %clang --target=mips64-unknown-freebsd -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANL-MIPS64-FREEBSD
// CHECK-SANL-MIPS64-FREEBSD: "-fsanitize=leak"

// RUN: %clang --target=powerpc64-unknown-linux -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANL-PPC64
// RUN: %clang --target=powerpc64le-unknown-linux -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANL-PPC64
// CHECK-SANL-PPC64: "-fsanitize=leak"
// RUN: not %clang --target=powerpc-unknown-linux -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANL-PPC
// CHECK-SANL-PPC: unsupported option '-fsanitize=leak' for target 'powerpc-unknown-linux'

// RUN: %clang --target=riscv64-linux-gnu -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANL-RISCV64
// CHECK-SANL-RISCV64: "-fsanitize=leak"

// RUN: %clang --target=riscv64-linux-gnu -fsanitize=address,leak -fno-sanitize=address %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANA-SANL-NO-SANA-RISCV64
// CHECK-SANA-SANL-NO-SANA-RISCV64: "-fsanitize=leak"

// RUN: %clang --target=loongarch64-unknown-linux-gnu -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANL-LOONGARCH64
// CHECK-SANL-LOONGARCH64: "-fsanitize=leak"

// RUN: %clang --target=loongarch64-unknown-linux-gnu -fsanitize=address,leak -fno-sanitize=address %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SANA-SANL-NO-SANA-LOONGARCH64
// CHECK-SANA-SANL-NO-SANA-LOONGARCH64: "-fsanitize=leak"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=memory %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-MSAN
// CHECK-MSAN: "-fno-assume-sane-operator-new"
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN
// CHECK-ASAN: "-fno-assume-sane-operator-new"

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=zzz %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-DIAG1
// CHECK-DIAG1: unsupported argument 'zzz' to option '-fsanitize='
// CHECK-DIAG1-NOT: unsupported argument 'zzz' to option '-fsanitize='

// RUN: not %clang --target=x86_64-apple-darwin10 -fsanitize=memory %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-MSAN-DARWIN
// CHECK-MSAN-DARWIN: unsupported option '-fsanitize=memory' for target 'x86_64-apple-darwin10'

// RUN: %clang --target=x86_64-apple-darwin10 -fsanitize=memory -fno-sanitize=memory %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-MSAN-NOMSAN-DARWIN
// CHECK-MSAN-NOMSAN-DARWIN-NOT: unsupported option

// RUN: not %clang --target=x86_64-apple-darwin10 -fsanitize=memory -fsanitize=thread,memory %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-MSAN-TSAN-MSAN-DARWIN
// CHECK-MSAN-TSAN-MSAN-DARWIN: unsupported option '-fsanitize=memory' for target 'x86_64-apple-darwin10'
// CHECK-MSAN-TSAN-MSAN-DARWIN-NOT: unsupported option

// RUN: not %clang --target=x86_64-apple-darwin10 -fsanitize=thread,memory -fsanitize=memory %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-MSAN-MSAN-DARWIN
// CHECK-TSAN-MSAN-MSAN-DARWIN: unsupported option '-fsanitize=memory' for target 'x86_64-apple-darwin10'
// CHECK-TSAN-MSAN-MSAN-DARWIN-NOT: unsupported option

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=numerical %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-NSAN-X86-64-LINUX
// CHECK-NSAN-X86-64-LINUX: "-fsanitize=numerical"

// RUN: not %clang --target=aarch64-unknown-linux-gnu -fsanitize=numerical %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-NSAN-AARCH64-LINUX
// CHECK-NSAN-AARCH64-LINUX: error: unsupported option '-fsanitize=numerical' for target 'aarch64-unknown-linux-gnu'

// RUN: not %clang --target=mips-unknown-linux -fsanitize=numerical %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-NSAN-MIPS-LINUX
// CHECK-NSAN-MIPS-LINUX: error: unsupported option '-fsanitize=numerical' for target 'mips-unknown-linux'

// RUN: %clang --target=x86_64-apple-macos -fsanitize=numerical %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-NSAN-X86-64-MACOS
// CHECK-NSAN-X86-64-MACOS: "-fsanitize=numerical"

// RUN: not %clang --target=arm64-apple-macos -fsanitize=numerical %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-NSAN-ARM64-MACOS
// CHECK-NSAN-ARM64-MACOS: error: unsupported option '-fsanitize=numerical' for target 'arm64-apple-macos'

// RUN: %clang --target=x86_64-apple-darwin -fsanitize=thread %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-X86-64-DARWIN
// CHECK-TSAN-X86-64-DARWIN-NOT: unsupported option
// RUN: %clang --target=x86_64-apple-macos -fsanitize=thread %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-X86-64-MACOS
// CHECK-TSAN-X86-64-MACOS-NOT: unsupported option
// RUN: %clang --target=arm64-apple-macos -fsanitize=thread %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-ARM64-MACOS
// CHECK-TSAN-ARM64-MACOS-NOT: unsupported option

// RUN: %clang --target=arm64-apple-ios-simulator -fsanitize=thread %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-ARM64-IOSSIMULATOR
// CHECK-TSAN-ARM64-IOSSIMULATOR-NOT: unsupported option

// RUN: %clang --target=arm64-apple-watchos-simulator -fsanitize=thread %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-ARM64-WATCHOSSIMULATOR
// CHECK-TSAN-ARM64-WATCHOSSIMULATOR-NOT: unsupported option

// RUN: %clang --target=arm64-apple-tvos-simulator -fsanitize=thread %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-ARM64-TVOSSIMULATOR
// CHECK-TSAN-ARM64-TVOSSIMULATOR-NOT: unsupported option

// RUN: %clang --target=x86_64-apple-ios-simulator -fsanitize=thread %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-X86-64-IOSSIMULATOR
// CHECK-TSAN-X86-64-IOSSIMULATOR-NOT: unsupported option

// RUN: %clang --target=x86_64-apple-watchos-simulator -fsanitize=thread %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-X86-64-WATCHOSSIMULATOR
// CHECK-TSAN-X86-64-WATCHOSSIMULATOR-NOT: unsupported option

// RUN: %clang --target=x86_64-apple-tvos-simulator -fsanitize=thread %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-X86-64-TVOSSIMULATOR
// CHECK-TSAN-X86-64-TVOSSIMULATOR-NOT: unsupported option

// RUN: not %clang --target=i386-apple-darwin -fsanitize=thread %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-I386-DARWIN
// CHECK-TSAN-I386-DARWIN: unsupported option '-fsanitize=thread' for target 'i386-apple-darwin'

// RUN: not %clang --target=arm-apple-ios -fsanitize=thread %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-ARM-IOS
// CHECK-TSAN-ARM-IOS: unsupported option '-fsanitize=thread' for target 'arm-apple-ios'

// RUN: not %clang --target=i386-apple-ios-simulator -fsanitize=thread %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-I386-IOSSIMULATOR
// CHECK-TSAN-I386-IOSSIMULATOR: unsupported option '-fsanitize=thread' for target 'i386-apple-ios-simulator'

// RUN: not %clang --target=i386-apple-tvos-simulator -fsanitize=thread %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-I386-TVOSSIMULATOR
// CHECK-TSAN-I386-TVOSSIMULATOR: unsupported option '-fsanitize=thread' for target 'i386-apple-tvos-simulator'

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=thread -fsanitize-thread-memory-access %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-MEMORY-ACCESS
// CHECK-TSAN-MEMORY-ACCESS-NOT: -cc1{{.*}}tsan-instrument-memory-accesses=0
// CHECK-TSAN-MEMORY-ACCESS-NOT: -cc1{{.*}}tsan-instrument-memintrinsics=0
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=thread -fno-sanitize-thread-memory-access %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-MEMORY-ACCESS-OFF
// CHECK-TSAN-MEMORY-ACCESS-OFF: -cc1{{.*}}tsan-instrument-memory-accesses=0{{.*}}tsan-instrument-memintrinsics=0
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=thread -fno-sanitize-thread-memory-access -fsanitize-thread-memory-access %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-MEMORY-ACCESS-BOTH
// CHECK-TSAN-MEMORY-ACCESS-BOTH-NOT: -cc1{{.*}}tsan-instrument-memory-accesses=0
// CHECK-TSAN-MEMORY-ACCESS-BOTH-NOT: -cc1{{.*}}tsan-instrument-memintrinsics=0
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=thread -fsanitize-thread-memory-access -fno-sanitize-thread-memory-access %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-MEMORY-ACCESS-BOTH-OFF
// CHECK-TSAN-MEMORY-ACCESS-BOTH-OFF: -cc1{{.*}}tsan-instrument-memory-accesses=0{{.*}}tsan-instrument-memintrinsics=0

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=thread -fsanitize-thread-func-entry-exit %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-FUNC-ENTRY-EXIT
// CHECK-TSAN-FUNC-ENTRY-EXIT-NOT: -cc1{{.*}}tsan-instrument-func-entry-exit=0
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=thread -fno-sanitize-thread-func-entry-exit %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-FUNC-ENTRY-EXIT-OFF
// CHECK-TSAN-FUNC-ENTRY-EXIT-OFF: -cc1{{.*}}tsan-instrument-func-entry-exit=0
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=thread -fno-sanitize-thread-func-entry-exit -fsanitize-thread-func-entry-exit %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-FUNC-ENTRY-EXIT-BOTH
// CHECK-TSAN-FUNC-ENTRY-EXIT-BOTH-NOT: -cc1{{.*}}tsan-instrument-func-entry-exit=0
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=thread -fsanitize-thread-func-entry-exit -fno-sanitize-thread-func-entry-exit %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-FUNC-ENTRY-EXIT-BOTH-OFF
// CHECK-TSAN-FUNC-ENTRY-EXIT-BOTH-OFF: -cc1{{.*}}tsan-instrument-func-entry-exit=0

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=thread -fsanitize-thread-atomics %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-ATOMICS
// CHECK-TSAN-ATOMICS-NOT: -cc1{{.*}}tsan-instrument-atomics=0
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=thread -fno-sanitize-thread-atomics %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-ATOMICS-OFF
// CHECK-TSAN-ATOMICS-OFF: -cc1{{.*}}tsan-instrument-atomics=0
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=thread -fno-sanitize-thread-atomics -fsanitize-thread-atomics %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-ATOMICS-BOTH
// CHECK-TSAN-ATOMICS-BOTH-NOT: -cc1{{.*}}tsan-instrument-atomics=0
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=thread -fsanitize-thread-atomics -fno-sanitize-thread-atomics %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-ATOMICS-BOTH-OFF
// CHECK-TSAN-ATOMICS-BOTH-OFF: -cc1{{.*}}tsan-instrument-atomics=0

// RUN: %clang --target=x86_64-apple-darwin10 -fsanitize=function %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-FUNCTION
// RUN: %clang --target=aarch64-unknown-linux-gnu -fsanitize=function %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-FUNCTION
// RUN: %clang --target=riscv64-pc-freebsd -fsanitize=function %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-FUNCTION
// CHECK-FUNCTION: -cc1{{.*}}"-fsanitize=function" "-fsanitize-recover=function"

// RUN: not %clang --target=x86_64-apple-darwin10 -mmacos-version-min=10.8 -fsanitize=vptr %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-VPTR-DARWIN-OLD
// CHECK-VPTR-DARWIN-OLD: unsupported option '-fsanitize=vptr' for target 'x86_64-apple-darwin10'

// RUN: not %clang --target=arm-apple-ios4 -fsanitize=vptr %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-VPTR-IOS-OLD
// CHECK-VPTR-IOS-OLD: unsupported option '-fsanitize=vptr' for target 'arm-apple-ios4'

// RUN: %clang --target=aarch64-apple-darwin15.0.0 -fsanitize=vptr %s -### 2>&1
// OK

// RUN: %clang --target=x86_64-apple-darwin15.0.0-simulator -fsanitize=vptr %s -### 2>&1
// OK

// RUN: %clang --target=x86_64-apple-darwin10 -mmacos-version-min=10.9 -fsanitize=alignment,vptr %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-VPTR-DARWIN-NEW
// CHECK-VPTR-DARWIN-NEW: -fsanitize=alignment,vptr

// RUN: %clang --target=armv7-apple-ios7 -miphoneos-version-min=7.0 -fsanitize=address %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-IOS
// CHECK-ASAN-IOS: -fsanitize=address

// RUN: %clang --target=i386-pc-openbsd -fsanitize=undefined %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UBSAN-OPENBSD
// CHECK-UBSAN-OPENBSD: "-fsanitize={{((signed-integer-overflow|integer-divide-by-zero|function|shift-base|shift-exponent|unreachable|return|vla-bound|alignment|null|pointer-overflow|float-cast-overflow|array-bounds|enum|bool|builtin|returns-nonnull-attribute|nonnull-attribute),?){18}"}}

// RUN: not %clang --target=i386-pc-openbsd -fsanitize=address %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-OPENBSD
// CHECK-ASAN-OPENBSD: unsupported option '-fsanitize=address' for target 'i386-pc-openbsd'

// RUN: not %clang --target=i386-pc-openbsd -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-LSAN-OPENBSD
// CHECK-LSAN-OPENBSD: unsupported option '-fsanitize=leak' for target 'i386-pc-openbsd'

// RUN: not %clang --target=i386-pc-openbsd -fsanitize=thread %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-OPENBSD
// CHECK-TSAN-OPENBSD: unsupported option '-fsanitize=thread' for target 'i386-pc-openbsd'

// RUN: not %clang --target=i386-pc-openbsd -fsanitize=memory %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-MSAN-OPENBSD
// CHECK-MSAN-OPENBSD: unsupported option '-fsanitize=memory' for target 'i386-pc-openbsd'

// RUN: %clang --target=x86_64-apple-darwin -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-LSAN-X86-64-DARWIN
// CHECK-LSAN-X86-64-DARWIN-NOT: unsupported option

// RUN: %clang --target=x86_64-apple-ios-simulator -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-LSAN-X86-64-IOSSIMULATOR
// CHECK-LSAN-X86-64-IOSSIMULATOR-NOT: unsupported option

// RUN: %clang --target=x86_64-apple-tvos-simulator -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-LSAN-X86-64-TVOSSIMULATOR
// CHECK-LSAN-X86-64-TVOSSIMULATOR-NOT: unsupported option

// RUN: %clang --target=i386-apple-darwin -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-LSAN-I386-DARWIN
// CHECK-LSAN-I386-DARWIN-NOT: unsupported option

// RUN: %clang --target=arm-apple-ios -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-LSAN-ARM-IOS
// CHECK-LSAN-ARM-IOS-NOT: unsupported option

// RUN: %clang --target=i386-apple-ios-simulator -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-LSAN-I386-IOSSIMULATOR
// CHECK-LSAN-I386-IOSSIMULATOR-NOT: unsupported option

// RUN: %clang --target=i386-apple-tvos-simulator -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-LSAN-I386-TVOSSIMULATOR
// CHECK-LSAN-I386-TVOSSIMULATOR-NOT: unsupported option


// RUN: %clang --target=x86_64-linux-gnu -fvisibility=hidden -fsanitize=cfi -flto -resource-dir=%S/Inputs/resource_dir -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI
// RUN: %clang --target=x86_64-apple-darwin10 -fvisibility=hidden -fsanitize=cfi -flto -resource-dir=%S/Inputs/resource_dir -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI
// RUN: %clang --target=x86_64-pc-win32 -fvisibility=hidden -fsanitize=cfi -flto -resource-dir=%S/Inputs/resource_dir -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-NOMFCALL
// RUN: %clang --target=x86_64-linux-gnu -fvisibility=hidden -fsanitize=cfi -fsanitize-cfi-cross-dso -flto -resource-dir=%S/Inputs/resource_dir -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-NOMFCALL
// RUN: %clang --target=x86_64-linux-gnu -fvisibility=hidden -fsanitize=cfi-derived-cast -flto -resource-dir=%S/Inputs/resource_dir -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-DCAST
// RUN: %clang --target=x86_64-linux-gnu -fvisibility=hidden -fsanitize=cfi-unrelated-cast -flto -resource-dir=%S/Inputs/resource_dir -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-UCAST
// RUN: %clang --target=x86_64-linux-gnu -flto -fvisibility=hidden -fsanitize=cfi-nvcall -resource-dir=%S/Inputs/resource_dir -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-NVCALL
// RUN: %clang --target=x86_64-linux-gnu -flto -fvisibility=hidden -fsanitize=cfi-vcall -resource-dir=%S/Inputs/resource_dir -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-VCALL
// RUN: %clang --target=arm-linux-gnu -fvisibility=hidden -fsanitize=cfi -flto -resource-dir=%S/Inputs/resource_dir -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI
// RUN: %clang --target=aarch64-linux-gnu -fvisibility=hidden -fsanitize=cfi -flto -resource-dir=%S/Inputs/resource_dir -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI
// RUN: %clang --target=arm-linux-android -fvisibility=hidden -fsanitize=cfi -flto -resource-dir=%S/Inputs/resource_dir -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI
// RUN: %clang --target=arm-none-eabi -fvisibility=hidden -fsanitize=cfi -flto -resource-dir=%S/Inputs/resource_dir -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI
// RUN: %clang --target=thumb-none-eabi -fvisibility=hidden -fsanitize=cfi -flto -resource-dir=%S/Inputs/resource_dir -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI
// RUN: %clang --target=aarch64-linux-android -fvisibility=hidden -fsanitize=cfi -flto -resource-dir=%S/Inputs/resource_dir -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI
// RUN: %clang --target=aarch64_be -fvisibility=hidden -fsanitize=cfi -flto -resource-dir=%S/Inputs/resource_dir -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI
// RUN: %clang --target=riscv32 -fvisibility=hidden -fsanitize=cfi -flto -resource-dir=%S/Inputs/resource_dir -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI
// RUN: %clang --target=riscv64 -fvisibility=hidden -fsanitize=cfi -flto -resource-dir=%S/Inputs/resource_dir -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI
// RUN: %clang --target=loongarch64 -fvisibility=hidden -fsanitize=cfi -flto -resource-dir=%S/Inputs/resource_dir -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI
// CHECK-CFI: -emit-llvm-bc{{.*}}-fsanitize=cfi-derived-cast,cfi-icall,cfi-mfcall,cfi-unrelated-cast,cfi-nvcall,cfi-vcall
// CHECK-CFI-NOMFCALL: -emit-llvm-bc{{.*}}-fsanitize=cfi-derived-cast,cfi-icall,cfi-unrelated-cast,cfi-nvcall,cfi-vcall
// CHECK-CFI-DCAST: -emit-llvm-bc{{.*}}-fsanitize=cfi-derived-cast
// CHECK-CFI-UCAST: -emit-llvm-bc{{.*}}-fsanitize=cfi-unrelated-cast
// CHECK-CFI-NVCALL: -emit-llvm-bc{{.*}}-fsanitize=cfi-nvcall
// CHECK-CFI-VCALL: -emit-llvm-bc{{.*}}-fsanitize=cfi-vcall

// RUN: not %clang --target=x86_64-linux-gnu -fvisibility=hidden -flto -fsanitize=cfi-derived-cast -fno-lto -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-NOLTO
// CHECK-CFI-NOLTO: '-fsanitize=cfi-derived-cast' only allowed with '-flto'

// RUN: not %clang --target=x86_64-linux-gnu -flto -fsanitize=cfi-derived-cast -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-NOVIS
// CHECK-CFI-NOVIS: error: invalid argument '-fsanitize=cfi-derived-cast' only allowed with '-fvisibility='

// RUN: %clang --target=x86_64-pc-win32 -flto -fsanitize=cfi-derived-cast -resource-dir=%S/Inputs/resource_dir -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-NOVIS-NOERROR
// RUN: echo > %t.o
// RUN: %clang --target=x86_64-linux-gnu -flto -fsanitize=cfi-derived-cast -resource-dir=%S/Inputs/resource_dir %t.o -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-NOVIS-NOERROR
// CHECK-CFI-NOVIS-NOERROR-NOT: only allowed with

// RUN: not %clang --target=mips-unknown-linux -fsanitize=cfi-icall %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-ICALL-MIPS
// CHECK-CFI-ICALL-MIPS: error: unsupported option '-fsanitize=cfi-icall' for target 'mips-unknown-linux'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize-trap=address -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-TRAP
// CHECK-ASAN-TRAP: error: unsupported argument 'address' to option '-fsanitize-trap='

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize-trap=hwaddress -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-HWASAN-TRAP
// CHECK-HWASAN-TRAP: error: unsupported argument 'hwaddress' to option '-fsanitize-trap='

// RUN: not %clang --target=x86_64-apple-darwin10 -mmacos-version-min=10.7 -flto -fsanitize=cfi-vcall -fno-sanitize-trap=cfi -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-NOTRAP-OLD-MACOS
// CHECK-CFI-NOTRAP-OLD-MACOS: error: unsupported option '-fno-sanitize-trap=cfi-vcall' for target 'x86_64-apple-darwin10'

// RUN: %clang --target=x86_64-pc-win32 -flto -fsanitize=cfi-vcall -fno-sanitize-trap=cfi -c -resource-dir=%S/Inputs/resource_dir %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-NOTRAP-WIN
// CHECK-CFI-NOTRAP-WIN: -emit-llvm-bc
// CHECK-CFI-NOTRAP-WIN-NOT: -fsanitize-trap=cfi

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=cfi -fsanitize-cfi-cross-dso -flto -fvisibility=hidden -c -resource-dir=%S/Inputs/resource_dir %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-CROSS-DSO
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=cfi -flto -fvisibility=hidden -c -resource-dir=%S/Inputs/resource_dir %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-NO-CROSS-DSO
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=cfi -fsanitize-cfi-cross-dso -fno-sanitize-cfi-cross-dso -fvisibility=hidden -flto -c -resource-dir=%S/Inputs/resource_dir %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-NO-CROSS-DSO
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=cfi -fno-sanitize-cfi-cross-dso -fsanitize-cfi-cross-dso -fvisibility=hidden -flto -c -resource-dir=%S/Inputs/resource_dir %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-CROSS-DSO
// CHECK-CFI-CROSS-DSO: -emit-llvm-bc
// CHECK-CFI-CROSS-DSO: -fsanitize-cfi-cross-dso
// CHECK-CFI-NO-CROSS-DSO: -emit-llvm-bc
// CHECK-CFI-NO-CROSS-DSO-NOT: -fsanitize-cfi-cross-dso

// RUN: not %clang --target=x86_64-linux-gnu -fvisibility=hidden -fsanitize=cfi-mfcall -fsanitize-cfi-cross-dso -flto -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-MFCALL-CROSS-DSO
// CHECK-CFI-MFCALL-CROSS-DSO: '-fsanitize=cfi-mfcall' not allowed with '-fsanitize-cfi-cross-dso'

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=cfi-icall -fsanitize-cfi-icall-generalize-pointers -fvisibility=hidden -flto -c -resource-dir=%S/Inputs/resource_dir %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-GENERALIZE-POINTERS
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=cfi-icall -fvisibility=hidden -flto -c -resource-dir=%S/Inputs/resource_dir %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-NO-CFI-GENERALIZE-POINTERS
// CHECK-CFI-GENERALIZE-POINTERS: -fsanitize-cfi-icall-generalize-pointers
// CHECK-NO-CFI-GENERALIZE-POINTERS-NOT: -fsanitize-cfi-icall-generalize-pointers

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=cfi-icall -fsanitize-cfi-icall-generalize-pointers -fsanitize-cfi-cross-dso -fvisibility=hidden -flto -c %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-GENERALIZE-AND-CROSS-DSO
// CHECK-CFI-GENERALIZE-AND-CROSS-DSO: error: invalid argument '-fsanitize-cfi-cross-dso' not allowed with '-fsanitize-cfi-icall-generalize-pointers'

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=cfi-icall -fsanitize-cfi-canonical-jump-tables -fvisibility=hidden -flto -c -resource-dir=%S/Inputs/resource_dir %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-CANONICAL-JUMP-TABLES
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=cfi-icall -fno-sanitize-cfi-canonical-jump-tables -fvisibility=hidden -flto -c %s -resource-dir=%S/Inputs/resource_dir -### 2>&1 | FileCheck %s --check-prefix=CHECK-NO-CFI-CANONICAL-JUMP-TABLES
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=cfi-icall -fvisibility=hidden -flto -c -resource-dir=%S/Inputs/resource_dir %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-CANONICAL-JUMP-TABLES
// CHECK-CFI-CANONICAL-JUMP-TABLES: -fsanitize-cfi-canonical-jump-tables
// CHECK-NO-CFI-CANONICAL-JUMP-TABLES-NOT: -fsanitize-cfi-canonical-jump-tables

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=cfi -fsanitize-stats -flto -fvisibility=hidden -c -resource-dir=%S/Inputs/resource_dir %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-STATS
// CHECK-CFI-STATS: -fsanitize-stats

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=kcfi -fsanitize=cfi -flto -fvisibility=hidden %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-KCFI-NOCFI
// CHECK-KCFI-NOCFI: error: invalid argument '-fsanitize=kcfi' not allowed with '-fsanitize=cfi'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=kcfi -fsanitize-trap=kcfi %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-KCFI-NOTRAP
// CHECK-KCFI-NOTRAP: error: unsupported argument 'kcfi' to option '-fsanitize-trap='

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=kcfi %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-KCFI
// CHECK-KCFI: "-fsanitize=kcfi"

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=kcfi -fno-sanitize-recover=kcfi %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-KCFI-RECOVER
// CHECK-KCFI-RECOVER: error: unsupported argument 'kcfi' to option '-fno-sanitize-recover='

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=kcfi,function %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-KCFI-FUNCTION
// CHECK-KCFI-FUNCTION: error: invalid argument '-fsanitize=kcfi' not allowed with '-fsanitize=function'

// RUN: not %clang_cl -fsanitize=address -c -MDd -### -- %s 2>&1 | FileCheck %s -check-prefix=CHECK-ASAN-DEBUGRTL
// RUN: not %clang_cl -fsanitize=address -c -MTd -### -- %s 2>&1 | FileCheck %s -check-prefix=CHECK-ASAN-DEBUGRTL
// RUN: not %clang_cl -fsanitize=address -c -LDd -### -- %s 2>&1 | FileCheck %s -check-prefix=CHECK-ASAN-DEBUGRTL
// RUN: not %clang_cl -fsanitize=address -c -MD -MDd -### -- %s 2>&1 | FileCheck %s -check-prefix=CHECK-ASAN-DEBUGRTL
// RUN: not %clang_cl -fsanitize=address -c -MT -MTd -### -- %s 2>&1 | FileCheck %s -check-prefix=CHECK-ASAN-DEBUGRTL
// RUN: not %clang_cl -fsanitize=address -c -LD -LDd -### -- %s 2>&1 | FileCheck %s -check-prefix=CHECK-ASAN-DEBUGRTL
// CHECK-ASAN-DEBUGRTL: error: invalid argument
// CHECK-ASAN-DEBUGRTL: not allowed with '-fsanitize=address'
// CHECK-ASAN-DEBUGRTL: note: AddressSanitizer doesn't support linking with debug runtime libraries yet

// RUN: %clang_cl -fsanitize=address -c -MT -### -- %s 2>&1 | FileCheck %s -check-prefix=CHECK-ASAN-RELEASERTL
// RUN: %clang_cl -fsanitize=address -c -MD -### -- %s 2>&1 | FileCheck %s -check-prefix=CHECK-ASAN-RELEASERTL
// RUN: %clang_cl -fsanitize=address -c -LD -### -- %s 2>&1 | FileCheck %s -check-prefix=CHECK-ASAN-RELEASERTL
// RUN: %clang_cl -fsanitize=address -c -MTd -MT -### -- %s 2>&1 | FileCheck %s -check-prefix=CHECK-ASAN-RELEASERTL
// RUN: %clang_cl -fsanitize=address -c -MDd -MD -### -- %s 2>&1 | FileCheck %s -check-prefix=CHECK-ASAN-RELEASERTL
// RUN: %clang_cl -fsanitize=address -c -LDd -LD -### -- %s 2>&1 | FileCheck %s -check-prefix=CHECK-ASAN-RELEASERTL
// CHECK-ASAN-RELEASERTL-NOT: error: invalid argument

// RUN: %clang -fno-sanitize=safe-stack -### %s 2>&1 | FileCheck %s -check-prefix=NOSP
// NOSP-NOT: "-fsanitize=safe-stack"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=safe-stack -### %s 2>&1 | FileCheck %s -check-prefix=NO-SP
// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=address,safe-stack -### %s 2>&1 | FileCheck %s -check-prefix=SP-ASAN
// RUN: %clang --target=x86_64-linux-gnu -fstack-protector -fsanitize=safe-stack -### %s 2>&1 | FileCheck %s -check-prefix=SP
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=safe-stack -fstack-protector-all -### %s 2>&1 | FileCheck %s -check-prefix=SP
// RUN: %clang --target=arm-linux-androideabi -fsanitize=safe-stack -### %s 2>&1 | FileCheck %s -check-prefix=NO-SP
// RUN: %clang --target=aarch64-linux-android -fsanitize=safe-stack -### %s 2>&1 | FileCheck %s -check-prefix=NO-SP
// NO-SP-NOT: stack-protector
// NO-SP: "-fsanitize=safe-stack"
// SP-ASAN: error: invalid argument '-fsanitize=safe-stack' not allowed with '-fsanitize=address'
// SP: -stack-protector
// SP: "-fsanitize=safe-stack"
// NO-SP-NOT: stack-protector

// RUN: %clang --target=powerpc64-unknown-linux-gnu -fsanitize=memory %s -### 2>&1 | FileCheck %s -check-prefix=CHECK-SANM
// RUN: %clang --target=powerpc64le-unknown-linux-gnu -fsanitize=memory %s -### 2>&1 | FileCheck %s -check-prefix=CHECK-SANM
// CHECK-SANM: "-fsanitize=memory"

// RUN: %clang --target=x86_64--freebsd -fsanitize=kernel-address %s -### 2>&1 | FileCheck %s -check-prefix=KERNEL-ADDRESS-FREEBSD
// RUN: %clang --target=aarch64--freebsd -fsanitize=kernel-address %s -### 2>&1 | FileCheck %s -check-prefix=KERNEL-ADDRESS-FREEBSD
// KERNEL-ADDRESS-FREEBSD: "-fsanitize=kernel-address"

// RUN: %clang --target=x86_64--freebsd -fsanitize=kernel-memory %s -### 2>&1 | FileCheck %s -check-prefix=KERNEL-MEMORY-FREEBSD
// RUN: %clang --target=aarch64--freebsd -fsanitize=kernel-memory %s -### 2>&1 | FileCheck %s -check-prefix=KERNEL-MEMORY-FREEBSD
// KERNEL-MEMORY-FREEBSD: "-fsanitize=kernel-memory"

// * NetBSD; please keep ordered as in Sanitizers.def *

// RUN: %clang --target=i386--netbsd -fsanitize=address %s -### 2>&1 | FileCheck %s -check-prefix=ADDRESS-NETBSD
// RUN: %clang --target=x86_64--netbsd -fsanitize=address %s -### 2>&1 | FileCheck %s -check-prefix=ADDRESS-NETBSD
// ADDRESS-NETBSD: "-fsanitize=address"

// RUN: %clang --target=x86_64--netbsd -fsanitize=kernel-address %s -### 2>&1 | FileCheck %s -check-prefix=KERNEL-ADDRESS-NETBSD
// KERNEL-ADDRESS-NETBSD: "-fsanitize=kernel-address"

// RUN: %clang --target=x86_64--netbsd -fsanitize=hwaddress %s -### 2>&1 | FileCheck %s -check-prefix=HWADDRESS-NETBSD
// HWADDRESS-NETBSD: "-fsanitize=hwaddress"

// RUN: %clang --target=x86_64--netbsd -fsanitize=kernel-hwaddress %s -### 2>&1 | FileCheck %s -check-prefix=KERNEL-HWADDRESS-NETBSD
// KERNEL-HWADDRESS-NETBSD: "-fsanitize=kernel-hwaddress"

// RUN: %clang --target=x86_64--netbsd -fsanitize=memory %s -### 2>&1 | FileCheck %s -check-prefix=MEMORY-NETBSD
// MEMORY-NETBSD: "-fsanitize=memory"

// RUN: %clang --target=x86_64--netbsd -fsanitize=kernel-memory %s -### 2>&1 | FileCheck %s -check-prefix=KERNEL-MEMORY-NETBSD
// KERNEL-MEMORY-NETBSD: "-fsanitize=kernel-memory"

// RUN: %clang --target=x86_64--netbsd -fsanitize=thread %s -### 2>&1 | FileCheck %s -check-prefix=THREAD-NETBSD
// THREAD-NETBSD: "-fsanitize=thread"

// RUN: %clang --target=i386--netbsd -fsanitize=leak %s -### 2>&1 | FileCheck %s -check-prefix=LEAK-NETBSD
// RUN: %clang --target=x86_64--netbsd -fsanitize=leak %s -### 2>&1 | FileCheck %s -check-prefix=LEAK-NETBSD
// LEAK-NETBSD: "-fsanitize=leak"

// RUN: %clang --target=i386--netbsd -fsanitize=function %s -### 2>&1 | FileCheck %s -check-prefix=FUNCTION-NETBSD
// RUN: %clang --target=x86_64--netbsd -fsanitize=function %s -### 2>&1 | FileCheck %s -check-prefix=FUNCTION-NETBSD
// FUNCTION-NETBSD: "-fsanitize=function"

// RUN: %clang --target=i386--netbsd -fsanitize=vptr %s -### 2>&1 | FileCheck %s -check-prefix=VPTR-NETBSD
// RUN: %clang --target=x86_64--netbsd -fsanitize=vptr %s -### 2>&1 | FileCheck %s -check-prefix=VPTR-NETBSD
// VPTR-NETBSD: "-fsanitize=vptr"

// RUN: %clang --target=x86_64--netbsd -fsanitize=dataflow %s -### 2>&1 | FileCheck %s -check-prefix=DATAFLOW-NETBSD
// DATAFLOW-NETBSD: "-fsanitize=dataflow"

// RUN: %clang --target=i386--netbsd -fsanitize=cfi -flto -fvisibility=hidden -resource-dir=%S/Inputs/resource_dir -c %s -### 2>&1 | FileCheck %s -check-prefix=CFI-NETBSD
// RUN: %clang --target=x86_64--netbsd -fsanitize=cfi -flto -fvisibility=hidden -resource-dir=%S/Inputs/resource_dir -c %s -### 2>&1 | FileCheck %s -check-prefix=CFI-NETBSD
// CFI-NETBSD: "-fsanitize=cfi-derived-cast,cfi-icall,cfi-mfcall,cfi-unrelated-cast,cfi-nvcall,cfi-vcall"

// RUN: %clang --target=i386--netbsd -fsanitize=safe-stack %s -### 2>&1 | FileCheck %s -check-prefix=SAFESTACK-NETBSD
// RUN: %clang --target=x86_64--netbsd -fsanitize=safe-stack %s -### 2>&1 | FileCheck %s -check-prefix=SAFESTACK-NETBSD
// SAFESTACK-NETBSD: "-fsanitize=safe-stack"

// RUN: %clang --target=x86_64--netbsd -fsanitize=shadow-call-stack %s -### 2>&1 | FileCheck %s -check-prefix=SHADOW-CALL-STACK-NETBSD
// SHADOW-CALL-STACK-NETBSD: "-fsanitize=shadow-call-stack"

// RUN: %clang --target=i386--netbsd -fsanitize=undefined %s -### 2>&1 | FileCheck %s -check-prefix=UNDEFINED-NETBSD
// RUN: %clang --target=x86_64--netbsd -fsanitize=undefined %s -### 2>&1 | FileCheck %s -check-prefix=UNDEFINED-NETBSD
// UNDEFINED-NETBSD: "-fsanitize={{.*}}

// RUN: %clang --target=i386--netbsd -fsanitize=scudo %s -### 2>&1 | FileCheck %s -check-prefix=SCUDO-NETBSD
// RUN: %clang --target=x86_64--netbsd -fsanitize=scudo %s -### 2>&1 | FileCheck %s -check-prefix=SCUDO-NETBSD
// SCUDO-NETBSD: "-fsanitize=scudo"

// RUN: %clang --target=i386--solaris -fsanitize=function %s -### 2>&1 | FileCheck %s -check-prefix=FUNCTION-SOLARIS
// RUN: %clang --target=x86_64--solaris -fsanitize=function %s -### 2>&1 | FileCheck %s -check-prefix=FUNCTION-SOLARIS
// FUNCTION-SOLARIS: "-fsanitize=function"


// RUN: not %clang --target=x86_64-scei-ps4 -fsanitize=dataflow %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-DFSAN-PS4
// CHECK-DFSAN-PS4: unsupported option '-fsanitize=dataflow' for target 'x86_64-scei-ps4'
// RUN: not %clang --target=x86_64-scei-ps4 -fsanitize=leak %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-LSAN-PS4
// CHECK-LSAN-PS4: unsupported option '-fsanitize=leak' for target 'x86_64-scei-ps4'
// RUN: not %clang --target=x86_64-scei-ps4 -fsanitize=memory %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-MSAN-PS4
// CHECK-MSAN-PS4: unsupported option '-fsanitize=memory' for target 'x86_64-scei-ps4'
// RUN: not %clang --target=x86_64-scei-ps4 -fsanitize=thread %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-PS4
// CHECK-TSAN-PS4: unsupported option '-fsanitize=thread' for target 'x86_64-scei-ps4'
// RUN: %clang --target=x86_64-scei-ps4 -fsanitize=address %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-PS4
// Make sure there are no *.{o,bc} or -l passed before the ASan library.
// CHECK-ASAN-PS4-NOT: {{(\.(o|bc)"? |-l).*-lSceDbgAddressSanitizer_stub_weak}}
// CHECK-ASAN-PS4: --dependent-lib=libSceDbgAddressSanitizer_stub_weak.a
// CHECK-ASAN-PS4-NOT: {{(\.(o|bc)"? |-l).*-lSceDbgAddressSanitizer_stub_weak}}
// CHECK-ASAN-PS4: -lSceDbgAddressSanitizer_stub_weak
// RUN: %clang --target=x86_64-scei-ps4 -fsanitize=address -nostdlib %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-NOLIB-PS4
// RUN: %clang --target=x86_64-scei-ps4 -fsanitize=address -nodefaultlibs %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-NOLIB-PS4
// RUN: %clang --target=x86_64-scei-ps4 -fsanitize=address -nodefaultlibs -nostdlib  %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-NOLIB-PS4
// CHECK-ASAN-NOLIB-PS4-NOT: SceDbgAddressSanitizer_stub_weak

// RUN: %clang --target=x86_64-sie-ps5 -fsanitize=address %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-PS5
// Make sure there are no *.{o,bc} or -l passed before the ASan library.
// CHECK-ASAN-PS5-NOT: {{(\.(o|bc)"? |-l).*-lSceAddressSanitizer_nosubmission_stub_weak}}
// CHECK-ASAN-PS5: --dependent-lib=libSceAddressSanitizer_nosubmission_stub_weak.a
// CHECK-ASAN-PS5-NOT: {{(\.(o|bc)"? |-l).*-lSceAddressSanitizer_nosubmission_stub_weak}}
// CHECK-ASAN-PS5: -lSceAddressSanitizer_nosubmission_stub_weak
// RUN: %clang --target=x86_64-sie-ps5 -fsanitize=address -nostdlib %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-NOLIB-PS5
// RUN: %clang --target=x86_64-sie-ps5 -fsanitize=address -nodefaultlibs %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-NOLIB-PS5
// RUN: %clang --target=x86_64-sie-ps5 -fsanitize=address -nodefaultlibs -nostdlib  %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-NOLIB-PS5
// CHECK-ASAN-NOLIB-PS5-NOT: SceAddressSanitizer_nosubmission_stub_weak

// RUN: %clang --target=x86_64-sie-ps5 -fsanitize=thread %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-PS5
// Make sure there are no *.{o,bc} or -l passed before the TSan library.
// CHECK-TSAN-PS5-NOT: {{(\.(o|bc)"? |-l).*-lSceThreadSanitizer_nosubmission_stub_weak}}
// CHECK-TSAN-PS5: --dependent-lib=libSceThreadSanitizer_nosubmission_stub_weak.a
// CHECK-TSAN-PS5-NOT: {{(\.(o|bc)"? |-l).*-lSceThreadSanitizer_nosubmission_stub_weak}}
// CHECK-TSAN-PS5: -lSceThreadSanitizer_nosubmission_stub_weak
// RUN: %clang --target=x86_64-sie-ps5 -fsanitize=thread -nostdlib %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-NOLIB-PS5
// RUN: %clang --target=x86_64-sie-ps5 -fsanitize=thread -nodefaultlibs %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-NOLIB-PS5
// RUN: %clang --target=x86_64-sie-ps5 -fsanitize=thread -nodefaultlibs -nostdlib  %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-NOLIB-PS5
// CHECK-TSAN-NOLIB-PS5-NOT: SceThreadSanitizer_nosubmission_stub_weak


// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=address -fsanitize-minimal-runtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-MINIMAL
// CHECK-ASAN-MINIMAL: error: invalid argument '-fsanitize-minimal-runtime' not allowed with '-fsanitize=address'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=thread -fsanitize-minimal-runtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-TSAN-MINIMAL
// CHECK-TSAN-MINIMAL: error: invalid argument '-fsanitize-minimal-runtime' not allowed with '-fsanitize=thread'

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-minimal-runtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UBSAN-MINIMAL
// CHECK-UBSAN-MINIMAL: "-fsanitize={{((signed-integer-overflow|integer-divide-by-zero|shift-base|shift-exponent|unreachable|return|vla-bound|alignment|null|pointer-overflow|float-cast-overflow|array-bounds|enum|bool|builtin|returns-nonnull-attribute|nonnull-attribute|function),?){18}"}}
// CHECK-UBSAN-MINIMAL: "-fsanitize-minimal-runtime"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=integer -fsanitize-trap=integer %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-INTSAN-TRAP
// CHECK-INTSAN-TRAP: "-fsanitize-trap=integer-divide-by-zero,shift-base,shift-exponent,signed-integer-overflow,unsigned-integer-overflow,unsigned-shift-base,implicit-unsigned-integer-truncation,implicit-signed-integer-truncation,implicit-integer-sign-change"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=integer -fsanitize-minimal-runtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-INTSAN-MINIMAL
// CHECK-INTSAN-MINIMAL: "-fsanitize=integer-divide-by-zero,shift-base,shift-exponent,signed-integer-overflow,unsigned-integer-overflow,unsigned-shift-base,implicit-unsigned-integer-truncation,implicit-signed-integer-truncation,implicit-integer-sign-change"
// CHECK-INTSAN-MINIMAL: "-fsanitize-minimal-runtime"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=implicit-conversion -fsanitize-trap=implicit-conversion %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-IMPL-CONV-TRAP
// CHECK-IMPL-CONV-TRAP: "-fsanitize-trap=implicit-unsigned-integer-truncation,implicit-signed-integer-truncation,implicit-integer-sign-change,implicit-bitfield-conversion"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=implicit-conversion -fsanitize-minimal-runtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-IMPL-CONV-MINIMAL
// CHECK-IMPL-CONV-MINIMAL: "-fsanitize=implicit-unsigned-integer-truncation,implicit-signed-integer-truncation,implicit-integer-sign-change,implicit-bitfield-conversion"
// CHECK-IMPL-CONV-MINIMAL: "-fsanitize-minimal-runtime"

// RUN: %clang --target=aarch64-linux-android -march=armv8-a+memtag -fsanitize=memtag -fsanitize-minimal-runtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-MEMTAG-MINIMAL
// CHECK-MEMTAG-MINIMAL: "-fsanitize=memtag-stack,memtag-heap,memtag-globals"
// CHECK-MEMTAG-MINIMAL: "-fsanitize-minimal-runtime"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize=function -fsanitize-minimal-runtime %s -### 2>&1 | FileCheck /dev/null --implicit-check-not=error:

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize=vptr -fsanitize-minimal-runtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UBSAN-VPTR-MINIMAL
// CHECK-UBSAN-VPTR-MINIMAL: error: invalid argument '-fsanitize=vptr' not allowed with '-fsanitize-minimal-runtime'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=address -fsanitize-minimal-runtime -fsanitize=undefined %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-ASAN-UBSAN-MINIMAL
// CHECK-ASAN-UBSAN-MINIMAL: error: invalid argument '-fsanitize-minimal-runtime' not allowed with '-fsanitize=address'
// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=hwaddress -fsanitize-minimal-runtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-HWASAN-MINIMAL
// CHECK-HWASAN-MINIMAL: error: invalid argument '-fsanitize-minimal-runtime' not allowed with '-fsanitize=hwaddress'

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=cfi -flto -fvisibility=hidden -fsanitize-minimal-runtime -resource-dir=%S/Inputs/resource_dir %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-MINIMAL
// CHECK-CFI-MINIMAL: "-fsanitize=cfi-derived-cast,cfi-icall,cfi-mfcall,cfi-unrelated-cast,cfi-nvcall,cfi-vcall"
// CHECK-CFI-MINIMAL: "-fsanitize-trap=cfi-derived-cast,cfi-icall,cfi-mfcall,cfi-unrelated-cast,cfi-nvcall,cfi-vcall"
// CHECK-CFI-MINIMAL: "-fsanitize-minimal-runtime"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=cfi -flto -fvisibility=hidden -fsanitize-minimal-runtime -fsanitize-recover=cfi -resource-dir=%S/Inputs/resource_dir %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-RECOVER-MINIMAL
// CHECK-CFI-RECOVER-MINIMAL: "-fsanitize=cfi-derived-cast,cfi-icall,cfi-mfcall,cfi-unrelated-cast,cfi-nvcall,cfi-vcall"
// CHECK-CFI-RECOVER-MINIMAL: "-fsanitize-trap=cfi-derived-cast,cfi-icall,cfi-mfcall,cfi-unrelated-cast,cfi-nvcall,cfi-vcall"
// CHECK-CFI-RECOVER-MINIMAL: "-fsanitize-minimal-runtime"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=cfi -flto -fvisibility=hidden -fsanitize-minimal-runtime -fno-sanitize-recover=cfi -resource-dir=%S/Inputs/resource_dir %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-ABORT-MINIMAL
// CHECK-CFI-ABORT-MINIMAL: "-fsanitize=cfi-derived-cast,cfi-icall,cfi-mfcall,cfi-unrelated-cast,cfi-nvcall,cfi-vcall"
// CHECK-CFI-ABORT-MINIMAL: "-fsanitize-trap=cfi-derived-cast,cfi-icall,cfi-mfcall,cfi-unrelated-cast,cfi-nvcall,cfi-vcall"
// CHECK-CFI-ABORT-MINIMAL: "-fsanitize-minimal-runtime"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=cfi -flto -fvisibility=hidden -fsanitize-minimal-runtime -fno-sanitize-trap=cfi -fsanitize-recover=cfi -resource-dir=%S/Inputs/resource_dir %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-NOTRAP-RECOVER-MINIMAL --
// CHECK-CFI-NOTRAP-RECOVER-MINIMAL: "-fsanitize=cfi-derived-cast,cfi-icall,cfi-mfcall,cfi-unrelated-cast,cfi-nvcall,cfi-vcall"
// CHECK-CFI-NOTRAP-RECOVER-MINIMAL: "-fsanitize-recover=cfi-derived-cast,cfi-icall,cfi-mfcall,cfi-unrelated-cast,cfi-nvcall,cfi-vcall"
// CHECK-CFI-NOTRAP-RECOVER-MINIMAL: "-fsanitize-minimal-runtime"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=cfi -flto -fvisibility=hidden -fsanitize-minimal-runtime -fno-sanitize-trap=cfi -fno-sanitize-recover=cfi -resource-dir=%S/Inputs/resource_dir %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-NOTRAP-ABORT-MINIMAL
// CHECK-CFI-NOTRAP-ABORT-MINIMAL: "-fsanitize=cfi-derived-cast,cfi-icall,cfi-mfcall,cfi-unrelated-cast,cfi-nvcall,cfi-vcall"
// CHECK-CFI-NOTRAP-ABORT-MINIMAL: "-fsanitize-minimal-runtime"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=cfi -fno-sanitize-trap=cfi-icall -flto -fvisibility=hidden -fsanitize-minimal-runtime -resource-dir=%S/Inputs/resource_dir %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-NOTRAP-MINIMAL
// CHECK-CFI-NOTRAP-MINIMAL: "-fsanitize=cfi-derived-cast,cfi-icall,cfi-mfcall,cfi-unrelated-cast,cfi-nvcall,cfi-vcall"
// CHECK-CFI-NOTRAP-MINIMAL: "-fsanitize-trap=cfi-derived-cast,cfi-mfcall,cfi-unrelated-cast,cfi-nvcall,cfi-vcall"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=cfi -fno-sanitize-trap=cfi-icall -fno-sanitize=cfi-icall -flto -fvisibility=hidden -fsanitize-minimal-runtime -resource-dir=%S/Inputs/resource_dir %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-CFI-NOICALL-MINIMAL
// CHECK-CFI-NOICALL-MINIMAL: "-fsanitize=cfi-derived-cast,cfi-mfcall,cfi-unrelated-cast,cfi-nvcall,cfi-vcall"
// CHECK-CFI-NOICALL-MINIMAL: "-fsanitize-trap=cfi-derived-cast,cfi-mfcall,cfi-unrelated-cast,cfi-nvcall,cfi-vcall"
// CHECK-CFI-NOICALL-MINIMAL: "-fsanitize-minimal-runtime"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=shadow-call-stack -fsanitize-minimal-runtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCS-MINIMAL
// CHECK-SCS-MINIMAL: "-fsanitize=shadow-call-stack"
// CHECK-SCS-MINIMAL: "-fsanitize-minimal-runtime"

// RUN: %clang --target=aarch64 -fsanitize=shadow-call-stack -ffixed-x18 %s -### 2>&1 | FileCheck %s --check-prefix=AARCH64-SCS
// RUN: %clang --target=aarch64_be -fsanitize=shadow-call-stack -ffixed-x18 %s -### 2>&1 | FileCheck %s --check-prefix=AARCH64-SCS
// AARCH64-SCS: "-fsanitize=shadow-call-stack"

// RUN: %clang --target=aarch64-linux-gnu -fsanitize=scudo %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCUDO
// RUN: %clang --target=arm-linux-androideabi -fsanitize=scudo %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCUDO
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=scudo %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCUDO
// RUN: %clang --target=i386-linux-gnu -fsanitize=scudo %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCUDO
// RUN: %clang --target=loongarch64-unknown-linux-gnu -fsanitize=scudo %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCUDO
// RUN: %clang --target=mips64-unknown-linux-gnu -fsanitize=scudo %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCUDO
// RUN: %clang --target=mips64el-unknown-linux-gnu -fsanitize=scudo %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCUDO
// RUN: %clang --target=mips-unknown-linux-gnu -fsanitize=scudo %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCUDO
// RUN: %clang --target=mipsel-unknown-linux-gnu -fsanitize=scudo %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCUDO
// RUN: %clang --target=powerpc64-unknown-linux -fsanitize=scudo %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCUDO
// RUN: %clang --target=powerpc64le-unknown-linux -fsanitize=scudo %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCUDO
// RUN: %clang --target=riscv64-linux-gnu -fsanitize=scudo %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCUDO
// CHECK-SCUDO: "-fsanitize=scudo"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=scudo,undefined %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCUDO-UBSAN
// CHECK-SCUDO-UBSAN: "-fsanitize={{.*}}scudo"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=scudo -fsanitize-minimal-runtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCUDO-MINIMAL
// CHECK-SCUDO-MINIMAL: "-fsanitize=scudo"
// CHECK-SCUDO-MINIMAL: "-fsanitize-minimal-runtime"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=undefined,scudo -fsanitize-minimal-runtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCUDO-UBSAN-MINIMAL
// CHECK-SCUDO-UBSAN-MINIMAL: "-fsanitize={{.*}}scudo"
// CHECK-SCUDO-UBSAN-MINIMAL: "-fsanitize-minimal-runtime"

// RUN: not %clang --target=powerpc-unknown-linux -fsanitize=scudo %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-NO-SCUDO
// CHECK-NO-SCUDO: unsupported option

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=scudo,address  %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCUDO-ASAN
// CHECK-SCUDO-ASAN: error: invalid argument '-fsanitize=scudo' not allowed with '-fsanitize=address'
// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=scudo,leak  %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCUDO-LSAN
// CHECK-SCUDO-LSAN: error: invalid argument '-fsanitize=scudo' not allowed with '-fsanitize=leak'
// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=scudo,memory  %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCUDO-MSAN
// CHECK-SCUDO-MSAN: error: invalid argument '-fsanitize=scudo' not allowed with '-fsanitize=memory'
// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=scudo,thread  %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCUDO-TSAN
// CHECK-SCUDO-TSAN: error: invalid argument '-fsanitize=scudo' not allowed with '-fsanitize=thread'
// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=scudo,hwaddress  %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCUDO-HWASAN
// CHECK-SCUDO-HWASAN: error: invalid argument '-fsanitize=scudo' not allowed with '-fsanitize=hwaddress'
//
// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=scudo,kernel-memory  %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SCUDO-KMSAN
// CHECK-SCUDO-KMSAN: error: invalid argument '-fsanitize=kernel-memory' not allowed with '-fsanitize=scudo'

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=hwaddress %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-HWASAN-INTERCEPTOR-ABI
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=hwaddress -fsanitize-hwaddress-abi=interceptor %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-HWASAN-INTERCEPTOR-ABI
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=hwaddress -fsanitize-hwaddress-abi=platform %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-HWASAN-PLATFORM-ABI
// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=hwaddress -fsanitize-hwaddress-abi=foo %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-HWASAN-FOO-ABI
// CHECK-HWASAN-INTERCEPTOR-ABI: "-default-function-attr" "hwasan-abi=interceptor"
// CHECK-HWASAN-PLATFORM-ABI: "-default-function-attr" "hwasan-abi=platform"
// CHECK-HWASAN-FOO-ABI: error: invalid value 'foo' in '-fsanitize-hwaddress-abi=foo'

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=hwaddress -fsanitize-hwaddress-experimental-aliasing %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-HWASAN-ALIAS
// CHECK-HWASAN-ALIAS: "-mllvm" "-hwasan-experimental-use-page-aliases=1"

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=address,pointer-compare,pointer-subtract %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-POINTER-ALL
// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=pointer-compare %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-POINTER-CMP-NEEDS-ADDRESS
// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=pointer-subtract %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-POINTER-SUB-NEEDS-ADDRESS
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=pointer-subtract -fno-sanitize=pointer-subtract %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-NO-POINTER-SUB
// RUN: %clang --target=x86_64-linux-gnu -fsanitize=pointer-compare -fno-sanitize=pointer-compare %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-NO-POINTER-CMP
// CHECK-POINTER-ALL: -cc1{{.*}}-fsanitize={{[^"]*}}pointer-compare,pointer-subtract{{.*}}" {{.*}} "-mllvm" "-asan-detect-invalid-pointer-cmp" {{.*}}"-mllvm" "-asan-detect-invalid-pointer-sub"
// CHECK-POINTER-CMP-NEEDS-ADDRESS: error: invalid argument '-fsanitize=pointer-compare' only allowed with '-fsanitize=address'
// CHECK-POINTER-SUB-NEEDS-ADDRESS: error: invalid argument '-fsanitize=pointer-subtract' only allowed with '-fsanitize=address'
// CHECK-NO-POINTER-SUB-NOT: {{.*}}asan-detect-invalid-pointer{{.*}}
// CHECK-NO-POINTER-CMP-NOT: {{.*}}asan-detect-invalid-pointer{{.*}}

// RUN: %clang -fsanitize=float-divide-by-zero %s -### 2>&1 | FileCheck %s --check-prefixes=CHECK-DIVBYZERO,CHECK-DIVBYZERO-RECOVER
// RUN: %clang -fsanitize=float-divide-by-zero %s -fno-sanitize-recover=float-divide-by-zero -### 2>&1 | FileCheck %s --check-prefixes=CHECK-DIVBYZERO,CHECK-DIVBYZERO-NORECOVER
// RUN: %clang -fsanitize=float-divide-by-zero -fsanitize-trap=float-divide-by-zero %s -### 2>&1 | FileCheck %s --check-prefixes=CHECK-DIVBYZERO,CHECK-DIVBYZERO-NORECOVER,CHECK-DIVBYZERO-TRAP
// CHECK-DIVBYZERO: "-fsanitize=float-divide-by-zero"
// CHECK-DIVBYZERO-RECOVER: "-fsanitize-recover=float-divide-by-zero"
// CHECK-DIVBYZERO-NORECOVER-NOT: "-fsanitize-recover=float-divide-by-zero"
// CHECK-DIVBYZERO-TRAP: "-fsanitize-trap=float-divide-by-zero"

// RUN: %clang -fsanitize=float-divide-by-zero -fsanitize-minimal-runtime %s -### 2>&1 | FileCheck %s --check-prefixes=CHECK-DIVBYZERO,CHECK-DIVBYZERO-MINIMAL
// CHECK-DIVBYZERO-MINIMAL: "-fsanitize-minimal-runtime"

// RUN: %clang -fsanitize=undefined,float-divide-by-zero %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-DIVBYZERO-UBSAN
// CHECK-DIVBYZERO-UBSAN: "-fsanitize={{.*}},float-divide-by-zero,{{.*}}"

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=undefined,function -mcmodel=large %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UBSAN-FUNCTION-CODE-MODEL
// CHECK-UBSAN-FUNCTION-CODE-MODEL: error: invalid argument '-fsanitize=function' only allowed with '-mcmodel=small'

// RUN: not %clang --target=x86_64-sie-ps5 -fsanitize=function %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UBSAN-FUNCTION-TARGET
// RUN: not %clang --target=x86_64-sie-ps5 -fsanitize=undefined -fsanitize=function %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UBSAN-FUNCTION-TARGET
// RUN: not %clang --target=x86_64-sie-ps5 -fsanitize=kcfi %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UBSAN-KCFI-TARGET
// RUN: not %clang --target=x86_64-sie-ps5 -fsanitize=function -fsanitize=kcfi %s -### 2>&1 | FileCheck %s  --check-prefix=CHECK-UBSAN-KCFI-TARGET --check-prefix=CHECK-UBSAN-FUNCTION-TARGET
// RUN: %clang --target=x86_64-sie-ps5 -fsanitize=undefined %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UBSAN-UNDEFINED

// RUN: not %clang --target=armv6t2-eabi -mexecute-only -fsanitize=function %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UBSAN-FUNCTION-MEXECUTE-ONLY
// RUN: not %clang --target=armv6t2-eabi -mpure-code -fsanitize=function %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UBSAN-FUNCTION-MPURE-CODE
// RUN: not %clang --target=armv6t2-eabi -mexecute-only -fsanitize=kcfi %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UBSAN-KCFI-MEXECUTE-ONLY
// RUN: not %clang --target=armv6t2-eabi -mpure-code -fsanitize=kcfi %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UBSAN-KCFI-MPURE-CODE
// RUN: %clang --target=armv6t2-eabi -mexecute-only -fsanitize=undefined %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UBSAN-UNDEFINED

// RUN: not %clang --target=aarch64 -mexecute-only -fsanitize=function %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UBSAN-FUNCTION-MEXECUTE-ONLY
// RUN: not %clang --target=aarch64 -mpure-code -fsanitize=function %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UBSAN-FUNCTION-MPURE-CODE
// RUN: not %clang --target=aarch64 -mexecute-only -fsanitize=kcfi %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UBSAN-KCFI-MEXECUTE-ONLY
// RUN: not %clang --target=aarch64 -mpure-code -fsanitize=kcfi %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UBSAN-KCFI-MPURE-CODE
// RUN: %clang --target=aarch64 -mexecute-only -fsanitize=undefined %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-UBSAN-UNDEFINED

// CHECK-UBSAN-KCFI-TARGET-DAG: error: unsupported option '-fsanitize=kcfi' for target 'x86_64-sie-ps5'
// CHECK-UBSAN-KCFI-MEXECUTE-ONLY-DAG: error: invalid argument '-fsanitize=kcfi' not allowed with '-mexecute-only'
// CHECK-UBSAN-KCFI-MPURE-CODE-DAG: error: invalid argument '-fsanitize=kcfi' not allowed with '-mpure-code'
// CHECK-UBSAN-FUNCTION-TARGET-DAG: error: unsupported option '-fsanitize=function' for target 'x86_64-sie-ps5'
// CHECK-UBSAN-FUNCTION-MEXECUTE-ONLY-DAG: error: invalid argument '-fsanitize=function' not allowed with '-mexecute-only'
// CHECK-UBSAN-FUNCTION-MPURE-CODE-DAG: error: invalid argument '-fsanitize=function' not allowed with '-mpure-code'
// CHECK-UBSAN-UNDEFINED: "-fsanitize={{((alignment|array-bounds|bool|builtin|enum|float-cast-overflow|integer-divide-by-zero|nonnull-attribute|null|pointer-overflow|return|returns-nonnull-attribute|shift-base|shift-exponent|signed-integer-overflow|unreachable|vla-bound),?){17}"}}

// * Test BareMetal toolchain sanitizer support *

// RUN: %clang --target=arm-arm-non-eabi -fsanitize=address %s -### 2>&1 | FileCheck %s -check-prefix=ADDRESS-BAREMETAL
// RUN: %clang --target=aarch64-none-elf -fsanitize=address %s -### 2>&1 | FileCheck %s -check-prefix=ADDRESS-BAREMETAL
// ADDRESS-BAREMETAL: "-fsanitize=address"

// RUN: %clang --target=arm-arm-none-eabi -fsanitize=kernel-address %s -### 2>&1 | FileCheck %s -check-prefix=KERNEL-ADDRESS-BAREMETAL
// RUN: %clang --target=aarch64-none-elf -fsanitize=kernel-address %s -### 2>&1 | FileCheck %s -check-prefix=KERNEL-ADDRESS-BAREMETAL
// KERNEL-ADDRESS-BAREMETAL: "-fsanitize=kernel-address"

// RUN: %clang --target=aarch64-none-elf -fsanitize=hwaddress %s -### 2>&1 | FileCheck %s -check-prefix=HWADDRESS-BAREMETAL
// HWADDRESS-BAREMETAL: "-fsanitize=hwaddress"

// RUN: %clang --target=aarch64-none-elf -fsanitize=kernel-hwaddress %s -### 2>&1 | FileCheck %s -check-prefix=KERNEL-HWADDRESS-BAREMETAL
// KERNEL-HWADDRESS-BAREMETAL: "-fsanitize=kernel-hwaddress"

// RUN: %clang --target=aarch64-none-elf -fsanitize=thread %s -### 2>&1 | FileCheck %s -check-prefix=THREAD-BAREMETAL
// THREAD-BAREMETAL: "-fsanitize=thread"

// RUN: %clang --target=arm-arm-none-eabi -fsanitize=vptr %s -### 2>&1 | FileCheck %s -check-prefix=VPTR-BAREMETAL
// RUN: %clang --target=aarch64-none-elf -fsanitize=vptr %s -### 2>&1 | FileCheck %s -check-prefix=VPTR-BAREMETAL
// VPTR-BAREMETAL: "-fsanitize=vptr"

// RUN: %clang --target=arm-arm-none-eabi -fsanitize=safe-stack %s -### 2>&1 | FileCheck %s -check-prefix=SAFESTACK-BAREMETAL
// RUN: %clang --target=aarch64-none-elf -fsanitize=safe-stack %s -### 2>&1 | FileCheck %s -check-prefix=SAFESTACK-BAREMETAL
// SAFESTACK-BAREMETAL: "-fsanitize=safe-stack"

// RUN: %clang --target=arm-arm-none-eabi -fsanitize=undefined %s -### 2>&1 | FileCheck %s -check-prefix=UNDEFINED-BAREMETAL
// RUN: %clang --target=aarch64-none-elf -fsanitize=undefined %s -### 2>&1 | FileCheck %s -check-prefix=UNDEFINED-BAREMETAL
// UNDEFINED-BAREMETAL: "-fsanitize={{.*}}

// RUN: %clang --target=arm-arm-none-eabi -fsanitize=scudo %s -### 2>&1 | FileCheck %s -check-prefix=SCUDO-BAREMETAL
// RUN: %clang --target=aarch64-none-elf -fsanitize=scudo %s -### 2>&1 | FileCheck %s -check-prefix=SCUDO-BAREMETAL
// SCUDO-BAREMETAL: "-fsanitize=scudo"

// RUN: %clang --target=arm-arm-none-eabi -fsanitize=function %s -### 2>&1 | FileCheck %s -check-prefix=FUNCTION-BAREMETAL
// RUN: %clang --target=aarch64-none-elf -fsanitize=function %s -### 2>&1 | FileCheck %s -check-prefix=FUNCTION-BAREMETAL
// FUNCTION-BAREMETAL: "-fsanitize=function"

// RUN: not %clang --target=aarch64-none-elf -fsanitize=memory %s -### 2>&1 | FileCheck %s -check-prefix=UNSUPPORTED-BAREMETAL
// RUN: not %clang --target=aarch64-none-elf -fsanitize=kernel-memory %s -### 2>&1 | FileCheck %s -check-prefix=UNSUPPORTED-BAREMETAL
// RUN: not %clang --target=aarch64-none-elf -fsanitize=leak %s -### 2>&1 | FileCheck %s -check-prefix=UNSUPPORTED-BAREMETAL
// RUN: not %clang --target=aarch64-none-elf -fsanitize=dataflow %s -### 2>&1 | FileCheck %s -check-prefix=UNSUPPORTED-BAREMETAL
// RUN: not %clang --target=arm-arm-none-eabi -fsanitize=shadow-call-stack %s -### 2>&1 | FileCheck %s -check-prefix=UNSUPPORTED-BAREMETAL
// UNSUPPORTED-BAREMETAL: unsupported option '-fsanitize={{.*}}' for target

// RUN: %clang --target=x86_64-apple-darwin -fsanitize=realtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-RTSAN-X86-64-DARWIN
// CHECK-RTSAN-X86-64-DARWIN-NOT: unsupported option

// RUN: %clang --target=x86_64-apple-darwin -fsanitize=realtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-RTSAN-X86-64-DARWIN
// CHECK-RTSAN-X86-64-DARWIN-NOT: unsupported option
// RUN: %clang --target=x86_64-apple-macos -fsanitize=realtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-RTSAN-X86-64-MACOS
// CHECK-RTSAN-X86-64-MACOS-NOT: unsupported option
// RUN: %clang --target=arm64-apple-macos -fsanitize=realtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-RTSAN-ARM64-MACOS
// CHECK-RTSAN-ARM64-MACOS-NOT: unsupported option

// RUN: %clang --target=arm64-apple-ios-simulator -fsanitize=realtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-RTSAN-ARM64-IOSSIMULATOR
// CHECK-RTSAN-ARM64-IOSSIMULATOR-NOT: unsupported option

// RUN: %clang --target=arm64-apple-watchos-simulator -fsanitize=realtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-RTSAN-ARM64-WATCHOSSIMULATOR
// CHECK-RTSAN-ARM64-WATCHOSSIMULATOR-NOT: unsupported option

// RUN: %clang --target=arm64-apple-tvos-simulator -fsanitize=realtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-RTSAN-ARM64-TVOSSIMULATOR
// CHECK-RTSAN-ARM64-TVOSSIMULATOR-NOT: unsupported option

// RUN: %clang --target=x86_64-apple-ios-simulator -fsanitize=realtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-RTSAN-X86-64-IOSSIMULATOR
// CHECK-RTSAN-X86-64-IOSSIMULATOR-NOT: unsupported option

// RUN: %clang --target=x86_64-apple-watchos-simulator -fsanitize=realtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-RTSAN-X86-64-WATCHOSSIMULATOR
// CHECK-RTSAN-X86-64-WATCHOSSIMULATOR-NOT: unsupported option

// RUN: %clang --target=x86_64-apple-tvos-simulator -fsanitize=realtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-RTSAN-X86-64-TVOSSIMULATOR
// CHECK-RTSAN-X86-64-TVOSSIMULATOR-NOT: unsupported option

// RUN: %clang --target=x86_64-linux-gnu -fsanitize=realtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-RTSAN-X86-64-LINUX
// CHECK-RTSAN-X86-64-LINUX-NOT: unsupported option

// RUN: not %clang --target=i386-pc-openbsd -fsanitize=realtime %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-RTSAN-OPENBSD
// CHECK-RTSAN-OPENBSD: unsupported option '-fsanitize=realtime' for target 'i386-pc-openbsd'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=realtime,thread  %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-REALTIME-TSAN
// CHECK-REALTIME-TSAN: error: invalid argument '-fsanitize=realtime' not allowed with '-fsanitize=thread'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=realtime,address  %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-REALTIME-ASAN
// CHECK-REALTIME-ASAN: error: invalid argument '-fsanitize=realtime' not allowed with '-fsanitize=address'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=realtime,memory  %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-REALTIME-MSAN
// CHECK-REALTIME-MSAN: error: invalid argument '-fsanitize=realtime' not allowed with '-fsanitize=memory'

// RUN: not %clang --target=x86_64-linux-gnu -fsanitize=realtime,undefined  %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-REALTIME-UBSAN
// CHECK-REALTIME-UBSAN: error: invalid argument '-fsanitize=realtime' not allowed with '-fsanitize=undefined'


// * Test -fsanitize-skip-hot-cutoff *

// -fsanitize-skip-hot-cutoff=undefined=0.5
// RUN: %clang -Werror --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-skip-hot-cutoff=undefined=0.5 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SKIP-HOT-CUTOFF1
// CHECK-SKIP-HOT-CUTOFF1: "-fsanitize-skip-hot-cutoff={{((signed-integer-overflow|integer-divide-by-zero|shift-base|shift-exponent|unreachable|return|vla-bound|alignment|null|pointer-overflow|float-cast-overflow|array-bounds|enum|bool|builtin|returns-nonnull-attribute|nonnull-attribute|function)=0.5(0*),?){18}"}}

// No-op: no sanitizers are specified
// RUN: %clang -Werror --target=x86_64-linux-gnu -fsanitize-skip-hot-cutoff=undefined=0.5 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SKIP-HOT-CUTOFF2
// CHECK-SKIP-HOT-CUTOFF2-NOT: "-fsanitize"
// CHECK-SKIP-HOT-CUTOFF2-NOT: "-fsanitize-skip-hot-cutoff"

// Enable undefined, then cancel out integer using a cutoff of 0.0
// RUN: %clang -Werror --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-skip-hot-cutoff=undefined=0.5,integer=0.0 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SKIP-HOT-CUTOFF3
// CHECK-SKIP-HOT-CUTOFF3: "-fsanitize-skip-hot-cutoff={{((unreachable|return|vla-bound|alignment|null|pointer-overflow|float-cast-overflow|array-bounds|enum|bool|builtin|returns-nonnull-attribute|nonnull-attribute|function)=0.5(0*),?){14}"}}

// Enable undefined, then cancel out integer using a cutoff of 0.0, then re-enable signed-integer-overflow
// RUN: %clang -Werror --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-skip-hot-cutoff=undefined=0.5,integer=0.0,signed-integer-overflow=0.7 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SKIP-HOT-CUTOFF4
// CHECK-SKIP-HOT-CUTOFF4: "-fsanitize-skip-hot-cutoff={{((signed-integer-overflow|unreachable|return|vla-bound|alignment|null|pointer-overflow|float-cast-overflow|array-bounds|enum|bool|builtin|returns-nonnull-attribute|nonnull-attribute|function)=0.[57]0*,?){15}"}}

// Check that -fsanitize-skip-hot-cutoff=undefined=0.4 does not widen the set of -fsanitize=integer checks.
// RUN: %clang -Werror --target=x86_64-linux-gnu -fsanitize=integer -fsanitize-skip-hot-cutoff=undefined=0.4 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SKIP-HOT-CUTOFF5
// CHECK-SKIP-HOT-CUTOFF5: "-fsanitize-skip-hot-cutoff={{((integer-divide-by-zero|shift-base|shift-exponent|signed-integer-overflow)=0.40*,?){4}"}}

// No-op: it's allowed for the user to specify a cutoff of 0.0, though the argument is not passed along by the driver.
// RUN: %clang -Werror --target=x86_64-linux-gnu -fsanitize=undefined -fsanitize-skip-hot-cutoff=undefined=0.0 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SKIP-HOT-CUTOFF6
// CHECK-SKIP-HOT-CUTOFF6: "-fsanitize={{((signed-integer-overflow|integer-divide-by-zero|shift-base|shift-exponent|unreachable|return|vla-bound|alignment|null|pointer-overflow|float-cast-overflow|array-bounds|enum|bool|builtin|returns-nonnull-attribute|nonnull-attribute|function),?){18}"}}
// CHECK-SKIP-HOT-CUTOFF6-NOT: "-fsanitize-skip-hot-cutoff"

// Invalid: bad sanitizer
// RUN: not %clang --target=x86_64-linux-gnu -fsanitize-skip-hot-cutoff=pot=0.0 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SKIP-HOT-CUTOFF7
// CHECK-SKIP-HOT-CUTOFF7: unsupported argument 'pot=0.0' to option '-fsanitize-skip-hot-cutoff='

// Invalid: bad cutoff
// RUN: not %clang --target=x86_64-linux-gnu -fsanitize-skip-hot-cutoff=undefined=xyzzy %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SKIP-HOT-CUTOFF8
// CHECK-SKIP-HOT-CUTOFF8: unsupported argument 'undefined=xyzzy' to option '-fsanitize-skip-hot-cutoff='

// Invalid: -fsanitize-skip-hot-cutoff without parameters
// RUN: not %clang --target=x86_64-linux-gnu -fsanitize-skip-hot-cutoff %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SKIP-HOT-CUTOFF9
// CHECK-SKIP-HOT-CUTOFF9: unknown argument: '-fsanitize-skip-hot-cutoff'

// Invalid: -fsanitize-skip-hot-cutoff=undefined without cutoff
// RUN: not %clang --target=x86_64-linux-gnu -fsanitize-skip-hot-cutoff=undefined %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SKIP-HOT-CUTOFF10
// CHECK-SKIP-HOT-CUTOFF10: unsupported argument 'undefined' to option '-fsanitize-skip-hot-cutoff='

// Invalid: -fsanitize-skip-hot-cutoff=undefined= without cutoff
// RUN: not %clang --target=x86_64-linux-gnu -fsanitize-skip-hot-cutoff=undefined= %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SKIP-HOT-CUTOFF11
// CHECK-SKIP-HOT-CUTOFF11: unsupported argument 'undefined=' to option '-fsanitize-skip-hot-cutoff='

// No-op: -fsanitize-skip-hot-cutoff= without parameters is unusual but valid
// RUN: %clang -Werror --target=x86_64-linux-gnu -fsanitize-skip-hot-cutoff= %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SKIP-HOT-CUTOFF12
// CHECK-SKIP-HOT-CUTOFF12-NOT: "-fsanitize-skip-hot-cutoff"

// Invalid: out of range cutoff
// RUN: not %clang --target=x86_64-linux-gnu -fsanitize-skip-hot-cutoff=undefined=1.1 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SKIP-HOT-CUTOFF13
// CHECK-SKIP-HOT-CUTOFF13: unsupported argument 'undefined=1.1' to option '-fsanitize-skip-hot-cutoff='

// Invalid: out of range cutoff
// RUN: not %clang --target=x86_64-linux-gnu -fsanitize-skip-hot-cutoff=undefined=-0.1 %s -### 2>&1 | FileCheck %s --check-prefix=CHECK-SKIP-HOT-CUTOFF14
// CHECK-SKIP-HOT-CUTOFF14: unsupported argument 'undefined=-0.1' to option '-fsanitize-skip-hot-cutoff='

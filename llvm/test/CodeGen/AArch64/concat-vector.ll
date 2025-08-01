; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -mtriple=aarch64 %s -o - | FileCheck %s --check-prefixes=CHECK,CHECK-SD
; RUN: llc -mtriple=aarch64 -global-isel %s -o - | FileCheck %s --check-prefixes=CHECK,CHECK-GI

define <4 x i8> @concat1(<2 x i8> %A, <2 x i8> %B) {
; CHECK-SD-LABEL: concat1:
; CHECK-SD:       // %bb.0:
; CHECK-SD-NEXT:    uzp1 v0.4h, v0.4h, v1.4h
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: concat1:
; CHECK-GI:       // %bb.0:
; CHECK-GI-NEXT:    // kill: def $d0 killed $d0 def $q0
; CHECK-GI-NEXT:    mov w8, v0.s[1]
; CHECK-GI-NEXT:    // kill: def $d1 killed $d1 def $q1
; CHECK-GI-NEXT:    mov w9, v1.s[1]
; CHECK-GI-NEXT:    mov v0.h[1], w8
; CHECK-GI-NEXT:    fmov w8, s1
; CHECK-GI-NEXT:    mov v0.h[2], w8
; CHECK-GI-NEXT:    mov v0.h[3], w9
; CHECK-GI-NEXT:    // kill: def $d0 killed $d0 killed $q0
; CHECK-GI-NEXT:    ret
   %v4i8 = shufflevector <2 x i8> %A, <2 x i8> %B, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
   ret <4 x i8> %v4i8
}

define <8 x i8> @concat2(<4 x i8> %A, <4 x i8> %B) {
; CHECK-SD-LABEL: concat2:
; CHECK-SD:       // %bb.0:
; CHECK-SD-NEXT:    uzp1 v0.8b, v0.8b, v1.8b
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: concat2:
; CHECK-GI:       // %bb.0:
; CHECK-GI-NEXT:    uzp1 v1.8b, v1.8b, v0.8b
; CHECK-GI-NEXT:    uzp1 v0.8b, v0.8b, v0.8b
; CHECK-GI-NEXT:    fmov w8, s1
; CHECK-GI-NEXT:    mov v0.s[1], w8
; CHECK-GI-NEXT:    // kill: def $d0 killed $d0 killed $q0
; CHECK-GI-NEXT:    ret
   %v8i8 = shufflevector <4 x i8> %A, <4 x i8> %B, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
   ret <8 x i8> %v8i8
}

define <16 x i8> @concat3(<8 x i8> %A, <8 x i8> %B) {
; CHECK-LABEL: concat3:
; CHECK:       // %bb.0:
; CHECK-NEXT:    // kill: def $d0 killed $d0 def $q0
; CHECK-NEXT:    // kill: def $d1 killed $d1 def $q1
; CHECK-NEXT:    mov v0.d[1], v1.d[0]
; CHECK-NEXT:    ret
   %v16i8 = shufflevector <8 x i8> %A, <8 x i8> %B, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
   ret <16 x i8> %v16i8
}

define <4 x i16> @concat4(<2 x i16> %A, <2 x i16> %B) {
; CHECK-SD-LABEL: concat4:
; CHECK-SD:       // %bb.0:
; CHECK-SD-NEXT:    uzp1 v0.4h, v0.4h, v1.4h
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: concat4:
; CHECK-GI:       // %bb.0:
; CHECK-GI-NEXT:    uzp1 v1.4h, v1.4h, v0.4h
; CHECK-GI-NEXT:    uzp1 v0.4h, v0.4h, v0.4h
; CHECK-GI-NEXT:    fmov w8, s1
; CHECK-GI-NEXT:    mov v0.s[1], w8
; CHECK-GI-NEXT:    // kill: def $d0 killed $d0 killed $q0
; CHECK-GI-NEXT:    ret
   %v4i16 = shufflevector <2 x i16> %A, <2 x i16> %B, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
   ret <4 x i16> %v4i16
}

define <8 x i16> @concat5(<4 x i16> %A, <4 x i16> %B) {
; CHECK-LABEL: concat5:
; CHECK:       // %bb.0:
; CHECK-NEXT:    // kill: def $d0 killed $d0 def $q0
; CHECK-NEXT:    // kill: def $d1 killed $d1 def $q1
; CHECK-NEXT:    mov v0.d[1], v1.d[0]
; CHECK-NEXT:    ret
   %v8i16 = shufflevector <4 x i16> %A, <4 x i16> %B, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
   ret <8 x i16> %v8i16
}

define <16 x i16> @concat6(ptr %A, ptr %B) {
; CHECK-LABEL: concat6:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldr q0, [x0]
; CHECK-NEXT:    ldr q1, [x1]
; CHECK-NEXT:    ret
   %tmp1 = load <8 x i16>, ptr %A
   %tmp2 = load <8 x i16>, ptr %B
   %v16i16 = shufflevector <8 x i16> %tmp1, <8 x i16> %tmp2, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
   ret <16 x i16> %v16i16
}

define <4 x i32> @concat7(<2 x i32> %A, <2 x i32> %B) {
; CHECK-LABEL: concat7:
; CHECK:       // %bb.0:
; CHECK-NEXT:    // kill: def $d0 killed $d0 def $q0
; CHECK-NEXT:    // kill: def $d1 killed $d1 def $q1
; CHECK-NEXT:    mov v0.d[1], v1.d[0]
; CHECK-NEXT:    ret
   %v4i32 = shufflevector <2 x i32> %A, <2 x i32> %B, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
   ret <4 x i32> %v4i32
}

define <8 x i32> @concat8(ptr %A, ptr %B) {
; CHECK-LABEL: concat8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldr q0, [x0]
; CHECK-NEXT:    ldr q1, [x1]
; CHECK-NEXT:    ret
   %tmp1 = load <4 x i32>, ptr %A
   %tmp2 = load <4 x i32>, ptr %B
   %v8i32 = shufflevector <4 x i32> %tmp1, <4 x i32> %tmp2, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
   ret <8 x i32> %v8i32
}

define <4 x half> @concat9(<2 x half> %A, <2 x half> %B) {
; CHECK-SD-LABEL: concat9:
; CHECK-SD:       // %bb.0:
; CHECK-SD-NEXT:    zip1 v0.2s, v0.2s, v1.2s
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: concat9:
; CHECK-GI:       // %bb.0:
; CHECK-GI-NEXT:    fmov w8, s1
; CHECK-GI-NEXT:    // kill: def $d0 killed $d0 def $q0
; CHECK-GI-NEXT:    mov v0.s[1], w8
; CHECK-GI-NEXT:    // kill: def $d0 killed $d0 killed $q0
; CHECK-GI-NEXT:    ret
   %v4half= shufflevector <2 x half> %A, <2 x half> %B, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
   ret <4 x half> %v4half
}

define <8 x half> @concat10(<4 x half> %A, <4 x half> %B) {
; CHECK-LABEL: concat10:
; CHECK:       // %bb.0:
; CHECK-NEXT:    // kill: def $d0 killed $d0 def $q0
; CHECK-NEXT:    // kill: def $d1 killed $d1 def $q1
; CHECK-NEXT:    mov v0.d[1], v1.d[0]
; CHECK-NEXT:    ret
   %v8half= shufflevector <4 x half> %A, <4 x half> %B, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
   ret <8 x half> %v8half
}

define <16 x half> @concat11(<8 x half> %A, <8 x half> %B) {
; CHECK-LABEL: concat11:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ret
   %v16half= shufflevector <8 x half> %A, <8 x half> %B, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
   ret <16 x half> %v16half
}

define <8 x i16> @concat_v8s16_v2s16(ptr %ptr) {
; CHECK-SD-LABEL: concat_v8s16_v2s16:
; CHECK-SD:       // %bb.0:
; CHECK-SD-NEXT:    ldr s0, [x0]
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: concat_v8s16_v2s16:
; CHECK-GI:       // %bb.0:
; CHECK-GI-NEXT:    ldrh w8, [x0]
; CHECK-GI-NEXT:    ldrh w9, [x0, #2]
; CHECK-GI-NEXT:    fmov s0, w8
; CHECK-GI-NEXT:    mov v0.h[1], w9
; CHECK-GI-NEXT:    ret
    %a = load <2 x i16>, ptr %ptr
    %b = shufflevector <2 x i16> %a, <2 x i16> %a, <8 x i32> <i32 0, i32 1, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef>
    ret <8 x i16> %b
}

define <16 x i8> @concat_v16s8_v4s8(ptr %ptr) {
; CHECK-LABEL: concat_v16s8_v4s8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldr s0, [x0]
; CHECK-NEXT:    ret
    %a = load <4 x i8>, ptr %ptr
    %b = shufflevector <4 x i8> %a, <4 x i8> %a, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef>
    ret <16 x i8> %b
}

define <16 x i8> @concat_v16s8_v4s8_load(ptr %ptrA, ptr %ptrB, ptr %ptrC, ptr %ptrD) {
; CHECK-LABEL: concat_v16s8_v4s8_load:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldr s0, [x0]
; CHECK-NEXT:    ld1 { v0.s }[1], [x1]
; CHECK-NEXT:    ld1 { v0.s }[2], [x2]
; CHECK-NEXT:    ld1 { v0.s }[3], [x3]
; CHECK-NEXT:    ret
    %A = load <4 x i8>, ptr %ptrA
    %B = load <4 x i8>, ptr %ptrB
    %C = load <4 x i8>, ptr %ptrC
    %D = load <4 x i8>, ptr %ptrD
    %b = shufflevector <4 x i8> %A, <4 x i8> %B, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef>
    %c = shufflevector <4 x i8> %C, <4 x i8> %D, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef>
    %d = shufflevector <16 x i8> %b, <16 x i8> %c, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 16, i32 17, i32 18, i32 19, i32 20, i32 21, i32 22, i32 23>
    ret <16 x i8> %d
}


define <16 x i8> @concat_v16s8_v4s8_reg(<4 x i8> %A, <4 x i8> %B, <4 x i8> %C, <4 x i8> %D) {
; CHECK-SD-LABEL: concat_v16s8_v4s8_reg:
; CHECK-SD:       // %bb.0:
; CHECK-SD-NEXT:    // kill: def $d2 killed $d2 def $q2
; CHECK-SD-NEXT:    // kill: def $d0 killed $d0 def $q0
; CHECK-SD-NEXT:    // kill: def $d3 killed $d3 def $q3
; CHECK-SD-NEXT:    // kill: def $d1 killed $d1 def $q1
; CHECK-SD-NEXT:    mov v2.d[1], v3.d[0]
; CHECK-SD-NEXT:    mov v0.d[1], v1.d[0]
; CHECK-SD-NEXT:    uzp1 v0.16b, v0.16b, v2.16b
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: concat_v16s8_v4s8_reg:
; CHECK-GI:       // %bb.0:
; CHECK-GI-NEXT:    uzp1 v1.8b, v1.8b, v0.8b
; CHECK-GI-NEXT:    uzp1 v0.8b, v0.8b, v0.8b
; CHECK-GI-NEXT:    fmov w8, s1
; CHECK-GI-NEXT:    uzp1 v2.8b, v2.8b, v0.8b
; CHECK-GI-NEXT:    mov v0.s[1], w8
; CHECK-GI-NEXT:    fmov w8, s2
; CHECK-GI-NEXT:    uzp1 v1.8b, v3.8b, v0.8b
; CHECK-GI-NEXT:    mov v0.s[2], w8
; CHECK-GI-NEXT:    fmov w8, s1
; CHECK-GI-NEXT:    mov v0.s[3], w8
; CHECK-GI-NEXT:    ret
    %b = shufflevector <4 x i8> %A, <4 x i8> %B, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef>
    %c = shufflevector <4 x i8> %C, <4 x i8> %D, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef>
    %d = shufflevector <16 x i8> %b, <16 x i8> %c, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 16, i32 17, i32 18, i32 19, i32 20, i32 21, i32 22, i32 23>
    ret <16 x i8> %d
}

define <8 x i16> @concat_v8s16_v2s16_reg(<2 x i16> %A, <2 x i16> %B, <2 x i16> %C, <2 x i16> %D) {
; CHECK-SD-LABEL: concat_v8s16_v2s16_reg:
; CHECK-SD:       // %bb.0:
; CHECK-SD-NEXT:    // kill: def $d3 killed $d3 killed $q0_q1_q2_q3 def $q0_q1_q2_q3
; CHECK-SD-NEXT:    adrp x8, .LCPI15_0
; CHECK-SD-NEXT:    // kill: def $d2 killed $d2 killed $q0_q1_q2_q3 def $q0_q1_q2_q3
; CHECK-SD-NEXT:    ldr q4, [x8, :lo12:.LCPI15_0]
; CHECK-SD-NEXT:    // kill: def $d1 killed $d1 killed $q0_q1_q2_q3 def $q0_q1_q2_q3
; CHECK-SD-NEXT:    // kill: def $d0 killed $d0 killed $q0_q1_q2_q3 def $q0_q1_q2_q3
; CHECK-SD-NEXT:    tbl v0.16b, { v0.16b, v1.16b, v2.16b, v3.16b }, v4.16b
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: concat_v8s16_v2s16_reg:
; CHECK-GI:       // %bb.0:
; CHECK-GI-NEXT:    uzp1 v1.4h, v1.4h, v0.4h
; CHECK-GI-NEXT:    uzp1 v0.4h, v0.4h, v0.4h
; CHECK-GI-NEXT:    fmov w8, s1
; CHECK-GI-NEXT:    uzp1 v2.4h, v2.4h, v0.4h
; CHECK-GI-NEXT:    mov v0.s[1], w8
; CHECK-GI-NEXT:    fmov w8, s2
; CHECK-GI-NEXT:    uzp1 v1.4h, v3.4h, v0.4h
; CHECK-GI-NEXT:    mov v0.s[2], w8
; CHECK-GI-NEXT:    fmov w8, s1
; CHECK-GI-NEXT:    mov v0.s[3], w8
; CHECK-GI-NEXT:    ret
    %b = shufflevector <2 x i16> %A, <2 x i16> %B, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 undef, i32 undef, i32 undef, i32 undef>
    %c = shufflevector <2 x i16> %C, <2 x i16> %D, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 undef, i32 undef, i32 undef, i32 undef>
    %d = shufflevector <8 x i16> %b, <8 x i16> %c, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 8, i32 9, i32 10, i32 11>
    ret <8 x i16> %d
}

define <4 x i16> @concat_undef_first_use_first(ptr %p1, ptr %p2) {
; CHECK-SD-LABEL: concat_undef_first_use_first:
; CHECK-SD:       // %bb.0:
; CHECK-SD-NEXT:    ld1r { v0.2s }, [x0]
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: concat_undef_first_use_first:
; CHECK-GI:       // %bb.0:
; CHECK-GI-NEXT:    ldrh w8, [x0]
; CHECK-GI-NEXT:    ldrh w9, [x0, #2]
; CHECK-GI-NEXT:    fmov s1, w8
; CHECK-GI-NEXT:    mov v1.h[1], w9
; CHECK-GI-NEXT:    mov v0.s[1], v1.s[0]
; CHECK-GI-NEXT:    // kill: def $d0 killed $d0 killed $q0
; CHECK-GI-NEXT:    ret
  %l1 = load <2 x i16>, ptr %p1
  %l2 = load <2 x i16>, ptr %p2
  %ext1 = shufflevector <2 x i16> %l1, <2 x i16> poison, <8 x i32> <i32 0, i32 1, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison>
  %ext2 = shufflevector <2 x i16> %l1, <2 x i16> poison, <8 x i32> <i32 0, i32 1, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison>
  %t = shufflevector <8 x i16> %ext1, <8 x i16> %ext2, <4 x i32> <i32 poison, i32 poison, i32 0, i32 1>
  ret <4 x i16> %t
}

define <4 x i16> @concat_undef_first_use_second(ptr %p1, ptr %p2) {
; CHECK-SD-LABEL: concat_undef_first_use_second:
; CHECK-SD:       // %bb.0:
; CHECK-SD-NEXT:    ld1r { v0.2s }, [x0]
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: concat_undef_first_use_second:
; CHECK-GI:       // %bb.0:
; CHECK-GI-NEXT:    ldrh w8, [x0]
; CHECK-GI-NEXT:    ldrh w9, [x0, #2]
; CHECK-GI-NEXT:    fmov s1, w8
; CHECK-GI-NEXT:    mov v1.h[1], w9
; CHECK-GI-NEXT:    mov v0.s[1], v1.s[0]
; CHECK-GI-NEXT:    // kill: def $d0 killed $d0 killed $q0
; CHECK-GI-NEXT:    ret
  %l1 = load <2 x i16>, ptr %p1
  %l2 = load <2 x i16>, ptr %p2
  %ext1 = shufflevector <2 x i16> %l1, <2 x i16> poison, <8 x i32> <i32 0, i32 1, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison>
  %ext2 = shufflevector <2 x i16> %l1, <2 x i16> poison, <8 x i32> <i32 0, i32 1, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison>
  %t = shufflevector <8 x i16> %ext1, <8 x i16> %ext2, <4 x i32> <i32 poison, i32 poison, i32 8, i32 9>
  ret <4 x i16> %t
}

define <4 x i16> @concat_undef_first_use_undef(ptr %p1, ptr %p2) {
; CHECK-SD-LABEL: concat_undef_first_use_undef:
; CHECK-SD:       // %bb.0:
; CHECK-SD-NEXT:    ldr s0, [x0]
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: concat_undef_first_use_undef:
; CHECK-GI:       // %bb.0:
; CHECK-GI-NEXT:    ret
  %l1 = load <2 x i16>, ptr %p1
  %l2 = load <2 x i16>, ptr %p2
  %ext1 = shufflevector <2 x i16> %l1, <2 x i16> poison, <8 x i32> <i32 0, i32 1, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison>
  %ext2 = shufflevector <2 x i16> %l1, <2 x i16> poison, <8 x i32> <i32 0, i32 1, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison>
  %t = shufflevector <8 x i16> %ext1, <8 x i16> %ext2, <4 x i32> <i32 poison, i32 poison, i32 2, i32 3>
  ret <4 x i16> %t
}

define <8 x i16> @concat_low_low_v8i16(<8 x i16> %a_vec, <8 x i16> %b_vec) {
; CHECK-LABEL: concat_low_low_v8i16:
; CHECK:       // %bb.0: // %entry
; CHECK-NEXT:    mov v0.d[1], v1.d[0]
; CHECK-NEXT:    ret
entry:
  %shuffle.i3 = shufflevector <8 x i16> %a_vec, <8 x i16> poison, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  %shuffle.i = shufflevector <8 x i16> %b_vec, <8 x i16> poison, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  %shuffle.i4 = shufflevector <4 x i16> %shuffle.i3, <4 x i16> %shuffle.i, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  ret <8 x i16> %shuffle.i4
}

define <8 x i16> @concat_high_low_v8i16(<8 x i16> %a_vec, <8 x i16> %b_vec) {
; CHECK-SD-LABEL: concat_high_low_v8i16:
; CHECK-SD:       // %bb.0: // %entry
; CHECK-SD-NEXT:    ext v0.16b, v0.16b, v1.16b, #8
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: concat_high_low_v8i16:
; CHECK-GI:       // %bb.0: // %entry
; CHECK-GI-NEXT:    mov d0, v0.d[1]
; CHECK-GI-NEXT:    mov v0.d[1], v1.d[0]
; CHECK-GI-NEXT:    ret
entry:
  %shuffle.i3 = shufflevector <8 x i16> %a_vec, <8 x i16> poison, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  %shuffle.i = shufflevector <8 x i16> %b_vec, <8 x i16> poison, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  %shuffle.i4 = shufflevector <4 x i16> %shuffle.i3, <4 x i16> %shuffle.i, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  ret <8 x i16> %shuffle.i4
}

define <8 x i16> @concat_low_high_v8i16(<8 x i16> %a_vec, <8 x i16> %b_vec) {
; CHECK-SD-LABEL: concat_low_high_v8i16:
; CHECK-SD:       // %bb.0: // %entry
; CHECK-SD-NEXT:    ext v1.16b, v1.16b, v1.16b, #8
; CHECK-SD-NEXT:    mov v0.d[1], v1.d[0]
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: concat_low_high_v8i16:
; CHECK-GI:       // %bb.0: // %entry
; CHECK-GI-NEXT:    mov d1, v1.d[1]
; CHECK-GI-NEXT:    mov v0.d[1], v1.d[0]
; CHECK-GI-NEXT:    ret
entry:
  %shuffle.i3 = shufflevector <8 x i16> %a_vec, <8 x i16> poison, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  %shuffle.i = shufflevector <8 x i16> %b_vec, <8 x i16> poison, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  %shuffle.i4 = shufflevector <4 x i16> %shuffle.i3, <4 x i16> %shuffle.i, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  ret <8 x i16> %shuffle.i4
}

define <8 x i16> @concat_high_high_v8i16(<8 x i16> %a_vec, <8 x i16> %b_vec) {
; CHECK-LABEL: concat_high_high_v8i16:
; CHECK:       // %bb.0: // %entry
; CHECK-NEXT:    mov v1.d[0], v0.d[1]
; CHECK-NEXT:    mov v0.16b, v1.16b
; CHECK-NEXT:    ret
entry:
  %shuffle.i3 = shufflevector <8 x i16> %a_vec, <8 x i16> poison, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  %shuffle.i = shufflevector <8 x i16> %b_vec, <8 x i16> poison, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  %shuffle.i4 = shufflevector <4 x i16> %shuffle.i3, <4 x i16> %shuffle.i, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  ret <8 x i16> %shuffle.i4
}

define <8 x half> @concat_high_high_v8f16(<8 x half> %a_vec, <8 x half> %b_vec) {
; CHECK-LABEL: concat_high_high_v8f16:
; CHECK:       // %bb.0: // %entry
; CHECK-NEXT:    mov v1.d[0], v0.d[1]
; CHECK-NEXT:    mov v0.16b, v1.16b
; CHECK-NEXT:    ret
entry:
  %shuffle.i3 = shufflevector <8 x half> %a_vec, <8 x half> poison, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  %shuffle.i = shufflevector <8 x half> %b_vec, <8 x half> poison, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  %shuffle.i4 = shufflevector <4 x half> %shuffle.i3, <4 x half> %shuffle.i, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  ret <8 x half> %shuffle.i4
}

define <8 x bfloat> @concat_high_high_v8bf16(<8 x bfloat> %a_vec, <8 x bfloat> %b_vec) {
; CHECK-LABEL: concat_high_high_v8bf16:
; CHECK:       // %bb.0: // %entry
; CHECK-NEXT:    mov v1.d[0], v0.d[1]
; CHECK-NEXT:    mov v0.16b, v1.16b
; CHECK-NEXT:    ret
entry:
  %shuffle.i3 = shufflevector <8 x bfloat> %a_vec, <8 x bfloat> poison, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  %shuffle.i = shufflevector <8 x bfloat> %b_vec, <8 x bfloat> poison, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  %shuffle.i4 = shufflevector <4 x bfloat> %shuffle.i3, <4 x bfloat> %shuffle.i, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  ret <8 x bfloat> %shuffle.i4
}

define <4 x i32> @concat_high_high_v4i32(<4 x i32> %a_vec, <4 x i32> %b_vec) {
; CHECK-SD-LABEL: concat_high_high_v4i32:
; CHECK-SD:       // %bb.0: // %entry
; CHECK-SD-NEXT:    zip2 v0.2d, v0.2d, v1.2d
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: concat_high_high_v4i32:
; CHECK-GI:       // %bb.0: // %entry
; CHECK-GI-NEXT:    mov v1.d[0], v0.d[1]
; CHECK-GI-NEXT:    mov v0.16b, v1.16b
; CHECK-GI-NEXT:    ret
entry:
  %shuffle.i3 = shufflevector <4 x i32> %a_vec, <4 x i32> poison, <2 x i32> <i32 2, i32 3>
  %shuffle.i = shufflevector <4 x i32> %b_vec, <4 x i32> poison, <2 x i32> <i32 2, i32 3>
  %shuffle.i4 = shufflevector <2 x i32> %shuffle.i3, <2 x i32> %shuffle.i, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  ret <4 x i32> %shuffle.i4
}

define <4 x float> @concat_high_high_v4f32(<4 x float> %a_vec, <4 x float> %b_vec) {
; CHECK-SD-LABEL: concat_high_high_v4f32:
; CHECK-SD:       // %bb.0: // %entry
; CHECK-SD-NEXT:    zip2 v0.2d, v0.2d, v1.2d
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: concat_high_high_v4f32:
; CHECK-GI:       // %bb.0: // %entry
; CHECK-GI-NEXT:    mov v1.d[0], v0.d[1]
; CHECK-GI-NEXT:    mov v0.16b, v1.16b
; CHECK-GI-NEXT:    ret
entry:
  %shuffle.i3 = shufflevector <4 x float> %a_vec, <4 x float> poison, <2 x i32> <i32 2, i32 3>
  %shuffle.i = shufflevector <4 x float> %b_vec, <4 x float> poison, <2 x i32> <i32 2, i32 3>
  %shuffle.i4 = shufflevector <2 x float> %shuffle.i3, <2 x float> %shuffle.i, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  ret <4 x float> %shuffle.i4
}

define <16 x i8> @concat_high_high_v16i8(<16 x i8> %a_vec, <16 x i8> %b_vec) {
; CHECK-LABEL: concat_high_high_v16i8:
; CHECK:       // %bb.0: // %entry
; CHECK-NEXT:    mov v1.d[0], v0.d[1]
; CHECK-NEXT:    mov v0.16b, v1.16b
; CHECK-NEXT:    ret
entry:
  %shuffle.i3 = shufflevector <16 x i8> %a_vec, <16 x i8> poison, <8 x i32> <i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
  %shuffle.i = shufflevector <16 x i8> %b_vec, <16 x i8> poison, <8 x i32> <i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
  %shuffle.i4 = shufflevector <8 x i8> %shuffle.i3, <8 x i8> %shuffle.i, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
  ret <16 x i8> %shuffle.i4
}

define <2 x i64> @concat_high_high_v2i64(<2 x i64> %a_vec, <2 x i64> %b_vec) {
; CHECK-SD-LABEL: concat_high_high_v2i64:
; CHECK-SD:       // %bb.0: // %entry
; CHECK-SD-NEXT:    zip2 v0.2d, v0.2d, v1.2d
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: concat_high_high_v2i64:
; CHECK-GI:       // %bb.0: // %entry
; CHECK-GI-NEXT:    mov v0.d[0], v0.d[1]
; CHECK-GI-NEXT:    mov v0.d[1], v1.d[1]
; CHECK-GI-NEXT:    ret
entry:
  %shuffle.i3 = shufflevector <2 x i64> %a_vec, <2 x i64> poison, <1 x i32> <i32 1>
  %shuffle.i = shufflevector <2 x i64> %b_vec, <2 x i64> poison, <1 x i32> <i32 1>
  %shuffle.i4 = shufflevector <1 x i64> %shuffle.i3, <1 x i64> %shuffle.i, <2 x i32> <i32 0, i32 1>
  ret <2 x i64> %shuffle.i4
}

define <2 x double> @concat_high_high_v2f64(<2 x double> %a_vec, <2 x double> %b_vec) {
; CHECK-SD-LABEL: concat_high_high_v2f64:
; CHECK-SD:       // %bb.0: // %entry
; CHECK-SD-NEXT:    zip2 v0.2d, v0.2d, v1.2d
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: concat_high_high_v2f64:
; CHECK-GI:       // %bb.0: // %entry
; CHECK-GI-NEXT:    mov v0.d[0], v0.d[1]
; CHECK-GI-NEXT:    mov v0.d[1], v1.d[1]
; CHECK-GI-NEXT:    ret
entry:
  %shuffle.i3 = shufflevector <2 x double> %a_vec, <2 x double> poison, <1 x i32> <i32 1>
  %shuffle.i = shufflevector <2 x double> %b_vec, <2 x double> poison, <1 x i32> <i32 1>
  %shuffle.i4 = shufflevector <1 x double> %shuffle.i3, <1 x double> %shuffle.i, <2 x i32> <i32 0, i32 1>
  ret <2 x double> %shuffle.i4
}

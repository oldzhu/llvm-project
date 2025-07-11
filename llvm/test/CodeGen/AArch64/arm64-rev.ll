; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=aarch64 -aarch64-neon-syntax=apple | FileCheck %s --check-prefixes=CHECK,CHECK-SD
; RUN: llc < %s -mtriple=aarch64 -aarch64-neon-syntax=apple -global-isel | FileCheck %s --check-prefixes=CHECK,CHECK-GI

define i32 @test_rev_w(i32 %a) nounwind {
; CHECK-LABEL: test_rev_w:
; CHECK:       // %bb.0: // %entry
; CHECK-NEXT:    rev w0, w0
; CHECK-NEXT:    ret
entry:
  %0 = tail call i32 @llvm.bswap.i32(i32 %a)
  ret i32 %0
}

define i64 @test_rev_x(i64 %a) nounwind {
; CHECK-LABEL: test_rev_x:
; CHECK:       // %bb.0: // %entry
; CHECK-NEXT:    rev x0, x0
; CHECK-NEXT:    ret
entry:
  %0 = tail call i64 @llvm.bswap.i64(i64 %a)
  ret i64 %0
}

; Canonicalize (srl (bswap x), 16) to (rotr (bswap x), 16) if the high 16-bits
; of %a are zero. This optimizes rev + lsr 16 to rev16.
define i32 @test_rev_w_srl16(i16 %a) {
; CHECK-SD-LABEL: test_rev_w_srl16:
; CHECK-SD:       // %bb.0: // %entry
; CHECK-SD-NEXT:    rev w8, w0
; CHECK-SD-NEXT:    lsr w0, w8, #16
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: test_rev_w_srl16:
; CHECK-GI:       // %bb.0: // %entry
; CHECK-GI-NEXT:    and w8, w0, #0xffff
; CHECK-GI-NEXT:    rev16 w0, w8
; CHECK-GI-NEXT:    ret
entry:
  %0 = zext i16 %a to i32
  %1 = tail call i32 @llvm.bswap.i32(i32 %0)
  %2 = lshr i32 %1, 16
  ret i32 %2
}

define i32 @test_rev_w_srl16_load(ptr %a) {
; CHECK-SD-LABEL: test_rev_w_srl16_load:
; CHECK-SD:       // %bb.0: // %entry
; CHECK-SD-NEXT:    ldrh w8, [x0]
; CHECK-SD-NEXT:    rev w8, w8
; CHECK-SD-NEXT:    lsr w0, w8, #16
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: test_rev_w_srl16_load:
; CHECK-GI:       // %bb.0: // %entry
; CHECK-GI-NEXT:    ldrh w8, [x0]
; CHECK-GI-NEXT:    rev16 w0, w8
; CHECK-GI-NEXT:    ret
entry:
  %0 = load i16, ptr %a
  %1 = zext i16 %0 to i32
  %2 = tail call i32 @llvm.bswap.i32(i32 %1)
  %3 = lshr i32 %2, 16
  ret i32 %3
}

define i32 @test_rev_w_srl16_add(i8 %a, i8 %b) {
; CHECK-SD-LABEL: test_rev_w_srl16_add:
; CHECK-SD:       // %bb.0: // %entry
; CHECK-SD-NEXT:    and w8, w0, #0xff
; CHECK-SD-NEXT:    add w8, w8, w1, uxtb
; CHECK-SD-NEXT:    rev16 w0, w8
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: test_rev_w_srl16_add:
; CHECK-GI:       // %bb.0: // %entry
; CHECK-GI-NEXT:    and w8, w1, #0xff
; CHECK-GI-NEXT:    add w8, w8, w0, uxtb
; CHECK-GI-NEXT:    rev16 w0, w8
; CHECK-GI-NEXT:    ret
entry:
  %0 = zext i8 %a to i32
  %1 = zext i8 %b to i32
  %2 = add i32 %0, %1
  %3 = tail call i32 @llvm.bswap.i32(i32 %2)
  %4 = lshr i32 %3, 16
  ret i32 %4
}

; Canonicalize (srl (bswap x), 32) to (rotr (bswap x), 32) if the high 32-bits
; of %a are zero. This optimizes rev + lsr 32 to rev32.
define i64 @test_rev_x_srl32(i32 %a) {
; CHECK-SD-LABEL: test_rev_x_srl32:
; CHECK-SD:       // %bb.0: // %entry
; CHECK-SD-NEXT:    // kill: def $w0 killed $w0 def $x0
; CHECK-SD-NEXT:    rev x8, x0
; CHECK-SD-NEXT:    lsr x0, x8, #32
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: test_rev_x_srl32:
; CHECK-GI:       // %bb.0: // %entry
; CHECK-GI-NEXT:    mov w8, w0
; CHECK-GI-NEXT:    rev32 x0, x8
; CHECK-GI-NEXT:    ret
entry:
  %0 = zext i32 %a to i64
  %1 = tail call i64 @llvm.bswap.i64(i64 %0)
  %2 = lshr i64 %1, 32
  ret i64 %2
}

define i64 @test_rev_x_srl32_load(ptr %a) {
; CHECK-SD-LABEL: test_rev_x_srl32_load:
; CHECK-SD:       // %bb.0: // %entry
; CHECK-SD-NEXT:    ldr w8, [x0]
; CHECK-SD-NEXT:    rev x8, x8
; CHECK-SD-NEXT:    lsr x0, x8, #32
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: test_rev_x_srl32_load:
; CHECK-GI:       // %bb.0: // %entry
; CHECK-GI-NEXT:    ldr w8, [x0]
; CHECK-GI-NEXT:    rev32 x0, x8
; CHECK-GI-NEXT:    ret
entry:
  %0 = load i32, ptr %a
  %1 = zext i32 %0 to i64
  %2 = tail call i64 @llvm.bswap.i64(i64 %1)
  %3 = lshr i64 %2, 32
  ret i64 %3
}

define i64 @test_rev_x_srl32_shift(i64 %a) {
; CHECK-LABEL: test_rev_x_srl32_shift:
; CHECK:       // %bb.0: // %entry
; CHECK-NEXT:    ubfx x8, x0, #2, #29
; CHECK-NEXT:    rev32 x0, x8
; CHECK-NEXT:    ret
entry:
  %0 = shl i64 %a, 33
  %1 = lshr i64 %0, 35
  %2 = tail call i64 @llvm.bswap.i64(i64 %1)
  %3 = lshr i64 %2, 32
  ret i64 %3
}

declare i32 @llvm.bswap.i32(i32) nounwind readnone
declare i64 @llvm.bswap.i64(i64) nounwind readnone

define i32 @test_rev16_w(i32 %X) nounwind {
; CHECK-SD-LABEL: test_rev16_w:
; CHECK-SD:       // %bb.0: // %entry
; CHECK-SD-NEXT:    rev16 w0, w0
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: test_rev16_w:
; CHECK-GI:       // %bb.0: // %entry
; CHECK-GI-NEXT:    lsr w8, w0, #8
; CHECK-GI-NEXT:    lsl w9, w0, #8
; CHECK-GI-NEXT:    and w10, w8, #0xff0000
; CHECK-GI-NEXT:    and w11, w9, #0xff000000
; CHECK-GI-NEXT:    and w8, w8, #0xff
; CHECK-GI-NEXT:    and w9, w9, #0xff00
; CHECK-GI-NEXT:    orr w10, w11, w10
; CHECK-GI-NEXT:    orr w8, w9, w8
; CHECK-GI-NEXT:    orr w0, w10, w8
; CHECK-GI-NEXT:    ret
entry:
  %tmp1 = lshr i32 %X, 8
  %X15 = bitcast i32 %X to i32
  %tmp4 = shl i32 %X15, 8
  %tmp2 = and i32 %tmp1, 16711680
  %tmp5 = and i32 %tmp4, -16777216
  %tmp9 = and i32 %tmp1, 255
  %tmp13 = and i32 %tmp4, 65280
  %tmp6 = or i32 %tmp5, %tmp2
  %tmp10 = or i32 %tmp6, %tmp13
  %tmp14 = or i32 %tmp10, %tmp9
  ret i32 %tmp14
}

; 64-bit REV16 is *not* a swap then a 16-bit rotation:
;   01234567 ->(bswap) 76543210 ->(rotr) 10765432
;   01234567 ->(rev16) 10325476
define i64 @test_rev16_x(i64 %a) nounwind {
; CHECK-LABEL: test_rev16_x:
; CHECK:       // %bb.0: // %entry
; CHECK-NEXT:    rev x8, x0
; CHECK-NEXT:    ror x0, x8, #16
; CHECK-NEXT:    ret
entry:
  %0 = tail call i64 @llvm.bswap.i64(i64 %a)
  %1 = lshr i64 %0, 16
  %2 = shl i64 %0, 48
  %3 = or i64 %1, %2
  ret i64 %3
}

define i64 @test_rev32_x(i64 %a) nounwind {
; CHECK-LABEL: test_rev32_x:
; CHECK:       // %bb.0: // %entry
; CHECK-NEXT:    rev32 x0, x0
; CHECK-NEXT:    ret
entry:
  %0 = tail call i64 @llvm.bswap.i64(i64 %a)
  %1 = lshr i64 %0, 32
  %2 = shl i64 %0, 32
  %3 = or i64 %1, %2
  ret i64 %3
}

define <8 x i8> @test_vrev64D8(ptr %A) nounwind {
; CHECK-LABEL: test_vrev64D8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldr d0, [x0]
; CHECK-NEXT:    rev64.8b v0, v0
; CHECK-NEXT:    ret
  %tmp1 = load <8 x i8>, ptr %A
  %tmp2 = shufflevector <8 x i8> %tmp1, <8 x i8> undef, <8 x i32> <i32 7, i32 6, i32 5, i32 4, i32 3, i32 2, i32 1, i32 0>
  ret <8 x i8> %tmp2
}

define <4 x i16> @test_vrev64D16(ptr %A) nounwind {
; CHECK-LABEL: test_vrev64D16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldr d0, [x0]
; CHECK-NEXT:    rev64.4h v0, v0
; CHECK-NEXT:    ret
  %tmp1 = load <4 x i16>, ptr %A
  %tmp2 = shufflevector <4 x i16> %tmp1, <4 x i16> undef, <4 x i32> <i32 3, i32 2, i32 1, i32 0>
  ret <4 x i16> %tmp2
}

define <2 x i32> @test_vrev64D32(ptr %A) nounwind {
; CHECK-LABEL: test_vrev64D32:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldr d0, [x0]
; CHECK-NEXT:    rev64.2s v0, v0
; CHECK-NEXT:    ret
  %tmp1 = load <2 x i32>, ptr %A
  %tmp2 = shufflevector <2 x i32> %tmp1, <2 x i32> undef, <2 x i32> <i32 1, i32 0>
  ret <2 x i32> %tmp2
}

define <2 x float> @test_vrev64Df(ptr %A) nounwind {
; CHECK-LABEL: test_vrev64Df:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldr d0, [x0]
; CHECK-NEXT:    rev64.2s v0, v0
; CHECK-NEXT:    ret
  %tmp1 = load <2 x float>, ptr %A
  %tmp2 = shufflevector <2 x float> %tmp1, <2 x float> undef, <2 x i32> <i32 1, i32 0>
  ret <2 x float> %tmp2
}

define <16 x i8> @test_vrev64Q8(ptr %A) nounwind {
; CHECK-LABEL: test_vrev64Q8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldr q0, [x0]
; CHECK-NEXT:    rev64.16b v0, v0
; CHECK-NEXT:    ret
  %tmp1 = load <16 x i8>, ptr %A
  %tmp2 = shufflevector <16 x i8> %tmp1, <16 x i8> undef, <16 x i32> <i32 7, i32 6, i32 5, i32 4, i32 3, i32 2, i32 1, i32 0, i32 15, i32 14, i32 13, i32 12, i32 11, i32 10, i32 9, i32 8>
  ret <16 x i8> %tmp2
}

define <8 x i16> @test_vrev64Q16(ptr %A) nounwind {
; CHECK-LABEL: test_vrev64Q16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldr q0, [x0]
; CHECK-NEXT:    rev64.8h v0, v0
; CHECK-NEXT:    ret
  %tmp1 = load <8 x i16>, ptr %A
  %tmp2 = shufflevector <8 x i16> %tmp1, <8 x i16> undef, <8 x i32> <i32 3, i32 2, i32 1, i32 0, i32 7, i32 6, i32 5, i32 4>
  ret <8 x i16> %tmp2
}

define <4 x i32> @test_vrev64Q32(ptr %A) nounwind {
; CHECK-LABEL: test_vrev64Q32:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldr q0, [x0]
; CHECK-NEXT:    rev64.4s v0, v0
; CHECK-NEXT:    ret
  %tmp1 = load <4 x i32>, ptr %A
  %tmp2 = shufflevector <4 x i32> %tmp1, <4 x i32> undef, <4 x i32> <i32 1, i32 0, i32 3, i32 2>
  ret <4 x i32> %tmp2
}

define <4 x float> @test_vrev64Qf(ptr %A) nounwind {
; CHECK-LABEL: test_vrev64Qf:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldr q0, [x0]
; CHECK-NEXT:    rev64.4s v0, v0
; CHECK-NEXT:    ret
  %tmp1 = load <4 x float>, ptr %A
  %tmp2 = shufflevector <4 x float> %tmp1, <4 x float> undef, <4 x i32> <i32 1, i32 0, i32 3, i32 2>
  ret <4 x float> %tmp2
}

define <8 x i8> @test_vrev32D8(ptr %A) nounwind {
; CHECK-LABEL: test_vrev32D8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldr d0, [x0]
; CHECK-NEXT:    rev32.8b v0, v0
; CHECK-NEXT:    ret
  %tmp1 = load <8 x i8>, ptr %A
  %tmp2 = shufflevector <8 x i8> %tmp1, <8 x i8> undef, <8 x i32> <i32 3, i32 2, i32 1, i32 0, i32 7, i32 6, i32 5, i32 4>
  ret <8 x i8> %tmp2
}

define <4 x i16> @test_vrev32D16(ptr %A) nounwind {
; CHECK-LABEL: test_vrev32D16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldr d0, [x0]
; CHECK-NEXT:    rev32.4h v0, v0
; CHECK-NEXT:    ret
  %tmp1 = load <4 x i16>, ptr %A
  %tmp2 = shufflevector <4 x i16> %tmp1, <4 x i16> undef, <4 x i32> <i32 1, i32 0, i32 3, i32 2>
  ret <4 x i16> %tmp2
}

define <16 x i8> @test_vrev32Q8(ptr %A) nounwind {
; CHECK-LABEL: test_vrev32Q8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldr q0, [x0]
; CHECK-NEXT:    rev32.16b v0, v0
; CHECK-NEXT:    ret
  %tmp1 = load <16 x i8>, ptr %A
  %tmp2 = shufflevector <16 x i8> %tmp1, <16 x i8> undef, <16 x i32> <i32 3, i32 2, i32 1, i32 0, i32 7, i32 6, i32 5, i32 4, i32 11, i32 10, i32 9, i32 8, i32 15, i32 14, i32 13, i32 12>
  ret <16 x i8> %tmp2
}

define <8 x i16> @test_vrev32Q16(ptr %A) nounwind {
; CHECK-LABEL: test_vrev32Q16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldr q0, [x0]
; CHECK-NEXT:    rev32.8h v0, v0
; CHECK-NEXT:    ret
  %tmp1 = load <8 x i16>, ptr %A
  %tmp2 = shufflevector <8 x i16> %tmp1, <8 x i16> undef, <8 x i32> <i32 1, i32 0, i32 3, i32 2, i32 5, i32 4, i32 7, i32 6>
  ret <8 x i16> %tmp2
}

define <8 x i8> @test_vrev16D8(ptr %A) nounwind {
; CHECK-LABEL: test_vrev16D8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldr d0, [x0]
; CHECK-NEXT:    rev16.8b v0, v0
; CHECK-NEXT:    ret
  %tmp1 = load <8 x i8>, ptr %A
  %tmp2 = shufflevector <8 x i8> %tmp1, <8 x i8> undef, <8 x i32> <i32 1, i32 0, i32 3, i32 2, i32 5, i32 4, i32 7, i32 6>
  ret <8 x i8> %tmp2
}

define <16 x i8> @test_vrev16Q8(ptr %A) nounwind {
; CHECK-LABEL: test_vrev16Q8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldr q0, [x0]
; CHECK-NEXT:    rev16.16b v0, v0
; CHECK-NEXT:    ret
  %tmp1 = load <16 x i8>, ptr %A
  %tmp2 = shufflevector <16 x i8> %tmp1, <16 x i8> undef, <16 x i32> <i32 1, i32 0, i32 3, i32 2, i32 5, i32 4, i32 7, i32 6, i32 9, i32 8, i32 11, i32 10, i32 13, i32 12, i32 15, i32 14>
  ret <16 x i8> %tmp2
}

define <4 x half> @test_vrev32Df16(<4 x half> %A) nounwind {
; CHECK-LABEL: test_vrev32Df16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    rev32.4h v0, v0
; CHECK-NEXT:    ret
  %tmp2 = shufflevector <4 x half> %A, <4 x half> poison, <4 x i32> <i32 1, i32 0, i32 3, i32 2>
  ret <4 x half> %tmp2
}

define <8 x half> @test_vrev32Qf16(<8 x half> %A) nounwind {
; CHECK-LABEL: test_vrev32Qf16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    rev32.8h v0, v0
; CHECK-NEXT:    ret
  %tmp2 = shufflevector <8 x half> %A, <8 x half> poison, <8 x i32> <i32 1, i32 0, i32 3, i32 2, i32 5, i32 4, i32 7, i32 6>
  ret <8 x half> %tmp2
}

define <4 x half> @test_vrev64Df16(<4 x half> %A) nounwind {
; CHECK-LABEL: test_vrev64Df16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    rev64.4h v0, v0
; CHECK-NEXT:    ret
  %tmp2 = shufflevector <4 x half> %A, <4 x half> poison, <4 x i32> <i32 3, i32 2, i32 1, i32 0>
  ret <4 x half> %tmp2
}

define <8 x half> @test_vrev64Qf16(<8 x half> %A) nounwind {
; CHECK-LABEL: test_vrev64Qf16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    rev64.8h v0, v0
; CHECK-NEXT:    ret
  %tmp2 = shufflevector <8 x half> %A, <8 x half> poison, <8 x i32> <i32 3, i32 2, i32 1, i32 0, i32 7, i32 6, i32 5, i32 4>
  ret <8 x half> %tmp2
}

define <4 x bfloat> @test_vrev32Dbf16(<4 x bfloat> %A) nounwind {
; CHECK-LABEL: test_vrev32Dbf16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    rev32.4h v0, v0
; CHECK-NEXT:    ret
  %tmp2 = shufflevector <4 x bfloat> %A, <4 x bfloat> poison, <4 x i32> <i32 1, i32 0, i32 3, i32 2>
  ret <4 x bfloat> %tmp2
}

define <8 x bfloat> @test_vrev32Qbf16(<8 x bfloat> %A) nounwind {
; CHECK-LABEL: test_vrev32Qbf16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    rev32.8h v0, v0
; CHECK-NEXT:    ret
  %tmp2 = shufflevector <8 x bfloat> %A, <8 x bfloat> poison, <8 x i32> <i32 1, i32 0, i32 3, i32 2, i32 5, i32 4, i32 7, i32 6>
  ret <8 x bfloat> %tmp2
}

define <4 x bfloat> @test_vrev64Dbf16(<4 x bfloat> %A) nounwind {
; CHECK-LABEL: test_vrev64Dbf16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    rev64.4h v0, v0
; CHECK-NEXT:    ret
  %tmp2 = shufflevector <4 x bfloat> %A, <4 x bfloat> poison, <4 x i32> <i32 3, i32 2, i32 1, i32 0>
  ret <4 x bfloat> %tmp2
}

define <8 x bfloat> @test_vrev64Qbf16(<8 x bfloat> %A) nounwind {
; CHECK-LABEL: test_vrev64Qbf16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    rev64.8h v0, v0
; CHECK-NEXT:    ret
  %tmp2 = shufflevector <8 x bfloat> %A, <8 x bfloat> poison, <8 x i32> <i32 3, i32 2, i32 1, i32 0, i32 7, i32 6, i32 5, i32 4>
  ret <8 x bfloat> %tmp2
}

; Undef shuffle indices should not prevent matching to VREV:

define <8 x i8> @test_vrev64D8_undef(ptr %A) nounwind {
; CHECK-LABEL: test_vrev64D8_undef:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldr d0, [x0]
; CHECK-NEXT:    rev64.8b v0, v0
; CHECK-NEXT:    ret
  %tmp1 = load <8 x i8>, ptr %A
  %tmp2 = shufflevector <8 x i8> %tmp1, <8 x i8> undef, <8 x i32> <i32 7, i32 undef, i32 undef, i32 4, i32 3, i32 2, i32 1, i32 0>
  ret <8 x i8> %tmp2
}

define <8 x i16> @test_vrev32Q16_undef(ptr %A) nounwind {
; CHECK-LABEL: test_vrev32Q16_undef:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ldr q0, [x0]
; CHECK-NEXT:    rev32.8h v0, v0
; CHECK-NEXT:    ret
  %tmp1 = load <8 x i16>, ptr %A
  %tmp2 = shufflevector <8 x i16> %tmp1, <8 x i16> undef, <8 x i32> <i32 undef, i32 0, i32 undef, i32 2, i32 5, i32 4, i32 7, i32 undef>
  ret <8 x i16> %tmp2
}

; vrev <4 x i16> should use REV32 and not REV64
define void @test_vrev64(ptr nocapture %source, ptr nocapture %dst) nounwind ssp {
; CHECK-SD-LABEL: test_vrev64:
; CHECK-SD:       // %bb.0: // %entry
; CHECK-SD-NEXT:    ldr q0, [x0]
; CHECK-SD-NEXT:    add x8, x1, #2
; CHECK-SD-NEXT:    st1.h { v0 }[5], [x8]
; CHECK-SD-NEXT:    st1.h { v0 }[6], [x1]
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: test_vrev64:
; CHECK-GI:       // %bb.0: // %entry
; CHECK-GI-NEXT:    ldr q0, [x0]
; CHECK-GI-NEXT:    add x8, x1, #2
; CHECK-GI-NEXT:    st1.h { v0 }[6], [x1]
; CHECK-GI-NEXT:    st1.h { v0 }[5], [x8]
; CHECK-GI-NEXT:    ret
entry:
  %tmp2 = load <8 x i16>, ptr %source, align 4
  %tmp3 = extractelement <8 x i16> %tmp2, i32 6
  %tmp5 = insertelement <2 x i16> undef, i16 %tmp3, i32 0
  %tmp9 = extractelement <8 x i16> %tmp2, i32 5
  %tmp11 = insertelement <2 x i16> %tmp5, i16 %tmp9, i32 1
  store <2 x i16> %tmp11, ptr %dst, align 4
  ret void
}

; Test vrev of float4
define void @float_vrev64(ptr nocapture %source, ptr nocapture %dest) nounwind noinline ssp {
; CHECK-SD-LABEL: float_vrev64:
; CHECK-SD:       // %bb.0: // %entry
; CHECK-SD-NEXT:    movi.2d v0, #0000000000000000
; CHECK-SD-NEXT:    add x8, x0, #12
; CHECK-SD-NEXT:    dup.4s v0, v0[0]
; CHECK-SD-NEXT:    ld1.s { v0 }[1], [x8]
; CHECK-SD-NEXT:    str q0, [x1, #176]
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: float_vrev64:
; CHECK-GI:       // %bb.0: // %entry
; CHECK-GI-NEXT:    movi d0, #0000000000000000
; CHECK-GI-NEXT:    adrp x8, .LCPI36_0
; CHECK-GI-NEXT:    ldr q1, [x0]
; CHECK-GI-NEXT:    ldr q2, [x8, :lo12:.LCPI36_0]
; CHECK-GI-NEXT:    tbl.16b v0, { v0, v1 }, v2
; CHECK-GI-NEXT:    str q0, [x1, #176]
; CHECK-GI-NEXT:    ret
entry:
  %tmp2 = load <4 x float>, ptr %source, align 4
  %tmp5 = shufflevector <4 x float> <float 0.000000e+00, float undef, float undef, float undef>, <4 x float> %tmp2, <4 x i32> <i32 0, i32 7, i32 0, i32 0>
  %arrayidx8 = getelementptr inbounds <4 x float>, ptr %dest, i32 11
  store <4 x float> %tmp5, ptr %arrayidx8, align 4
  ret void
}


define <4 x i32> @test_vrev32_bswap(<4 x i32> %source) nounwind {
; CHECK-LABEL: test_vrev32_bswap:
; CHECK:       // %bb.0:
; CHECK-NEXT:    rev32.16b v0, v0
; CHECK-NEXT:    ret
  %bswap = call <4 x i32> @llvm.bswap.v4i32(<4 x i32> %source)
  ret <4 x i32> %bswap
}

declare <4 x i32> @llvm.bswap.v4i32(<4 x i32>) nounwind readnone

; Reduced regression from D114354
define void @test_rev16_truncstore() {
; CHECK-SD-LABEL: test_rev16_truncstore:
; CHECK-SD:       // %bb.0: // %entry
; CHECK-SD-NEXT:    cbnz wzr, .LBB38_2
; CHECK-SD-NEXT:  .LBB38_1: // %cleanup
; CHECK-SD-NEXT:    // =>This Inner Loop Header: Depth=1
; CHECK-SD-NEXT:    ldrh w8, [x8]
; CHECK-SD-NEXT:    rev16 w8, w8
; CHECK-SD-NEXT:    strh w8, [x8]
; CHECK-SD-NEXT:    cbz wzr, .LBB38_1
; CHECK-SD-NEXT:  .LBB38_2: // %fail
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: test_rev16_truncstore:
; CHECK-GI:       // %bb.0: // %entry
; CHECK-GI-NEXT:    tbnz wzr, #0, .LBB38_2
; CHECK-GI-NEXT:  .LBB38_1: // %cleanup
; CHECK-GI-NEXT:    // =>This Inner Loop Header: Depth=1
; CHECK-GI-NEXT:    ldrh w8, [x8]
; CHECK-GI-NEXT:    rev w8, w8
; CHECK-GI-NEXT:    lsr w8, w8, #16
; CHECK-GI-NEXT:    strh w8, [x8]
; CHECK-GI-NEXT:    tbz wzr, #0, .LBB38_1
; CHECK-GI-NEXT:  .LBB38_2: // %fail
; CHECK-GI-NEXT:    ret
entry:
  br label %body

body:
  %out.6269.i = phi ptr [ undef, %cleanup ], [ undef, %entry ]
  %0 = load i16, ptr undef, align 2
  %1 = icmp eq i16 undef, -10240
  br i1 %1, label %fail, label %cleanup

cleanup:
  %or130.i = call i16 @llvm.bswap.i16(i16 %0)
  store i16 %or130.i, ptr %out.6269.i, align 2
  br label %body

fail:
  ret void
}
declare i16 @llvm.bswap.i16(i16)

; Reduced regression from D120192
define void @test_bswap32_narrow(ptr %p0, ptr %p1) nounwind {
; CHECK-SD-LABEL: test_bswap32_narrow:
; CHECK-SD:       // %bb.0:
; CHECK-SD-NEXT:    stp x30, x19, [sp, #-16]! // 16-byte Folded Spill
; CHECK-SD-NEXT:    ldrh w8, [x0, #2]
; CHECK-SD-NEXT:    mov x19, x1
; CHECK-SD-NEXT:    rev16 w0, w8
; CHECK-SD-NEXT:    bl gid_tbl_len
; CHECK-SD-NEXT:    strh wzr, [x19]
; CHECK-SD-NEXT:    ldp x30, x19, [sp], #16 // 16-byte Folded Reload
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: test_bswap32_narrow:
; CHECK-GI:       // %bb.0:
; CHECK-GI-NEXT:    stp x30, x19, [sp, #-16]! // 16-byte Folded Spill
; CHECK-GI-NEXT:    ldr w8, [x0]
; CHECK-GI-NEXT:    mov x19, x1
; CHECK-GI-NEXT:    and w8, w8, #0xffff0000
; CHECK-GI-NEXT:    rev w0, w8
; CHECK-GI-NEXT:    bl gid_tbl_len
; CHECK-GI-NEXT:    strh wzr, [x19]
; CHECK-GI-NEXT:    ldp x30, x19, [sp], #16 // 16-byte Folded Reload
; CHECK-GI-NEXT:    ret
  %ld = load i32, ptr %p0, align 4
  %and = and i32 %ld, -65536
  %bswap = tail call i32 @llvm.bswap.i32(i32 %and)
  %and16 = zext i32 %bswap to i64
  %call17 = tail call i32 @gid_tbl_len(i64 %and16)
  store i16 0, ptr %p1, align 4
  ret void
}
declare i32 @gid_tbl_len(...)

; 64-bit REV16 is *not* a swap then a 16-bit rotation:
;   01234567 ->(bswap) 76543210 ->(rotr) 10765432
;   01234567 ->(rev16) 10325476
; Optimize patterns where rev16 can be generated for a 64-bit input.
define i64 @test_rev16_x_hwbyteswaps(i64 %a) nounwind {
; CHECK-LABEL: test_rev16_x_hwbyteswaps:
; CHECK:       // %bb.0: // %entry
; CHECK-NEXT:    rev16 x0, x0
; CHECK-NEXT:    ret
entry:
  %0 = lshr i64 %a, 8
  %1 = and i64 %0, 71777214294589695
  %2 = shl i64 %a, 8
  %3 = and i64 %2, -71777214294589696
  %4 = or i64 %1, %3
  ret i64 %4
}

; Optimize pattern with multiple and/or to a simple pattern which can enable generation of rev16.
define i64 @test_rev16_x_hwbyteswaps_complex1(i64 %a) nounwind {
; CHECK-SD-LABEL: test_rev16_x_hwbyteswaps_complex1:
; CHECK-SD:       // %bb.0: // %entry
; CHECK-SD-NEXT:    lsr x8, x0, #8
; CHECK-SD-NEXT:    lsr x9, x0, #48
; CHECK-SD-NEXT:    and x10, x8, #0xff000000000000
; CHECK-SD-NEXT:    and x11, x8, #0xff00000000
; CHECK-SD-NEXT:    and x8, x8, #0xff0000
; CHECK-SD-NEXT:    bfi x10, x9, #56, #8
; CHECK-SD-NEXT:    lsr x9, x0, #32
; CHECK-SD-NEXT:    orr x10, x10, x11
; CHECK-SD-NEXT:    bfi x10, x9, #40, #8
; CHECK-SD-NEXT:    lsr x9, x0, #16
; CHECK-SD-NEXT:    orr x8, x10, x8
; CHECK-SD-NEXT:    bfi x8, x9, #24, #8
; CHECK-SD-NEXT:    ubfiz x9, x0, #8, #8
; CHECK-SD-NEXT:    bfxil x8, x0, #8, #8
; CHECK-SD-NEXT:    orr x0, x8, x9
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: test_rev16_x_hwbyteswaps_complex1:
; CHECK-GI:       // %bb.0: // %entry
; CHECK-GI-NEXT:    lsr x8, x0, #8
; CHECK-GI-NEXT:    lsl x9, x0, #8
; CHECK-GI-NEXT:    and x10, x8, #0xff000000000000
; CHECK-GI-NEXT:    and x11, x9, #0xff00000000000000
; CHECK-GI-NEXT:    and x12, x8, #0xff00000000
; CHECK-GI-NEXT:    and x13, x9, #0xff0000000000
; CHECK-GI-NEXT:    and x14, x8, #0xff0000
; CHECK-GI-NEXT:    orr x10, x10, x11
; CHECK-GI-NEXT:    and x11, x9, #0xff000000
; CHECK-GI-NEXT:    orr x12, x12, x13
; CHECK-GI-NEXT:    and x8, x8, #0xff
; CHECK-GI-NEXT:    orr x11, x14, x11
; CHECK-GI-NEXT:    orr x10, x10, x12
; CHECK-GI-NEXT:    and x9, x9, #0xff00
; CHECK-GI-NEXT:    orr x8, x11, x8
; CHECK-GI-NEXT:    orr x8, x10, x8
; CHECK-GI-NEXT:    orr x0, x8, x9
; CHECK-GI-NEXT:    ret
entry:
  %0 = lshr i64 %a, 8
  %1 = and i64 %0, 71776119061217280
  %2 = shl i64 %a, 8
  %3 = and i64 %2, -72057594037927936
  %4 = or i64 %1, %3
  %5 = and i64 %0, 1095216660480
  %6 = or i64 %4, %5
  %7 = and i64 %2, 280375465082880
  %8 = or i64 %6, %7
  %9 = and i64 %0, 16711680
  %10 = or i64 %8, %9
  %11 = and i64 %2, 4278190080
  %12 = or i64 %10, %11
  %13 = and i64 %0, 255
  %14 = or i64 %12, %13
  %15 = and i64 %2, 65280
  %16 = or i64 %14, %15
  ret i64 %16
}

define i64 @test_rev16_x_hwbyteswaps_complex2(i64 %a) nounwind {
; CHECK-SD-LABEL: test_rev16_x_hwbyteswaps_complex2:
; CHECK-SD:       // %bb.0: // %entry
; CHECK-SD-NEXT:    lsr x8, x0, #8
; CHECK-SD-NEXT:    lsr x9, x0, #48
; CHECK-SD-NEXT:    lsr x10, x0, #32
; CHECK-SD-NEXT:    and x8, x8, #0xff00ff00ff00ff
; CHECK-SD-NEXT:    bfi x8, x9, #56, #8
; CHECK-SD-NEXT:    lsr x9, x0, #16
; CHECK-SD-NEXT:    bfi x8, x10, #40, #8
; CHECK-SD-NEXT:    bfi x8, x9, #24, #8
; CHECK-SD-NEXT:    bfi x8, x0, #8, #8
; CHECK-SD-NEXT:    mov x0, x8
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: test_rev16_x_hwbyteswaps_complex2:
; CHECK-GI:       // %bb.0: // %entry
; CHECK-GI-NEXT:    lsr x8, x0, #8
; CHECK-GI-NEXT:    lsl x9, x0, #8
; CHECK-GI-NEXT:    and x10, x8, #0xff000000000000
; CHECK-GI-NEXT:    and x11, x8, #0xff00000000
; CHECK-GI-NEXT:    and x12, x8, #0xff0000
; CHECK-GI-NEXT:    and x8, x8, #0xff
; CHECK-GI-NEXT:    and x13, x9, #0xff00000000000000
; CHECK-GI-NEXT:    orr x10, x10, x11
; CHECK-GI-NEXT:    and x11, x9, #0xff0000000000
; CHECK-GI-NEXT:    orr x8, x12, x8
; CHECK-GI-NEXT:    and x12, x9, #0xff000000
; CHECK-GI-NEXT:    orr x11, x13, x11
; CHECK-GI-NEXT:    orr x8, x10, x8
; CHECK-GI-NEXT:    and x9, x9, #0xff00
; CHECK-GI-NEXT:    orr x10, x11, x12
; CHECK-GI-NEXT:    orr x8, x8, x10
; CHECK-GI-NEXT:    orr x0, x8, x9
; CHECK-GI-NEXT:    ret
entry:
  %0 = lshr i64 %a, 8
  %1 = and i64 %0, 71776119061217280
  %2 = shl i64 %a, 8
  %3 = and i64 %0, 1095216660480
  %4 = or i64 %1, %3
  %5 = and i64 %0, 16711680
  %6 = or i64 %4, %5
  %7 = and i64 %0, 255
  %8 = or i64 %6, %7
  %9 = and i64 %2, -72057594037927936
  %10 = or i64 %8, %9
  %11 = and i64 %2, 280375465082880
  %12 = or i64 %10, %11
  %13 = and i64 %2, 4278190080
  %14 = or i64 %12, %13
  %15 = and i64 %2, 65280
  %16 = or i64 %14, %15
  ret i64 %16
}

; Optimize pattern with multiple and/or to a simple pattern which can enable generation of rev16.
define i64 @test_rev16_x_hwbyteswaps_complex3(i64 %a) nounwind {
; CHECK-SD-LABEL: test_rev16_x_hwbyteswaps_complex3:
; CHECK-SD:       // %bb.0: // %entry
; CHECK-SD-NEXT:    lsr x8, x0, #8
; CHECK-SD-NEXT:    lsr x9, x0, #48
; CHECK-SD-NEXT:    and x10, x8, #0xff000000000000
; CHECK-SD-NEXT:    and x11, x8, #0xff00000000
; CHECK-SD-NEXT:    and x8, x8, #0xff0000
; CHECK-SD-NEXT:    bfi x10, x9, #56, #8
; CHECK-SD-NEXT:    lsr x9, x0, #32
; CHECK-SD-NEXT:    orr x10, x11, x10
; CHECK-SD-NEXT:    bfi x10, x9, #40, #8
; CHECK-SD-NEXT:    lsr x9, x0, #16
; CHECK-SD-NEXT:    orr x8, x8, x10
; CHECK-SD-NEXT:    bfi x8, x9, #24, #8
; CHECK-SD-NEXT:    ubfiz x9, x0, #8, #8
; CHECK-SD-NEXT:    bfxil x8, x0, #8, #8
; CHECK-SD-NEXT:    orr x0, x9, x8
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: test_rev16_x_hwbyteswaps_complex3:
; CHECK-GI:       // %bb.0: // %entry
; CHECK-GI-NEXT:    lsr x8, x0, #8
; CHECK-GI-NEXT:    lsl x9, x0, #8
; CHECK-GI-NEXT:    and x10, x8, #0xff000000000000
; CHECK-GI-NEXT:    and x11, x9, #0xff00000000000000
; CHECK-GI-NEXT:    and x12, x8, #0xff00000000
; CHECK-GI-NEXT:    and x13, x9, #0xff0000000000
; CHECK-GI-NEXT:    and x14, x8, #0xff0000
; CHECK-GI-NEXT:    orr x10, x11, x10
; CHECK-GI-NEXT:    and x11, x9, #0xff000000
; CHECK-GI-NEXT:    orr x12, x13, x12
; CHECK-GI-NEXT:    and x8, x8, #0xff
; CHECK-GI-NEXT:    orr x11, x11, x14
; CHECK-GI-NEXT:    orr x10, x12, x10
; CHECK-GI-NEXT:    and x9, x9, #0xff00
; CHECK-GI-NEXT:    orr x8, x8, x11
; CHECK-GI-NEXT:    orr x8, x8, x10
; CHECK-GI-NEXT:    orr x0, x9, x8
; CHECK-GI-NEXT:    ret
entry:
  %0 = lshr i64 %a, 8
  %1 = and i64 %0, 71776119061217280
  %2 = shl i64 %a, 8
  %3 = and i64 %2, -72057594037927936
  %4 = or i64 %3, %1
  %5 = and i64 %0, 1095216660480
  %6 = or i64 %5, %4
  %7 = and i64 %2, 280375465082880
  %8 = or i64 %7, %6
  %9 = and i64 %0, 16711680
  %10 = or i64 %9, %8
  %11 = and i64 %2, 4278190080
  %12 = or i64 %11, %10
  %13 = and i64 %0, 255
  %14 = or i64 %13, %12
  %15 = and i64 %2, 65280
  %16 = or i64 %15, %14
  ret i64 %16
}

define i64 @test_or_and_combine1(i64 %a) nounwind {
; CHECK-SD-LABEL: test_or_and_combine1:
; CHECK-SD:       // %bb.0: // %entry
; CHECK-SD-NEXT:    lsr x8, x0, #8
; CHECK-SD-NEXT:    lsr x9, x0, #24
; CHECK-SD-NEXT:    and x10, x8, #0xff000000000000
; CHECK-SD-NEXT:    and x8, x8, #0xff0000
; CHECK-SD-NEXT:    bfi x10, x9, #32, #8
; CHECK-SD-NEXT:    orr x0, x10, x8
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: test_or_and_combine1:
; CHECK-GI:       // %bb.0: // %entry
; CHECK-GI-NEXT:    lsr x8, x0, #8
; CHECK-GI-NEXT:    lsl x9, x0, #8
; CHECK-GI-NEXT:    and x10, x8, #0xff000000000000
; CHECK-GI-NEXT:    and x9, x9, #0xff00000000
; CHECK-GI-NEXT:    and x8, x8, #0xff0000
; CHECK-GI-NEXT:    orr x9, x10, x9
; CHECK-GI-NEXT:    orr x0, x9, x8
; CHECK-GI-NEXT:    ret
entry:
  %0 = lshr i64 %a, 8
  %1 = and i64 %0, 71776119061217280
  %2 = shl i64 %a, 8
  %3 = and i64 %2, 1095216660480
  %4 = or i64 %1, %3
  %5 = and i64 %0, 16711680
  %6 = or i64 %4, %5
  ret i64 %6
}

define i64 @test_or_and_combine2(i64 %a, i64 %b) nounwind {
; CHECK-LABEL: test_or_and_combine2:
; CHECK:       // %bb.0: // %entry
; CHECK-NEXT:    lsr x8, x0, #8
; CHECK-NEXT:    lsl x9, x0, #8
; CHECK-NEXT:    and x10, x8, #0xff000000000000
; CHECK-NEXT:    and x11, x9, #0xff00000000
; CHECK-NEXT:    and x8, x8, #0xff0000
; CHECK-NEXT:    orr x9, x10, x9
; CHECK-NEXT:    orr x8, x11, x8
; CHECK-NEXT:    orr x0, x9, x8
; CHECK-NEXT:    ret
entry:
  %0 = lshr i64 %a, 8
  %1 = and i64 %0, 71776119061217280
  %2 = shl i64 %a, 8
  %3 = or i64 %1, %2
  %4 = and i64 %2, 1095216660480
  %5 = or i64 %3, %4
  %6 = and i64 %0, 16711680
  %7 = or i64 %5, %6
  ret i64 %7
}

define i32 @pr55484(i32 %0) {
; CHECK-SD-LABEL: pr55484:
; CHECK-SD:       // %bb.0:
; CHECK-SD-NEXT:    lsr w8, w0, #8
; CHECK-SD-NEXT:    orr w8, w8, w0, lsl #8
; CHECK-SD-NEXT:    sxth w0, w8
; CHECK-SD-NEXT:    ret
;
; CHECK-GI-LABEL: pr55484:
; CHECK-GI:       // %bb.0:
; CHECK-GI-NEXT:    lsl w8, w0, #8
; CHECK-GI-NEXT:    orr w8, w8, w0, lsr #8
; CHECK-GI-NEXT:    sxth w0, w8
; CHECK-GI-NEXT:    ret
  %2 = lshr i32 %0, 8
  %3 = shl i32 %0, 8
  %4 = or i32 %2, %3
  %5 = trunc i32 %4 to i16
  %6 = sext i16 %5 to i32
  ret i32 %6
}

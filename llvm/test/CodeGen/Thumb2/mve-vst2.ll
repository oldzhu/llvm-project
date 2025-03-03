; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -mtriple=thumbv8.1m.main-none-none-eabi -mattr=+mve.fp,+fp64 -verify-machineinstrs %s -o - | FileCheck %s

; i32

define void @vst2_v2i32(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v2i32:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    ldm.w r0, {r2, r3, r12}
; CHECK-NEXT:    ldr r0, [r0, #12]
; CHECK-NEXT:    vmov q0[2], q0[0], r2, r3
; CHECK-NEXT:    vmov q0[3], q0[1], r12, r0
; CHECK-NEXT:    vstrw.32 q0, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <2 x i32>, ptr %src, align 4
  %s2 = getelementptr <2 x i32>, ptr %src, i32 1
  %l2 = load <2 x i32>, ptr %s2, align 4
  %s = shufflevector <2 x i32> %l1, <2 x i32> %l2, <4 x i32> <i32 0, i32 2, i32 1, i32 3>
  store <4 x i32> %s, ptr %dst, align 4
  ret void
}

define void @vst2_v4i32(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v4i32:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    vldrw.u32 q1, [r0, #16]
; CHECK-NEXT:    vldrw.u32 q0, [r0]
; CHECK-NEXT:    vst20.32 {q0, q1}, [r1]
; CHECK-NEXT:    vst21.32 {q0, q1}, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <4 x i32>, ptr %src, align 4
  %s2 = getelementptr <4 x i32>, ptr %src, i32 1
  %l2 = load <4 x i32>, ptr %s2, align 4
  %s = shufflevector <4 x i32> %l1, <4 x i32> %l2, <8 x i32> <i32 0, i32 4, i32 1, i32 5, i32 2, i32 6, i32 3, i32 7>
  store <8 x i32> %s, ptr %dst, align 4
  ret void
}

define void @vst2_v8i32(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v8i32:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    vldrw.u32 q1, [r0, #32]
; CHECK-NEXT:    vldrw.u32 q0, [r0]
; CHECK-NEXT:    vldrw.u32 q3, [r0, #48]
; CHECK-NEXT:    vldrw.u32 q2, [r0, #16]
; CHECK-NEXT:    vst20.32 {q0, q1}, [r1]
; CHECK-NEXT:    vst21.32 {q0, q1}, [r1]!
; CHECK-NEXT:    vst20.32 {q2, q3}, [r1]
; CHECK-NEXT:    vst21.32 {q2, q3}, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <8 x i32>, ptr %src, align 4
  %s2 = getelementptr <8 x i32>, ptr %src, i32 1
  %l2 = load <8 x i32>, ptr %s2, align 4
  %s = shufflevector <8 x i32> %l1, <8 x i32> %l2, <16 x i32> <i32 0, i32 8, i32 1, i32 9, i32 2, i32 10, i32 3, i32 11, i32 4, i32 12, i32 5, i32 13, i32 6, i32 14, i32 7, i32 15>
  store <16 x i32> %s, ptr %dst, align 4
  ret void
}

define void @vst2_v16i32(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v16i32:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    .vsave {d8, d9, d10, d11, d12, d13, d14, d15}
; CHECK-NEXT:    vpush {d8, d9, d10, d11, d12, d13, d14, d15}
; CHECK-NEXT:    vldrw.u32 q7, [r0, #64]
; CHECK-NEXT:    vldrw.u32 q6, [r0]
; CHECK-NEXT:    vldrw.u32 q1, [r0, #112]
; CHECK-NEXT:    vldrw.u32 q3, [r0, #96]
; CHECK-NEXT:    vldrw.u32 q5, [r0, #80]
; CHECK-NEXT:    vldrw.u32 q0, [r0, #48]
; CHECK-NEXT:    vldrw.u32 q2, [r0, #32]
; CHECK-NEXT:    vldrw.u32 q4, [r0, #16]
; CHECK-NEXT:    add.w r0, r1, #96
; CHECK-NEXT:    add.w r2, r1, #64
; CHECK-NEXT:    vst20.32 {q6, q7}, [r1]
; CHECK-NEXT:    vst21.32 {q6, q7}, [r1]!
; CHECK-NEXT:    vst20.32 {q4, q5}, [r1]
; CHECK-NEXT:    vst20.32 {q2, q3}, [r2]
; CHECK-NEXT:    vst20.32 {q0, q1}, [r0]
; CHECK-NEXT:    vst21.32 {q4, q5}, [r1]
; CHECK-NEXT:    vst21.32 {q2, q3}, [r2]
; CHECK-NEXT:    vst21.32 {q0, q1}, [r0]
; CHECK-NEXT:    vpop {d8, d9, d10, d11, d12, d13, d14, d15}
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <16 x i32>, ptr %src, align 4
  %s2 = getelementptr <16 x i32>, ptr %src, i32 1
  %l2 = load <16 x i32>, ptr %s2, align 4
  %s = shufflevector <16 x i32> %l1, <16 x i32> %l2, <32 x i32> <i32 0, i32 16, i32 1, i32 17, i32 2, i32 18, i32 3, i32 19, i32 4, i32 20, i32 5, i32 21, i32 6, i32 22, i32 7, i32 23, i32 8, i32 24, i32 9, i32 25, i32 10, i32 26, i32 11, i32 27, i32 12, i32 28, i32 13, i32 29, i32 14, i32 30, i32 15, i32 31>
  store <32 x i32> %s, ptr %dst, align 4
  ret void
}

define void @vst2_v4i32_align1(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v4i32_align1:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    vldrw.u32 q0, [r0, #16]
; CHECK-NEXT:    vldrw.u32 q1, [r0]
; CHECK-NEXT:    vmov.f32 s8, s6
; CHECK-NEXT:    vmov.f32 s9, s2
; CHECK-NEXT:    vmov.f32 s10, s7
; CHECK-NEXT:    vmov.f32 s11, s3
; CHECK-NEXT:    vmov.f32 s12, s4
; CHECK-NEXT:    vstrb.8 q2, [r1, #16]
; CHECK-NEXT:    vmov.f32 s13, s0
; CHECK-NEXT:    vmov.f32 s14, s5
; CHECK-NEXT:    vmov.f32 s15, s1
; CHECK-NEXT:    vstrb.8 q3, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <4 x i32>, ptr %src, align 4
  %s2 = getelementptr <4 x i32>, ptr %src, i32 1
  %l2 = load <4 x i32>, ptr %s2, align 4
  %s = shufflevector <4 x i32> %l1, <4 x i32> %l2, <8 x i32> <i32 0, i32 4, i32 1, i32 5, i32 2, i32 6, i32 3, i32 7>
  store <8 x i32> %s, ptr %dst, align 1
  ret void
}

; i16

define void @vst2_v2i16(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v2i16:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    ldrh r2, [r0, #2]
; CHECK-NEXT:    ldrh r3, [r0]
; CHECK-NEXT:    ldrh.w r12, [r0, #6]
; CHECK-NEXT:    ldrh r0, [r0, #4]
; CHECK-NEXT:    vmov q0[2], q0[0], r3, r2
; CHECK-NEXT:    vmov q0[3], q0[1], r0, r12
; CHECK-NEXT:    vstrh.32 q0, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <2 x i16>, ptr %src, align 4
  %s2 = getelementptr <2 x i16>, ptr %src, i32 1
  %l2 = load <2 x i16>, ptr %s2, align 4
  %s = shufflevector <2 x i16> %l1, <2 x i16> %l2, <4 x i32> <i32 0, i32 2, i32 1, i32 3>
  store <4 x i16> %s, ptr %dst, align 2
  ret void
}

define void @vst2_v4i16(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v4i16:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    vldrh.u32 q0, [r0, #8]
; CHECK-NEXT:    vldrh.u32 q1, [r0]
; CHECK-NEXT:    vmovnt.i32 q1, q0
; CHECK-NEXT:    vstrh.16 q1, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <4 x i16>, ptr %src, align 4
  %s2 = getelementptr <4 x i16>, ptr %src, i32 1
  %l2 = load <4 x i16>, ptr %s2, align 4
  %s = shufflevector <4 x i16> %l1, <4 x i16> %l2, <8 x i32> <i32 0, i32 4, i32 1, i32 5, i32 2, i32 6, i32 3, i32 7>
  store <8 x i16> %s, ptr %dst, align 2
  ret void
}

define void @vst2_v8i16(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v8i16:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    vldrw.u32 q1, [r0, #16]
; CHECK-NEXT:    vldrw.u32 q0, [r0]
; CHECK-NEXT:    vst20.16 {q0, q1}, [r1]
; CHECK-NEXT:    vst21.16 {q0, q1}, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <8 x i16>, ptr %src, align 4
  %s2 = getelementptr <8 x i16>, ptr %src, i32 1
  %l2 = load <8 x i16>, ptr %s2, align 4
  %s = shufflevector <8 x i16> %l1, <8 x i16> %l2, <16 x i32> <i32 0, i32 8, i32 1, i32 9, i32 2, i32 10, i32 3, i32 11, i32 4, i32 12, i32 5, i32 13, i32 6, i32 14, i32 7, i32 15>
  store <16 x i16> %s, ptr %dst, align 2
  ret void
}

define void @vst2_v16i16(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v16i16:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    vldrw.u32 q1, [r0, #32]
; CHECK-NEXT:    vldrw.u32 q0, [r0]
; CHECK-NEXT:    vldrw.u32 q3, [r0, #48]
; CHECK-NEXT:    vldrw.u32 q2, [r0, #16]
; CHECK-NEXT:    vst20.16 {q0, q1}, [r1]
; CHECK-NEXT:    vst21.16 {q0, q1}, [r1]!
; CHECK-NEXT:    vst20.16 {q2, q3}, [r1]
; CHECK-NEXT:    vst21.16 {q2, q3}, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <16 x i16>, ptr %src, align 4
  %s2 = getelementptr <16 x i16>, ptr %src, i32 1
  %l2 = load <16 x i16>, ptr %s2, align 4
  %s = shufflevector <16 x i16> %l1, <16 x i16> %l2, <32 x i32> <i32 0, i32 16, i32 1, i32 17, i32 2, i32 18, i32 3, i32 19, i32 4, i32 20, i32 5, i32 21, i32 6, i32 22, i32 7, i32 23, i32 8, i32 24, i32 9, i32 25, i32 10, i32 26, i32 11, i32 27, i32 12, i32 28, i32 13, i32 29, i32 14, i32 30, i32 15, i32 31>
  store <32 x i16> %s, ptr %dst, align 2
  ret void
}

define void @vst2_v8i16_align1(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v8i16_align1:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    vldrw.u32 q2, [r0]
; CHECK-NEXT:    vldrw.u32 q1, [r0, #16]
; CHECK-NEXT:    vmovx.f16 s1, s10
; CHECK-NEXT:    vmovx.f16 s0, s6
; CHECK-NEXT:    vins.f16 s10, s6
; CHECK-NEXT:    vmovx.f16 s3, s11
; CHECK-NEXT:    vmovx.f16 s6, s7
; CHECK-NEXT:    vins.f16 s11, s7
; CHECK-NEXT:    vins.f16 s3, s6
; CHECK-NEXT:    vmovx.f16 s6, s8
; CHECK-NEXT:    vins.f16 s8, s4
; CHECK-NEXT:    vmovx.f16 s4, s4
; CHECK-NEXT:    vmov q3, q2
; CHECK-NEXT:    vins.f16 s6, s4
; CHECK-NEXT:    vmovx.f16 s15, s9
; CHECK-NEXT:    vins.f16 s9, s5
; CHECK-NEXT:    vmovx.f16 s4, s5
; CHECK-NEXT:    vins.f16 s1, s0
; CHECK-NEXT:    vmov.f32 s0, s10
; CHECK-NEXT:    vins.f16 s15, s4
; CHECK-NEXT:    vmov.f32 s2, s11
; CHECK-NEXT:    vmov.f32 s13, s6
; CHECK-NEXT:    vstrb.8 q0, [r1, #16]
; CHECK-NEXT:    vmov.f32 s14, s9
; CHECK-NEXT:    vstrb.8 q3, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <8 x i16>, ptr %src, align 4
  %s2 = getelementptr <8 x i16>, ptr %src, i32 1
  %l2 = load <8 x i16>, ptr %s2, align 4
  %s = shufflevector <8 x i16> %l1, <8 x i16> %l2, <16 x i32> <i32 0, i32 8, i32 1, i32 9, i32 2, i32 10, i32 3, i32 11, i32 4, i32 12, i32 5, i32 13, i32 6, i32 14, i32 7, i32 15>
  store <16 x i16> %s, ptr %dst, align 1
  ret void
}

; i8

define void @vst2_v2i8(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v2i8:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    ldrb r2, [r0]
; CHECK-NEXT:    ldrb r3, [r0, #1]
; CHECK-NEXT:    ldrb.w r12, [r0, #2]
; CHECK-NEXT:    ldrb r0, [r0, #3]
; CHECK-NEXT:    vmov q0[2], q0[0], r2, r3
; CHECK-NEXT:    vmov q0[3], q0[1], r12, r0
; CHECK-NEXT:    vstrb.32 q0, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <2 x i8>, ptr %src, align 4
  %s2 = getelementptr <2 x i8>, ptr %src, i32 1
  %l2 = load <2 x i8>, ptr %s2, align 4
  %s = shufflevector <2 x i8> %l1, <2 x i8> %l2, <4 x i32> <i32 0, i32 2, i32 1, i32 3>
  store <4 x i8> %s, ptr %dst, align 1
  ret void
}

define void @vst2_v4i8(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v4i8:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    vldrb.u32 q0, [r0, #4]
; CHECK-NEXT:    vldrb.u32 q1, [r0]
; CHECK-NEXT:    vmovnt.i32 q1, q0
; CHECK-NEXT:    vstrb.16 q1, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <4 x i8>, ptr %src, align 4
  %s2 = getelementptr <4 x i8>, ptr %src, i32 1
  %l2 = load <4 x i8>, ptr %s2, align 4
  %s = shufflevector <4 x i8> %l1, <4 x i8> %l2, <8 x i32> <i32 0, i32 4, i32 1, i32 5, i32 2, i32 6, i32 3, i32 7>
  store <8 x i8> %s, ptr %dst, align 1
  ret void
}

define void @vst2_v8i8(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v8i8:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    vldrb.u16 q0, [r0, #8]
; CHECK-NEXT:    vldrb.u16 q1, [r0]
; CHECK-NEXT:    vmovnt.i16 q1, q0
; CHECK-NEXT:    vstrb.8 q1, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <8 x i8>, ptr %src, align 4
  %s2 = getelementptr <8 x i8>, ptr %src, i32 1
  %l2 = load <8 x i8>, ptr %s2, align 4
  %s = shufflevector <8 x i8> %l1, <8 x i8> %l2, <16 x i32> <i32 0, i32 8, i32 1, i32 9, i32 2, i32 10, i32 3, i32 11, i32 4, i32 12, i32 5, i32 13, i32 6, i32 14, i32 7, i32 15>
  store <16 x i8> %s, ptr %dst, align 1
  ret void
}

define void @vst2_v16i8(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v16i8:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    vldrw.u32 q1, [r0, #16]
; CHECK-NEXT:    vldrw.u32 q0, [r0]
; CHECK-NEXT:    vst20.8 {q0, q1}, [r1]
; CHECK-NEXT:    vst21.8 {q0, q1}, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <16 x i8>, ptr %src, align 4
  %s2 = getelementptr <16 x i8>, ptr %src, i32 1
  %l2 = load <16 x i8>, ptr %s2, align 4
  %s = shufflevector <16 x i8> %l1, <16 x i8> %l2, <32 x i32> <i32 0, i32 16, i32 1, i32 17, i32 2, i32 18, i32 3, i32 19, i32 4, i32 20, i32 5, i32 21, i32 6, i32 22, i32 7, i32 23, i32 8, i32 24, i32 9, i32 25, i32 10, i32 26, i32 11, i32 27, i32 12, i32 28, i32 13, i32 29, i32 14, i32 30, i32 15, i32 31>
  store <32 x i8> %s, ptr %dst, align 1
  ret void
}

; i64

define void @vst2_v2i64(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v2i64:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    vldrw.u32 q0, [r0, #16]
; CHECK-NEXT:    vldrw.u32 q1, [r0]
; CHECK-NEXT:    vmov.f64 d4, d3
; CHECK-NEXT:    vmov.f64 d5, d1
; CHECK-NEXT:    vmov.f64 d3, d0
; CHECK-NEXT:    vstrw.32 q2, [r1, #16]
; CHECK-NEXT:    vstrw.32 q1, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <2 x i64>, ptr %src, align 4
  %s2 = getelementptr <2 x i64>, ptr %src, i32 1
  %l2 = load <2 x i64>, ptr %s2, align 4
  %s = shufflevector <2 x i64> %l1, <2 x i64> %l2, <4 x i32> <i32 0, i32 2, i32 1, i32 3>
  store <4 x i64> %s, ptr %dst, align 8
  ret void
}

define void @vst2_v4i64(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v4i64:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    .vsave {d8, d9}
; CHECK-NEXT:    vpush {d8, d9}
; CHECK-NEXT:    vldrw.u32 q0, [r0, #32]
; CHECK-NEXT:    vldrw.u32 q1, [r0]
; CHECK-NEXT:    vldrw.u32 q2, [r0, #48]
; CHECK-NEXT:    vldrw.u32 q3, [r0, #16]
; CHECK-NEXT:    vmov.f64 d8, d2
; CHECK-NEXT:    vmov.f64 d9, d0
; CHECK-NEXT:    vmov.f64 d0, d3
; CHECK-NEXT:    vstrw.32 q4, [r1]
; CHECK-NEXT:    vmov.f64 d3, d4
; CHECK-NEXT:    vstrw.32 q0, [r1, #16]
; CHECK-NEXT:    vmov.f64 d2, d6
; CHECK-NEXT:    vmov.f64 d4, d7
; CHECK-NEXT:    vstrw.32 q1, [r1, #32]
; CHECK-NEXT:    vstrw.32 q2, [r1, #48]
; CHECK-NEXT:    vpop {d8, d9}
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <4 x i64>, ptr %src, align 4
  %s2 = getelementptr <4 x i64>, ptr %src, i32 1
  %l2 = load <4 x i64>, ptr %s2, align 4
  %s = shufflevector <4 x i64> %l1, <4 x i64> %l2, <8 x i32> <i32 0, i32 4, i32 1, i32 5, i32 2, i32 6, i32 3, i32 7>
  store <8 x i64> %s, ptr %dst, align 8
  ret void
}

; f32

define void @vst2_v2f32(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v2f32:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    vldr s0, [r0]
; CHECK-NEXT:    vldr s2, [r0, #4]
; CHECK-NEXT:    vldr s1, [r0, #8]
; CHECK-NEXT:    vldr s3, [r0, #12]
; CHECK-NEXT:    vstrw.32 q0, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <2 x float>, ptr %src, align 4
  %s2 = getelementptr <2 x float>, ptr %src, i32 1
  %l2 = load <2 x float>, ptr %s2, align 4
  %s = shufflevector <2 x float> %l1, <2 x float> %l2, <4 x i32> <i32 0, i32 2, i32 1, i32 3>
  store <4 x float> %s, ptr %dst, align 4
  ret void
}

define void @vst2_v4f32(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v4f32:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    vldrw.u32 q1, [r0, #16]
; CHECK-NEXT:    vldrw.u32 q0, [r0]
; CHECK-NEXT:    vst20.32 {q0, q1}, [r1]
; CHECK-NEXT:    vst21.32 {q0, q1}, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <4 x float>, ptr %src, align 4
  %s2 = getelementptr <4 x float>, ptr %src, i32 1
  %l2 = load <4 x float>, ptr %s2, align 4
  %s = shufflevector <4 x float> %l1, <4 x float> %l2, <8 x i32> <i32 0, i32 4, i32 1, i32 5, i32 2, i32 6, i32 3, i32 7>
  store <8 x float> %s, ptr %dst, align 4
  ret void
}

define void @vst2_v8f32(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v8f32:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    vldrw.u32 q1, [r0, #32]
; CHECK-NEXT:    vldrw.u32 q0, [r0]
; CHECK-NEXT:    vldrw.u32 q3, [r0, #48]
; CHECK-NEXT:    vldrw.u32 q2, [r0, #16]
; CHECK-NEXT:    vst20.32 {q0, q1}, [r1]
; CHECK-NEXT:    vst21.32 {q0, q1}, [r1]!
; CHECK-NEXT:    vst20.32 {q2, q3}, [r1]
; CHECK-NEXT:    vst21.32 {q2, q3}, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <8 x float>, ptr %src, align 4
  %s2 = getelementptr <8 x float>, ptr %src, i32 1
  %l2 = load <8 x float>, ptr %s2, align 4
  %s = shufflevector <8 x float> %l1, <8 x float> %l2, <16 x i32> <i32 0, i32 8, i32 1, i32 9, i32 2, i32 10, i32 3, i32 11, i32 4, i32 12, i32 5, i32 13, i32 6, i32 14, i32 7, i32 15>
  store <16 x float> %s, ptr %dst, align 4
  ret void
}

define void @vst2_v16f32(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v16f32:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    .vsave {d8, d9, d10, d11, d12, d13, d14, d15}
; CHECK-NEXT:    vpush {d8, d9, d10, d11, d12, d13, d14, d15}
; CHECK-NEXT:    vldrw.u32 q7, [r0, #64]
; CHECK-NEXT:    vldrw.u32 q6, [r0]
; CHECK-NEXT:    vldrw.u32 q1, [r0, #112]
; CHECK-NEXT:    vldrw.u32 q3, [r0, #96]
; CHECK-NEXT:    vldrw.u32 q5, [r0, #80]
; CHECK-NEXT:    vldrw.u32 q0, [r0, #48]
; CHECK-NEXT:    vldrw.u32 q2, [r0, #32]
; CHECK-NEXT:    vldrw.u32 q4, [r0, #16]
; CHECK-NEXT:    add.w r0, r1, #96
; CHECK-NEXT:    add.w r2, r1, #64
; CHECK-NEXT:    vst20.32 {q6, q7}, [r1]
; CHECK-NEXT:    vst21.32 {q6, q7}, [r1]!
; CHECK-NEXT:    vst20.32 {q4, q5}, [r1]
; CHECK-NEXT:    vst20.32 {q2, q3}, [r2]
; CHECK-NEXT:    vst20.32 {q0, q1}, [r0]
; CHECK-NEXT:    vst21.32 {q4, q5}, [r1]
; CHECK-NEXT:    vst21.32 {q2, q3}, [r2]
; CHECK-NEXT:    vst21.32 {q0, q1}, [r0]
; CHECK-NEXT:    vpop {d8, d9, d10, d11, d12, d13, d14, d15}
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <16 x float>, ptr %src, align 4
  %s2 = getelementptr <16 x float>, ptr %src, i32 1
  %l2 = load <16 x float>, ptr %s2, align 4
  %s = shufflevector <16 x float> %l1, <16 x float> %l2, <32 x i32> <i32 0, i32 16, i32 1, i32 17, i32 2, i32 18, i32 3, i32 19, i32 4, i32 20, i32 5, i32 21, i32 6, i32 22, i32 7, i32 23, i32 8, i32 24, i32 9, i32 25, i32 10, i32 26, i32 11, i32 27, i32 12, i32 28, i32 13, i32 29, i32 14, i32 30, i32 15, i32 31>
  store <32 x float> %s, ptr %dst, align 4
  ret void
}

define void @vst2_v4f32_align1(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v4f32_align1:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    vldrw.u32 q0, [r0, #16]
; CHECK-NEXT:    vldrw.u32 q1, [r0]
; CHECK-NEXT:    vmov.f32 s8, s6
; CHECK-NEXT:    vmov.f32 s9, s2
; CHECK-NEXT:    vmov.f32 s10, s7
; CHECK-NEXT:    vmov.f32 s11, s3
; CHECK-NEXT:    vmov.f32 s12, s4
; CHECK-NEXT:    vstrb.8 q2, [r1, #16]
; CHECK-NEXT:    vmov.f32 s13, s0
; CHECK-NEXT:    vmov.f32 s14, s5
; CHECK-NEXT:    vmov.f32 s15, s1
; CHECK-NEXT:    vstrb.8 q3, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <4 x float>, ptr %src, align 4
  %s2 = getelementptr <4 x float>, ptr %src, i32 1
  %l2 = load <4 x float>, ptr %s2, align 4
  %s = shufflevector <4 x float> %l1, <4 x float> %l2, <8 x i32> <i32 0, i32 4, i32 1, i32 5, i32 2, i32 6, i32 3, i32 7>
  store <8 x float> %s, ptr %dst, align 1
  ret void
}

; f16

define void @vst2_v2f16(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v2f16:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    ldrd r0, r2, [r0]
; CHECK-NEXT:    vmov.32 q1[0], r0
; CHECK-NEXT:    vmov.32 q0[0], r2
; CHECK-NEXT:    vmovx.f16 s5, s4
; CHECK-NEXT:    vins.f16 s4, s0
; CHECK-NEXT:    vmovx.f16 s0, s0
; CHECK-NEXT:    vins.f16 s5, s0
; CHECK-NEXT:    vmov r0, r2, d2
; CHECK-NEXT:    str r2, [r1, #4]
; CHECK-NEXT:    str r0, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <2 x half>, ptr %src, align 4
  %s2 = getelementptr <2 x half>, ptr %src, i32 1
  %l2 = load <2 x half>, ptr %s2, align 4
  %s = shufflevector <2 x half> %l1, <2 x half> %l2, <4 x i32> <i32 0, i32 2, i32 1, i32 3>
  store <4 x half> %s, ptr %dst, align 2
  ret void
}

define void @vst2_v4f16(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v4f16:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    .vsave {d8, d9}
; CHECK-NEXT:    vpush {d8, d9}
; CHECK-NEXT:    ldrd r2, r12, [r0]
; CHECK-NEXT:    ldrd r3, r0, [r0, #8]
; CHECK-NEXT:    vmov.32 q4[0], r2
; CHECK-NEXT:    vmov q0, q4
; CHECK-NEXT:    vmov.32 q3[0], r3
; CHECK-NEXT:    vmov q2, q3
; CHECK-NEXT:    vmov.32 q0[1], r12
; CHECK-NEXT:    vmov.32 q2[1], r0
; CHECK-NEXT:    vmovx.f16 s0, s12
; CHECK-NEXT:    vmovx.f16 s5, s16
; CHECK-NEXT:    vmov.f32 s4, s16
; CHECK-NEXT:    vins.f16 s5, s0
; CHECK-NEXT:    vmovx.f16 s7, s1
; CHECK-NEXT:    vins.f16 s1, s9
; CHECK-NEXT:    vmovx.f16 s0, s9
; CHECK-NEXT:    vins.f16 s4, s12
; CHECK-NEXT:    vins.f16 s7, s0
; CHECK-NEXT:    vmov.f32 s6, s1
; CHECK-NEXT:    vstrh.16 q1, [r1]
; CHECK-NEXT:    vpop {d8, d9}
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <4 x half>, ptr %src, align 4
  %s2 = getelementptr <4 x half>, ptr %src, i32 1
  %l2 = load <4 x half>, ptr %s2, align 4
  %s = shufflevector <4 x half> %l1, <4 x half> %l2, <8 x i32> <i32 0, i32 4, i32 1, i32 5, i32 2, i32 6, i32 3, i32 7>
  store <8 x half> %s, ptr %dst, align 2
  ret void
}

define void @vst2_v8f16(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v8f16:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    vldrw.u32 q1, [r0, #16]
; CHECK-NEXT:    vldrw.u32 q0, [r0]
; CHECK-NEXT:    vst20.16 {q0, q1}, [r1]
; CHECK-NEXT:    vst21.16 {q0, q1}, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <8 x half>, ptr %src, align 4
  %s2 = getelementptr <8 x half>, ptr %src, i32 1
  %l2 = load <8 x half>, ptr %s2, align 4
  %s = shufflevector <8 x half> %l1, <8 x half> %l2, <16 x i32> <i32 0, i32 8, i32 1, i32 9, i32 2, i32 10, i32 3, i32 11, i32 4, i32 12, i32 5, i32 13, i32 6, i32 14, i32 7, i32 15>
  store <16 x half> %s, ptr %dst, align 2
  ret void
}

define void @vst2_v16f16(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v16f16:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    vldrw.u32 q3, [r0, #32]
; CHECK-NEXT:    vldrw.u32 q2, [r0]
; CHECK-NEXT:    vldrw.u32 q1, [r0, #48]
; CHECK-NEXT:    vldrw.u32 q0, [r0, #16]
; CHECK-NEXT:    vst20.16 {q2, q3}, [r1]
; CHECK-NEXT:    vst21.16 {q2, q3}, [r1]!
; CHECK-NEXT:    vst20.16 {q0, q1}, [r1]
; CHECK-NEXT:    vst21.16 {q0, q1}, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <16 x half>, ptr %src, align 4
  %s2 = getelementptr <16 x half>, ptr %src, i32 1
  %l2 = load <16 x half>, ptr %s2, align 4
  %s = shufflevector <16 x half> %l1, <16 x half> %l2, <32 x i32> <i32 0, i32 16, i32 1, i32 17, i32 2, i32 18, i32 3, i32 19, i32 4, i32 20, i32 5, i32 21, i32 6, i32 22, i32 7, i32 23, i32 8, i32 24, i32 9, i32 25, i32 10, i32 26, i32 11, i32 27, i32 12, i32 28, i32 13, i32 29, i32 14, i32 30, i32 15, i32 31>
  store <32 x half> %s, ptr %dst, align 2
  ret void
}

define void @vst2_v8f16_align1(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v8f16_align1:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    vldrw.u32 q1, [r0]
; CHECK-NEXT:    vldrw.u32 q2, [r0, #16]
; CHECK-NEXT:    vmovx.f16 s1, s6
; CHECK-NEXT:    vmovx.f16 s0, s10
; CHECK-NEXT:    vins.f16 s1, s0
; CHECK-NEXT:    vmovx.f16 s3, s7
; CHECK-NEXT:    vmovx.f16 s0, s11
; CHECK-NEXT:    vins.f16 s6, s10
; CHECK-NEXT:    vins.f16 s3, s0
; CHECK-NEXT:    vmovx.f16 s10, s4
; CHECK-NEXT:    vmovx.f16 s0, s8
; CHECK-NEXT:    vins.f16 s7, s11
; CHECK-NEXT:    vins.f16 s4, s8
; CHECK-NEXT:    vins.f16 s10, s0
; CHECK-NEXT:    vmovx.f16 s8, s5
; CHECK-NEXT:    vins.f16 s5, s9
; CHECK-NEXT:    vmovx.f16 s0, s9
; CHECK-NEXT:    vmov q3, q1
; CHECK-NEXT:    vins.f16 s8, s0
; CHECK-NEXT:    vmov.f32 s0, s6
; CHECK-NEXT:    vmov.f32 s2, s7
; CHECK-NEXT:    vmov.f32 s13, s10
; CHECK-NEXT:    vstrb.8 q0, [r1, #16]
; CHECK-NEXT:    vmov.f32 s14, s5
; CHECK-NEXT:    vmov.f32 s15, s8
; CHECK-NEXT:    vstrb.8 q3, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <8 x half>, ptr %src, align 4
  %s2 = getelementptr <8 x half>, ptr %src, i32 1
  %l2 = load <8 x half>, ptr %s2, align 4
  %s = shufflevector <8 x half> %l1, <8 x half> %l2, <16 x i32> <i32 0, i32 8, i32 1, i32 9, i32 2, i32 10, i32 3, i32 11, i32 4, i32 12, i32 5, i32 13, i32 6, i32 14, i32 7, i32 15>
  store <16 x half> %s, ptr %dst, align 1
  ret void
}

; f64

define void @vst2_v2f64(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v2f64:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    vldrw.u32 q0, [r0, #16]
; CHECK-NEXT:    vldrw.u32 q1, [r0]
; CHECK-NEXT:    vmov.f64 d4, d3
; CHECK-NEXT:    vmov.f64 d5, d1
; CHECK-NEXT:    vmov.f64 d3, d0
; CHECK-NEXT:    vstrw.32 q2, [r1, #16]
; CHECK-NEXT:    vstrw.32 q1, [r1]
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <2 x double>, ptr %src, align 4
  %s2 = getelementptr <2 x double>, ptr %src, i32 1
  %l2 = load <2 x double>, ptr %s2, align 4
  %s = shufflevector <2 x double> %l1, <2 x double> %l2, <4 x i32> <i32 0, i32 2, i32 1, i32 3>
  store <4 x double> %s, ptr %dst, align 8
  ret void
}

define void @vst2_v4f64(ptr %src, ptr %dst) {
; CHECK-LABEL: vst2_v4f64:
; CHECK:       @ %bb.0: @ %entry
; CHECK-NEXT:    .vsave {d8, d9}
; CHECK-NEXT:    vpush {d8, d9}
; CHECK-NEXT:    vldrw.u32 q0, [r0, #32]
; CHECK-NEXT:    vldrw.u32 q1, [r0]
; CHECK-NEXT:    vldrw.u32 q2, [r0, #48]
; CHECK-NEXT:    vldrw.u32 q3, [r0, #16]
; CHECK-NEXT:    vmov.f64 d8, d2
; CHECK-NEXT:    vmov.f64 d9, d0
; CHECK-NEXT:    vmov.f64 d0, d3
; CHECK-NEXT:    vstrw.32 q4, [r1]
; CHECK-NEXT:    vmov.f64 d3, d4
; CHECK-NEXT:    vstrw.32 q0, [r1, #16]
; CHECK-NEXT:    vmov.f64 d2, d6
; CHECK-NEXT:    vmov.f64 d4, d7
; CHECK-NEXT:    vstrw.32 q1, [r1, #32]
; CHECK-NEXT:    vstrw.32 q2, [r1, #48]
; CHECK-NEXT:    vpop {d8, d9}
; CHECK-NEXT:    bx lr
entry:
  %l1 = load <4 x double>, ptr %src, align 4
  %s2 = getelementptr <4 x double>, ptr %src, i32 1
  %l2 = load <4 x double>, ptr %s2, align 4
  %s = shufflevector <4 x double> %l1, <4 x double> %l2, <8 x i32> <i32 0, i32 4, i32 1, i32 5, i32 2, i32 6, i32 3, i32 7>
  store <8 x double> %s, ptr %dst, align 8
  ret void
}

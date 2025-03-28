; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt -passes=instcombine -S < %s | FileCheck %s

define ptr @memcpy_nonconst_n(ptr %d, ptr nocapture readonly %s, i64 %n) {
; CHECK-LABEL: @memcpy_nonconst_n(
; CHECK-NEXT:    tail call void @llvm.memcpy.p0.p0.i64(ptr align 1 [[D:%.*]], ptr align 1 [[S:%.*]], i64 [[N:%.*]], i1 false)
; CHECK-NEXT:    [[R:%.*]] = getelementptr inbounds i8, ptr [[D]], i64 [[N]]
; CHECK-NEXT:    ret ptr [[R]]
;
  %r = tail call ptr @mempcpy(ptr %d, ptr %s, i64 %n)
  ret ptr %r
}

define ptr @memcpy_nonconst_n_copy_attrs(ptr %d, ptr nocapture readonly %s, i64 %n) {
; CHECK-LABEL: @memcpy_nonconst_n_copy_attrs(
; CHECK-NEXT:    tail call void @llvm.memcpy.p0.p0.i64(ptr align 1 dereferenceable(16) [[D:%.*]], ptr align 1 [[S:%.*]], i64 [[N:%.*]], i1 false)
; CHECK-NEXT:    [[R:%.*]] = getelementptr inbounds i8, ptr [[D]], i64 [[N]]
; CHECK-NEXT:    ret ptr [[R]]
;
  %r = tail call ptr @mempcpy(ptr dereferenceable(16) %d, ptr %s, i64 %n)
  ret ptr %r
}

define void @memcpy_nonconst_n_unused_retval(ptr %d, ptr nocapture readonly %s, i64 %n) {
; CHECK-LABEL: @memcpy_nonconst_n_unused_retval(
; CHECK-NEXT:    call void @llvm.memcpy.p0.p0.i64(ptr align 1 [[D:%.*]], ptr align 1 [[S:%.*]], i64 [[N:%.*]], i1 false)
; CHECK-NEXT:    ret void
;
  call ptr @mempcpy(ptr %d, ptr %s, i64 %n)
  ret void
}

define ptr @memcpy_small_const_n(ptr %d, ptr nocapture readonly %s) {
; CHECK-LABEL: @memcpy_small_const_n(
; CHECK-NEXT:    [[TMP1:%.*]] = load i64, ptr [[S:%.*]], align 1
; CHECK-NEXT:    store i64 [[TMP1]], ptr [[D:%.*]], align 1
; CHECK-NEXT:    [[R:%.*]] = getelementptr inbounds nuw i8, ptr [[D]], i64 8
; CHECK-NEXT:    ret ptr [[R]]
;
  %r = tail call ptr @mempcpy(ptr %d, ptr %s, i64 8)
  ret ptr %r
}

define ptr @memcpy_big_const_n(ptr %d, ptr nocapture readonly %s) {
; CHECK-LABEL: @memcpy_big_const_n(
; CHECK-NEXT:    tail call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 1 dereferenceable(1024) [[D:%.*]], ptr noundef nonnull align 1 dereferenceable(1024) [[S:%.*]], i64 1024, i1 false)
; CHECK-NEXT:    [[R:%.*]] = getelementptr inbounds nuw i8, ptr [[D]], i64 1024
; CHECK-NEXT:    ret ptr [[R]]
;
  %r = tail call ptr @mempcpy(ptr %d, ptr %s, i64 1024)
  ret ptr %r
}

; The original call may have attributes that can not propagate to memcpy.

define i32 @PR48810() {
; CHECK-LABEL: @PR48810(
; CHECK-NEXT:    store i1 true, ptr poison, align 1
; CHECK-NEXT:    ret i32 undef
;
  %r = call dereferenceable(1) ptr @mempcpy(ptr undef, ptr null, i64 undef)
  ret i32 undef
}

define ptr @memcpy_no_simplify1(ptr %d, ptr nocapture readonly %s, i64 %n) {
; CHECK-LABEL: @memcpy_no_simplify1(
; CHECK-NEXT:    [[R:%.*]] = musttail call ptr @mempcpy(ptr [[D:%.*]], ptr [[S:%.*]], i64 [[N:%.*]])
; CHECK-NEXT:    ret ptr [[R]]
;
  %r = musttail call ptr @mempcpy(ptr %d, ptr %s, i64 %n)
  ret ptr %r
}

declare ptr @mempcpy(ptr, ptr nocapture readonly, i64)

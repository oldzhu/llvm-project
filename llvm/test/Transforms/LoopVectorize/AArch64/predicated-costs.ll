; NOTE: Assertions have been autogenerated by utils/update_test_checks.py UTC_ARGS: --version 5
; RUN: opt -p loop-vectorize -S %s | FileCheck %s

target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-linux"

; Test case from https://github.com/llvm/llvm-project/issues/148431.
define void @test_predicated_load_cast_hint(ptr %dst.1, ptr %dst.2, ptr %src, i8 %n, i64 %off) #0 {
; CHECK-LABEL: define void @test_predicated_load_cast_hint(
; CHECK-SAME: ptr [[DST_1:%.*]], ptr [[DST_2:%.*]], ptr [[SRC:%.*]], i8 [[N:%.*]], i64 [[OFF:%.*]]) {
; CHECK-NEXT:  [[ENTRY:.*]]:
; CHECK-NEXT:    [[N_EXT:%.*]] = sext i8 [[N]] to i32
; CHECK-NEXT:    [[N_SUB:%.*]] = add i32 [[N_EXT]], -15
; CHECK-NEXT:    [[SMAX16:%.*]] = call i32 @llvm.smax.i32(i32 [[N_SUB]], i32 4)
; CHECK-NEXT:    [[TMP0:%.*]] = add nsw i32 [[SMAX16]], -1
; CHECK-NEXT:    [[TMP1:%.*]] = lshr i32 [[TMP0]], 2
; CHECK-NEXT:    [[TMP2:%.*]] = add nuw nsw i32 [[TMP1]], 1
; CHECK-NEXT:    br i1 false, label %[[SCALAR_PH:.*]], label %[[VECTOR_SCEVCHECK:.*]]
; CHECK:       [[VECTOR_SCEVCHECK]]:
; CHECK-NEXT:    [[SMAX:%.*]] = call i32 @llvm.smax.i32(i32 [[N_SUB]], i32 4)
; CHECK-NEXT:    [[TMP3:%.*]] = add nsw i32 [[SMAX]], -1
; CHECK-NEXT:    [[TMP4:%.*]] = lshr i32 [[TMP3]], 2
; CHECK-NEXT:    [[TMP5:%.*]] = trunc i32 [[TMP4]] to i8
; CHECK-NEXT:    [[MUL:%.*]] = call { i8, i1 } @llvm.umul.with.overflow.i8(i8 4, i8 [[TMP5]])
; CHECK-NEXT:    [[MUL_RESULT:%.*]] = extractvalue { i8, i1 } [[MUL]], 0
; CHECK-NEXT:    [[MUL_OVERFLOW:%.*]] = extractvalue { i8, i1 } [[MUL]], 1
; CHECK-NEXT:    [[TMP6:%.*]] = add i8 4, [[MUL_RESULT]]
; CHECK-NEXT:    [[TMP7:%.*]] = icmp ult i8 [[TMP6]], 4
; CHECK-NEXT:    [[TMP8:%.*]] = or i1 [[TMP7]], [[MUL_OVERFLOW]]
; CHECK-NEXT:    [[TMP9:%.*]] = icmp ugt i32 [[TMP4]], 255
; CHECK-NEXT:    [[TMP10:%.*]] = or i1 [[TMP8]], [[TMP9]]
; CHECK-NEXT:    [[TMP11:%.*]] = shl i64 [[OFF]], 3
; CHECK-NEXT:    [[SCEVGEP:%.*]] = getelementptr i8, ptr [[DST_1]], i64 [[TMP11]]
; CHECK-NEXT:    [[TMP12:%.*]] = zext i32 [[TMP4]] to i64
; CHECK-NEXT:    [[MUL1:%.*]] = call { i64, i1 } @llvm.umul.with.overflow.i64(i64 512, i64 [[TMP12]])
; CHECK-NEXT:    [[MUL_RESULT2:%.*]] = extractvalue { i64, i1 } [[MUL1]], 0
; CHECK-NEXT:    [[MUL_OVERFLOW3:%.*]] = extractvalue { i64, i1 } [[MUL1]], 1
; CHECK-NEXT:    [[TMP13:%.*]] = sub i64 0, [[MUL_RESULT2]]
; CHECK-NEXT:    [[TMP14:%.*]] = getelementptr i8, ptr [[SCEVGEP]], i64 [[MUL_RESULT2]]
; CHECK-NEXT:    [[TMP15:%.*]] = icmp ult ptr [[TMP14]], [[SCEVGEP]]
; CHECK-NEXT:    [[TMP16:%.*]] = or i1 [[TMP15]], [[MUL_OVERFLOW3]]
; CHECK-NEXT:    [[TMP17:%.*]] = or i1 [[TMP10]], [[TMP16]]
; CHECK-NEXT:    br i1 [[TMP17]], label %[[SCALAR_PH]], label %[[VECTOR_MEMCHECK:.*]]
; CHECK:       [[VECTOR_MEMCHECK]]:
; CHECK-NEXT:    [[SCEVGEP4:%.*]] = getelementptr i8, ptr [[DST_2]], i64 1
; CHECK-NEXT:    [[SCEVGEP5:%.*]] = getelementptr i8, ptr [[SRC]], i64 1
; CHECK-NEXT:    [[TMP18:%.*]] = shl i64 [[OFF]], 3
; CHECK-NEXT:    [[SCEVGEP6:%.*]] = getelementptr i8, ptr [[DST_1]], i64 [[TMP18]]
; CHECK-NEXT:    [[SMAX7:%.*]] = call i32 @llvm.smax.i32(i32 [[N_SUB]], i32 4)
; CHECK-NEXT:    [[TMP19:%.*]] = add nsw i32 [[SMAX7]], -1
; CHECK-NEXT:    [[TMP20:%.*]] = zext nneg i32 [[TMP19]] to i64
; CHECK-NEXT:    [[TMP21:%.*]] = lshr i64 [[TMP20]], 2
; CHECK-NEXT:    [[TMP22:%.*]] = shl nuw nsw i64 [[TMP21]], 9
; CHECK-NEXT:    [[TMP23:%.*]] = add i64 [[TMP22]], [[TMP18]]
; CHECK-NEXT:    [[TMP24:%.*]] = add i64 [[TMP23]], 8
; CHECK-NEXT:    [[SCEVGEP8:%.*]] = getelementptr i8, ptr [[DST_1]], i64 [[TMP24]]
; CHECK-NEXT:    [[BOUND0:%.*]] = icmp ult ptr [[DST_2]], [[SCEVGEP5]]
; CHECK-NEXT:    [[BOUND1:%.*]] = icmp ult ptr [[SRC]], [[SCEVGEP4]]
; CHECK-NEXT:    [[FOUND_CONFLICT:%.*]] = and i1 [[BOUND0]], [[BOUND1]]
; CHECK-NEXT:    [[BOUND09:%.*]] = icmp ult ptr [[DST_2]], [[SCEVGEP8]]
; CHECK-NEXT:    [[BOUND110:%.*]] = icmp ult ptr [[SCEVGEP6]], [[SCEVGEP4]]
; CHECK-NEXT:    [[FOUND_CONFLICT11:%.*]] = and i1 [[BOUND09]], [[BOUND110]]
; CHECK-NEXT:    [[CONFLICT_RDX:%.*]] = or i1 [[FOUND_CONFLICT]], [[FOUND_CONFLICT11]]
; CHECK-NEXT:    [[BOUND012:%.*]] = icmp ult ptr [[SRC]], [[SCEVGEP8]]
; CHECK-NEXT:    [[BOUND113:%.*]] = icmp ult ptr [[SCEVGEP6]], [[SCEVGEP5]]
; CHECK-NEXT:    [[FOUND_CONFLICT14:%.*]] = and i1 [[BOUND012]], [[BOUND113]]
; CHECK-NEXT:    [[CONFLICT_RDX15:%.*]] = or i1 [[CONFLICT_RDX]], [[FOUND_CONFLICT14]]
; CHECK-NEXT:    br i1 [[CONFLICT_RDX15]], label %[[SCALAR_PH]], label %[[VECTOR_PH:.*]]
; CHECK:       [[VECTOR_PH]]:
; CHECK-NEXT:    [[N_RND_UP:%.*]] = add i32 [[TMP2]], 15
; CHECK-NEXT:    [[N_MOD_VF:%.*]] = urem i32 [[N_RND_UP]], 16
; CHECK-NEXT:    [[N_VEC:%.*]] = sub i32 [[N_RND_UP]], [[N_MOD_VF]]
; CHECK-NEXT:    [[TRIP_COUNT_MINUS_1:%.*]] = sub i32 [[TMP2]], 1
; CHECK-NEXT:    [[BROADCAST_SPLATINSERT:%.*]] = insertelement <16 x i32> poison, i32 [[TRIP_COUNT_MINUS_1]], i64 0
; CHECK-NEXT:    [[BROADCAST_SPLAT:%.*]] = shufflevector <16 x i32> [[BROADCAST_SPLATINSERT]], <16 x i32> poison, <16 x i32> zeroinitializer
; CHECK-NEXT:    br label %[[VECTOR_BODY:.*]]
; CHECK:       [[VECTOR_BODY]]:
; CHECK-NEXT:    [[INDEX:%.*]] = phi i32 [ 0, %[[VECTOR_PH]] ], [ [[INDEX_NEXT:%.*]], %[[PRED_STORE_CONTINUE50:.*]] ]
; CHECK-NEXT:    [[DOTCAST:%.*]] = trunc i32 [[INDEX]] to i8
; CHECK-NEXT:    [[OFFSET_IDX:%.*]] = mul i8 [[DOTCAST]], 4
; CHECK-NEXT:    [[BROADCAST_SPLATINSERT17:%.*]] = insertelement <16 x i32> poison, i32 [[INDEX]], i64 0
; CHECK-NEXT:    [[BROADCAST_SPLAT18:%.*]] = shufflevector <16 x i32> [[BROADCAST_SPLATINSERT17]], <16 x i32> poison, <16 x i32> zeroinitializer
; CHECK-NEXT:    [[VEC_IV:%.*]] = add <16 x i32> [[BROADCAST_SPLAT18]], <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
; CHECK-NEXT:    [[TMP25:%.*]] = icmp ule <16 x i32> [[VEC_IV]], [[BROADCAST_SPLAT]]
; CHECK-NEXT:    [[TMP26:%.*]] = load i8, ptr [[SRC]], align 1, !alias.scope [[META0:![0-9]+]], !noalias [[META3:![0-9]+]]
; CHECK-NEXT:    [[BROADCAST_SPLATINSERT19:%.*]] = insertelement <16 x i8> poison, i8 [[TMP26]], i64 0
; CHECK-NEXT:    [[BROADCAST_SPLAT20:%.*]] = shufflevector <16 x i8> [[BROADCAST_SPLATINSERT19]], <16 x i8> poison, <16 x i32> zeroinitializer
; CHECK-NEXT:    [[TMP27:%.*]] = zext <16 x i8> [[BROADCAST_SPLAT20]] to <16 x i64>
; CHECK-NEXT:    [[TMP28:%.*]] = extractelement <16 x i1> [[TMP25]], i32 0
; CHECK-NEXT:    br i1 [[TMP28]], label %[[PRED_STORE_IF:.*]], label %[[PRED_STORE_CONTINUE:.*]]
; CHECK:       [[PRED_STORE_IF]]:
; CHECK-NEXT:    [[TMP29:%.*]] = add i8 [[OFFSET_IDX]], 0
; CHECK-NEXT:    [[TMP30:%.*]] = zext i8 [[TMP29]] to i64
; CHECK-NEXT:    [[TMP31:%.*]] = getelementptr [16 x i64], ptr [[DST_1]], i64 [[TMP30]], i64 [[OFF]]
; CHECK-NEXT:    [[TMP32:%.*]] = extractelement <16 x i64> [[TMP27]], i32 0
; CHECK-NEXT:    [[TMP33:%.*]] = or i64 [[TMP32]], 1
; CHECK-NEXT:    store i64 [[TMP33]], ptr [[TMP31]], align 8, !alias.scope [[META3]]
; CHECK-NEXT:    br label %[[PRED_STORE_CONTINUE]]
; CHECK:       [[PRED_STORE_CONTINUE]]:
; CHECK-NEXT:    [[TMP34:%.*]] = extractelement <16 x i1> [[TMP25]], i32 1
; CHECK-NEXT:    br i1 [[TMP34]], label %[[PRED_STORE_IF21:.*]], label %[[PRED_STORE_CONTINUE22:.*]]
; CHECK:       [[PRED_STORE_IF21]]:
; CHECK-NEXT:    [[TMP35:%.*]] = add i8 [[OFFSET_IDX]], 4
; CHECK-NEXT:    [[TMP36:%.*]] = zext i8 [[TMP35]] to i64
; CHECK-NEXT:    [[TMP37:%.*]] = getelementptr [16 x i64], ptr [[DST_1]], i64 [[TMP36]], i64 [[OFF]]
; CHECK-NEXT:    [[TMP38:%.*]] = extractelement <16 x i64> [[TMP27]], i32 1
; CHECK-NEXT:    [[TMP39:%.*]] = or i64 [[TMP38]], 1
; CHECK-NEXT:    store i64 [[TMP39]], ptr [[TMP37]], align 8, !alias.scope [[META3]]
; CHECK-NEXT:    br label %[[PRED_STORE_CONTINUE22]]
; CHECK:       [[PRED_STORE_CONTINUE22]]:
; CHECK-NEXT:    [[TMP40:%.*]] = extractelement <16 x i1> [[TMP25]], i32 2
; CHECK-NEXT:    br i1 [[TMP40]], label %[[PRED_STORE_IF23:.*]], label %[[PRED_STORE_CONTINUE24:.*]]
; CHECK:       [[PRED_STORE_IF23]]:
; CHECK-NEXT:    [[TMP41:%.*]] = add i8 [[OFFSET_IDX]], 8
; CHECK-NEXT:    [[TMP42:%.*]] = zext i8 [[TMP41]] to i64
; CHECK-NEXT:    [[TMP43:%.*]] = getelementptr [16 x i64], ptr [[DST_1]], i64 [[TMP42]], i64 [[OFF]]
; CHECK-NEXT:    [[TMP44:%.*]] = extractelement <16 x i64> [[TMP27]], i32 2
; CHECK-NEXT:    [[TMP45:%.*]] = or i64 [[TMP44]], 1
; CHECK-NEXT:    store i64 [[TMP45]], ptr [[TMP43]], align 8, !alias.scope [[META3]]
; CHECK-NEXT:    br label %[[PRED_STORE_CONTINUE24]]
; CHECK:       [[PRED_STORE_CONTINUE24]]:
; CHECK-NEXT:    [[TMP46:%.*]] = extractelement <16 x i1> [[TMP25]], i32 3
; CHECK-NEXT:    br i1 [[TMP46]], label %[[PRED_STORE_IF25:.*]], label %[[PRED_STORE_CONTINUE26:.*]]
; CHECK:       [[PRED_STORE_IF25]]:
; CHECK-NEXT:    [[TMP47:%.*]] = add i8 [[OFFSET_IDX]], 12
; CHECK-NEXT:    [[TMP48:%.*]] = zext i8 [[TMP47]] to i64
; CHECK-NEXT:    [[TMP49:%.*]] = getelementptr [16 x i64], ptr [[DST_1]], i64 [[TMP48]], i64 [[OFF]]
; CHECK-NEXT:    [[TMP50:%.*]] = extractelement <16 x i64> [[TMP27]], i32 3
; CHECK-NEXT:    [[TMP51:%.*]] = or i64 [[TMP50]], 1
; CHECK-NEXT:    store i64 [[TMP51]], ptr [[TMP49]], align 8, !alias.scope [[META3]]
; CHECK-NEXT:    br label %[[PRED_STORE_CONTINUE26]]
; CHECK:       [[PRED_STORE_CONTINUE26]]:
; CHECK-NEXT:    [[TMP52:%.*]] = extractelement <16 x i1> [[TMP25]], i32 4
; CHECK-NEXT:    br i1 [[TMP52]], label %[[PRED_STORE_IF27:.*]], label %[[PRED_STORE_CONTINUE28:.*]]
; CHECK:       [[PRED_STORE_IF27]]:
; CHECK-NEXT:    [[TMP53:%.*]] = add i8 [[OFFSET_IDX]], 16
; CHECK-NEXT:    [[TMP54:%.*]] = zext i8 [[TMP53]] to i64
; CHECK-NEXT:    [[TMP55:%.*]] = getelementptr [16 x i64], ptr [[DST_1]], i64 [[TMP54]], i64 [[OFF]]
; CHECK-NEXT:    [[TMP56:%.*]] = extractelement <16 x i64> [[TMP27]], i32 4
; CHECK-NEXT:    [[TMP57:%.*]] = or i64 [[TMP56]], 1
; CHECK-NEXT:    store i64 [[TMP57]], ptr [[TMP55]], align 8, !alias.scope [[META3]]
; CHECK-NEXT:    br label %[[PRED_STORE_CONTINUE28]]
; CHECK:       [[PRED_STORE_CONTINUE28]]:
; CHECK-NEXT:    [[TMP58:%.*]] = extractelement <16 x i1> [[TMP25]], i32 5
; CHECK-NEXT:    br i1 [[TMP58]], label %[[PRED_STORE_IF29:.*]], label %[[PRED_STORE_CONTINUE30:.*]]
; CHECK:       [[PRED_STORE_IF29]]:
; CHECK-NEXT:    [[TMP59:%.*]] = add i8 [[OFFSET_IDX]], 20
; CHECK-NEXT:    [[TMP60:%.*]] = zext i8 [[TMP59]] to i64
; CHECK-NEXT:    [[TMP61:%.*]] = getelementptr [16 x i64], ptr [[DST_1]], i64 [[TMP60]], i64 [[OFF]]
; CHECK-NEXT:    [[TMP62:%.*]] = extractelement <16 x i64> [[TMP27]], i32 5
; CHECK-NEXT:    [[TMP63:%.*]] = or i64 [[TMP62]], 1
; CHECK-NEXT:    store i64 [[TMP63]], ptr [[TMP61]], align 8, !alias.scope [[META3]]
; CHECK-NEXT:    br label %[[PRED_STORE_CONTINUE30]]
; CHECK:       [[PRED_STORE_CONTINUE30]]:
; CHECK-NEXT:    [[TMP64:%.*]] = extractelement <16 x i1> [[TMP25]], i32 6
; CHECK-NEXT:    br i1 [[TMP64]], label %[[PRED_STORE_IF31:.*]], label %[[PRED_STORE_CONTINUE32:.*]]
; CHECK:       [[PRED_STORE_IF31]]:
; CHECK-NEXT:    [[TMP65:%.*]] = add i8 [[OFFSET_IDX]], 24
; CHECK-NEXT:    [[TMP66:%.*]] = zext i8 [[TMP65]] to i64
; CHECK-NEXT:    [[TMP67:%.*]] = getelementptr [16 x i64], ptr [[DST_1]], i64 [[TMP66]], i64 [[OFF]]
; CHECK-NEXT:    [[TMP68:%.*]] = extractelement <16 x i64> [[TMP27]], i32 6
; CHECK-NEXT:    [[TMP69:%.*]] = or i64 [[TMP68]], 1
; CHECK-NEXT:    store i64 [[TMP69]], ptr [[TMP67]], align 8, !alias.scope [[META3]]
; CHECK-NEXT:    br label %[[PRED_STORE_CONTINUE32]]
; CHECK:       [[PRED_STORE_CONTINUE32]]:
; CHECK-NEXT:    [[TMP70:%.*]] = extractelement <16 x i1> [[TMP25]], i32 7
; CHECK-NEXT:    br i1 [[TMP70]], label %[[PRED_STORE_IF33:.*]], label %[[PRED_STORE_CONTINUE34:.*]]
; CHECK:       [[PRED_STORE_IF33]]:
; CHECK-NEXT:    [[TMP71:%.*]] = add i8 [[OFFSET_IDX]], 28
; CHECK-NEXT:    [[TMP72:%.*]] = zext i8 [[TMP71]] to i64
; CHECK-NEXT:    [[TMP73:%.*]] = getelementptr [16 x i64], ptr [[DST_1]], i64 [[TMP72]], i64 [[OFF]]
; CHECK-NEXT:    [[TMP74:%.*]] = extractelement <16 x i64> [[TMP27]], i32 7
; CHECK-NEXT:    [[TMP75:%.*]] = or i64 [[TMP74]], 1
; CHECK-NEXT:    store i64 [[TMP75]], ptr [[TMP73]], align 8, !alias.scope [[META3]]
; CHECK-NEXT:    br label %[[PRED_STORE_CONTINUE34]]
; CHECK:       [[PRED_STORE_CONTINUE34]]:
; CHECK-NEXT:    [[TMP76:%.*]] = extractelement <16 x i1> [[TMP25]], i32 8
; CHECK-NEXT:    br i1 [[TMP76]], label %[[PRED_STORE_IF35:.*]], label %[[PRED_STORE_CONTINUE36:.*]]
; CHECK:       [[PRED_STORE_IF35]]:
; CHECK-NEXT:    [[TMP77:%.*]] = add i8 [[OFFSET_IDX]], 32
; CHECK-NEXT:    [[TMP78:%.*]] = zext i8 [[TMP77]] to i64
; CHECK-NEXT:    [[TMP79:%.*]] = getelementptr [16 x i64], ptr [[DST_1]], i64 [[TMP78]], i64 [[OFF]]
; CHECK-NEXT:    [[TMP80:%.*]] = extractelement <16 x i64> [[TMP27]], i32 8
; CHECK-NEXT:    [[TMP81:%.*]] = or i64 [[TMP80]], 1
; CHECK-NEXT:    store i64 [[TMP81]], ptr [[TMP79]], align 8, !alias.scope [[META3]]
; CHECK-NEXT:    br label %[[PRED_STORE_CONTINUE36]]
; CHECK:       [[PRED_STORE_CONTINUE36]]:
; CHECK-NEXT:    [[TMP82:%.*]] = extractelement <16 x i1> [[TMP25]], i32 9
; CHECK-NEXT:    br i1 [[TMP82]], label %[[PRED_STORE_IF37:.*]], label %[[PRED_STORE_CONTINUE38:.*]]
; CHECK:       [[PRED_STORE_IF37]]:
; CHECK-NEXT:    [[TMP83:%.*]] = add i8 [[OFFSET_IDX]], 36
; CHECK-NEXT:    [[TMP84:%.*]] = zext i8 [[TMP83]] to i64
; CHECK-NEXT:    [[TMP85:%.*]] = getelementptr [16 x i64], ptr [[DST_1]], i64 [[TMP84]], i64 [[OFF]]
; CHECK-NEXT:    [[TMP86:%.*]] = extractelement <16 x i64> [[TMP27]], i32 9
; CHECK-NEXT:    [[TMP87:%.*]] = or i64 [[TMP86]], 1
; CHECK-NEXT:    store i64 [[TMP87]], ptr [[TMP85]], align 8, !alias.scope [[META3]]
; CHECK-NEXT:    br label %[[PRED_STORE_CONTINUE38]]
; CHECK:       [[PRED_STORE_CONTINUE38]]:
; CHECK-NEXT:    [[TMP88:%.*]] = extractelement <16 x i1> [[TMP25]], i32 10
; CHECK-NEXT:    br i1 [[TMP88]], label %[[PRED_STORE_IF39:.*]], label %[[PRED_STORE_CONTINUE40:.*]]
; CHECK:       [[PRED_STORE_IF39]]:
; CHECK-NEXT:    [[TMP89:%.*]] = add i8 [[OFFSET_IDX]], 40
; CHECK-NEXT:    [[TMP90:%.*]] = zext i8 [[TMP89]] to i64
; CHECK-NEXT:    [[TMP91:%.*]] = getelementptr [16 x i64], ptr [[DST_1]], i64 [[TMP90]], i64 [[OFF]]
; CHECK-NEXT:    [[TMP92:%.*]] = extractelement <16 x i64> [[TMP27]], i32 10
; CHECK-NEXT:    [[TMP93:%.*]] = or i64 [[TMP92]], 1
; CHECK-NEXT:    store i64 [[TMP93]], ptr [[TMP91]], align 8, !alias.scope [[META3]]
; CHECK-NEXT:    br label %[[PRED_STORE_CONTINUE40]]
; CHECK:       [[PRED_STORE_CONTINUE40]]:
; CHECK-NEXT:    [[TMP94:%.*]] = extractelement <16 x i1> [[TMP25]], i32 11
; CHECK-NEXT:    br i1 [[TMP94]], label %[[PRED_STORE_IF41:.*]], label %[[PRED_STORE_CONTINUE42:.*]]
; CHECK:       [[PRED_STORE_IF41]]:
; CHECK-NEXT:    [[TMP95:%.*]] = add i8 [[OFFSET_IDX]], 44
; CHECK-NEXT:    [[TMP96:%.*]] = zext i8 [[TMP95]] to i64
; CHECK-NEXT:    [[TMP97:%.*]] = getelementptr [16 x i64], ptr [[DST_1]], i64 [[TMP96]], i64 [[OFF]]
; CHECK-NEXT:    [[TMP98:%.*]] = extractelement <16 x i64> [[TMP27]], i32 11
; CHECK-NEXT:    [[TMP99:%.*]] = or i64 [[TMP98]], 1
; CHECK-NEXT:    store i64 [[TMP99]], ptr [[TMP97]], align 8, !alias.scope [[META3]]
; CHECK-NEXT:    br label %[[PRED_STORE_CONTINUE42]]
; CHECK:       [[PRED_STORE_CONTINUE42]]:
; CHECK-NEXT:    [[TMP100:%.*]] = extractelement <16 x i1> [[TMP25]], i32 12
; CHECK-NEXT:    br i1 [[TMP100]], label %[[PRED_STORE_IF43:.*]], label %[[PRED_STORE_CONTINUE44:.*]]
; CHECK:       [[PRED_STORE_IF43]]:
; CHECK-NEXT:    [[TMP101:%.*]] = add i8 [[OFFSET_IDX]], 48
; CHECK-NEXT:    [[TMP102:%.*]] = zext i8 [[TMP101]] to i64
; CHECK-NEXT:    [[TMP103:%.*]] = getelementptr [16 x i64], ptr [[DST_1]], i64 [[TMP102]], i64 [[OFF]]
; CHECK-NEXT:    [[TMP104:%.*]] = extractelement <16 x i64> [[TMP27]], i32 12
; CHECK-NEXT:    [[TMP105:%.*]] = or i64 [[TMP104]], 1
; CHECK-NEXT:    store i64 [[TMP105]], ptr [[TMP103]], align 8, !alias.scope [[META3]]
; CHECK-NEXT:    br label %[[PRED_STORE_CONTINUE44]]
; CHECK:       [[PRED_STORE_CONTINUE44]]:
; CHECK-NEXT:    [[TMP106:%.*]] = extractelement <16 x i1> [[TMP25]], i32 13
; CHECK-NEXT:    br i1 [[TMP106]], label %[[PRED_STORE_IF45:.*]], label %[[PRED_STORE_CONTINUE46:.*]]
; CHECK:       [[PRED_STORE_IF45]]:
; CHECK-NEXT:    [[TMP107:%.*]] = add i8 [[OFFSET_IDX]], 52
; CHECK-NEXT:    [[TMP108:%.*]] = zext i8 [[TMP107]] to i64
; CHECK-NEXT:    [[TMP109:%.*]] = getelementptr [16 x i64], ptr [[DST_1]], i64 [[TMP108]], i64 [[OFF]]
; CHECK-NEXT:    [[TMP110:%.*]] = extractelement <16 x i64> [[TMP27]], i32 13
; CHECK-NEXT:    [[TMP111:%.*]] = or i64 [[TMP110]], 1
; CHECK-NEXT:    store i64 [[TMP111]], ptr [[TMP109]], align 8, !alias.scope [[META3]]
; CHECK-NEXT:    br label %[[PRED_STORE_CONTINUE46]]
; CHECK:       [[PRED_STORE_CONTINUE46]]:
; CHECK-NEXT:    [[TMP112:%.*]] = extractelement <16 x i1> [[TMP25]], i32 14
; CHECK-NEXT:    br i1 [[TMP112]], label %[[PRED_STORE_IF47:.*]], label %[[PRED_STORE_CONTINUE48:.*]]
; CHECK:       [[PRED_STORE_IF47]]:
; CHECK-NEXT:    [[TMP113:%.*]] = add i8 [[OFFSET_IDX]], 56
; CHECK-NEXT:    [[TMP114:%.*]] = zext i8 [[TMP113]] to i64
; CHECK-NEXT:    [[TMP115:%.*]] = getelementptr [16 x i64], ptr [[DST_1]], i64 [[TMP114]], i64 [[OFF]]
; CHECK-NEXT:    [[TMP116:%.*]] = extractelement <16 x i64> [[TMP27]], i32 14
; CHECK-NEXT:    [[TMP117:%.*]] = or i64 [[TMP116]], 1
; CHECK-NEXT:    store i64 [[TMP117]], ptr [[TMP115]], align 8, !alias.scope [[META3]]
; CHECK-NEXT:    br label %[[PRED_STORE_CONTINUE48]]
; CHECK:       [[PRED_STORE_CONTINUE48]]:
; CHECK-NEXT:    [[TMP118:%.*]] = extractelement <16 x i1> [[TMP25]], i32 15
; CHECK-NEXT:    br i1 [[TMP118]], label %[[PRED_STORE_IF49:.*]], label %[[PRED_STORE_CONTINUE50]]
; CHECK:       [[PRED_STORE_IF49]]:
; CHECK-NEXT:    [[TMP119:%.*]] = add i8 [[OFFSET_IDX]], 60
; CHECK-NEXT:    [[TMP120:%.*]] = zext i8 [[TMP119]] to i64
; CHECK-NEXT:    [[TMP121:%.*]] = getelementptr [16 x i64], ptr [[DST_1]], i64 [[TMP120]], i64 [[OFF]]
; CHECK-NEXT:    [[TMP122:%.*]] = extractelement <16 x i64> [[TMP27]], i32 15
; CHECK-NEXT:    [[TMP123:%.*]] = or i64 [[TMP122]], 1
; CHECK-NEXT:    store i64 [[TMP123]], ptr [[TMP121]], align 8, !alias.scope [[META3]]
; CHECK-NEXT:    br label %[[PRED_STORE_CONTINUE50]]
; CHECK:       [[PRED_STORE_CONTINUE50]]:
; CHECK-NEXT:    store i8 0, ptr [[DST_2]], align 1, !alias.scope [[META5:![0-9]+]], !noalias [[META7:![0-9]+]]
; CHECK-NEXT:    [[INDEX_NEXT]] = add nuw i32 [[INDEX]], 16
; CHECK-NEXT:    [[TMP124:%.*]] = icmp eq i32 [[INDEX_NEXT]], [[N_VEC]]
; CHECK-NEXT:    br i1 [[TMP124]], label %[[MIDDLE_BLOCK:.*]], label %[[VECTOR_BODY]], !llvm.loop [[LOOP8:![0-9]+]]
; CHECK:       [[MIDDLE_BLOCK]]:
; CHECK-NEXT:    br label %[[EXIT:.*]]
; CHECK:       [[SCALAR_PH]]:
; CHECK-NEXT:    [[BC_RESUME_VAL:%.*]] = phi i8 [ 0, %[[ENTRY]] ], [ 0, %[[VECTOR_SCEVCHECK]] ], [ 0, %[[VECTOR_MEMCHECK]] ]
; CHECK-NEXT:    br label %[[LOOP:.*]]
; CHECK:       [[LOOP]]:
; CHECK-NEXT:    [[IV:%.*]] = phi i8 [ [[BC_RESUME_VAL]], %[[SCALAR_PH]] ], [ [[IV_NEXT:%.*]], %[[LOOP]] ]
; CHECK-NEXT:    [[L:%.*]] = load i8, ptr [[SRC]], align 1
; CHECK-NEXT:    [[L_EXT:%.*]] = zext i8 [[L]] to i64
; CHECK-NEXT:    [[ADD:%.*]] = or i64 [[L_EXT]], 1
; CHECK-NEXT:    [[IV_EXT:%.*]] = zext i8 [[IV]] to i64
; CHECK-NEXT:    [[GEP_DST_1:%.*]] = getelementptr [16 x i64], ptr [[DST_1]], i64 [[IV_EXT]], i64 [[OFF]]
; CHECK-NEXT:    store i64 [[ADD]], ptr [[GEP_DST_1]], align 8
; CHECK-NEXT:    store i8 0, ptr [[DST_2]], align 1
; CHECK-NEXT:    [[IV_NEXT]] = add i8 [[IV]], 4
; CHECK-NEXT:    [[IV_NEXT_EXT:%.*]] = zext i8 [[IV_NEXT]] to i32
; CHECK-NEXT:    [[CMP:%.*]] = icmp sgt i32 [[N_SUB]], [[IV_NEXT_EXT]]
; CHECK-NEXT:    br i1 [[CMP]], label %[[LOOP]], label %[[EXIT]], !llvm.loop [[LOOP12:![0-9]+]]
; CHECK:       [[EXIT]]:
; CHECK-NEXT:    ret void
;
entry:
  %n.ext = sext i8 %n to i32
  %n.sub = add i32 %n.ext, -15
  br label %loop

loop:
  %iv = phi i8 [ 0, %entry ], [ %iv.next, %loop ]
  %l = load i8, ptr %src, align 1
  %l.ext = zext i8 %l to i64
  %add = or i64 %l.ext, 1
  %iv.ext = zext i8 %iv to i64
  %gep.dst.1 = getelementptr [16 x i64], ptr %dst.1, i64 %iv.ext, i64 %off
  store i64 %add, ptr %gep.dst.1, align 8
  store i8 0, ptr %dst.2, align 1
  %iv.next = add i8 %iv, 4
  %iv.next.ext = zext i8 %iv.next to i32
  %cmp = icmp sgt i32 %n.sub, %iv.next.ext
  br i1 %cmp, label %loop, label %exit, !llvm.loop !0

exit:
  ret void
}

!0 = distinct !{!0, !1, !2, !3}
!1 = !{!"llvm.loop.mustprogress"}
!2 = !{!"llvm.loop.vectorize.predicate.enable", i1 true}
!3 = !{!"llvm.loop.vectorize.enable", i1 true}
;.
; CHECK: [[META0]] = !{[[META1:![0-9]+]]}
; CHECK: [[META1]] = distinct !{[[META1]], [[META2:![0-9]+]]}
; CHECK: [[META2]] = distinct !{[[META2]], !"LVerDomain"}
; CHECK: [[META3]] = !{[[META4:![0-9]+]]}
; CHECK: [[META4]] = distinct !{[[META4]], [[META2]]}
; CHECK: [[META5]] = !{[[META6:![0-9]+]]}
; CHECK: [[META6]] = distinct !{[[META6]], [[META2]]}
; CHECK: [[META7]] = !{[[META1]], [[META4]]}
; CHECK: [[LOOP8]] = distinct !{[[LOOP8]], [[META9:![0-9]+]], [[META10:![0-9]+]], [[META11:![0-9]+]]}
; CHECK: [[META9]] = !{!"llvm.loop.mustprogress"}
; CHECK: [[META10]] = !{!"llvm.loop.isvectorized", i32 1}
; CHECK: [[META11]] = !{!"llvm.loop.unroll.runtime.disable"}
; CHECK: [[LOOP12]] = distinct !{[[LOOP12]], [[META9]], [[META10]]}
;.

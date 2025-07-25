//===- TosaTestPasses.cpp -------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Test passes to exercise TOSA helper functions.
//
//===----------------------------------------------------------------------===//

#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/Tensor/IR/Tensor.h"
#include "mlir/Dialect/Tosa/IR/TosaOps.h"
#include "mlir/Dialect/Tosa/Transforms/Passes.h"
#include "mlir/Dialect/Tosa/Utils/QuantUtils.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/Matchers.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"

#define PASS_NAME "tosa-test-quant-utils"

using namespace mlir;
using namespace mlir::tosa;

// This transformation converts quantized uint8 to quantized int8. The
// construction of the new type invokes buildQTypeFromMinMax. Extracted from
// TOSA legalization infrastructure.
struct ConvertTosaNegateOp : public RewritePattern {
  explicit ConvertTosaNegateOp(MLIRContext *context)
      : RewritePattern(tosa::NegateOp::getOperationName(), 1, context) {}
  LogicalResult matchAndRewrite(Operation *op,
                                PatternRewriter &rewriter) const override;
};

LogicalResult
ConvertTosaNegateOp::matchAndRewrite(Operation *op,
                                     PatternRewriter &rewriter) const {

  auto tosaNegateOp = cast<tosa::NegateOp>(op);

  auto inputType =
      dyn_cast<mlir::RankedTensorType>(tosaNegateOp.getInput1().getType());
  // skip if input is not ranked tensor type
  if (!inputType)
    return failure();

  // skip if it's not ranked tensor type.
  auto outputType =
      dyn_cast<mlir::RankedTensorType>(tosaNegateOp.getResult().getType());
  if (!outputType)
    return failure();

  // skip if output is not per-tensor quantized type.
  auto outputElementType =
      dyn_cast<mlir::quant::UniformQuantizedType>(outputType.getElementType());
  if (!outputElementType)
    return failure();

  // skip if output is not uint8.
  if (outputElementType.isSigned() ||
      outputElementType.getStorageTypeIntegralWidth() != 8)
    return failure();

  double typeRangeMin = double(outputElementType.getStorageTypeMin() -
                               outputElementType.getZeroPoint()) *
                        outputElementType.getScale();
  double typeRangeMax = double(outputElementType.getStorageTypeMax() -
                               outputElementType.getZeroPoint()) *
                        outputElementType.getScale();
  bool narrowRange = outputElementType.getStorageTypeMin() == 1;

  auto dstQConstType = RankedTensorType::get(
      outputType.getShape(),
      buildQTypeFromMinMax(rewriter, outputElementType.getExpressedType(),
                           rewriter.getF64FloatAttr(typeRangeMin),
                           rewriter.getF64FloatAttr(typeRangeMax),
                           rewriter.getI32IntegerAttr(
                               outputElementType.getStorageTypeIntegralWidth()),
                           0, true /* signed */,
                           rewriter.getBoolAttr(narrowRange)));

  ElementsAttr inputElems;
  if (!matchPattern(tosaNegateOp.getInput1(), m_Constant(&inputElems)))
    return failure();

  auto newConstOp =
      tosa::ConstOp::create(rewriter, op->getLoc(), dstQConstType, inputElems);
  auto newNegateOp = tosa::NegateOp::create(
      rewriter, op->getLoc(), dstQConstType, newConstOp.getResult());

  rewriter.replaceOp(op, {newNegateOp.getResult()});
  return success();
}

// This transformation modifies the quantized output of a test conv2d input and
// appends a TOSA rescale after it. The rescale op requires the invocation of
// computeMultiplierAndShift. From TOSA legalization infrastructure.
struct ConvertTosaConv2DOp : public RewritePattern {
  explicit ConvertTosaConv2DOp(MLIRContext *context)
      : RewritePattern(tosa::Conv2DOp::getOperationName(), 1, context) {}
  LogicalResult matchAndRewrite(Operation *op,
                                PatternRewriter &rewriter) const override;
};

LogicalResult
ConvertTosaConv2DOp::matchAndRewrite(Operation *op,
                                     PatternRewriter &rewriter) const {

  auto tosaConv2DOp = cast<tosa::Conv2DOp>(op);

  auto inputType =
      dyn_cast<mlir::RankedTensorType>(tosaConv2DOp.getInput().getType());

  // skip if input is not ranked tensor type
  if (!inputType)
    return failure();

  auto weightType =
      dyn_cast<mlir::RankedTensorType>(tosaConv2DOp.getWeight().getType());

  // skip if wt is not ranked tensor type
  if (!weightType)
    return failure();

  // skip if it's not ranked tensor type.
  auto outputType =
      dyn_cast<mlir::RankedTensorType>(tosaConv2DOp.getResult().getType());
  if (!outputType)
    return failure();

  auto inputQType =
      dyn_cast<mlir::quant::UniformQuantizedType>(inputType.getElementType());
  auto weightQType =
      dyn_cast<mlir::quant::UniformQuantizedType>(weightType.getElementType());
  auto outputQType =
      dyn_cast<mlir::quant::UniformQuantizedType>(outputType.getElementType());

  // Works on quantized type only.
  if (!(inputQType && weightQType && outputQType))
    return failure();

  auto newTosaConv2DOpType =
      RankedTensorType::get(outputType.getShape(), rewriter.getIntegerType(32));

  auto newTosaConv2DOp = tosa::Conv2DOp::create(
      rewriter, op->getLoc(), newTosaConv2DOpType, tosaConv2DOp.getInput(),
      tosaConv2DOp.getWeight(), tosaConv2DOp.getBias(),
      tosaConv2DOp.getPadAttr(), tosaConv2DOp.getStrideAttr(),
      tosaConv2DOp.getDilationAttr(), tosaConv2DOp.getAccTypeAttr());

  // Create rescale to quantized type
  double inputScale = inputQType.getScale();
  double weightScale = weightQType.getScale();
  double outputScale = outputQType.getScale();
  int64_t outputZpVal = outputQType.getZeroPoint();

  auto inputZp =
      createZeroPointTensor(rewriter, op->getLoc(), newTosaConv2DOpType, 0);
  auto outputZp = createZeroPointTensor(
      rewriter, op->getLoc(), tosaConv2DOp.getOutput().getType(), outputZpVal);

  if (!inputZp || !outputZp)
    return failure();

  double opTensorScale = (inputScale * weightScale) / outputScale;

  int32_t multiplier;
  int32_t shift;

  // Obtain the quantized scale = multiplier and shift.
  if (!computeMultiplierAndShift(opTensorScale, multiplier, shift, 32))
    return failure();

  bool inputUnsigned =
      newTosaConv2DOp.getResult().getType().isUnsignedInteger();
  bool outputUnsigned = outputType.isUnsignedInteger();

  auto newTosaRescaleOp = tosa::RescaleOp::create(
      rewriter, op->getLoc(), outputType, newTosaConv2DOp.getResult(),
      getConstTensorInt<int32_t>(rewriter, op->getLoc(), {multiplier}),
      getConstTensorInt<int8_t>(rewriter, op->getLoc(),
                                {static_cast<int8_t>(shift)}),
      inputZp.value(), outputZp.value(),
      /* scale32 = */ rewriter.getBoolAttr(true),
      /* double_round = */ rewriter.getStringAttr("DOUBLE_ROUND"),
      /* per_channel = */ rewriter.getBoolAttr(false),
      rewriter.getBoolAttr(inputUnsigned),
      rewriter.getBoolAttr(outputUnsigned));

  rewriter.replaceOp(op, {newTosaRescaleOp.getResult()});
  return success();
}

namespace {

struct TosaTestQuantUtilAPI
    : public PassWrapper<TosaTestQuantUtilAPI, OperationPass<func::FuncOp>> {
  MLIR_DEFINE_EXPLICIT_INTERNAL_INLINE_TYPE_ID(TosaTestQuantUtilAPI)

  StringRef getArgument() const final { return PASS_NAME; }
  StringRef getDescription() const final {
    return "TOSA Test: Exercise the APIs in QuantUtils.cpp.";
  }
  void runOnOperation() override;
};

void TosaTestQuantUtilAPI::runOnOperation() {
  auto *ctx = &getContext();
  RewritePatternSet patterns(ctx);
  auto func = getOperation();

  patterns.add<ConvertTosaNegateOp>(ctx);
  patterns.add<ConvertTosaConv2DOp>(ctx);
  (void)applyPatternsGreedily(func, std::move(patterns));
}

} // namespace

namespace mlir {
void registerTosaTestQuantUtilAPIPass() {
  PassRegistration<TosaTestQuantUtilAPI>();
}
} // namespace mlir

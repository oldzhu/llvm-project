//===- Pattern.h - Pattern for conversion to the LLVM dialect ---*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef MLIR_CONVERSION_LLVMCOMMON_PATTERN_H
#define MLIR_CONVERSION_LLVMCOMMON_PATTERN_H

#include "mlir/Conversion/LLVMCommon/MemRefBuilder.h"
#include "mlir/Conversion/LLVMCommon/TypeConverter.h"
#include "mlir/Dialect/LLVMIR/LLVMAttrs.h"
#include "mlir/Transforms/DialectConversion.h"

namespace mlir {
class CallOpInterface;

namespace LLVM {
namespace detail {
/// Handle generically setting flags as native properties on LLVM operations.
void setNativeProperties(Operation *op, IntegerOverflowFlags overflowFlags);

/// Replaces the given operation "op" with a new operation of type "targetOp"
/// and given operands.
LogicalResult oneToOneRewrite(
    Operation *op, StringRef targetOp, ValueRange operands,
    ArrayRef<NamedAttribute> targetAttrs,
    const LLVMTypeConverter &typeConverter, ConversionPatternRewriter &rewriter,
    IntegerOverflowFlags overflowFlags = IntegerOverflowFlags::none);

/// Replaces the given operation "op" with a call to an LLVM intrinsic with the
/// specified name "intrinsic" and operands.
///
/// The rewrite performs a simple one-to-one matching between the op and LLVM
/// intrinsic. For example:
///
/// ```mlir
/// %res = intr.op %val : vector<16xf32>
/// ```
///
/// can be converted to
///
/// ```mlir
/// %res = llvm.call_intrinsic "intrinsic"(%val)
/// ```
///
/// The provided operands must be LLVM-compatible.
///
/// Upholds a convention that multi-result operations get converted into an
/// operation returning the LLVM IR structure type, in which case individual
/// values are first extracted before replacing the original results.
LogicalResult intrinsicRewrite(Operation *op, StringRef intrinsic,
                               ValueRange operands,
                               const LLVMTypeConverter &typeConverter,
                               RewriterBase &rewriter);

} // namespace detail

/// Decomposes a `src` value into a set of values of type `dstType` through
/// series of bitcasts and vector ops. Src and dst types are expected to be int
/// or float types or vector types of them.
SmallVector<Value> decomposeValue(OpBuilder &builder, Location loc, Value src,
                                  Type dstType);

/// Composes a set of `src` values into a single value of type `dstType` through
/// series of bitcasts and vector ops. Inversely to `decomposeValue`, this
/// function is used to combine multiple values into a single value.
Value composeValue(OpBuilder &builder, Location loc, ValueRange src,
                   Type dstType);

/// Performs the index computation to get to the element at `indices` of the
/// memory pointed to by `memRefDesc`, using the layout map of `type`.
/// The indices are linearized as:
///   `base_offset + index_0 * stride_0 + ... + index_n * stride_n`.
Value getStridedElementPtr(
    OpBuilder &builder, Location loc, const LLVMTypeConverter &converter,
    MemRefType type, Value memRefDesc, ValueRange indices,
    LLVM::GEPNoWrapFlags noWrapFlags = LLVM::GEPNoWrapFlags::none);
} // namespace LLVM

/// Base class for operation conversions targeting the LLVM IR dialect. It
/// provides the conversion patterns with access to the LLVMTypeConverter and
/// the LowerToLLVMOptions. The class captures the LLVMTypeConverter and the
/// LowerToLLVMOptions by reference meaning the references have to remain alive
/// during the entire pattern lifetime.
class ConvertToLLVMPattern : public ConversionPattern {
public:
  /// `SplitMatchAndRewrite` is deprecated. Use `matchAndRewrite` instead of
  /// separate `match` and `rewrite`.
  using SplitMatchAndRewrite =
      detail::ConversionSplitMatchAndRewriteImpl<ConvertToLLVMPattern>;

  ConvertToLLVMPattern(StringRef rootOpName, MLIRContext *context,
                       const LLVMTypeConverter &typeConverter,
                       PatternBenefit benefit = 1);

protected:
  /// See `ConversionPattern::ConversionPattern` for information on the other
  /// available constructors.
  using ConversionPattern::ConversionPattern;

  /// Returns the LLVM dialect.
  LLVM::LLVMDialect &getDialect() const;

  const LLVMTypeConverter *getTypeConverter() const;

  /// Gets the MLIR type wrapping the LLVM integer type whose bit width is
  /// defined by the used type converter.
  Type getIndexType() const;

  /// Gets the MLIR type wrapping the LLVM integer type whose bit width
  /// corresponds to that of a LLVM pointer type.
  Type getIntPtrType(unsigned addressSpace = 0) const;

  /// Gets the MLIR type wrapping the LLVM void type.
  Type getVoidType() const;

  /// Get the MLIR type wrapping the LLVM i8* type.
  [[deprecated("Use getPtrType() instead!")]]
  Type getVoidPtrType() const;

  /// Get the MLIR type wrapping the LLVM ptr type.
  Type getPtrType(unsigned addressSpace = 0) const;

  /// Create a constant Op producing a value of `resultType` from an index-typed
  /// integer attribute.
  static Value createIndexAttrConstant(OpBuilder &builder, Location loc,
                                       Type resultType, int64_t value);

  /// Convenience wrapper for the corresponding helper utility.
  /// This is a strided getElementPtr variant with linearized subscripts.
  Value getStridedElementPtr(
      ConversionPatternRewriter &rewriter, Location loc, MemRefType type,
      Value memRefDesc, ValueRange indices,
      LLVM::GEPNoWrapFlags noWrapFlags = LLVM::GEPNoWrapFlags::none) const;

  /// Returns if the given memref type is convertible to LLVM and has an
  /// identity layout map.
  bool isConvertibleAndHasIdentityMaps(MemRefType type) const;

  /// Returns the type of a pointer to an element of the memref.
  Type getElementPtrType(MemRefType type) const;

  /// Computes sizes, strides and buffer size of `memRefType` with identity
  /// layout. Emits constant ops for the static sizes of `memRefType`, and uses
  /// `dynamicSizes` for the others. Emits instructions to compute strides and
  /// buffer size from these sizes.
  ///
  /// For example, memref<4x?xf32> with `sizeInBytes = true` emits:
  /// `sizes[0]`   = llvm.mlir.constant(4 : index) : i64
  /// `sizes[1]`   = `dynamicSizes[0]`
  /// `strides[1]` = llvm.mlir.constant(1 : index) : i64
  /// `strides[0]` = `sizes[0]`
  /// %size        = llvm.mul `sizes[0]`, `sizes[1]` : i64
  /// %nullptr     = llvm.mlir.zero : !llvm.ptr
  /// %gep         = llvm.getelementptr %nullptr[%size]
  ///                  : (!llvm.ptr, i64) -> !llvm.ptr, f32
  /// `sizeBytes`  = llvm.ptrtoint %gep : !llvm.ptr to i64
  ///
  /// If `sizeInBytes = false`, memref<4x?xf32> emits:
  /// `sizes[0]`   = llvm.mlir.constant(4 : index) : i64
  /// `sizes[1]`   = `dynamicSizes[0]`
  /// `strides[1]` = llvm.mlir.constant(1 : index) : i64
  /// `strides[0]` = `sizes[0]`
  /// %size        = llvm.mul `sizes[0]`, `sizes[1]` : i64
  void getMemRefDescriptorSizes(Location loc, MemRefType memRefType,
                                ValueRange dynamicSizes,
                                ConversionPatternRewriter &rewriter,
                                SmallVectorImpl<Value> &sizes,
                                SmallVectorImpl<Value> &strides, Value &size,
                                bool sizeInBytes = true) const;

  /// Computes the size of type in bytes.
  Value getSizeInBytes(Location loc, Type type,
                       ConversionPatternRewriter &rewriter) const;

  /// Computes total number of elements for the given MemRef and dynamicSizes.
  Value getNumElements(Location loc, MemRefType memRefType,
                       ValueRange dynamicSizes,
                       ConversionPatternRewriter &rewriter) const;

  /// Creates and populates a canonical memref descriptor struct.
  MemRefDescriptor
  createMemRefDescriptor(Location loc, MemRefType memRefType,
                         Value allocatedPtr, Value alignedPtr,
                         ArrayRef<Value> sizes, ArrayRef<Value> strides,
                         ConversionPatternRewriter &rewriter) const;

  /// Copies the memory descriptor for any operands that were unranked
  /// descriptors originally to heap-allocated memory (if toDynamic is true) or
  /// to stack-allocated memory (otherwise). Also frees the previously used
  /// memory (that is assumed to be heap-allocated) if toDynamic is false.
  LogicalResult copyUnrankedDescriptors(OpBuilder &builder, Location loc,
                                        TypeRange origTypes,
                                        SmallVectorImpl<Value> &operands,
                                        bool toDynamic) const;
};

/// Utility class for operation conversions targeting the LLVM dialect that
/// match exactly one source operation.
template <typename SourceOp>
class ConvertOpToLLVMPattern : public ConvertToLLVMPattern {
public:
  using OperationT = SourceOp;
  using OpAdaptor = typename SourceOp::Adaptor;
  using OneToNOpAdaptor =
      typename SourceOp::template GenericAdaptor<ArrayRef<ValueRange>>;

  /// `SplitMatchAndRewrite` is deprecated. Use `matchAndRewrite` instead of
  /// separate `match` and `rewrite`.
  using SplitMatchAndRewrite = detail::ConversionSplitMatchAndRewriteImpl<
      ConvertOpToLLVMPattern<SourceOp>>;

  explicit ConvertOpToLLVMPattern(const LLVMTypeConverter &typeConverter,
                                  PatternBenefit benefit = 1)
      : ConvertToLLVMPattern(SourceOp::getOperationName(),
                             &typeConverter.getContext(), typeConverter,
                             benefit) {}

  /// Wrappers around the RewritePattern methods that pass the derived op type.
  LogicalResult
  matchAndRewrite(Operation *op, ArrayRef<Value> operands,
                  ConversionPatternRewriter &rewriter) const final {
    auto sourceOp = cast<SourceOp>(op);
    return matchAndRewrite(sourceOp, OpAdaptor(operands, sourceOp), rewriter);
  }
  LogicalResult
  matchAndRewrite(Operation *op, ArrayRef<ValueRange> operands,
                  ConversionPatternRewriter &rewriter) const final {
    auto sourceOp = cast<SourceOp>(op);
    return matchAndRewrite(sourceOp, OneToNOpAdaptor(operands, sourceOp),
                           rewriter);
  }

  /// Methods that operate on the SourceOp type. One of these must be
  /// overridden by the derived pattern class.
  virtual LogicalResult
  matchAndRewrite(SourceOp op, OpAdaptor adaptor,
                  ConversionPatternRewriter &rewriter) const {
    llvm_unreachable("matchAndRewrite is not implemented");
  }
  virtual LogicalResult
  matchAndRewrite(SourceOp op, OneToNOpAdaptor adaptor,
                  ConversionPatternRewriter &rewriter) const {
    SmallVector<Value> oneToOneOperands =
        getOneToOneAdaptorOperands(adaptor.getOperands());
    return matchAndRewrite(op, OpAdaptor(oneToOneOperands, adaptor), rewriter);
  }

private:
  using ConvertToLLVMPattern::matchAndRewrite;
};

/// Utility class for operation conversions targeting the LLVM dialect that
/// allows for matching and rewriting against an instance of an OpInterface
/// class.
template <typename SourceOp>
class ConvertOpInterfaceToLLVMPattern : public ConvertToLLVMPattern {
public:
  explicit ConvertOpInterfaceToLLVMPattern(
      const LLVMTypeConverter &typeConverter, PatternBenefit benefit = 1)
      : ConvertToLLVMPattern(typeConverter, Pattern::MatchInterfaceOpTypeTag(),
                             SourceOp::getInterfaceID(), benefit,
                             &typeConverter.getContext()) {}

  /// Wrappers around the RewritePattern methods that pass the derived op type.
  LogicalResult
  matchAndRewrite(Operation *op, ArrayRef<Value> operands,
                  ConversionPatternRewriter &rewriter) const final {
    return matchAndRewrite(cast<SourceOp>(op), operands, rewriter);
  }
  LogicalResult
  matchAndRewrite(Operation *op, ArrayRef<ValueRange> operands,
                  ConversionPatternRewriter &rewriter) const final {
    return matchAndRewrite(cast<SourceOp>(op), operands, rewriter);
  }

  /// Methods that operate on the SourceOp type. One of these must be
  /// overridden by the derived pattern class.
  virtual LogicalResult
  matchAndRewrite(SourceOp op, ArrayRef<Value> operands,
                  ConversionPatternRewriter &rewriter) const {
    llvm_unreachable("matchAndRewrite is not implemented");
  }
  virtual LogicalResult
  matchAndRewrite(SourceOp op, ArrayRef<ValueRange> operands,
                  ConversionPatternRewriter &rewriter) const {
    return matchAndRewrite(op, getOneToOneAdaptorOperands(operands), rewriter);
  }

private:
  using ConvertToLLVMPattern::matchAndRewrite;
};

/// Generic implementation of one-to-one conversion from "SourceOp" to
/// "TargetOp" where the latter belongs to the LLVM dialect or an equivalent.
/// Upholds a convention that multi-result operations get converted into an
/// operation returning the LLVM IR structure type, in which case individual
/// values must be extracted from using LLVM::ExtractValueOp before being used.
template <typename SourceOp, typename TargetOp>
class OneToOneConvertToLLVMPattern : public ConvertOpToLLVMPattern<SourceOp> {
public:
  using ConvertOpToLLVMPattern<SourceOp>::ConvertOpToLLVMPattern;
  using Super = OneToOneConvertToLLVMPattern<SourceOp, TargetOp>;

  /// Converts the type of the result to an LLVM type, pass operands as is,
  /// preserve attributes.
  LogicalResult
  matchAndRewrite(SourceOp op, typename SourceOp::Adaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override {
    return LLVM::detail::oneToOneRewrite(op, TargetOp::getOperationName(),
                                         adaptor.getOperands(), op->getAttrs(),
                                         *this->getTypeConverter(), rewriter);
  }
};

} // namespace mlir

#endif // MLIR_CONVERSION_LLVMCOMMON_PATTERN_H

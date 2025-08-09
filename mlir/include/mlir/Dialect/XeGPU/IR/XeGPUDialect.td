//===- XeGPUDialect.td - XeGPU dialect definition -----------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef MLIR_DIALECT_XEGPU_IR_XEGPUDIALECT_TD
#define MLIR_DIALECT_XEGPU_IR_XEGPUDIALECT_TD

include "mlir/IR/OpBase.td"

def XeGPU_Dialect : Dialect {
    let name = "xegpu";
    let cppNamespace = "::mlir::xegpu";
    let summary = "The XeGPU dialect that models Intel GPU's ISA";
    let description = [{
      The XeGPU dialect models Intel Xe ISA semantics but works at vector and
      TensorDesc data type. It provides 1:1 mappings to match Xe instructions
      like DPAS and 2D block load. The matrix size being processed at this level
      exactly matches the hardware instructions or the intrinsic supported by
      the lower-level GPU compiler.
    }];

    let dependentDialects = ["arith::ArithDialect"];

    let useDefaultTypePrinterParser = true;
    let useDefaultAttributePrinterParser = true;

    let extraClassDeclaration = [{
      /// Checks if the given shape can be evenly distributed based on the layout
      /// and data factors provided by the LayoutAttr.
      static bool isEvenlyDistributable(llvm::ArrayRef<int64_t> shape, xegpu::LayoutAttr attr);

      /// drops/slices the shape in the specified dims, and return the rest. e.g.,
      /// for shape = [32, 64, 8], dims = [0, 2], it will return [64]
      template<typename T, typename U>
      static llvm::SmallVector<T> slice(llvm::ArrayRef<T> shape, llvm::ArrayRef<U> dims) {
        llvm::SmallVector<T> result;
        for (auto [i, v]: llvm::enumerate(shape)) {
          if (!llvm::is_contained(dims, i))
            result.push_back(v);
        }
        return result;
      }
    }];
}

#endif // MLIR_DIALECT_XEGPU_IR_XEGPUDIALECT_TD

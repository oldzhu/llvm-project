add_mlir_dialect_library(MLIRSCFTransforms
  BufferDeallocationOpInterfaceImpl.cpp
  BufferizableOpInterfaceImpl.cpp
  ForallToFor.cpp
  ForallToParallel.cpp
  ForToWhile.cpp
  IfConditionPropagation.cpp
  LoopCanonicalization.cpp
  LoopPipelining.cpp
  LoopRangeFolding.cpp
  LoopSpecialization.cpp
  OneToNTypeConversion.cpp
  ParallelLoopCollapsing.cpp
  ParallelLoopFusion.cpp
  ParallelLoopTiling.cpp
  RotateWhileLoop.cpp
  StructuralTypeConversions.cpp
  TileUsingInterface.cpp
  WrapInZeroTripCheck.cpp
  UpliftWhileToFor.cpp

  ADDITIONAL_HEADER_DIRS
  ${MLIR_MAIN_INCLUDE_DIR}/mlir/Dialect/SCF

  DEPENDS
  MLIRSCFPassIncGen

  LINK_LIBS PUBLIC
  MLIRAffineDialect
  MLIRAffineAnalysis
  MLIRArithDialect
  MLIRBufferizationDialect
  MLIRBufferizationTransforms
  MLIRDestinationStyleOpInterface
  MLIRDialectUtils
  MLIRIR
  MLIRMemRefDialect
  MLIRPass
  MLIRSCFDialect
  MLIRSCFUtils
  MLIRSideEffectInterfaces
  MLIRSupport
  MLIRTensorTransforms
  MLIRTransforms
  MLIRTransformUtils
)

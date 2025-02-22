// RUN: mlir-opt %s --transform-interpreter | FileCheck %s

func.func @vector_bitcast_0d(%arg0: vector<i32>) -> vector<f32> {
  %0 = vector.bitcast %arg0 : vector<i32> to vector<f32>
  return %0 : vector<f32>
}
// CHECK-LABEL: func.func @vector_bitcast_0d
// CHECK-SAME:    %[[IN:[a-zA-Z0-9]+]]
// CHECK:         %[[RES:.+]] = vector.bitcast %[[IN]] : vector<i32> to vector<f32>
// CHECK:         return %[[RES]]

func.func @vector_bitcast_1d(%arg0: vector<10xi64>) -> vector<20xi32> {
  %0 = vector.bitcast %arg0 : vector<10xi64> to vector<20xi32>
  return %0 : vector<20xi32>
}
// CHECK-LABEL: func.func @vector_bitcast_1d
// CHECK-SAME:    %[[IN:[a-zA-Z0-9]+]]
// CHECK:         %[[RES:.+]] = vector.bitcast %[[IN]] : vector<10xi64> to vector<20xi32>
// CHECK:         return %[[RES]]

func.func @vector_bitcast_2d(%arg0: vector<2x4xi32>) -> vector<2x2xi64> {
  %0 = vector.bitcast %arg0 : vector<2x4xi32> to vector<2x2xi64>
  return %0 : vector<2x2xi64>
}
// CHECK-LABEL: func.func @vector_bitcast_2d
// CHECK-SAME:    %[[IN:[a-zA-Z0-9]+]]
// CHECK:         %[[INIT:.+]] = ub.poison : vector<2x2xi64>
// CHECK:         %[[V1:.+]] = vector.extract %[[IN]][0] : vector<4xi32> from vector<2x4xi32>
// CHECK:         %[[B1:.+]] = vector.bitcast %[[V1]] : vector<4xi32> to vector<2xi64>
// CHECK:         %[[R1:.+]] = vector.insert %[[B1]], %[[INIT]] [0]
// CHECK:         %[[V2:.+]] = vector.extract %[[IN]][1] : vector<4xi32> from vector<2x4xi32>
// CHECK:         %[[B2:.+]] = vector.bitcast %[[V2]] : vector<4xi32> to vector<2xi64>
// CHECK:         %[[R2:.+]] = vector.insert %[[B2]], %[[R1]] [1]
// CHECK:         return %[[R2]]

func.func @vector_bitcast_4d_with_scalable_dim(%arg0: vector<1x2x[3]x4xi64>) -> vector<1x2x[3]x8xi32> {
  %0 = vector.bitcast %arg0 : vector<1x2x[3]x4xi64> to vector<1x2x[3]x8xi32>
  return %0 : vector<1x2x[3]x8xi32>
}
// CHECK-LABEL: func.func @vector_bitcast_4d_with_scalable_dim
// CHECK-SAME:    %[[IN:[a-zA-Z0-9]+]]
// CHECK:         %[[INIT:.+]] = ub.poison : vector<1x2x[3]x8xi32>
// CHECK:         %[[V1:.+]] = vector.extract %[[IN]][0, 0] : vector<[3]x4xi64> from vector<1x2x[3]x4xi64>
// CHECK:         %[[B1:.+]] = vector.bitcast %[[V1]] : vector<[3]x4xi64> to vector<[3]x8xi32>
// CHECK:         %[[R1:.+]] = vector.insert %[[B1]], %[[INIT]] [0, 0] : vector<[3]x8xi32> into vector<1x2x[3]x8xi32>
// CHECK:         %[[V2:.+]] = vector.extract %[[IN]][0, 1] : vector<[3]x4xi64> from vector<1x2x[3]x4xi64>
// CHECK:         %[[B2:.+]] = vector.bitcast %[[V2]] : vector<[3]x4xi64> to vector<[3]x8xi32>
// CHECK:         %[[R2:.+]] = vector.insert %[[B2]], %[[R1]] [0, 1] : vector<[3]x8xi32> into vector<1x2x[3]x8xi32>
// CHECK:         return %[[R2]] : vector<1x2x[3]x8xi32>

func.func @vector_bitcast_2d_trailing_scalable_dim(%arg0: vector<2x[2]xi64>) -> vector<2x[4]xi32> {
  %0 = vector.bitcast %arg0 : vector<2x[2]xi64> to vector<2x[4]xi32>
  return %0 : vector<2x[4]xi32>
}
// CHECK-LABEL: func.func @vector_bitcast_2d_trailing_scalable_dim
// CHECK-SAME:    %[[IN:[a-zA-Z0-9]+]]
// CHECK:         %[[INIT:.+]] = ub.poison : vector<2x[4]xi32>
// CHECK:         %[[V1:.+]] = vector.extract %[[IN]][0] : vector<[2]xi64> from vector<2x[2]xi64>
// CHECK:         %[[B1:.+]] = vector.bitcast %[[V1]] : vector<[2]xi64> to vector<[4]xi32>
// CHECK:         %[[R1:.+]] = vector.insert %[[B1]], %[[INIT]] [0] : vector<[4]xi32> into vector<2x[4]xi32>
// CHECK:         %[[V2:.+]] = vector.extract %[[IN]][1] : vector<[2]xi64> from vector<2x[2]xi64>
// CHECK:         %[[B2:.+]] = vector.bitcast %[[V2]] : vector<[2]xi64> to vector<[4]xi32>
// CHECK:         %[[R2:.+]] = vector.insert %[[B2]], %[[R1]] [1] : vector<[4]xi32> into vector<2x[4]xi32>
// CHECK:         return %[[R2]] : vector<2x[4]xi32>

func.func @negative_vector_bitcast_2d_leading_scalable_dim(%arg0: vector<[2]x2xi64>) -> vector<[2]x4xi32>
{
  %0 = vector.bitcast %arg0 : vector<[2]x2xi64> to vector<[2]x4xi32>
  return %0 : vector<[2]x4xi32>
}
// CHECK-LABEL: func.func @negative_vector_bitcast_2d_leading_scalable_dim
// CHECK-NOT: vector.extract
// CHECK-NOT: vector.insert

module attributes {transform.with_named_sequence} {
  transform.named_sequence @__transform_main(%module_op: !transform.any_op {transform.readonly}) {
    %f = transform.structured.match ops{["func.func"]} in %module_op
      : (!transform.any_op) -> !transform.any_op

    transform.apply_patterns to %f {
      transform.apply_patterns.vector.lower_bitcast
    } : !transform.any_op
    transform.yield
  }
}

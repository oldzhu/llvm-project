// REQUIRES: arm-registered-target
// RUN: %clang_cc1 -triple aarch64-none-elf \
// RUN:   -O2 \
// RUN:   -emit-llvm -fexperimental-max-bitint-width=1024 -o - %s | FileCheck %s

extern "C" {

// CHECK: @sizeof_OverSizedBitfield ={{.*}} global i32 8
// CHECK: @alignof_OverSizedBitfield ={{.*}} global i32 8
// CHECK: @sizeof_VeryOverSizedBitfield ={{.*}} global i32 16
// CHECK: @alignof_VeryOverSizedBitfield ={{.*}} global i32 16
// CHECK: @sizeof_RidiculouslyOverSizedBitfield ={{.*}} global i32 32
// CHECK: @alignof_RidiculouslyOverSizedBitfield ={{.*}} global i32 16

// Base case, nothing interesting.
struct S {
  long x, y;
};

void f0(long, S);
void f0m(long, long, long, long, long, S);
void g0() {
  S s = {6, 7};
  f0(1, s);
  f0m(1, 2, 3, 4, 5, s);
}
// CHECK: define{{.*}} void @g0
// CHECK: call void @f0(i64 noundef 1, [2 x i64] [i64 6, i64 7]
// CHECK: call void @f0m{{.*}}[2 x i64] [i64 6, i64 7]
// CHECK: declare void @f0(i64 noundef, [2 x i64])
// CHECK: declare void @f0m(i64 noundef, i64 noundef, i64 noundef, i64 noundef, i64 noundef, [2 x i64])

// Aligned struct, passed according to its natural alignment.
struct __attribute__((aligned(16))) S16 {
  long x, y;
} s16;

void f1(long, S16);
void f1m(long, long, long, long, long, S16);
void g1() {
  S16 s = {6, 7};
  f1(1, s);
  f1m(1, 2, 3, 4, 5, s);
}
// CHECK: define{{.*}} void @g1
// CHECK: call void @f1{{.*}}[2 x i64] [i64 6, i64 7]
// CHECK: call void @f1m{{.*}}[2 x i64] [i64 6, i64 7]
// CHECK: declare void @f1(i64 noundef, [2 x i64])
// CHECK: declare void @f1m(i64 noundef, i64 noundef, i64 noundef, i64 noundef, i64 noundef, [2 x i64])

// Increased natural alignment.
struct SF16 {
  long x __attribute__((aligned(16)));
  long y;
};

void f3(long, SF16);
void f3m(long, long, long, long, long, SF16);
void g3() {
  SF16 s = {6, 7};
  f3(1, s);
  f3m(1, 2, 3, 4, 5, s);
}
// CHECK: define{{.*}} void @g3
// CHECK: call void @f3(i64 noundef 1, i128 129127208515966861318)
// CHECK: call void @f3m(i64 noundef 1, i64 noundef 2, i64 noundef 3, i64 noundef 4, i64 noundef 5, i128 129127208515966861318)
// CHECK: declare void @f3(i64 noundef, i128)
// CHECK: declare void @f3m(i64 noundef, i64 noundef, i64 noundef, i64 noundef, i64 noundef, i128)


// Packed structure.
struct  __attribute__((packed)) P {
  int x;
  long u;
};

void f4(int, P);
void f4m(int, int, int, int, int, P);
void g4() {
  P s = {6, 7};
  f4(1, s);
  f4m(1, 2, 3, 4, 5, s);
}
// CHECK: define{{.*}} void @g4()
// CHECK: call void @f4(i32 noundef 1, [2 x i64] [i64 30064771078, i64 0])
// CHECK: void @f4m(i32 noundef 1, i32 noundef 2, i32 noundef 3, i32 noundef 4, i32 noundef 5, [2 x i64] [i64 30064771078, i64 0])
// CHECK: declare void @f4(i32 noundef, [2 x i64])
// CHECK: declare void @f4m(i32 noundef, i32 noundef, i32 noundef, i32 noundef, i32 noundef, [2 x i64])


// Packed structure, overaligned, same as above.
struct  __attribute__((packed, aligned(16))) P16 {
  int x;
  long y;
};

void f5(int, P16);
void f5m(int, int, int, int, int, P16);
  void g5() {
    P16 s = {6, 7};
    f5(1, s);
    f5m(1, 2, 3, 4, 5, s);
}
// CHECK: define{{.*}} void @g5()
// CHECK: call void @f5(i32 noundef 1, [2 x i64] [i64 30064771078, i64 0])
// CHECK: void @f5m(i32 noundef 1, i32 noundef 2, i32 noundef 3, i32 noundef 4, i32 noundef 5, [2 x i64] [i64 30064771078, i64 0])
// CHECK: declare void @f5(i32 noundef, [2 x i64])
// CHECK: declare void @f5m(i32 noundef, i32 noundef, i32 noundef, i32 noundef, i32 noundef, [2 x i64])

//BitInt alignment
struct BITINT129 {
    char ch;
    unsigned _BitInt(129) v;
};

int test_bitint129(){
  return __builtin_offsetof(struct BITINT129, v);
}
// CHECK:  ret i32 16 

struct BITINT127 {
    char ch;
    _BitInt(127) v;
};

int test_bitint127(){
  return __builtin_offsetof(struct BITINT127, v);
}
// CHECK:  ret i32 16 

struct BITINT63 {
    char ch;
    _BitInt(63) v;
};

int test_bitint63(){
  return __builtin_offsetof(struct BITINT63, v);
}
// CHECK:  ret i32 8 

struct BITINT32 {
    char ch;
    unsigned _BitInt(32) v;
};

int test_bitint32(){
  return __builtin_offsetof(struct BITINT32, v);
}
// CHECK:  ret i32 4

struct BITINT9 {
    char ch;
    unsigned _BitInt(9) v;
};

int test_bitint9(){
  return __builtin_offsetof(struct BITINT9, v);
}
// CHECK:  ret i32 2

struct BITINT8 {
    char ch;
    unsigned _BitInt(8) v;
};

int test_bitint8(){
  return __builtin_offsetof(struct BITINT8, v);
}
// CHECK:  ret i32 1

// Over-sized bitfield, which results in a 64-bit container type, so 64-bit
// alignment.
struct OverSizedBitfield {
  int x : 64;
};

unsigned sizeof_OverSizedBitfield = sizeof(OverSizedBitfield);
unsigned alignof_OverSizedBitfield = alignof(OverSizedBitfield);

// CHECK: define{{.*}} void @g7
// CHECK: call void @f7(i32 noundef 1, i64 42)
// CHECK: declare void @f7(i32 noundef, i64)
void f7(int a, OverSizedBitfield b);
void g7() {
  OverSizedBitfield s = {42};
  f7(1, s);
}

// AAPCS64 does have a 128-bit integer fundamental data type, so this gets a
// 128-bit container with 128-bit alignment. This is just within the limit of
// what can be passed directly.
struct VeryOverSizedBitfield {
  int x : 128;
};

unsigned sizeof_VeryOverSizedBitfield = sizeof(VeryOverSizedBitfield);
unsigned alignof_VeryOverSizedBitfield = alignof(VeryOverSizedBitfield);

// CHECK: define{{.*}} void @g8
// CHECK: call void @f8(i32 noundef 1, i128 42)
// CHECK: declare void @f8(i32 noundef, i128)
void f8(int a, VeryOverSizedBitfield b);
void g8() {
  VeryOverSizedBitfield s = {42};
  f8(1, s);
}

// There are no bigger fundamental data types, so this gets a 128-bit container
// and 128 bits of padding, giving the struct a size of 32 bytes, and an
// alignment of 16 bytes. This is over the PCS size limit of 16 bytes, so it
// will be passed indirectly.
struct RidiculouslyOverSizedBitfield {
  int x : 256;
};

unsigned sizeof_RidiculouslyOverSizedBitfield = sizeof(RidiculouslyOverSizedBitfield);
unsigned alignof_RidiculouslyOverSizedBitfield = alignof(RidiculouslyOverSizedBitfield);

// CHECK: define{{.*}} void @g9
// CHECK: call void @f9(i32 noundef 1, ptr dead_on_return noundef nonnull %agg.tmp)
// CHECK: declare void @f9(i32 noundef, ptr dead_on_return noundef)
void f9(int a, RidiculouslyOverSizedBitfield b);
void g9() {
  RidiculouslyOverSizedBitfield s = {42};
  f9(1, s);
}

}


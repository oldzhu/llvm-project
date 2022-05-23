//===-- NativeRegisterContextLinux_riscv.cpp --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#if defined(__riscv)

#include "NativeRegisterContextLinux_riscv.h"

#include "Plugins/Process/Linux/NativeProcessLinux.h"
#include "Plugins/Process/Utility/RegisterInfoPOSIX_riscv.h"


using namespace lldb;
using namespace lldb_private;
using namespace lldb_private::process_linux;

#if defined(__riscv)

std::unique_ptr<NativeRegisterContextLinux>
NativeRegisterContextLinux::CreateHostNativeRegisterContextLinux(
    const ArchSpec &target_arch, NativeThreadLinux &native_thread) {
  return std::make_unique<NativeRegisterContextLinux_riscv>(target_arch,
                                                           native_thread);
}

#endif // defined(__riscv)

NativeRegisterContextLinux_riscv::NativeRegisterContextLinux_riscv(
    const ArchSpec &target_arch, NativeThreadProtocol &native_thread)
    : NativeRegisterContextRegisterInfo(native_thread,
                                        new RegisterInfoPOSIX_riscv(target_arch)),
      NativeRegisterContextLinux(native_thread) {
}


uint32_t NativeRegisterContextLinux_riscv::GetRegisterSetCount() const {
  return 0;
}

uint32_t NativeRegisterContextLinux_riscv::GetUserRegisterCount() const {
  return 0;
}

const RegisterSet *
NativeRegisterContextLinux_riscv::GetRegisterSet(uint32_t set_index) const {
  return nullptr;
}

Status
NativeRegisterContextLinux_riscv::ReadRegister(const RegisterInfo *reg_info,
                                             RegisterValue &reg_value) {
  return Status("not implemented");
}

Status
NativeRegisterContextLinux_riscv::WriteRegister(const RegisterInfo *reg_info,
                                              const RegisterValue &reg_value) {
 return Status("not implemented");
}

Status NativeRegisterContextLinux_riscv::ReadAllRegisterValues(
    lldb::WritableDataBufferSP &data_sp) {
 return Status("not implemented");
}

Status NativeRegisterContextLinux_riscv::WriteAllRegisterValues(
    const lldb::DataBufferSP &data_sp) {
 return Status("not implemented");
}


uint32_t NativeRegisterContextLinux_riscv::NumSupportedHardwareBreakpoints() {
  return 0; 
}

uint32_t
NativeRegisterContextLinux_riscv::SetHardwareBreakpoint(lldb::addr_t addr,
                                                      size_t size) {
  return LLDB_INVALID_INDEX32;
}

bool NativeRegisterContextLinux_riscv::ClearHardwareBreakpoint(uint32_t hw_idx) {
  return false;
}

Status NativeRegisterContextLinux_riscv::GetHardwareBreakHitIndex(
    uint32_t &bp_index, lldb::addr_t trap_addr) {
  return Status();
}

Status NativeRegisterContextLinux_riscv::ClearAllHardwareBreakpoints() {
  return Status();
}

uint32_t NativeRegisterContextLinux_riscv::NumSupportedHardwareWatchpoints() {
  return 0;
}

uint32_t NativeRegisterContextLinux_riscv::SetHardwareWatchpoint(
    lldb::addr_t addr, size_t size, uint32_t watch_flags) {
  return LLDB_INVALID_INDEX32;
}

bool NativeRegisterContextLinux_riscv::ClearHardwareWatchpoint(
    uint32_t wp_index) {
  return false;
}

Status NativeRegisterContextLinux_riscv::ClearAllHardwareWatchpoints() {
  return Status();
}

Status
NativeRegisterContextLinux_riscv::GetWatchpointHitIndex(uint32_t &wp_index,
                                                      lldb::addr_t trap_addr) {
  return Status();
}

lldb::addr_t
NativeRegisterContextLinux_riscv::GetWatchpointAddress(uint32_t wp_index) {
  return LLDB_INVALID_ADDRESS;
}

lldb::addr_t
NativeRegisterContextLinux_riscv::GetWatchpointHitAddress(uint32_t wp_index) {
  return LLDB_INVALID_ADDRESS;
}

#endif // defined(__riscv)

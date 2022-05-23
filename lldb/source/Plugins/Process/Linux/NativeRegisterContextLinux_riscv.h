//===-- NativeRegisterContextLinux_arm.h ---------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#if defined(__riscv)

#ifndef lldb_NativeRegisterContextLinux_riscv_h
#define lldb_NativeRegisterContextLinux_riscv_h

#include "Plugins/Process/Linux/NativeRegisterContextLinux.h"
#include "Plugins/Process/Utility/RegisterInfoPOSIX_riscv.h"

namespace lldb_private {
namespace process_linux {

class NativeProcessLinux;

class NativeRegisterContextLinux_riscv: public NativeRegisterContextLinux {
public:
  NativeRegisterContextLinux_riscv(const ArchSpec &target_arch,
                                 NativeThreadProtocol &native_thread);
  
  uint32_t GetRegisterSetCount() const override;

  const RegisterSet *GetRegisterSet(uint32_t set_index) const override;

  uint32_t GetUserRegisterCount() const override;

  Status ReadRegister(const RegisterInfo *reg_info,
                      RegisterValue &reg_value) override;

  Status WriteRegister(const RegisterInfo *reg_info,
                       const RegisterValue &reg_value) override;

  Status ReadAllRegisterValues(lldb::WritableDataBufferSP &data_sp) override;

  Status WriteAllRegisterValues(const lldb::DataBufferSP &data_sp) override;

  // Hardware breakpoints/watchpoint management functions

  uint32_t NumSupportedHardwareBreakpoints() override;

  uint32_t SetHardwareBreakpoint(lldb::addr_t addr, size_t size) override;

  bool ClearHardwareBreakpoint(uint32_t hw_idx) override;

  Status ClearAllHardwareBreakpoints() override;

  Status GetHardwareBreakHitIndex(uint32_t &bp_index,
                                  lldb::addr_t trap_addr) override;

  uint32_t NumSupportedHardwareWatchpoints() override;

  uint32_t SetHardwareWatchpoint(lldb::addr_t addr, size_t size,
                                 uint32_t watch_flags) override;

  bool ClearHardwareWatchpoint(uint32_t hw_index) override;

  Status ClearAllHardwareWatchpoints() override;

  Status GetWatchpointHitIndex(uint32_t &wp_index,
                               lldb::addr_t trap_addr) override;

  lldb::addr_t GetWatchpointHitAddress(uint32_t wp_index) override;

  lldb::addr_t GetWatchpointAddress(uint32_t wp_index) override;

protected:
  void *GetGPRBuffer() override { return nullptr; }

  void *GetFPRBuffer() override { return nullptr; }

  size_t GetFPRSize() override { return 0; }

};

} // namespace process_linux
} // namespace lldb_private

#endif // #ifndef lldb_NativeRegisterContextLinux_riscv_h

#endif // defined(__riscv__) 

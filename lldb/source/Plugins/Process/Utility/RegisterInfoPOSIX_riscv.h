//===-- RegisterInfoPOSIX_arm.h ---------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLDB_SOURCE_PLUGINS_PROCESS_UTILITY_REGISTERINFOPOSIX_RISCV_H
#define LLDB_SOURCE_PLUGINS_PROCESS_UTILITY_REGISTERINFOPOSIX_RISCV_H

#include "RegisterInfoAndSetInterface.h"
#include "lldb/Target/RegisterContext.h"
#include "lldb/lldb-private.h"

class RegisterInfoPOSIX_riscv : public lldb_private::RegisterInfoAndSetInterface {
public:

  RegisterInfoPOSIX_riscv(const lldb_private::ArchSpec &target_arch);

  size_t GetGPRSize() const override;

  size_t GetFPRSize() const override;

  const lldb_private::RegisterInfo *GetRegisterInfo() const override;

  uint32_t GetRegisterCount() const override;

  const lldb_private::RegisterSet *
  GetRegisterSet(size_t reg_set) const override;

  size_t GetRegisterSetCount() const override;

  size_t GetRegisterSetFromRegisterIndex(uint32_t reg_index) const override;

private:
  const lldb_private::RegisterInfo *m_register_info_p;
  uint32_t m_register_info_count;
};

#endif // LLDB_SOURCE_PLUGINS_PROCESS_UTILITY_REGISTERINFOPOSIX_ARM_H

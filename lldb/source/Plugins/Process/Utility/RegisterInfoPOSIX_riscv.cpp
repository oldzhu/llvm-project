//===-- RegisterInfoPOSIX_riscv.cpp -----------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===---------------------------------------------------------------------===//

#include <cassert>
#include <cstddef>
#include <vector>

#include "lldb/lldb-defines.h"
#include "llvm/Support/Compiler.h"

#include "RegisterInfoPOSIX_riscv.h"

using namespace lldb;
using namespace lldb_private;


static const lldb_private::RegisterInfo *
GetRegisterInfoPtr(const lldb_private::ArchSpec &target_arch) {
    return nullptr;
}

static uint32_t
GetRegisterInfoCount(const lldb_private::ArchSpec &target_arch) {
    return 0;
}

RegisterInfoPOSIX_riscv::RegisterInfoPOSIX_riscv(
    const lldb_private::ArchSpec &target_arch)
    : lldb_private::RegisterInfoAndSetInterface(target_arch),
      m_register_info_p(GetRegisterInfoPtr(target_arch)),
      m_register_info_count(GetRegisterInfoCount(target_arch)) {}

size_t RegisterInfoPOSIX_riscv::GetGPRSize() const {
  return 0; 
}

size_t RegisterInfoPOSIX_riscv::GetFPRSize() const {
  return 0;
}

const lldb_private::RegisterInfo *
RegisterInfoPOSIX_riscv::GetRegisterInfo() const {
  return m_register_info_p;
}

size_t RegisterInfoPOSIX_riscv::GetRegisterSetCount() const {
  return 0;
}

size_t RegisterInfoPOSIX_riscv::GetRegisterSetFromRegisterIndex(
    uint32_t reg_index) const {
  return LLDB_INVALID_REGNUM;
}

const lldb_private::RegisterSet *
RegisterInfoPOSIX_riscv::GetRegisterSet(size_t set_index) const {
  return nullptr;
}

uint32_t RegisterInfoPOSIX_riscv::GetRegisterCount() const {
  return m_register_info_count;
}

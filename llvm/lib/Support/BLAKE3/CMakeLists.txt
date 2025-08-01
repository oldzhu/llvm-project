set(LLVM_BLAKE3_FILES
  blake3.c
  blake3_dispatch.c
  blake3_portable.c
  blake3_neon.c
)

if (LLVM_DISABLE_ASSEMBLY_FILES)
  set(CAN_USE_ASSEMBLER FALSE)
else()
  set(CAN_USE_ASSEMBLER TRUE)
endif()

#to workaround the assembler error to build BLAKE3
set(CAN_USE_ASSEMBLER FALSE)
macro(disable_blake3_x86_simd)
  add_compile_definitions(BLAKE3_NO_AVX512 BLAKE3_NO_AVX2 BLAKE3_NO_SSE41 BLAKE3_NO_SSE2)
endmacro()

# The BLAKE3 team recommends using the assembly versions, from the README:
#
# "For each of the x86 SIMD instruction sets, four versions are available:
# three flavors of assembly (Unix, Windows MSVC, and Windows GNU) and one
# version using C intrinsics. The assembly versions are generally
# preferred. They perform better, they perform more consistently across
# different compilers, and they build more quickly."

if (CAN_USE_ASSEMBLER)
  if (MSVC)
    check_symbol_exists(_M_X64 "" IS_X64)
    check_symbol_exists(_M_ARM64EC "" IS_ARM64EC)
    if (IS_X64 AND NOT IS_ARM64EC)
      enable_language(ASM_MASM)
      set(LLVM_BLAKE3_ASM_FILES
        blake3_sse2_x86-64_windows_msvc.asm
        blake3_sse41_x86-64_windows_msvc.asm
        blake3_avx2_x86-64_windows_msvc.asm
        blake3_avx512_x86-64_windows_msvc.asm
      )
      list(APPEND LLVM_BLAKE3_FILES ${LLVM_BLAKE3_ASM_FILES})
      # Supress the copyright message.
      set_source_files_properties(${LLVM_BLAKE3_ASM_FILES}
        PROPERTIES COMPILE_OPTIONS "/nologo")
    else()
      disable_blake3_x86_simd()
    endif()
  elseif(WIN32 OR CYGWIN)
    check_symbol_exists(__x86_64__ "" IS_X64)
    if (IS_X64)
      list(APPEND LLVM_BLAKE3_FILES
        blake3_sse2_x86-64_windows_gnu.S
        blake3_sse41_x86-64_windows_gnu.S
        blake3_avx2_x86-64_windows_gnu.S
        blake3_avx512_x86-64_windows_gnu.S
      )
      # Clang before 7 needs -mavx512vl to assemble some instructions.
      set_source_files_properties(blake3_avx512_x86-64_windows_gnu.S
        PROPERTIES COMPILE_OPTIONS "-mavx512vl")
    else()
      disable_blake3_x86_simd()
    endif()
  else()
    check_symbol_exists(__x86_64__ "" IS_X64)
    if (IS_X64 OR CMAKE_OSX_ARCHITECTURES MATCHES "x86_64")
      # In a macOS Universal build (setting CMAKE_OSX_ARCHITECTURES to multiple
      # values), compilation of the source files will target multiple architectures
      # (each source file is internally compiled once for each architecture).
      # To accomodate this configuration we include these assembly files without a
      # CMake check but their source is guarded with architecture "#ifdef" checks.
      list(APPEND LLVM_BLAKE3_FILES
        blake3_sse2_x86-64_unix.S
        blake3_sse41_x86-64_unix.S
        blake3_avx2_x86-64_unix.S
        blake3_avx512_x86-64_unix.S
      )
      # Clang before 7 needs -mavx512vl to assemble some instructions.
      set_source_files_properties(blake3_avx512_x86-64_unix.S
        PROPERTIES COMPILE_OPTIONS "-mavx512vl")
    else()
      disable_blake3_x86_simd()
    endif()
  endif()
else()
  # CAN_USE_ASSEMBLER == FALSE
  disable_blake3_x86_simd()
endif()

add_library(LLVMSupportBlake3 OBJECT EXCLUDE_FROM_ALL ${LLVM_BLAKE3_FILES})
set_target_properties(LLVMSupportBlake3 PROPERTIES FOLDER "LLVM/Libraries")
llvm_update_compile_flags(LLVMSupportBlake3)
if(LLVM_BUILD_LLVM_DYLIB OR BUILD_SHARED_LIBS)
  # Since LLVMSupportBlake3 is not defined using llvm_add_library(), we must
  # define LLVM_EXPORTS here so its public interface is annotated with
  # __declspec(dllexport) when building as a DLL on Windows.
  target_compile_definitions(LLVMSupportBlake3 PRIVATE LLVM_EXPORTS)
endif()

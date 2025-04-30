
option(LLVM_ALLOW_PROBLEMATIC_CONFIGURATIONS OFF "Set this option to ON to allow problematic toolchain configurations. Use on your own risk.")

macro(log_problematic MESSAGE)
  if(LLVM_ALLOW_PROBLEMATIC_CONFIGURATIONS)
    message(WARNING "${MESSAGE}")
  else()
    message(FATAL_ERROR "${MESSAGE}\nYou can force usage of this configuration by passing -DLLVM_ALLOW_PROBLEMATIC_CONFIGURATIONS=ON")
  endif()
endmacro()

# MSVC and /arch:AVX is untested and have created problems before. See:
# https://github.com/llvm/llvm-project/issues/54645

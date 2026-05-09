################################################################################
# CMake toolchain file — rx-elf-gcc
################################################################################
set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR RX)

set(CMAKE_C_COMPILER   rx-elf-gcc)
set(CMAKE_ASM_COMPILER  rx-elf-gcc)
set(CMAKE_LINKER        rx-elf-gcc)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

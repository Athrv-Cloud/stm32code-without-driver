cmake_minimum_required(VERSION 3.30)

# Project setup
project(stm32_project C ASM CXX)  # Added CXX for C++ tests
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Define an option to enable or disable the 'tests' directory
option(ENABLE_TESTS "Enable building tests" OFF)

# If tests are enabled, we'll build for host system
# Otherwise, proceed with STM32 cross-compilation
if(NOT ENABLE_TESTS)
    # Avoid linking standard library system calls
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

    # Set cross-compilation environment
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_PROCESSOR ARM)

    # Define the toolchain with explicit paths
    set(TOOLCHAIN arm-none-eabi)
    set(TOOLCHAIN_PATH "/usr/bin")

    set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/${TOOLCHAIN}-gcc)
    set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PATH}/${TOOLCHAIN}-gcc)
    set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/${TOOLCHAIN}-g++)
    set(CMAKE_AR ${TOOLCHAIN_PATH}/${TOOLCHAIN}-ar)
    set(CMAKE_OBJCOPY ${TOOLCHAIN_PATH}/${TOOLCHAIN}-objcopy)
    set(CMAKE_OBJDUMP ${TOOLCHAIN_PATH}/${TOOLCHAIN}-objdump)
    set(CMAKE_SIZE ${TOOLCHAIN_PATH}/${TOOLCHAIN}-size)

    # MCU-specific flags
    set(CPU_PARAMETERS
        -mcpu=cortex-m3
        -mthumb
       # -mfpu=none
        -mfloat-abi=soft
    )

    # Compiler options
    add_compile_options(
        -DSTM32F103xB
        ${CPU_PARAMETERS}
        -Wall
        -Wextra
        -Wpedantic
        -Wno-unused-parameter
        -O2
        -g3
        -ffunction-sections
        -fdata-sections
        -fno-common
        -fno-builtin
        -Wno-unused-but-set-variable
        -Wno-sign-compare
    )

    # Linker options
    add_link_options(
        ${CPU_PARAMETERS}
        -Wl,--gc-sections
        -specs=nano.specs
        -specs=nosys.specs
        -T${CMAKE_SOURCE_DIR}/STM32F103C8TX_FLASH.ld
        -Wl,--start-group
        -lc
        -lm
        -Wl,--end-group
    )

# Include directories for firmware
include_directories(
    ${CMAKE_SOURCE_DIR}/Inc
)

    # Include directories for firmware
   #include_directories(
        # Inc/cmsis_armcc.h
        # Inc/cmsis_armclang.h
        # Inc/cmsis_compiler.h
        # Inc/cmsis_gcc.h
        # Inc/cmsis_iccarm.h
         #Inc/cmsis_version.h
        # Inc/core_cm3.h
        # Inc/default.h
        # Inc/GB_adc.h
        # Inc/gb_gpio.h
        # Inc/gb_rcc.h
         #Inc/gb_t_delay.h
        # Inc/GB_uart.h
         #Inc/mpu_armv7.h
       #  Inc/stm32f1xx.h
      #  Inc/stm32f103xb.h
     #   Inc/system_stm32f1xx.h
        
    #)

    # Add main executable
    add_executable(${PROJECT_NAME}_firmware
        Src/main.c
        Src/syscalls.c
        Src/sysmem.c
        Src/GB_adc.c
        Src/gb_gpio.c
        Src/gb_rcc.c
        Src/gb_t_delay.c
        Src/GB_uart.c
        Startup/startup_stm32f103c8tx.s
    )

    # Generate HEX and BIN files after build
    add_custom_command(TARGET ${PROJECT_NAME}_firmware POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}_firmware ${PROJECT_NAME}_firmware.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}_firmware ${PROJECT_NAME}_firmware.bin
    )

    # Print size after build
    add_custom_command(TARGET ${PROJECT_NAME}_firmware POST_BUILD
        COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}_firmware
    )

else()
    # Add test directory
    add_subdirectory(tests)
endif()


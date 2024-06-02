# STM32 Minimal CMake project for C/C++ projects
cmake_minimum_required(VERSION 3.12)
###############################################################################

include(./gcc-arm-none-eabi.cmake)

# Set project name and some defaults
if ("${PROJECT_NAME}" STREQUAL "")
    set(PROJECT_NAME firmware)
endif()
project(${PROJECT_NAME})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set debug mode as default
if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()
message("Build type: ${CMAKE_BUILD_TYPE}")

# Set microcontroller information
set(MCU_FAMILY STM32F1xx)
set(MCU_MODEL STM32F103x6)
set(CPU_PARAMETERS
    -mcpu=cortex-m3
    -mthumb
    -mfloat-abi=soft)

set(STARTUP_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/Core/Startup/startup_stm32f103c6tx.s)
set(MCU_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/Device/STM32F103C6TX_FLASH.ld)

###############################################################################
set(EXECUTABLE ${CMAKE_PROJECT_NAME})

enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
set(CMAKE_CXX_EXTENSIONS ON)
###############################################################################

set(STM32CUBEMX_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY}_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY}_HAL_Driver/Inc/Legacy
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/${MCU_FAMILY}/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/vcom/Inc
    )

set(PROJECT_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE STM32CUBEMX_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/*.c
    )

set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Project)

add_executable(${EXECUTABLE}
    ${STM32CUBEMX_SOURCES}
    ${STARTUP_SCRIPT})

target_compile_definitions(${EXECUTABLE} PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    ${MCU_MODEL}
    USE_HAL_DRIVER)

target_include_directories(${EXECUTABLE} SYSTEM PRIVATE
    ${STM32CUBEMX_INCLUDE_DIRECTORIES})

target_include_directories(${EXECUTABLE} PRIVATE
    ${PROJECT_INCLUDE_DIRECTORIES})
###############################################################################
target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wdouble-promotion
    -Wformat=2 -Wformat-truncation
    -Wundef
    -fno-common
    -Wno-unused-parameter
    $<$<COMPILE_LANGUAGE:CXX>:
        -Wconversion
        -Wno-volatile
        -Wold-style-cast
        -Wuseless-cast
        -Wsuggest-override>
    $<$<CONFIG:Debug>:-Og -g3 -ggdb>
    $<$<CONFIG:Release>:-Og -g0>)

target_link_options(${EXECUTABLE} PRIVATE
    -T${MCU_LINKER_SCRIPT}
    ${CPU_PARAMETERS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    $<$<VERSION_GREATER:$<C_COMPILER_VERSION>,10.3.1>:-Wl,--no-warn-rwx-segments>
    -Wl,--start-group
    -lc
    -lm
    -Wl,--end-group
    -Wl,--print-memory-usage)
###############################################################################

# The last command can take a couple of seconds on larger project, usefull for debugging
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.bin)
# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Board selection
set(PICO_BOARD pico CACHE STRING "Board type")

# == DO NOT EDIT below for Pico VS Code Extension ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if(EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ================================================

# Pull in Raspberry Pi Pico SDK
include(pico_sdk_import.cmake)

project(termometr C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Import FreeRTOS for RP2040 (GCC)
include(external/FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# Add executable and its source files
add_executable(termometr
    src/termometr.c
    src/onewire.c
    src/DS18B20.c
)

# Include header directories
target_include_directories(termometr PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

# Enable USB CDC for stdio
pico_enable_stdio_uart(termometr 0)
pico_enable_stdio_usb(termometr 1)

# Link libraries
target_link_libraries(termometr
    pico_stdlib
    hardware_gpio
    pico_multicore
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
)

# Set program name and version
pico_set_program_name(termometr "termometr")
pico_set_program_version(termometr "0.1")

# Generate UF2, map/bin
pico_add_extra_outputs(termometr)

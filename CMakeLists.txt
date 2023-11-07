#    if you want to use this library you must add the following lines in your root CMakeLists.txt:
#    unfortunately the PICO definitions must be inserted before the first project.
#
#    # This project uses the Raspberry Pi Pico SDK which has to be initialized prior to the project() command
#    set(PICO_SDK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/prgb-pico/external/pico-sdk)
#    include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
#
#    project(<yourproject> C CXX ASM)
#
#    # Initialize the Pico SDK
#    pico_sdk_init()

#    message("======================= Adding extra output ====================")
#    #enable STDIO via USB (cat << /dev/ttyACM0)
#    pico_enable_stdio_usb(${PROJECT_NAME} 1)
#    pico_enable_stdio_uart(${PROJECT_NAME} 0)
#    pico_add_extra_outputs(${PROJECT_NAME})


set(PICO_NO_FLASH 0)

add_definitions(-UPICO_NO_FLASH)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

project(prgb-pico)

add_library(${PROJECT_NAME} INTERFACE)

add_subdirectory(WS2812B)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        include
        external/FastLED/src
)

target_link_libraries(${PROJECT_NAME} INTERFACE
    pico_stdlib
    pico_multicore
    hardware_adc
    hardware_dma
    kissfft
)

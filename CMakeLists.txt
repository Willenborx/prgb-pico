
set(PICO_NO_FLASH 0)

set (PICO_SDK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/pico-sdk)

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
add_definitions(-UPICO_NO_FLASH)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

pico_sdk_init()

project(pico)

add_library(${PROJECT_NAME} INTERFACE)


add_subdirectory(WS2812B)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        include
    #PRIVATE
        ../prgb/include
        
)

target_link_libraries(${PROJECT_NAME} INTERFACE
    pico_stdlib
    pico_multicore
    hardware_adc
    hardware_dma
    kissfft
)

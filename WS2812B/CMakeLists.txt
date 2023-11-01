# WS2812B Project: Implementation of WS2812B LED control

set (PICO_SDK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../external/pico-sdk)
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project (WS2812B)

pico_sdk_init()

add_library(WS2812B STATIC 
    src/W2812Array.cpp
)
# pico_generate_pio_header(WS2812B ${CMAKE_CURRENT_LIST_DIR}/src/WS2812.pio)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		include
		#../include
	PRIVATE
		src
		#../../prgb/include
)

target_link_libraries(${PROJECT_NAME} 
		INTERFACE 
			pico_stdlib
			hardware_pio
		PRIVATE
			prgb
)

target_link_libraries(${PROJECT_NAME} 
    INTERFACE 
     	WS2812B 
    PRIVATE 
        pico_stdlib
		hardware_pio
        pico)
#endif()


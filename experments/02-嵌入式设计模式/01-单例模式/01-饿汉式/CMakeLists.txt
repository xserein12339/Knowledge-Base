cmake_minimum_required(VERSION 3.16)
project(uart_singleton C)


add_library(uart_singleton STATIC
    src/singleton_pattern1.c
    src/uart_mock.c
)

target_include_directories(uart_singleton
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include  
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src 
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_executable(uart_test src/main.c)
    target_link_libraries(uart_test PRIVATE uart_singleton)
endif()
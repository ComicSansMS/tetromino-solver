project(tetromino_solver)

cmake_minimum_required(VERSION 3.0)

set(TETROMINO_SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(TETROMINO_INCLUDE_DIR ${PROJECT_SOURCE_DIR})

set(TETROMINO_SOURCE_FILES
    ${TETROMINO_SOURCE_DIR}/main.cpp
    ${TETROMINO_SOURCE_DIR}/DLX.cpp
    ${TETROMINO_SOURCE_DIR}/tetromino.cpp
)

set(TETROMINO_HEADER_FILES
    ${TETROMINO_INCLUDE_DIR}/DLX.hpp
    ${TETROMINO_INCLUDE_DIR}/exceptions.hpp
    ${TETROMINO_INCLUDE_DIR}/polyomino.hpp
    ${TETROMINO_INCLUDE_DIR}/problem_instance.hpp
    ${TETROMINO_INCLUDE_DIR}/tetromino.hpp
)
source_group("Tetromino Headers" FILES ${TETROMINO_HEADER_FILES})

add_executable(tetromino_solver ${TETROMINO_SOURCE_FILES} ${TETROMINO_HEADER_FILES})
target_include_directories(tetromino_solver PUBLIC ${TETROMINO_INCLUDE_DIR})
if(MSVC)
    target_compile_options(tetromino_solver PUBLIC $<$<CONFIG:DEBUG>:/W4>)
else()
    target_compile_options(tetromino_solver PUBLIC -std=c++11 -stdlib=libc++)
endif()

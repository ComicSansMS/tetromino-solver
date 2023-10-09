cmake_minimum_required(VERSION 3.25)

project(tetromino_solver)

set(CMAKE_CXX_STANDARD 20)

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

add_executable(tetromino_solver)
target_sources(tetromino_solver
    PUBLIC
    ${TETROMINO_SOURCE_FILES}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${TETROMINO_INCLUDE_DIR} FILES
    ${TETROMINO_HEADER_FILES}
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT tetromino_solver)

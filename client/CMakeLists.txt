cmake_minimum_required(VERSION 3.23)

project(client VERSION 1.0)

set(ASIO_DIR ../Asio/asio/include)
set(SFML_DIR ../SFML/include/SFML)

find_path(ASIO_INCLUDE_DIR asio.hpp
    PATHS ${ASIO_DIR}
    PATH_SUFFIXES asio
)

include_directories(${ASIO_INCLUDE_DIR})
include_directories(${SFML_DIR})

find_library(SFML_GRAPHICS_LIBRARY sfml-graphics HINTS ../SFML/lib)
find_library(SFML_WINDOW_LIBRARY sfml-window HINTS ../SFML/lib)
find_library(SFML_SYSTEM_LIBRARY sfml-system HINTS ../SFML/lib)
find_library(SFML_SYSTEM_LIBRARY sfml-audio HINTS ../SFML/lib)
find_library(SFML_SYSTEM_LIBRARY sfml-network HINTS ../SFML/lib)

file(GLOB SOURCES "src/*.cpp" "src/**/*.cpp" "src/**/**/*.cpp" "src/**/**/**/*.cpp" "src/**/**/**/**/*.cpp")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/monsters/ DESTINATION ${CMAKE_BINARY_DIR}/assets/monsters)
file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/player/ DESTINATION ${CMAKE_BINARY_DIR}/assets/player)
file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/ DESTINATION ${CMAKE_BINARY_DIR}/assets/entity)
file(COPY ${CMAKE_SOURCE_DIR}/assets/parallax/ DESTINATION ${CMAKE_BINARY_DIR}/assets/parallax)

add_executable(
	client
	${SOURCES}
)

target_compile_features(client PUBLIC cxx_std_20)
target_compile_options(client PRIVATE -std=c++20)

target_link_libraries(
	client
	${SFML_SYSTEM_LIBRARY}
	${SFML_WINDOW_LIBRARY}
	${SFML_GRAPHICS_LIBRARY}
	${SFML_SYSTEM_LIBRARY}
	${SFML_SYSTEM_LIBRARY}
	Threads::Threads
)
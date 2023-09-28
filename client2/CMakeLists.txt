cmake_minimum_required(VERSION 3.23)

project(client VERSION 1.0)

set(SFML_BINARY_DIR ${CMAKE_BINARY_DIR}/sfml_build)

add_subdirectory(../libs/submodules ${SFML_BINARY_DIR})

file(GLOB SOURCES "src/*.cpp" "src/**/*.cpp" "src/**/**/*.cpp" "src/**/**/**/*.cpp" "src/**/**/**/**/*.cpp")

file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/monsters/ DESTINATION ${CMAKE_BINARY_DIR}/monsters)
file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/player/ DESTINATION ${CMAKE_BINARY_DIR}/player)
file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/ DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/assets/parallax/ DESTINATION ${CMAKE_BINARY_DIR})

add_executable(
	client
	${SOURCES}
)

target_compile_features(client PUBLIC cxx_std_20)
target_compile_options(client PRIVATE -std=c++20)

target_link_libraries(
	client
	sfml-system
	sfml-window
	sfml-graphics
	sfml-audio
	sfml-network
)
cmake_minimum_required(VERSION 3.19)

project(client VERSION 1.0)

include(FetchContent)

set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(
	SFML
	GIT_REPOSITORY https://github.com/SFML/SFML.git
	GIT_TAG 2.5.1
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

FetchContent_MakeAvailable(SFML)

file(GLOB SOURCES "src/*.cpp" "src/**/*.cpp" "src/**/**/*.cpp" "src/**/**/**/*.cpp" "src/**/**/**/**/*.cpp")

file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/monsters/ DESTINATION ${CMAKE_BINARY_DIR}/monsters)
file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/player/ DESTINATION ${CMAKE_BINARY_DIR}/player)
file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/ DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/assets/parallax/ DESTINATION ${CMAKE_BINARY_DIR})

add_executable(
	client
	${SOURCES}
)

target_link_libraries(
	client
	sfml-graphics
)
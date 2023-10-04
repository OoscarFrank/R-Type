cmake_minimum_required(VERSION 3.23)

project(client VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)

set(ASIO_DIR ../Asio/asio/include)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

set(SFML_STATIC_LIBRARIES TRUE)

find_path(ASIO_INCLUDE_DIR asio.hpp
    PATHS ${ASIO_DIR}
    PATH_SUFFIXES asio
)

include_directories(${ASIO_INCLUDE_DIR})

file(GLOB SOURCES "src/*.cpp" "src/**/*.cpp" "src/**/**/*.cpp" "src/**/**/**/*.cpp" "src/**/**/**/**/*.cpp")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/monsters/ DESTINATION ${CMAKE_BINARY_DIR}/assets/monsters)
file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/player/ DESTINATION ${CMAKE_BINARY_DIR}/assets/player)
file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/ DESTINATION ${CMAKE_BINARY_DIR}/assets/entity)
file(COPY ${CMAKE_SOURCE_DIR}/assets/parallax/ DESTINATION ${CMAKE_BINARY_DIR}/assets/parallax)

add_subdirectory(../extern/SFML extern/SFML_build)

add_executable(
	client
	${SOURCES}
)

target_compile_features(sfml-audio PRIVATE cxx_std_11)
target_compile_features(client PRIVATE cxx_std_20)

target_link_libraries(
	client
	sfml-system
    sfml-graphics
	sfml-audio
	sfml-window
	sfml-network
	Threads::Threads
)

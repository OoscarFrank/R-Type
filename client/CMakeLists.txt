cmake_minimum_required(VERSION 3.23)

project(r-type_client VERSION 1.0)

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

file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/monsters/ DESTINATION ${CMAKE_BINARY_DIR}/Release/assets/monsters)
file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/player/ DESTINATION ${CMAKE_BINARY_DIR}/Release/assets/player)
file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/ DESTINATION ${CMAKE_BINARY_DIR}/Release/assets/entity)
file(COPY ${CMAKE_SOURCE_DIR}/assets/parallax/ DESTINATION ${CMAKE_BINARY_DIR}/Release/assets/parallax)

file(COPY ${CMAKE_SOURCE_DIR}/../extern/dll/ DESTINATION ${CMAKE_BINARY_DIR}/Release/)
file(COPY ${CMAKE_SOURCE_DIR}/../extern/SFML/extlibs/bin/x64/openal32.dll DESTINATION ${CMAKE_BINARY_DIR}/Release/)

add_subdirectory(../extern/SFML extern/SFML_build)

add_executable(
	${PROJECT_NAME}
	${SOURCES}
)

if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PRIVATE _WIN32_WINNT=0x0A00 WIN32_LEAN_AND_MEAN)
endif()

target_compile_features(sfml-audio PRIVATE cxx_std_11)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_link_libraries(
	${PROJECT_NAME}
	sfml-system
    sfml-graphics
	sfml-audio
	sfml-window
	sfml-network
	Threads::Threads
)

cmake_minimum_required(VERSION 3.12)
project(server)

set(ASIO_DIR ../extern/asio/include)

find_path(ASIO_INCLUDE_DIR asio.hpp
    PATHS ${ASIO_DIR}
    PATH_SUFFIXES asio
)

include_directories(${ASIO_INCLUDE_DIR})

if(WIN32)
  macro(get_WIN32_WINNT version)
    if (CMAKE_SYSTEM_VERSION)
      set(ver ${CMAKE_SYSTEM_VERSION})
      string(REGEX MATCH "^([0-9]+).([0-9])" ver ${ver})
      string(REGEX MATCH "^([0-9]+)" verMajor ${ver})
      if ("${verMajor}" MATCHES "10")
        set(verMajor "A")
        string(REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver})
      endif ("${verMajor}" MATCHES "10")
      string(REPLACE "." "" ver ${ver})
      string(REGEX REPLACE "([0-9A-Z])" "0\\1" ver ${ver})
      set(${version} "0x${ver}")
    endif(CMAKE_SYSTEM_VERSION)
  endmacro(get_WIN32_WINNT)

  if(NOT DEFINED _WIN32_WINNT)
    get_WIN32_WINNT(ver)
    set(_WIN32_WINNT ${ver})
  endif()

  message(STATUS "Set _WIN32_WINNET=${_WIN32_WINNT}")

  target_compile_definitions(asio
    INTERFACE
      _WIN32_WINNT=${_WIN32_WINNT}
      WIN32_LEAN_AND_MEAN
  )
endif()

aux_source_directory(./src/ dir_src)
aux_source_directory(./src/Game/ dir_game)
aux_source_directory(./src/Pool/ dir_pool)
aux_source_directory(./src/Reader/ dir_reader)
aux_source_directory(./src/Utils/ dir_utils)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(
	server
	${dir_src}
    ${dir_game}
    ${dir_pool}
    ${dir_reader}
    ${dir_utils}
)

# Si vous avez besoin de lier la bibliothèque Asio, utilisez
target_link_libraries(
  server 
  ${ASIO_LIBRARY}
  PRIVATE Threads::Threads
)

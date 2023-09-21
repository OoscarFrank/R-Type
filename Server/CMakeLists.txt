cmake_minimum_required(VERSION 3.19)

project(server VERSION 1.0)

set(ASIO_CMAKE_ASIO_TAG origin/master)

if(NOT DEFINED ASIO_CMAKE_ASIO_TAG)
  message(FATAL_ERROR "You must specify ASIO_CMAKE_ASIO_TAG!")
endif()

if(NOT DEFINED ASIO_CMAKE_ASIO_SOURCE_DIR)
  set(ASIO_CMAKE_ASIO_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
endif()

set(ASIO_CMAKE_ASIO_DEP_DIR "${ASIO_CMAKE_ASIO_SOURCE_DIR}/asio-${ASIO_CMAKE_ASIO_TAG}-src")

include(FetchContent)

FetchContent_Declare(
	  ASIO
  	GIT_REPOSITORY  https://github.com/chriskohlhoff/asio.git
  	GIT_TAG         ${ASIO_CMAKE_ASIO_TAG}
  	GIT_SHALLOW     TRUE
  	SOURCE_DIR      ${ASIO_CMAKE_ASIO_DEP_DIR}
)

set(ASIO_CMAKE_ASIO_TAG asio-1-12-2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

FetchContent_MakeAvailable(ASIO)

if(NOT asio_POPULATED)
  message(STATUS "Fetching asio ${ASIO_CMAKE_ASIO_TAG}")

  if(EXISTS "${ASIO_CMAKE_ASIO_DEP_DIR}/asio")
    set(FETCHCONTENT_SOURCE_DIR_ASIO ${ASIO_CMAKE_ASIO_DEP_DIR})
    message(STATUS "asio ${ASIO_CMAKE_ASIO_TAG} source found at ${ASIO_CMAKE_ASIO_DEP_DIR}; skip fetching.")
  endif()

  FetchContent_Populate(asio)
  message(STATUS "Populating asio ${ASIO_CMAKE_ASIO_TAG} done; src at ${asio_SOURCE_DIR}")
endif()

add_library(asio INTERFACE)

target_include_directories(asio
  INTERFACE ${asio_SOURCE_DIR}/asio/include
)

target_compile_definitions(asio
  INTERFACE
    ASIO_STANDALONE
    ASIO_NO_DEPRECATED
)

if(UNIX)
  target_link_libraries(
    asio
    INTERFACE pthread
  )
elseif(WIN32)
  # macro see @ https://stackoverflow.com/a/40217291/1746503
  macro(get_WIN32_WINNT version)
    if (CMAKE_SYSTEM_VERSION)
      set(ver ${CMAKE_SYSTEM_VERSION})
      string(REGEX MATCH "^([0-9]+).([0-9])" ver ${ver})
      string(REGEX MATCH "^([0-9]+)" verMajor ${ver})
      # Check for Windows 10, b/c we'll need to convert to hex 'A'.
      if ("${verMajor}" MATCHES "10")
        set(verMajor "A")
        string(REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver})
      endif ("${verMajor}" MATCHES "10")
      # Remove all remaining '.' characters.
      string(REPLACE "." "" ver ${ver})
      # Prepend each digit with a zero.
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

add_executable(
	server
	${dir_src}
    ${dir_game}
    ${dir_pool}
    ${dir_reader}
    ${dir_utils}
)

target_link_libraries(
	server
	PRIVATE asio
)
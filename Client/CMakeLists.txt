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

if(UNIX)
  target_link_libraries(
    sfml-graphics
  )
elseif(WIN32)
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
endif()

aux_source_directory(./client/src/ dir_src)
aux_source_directory(./client/src/Error/ dir_error)
aux_source_directory(./client/src/Game/ dir_game)
aux_source_directory(./client/src/Game/Components/ dir_game_components)
aux_source_directory(./client/src/Game/Entity/ dir_game_entity)
aux_source_directory(./client/src/Game/Loader/ dir_game_loader)

file(COPY ${CMAKE_SOURCE_DIR}/client/assets/entity/monsters/ DESTINATION ${CMAKE_BINARY_DIR}/monsters)
file(COPY ${CMAKE_SOURCE_DIR}/client/assets/entity/player/ DESTINATION ${CMAKE_BINARY_DIR}/player)
file(COPY ${CMAKE_SOURCE_DIR}/client/assets/entity/ DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/client/assets/parallax/ DESTINATION ${CMAKE_BINARY_DIR})

add_executable(
	client
	${dir_src}
    ${dir_error}
    ${dir_game}
    ${dir_game_components}
    ${dir_game_entity}
    ${dir_game_loader}
)
target_link_libraries(
	client
	sfml-graphics
)
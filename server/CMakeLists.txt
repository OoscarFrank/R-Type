cmake_minimum_required(VERSION 3.19)
project(r-type_server)

set(ASIO_DIR ../Asio/asio/include)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

find_path(ASIO_INCLUDE_DIR asio.hpp
    PATHS ${ASIO_DIR}
    PATH_SUFFIXES asio
)

include_directories(${ASIO_INCLUDE_DIR})

aux_source_directory(./src/ dir_src)
aux_source_directory(./src/Game/ dir_game)
aux_source_directory(./src/Game/Entities/ dir_entities)
aux_source_directory(./src/Game/Entities/Monsters/ dir_monsters)
aux_source_directory(./src/Pool/ dir_pool)
aux_source_directory(./src/Reader/ dir_reader)
aux_source_directory(./src/Utils/ dir_utils)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(
	r-type_server
	${dir_src}
  ${dir_game}
  ${dir_entities}
  ${dir_monsters}
  ${dir_pool}
  ${dir_reader}
  ${dir_utils}
)

if(WIN32)
  target_compile_definitions(r-type_server PRIVATE _WIN32_WINNT=0x0A00 WIN32_LEAN_AND_MEAN)
endif()

target_link_libraries(
  r-type_server
  Threads::Threads
)

install(TARGETS r-type_server RUNTIME DESTINATION .)

if(UNIX)
  set(CPACK_GENERATOR "DEB;RPM")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "martin.d-herouville@epitech.eu")
elseif(WIN32)
  set(CPACK_NSIS_MODIFY_PATH "ON")
  set(CPACK_NSIS_CREATE_ICONS_EXTRA
    "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\RType_server.lnk' '$INSTDIR\\\\r-type_server' '-p 4243'"
  )
endif()

include(CPack)

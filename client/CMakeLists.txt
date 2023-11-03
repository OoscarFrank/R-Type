cmake_minimum_required(VERSION 3.19)
project(r-type_client)

set(CMAKE_CXX_STANDARD 11)

set(ASIO_DIR ../Asio/asio/include)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

if(UNIX)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Release)
endif()

set(SFML_STATIC_LIBRARIES TRUE)

find_path(ASIO_INCLUDE_DIR asio.hpp
    PATHS ${ASIO_DIR}
    PATH_SUFFIXES asio
)

include_directories(${ASIO_INCLUDE_DIR})

file(GLOB SOURCES "src/*.cpp" "src/**/*.cpp" "src/**/**/*.cpp" "src/**/**/**/*.cpp" "src/**/**/**/**/*.cpp")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/buttons/ DESTINATION ${CMAKE_BINARY_DIR}/Release/assets/buttons)
file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/missile/ DESTINATION ${CMAKE_BINARY_DIR}/Release/assets/missile)
file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/monsters/ DESTINATION ${CMAKE_BINARY_DIR}/Release/assets/monsters)
file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/player/ DESTINATION ${CMAKE_BINARY_DIR}/Release/assets/player)
file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/ DESTINATION ${CMAKE_BINARY_DIR}/Release/assets/entity)
file(COPY ${CMAKE_SOURCE_DIR}/assets/parallax/ DESTINATION ${CMAKE_BINARY_DIR}/Release/assets/parallax)
file(COPY ${CMAKE_SOURCE_DIR}/assets/screens/ DESTINATION ${CMAKE_BINARY_DIR}/Release/assets/screens)
file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/buttons DESTINATION ${CMAKE_BINARY_DIR}/Release/assets/entity/buttons)
file(COPY ${CMAKE_SOURCE_DIR}/assets/fonts/ DESTINATION ${CMAKE_BINARY_DIR}/Release/assets/fonts)
file(COPY ${CMAKE_SOURCE_DIR}/assets/songs/ DESTINATION ${CMAKE_BINARY_DIR}/Release/assets/songs/)
file(COPY ${CMAKE_SOURCE_DIR}/assets/entity/bonus DESTINATION ${CMAKE_BINARY_DIR}/Release/assets/entity/bonus)

if(WIN32)
  file(COPY ${CMAKE_SOURCE_DIR}/../extern/dll/ DESTINATION ${CMAKE_BINARY_DIR}/Release/)
  file(COPY ${CMAKE_SOURCE_DIR}/../extern/SFML/extlibs/bin/x64/openal32.dll DESTINATION ${CMAKE_BINARY_DIR}/Release/)
endif()

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

if(UNIX)
  install(TARGETS ${PROJECT_NAME} DESTINATION /usr/bin/)
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets/parallax/ DESTINATION /usr/share/${PROJECT_NAME}/assets/parallax/)
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets/entity/ DESTINATION /usr/share/${PROJECT_NAME}/assets/entity/)
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets/screens/ DESTINATION /usr/share/${PROJECT_NAME}/assets/screens/)
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets/songs/ DESTINATION /usr/share/${PROJECT_NAME}/assets/songs/)
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets/songs/ambient DESTINATION /usr/share/${PROJECT_NAME}/assets/songs/ambient)
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets/songs/effects DESTINATION /usr/share/${PROJECT_NAME}/assets/songs/effects)
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets/songs/levels DESTINATION /usr/share/${PROJECT_NAME}/assets/songs/levels)

  install(FILES ${CMAKE_SOURCE_DIR}/assets/logo.png DESTINATION /usr/share/${PROJECT_NAME}/)
  install(FILES ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.desktop DESTINATION /usr/share/applications/)
elseif(WIN32)
  install(TARGETS ${PROJECT_NAME} DESTINATION .)
  install(FILES ${CMAKE_SOURCE_DIR}/../extern/dll/sfml-audio-d-2.dll DESTINATION .)
  install(FILES ${CMAKE_SOURCE_DIR}/../extern/dll/sfml-graphics-d-2.dll DESTINATION .)
  install(FILES ${CMAKE_SOURCE_DIR}/../extern/dll/sfml-network-d-2.dll DESTINATION .)
  install(FILES ${CMAKE_SOURCE_DIR}/../extern/dll/sfml-system-d-2.dll DESTINATION .)
  install(FILES ${CMAKE_SOURCE_DIR}/../extern/dll/sfml-window-d-2.dll DESTINATION .)
  install(FILES ${CMAKE_SOURCE_DIR}/../extern/dll/sfml-audio-2.dll DESTINATION .)
  install(FILES ${CMAKE_SOURCE_DIR}/../extern/dll/sfml-graphics-2.dll DESTINATION .)
  install(FILES ${CMAKE_SOURCE_DIR}/../extern/dll/sfml-network-2.dll DESTINATION .)
  install(FILES ${CMAKE_SOURCE_DIR}/../extern/dll/sfml-system-2.dll DESTINATION .)
  install(FILES ${CMAKE_SOURCE_DIR}/../extern/dll/sfml-window-2.dll DESTINATION .)
  install(FILES ${CMAKE_SOURCE_DIR}/../extern/SFML/extlibs/bin/x64/openal32.dll DESTINATION .)
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets/parallax/ DESTINATION ./assets/parallax/)
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets/entity/ DESTINATION ./assets/entity/)
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets/screens/ DESTINATION ./assets/screens/)
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets/songs/ DESTINATION ./assets/songs/)
endif()

if(UNIX)
  set(CPACK_GENERATOR "DEB;RPM")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "martin.d-herouville@epitech.eu")
elseif(WIN32)
  set(CPACK_NSIS_MODIFY_PATH "ON")
  set(CPACK_NSIS_CREATE_ICONS_EXTRA
    "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\RType_client.lnk' '$INSTDIR\\\\r-type_client'"
  )
endif()

include(CPack)

cmake_minimum_required(VERSION 3.20)
project(ARXXDesktop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Shared game sources, matching web/CMakeLists.txt (see cmake/ArxSources.cmake).
include(cmake/ArxSources.cmake)

# Reuse the web front-end. main_web.cpp already runs a native
# while (appIsRunning()) loop for non-Emscripten builds; web_stubs.cpp
# provides link stubs. They will be renamed in a later step.
add_executable(arxx
  ${ARX_GAME_SOURCES}
  web/main_web.cpp
  web/web_stubs.cpp
)

target_include_directories(arxx PRIVATE ${CMAKE_SOURCE_DIR})

target_compile_definitions(arxx PRIVATE ARX_USE_SDL2=1)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)

target_link_libraries(arxx PRIVATE
  $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

# SDL2_image / SDL2_mixer config targets differ by triplet (shared vs static).
if(TARGET SDL2_image::SDL2_image)
  target_link_libraries(arxx PRIVATE SDL2_image::SDL2_image)
else()
  target_link_libraries(arxx PRIVATE SDL2_image::SDL2_image-static)
endif()

if(TARGET SDL2_mixer::SDL2_mixer)
  target_link_libraries(arxx PRIVATE SDL2_mixer::SDL2_mixer)
else()
  target_link_libraries(arxx PRIVATE SDL2_mixer::SDL2_mixer-static)
endif()

# Copy data/ next to the executable so the game finds its files when run
# from Visual Studio or Explorer. Keep Windows-specific bits inside if(WIN32).
if(WIN32)
  add_custom_command(TARGET arxx POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${CMAKE_SOURCE_DIR}/data
      $<TARGET_FILE_DIR:arxx>/data
    COMMENT "Copying data/ next to arxx executable"
  )
endif()

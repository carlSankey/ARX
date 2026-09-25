cmake_minimum_required(VERSION 3.20)
project(ARXXWebScaffold LANGUAGES CXX)

if(NOT EMSCRIPTEN)
  message(FATAL_ERROR "Use emcmake/emmake (Emscripten) for this target.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(arxx_web
  web_stubs.cpp
  main_web.cpp
  ../platform/AppLoop.cpp
  ../platform/Window.cpp
  ../platform/InputQueue.cpp
  ../platform/SDL2Window.cpp
  ../platform/FileSystem.cpp
  ../game.cpp
  ../actor.cpp
  ../level.cpp
  ../display.cpp
  ../font.cpp
  ../misc.cpp
  ../audio.cpp
  ../renderer/Sprite2D.cpp
  ../renderer/View3D.cpp
  ../automap.cpp
  ../bank.cpp
  ../chapel.cpp
  ../createCharacter.cpp
  ../damon.cpp
  ../dev.cpp
  ../dwarvenSmithy.cpp
  ../effects.cpp
  ../encounter.cpp
  ../ferry.cpp
  ../fountain.cpp
  ../goblins.cpp
  ../guild.cpp
  ../healer.cpp
  ../inn.cpp
  ../items.cpp
  ../itemSelect.cpp
  ../lift.cpp
  ../lyrics.cpp
  ../oracle.cpp
  ../player.cpp
  ../prison.cpp
  ../rathskeller.cpp
  ../retreat.cpp
  ../saveGame.cpp
  ../scenarioEntrance.cpp
  ../screen.cpp
  ../shop.cpp
  ../smithy.cpp
  ../spells.cpp
  ../staircase.cpp
  ../tavern.cpp
  ../trolls.cpp
  ../undeadKing.cpp
  ../vaults.cpp
  ../input.cpp
  ../constants.h
)

set_target_properties(arxx_web PROPERTIES SUFFIX ".html")

target_compile_definitions(arxx_web PRIVATE
  ARX_USE_SDL2=1
)

target_compile_options(arxx_web PRIVATE
  -O2
  -sUSE_SDL=2
  -sUSE_WEBGL2=1
)

target_link_options(arxx_web PRIVATE
  -O2
  -sWASM=1
  -sUSE_SDL=2
  -sUSE_SDL_MIXER=2
  -sUSE_WEBGL2=1
  -sUSE_VORBIS=1
  -sALLOW_MEMORY_GROWTH=1
  -sASSERTIONS=1
  --preload-file ../data@/data
)

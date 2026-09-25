# Shared SDL2 game sources (used by both the web build and the native desktop build).
#
# This list mirrors web/CMakeLists.txt (everything except web_stubs.cpp and
# main_web.cpp). Paths are prefixed with the repo root so this file can be
# included from any CMakeLists.txt.
#
# Known list differences (reported, not guessed):
# - web/build.cmd does NOT compile rathskeller.cpp, but web/CMakeLists.txt
#   does. This list follows web/CMakeLists.txt and INCLUDES rathskeller.cpp.
# - web/CMakeLists.txt also lists ../constants.h as an "executable source"
#   (a header, harmless for the linker). It is intentionally NOT included
#   here because this variable holds .cpp files only.
# - platform/platform.cpp (the plat:: layer from step 1) is NOT listed here:
#   nothing uses it yet; it stays out of this build.

set(_ARX_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")

set(ARX_GAME_SOURCES
  ${_ARX_ROOT}/platform/AppLoop.cpp
  ${_ARX_ROOT}/platform/Window.cpp
  ${_ARX_ROOT}/platform/InputQueue.cpp
  ${_ARX_ROOT}/platform/SDL2Window.cpp
  ${_ARX_ROOT}/game.cpp
  ${_ARX_ROOT}/actor.cpp
  ${_ARX_ROOT}/level.cpp
  ${_ARX_ROOT}/display.cpp
  ${_ARX_ROOT}/font.cpp
  ${_ARX_ROOT}/misc.cpp
  ${_ARX_ROOT}/audio.cpp
  ${_ARX_ROOT}/renderer/Sprite2D.cpp
  ${_ARX_ROOT}/renderer/View3D.cpp
  ${_ARX_ROOT}/automap.cpp
  ${_ARX_ROOT}/bank.cpp
  ${_ARX_ROOT}/chapel.cpp
  ${_ARX_ROOT}/createCharacter.cpp
  ${_ARX_ROOT}/damon.cpp
  ${_ARX_ROOT}/dev.cpp
  ${_ARX_ROOT}/dwarvenSmithy.cpp
  ${_ARX_ROOT}/effects.cpp
  ${_ARX_ROOT}/encounter.cpp
  ${_ARX_ROOT}/ferry.cpp
  ${_ARX_ROOT}/fountain.cpp
  ${_ARX_ROOT}/goblins.cpp
  ${_ARX_ROOT}/guild.cpp
  ${_ARX_ROOT}/healer.cpp
  ${_ARX_ROOT}/inn.cpp
  ${_ARX_ROOT}/items.cpp
  ${_ARX_ROOT}/itemSelect.cpp
  ${_ARX_ROOT}/lift.cpp
  ${_ARX_ROOT}/lyrics.cpp
  ${_ARX_ROOT}/oracle.cpp
  ${_ARX_ROOT}/player.cpp
  ${_ARX_ROOT}/prison.cpp
  ${_ARX_ROOT}/rathskeller.cpp
  ${_ARX_ROOT}/retreat.cpp
  ${_ARX_ROOT}/saveGame.cpp
  ${_ARX_ROOT}/scenarioEntrance.cpp
  ${_ARX_ROOT}/screen.cpp
  ${_ARX_ROOT}/shop.cpp
  ${_ARX_ROOT}/smithy.cpp
  ${_ARX_ROOT}/spells.cpp
  ${_ARX_ROOT}/staircase.cpp
  ${_ARX_ROOT}/tavern.cpp
  ${_ARX_ROOT}/trolls.cpp
  ${_ARX_ROOT}/undeadKing.cpp
  ${_ARX_ROOT}/vaults.cpp
  ${_ARX_ROOT}/input.cpp
)

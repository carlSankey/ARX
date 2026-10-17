@echo off
setlocal EnableExtensions

REM Build Milestone A web scaffold (no CMake, no PowerShell).
REM Usage: double-click or from repo root:  web\build.cmd

set "REPO_ROOT=%~dp0.."
set "OUT_DIR=%REPO_ROOT%\build-web"

if defined EMSDK (
  set "EMSDK_ROOT=%EMSDK%"
) else (
  set "EMSDK_ROOT=C:\dev\emsdk"
)

if not exist "%EMSDK_ROOT%\emsdk_env.bat" (
  echo ERROR: emsdk_env.bat not found at "%EMSDK_ROOT%"
  echo Set EMSDK to your emsdk folder, or install to C:\dev\emsdk
  exit /b 1
)

call "%EMSDK_ROOT%\emsdk_env.bat"
if errorlevel 1 (
  echo ERROR: failed to run "%EMSDK_ROOT%\emsdk_env.bat"
  exit /b 1
)

if not exist "%OUT_DIR%" mkdir "%OUT_DIR%"

REM Build stamp shown on the title screen: git commit + build time
set "GITREV=unknown"
for /f %%i in ('git -C "%REPO_ROOT%" rev-parse --short HEAD 2^>nul') do set "GITREV=%%i"
git -C "%REPO_ROOT%" diff --quiet 2>nul || set "GITREV=%GITREV%+changes"
> "%~dp0build_info.h" echo #pragma once
>> "%~dp0build_info.h" echo #define ARX_BUILD_REV "%GITREV%"
>> "%~dp0build_info.h" echo #define ARX_BUILD_TIME "%DATE% %TIME:~0,5%"
echo Build stamp: %GITREV% %DATE% %TIME:~0,5%

pushd "%~dp0"
emcc -O2 -std=c++20 main_web.cpp web_stubs.cpp ..\platform\AppLoop.cpp ..\platform\Window.cpp ..\platform\InputQueue.cpp ..\platform\SDL2Window.cpp ..\platform\FileSystem.cpp ..\game.cpp ..\actor.cpp ..\level.cpp ..\display.cpp ..\font.cpp ..\misc.cpp ..\audio.cpp ..\renderer\Sprite2D.cpp ..\renderer\View3D.cpp ..\automap.cpp ..\bank.cpp ..\createCharacter.cpp ..\damon.cpp ..\dev.cpp ..\effects.cpp ..\encounter.cpp ..\guild.cpp ..\items.cpp ..\itemSelect.cpp ..\player.cpp ..\saveGame.cpp ..\scenarioEntrance.cpp ..\spells.cpp ..\input.cpp ..\smithy.cpp ..\shop.cpp ..\tavern.cpp ..\healer.cpp ..\inn.cpp ..\ferry.cpp ..\staircase.cpp ..\lift.cpp ..\vaults.cpp ..\undeadKing.cpp ..\goblins.cpp ..\trolls.cpp ..\prison.cpp ..\chapel.cpp ..\fountain.cpp ..\oracle.cpp ..\lyrics.cpp ..\dwarvenSmithy.cpp ..\screen.cpp ..\retreat.cpp ..\rathskeller.cpp ..\module.cpp ..\arena.cpp -I.. -I"%EMSDK_ROOT%\upstream\emscripten\system\include" -o "%OUT_DIR%\arxx_web_v2.html" -DARX_USE_SDL2=1 -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS=png -sUSE_SDL_MIXER=2 -sUSE_WEBGL2=1 -sWASM=1 -sALLOW_MEMORY_GROWTH=1 -sASSERTIONS=1 -sASYNCIFY -sASYNCIFY_STACK_SIZE=524288 --preload-file "%REPO_ROOT%\data\images@data/images" --preload-file "%REPO_ROOT%\data\audio\cityGate1.ogg@data/audio/cityGate1.ogg" --preload-file "%REPO_ROOT%\data\audio\citygate4.ogg@data/audio/citygate4.ogg" --preload-file "%REPO_ROOT%\data\audio\gate1.wav@data/audio/gate1.wav" --preload-file "%REPO_ROOT%\data\audio\gate2.wav@data/audio/gate2.wav" --preload-file "%REPO_ROOT%\data\map\Scenario_0@data/map/Scenario_0" --preload-file "%REPO_ROOT%\data\map\Scenario_1@data/map/Scenario_1" --preload-file "%REPO_ROOT%\data\map\core@data/map/core"
if errorlevel 1 (
  popd
  exit /b 1
)
popd

echo OK: %OUT_DIR%\arxx_web_v2.html
exit /b 0
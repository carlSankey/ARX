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

pushd "%~dp0"
emcc -O2 -std=c++20 main_web.cpp web_stubs.cpp ..\platform\AppLoop.cpp ..\platform\Window.cpp ..\platform\InputQueue.cpp ..\platform\SDL2Window.cpp ..\game.cpp ..\actor.cpp ..\level.cpp ..\display.cpp ..\font.cpp ..\misc.cpp ..\audio.cpp ..\renderer\Sprite2D.cpp ..\renderer\View3D.cpp ..\automap.cpp ..\bank.cpp ..\createCharacter.cpp ..\damon.cpp ..\dev.cpp ..\effects.cpp ..\encounter.cpp ..\guild.cpp ..\items.cpp ..\itemSelect.cpp ..\player.cpp ..\saveGame.cpp ..\scenarioEntrance.cpp ..\spells.cpp ..\input.cpp ..\smithy.cpp ..\shop.cpp ..\tavern.cpp ..\healer.cpp ..\inn.cpp ..\ferry.cpp ..\staircase.cpp ..\lift.cpp ..\vaults.cpp ..\undeadKing.cpp ..\goblins.cpp ..\trolls.cpp ..\prison.cpp ..\chapel.cpp ..\fountain.cpp ..\oracle.cpp ..\lyrics.cpp ..\dwarvenSmithy.cpp ..\screen.cpp ..\retreat.cpp -I.. -I"%EMSDK_ROOT%\upstream\emscripten\system\include" -o "%OUT_DIR%\arxx_web_v2.html" -DARX_USE_SDL2=1 -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS=png -sUSE_SDL_MIXER=2 -sUSE_WEBGL2=1 -sWASM=1 -sALLOW_MEMORY_GROWTH=1 -sASSERTIONS=1 --preload-file "%REPO_ROOT%\data\images\core\arfont.png@data/images/core/arfont.png" --preload-file "%REPO_ROOT%\data\images\core\arfontSmooth.png@data/images/core/arfontSmooth.png" --preload-file "%REPO_ROOT%\data\images\core\logo640x240.png@data/images/core/logo640x240.png" --preload-file "%REPO_ROOT%\data\images\core\0.png@data/images/core/0.png" --preload-file "%REPO_ROOT%\data\images\core\1.png@data/images/core/1.png" --preload-file "%REPO_ROOT%\data\images\core\2.png@data/images/core/2.png" --preload-file "%REPO_ROOT%\data\images\core\3.png@data/images/core/3.png" --preload-file "%REPO_ROOT%\data\images\core\4.png@data/images/core/4.png" --preload-file "%REPO_ROOT%\data\images\core\5.png@data/images/core/5.png" --preload-file "%REPO_ROOT%\data\images\core\6.png@data/images/core/6.png" --preload-file "%REPO_ROOT%\data\images\core\7.png@data/images/core/7.png" --preload-file "%REPO_ROOT%\data\images\core\8.png@data/images/core/8.png" --preload-file "%REPO_ROOT%\data\images\core\9.png@data/images/core/9.png" --preload-file "%REPO_ROOT%\data\images\Scenario_0\Gate.png@data/images/Scenario_0/Gate.png" --preload-file "%REPO_ROOT%\data\images\Scenario_1\Gate.png@data/images/Scenario_1/Gate.png" --preload-file "%REPO_ROOT%\data\audio\cityGate1.ogg@data/audio/cityGate1.ogg" --preload-file "%REPO_ROOT%\data\audio\citygate4.ogg@data/audio/citygate4.ogg" --preload-file "%REPO_ROOT%\data\audio\gate1.wav@data/audio/gate1.wav" --preload-file "%REPO_ROOT%\data\audio\gate2.wav@data/audio/gate2.wav" --preload-file "%REPO_ROOT%\data\map\Scenario_0@data/map/Scenario_0" --preload-file "%REPO_ROOT%\data\map\Scenario_1@data/map/Scenario_1" --preload-file "%REPO_ROOT%\data\map\core@data/map/core" --preload-file "%REPO_ROOT%\data\images\backgrounds@data/images/backgrounds" --preload-file "%REPO_ROOT%\data\images\backgrounds\Updated@data/images/backgrounds/Updated" --preload-file "%REPO_ROOT%\data\images\textures_alternate@data/images/textures_alternate" --preload-file "%REPO_ROOT%\data\images\textures_original@data/images/textures_original" --preload-file "%REPO_ROOT%\data\images\Scenario_0\Banner.png@data/images/Scenario_0/Banner.png" --preload-file "%REPO_ROOT%\data\images\Scenario_0\BannerStatusLine.png@data/images/Scenario_0/BannerStatusLine.png" --preload-file "%REPO_ROOT%\data\images\core\compass_n.png@data/images/core/compass_n.png" --preload-file "%REPO_ROOT%\data\images\core\compass_s.png@data/images/core/compass_s.png" --preload-file "%REPO_ROOT%\data\images\core\compass_w.png@data/images/core/compass_w.png" --preload-file "%REPO_ROOT%\data\images\core\compass_e.png@data/images/core/compass_e.png" --preload-file "%REPO_ROOT%\data\images\Scenario_0@data/images/Scenario_0"
if errorlevel 1 (
  popd
  exit /b 1
)
popd

echo OK: %OUT_DIR%\arxx_web_v2.html
exit /b 0
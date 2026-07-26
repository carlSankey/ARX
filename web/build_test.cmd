@echo off
setlocal EnableExtensions

set "REPO_ROOT=%~dp0.."

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

pushd "%~dp0"
emcc graphics_test.cpp -o graphics_test.html -I.. -I"%EMSDK_ROOT%\upstream\emscripten\system\include" -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=png --preload-file "%REPO_ROOT%\data\images\backgrounds\cityBackE.png@data/images/backgrounds/cityBackE.png"
if errorlevel 1 (
  popd
  exit /b 1
)
popd

echo OK: graphics_test.html
exit /b 0
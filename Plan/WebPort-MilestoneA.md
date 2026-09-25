# ARXX Browser Port - Milestone A

Goal: add a browser build scaffold without changing the current Visual Studio build.

## What was added

- `web/main_web.cpp` — tiny Emscripten main-loop proof.
- `web/CMakeLists.txt` — optional CMake target (`arxx_web.html`) if you have CMake on PATH.
- `web/build.cmd` — **recommended on Windows** if PowerShell blocks scripts: uses `emsdk_env.bat`, no `.ps1`.
- `web/build.ps1` — optional; needs PowerShell execution policy (see below).

No existing native project files were modified (except `.gitignore` ignores `build-web/`).

## Build (recommended if scripts are disabled)

From repo root `c:\Source\ARXX`:

```bat
web\build.cmd
```

Or double-click `web\build.cmd` in Explorer.

Uses `EMSDK` if set, otherwise `C:\dev\emsdk`.

Output: `build-web/arxx_web.html` plus `.js` and `.wasm`.

## Manual activation (Command Prompt, no scripts)

```bat
C:\dev\emsdk\emsdk_env.bat
cd /d C:\Source\ARXX\web
mkdir ..\build-web 2>nul
emcc -O2 -std=c++20 main_web.cpp -o ..\build-web\arxx_web.html -sUSE_WEBGL2=1 -sWASM=1 -sALLOW_MEMORY_GROWTH=1 -sASSERTIONS=1
```

## PowerShell: “running scripts is disabled”

Pick one:

**A — One-off (current window only):**

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
& C:\dev\emsdk\emsdk_env.ps1
```

**B — Persistent for your user (common):**

```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

Then `& C:\dev\emsdk\emsdk_env.ps1` or `powershell -ExecutionPolicy Bypass -File web\build.ps1`.

## Build (CMake, optional)

Only if `cmake` is on PATH. After activating Emscripten in **cmd** (`emsdk_env.bat`) or PowerShell:

```bat
emcmake cmake -S web -B build-web
cmake --build build-web
```

## Run locally

After `emsdk_env.bat` (or equivalent):

```bat
emrun --no_browser --port 8080 build-web\arxx_web.html
```

Open `http://localhost:8080/arxx_web.html` and check the browser devtools console.

## Expected result

- Page loads.
- Console includes:
  - `ARXX web scaffold booted.`
  - `ARXX web scaffold running, frame=...`

## If something fails, send this

- Output of `emcc -v` (after `emsdk_env.bat`)
- Full error from `web\build.cmd` or CMake
- Whether `cmake` is installed (CMake-only failures)

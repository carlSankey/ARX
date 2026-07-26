# Build Milestone A web scaffold (no CMake required).
# Usage from repo root:  powershell -ExecutionPolicy Bypass -File web\build.ps1

$ErrorActionPreference = "Stop"
$repoRoot = Split-Path $PSScriptRoot -Parent

$emsdkRoot = $env:EMSDK
if (-not $emsdkRoot) { $emsdkRoot = "C:\dev\emsdk" }
$emsdkEnv = Join-Path $emsdkRoot "emsdk_env.ps1"
if (-not (Test-Path $emsdkEnv)) {
    Write-Error "emsdk_env.ps1 not found at '$emsdkEnv'. Set EMSDK to your emsdk folder or install to C:\dev\emsdk."
}

& $emsdkEnv
$outDir = Join-Path $repoRoot "build-web"
New-Item -ItemType Directory -Force -Path $outDir | Out-Null

Push-Location $PSScriptRoot
try {
    $html = Join-Path $outDir "arxx_web.html"
    emcc -O2 -std=c++20 main_web.cpp ..\platform\AppLoop.cpp ..\platform\Window.cpp ..\platform\InputQueue.cpp ..\platform\SDL2Window.cpp -I.. -o $html `
        -DARX_USE_SDL2=1 -sUSE_SDL=2 -sUSE_WEBGL2=1 -sWASM=1 -sALLOW_MEMORY_GROWTH=1 -sASSERTIONS=1
    Write-Host "OK: $html"
}
finally {
    Pop-Location
}

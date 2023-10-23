@echo off
setlocal enabledelayedexpansion

rem Define the macro to be used in the guards
set MACRO_NAME MY_HEADER_GUARD

rem Set the directory where your header files are located
set HEADER_DIRECTORY=F:\Alternate Reality

rem Iterate over all header files in the directory
for %%f in ("%HEADER_DIRECTORY%\*.h") do (
    set HEADER_FILE=%%~nxf
    set GUARD_NAME=!HEADER_FILE:.=!

    echo #ifndef %MACRO_NAME%_!GUARD_NAME! >> %%f
    echo #define %MACRO_NAME%_!GUARD_NAME! >> %%f
    echo. >> %%f
    echo // Your existing header content goes here >> %%f
    echo. >> %%f
    echo #endif // %MACRO_NAME%_!GUARD_NAME! >> %%f

    echo Added guards to !HEADER_FILE!
)

echo Header file guards have been added to all header files in %HEADER_DIRECTORY%.
pause

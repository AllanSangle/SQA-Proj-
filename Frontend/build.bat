@echo off
setlocal
rem Build all frontend sources using the project's CMakeLists.txt.
rem Usage: build.bat
where cmake >nul 2>&1
if errorlevel 1 (
    echo ERROR: CMake is not on PATH. Install CMake and reopen your terminal.
    exit /b 1
)
if not exist "%~dp0CMakeLists.txt" (
    echo ERROR: Place build.bat beside CMakeLists.txt in the Frontend folder.
    exit /b 1
)
echo Configuring frontend...
cmake -S "%~dp0." -B "%~dp0build" -DCMAKE_BUILD_TYPE=Debug
if errorlevel 1 (
    echo ERROR: Configuration failed. Check the CMake errors above.
    echo A C++17 compiler is required, for example Visual Studio with Desktop development with C++.
    exit /b 1
)
echo Building frontend...
cmake --build "%~dp0build" --config Debug --parallel
if errorlevel 1 (
    echo ERROR: Build failed. Check the compiler errors above.
    echo If frontend.exe is running, close it before rebuilding.
    exit /b 1
)
if exist "%~dp0build\Debug\frontend.exe" (
    echo Built: "%~dp0build\Debug\frontend.exe"
    exit /b 0
)
if exist "%~dp0build\frontend.exe" (
    echo Built: "%~dp0build\frontend.exe"
    exit /b 0
)
echo ERROR: Build completed but frontend.exe was not found in the expected locations.
exit /b 1

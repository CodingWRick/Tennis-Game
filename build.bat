@echo off
setlocal EnableDelayedExpansion

for /F %%a in ('echo prompt $E ^| cmd') do set "ESC=%%a"

echo %ESC%[35m============================================%ESC%[0m
echo %ESC%[35mBuilding test.exe...%ESC%[0m
echo %ESC%[35m============================================%ESC%[0m

set COMPILER=g++
set FLAGS=-std=c++20 -Wall -Wextra -O2 -static

set OUT_DIR=build
set EXE_NAME=test.exe

set ENET_ROOT=enet-1.3.18
set ENET_INCLUDE=%ENET_ROOT%\include
set ENET_SRC=%ENET_ROOT%\callbacks.c ^
             %ENET_ROOT%\compress.c ^
             %ENET_ROOT%\host.c ^
             %ENET_ROOT%\list.c ^
             %ENET_ROOT%\packet.c ^
             %ENET_ROOT%\peer.c ^
             %ENET_ROOT%\protocol.c ^
             %ENET_ROOT%\win32.c

if not exist "%OUT_DIR%" mkdir "%OUT_DIR%"

set SOURCES=

for %%f in (*.cpp) do (
    echo Found: %%f
    set SOURCES=!SOURCES! "%%f"
)

if "%SOURCES%"=="" (
    echo %ESC%[31mNO .cpp FILES FOUND%ESC%[0m
    pause
    exit /b 1
)

echo.
echo %ESC%[33mCompiling and Linking (with ENet)...%ESC%[0m

%COMPILER% ^
%SOURCES% ^
%ENET_SRC% ^
%FLAGS% ^
-I"%ENET_INCLUDE%" ^
-lgdi32 -luser32 -lws2_32 -lwinmm ^
-o "%OUT_DIR%\%EXE_NAME%"

if errorlevel 1 (
    echo.
    echo %ESC%[31mBuild FAILED%ESC%[0m
    pause
    exit /b 1
)

echo.
echo %ESC%[32mBuild SUCCESS%ESC%[0m
echo %ESC%[32mOutput: %OUT_DIR%\%EXE_NAME%%ESC%[0m
echo ============================================
pause
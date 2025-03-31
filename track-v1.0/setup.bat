@echo off
echo Installing Trak v1.0...
echo Current directory: %CD%
echo Files in current directory:
dir

set "INSTALL_DIR=%USERPROFILE%\bin"
set "BIN_PATH=%INSTALL_DIR%\trak.exe"
echo Install dir: %INSTALL_DIR%

if not exist "%INSTALL_DIR%" (
    echo Creating %INSTALL_DIR%...
    mkdir "%INSTALL_DIR%"
    if %ERRORLEVEL% NEQ 0 echo Failed to create directory.
)

echo Moving trak.exe to %BIN_PATH%...
move trak.exe "%BIN_PATH%"
if %ERRORLEVEL% NEQ 0 (
    echo Move failed. Run as Administrator.
    pause
    exit /b 1
)

echo Checking PATH...
echo %PATH% | find /i "%INSTALL_DIR%" >nul
if %ERRORLEVEL% NEQ 0 (
    echo Adding %INSTALL_DIR% to PATH...
    setx PATH "%PATH%;%INSTALL_DIR%" >nul
    if %ERRORLEVEL% NEQ 0 (
        echo PATH update failed. Add manually.
    ) else (
        echo PATH updated.
    )
) else (
    echo %INSTALL_DIR% already in PATH.
)

echo Launching trak...
start cmd /k "%BIN_PATH%"
if %ERRORLEVEL% NEQ 0 echo Launch failed.
echo Done!
pause
@echo off
echo ========================================
echo       Building No Seen Win Project      
echo ========================================
echo.

echo [1/2] Compiling Hook DLL...
gcc -shared -o hook.dll hook.c -Wl,--out-implib,libhook.a

if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to compile hook.dll
    goto end
)

echo [2/2] Compiling Main Executable...
gcc -o ghost.exe ghost.c

if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to compile ghost.exe
    goto end
)

echo.
echo [SUCCESS] Build completed successfully!
echo You can now run ghost.exe as Administrator.
echo.

:end
pause

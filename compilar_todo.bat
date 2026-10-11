@echo off
REM ===========================================================================
REM  compilar_todo.bat - compila y ejecuta los 14 ejercicios de EDD
REM  Uso:  compilar_todo.bat          (compila y ejecuta todos)
REM        compilar_todo.bat 07       (solo el ejercicio 07)
REM ===========================================================================
setlocal enabledelayedexpansion

cd /d "%~dp0"

where g++ >nul 2>nul
if errorlevel 1 (
    echo [ERROR] No se encontro g++ en el PATH.
    echo         Instala MinGW-w64 o agrega la carpeta bin de MinGW al PATH.
    exit /b 1
)

set DIR=Ejercicios\Resueltos
REM -static evita depender de las DLL de MinGW en tiempo de ejecucion.
set FLAGS=-std=c++17 -Wall -Wextra -O2 -static

if "%~1"=="" (
    echo.
    echo ==========================================================
    echo  COMPILANDO Y EJECUTANDO LOS 14 EJERCICIOS
    echo ==========================================================
    for %%F in ("%DIR%\*.cpp") do (
        set NAME=%%~nF
        echo.
        echo ---------- %%~nxF ----------
        g++ !FLAGS! "%%F" -o "%DIR%\%%~nF.exe"
        if errorlevel 1 (
            echo [FALLO AL COMPILAR] %%~nxF
        ) else (
            "%DIR%\%%~nF.exe"
            if errorlevel 1 echo [FALLO AL EJECUTAR] %%~nxF
        )
    )
    echo.
    echo ==========================================================
    echo  LISTO. Revisa las salidas anteriores.
    echo ==========================================================
) else (
    set BUSCADO=%1_
    for %%F in ("%DIR%\*.cpp") do (
        echo %%~nF | findstr /b /c:"!BUSCADO!" >nul && (
            g++ !FLAGS! "%%F" -o "%DIR%\%%~nF.exe"
            if errorlevel 1 (
                echo [FALLO AL COMPILAR] %%~nxF
            ) else (
                "%DIR%\%%~nF.exe"
            )
        )
    )
)

endlocal
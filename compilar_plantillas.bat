@echo off
REM ===========================================================================
REM  compilar_plantillas.bat - compila y ejecuta todas las plantillas de EDD
REM  Uso:  compilar_plantillas.bat        (todas)
REM        compilar_plantillas.bat 03     (solo la 03)
REM ===========================================================================
setlocal enabledelayedexpansion

cd /d "%~dp0Plantillas"

where g++ >nul 2>nul
if errorlevel 1 (
    echo [ERROR] No se encontro g++ en el PATH.
    exit /b 1
)

REM -static evita depender de las DLL de MinGW en tiempo de ejecucion
set FLAGS=-std=c++17 -Wall -Wextra -O2 -static

if "%~1"=="" (
    echo.
    echo ===========================================================
    echo  COMPILANDO Y EJECUTANDO todas las plantillas
    echo ===========================================================
    for %%F in ("*.cpp") do (
        echo.
        echo ---------- %%~nxF ----------
        g++ !FLAGS! "%%F" -o "%%~nF.exe"
        if errorlevel 1 (
            echo [FALLO AL COMPILAR] %%~nxF
        ) else (
            "%%~nF.exe"
            if errorlevel 1 echo [FALLO AL EJECUTAR] %%~nxF
        )
    )
    echo.
    echo ===========================================================
    echo  LISTO.
    echo ===========================================================
) else (
    set BUSCADO=%1_
    for %%F in ("*.cpp") do (
        echo %%~nF | findstr /b /c:"!BUSCADO!" >nul && (
            g++ !FLAGS! "%%F" -o "%%~nF.exe"
            if errorlevel 1 (
                echo [FALLO AL COMPILAR] %%~nxF
            ) else (
                "%%~nF.exe"
            )
        )
    )
)

endlocal
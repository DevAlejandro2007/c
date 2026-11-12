@echo off
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 main.c logica.c render.c ui.c puntajes.c platform/term.c -o tetris.exe
if errorlevel 1 exit /b %errorlevel%
echo Compilacion completada: tetris.exe

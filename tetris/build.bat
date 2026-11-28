@echo off
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 -I. main.c juego/logica.c juego/puntajes.c interfaz/render.c interfaz/ui.c platform/term.c -o tetris.exe
if errorlevel 1 exit /b %errorlevel%
echo Compilacion completada: tetris.exe

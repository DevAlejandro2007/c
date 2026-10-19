# Aprendizaje del lenguaje C

> Proyecto: Una seria de sub-proyectos con el objetivo de aprender C

---

## Badges

![C](https://img.shields.io/badge/language-16.1.0-brightgreen?logo=C)
![Git](https://img.shields.io/badge/Git--orange?logo=Git\&logoColor=orange)
![GitHub](https://img.shields.io/badge/Github--grey?logo=Github\&logoColor=black)

---

## Tabla de contenidos

* [Descripción](#descripción)
* [Características](#características)
* [Tecnologías](#tecnologías)
* [Requisitos previos](#requisitos-previos)
* [Instalación (local)](#instalación-local)
* [Ejecutar juegos](#ejecutar-juegos)
---

## Descripción

En este repositorio veremos distintas formas de aprender C, desde lo basico hasta lo avanzado, cada carpeta es un juego diferente programado en C, para ejecutar en terminal o PowerShell

> Nota: la carpeta `tetris` solo esta disponible para si ejecución en sistemas WINDOWS 11, ya que las librerias para reconocer las teclas son independientes para cada sistema operativo.

---

## Características

* Solo lenguaje C
* Diseñado para estudiantes en español
* Se utiliza .h para pre cargar funciones y bibliotecas

---

## Tecnologías
### Lenguaje 
* C / gcc 16.1.0
### admin_personas

* stdio.h
* stdlib.h
* string.h

### tetris 

* stdio.h
* stdlib.h
* time.h

###  tres_raya

* stdio.h 
* math.h

---

## Requisitos previos

* Git
* C / gcc -version 16.1.0
* Ganas de estudiar C

---

## Instalación (local)

1. Clona el repo:

```bash
git https://github.com/DevAlejandro2007/c.git
cd c
```

---

## Ejecutar juegos 

Asegurate de estar en ka carpeta del juego ejemplo : "cd /tetris" 

```bash
tetris.exe -> PARA WINDOWS
tetris -> PARA LINUX
```
debes de estar en la misma carpeta para ejecutarlo, sino, no reconocera tu archivo 

---



## Cambios

Si has realizado algun cambio en el codigo, recuerda volver a compilarlo antes de ejecutarlo con: 

EJEMPLO PARA TETRIS:
(se deben de incluir todos los archivos para su compilacion)

```bash
gcc logica.c main.c render.c -o tetris.exe 
```

---





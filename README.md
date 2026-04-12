# Sistema de Liga de Futbol

**Estudiante:** Alessandro De Bonis

---

## 1. Descripcion general del proyecto

Este proyecto consiste en una aplicacion de consola desarrollada en **C++** para gestionar una liga de futbol.

El programa permite leer la configuracion inicial de la liga desde un archivo, registrar partidos jugados, guardar la informacion en archivos de texto, construir la tabla de posiciones, mostrar estadisticas especiales y exportar ciertos datos a formato CSV.

La aplicacion fue desarrollada siguiendo una estructura modular, usando `struct`, `vector`, funciones y punteros, tal como se pide en el enunciado del proyecto.

---

## 2. Objetivo del programa

El objetivo principal del sistema es administrar una liga de futbol de manera sencilla desde consola, permitiendo que la informacion quede almacenada en archivos para que no se pierda al cerrar el programa.

El sistema busca resolver las siguientes necesidades:

- Cargar el nombre de la liga, sistema de puntos y equipos participantes
- Registrar partidos jugados
- Guardar resultados en archivos
- Construir automaticamente la tabla de posiciones
- Mostrar el historial de jornadas
- Mostrar todos los partidos registrados
- Exportar informacion a archivos CSV
- Incluir una funcionalidad extra creativa: estadisticas especiales de la liga

---

## 3. Funcionalidades implementadas

El programa incluye las siguientes opciones en el menu principal:

### 1. Ver tabla de posiciones

Lee todos los partidos almacenados, construye la tabla de posiciones, la ordena por puntos y la muestra en consola. Ademas, guarda la tabla en el archivo `tabla.txt`.

### 2. Registrar resultado de un partido

Permite al usuario ingresar la fecha, seleccionar el equipo local, seleccionar el equipo visitante e ingresar los goles de cada uno. Luego guarda el partido en `partidos.txt` y lo registra tambien en `fechas.txt` como parte del historial de jornadas.

### 3. Ver historial de jornadas

Muestra en consola el contenido de `fechas.txt`, donde quedan guardadas las jornadas registradas.

### 4. Ver todos los partidos jugados

Lee `partidos.txt` y muestra todos los partidos registrados hasta el momento.

### 5. Exportar archivos a CSV

Convierte la informacion de partidos y tabla de posiciones a archivos `.csv`, para que pueda abrirse facilmente en Excel o herramientas similares.

### 6. Ver estadisticas especiales

Esta es una funcionalidad extra agregada de forma creativa. Muestra:

- Lider actual de la liga
- Ultimo lugar
- Mejor ataque
- Mejor defensa
- Partido mas goleador

### 7. Reiniciar estadisticas de la tabla actual

Esta opcion reinicia en memoria las estadisticas de la tabla actual usando punteros. Se implemento como ejercicio adicional para demostrar el uso de punteros sobre estructuras de tipo `Team`.

### 8. Salir

Cierra el programa.

---

## 4. Estructura del proyecto

El proyecto esta organizado en carpetas para separar el codigo fuente de los archivos de datos.

```txt
liga-futbol/
  data/
    config.txt
    fechas.txt
    partidos.txt
    tabla.txt
  src/
    main.cpp
    league.cpp
    league.h
  README.md
  .gitignore
```

---

## 5. Compilacion del programa

Para compilar el proyecto, debes ubicarte en la carpeta raiz del mismo y ejecutar el siguiente comando en la terminal:

```bash
g++ src/main.cpp src/league.cpp -o program
```
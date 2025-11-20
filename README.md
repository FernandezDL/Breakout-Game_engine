# Raíces eternas - Sprites

## Descripción
Raíces eternas es un juego donde el personaje principal es el Árbol Raíz, un guardian ancestral que unía la tierra, el agua y el aire y potegía a todos los habitantes del bosque de los distintos enemigos que amenazaban con destruir el ciclo natural y marchitar por completo todo el ecosistema.

La primera versión del fondo del juego simula un background de grama para simular que se está dentro de un bosque, mientras que el sprite del personaje principal es directamente el Árbol Raíz.

## Sprites
<img width="1024" height="1024" alt="Arbol" src="https://github.com/user-attachments/assets/e888df5f-b1b1-451e-ae21-897817749e27" />
<img width="1024" height="520" alt="Abeja" src="https://github.com/user-attachments/assets/64c37825-7d90-4acb-9315-99acd793cdf4" />

## Video
https://github.com/user-attachments/assets/c129158b-3990-482c-afec-9521e8320ac9

## Reflexión

## Comandos para compliar
1. Borrar build en caso de tenerlo
`rmdir .\build -Recurse -Force 2>$null`

2. 
`$env:PATH = "ruta-al-" + $env:PATH`

3. 
`cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_C_COMPILER="ruta-al-gcc.exe-de-mingw64" -DCMAKE_CXX_COMPILER="ruta-al-g++.exe-de-mingw64" -DCMAKE_MAKE_PROGRAM="ruta-al-mingw32-make.exe-de-mingw64" -DCMAKE_SH="CMAKE_SH-NOTFOUND"`

4. 
`cmake --build build -j`

5. 
`.\build\GAME.exe`

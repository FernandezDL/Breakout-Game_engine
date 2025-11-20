# Raíces perdidas - Intgrid

## Descripción
Para poder hacer un juego más interesante se agregó un IntGrid y algunas variaciones de celdas:

- Caminable: Permite caminar por ella
- No caminable : No permite caminar por ahí
- Peligrosa: Baja vida
- Lenta: Realentiza al personaje

Cuando el juego está corriendo se puede presionar G para activar o desactivar el grid para "pintar" los distintos tipos de celdas, y el "pincel" se puede cambiar con los números para cada uno de los tipos de celda.

Con F5 se puede guardar la configuración hecha en el respectivo CSV para usar en próximos juegos.

## Video 
https://github.com/user-attachments/assets/61d1a5dc-a86d-4958-aaac-314e16d7e11d

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

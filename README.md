# Raíces perdidas - Tilemap

## Descripción
Teniendo en cuenta que en los bosques no hay solo grama, con los tilemaps se agregó la posibilidad de tener pedazos de tierra también, por lo que se definió un archivo CSV que indicara el tipo de tile que se iba a poner en cada espacio de la pantalla y así poder variar el fondo del juego.

## Video 
https://github.com/user-attachments/assets/6ae41ae8-574e-43df-b771-2635eecaea46

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

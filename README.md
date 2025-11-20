# Raíces perdidas - Tilemap

## Descripción
Teniendo en cuenta que en los bosques no hay solo grama, con los tilemaps se agregó la posibilidad de tener pedazos de tierra también, por lo que se definió un archivo CSV que indicara el tipo de tile que se iba a poner en cada espacio de la pantalla y así poder variar el fondo del juego.

## Video 
https://github.com/user-attachments/assets/6ae41ae8-574e-43df-b771-2635eecaea46

## Reflexión
Usar una imagen de fondo en la fase de los sprites representa una buena opción para dar un inicio al juego, sin embargo, y por lo menos en mi caso, hacerlo de esta manera generaba un resultado que, a pesar de verse bien, tenía una baja calidad y se miraba incluso "estirado".


Agregar los tilemaps permitió generar un nuevo patrón para el fondo del videojuego y evitar que el fondo sea de un solo color y diseño repetitivo, sino que utiliza una mezcla de varios tiles para dar personalidad al juego desde el background.


Uno de los problemas que enfrenté al implementar este detalle fue el definir el tamaño correcto que tuvieras las tiles, pues en la mayoría de casos los tamaños que generaba eran o demasiado pequeños, o todos se sobreponían entre ellos. Hubieron varias iteraciones de prueba y error, hasta que logré llegar a una configuración donde las tiles se miraban de un buen tamaño relativo al de los personajes, lo suficientemente grande para resaltar y mostrar los detalles en el juego, pero no demasiado que con dos tiles se llenara toda la pantalla

## Comandos para compliar
1. Borrar build en caso de tenerlo
`rmdir .\build -Recurse -Force 2>$null`

2. `$env:PATH = "ruta-al-" + $env:PATH`

3. `cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_C_COMPILER="ruta-al-gcc.exe-de-mingw64" -DCMAKE_CXX_COMPILER="ruta-al-g++.exe-de-mingw64" -DCMAKE_MAKE_PROGRAM="ruta-al-mingw32-make.exe-de-mingw64" -DCMAKE_SH="CMAKE_SH-NOTFOUND"`

4. `cmake --build build -j`

5. `.\build\GAME.exe`

# Raíces eternas - Intgrid

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
Con un IntGrid se pueden agregar nuevas reglas avanzadas para dar una capa mayor de complejidad al videojuego y hacerlo más interesante.

En este caso se agregaron variaciones para permitir distinto tipos de celdas, entre las cuales están:

- Las caminables: donde el jugador puede pasar, permitiendo moverse al rededor de la partida
- Los no caminables: funcionan como paredes invisibles y áreas donde donde el jugador no puede entrar, agregando dificultad a la partida al bloquear ciertos caminos por los cuales el jugador pueda pasar para huir de los enemigos.
- Peligrosa: las celdas peligrosas funcionan como espacios en los cuales la vida del jugador va disminuyendo, agregando un nivel extra de dificultad al tener otro espacio del cual el jugador debe estar pendiente de no pisar al correr el riesgo de hacerse daño.
- Lenta: durante el juego el personaje mantiene una velocidad constante, sin embargo en estas celdas las velocidad a la que se traslada el personaje se reduce, haciendo que vaya más lento durante unos segundos, dificultando su habilidad de escapar y esquivar a los enemigos.

Realizar esta nueva capa de reglas permite que el juego sea más interesante, sin embargo, también fue un reto poder aplicar estas nuevas reglas, primero el pensar en powerups o "trampas" interesantes y que se puedan basar y combinar con el juego y la historia. Así mismo también fue un reto poder aplicar los cambios de velocidad y el manejo de las vidas del personaje al pasar sobre las determinadas celdas.


Algunos de los elementos visuales también fueron un reto, se utilizó la grilla del tilemap como una base, sin embargo, el manejo de color para cada "pincel" fue un poco de reto tomando en cuenta que los colores cambiaban dependiendo de la textura del tile que tenían por debajo. A pesar de los retos, creo que el resultado que le da esta implementación impulsa el juego. Me gusta especialmente el hecho de poder activar y desactivar el pintado de los cuadros con la tecla G, dando al impresión de ser un juego mucho más formal, y el guardado en el CSV, que permite hacer permaniencia de datos entre partidas, también le da un toque extra.

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

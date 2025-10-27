# Breakout-Game_engine

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
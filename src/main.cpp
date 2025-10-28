#include "Game.h"

int main() {
    Game game("Final", 800, 450);

    Scene scene;                
    game.setScene(&scene);

    game.run(); 
    return 0;
}


// #include <raylib.h>
// int main() {
//     InitWindow(800, 450, "Sanity");
//     SetTargetFPS(60);
//     while (!WindowShouldClose()) {
//         BeginDrawing();
//         ClearBackground(RAYWHITE);
//         DrawText("Raylib OK", 20, 20, 20, BLACK);
//         EndDrawing();
//     }
//     CloseWindow();
//     return 0;
// }

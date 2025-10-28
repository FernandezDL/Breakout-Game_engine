#include "Game.h"

int main() {
    Game game("Final", 800, 450);

    Scene scene;                
    game.setScene(&scene);

    game.run(); 
    return 0;
}
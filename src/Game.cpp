#include <raylib.h>
#include <iostream>
#include "Game.h"

#define Num_blocks 10

Rectangle ball;
Rectangle paddle;
Rectangle blocks[Num_blocks];

int ball_sx = 150, ball_sy = -150;
int paddle_sx = 200;
int cols = 10, rows = 7, spacing= 5, width = 100, height = 20;
  
Game::Game(const char* title, int width, int height)
: screen_width(width), screen_height(height) {
	InitWindow(width, height, title);
	std::cout<<"GAME STARTED";
	isRunning = true;
	counter = 0;
}

Game::~Game() {}

void Game::setup() {
	SetTargetFPS(60);
	paddle = Rectangle{
        (float)(screen_width / 2) - 50, 
        (float)(screen_height - 30), 
        100, 
        15
    };

	ball = Rectangle{
        paddle.x + paddle.width / 2 - 7.5f,
        paddle.y - 15,
        15,
        15
    };

	int index = 0;
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			blocks[index] = Rectangle{spacing + (float)(j * (width + spacing)), spacing + (float)(i * (height + spacing)), (float)width, (float)height};

			// Asignar color según la fila
			switch(i) {
				case 0: blockColors[index] = RED; break;
				case 1: blockColors[index] = ORANGE; break;
				case 2: blockColors[index] = YELLOW; break;
				case 3: blockColors[index] = GREEN; break;
				case 4: blockColors[index] = BLUE; break;
				case 5: blockColors[index] = PURPLE; break;
				case 6: blockColors[index] = PINK; break;
				default: blockColors[index] = DARKGRAY; break;
			}

			index++;
		}
	}

	for (int i = 0; i < Num_blocks; i++) {
		blockAlive[i] = true;
	}
}

void Game::frame_start() {
	std::cout<<"==== FRAME "<< counter<<" START ====";
	BeginDrawing();
}

void Game::frame_end() {
	std::cout<<"==== FRAME END ====";
	EndDrawing();
	counter++;
}

void Game::handle_events() {
	float dT = GetFrameTime();
	if (WindowShouldClose()) isRunning = false;
	if (IsKeyDown(KEY_RIGHT)) {
        paddle.x += paddle_sx * dT;
        if (paddle.x + paddle.width > screen_width) {
            paddle.x = screen_width - paddle.width;
        }
    }

    if (IsKeyDown(KEY_LEFT)) {
        paddle.x -= paddle_sx * dT;
        if (paddle.x < 0) {
            paddle.x = 0;
        }
    }
}

void Game::update() {
	float dT = GetFrameTime();
	if (ball.x >= screen_width || ball.x < 0) ball_sx *= -1;
	if (ball.y >= screen_height) {
		std::cout<<"YOU FAIL";
		exit(1);
	}
	if (CheckCollisionRecs(ball, paddle)) {
		ball_sy *= -1;

		float paddleCenter = paddle.x + paddle.width / 2;
		float ballCenter = ball.x + ball.width / 2;
		float offset = (ballCenter - paddleCenter) / (paddle.width / 2);  // -1 a 1
		ball_sx += offset * 50;

		if (ball_sx > 0) ball_sx += 5;
		else ball_sx -= 5;

		if (ball_sy > 0) ball_sy += 5;
		else ball_sy -= 5;
	}

	// Colisión con bloques
    for (int i = 0; i < Num_blocks; i++) {
        if (blockAlive[i] && CheckCollisionRecs(ball, blocks[i])) {
            blockAlive[i] = false;  // destruir el bloque
            ball_sy *= -1;          // cambiar dirección vertical
        }
    }

	if (ball.y < 0) ball_sy *= -1;
	
	ball.x += ball_sx * dT;
	ball.y += ball_sy * dT;

	bool win = true;
	for (int i = 0; i < Num_blocks; i++) {
		if (blockAlive[i]) {
			win = false;
			break;
		}
	}

	if (win) {
		std::cout << "You win!" << std::endl;
		exit(0);
	}
}

void Game::render() {
	ClearBackground(GRAY);
	DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, GRAY);
	DrawRectangleRec(ball, BLACK);
	DrawRectangleRec(paddle, BLACK);

	for (int i = 0; i < Num_blocks; i++) {
		if (blockAlive[i]) {
			DrawRectangleRec(blocks[i], blockColors[i]);
		}
	}
}

void Game::clean() {
  	CloseWindow();
}

bool Game::running() {
  	return isRunning;
}

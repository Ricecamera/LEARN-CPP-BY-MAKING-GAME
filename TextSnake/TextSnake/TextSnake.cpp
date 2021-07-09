#include "TextSnake.h"
#include "CursesUtils.h"

using namespace std;

void InitGame(Game& game);
void InitPlayer(Game& game, Player& player);
void InitAppleSpawner(Game& game, AppleSpawner& appleSpawner);

int HandleInput(Game& game, Player& player);
void MovePlayer(const Game& game, Player& player);
void UpdateGame(Game& game, Player& player, AppleSpawner& appleSpawner, clock_t dt);
bool UpdateApple(const Game& game, Player& player, AppleSpawner& appleSpawner);
void UpdatePlayer(Game& game, Player& player, bool isAppleEaten);

bool IsCollision(Player& player, Apple& apple);

void DrawGame(const Game& game, const Player& player, const AppleSpawner& appleSpawner);
void DrawPlayer(const Player& player);
void DrawApples(const AppleSpawner& appleSpawner);

void ResetMovementTime(Player& player);
void ResetPlayer(Game& game, Player& player);

int main() {
	srand(time(NULL));

	Game game;
	Player player;
	AppleSpawner appleSpawner;

	InitializeCurses(true);
	InitGame(game);
	InitPlayer(game, player);
	InitAppleSpawner(game, appleSpawner);

	bool quit = false;
	int input{ 0 };
	clock_t lastTime = clock(); // from game loop

	while (!quit) {
		input = HandleInput(game, player);

		if (input != 'q') {

			clock_t currentTime = clock();
			clock_t dt = currentTime - lastTime;

			if (dt > CLOCKS_PER_SEC / FPS) { // ( tick per sec / frame per sec ) --> tick per frame
				lastTime = currentTime;

				UpdateGame(game, player, appleSpawner, dt);
				ClearScreen();
				DrawGame(game, player, appleSpawner);
				RefreshScreen();
			}
		}
		else {
			quit = true;
		}
	}

	//ClearSnakeBody(player);
	ShutdownCurses();
	return 0;
}

void InitGame(Game& game) {
	game.windowSize.height = ScreenHeight();
	game.windowSize.width = ScreenWidth();
	game.currentState = GS_PLAY;
	game.level = 1;
	game.gameTimer = 0;
}

void InitPlayer(Game& game, Player& player) {
	for (int i = 0; i < PLAYER_START_LENGTH; i++) {
		SnakePart block(game.windowSize.width / 2 + i, game.windowSize.height / 2);
		player.body.push_back(block);
	}
	
	
	player.length = PLAYER_START_LENGTH;
	player.live = MAX_NUMBER_OF_LIVE;
	player.score = 0;
	player.direction = PS_LEFT;
	ResetMovementTime(player);
}


void ResetPlayer(Game& game, Player& player) {
	player.body.clear();
	InitPlayer(game, player);
}

void InitAppleSpawner(Game& game, AppleSpawner& appleSpawner) {
	for (int i = 0; i < MAX_NUMBER_OF_APPLE; i++) {
		appleSpawner.apples[i].position.x = NOT_IN_PLAY;
		appleSpawner.apples[i].position.y = NOT_IN_PLAY;
		appleSpawner.apples[i].point = APPLE_POINT;
	}
	appleSpawner.spawnTimer = 0;
	appleSpawner.appleInPlay = 0;
}

int HandleInput(Game& game, Player& player) {
	int input = GetChar();

	switch (input) {
	case 'q':
		return input;

	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	}

	return ' ';
}

void MovePlayer(const Game& game, Player& player) {
	Position HeadPos = player.body[0].position; // save old position

	// move body parts after the snake head
	for (int i = 1; i < player.length; i++) {
		player.body[i].position = player.body[i - 1].position;
	}

	if (player.direction == PS_DOWN) {
		HeadPos.y++;
	}
	else if (player.direction == PS_UP) {
		HeadPos.y--;
	}
	else if (player.direction == PS_RIGHT) {
		HeadPos.x++;
	}
	else { // PS_LEFT
		HeadPos.x--;
	}

	player.body[0].position = HeadPos; // update next head position
}

void ResetMovementTime(Player& player) {
	player.movementTime = 1;
}
void UpdateGame(Game& game, Player& player, AppleSpawner& appleSpawner, clock_t dt) {
	game.gameTimer += dt;

	if (game.currentState == GS_PLAY) {
		bool isAppleEaten = false;
		isAppleEaten = UpdateApple(game, player, appleSpawner);

		UpdatePlayer(game, player, isAppleEaten);
	}
}

bool IsValidPosition(const AppleSpawner& applespawner, const Player& player, int xPos, int yPos) {
	for (int i = 0; i < player.length; i++) {
		SnakePart body = player.body[i];
		if (xPos == body.position.x && yPos == body.position.y) {
			return false;
		}
	}

	for (int i = 0; i < MAX_NUMBER_OF_APPLE; i++) {
		Apple apple = applespawner.apples[i];
		if (xPos == apple.position.x && yPos == apple.position.y) {
			return false;
		}
	}

	return true;
}

bool UpdateApple(const Game& game, Player& player, AppleSpawner& appleSpawner) {
	int countdown = ++appleSpawner.spawnTimer; // increment every frame

	// respawn a new apple
	if (countdown > (FPS * APPLE_RESPAWN_TIME) && appleSpawner.appleInPlay < MAX_NUMBER_OF_APPLE) {
		for (int i = 0; i < MAX_NUMBER_OF_APPLE; i++) {
			Apple* apple = &appleSpawner.apples[i];
			if (apple->position.x == NOT_IN_PLAY || apple->position.y == NOT_IN_PLAY) {
				int count = 0; // protected from forever loop
				int xPos{ -1 }, yPos{ -1 };

				// random position of the new apple
				do {
					count++;
					xPos = rand() % (game.windowSize.width + 1);
					yPos = rand() & (game.windowSize.height + 1);
				} while (!IsValidPosition(appleSpawner, player, xPos, yPos) && count < 100);
				
				// spawn the new apple
				if (count < 100) {
					apple->position.x = xPos;
					apple->position.y = yPos;

					appleSpawner.appleInPlay++;
					appleSpawner.spawnTimer = 0;
				}
				break;
			}
		}
	}

	for (int i = 0; i < MAX_NUMBER_OF_APPLE; i++) {
		Apple* apple = &appleSpawner.apples[i];
		if (IsCollision(player, *apple)) {
			player.score += apple->point;
			
			apple->position.x = NOT_IN_PLAY;
			apple->position.y = NOT_IN_PLAY;
			appleSpawner.appleInPlay--;
			return true;
		}
	}
	return false;
}

bool IsCollision(Player& player, Apple& apple) {
	SnakePart head = player.body[0];
	return (head.position.x == apple.position.x && head.position.y == apple.position.y);
}

void DrawGame(const Game& game, const Player& player, const AppleSpawner& appleSpawner) {
	DrawPlayer(player);
	DrawApples(appleSpawner);
}

void DrawPlayer(const Player& player) {
	for (int i = 0; i < player.length; i++) {
		SnakePart block = player.body[i];
		DrawCharacter(block.position.x, block.position.y, SNAKE_SPRITE);
	}
}

void DrawApples(const AppleSpawner& appleSpawner) {
	for (int i = 0; i < MAX_NUMBER_OF_APPLE; i++) {
		Apple apple = appleSpawner.apples[i];
		if (apple.position.x != NOT_IN_PLAY && apple.position.y != NOT_IN_PLAY) {
			DrawCharacter(apple.position.x, apple.position.y, APPLE_SPRITE);
		}
	}
}

void UpdatePlayer(Game& game, Player& player, bool isAppleEaten) {
	player.movementTime--;

	if (isAppleEaten) {
		// snake grow
	}

	if (player.movementTime < 0) {
		// move player
		MovePlayer(game, player);
		ResetMovementTime(player);
	}

	
}
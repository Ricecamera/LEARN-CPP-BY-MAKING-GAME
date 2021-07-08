#include "TextSnake.h"
#include "CursesUtils.h"

using namespace std;

void InitGame(Game& game);
void InitPlayer(Game& game, Player& player);
void InitAppleSpawner(Game& game, AppleSpawner& appleSpawner);

int HandleInput(Game& game, Player& player);
void MovePlayer(const Game& game, Player& player, int dx, int dy);
void UpdateGame(Game& game, Player& player, AppleSpawner& appleSpawner);

void DrawGame(const Game& game, const Player& player, const AppleSpawner& appleSpawner);
void DrawPlayer(const Player& player);

void ResetPlayer(Player& player);

int main() {
	Game game;
	Player player;
	AppleSpawner appleSpawner;

	InitializeCurses(true);
	InitGame(game);
	InitPlayer(game, player);
	InitAppleSpawner(game, appleSpawner);

	bool quit = false;
	int input{ 0 };

	while (!quit) {
		input = HandleInput(game, player);

		if (input != 'q') {
			UpdateGame(game, player, appleSpawner);
			ClearScreen();
			DrawGame(game, player, appleSpawner);
			RefreshScreen();
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
}

void InitPlayer(Game& game, Player& player) {
	SnakePart head(game.windowSize.width / 2 - 1, game.windowSize.height / 2);
	player.length = 1;
	player.live = MAX_NUMBER_OF_LIVE;
	player.score = 0;
	player.body.push_back(head);
}


void ResetPlayer(Player& player) {
	player.body.clear();
	player.length = 0;
}

void InitAppleSpawner(Game& game, AppleSpawner& appleSpawner) {

}

int HandleInput(Game& game, Player& player) {
	int input = GetChar();

	switch (input) {
	case 'q':
		return input;

	case KEY_LEFT:
		MovePlayer(game, player, -PLAYER_SPEED, 0);
		break;
	case KEY_RIGHT:
		MovePlayer(game, player, PLAYER_SPEED, 0);
		break;
	case KEY_UP:
		MovePlayer(game, player, 0, -PLAYER_SPEED);
		break;
	case KEY_DOWN:
		MovePlayer(game, player, 0, PLAYER_SPEED);
		break;
	}

	return ' ';
}

void MovePlayer(const Game& game, Player& player, int dx, int dy) {
	player.body[0].position.x += dx;
	player.body[0].position.y += dy;
}

void UpdateGame(Game& game, Player& player, AppleSpawner& appleSpawner) {

}

void DrawGame(const Game& game, const Player& player, const AppleSpawner& appleSpawner) {
	DrawPlayer(player);
}

void DrawPlayer(const Player& player) {
	for (int i = 0; i < player.length; i++) {
		SnakePart block = player.body[i];
		DrawCharacter(block.position.x, block.position.y, SNAKE_SPRITE);
	}
}


#include "TextSnake.h"
#include "CursesUtils.h"

using namespace std;

void InitGame(Game& game);
void InitPlayer(Game& game, Player& player);
void InitAppleSpawner(Game& game, AppleSpawner& appleSpawner);

int HandleInput(Game& game, Player& player);
void UpdateGame(Game& game, Player& player, AppleSpawner& appleSpawner);
void DrawGame(const Game& game, const Player& player, const AppleSpawner& appleSpawner);

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

}

void InitAppleSpawner(Game& game, AppleSpawner& appleSpawner) {

}

int HandleInput(Game& game, Player& player) {
	return 0;
}

void UpdateGame(Game& game, Player& player, AppleSpawner& appleSpawner) {

}

void DrawGame(const Game& game, const Player& player, const AppleSpawner& appleSpawner) {

}
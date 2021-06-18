#include "TextInvaders.h"
#include "CursesUtils.h"
#include <ctime>

using namespace std;

void InitGame(Game& game);
void InitPlayer(const Game& game, Player& player);
void ResetPlayer(const Game& game, Player& player);
void ResetMissile(Player& player);

int HandleInput(const Game& game, Player& player);
void PlayerShoot(Player& player);
void UpdateGame(Game& game, Player& player);
void UpdateMissile(Player& player);
void MovePlayer(const Game& game, Player& player, int dx);

void DrawGame(const Game& game, const Player& player);
void DrawPlayer(const Player& player, const char* sprite[]);

int main() {
	Game game;
	Player player;
	
	InitializeCurses(true);
	InitGame(game);
	InitPlayer(game, player);

	bool quit = false;
	int input{ 0 };
	clock_t lastTime = clock();

	while (!quit) {
		input = HandleInput(game, player);
		if (input != 'q') {

			clock_t currentTime = clock();
			clock_t dt = currentTime - lastTime;

			if (dt > CLOCKS_PER_SEC / FPS) {
				lastTime = currentTime;

				UpdateGame(game, player);
				ClearScreen();
				DrawGame(game, player);
				RefreshScreen();
			}
			
		}
		else {
			quit = true;
		}
		
	}

	ShutdownCurses();
	return 0;
}

void InitGame(Game& game) {
	game.windowSize.width = ScreenWidth();
	game.windowSize.height = ScreenHeight();
	game.level = 1;
	game.currentState = GS_PLAY;
}

void InitPlayer(const Game& game, Player& player) {
	player.live = MAX_NUMBER_OF_LIVES;
	player.spriteSize.height = PLAYER_SPRITE_HEIGHT;
	player.spriteSize.width = PLAYER_SPRITE_WIDTH;
	ResetPlayer(game, player);
}

void ResetPlayer(const Game& game, Player& player) {
	player.position.x = game.windowSize.width / 2 - player.spriteSize.width / 2;
	player.position.y = game.windowSize.height - player.spriteSize.height - 1;
	player.animation = 0;
	ResetMissile(player);
}

void ResetMissile(Player& player) {
	player.missile.x = NOT_IN_PLAY;
	player.missile.y = NOT_IN_PLAY;
}


int HandleInput(const Game& game, Player& player) {
	int input = GetChar();

	switch (input) {
	case 'q':
		return input;
	case KEY_LEFT:
		MovePlayer(game, player, -PLAYER_MOVEMENT_AMOUNT);
		break;
	case KEY_RIGHT:
		MovePlayer(game, player, PLAYER_MOVEMENT_AMOUNT);
		break;
	case ' ':
		PlayerShoot(player);
		break;
	}
	
	return ' ';
}

void UpdateGame(Game& game, Player& player) {
	UpdateMissile(player);
}

void DrawGame(const Game& game, const Player& player) {
	DrawPlayer(player, PLAYER_SPRITE);
}

void MovePlayer(const Game& game, Player& player, int dx) {
	if (player.position.x + player.spriteSize.width + dx > game.windowSize.width) {
		player.position.x = game.windowSize.width - player.spriteSize.width;
	}
	else if (player.position.x + dx < 0) {
		player.position.x = 0;
	}
	else {
		player.position.x += dx;
	}
}

void PlayerShoot(Player& player) {

	if ( player.missile.x == NOT_IN_PLAY || player.missile.y == NOT_IN_PLAY) {
		player.missile.y = player.position.y - 1;
		player.missile.x = player.position.x + player.spriteSize.width / 2;
	}
}

void DrawPlayer(const Player& player, const char* sprite[]) {
	DrawSprite(player.position.x, player.position.y, sprite, player.spriteSize.height);

	if (player.missile.y != NOT_IN_PLAY) {
		DrawCharacter(player.missile.x, player.missile.y, PLAYER_MISSILE_SPRITE);
	}
}

void UpdateMissile(Player& player) {
	if (player.missile.y != NOT_IN_PLAY) {
		player.missile.y -= PLAYER_MISSILE_SPEED;

		if (player.missile.y < 0) {
			ResetMissile(player);
		}
	}
}
#include "TextInvaders.h"
#include "CursesUtils.h"
#include <ctime>
#include <cstring>

using namespace std;

void InitGame(Game& game);
void InitPlayer(const Game& game, Player& player);
void InitShields(const Game& game, Shield shields[], int numberOfShields);
void InitAliens(const Game& game, AlienSwarm& aliens);
void CleanUpShields(Shield shields[], int numberOfShields);
void ResetPlayer(const Game& game, Player& player);
void ResetMissile(Player& player);
void ResetMovementTime(AlienSwarm& aliens);

int HandleInput(const Game& game, Player& player);
void PlayerShoot(Player& player);
void UpdateGame(Game& game, Player& player, Shield shields[], int numberOfShield, AlienSwarm& aliens);
void UpdateMissile(Player& player);
void UpdateAliens(const Game& game, AlienSwarm& aliens, Player& player, Shield shields[], int numberOfShields);
void MovePlayer(const Game& game, Player& player, int dx);
int IsCollision(const Position& projectile, const Shield shields[], int numberOfShields, Position& shieldCollisionPoint);
bool IsColission(const Player& player, const AlienSwarm& aliens, Position& alienCollisionPositionInArray);
void ResolveShieldCollision(Shield shields[], int shieldIndex, const Position& shieldCollisionPoint);
int ResolveAlienCollison(AlienSwarm& aliens, const Position& hitPositionInAliensArray);
void FindEmptyRowAndColumns(const AlienSwarm& aliens, int& emptyColLeft, int& emptyColRight, int& emptyRowsBottom);

void DrawGame(const Game& game, const Player& player, Shield shields[], int numberOfShields, const AlienSwarm& aliens);
void DrawPlayer(const Player& player, const char* sprite[]);
void DrawShields(const Shield shields[], int numberOfShields);
void DrawAliens(const AlienSwarm& aliens);

int main() {
	Game game;
	Player player;
	Shield shields[NUM_SHIELDS];
	AlienSwarm aliens;
	
	InitializeCurses(true);
	InitGame(game);
	InitPlayer(game, player);
	InitShields(game, shields, NUM_SHIELDS);
	InitAliens(game, aliens);

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

				UpdateGame(game, player, shields, NUM_SHIELDS, aliens);
				ClearScreen();
				DrawGame(game, player, shields, NUM_SHIELDS, aliens);
				RefreshScreen();
			}
			
		}
		else {
			quit = true;
		}
		
	}

	CleanUpShields(shields, NUM_SHIELDS);
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

void UpdateGame(Game& game, Player& player, Shield shields[], int numberOfShield, AlienSwarm& aliens) {
	UpdateMissile(player);

	Position shieldCollisionPoint;

	int shieldIndex = IsCollision(player.missile, shields, numberOfShield, shieldCollisionPoint);

	if (shieldIndex != NOT_IN_PLAY) {
		ResetMissile(player);
		ResolveShieldCollision(shields, shieldIndex, shieldCollisionPoint);
	}

	Position AlienCollisionPoint;

	if (IsColission(player, aliens, AlienCollisionPoint)) {
		ResetMissile(player);
		player.score += ResolveAlienCollison(aliens, AlienCollisionPoint);
	}
	UpdateAliens(game, aliens, player, shields, numberOfShield);
}
void FindEmptyRowAndColumns(const AlienSwarm& aliens, int& emptyColLeft, int& emptyColRight, int& emptyRowsBottom) {
	bool found = false;
	 
	for (int col = 0; col < NUM_ALIEN_COLUMNS && !found; col++) {
		for (int row = 0; row < NUM_ALIEN_ROWS && !found; row++) {
			if (aliens.aliens[row][col] == AS_DEAD) {
				if (row == NUM_ALIEN_ROWS - 1) {
					emptyColLeft++;
				}
			}
			else {
				found = true;
			}
		}
	}

	found = false;
	for (int col = NUM_ALIEN_COLUMNS - 1; col >= 0 && !found; col--) {
		for (int row = 0; row < NUM_ALIEN_ROWS && !found; row++) {
			if (aliens.aliens[row][col] == AS_DEAD) {
				if (row == NUM_ALIEN_ROWS - 1) {
					emptyColRight++;
				}
			}
			else {
				found = true;
			}
		}
	}

	found = false;
	for (int row = NUM_ALIEN_ROWS - 1; row >= 0 && !found; row--) {
		for (int col = 0; col < NUM_ALIEN_COLUMNS && !found; col++) {
			if (aliens.aliens[row][col] == AS_DEAD) {
				if (col == NUM_ALIEN_COLUMNS - 1) {
					emptyRowsBottom++;
				}
			}
			else {
				found = true;
			}
		}
	}
}

void UpdateAliens(const Game& game, AlienSwarm& aliens, Player& player, Shield shields[], int numberOfShields) {
	if (aliens.explosionTimer >= 0) {
		aliens.explosionTimer--; //if explosionTimer == 0 -> explosionTimer = NOT_IN_PLAY
	}

	for (int row = 0; row < NUM_ALIEN_ROWS; row++) {
		for (int col = 0; col < NUM_ALIEN_COLUMNS; col++) {
			if (aliens.aliens[row][col] == AS_EXPLODING && aliens.explosionTimer == NOT_IN_PLAY) {
				aliens.aliens[row][col] = AS_DEAD;
			}
		}
	}

	aliens.movementTime--;
	bool moveHorizontal = 0 >= aliens.movementTime;
	int emptyColsLeft = 0;
	int emptyColsRight = 0;
	int emptyRowsBottom = 0;

	FindEmptyRowAndColumns(aliens, emptyColsLeft, emptyColsRight, emptyRowsBottom);

	int numberOfColumns = NUM_ALIEN_COLUMNS - emptyColsLeft - emptyColsRight;
	int leftAlienPosition = aliens.position.x + emptyColsLeft * (aliens.spriteSize.width + ALIEN_PADDING);
	int rightAlienPosition = leftAlienPosition + numberOfColumns * aliens.spriteSize.width + (numberOfColumns - 1) * ALIEN_PADDING;

	if (((rightAlienPosition >= game.windowSize.width && aliens.direction > 0) || (leftAlienPosition <= 0 && aliens.direction < 0))
		&& moveHorizontal && (aliens.line > 0)) {
		//move down
		moveHorizontal = false;
		aliens.position.y++;
		aliens.line--;
		aliens.direction = -aliens.direction;
		ResetMovementTime(aliens);
	}

	if (moveHorizontal) {
		aliens.position.x += aliens.direction;
		ResetMovementTime(aliens);
		aliens.animation = aliens.animation == 0 ? 1 : 0;
	}
}

void DrawGame(const Game& game, const Player& player, Shield shields[], int numberOfShields, const AlienSwarm& aliens) {
	DrawPlayer(player, PLAYER_SPRITE);
	DrawAliens(aliens);
	DrawShields(shields, numberOfShields);
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

void InitShields(const Game& game, Shield shields[], int numberOfShields) {
	int firstPadding = ceil(float(game.windowSize.width - numberOfShields * SHEILD_SPRITE_WIDTH) / float(numberOfShields + 1));
	int xPadding = floor(float(game.windowSize.width - numberOfShields * SHEILD_SPRITE_WIDTH) / float(numberOfShields + 1));

	for (int i = 0; i < numberOfShields; i++) {
		Shield& shield = shields[i];
		shield.position.x = firstPadding + i * (SHEILD_SPRITE_WIDTH + xPadding);
		shield.position.y = game.windowSize.height - PLAYER_SPRITE_HEIGHT - SHEILD_SPRITE_HEIGHT - 3;

		for (int row = 0; row < SHEILD_SPRITE_HEIGHT; row++) {
			shield.sprite[row] = new char[SHEILD_SPRITE_WIDTH + 1];
			strcpy_s(shield.sprite[row], SHEILD_SPRITE_WIDTH + 1, SHIELD_SPRITE[row]);
		}
	}
}

void CleanUpShields(Shield shields[], int numberOfShields) {
	for (int i = 0; i < numberOfShields; i++) {
		Shield& shield = shields[i];
		for (int row = 0; row < SHEILD_SPRITE_HEIGHT; row++) {
			delete[] shield.sprite[row];
		}
	}
}

void DrawShields(const Shield shields[], int numberOfShields) {

	for (int i = 0; i < numberOfShields; i++) {
		const Shield& shield = shields[i];

		DrawSprite(shield.position.x, shield.position.y, (const char**)shield.sprite, SHEILD_SPRITE_HEIGHT);
	}
}

int IsCollision(const Position& projectile, const Shield shields[], int numberOfShields, Position& shieldCollisionPoint) {
	shieldCollisionPoint.x = NOT_IN_PLAY;
	shieldCollisionPoint.y = NOT_IN_PLAY;

	if (projectile.y != NOT_IN_PLAY) {
		for (int i = 0; i < numberOfShields; i++) {
			const Shield& shield = shields[i];

			if ((projectile.x >= shield.position.x && projectile.x < (shield.position.x + SHEILD_SPRITE_WIDTH))
				&& (projectile.y >= shield.position.y && projectile.y < (shield.position.y + SHEILD_SPRITE_HEIGHT))
				&& shield.sprite[projectile.y - shield.position.y][projectile.x - shield.position.x] != ' '){

				//We collided
				shieldCollisionPoint.x = projectile.x - shield.position.x;
				shieldCollisionPoint.y = projectile.y - shield.position.y;
				return i;
			}
		}
	}

	return NOT_IN_PLAY;
}

void ResolveShieldCollision(Shield shields[], int shieldIndex, const Position& shieldCollisionPoint) {
	shields[shieldIndex].sprite[shieldCollisionPoint.y][shieldCollisionPoint.x] = ' ';
}

void InitAliens(const Game& game, AlienSwarm& aliens) {
	for (int row = 0; row < NUM_ALIEN_ROWS; row++) {
		for (int col = 0; col < NUM_ALIEN_COLUMNS; col++) {
			aliens.aliens[row][col] = AS_ALIVE;
		}
	}

	aliens.direction = -1; //left
	aliens.numAliensLeft = NUM_ALIEN_ROWS * NUM_ALIEN_COLUMNS;
	aliens.animation = 0;
	aliens.spriteSize.width = ALIEN_SPRITE_WIDTH;
	aliens.spriteSize.height = ALIEN_SPRITE_HEIGHT;
	aliens.numberOfBombsInPlay = 0;
	aliens.position.x = (game.windowSize.width - NUM_ALIEN_COLUMNS * (ALIEN_SPRITE_WIDTH + ALIEN_PADDING)) / 2;
	aliens.position.y = game.windowSize.height - NUM_ALIEN_COLUMNS - NUM_ALIEN_ROWS * ALIEN_SPRITE_HEIGHT - ALIEN_PADDING - NUM_ALIEN_ROWS - 1 - 3 + game.level;
	aliens.line = NUM_ALIEN_COLUMNS - (game.level - 1);
	aliens.explosionTimer = 0;
}

void DrawAliens(const AlienSwarm& aliens) {
	//draw 1 row of the 30 point aliens
	const int NUM_30_POINT_ALIEN_ROWS = 1;

	for (int col = 0; col < NUM_ALIEN_COLUMNS; col++) {
		int xPos = aliens.position.x + col * (aliens.spriteSize.width + ALIEN_PADDING);
		int yPos = aliens.position.y;

		if (aliens.aliens[0][col] == AS_ALIVE) {
			DrawSprite(xPos, yPos, ALIEN30_SPRITE, aliens.spriteSize.height, aliens.animation*aliens.spriteSize.height);
		}
		else if (aliens.aliens[0][col] == AS_EXPLODING) {
			DrawSprite(xPos, yPos, ALIEN_EXPLOSION, aliens.spriteSize.height);
		}
	}

	//draw 2 row of the 20 point aliens
	const int NUM_20_POINT_ALIEN_ROWS = 2;

	for (int row = 0; row < NUM_20_POINT_ALIEN_ROWS; row++) {
		for (int col = 0; col < NUM_ALIEN_COLUMNS; col++) {
			int xPos = aliens.position.x + col * (aliens.spriteSize.width + ALIEN_PADDING);
			int yPos = aliens.position.y + row * (aliens.spriteSize.height + ALIEN_PADDING) + NUM_30_POINT_ALIEN_ROWS * (aliens.spriteSize.height + +ALIEN_PADDING);
			
			if (aliens.aliens[NUM_30_POINT_ALIEN_ROWS + row][col] == AS_ALIVE) {
				DrawSprite(xPos, yPos, ALIEN20_SPRITE, aliens.spriteSize.height, aliens.animation * aliens.spriteSize.height);
			}
			else if (aliens.aliens[NUM_30_POINT_ALIEN_ROWS + row][col] == AS_EXPLODING) {
				DrawSprite(xPos, yPos, ALIEN_EXPLOSION, aliens.spriteSize.height);
			}
		}
	}

	//draw 2 rows of the 10 point aliens
	const int NUM_10_POINT_ALIEN_ROWS = 2;
	for (int row = 0; row < NUM_10_POINT_ALIEN_ROWS; row++) {
		for (int col = 0; col < NUM_ALIEN_COLUMNS; col++) {
			int xPos = aliens.position.x + col * (aliens.spriteSize.width + ALIEN_PADDING);
			int yPos = aliens.position.y + row * (aliens.spriteSize.height + ALIEN_PADDING) + NUM_30_POINT_ALIEN_ROWS * (aliens.spriteSize.height + +ALIEN_PADDING) + NUM_20_POINT_ALIEN_ROWS * (aliens.spriteSize.height + +ALIEN_PADDING);

			if (aliens.aliens[NUM_30_POINT_ALIEN_ROWS + NUM_20_POINT_ALIEN_ROWS + row][col] == AS_ALIVE) {
				DrawSprite(xPos, yPos, ALIEN10_SPRITE, aliens.spriteSize.height, aliens.animation * aliens.spriteSize.height);
			}
			else if (aliens.aliens[NUM_30_POINT_ALIEN_ROWS + NUM_20_POINT_ALIEN_ROWS + row][col] == AS_EXPLODING) {
				DrawSprite(xPos, yPos, ALIEN_EXPLOSION, aliens.spriteSize.height);
			}
		}
	}
}

//Alien collision
bool IsColission(const Player& player, const AlienSwarm& aliens, Position& alienCollisionPositionInArray) {
	alienCollisionPositionInArray.x = NOT_IN_PLAY;
	alienCollisionPositionInArray.y = NOT_IN_PLAY;

	for (int row = 0; row < NUM_ALIEN_ROWS; row++) {
		for (int col = 0; col < NUM_ALIEN_COLUMNS; col++) {
			int xPos = aliens.position.x + col * (aliens.spriteSize.width + ALIEN_PADDING);
			int yPos = aliens.position.y + row * (aliens.spriteSize.height + ALIEN_PADDING);

			if (aliens.aliens[row][col] == AS_ALIVE &&
				player.missile.x >= xPos && player.missile.x < xPos + aliens.spriteSize.width &&
				player.missile.y >= yPos && player.missile.y < yPos + aliens.spriteSize.height)
			{
				alienCollisionPositionInArray.x = col;
				alienCollisionPositionInArray.y = row;
				return true;
			}
		}
	}
	return false;
}

int ResolveAlienCollison(AlienSwarm& aliens, const Position& hitPositionInAliensArray) {
	aliens.aliens[hitPositionInAliensArray.y][hitPositionInAliensArray.x] = AS_EXPLODING;
	aliens.numAliensLeft--;

	if (aliens.explosionTimer == NOT_IN_PLAY) {
		aliens.explosionTimer = ALIEN_EXPLOSION_TIME;
	}

	if (hitPositionInAliensArray.y == 0) {
		return 30;
	}
	else if (hitPositionInAliensArray.y >= 1 && hitPositionInAliensArray.y < 3) {
		return 20;
	}
	else {
		return 10;
	}
}

void ResetMovementTime(AlienSwarm& aliens) {
	aliens.movementTime = aliens.line * 2 + (5 * (float(aliens.numAliensLeft) / float(NUM_ALIEN_COLUMNS * NUM_ALIEN_ROWS)));
}
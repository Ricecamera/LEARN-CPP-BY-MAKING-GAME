#include "TextSnake.h"
#include "CursesUtils.h"

using namespace std;

void InitGame(Game& game);
void InitPlayer(Game& game, Player& player);
void InitAppleSpawner(AppleSpawner& appleSpawner);
void LoadHighScore(HighScoreTable& table);

int HandleInput(Game& game, Player& player, HighScoreTable& table, AppleSpawner& appleSpawner);
void MovePlayer(const Game& game, Player& player);
void ResetMovementTime(Player& player);
void ChangePlayerDirection(Player& player, PlayerDirection direction);
void UpdateGame(Game& game, Player& player, AppleSpawner& appleSpawner, clock_t dt);
void UpdateApple(const Game& game, Player& player, AppleSpawner& appleSpawner);
void UpdatePlayer(Game& game, Player& player, AppleSpawner& appleSpawner);
void SaveHighScore(const HighScoreTable& table);

bool IsCollision(Player& player, Apple& apple);
bool IsSelfCollision(vector<SnakePart>& snakeBody);
bool PlayerOutOfBound(const Game& game, const SnakePart& snakeHead);
void ResolveAppleCollision(Player& player, AppleSpawner& appleSpawner, Apple* apple);

void DrawGame(const Game& game, const Player& player, const AppleSpawner& appleSpawner, const HighScoreTable& table);
void DrawPlayer(const Player& player);
void DrawApples(const AppleSpawner& appleSpawner);
void ShowPlayerLive(const Game& game, const Player& player);
void DrawIntroScreen(const Game& game);
void DrawGameOverScreen(const Game& game);
void DrawHighScoreTable(const Game& game, const HighScoreTable& table);
void AddHighScore(HighScoreTable& table, int score, const std::string& name);

void ResetGame(Game& game, Player& player, AppleSpawner& appleSpawner);
void ResetPlayer(Game& game, Player& player);
void ResetApples(AppleSpawner& appleSpawner);
void ResetGameOverPositionCursor(Game& game);

int main() {
	srand(time(NULL));

	Game game;
	Player player;
	AppleSpawner appleSpawner;
	HighScoreTable table;

	InitializeCurses(true);
	InitGame(game);
	InitPlayer(game, player);
	InitAppleSpawner(appleSpawner);
	LoadHighScore(table);

	bool quit = false;
	int input{ 0 };
	clock_t lastTime = clock(); // from game loop

	while (!quit) {
		input = HandleInput(game, player, table, appleSpawner);

		if (input != 'q') {

			clock_t currentTime = clock();
			clock_t dt = currentTime - lastTime;

			if (dt > CLOCKS_PER_SEC / FPS) { // ( tick per sec / frame per sec ) --> tick per frame
				lastTime = currentTime;

				UpdateGame(game, player, appleSpawner, dt);
				ClearScreen();
				DrawGame(game, player, appleSpawner, table);
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
	game.windowSize.height = ScreenHeight();
	game.windowSize.width = ScreenWidth();
	game.currentState = GS_INTRO;
	game.level = 1;
	game.gameTimer = 0;

	ResetGameOverPositionCursor(game);
}

void ResetGame(Game& game, Player& player, AppleSpawner& appleSpawner) {
	game.gameTimer = 0;
	player.live = MAX_NUMBER_OF_LIVE;
	player.score = 0;

	ResetPlayer(game, player);
	ResetApples(appleSpawner);
	ResetGameOverPositionCursor(game);
}

void InitPlayer(Game& game, Player& player) {
	player.live = MAX_NUMBER_OF_LIVE;
	player.score = 0;
	ResetPlayer(game, player);
}

void ResetPlayer(Game& game, Player& player) {
	player.body.clear();
	for (int i = 0; i < PLAYER_START_LENGTH; i++) {
		SnakePart block(game.windowSize.width / 2 + i, game.windowSize.height / 2);
		player.body.push_back(block);
	}

	player.length = PLAYER_START_LENGTH;
	player.direction = PS_LEFT;
	player.animation = 0;
	ResetMovementTime(player);
}

void InitAppleSpawner(AppleSpawner& appleSpawner) {
	ResetApples(appleSpawner);
}

void ResetApples(AppleSpawner& appleSpawner) {
	for (int i = 0; i < MAX_NUMBER_OF_APPLE; i++) {
		appleSpawner.apples[i].position.x = NOT_IN_PLAY;
		appleSpawner.apples[i].position.y = NOT_IN_PLAY;
		appleSpawner.apples[i].point = APPLE_POINT;
	}
	appleSpawner.spawnTimer = 0;
	appleSpawner.appleInPlay = 0;
}

int HandleInput(Game& game, Player& player, HighScoreTable& table, AppleSpawner& appleSpawner) {
	int input = GetChar();

	switch (input) {
	case 'q':
		return input;
	case 's':
		if (game.currentState == GS_INTRO) {
			game.currentState = GS_HIGH_SCORES;
		}
		else if (game.currentState == GS_HIGH_SCORES) {
			game.currentState = GS_INTRO;
		}
		break;
	case KEY_LEFT:
		if (game.currentState == GS_PLAY) {
			ChangePlayerDirection(player, PS_LEFT);
		}
		else if (game.currentState == GS_GAME_OVER) {
			game.gameOverHPositionCursor -= 1;
			if (game.gameOverHPositionCursor < 0) {
				game.gameOverHPositionCursor = MAX_LENGTH_OF_NAME - 1;
			}
		}
		break;
	case KEY_RIGHT:
		if (game.currentState == GS_PLAY) {
			ChangePlayerDirection(player, PS_RIGHT);
		}
		else if (game.currentState == GS_GAME_OVER) {
			game.gameOverHPositionCursor = (game.gameOverHPositionCursor + 1) % MAX_LENGTH_OF_NAME;
		}
		break;
	case KEY_UP:
		if (game.currentState == GS_PLAY) {
			ChangePlayerDirection(player, PS_UP);
		}
		else if (game.currentState == GS_GAME_OVER) {
			int idx = game.gameOverHPositionCursor;
			game.gameOverVPositionCursor[idx] = (game.gameOverVPositionCursor[idx] + 1) % 26;
		}
		game.playerName[game.gameOverHPositionCursor] = 'A' + game.gameOverVPositionCursor[game.gameOverHPositionCursor];
		break;
	case KEY_DOWN:
		if (game.currentState == GS_PLAY) {
			ChangePlayerDirection(player, PS_DOWN);
		}
		else if (game.currentState == GS_GAME_OVER) {
			game.gameOverVPositionCursor[game.gameOverHPositionCursor] -= 1;

			if (game.gameOverVPositionCursor[game.gameOverHPositionCursor] < 0) {
				game.gameOverVPositionCursor[game.gameOverHPositionCursor] = 26 - 1;
			}
			game.playerName[game.gameOverHPositionCursor] = 'A' + game.gameOverVPositionCursor[game.gameOverHPositionCursor];
		}
		break;
	case ' ':
		if (game.currentState == GS_PLAYER_DEAD) {
			player.live--;
			player.animation = 0;
			if (player.live == 0) {
				game.currentState = GS_GAME_OVER;
			}
			else {
				ResetApples(appleSpawner);
				ResetPlayer(game, player);
				game.currentState = GS_WAIT;
				game.waitTimer = PLAYER_WAIT_TIME;
			}
		}
		else if (game.currentState == GS_INTRO) {
			game.currentState = GS_WAIT;
			game.waitTimer = PLAYER_WAIT_TIME;
		}
		else if (game.currentState == GS_GAME_OVER) {
			game.playerName[MAX_LENGTH_OF_NAME] = '\0';
			AddHighScore(table, player.score, game.playerName);
			ResetGame(game, player, appleSpawner);
			game.currentState = GS_HIGH_SCORES;
		}
		else if (game.currentState == GS_HIGH_SCORES) {
			game.currentState = GS_INTRO;
		}
	}
	
	return ' ';
}

void MovePlayer(const Game& game, Player& player) {
	Position HeadPos = player.body[0].position; // save old position

	// move body parts after the snake head
	for (int i = player.length - 1; i > 0; i--) {
		player.body[i].position = player.body[i - 1].position;
	}

	switch (player.direction) {
	case PS_UP:
		HeadPos.y -= PLAYER_SPEED;
		break;
	case PS_LEFT:
		HeadPos.x -= PLAYER_SPEED;
		break;
	case PS_DOWN:
		HeadPos.y += PLAYER_SPEED;
		break;
	case PS_RIGHT:
		HeadPos.x += PLAYER_SPEED;
		break;
	default:
		break;
	}

	player.body[0].position = HeadPos; // update next head position
}

void ResetMovementTime(Player& player) {
	player.movementTime = 3;
}

void ChangePlayerDirection(Player& player, PlayerDirection direction) {
	switch (direction) {
	case PS_UP:
		if (player.direction != PS_DOWN) {
			player.direction = PS_UP;
		}
		break;
	case PS_LEFT:
		if (player.direction != PS_RIGHT) {
			player.direction = PS_LEFT;
		}
		break;
	case PS_DOWN:
		if (player.direction != PS_UP) {
			player.direction = PS_DOWN;
		}
		break;
	case PS_RIGHT:
		if (player.direction != PS_LEFT) {
			player.direction = PS_RIGHT;
		}
		break;
	}
}

void UpdateGame(Game& game, Player& player, AppleSpawner& appleSpawner, clock_t dt) {
	game.gameTimer += dt;

	if (game.currentState == GS_PLAY) {
		UpdateApple(game, player, appleSpawner);
		UpdatePlayer(game, player, appleSpawner);

		// player's snake eat itself or player's snake out of game window
		if (IsSelfCollision(player.body) || PlayerOutOfBound(game, player.body[0])) {
			game.currentState = GS_PLAYER_DEAD;
		}
	}
	else if (game.currentState == GS_PLAYER_DEAD) {
		player.animation = (player.animation + 1) % PLAYER_ANIMATION_LENGTH;
	}
	else if (game.currentState == GS_WAIT) {
		game.waitTimer--;

		if (game.waitTimer == 0) {
			game.currentState = GS_PLAY;
		}
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

void UpdateApple(const Game& game, Player& player, AppleSpawner& appleSpawner) {
	if (appleSpawner.appleInPlay == MAX_NUMBER_OF_APPLE) {
		return;
	}
	int countdown = ++appleSpawner.spawnTimer; // increment every frame

	// respawn a new apple
	if (countdown > (FPS * APPLE_RESPAWN_TIME)) {

		for (int i = 0; i < MAX_NUMBER_OF_APPLE; i++) {
			Apple* apple = &appleSpawner.apples[i];
			if (apple->position.x == NOT_IN_PLAY || apple->position.y == NOT_IN_PLAY) {
				int count = 0; // protected from forever loop
				int xPos{ -1 }, yPos{ -1 };

				// random position of the new apple
				do {
					count++;
					xPos = rand() % (game.windowSize.width - 1);
					yPos = rand() % (game.windowSize.height - 1);
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
}

bool IsCollision(Player& player, Apple& apple) {
	SnakePart head = player.body[0];
	return (head.position.x == apple.position.x && head.position.y == apple.position.y);
}


void ResolveAppleCollision(Player& player, AppleSpawner& appleSpawner, Apple* apple) {
	player.score += apple->point;

	apple->position.x = NOT_IN_PLAY;
	apple->position.y = NOT_IN_PLAY;
	appleSpawner.appleInPlay--;
}

void DrawGame(const Game& game, const Player& player, const AppleSpawner& appleSpawner, const HighScoreTable& table) {
	if (game.currentState == GS_INTRO) {
		DrawIntroScreen(game);
	}
	else if (game.currentState == GS_GAME_OVER) {
		DrawGameOverScreen(game);
	}
	else if (game.currentState == GS_HIGH_SCORES) {
		DrawHighScoreTable(game, table);
	}
	else {
		if (game.currentState == GS_WAIT) {
			ShowPlayerLive(game, player);
		}
		DrawApples(appleSpawner);
		DrawPlayer(player);
	}
}

void DrawPlayer(const Player& player) {
	for (int i = 0; i < player.length; i++) {
		SnakePart block = player.body[i];
		DrawCharacter(block.position.x, block.position.y, SNAKE_SPRITE[player.animation]);
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

void UpdatePlayer(Game& game, Player& player, AppleSpawner& appleSpawner) {
	player.movementTime--;

	if (player.movementTime == 0) {
		// move player
		MovePlayer(game, player);
		ResetMovementTime(player);
	}

	// check if collision with apple
	bool isAppleEaten = false;
	for (int i = 0; i < MAX_NUMBER_OF_APPLE; i++) {
		Apple* apple = &appleSpawner.apples[i];
		if (IsCollision(player, *apple)) {
			ResolveAppleCollision(player, appleSpawner, apple);
			isAppleEaten = true;
			break;
		}
	}

	if (isAppleEaten) {
		// snake grow
		Position tailEndPos = player.body[player.length - 1].position;
		switch (player.direction)
		{
		case PS_UP:
			tailEndPos.y++;
			break;
		case PS_LEFT:
			tailEndPos.x++;
			break;
		case PS_DOWN:
			tailEndPos.y--;
			break;
		case PS_RIGHT:
			tailEndPos.x--;
			break;
		default:
			break;
		}

		SnakePart tailEnd(tailEndPos);
		player.body.push_back(tailEnd);
		player.length++;
	}
}

bool IsSelfCollision(vector<SnakePart>& snakeBody) {
	SnakePart head = snakeBody[0];

	for (int i = 1; i < snakeBody.size(); i++) {
		if (head.position.x == snakeBody[i].position.x &&
			head.position.y == snakeBody[i].position.y) {
			return true;
		}
	}
	return false;
}

bool PlayerOutOfBound(const Game& game, const SnakePart& snakeHead) {
	return (snakeHead.position.x >= game.windowSize.width) || (snakeHead.position.x < 0)
		|| (snakeHead.position.y < 0) || (snakeHead.position.y >= game.windowSize.height);
}

void ShowPlayerLive(const Game& game, const Player& player) {
	string gameTip = "Snake will grow every time you eat an apple";
	string playerLiveStr = "Now you have " + to_string(player.live) + " lives";

	const int yPos = game.windowSize.height / 2 - 3;
	const int xPos1 = game.windowSize.width / 2 - gameTip.length() / 2;
	const int xPos2 = game.windowSize.width / 2 - playerLiveStr.length() / 2;

	DrawString(xPos1, yPos, gameTip.c_str());
	DrawString(xPos2, yPos + 1, playerLiveStr.c_str());
}

void DrawIntroScreen(const Game& game) {
	string startString = "Welcome to Text Snake";
	string pressSpaceString = "Press Space Bar to continue";
	string pressSString = "Press (s) to go to the high scores";

	const int yPos = game.windowSize.height / 2 - 1;

	const int startXPos = game.windowSize.width / 2 - startString.length() / 2;
	const int pressSpaceXPos = game.windowSize.width / 2 - pressSpaceString.length() / 2;
	const int pressSXPos = game.windowSize.width / 2 - pressSString.length() / 2;

	DrawString(startXPos, yPos, startString);
	DrawString(pressSpaceXPos, yPos + 1, pressSpaceString);
	DrawString(pressSXPos, yPos + 2, pressSString);
}

void DrawGameOverScreen(const Game& game) {
	string gameOverString = "Game Over!";
	string pressSpaceString = "Press Space Bar to continue";
	string namePromptString = "Please Enter you name: ";

	const int yPos = game.windowSize.height / 2 - 3;

	const int gameOverXPos = game.windowSize.width / 2 - gameOverString.length()/ 2;
	const int pressSpaceXPos = game.windowSize.width / 2 - pressSpaceString.length() / 2;
	const int namePromptXPos = game.windowSize.width / 2 - namePromptString.length() / 2;

	DrawString(gameOverXPos, yPos, gameOverString);
	DrawString(pressSpaceXPos, yPos + 1, pressSpaceString);
	DrawString(namePromptXPos, yPos + 3, namePromptString);

	for (int i = 0; i < MAX_LENGTH_OF_NAME; i++) {
		if (i == game.gameOverHPositionCursor) {
			attron(WA_UNDERLINE);
		}

		DrawCharacter(game.windowSize.width / 2 - MAX_LENGTH_OF_NAME / 2 + i, yPos + 5, game.playerName[i]);

		if (i == game.gameOverHPositionCursor) {
			attroff(WA_UNDERLINE);
		}

	}
}

void ResetGameOverPositionCursor(Game& game) {
	game.gameOverHPositionCursor = 0;
	for (int i = 0; i < MAX_LENGTH_OF_NAME; i++) {
		game.playerName[i] = 'A';
		game.gameOverVPositionCursor[i] = 0; // Be at 'A'
	}
}

bool ScoreCompare(const Score& score1, const Score& score2) {
	return score1.score > score2.score;
}

void AddHighScore(HighScoreTable& table, int score, const std::string& name) {
	Score highScore;
	highScore.score = score;
	highScore.name = name;

	bool alreadyPlay = false;
	for (auto& player : table.scores) {
		if (player.name == name) {
			if (score > player.score) {
				player.score = score;
			}
			alreadyPlay = true;
		}
	}

	if (!alreadyPlay) {
		table.scores.push_back(highScore);
	}
	
	sort(table.scores.begin(), table.scores.end(), ScoreCompare);

	SaveHighScore(table);
}

void SaveHighScore(const HighScoreTable& table) {
	ofstream outFile;
	outFile.open(filename);

	if (outFile.is_open()) {
		for (int i = 0; i < table.scores.size() && i < MAX_HIGH_SCORES; i++) {
			outFile << table.scores[i].name << " " << table.scores[i].score << endl;
		}
	}
}

void LoadHighScore(HighScoreTable& table) {
	ifstream inFile;

	inFile.open(filename);

	string name;
	int scoreVal;

	Score score;

	if (inFile.is_open()) {
		while (!inFile.eof()) {
			inFile >> ws;
			if (inFile.eof()) {
				break;
			}

			inFile >> name >> scoreVal;
			score.name = name;
			score.score = scoreVal;

			table.scores.push_back(score);
		}

		inFile.close();
	}
}

void DrawHighScoreTable(const Game& game, const HighScoreTable& table) {
	string title = "High Scores";
	int titleXPos = game.windowSize.width / 2 - title.length() / 2;
	int yPos = 5;
	int yPadding = 2;

	attron(A_UNDERLINE);
	DrawString(titleXPos, yPos, title);
	attroff(A_UNDERLINE);

	for (int i = 0; i < table.scores.size() && i < MAX_HIGH_SCORES; i++) {
		Score score = table.scores[i];

		mvprintw(yPos + (i + 1) * yPadding, titleXPos - MAX_LENGTH_OF_NAME/2, "%s        %i", score.name.c_str(), score.score);
	}
}
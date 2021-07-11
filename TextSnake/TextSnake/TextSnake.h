#ifndef  _TEXTSNAKE_H_
#define  _TEXTSNAKE_H_

#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>

const char APPLE_SPRITE = 'o';
const char SNAKE_SPRITE[] = { '#', ' '};

enum {
	MAX_NUMBER_OF_LIVE = 3,
	MAX_NUMBER_OF_APPLE = 4,
	APPLE_RESPAWN_TIME = 5,
	APPLE_POINT = 10,
	PLAYER_SPEED = 1,
	PLAYER_START_LENGTH = 2,
	FPS = 24,
	NOT_IN_PLAY = -1,
	PLAYER_WAIT_TIME = 48,
	PLAYER_ANIMATION_LENGTH = 2,
	MAX_LENGTH_OF_NAME = 4
};

enum GameState {
	GS_INTRO = 0,
	GS_HIGH_SCORES,
	GS_PLAY,
	GS_PLAYER_DEAD,
	GS_WAIT,
	GS_GAME_OVER
};

enum PlayerDirection {
	PS_UP = 0,
	PS_RIGHT,
	PS_DOWN,
	PS_LEFT,
};
struct Position {
	int x;
	int y;
};

struct Size {
	int width;
	int height;
};

struct Apple {
	Position position;
	int point;
};

struct AppleSpawner {
	Apple apples[MAX_NUMBER_OF_APPLE];
	int appleInPlay;
	int spawnTimer;
};

struct SnakePart {
	Position position;

	// Constructor
	SnakePart(int xPos, int yPos) {
		this->position.x = xPos;
		this->position.y = yPos;
	}

	SnakePart(Position pos) {
		this->position = pos;
	}
};

struct Player {
	int length;
	int live;
	int score;

	PlayerDirection direction;
	int movementTime;
	int animation;
	std::vector<SnakePart> body; // head is on index zero
	
};

struct Score {
	int score;
	std::string name;
};

struct HighScoreTable {
	std::vector<Score> scores;
};

struct Game {
	Size windowSize;
	GameState currentState;
	int level;
	int waitTimer;
	clock_t gameTimer;

	int gameOverHPositionCursor;
	char playerName[MAX_LENGTH_OF_NAME + 1];
	int gameOverVPositionCursor[MAX_LENGTH_OF_NAME + 1];
};
#endif

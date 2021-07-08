#ifndef  _TEXTSNAKE_H_
#define  _TEXTSNAKE_H_

#include <vector>
#include <string>
#include <ctime>

const char APPLE_SPRITE = 'O';
const char SNAKE_SPRITE = '#';
const char SNAKE_HEAD_SPRITE = 'C';

enum {
	MAX_NUMBER_OF_LIVE = 3,
	MAX_NUMBER_OF_APPLE = 3,
	APPLE_RESPAWN_TIME = 5,
	PLAYER_SPEED = 1,
	FPS = 24,
};

enum GameState {
	GS_INTRO = 0,
	GS_HIGH_SCORES,
	GS_PLAY,
	GS_PLAYER_DEAD,
	GS_WAIT,
	GS_GAME_OVER
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
	char sprite;
};

struct AppleSpawner {
	Apple apples[MAX_NUMBER_OF_APPLE];
	clock_t spawnTimer;
};

struct SnakePart {
	Position position;

	// Constructor
	SnakePart(int xPos, int yPos) {
		this->position.x = xPos;
		this->position.y = yPos;
	}
};

struct Player {
	int length;
	int live;
	int score;
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
};
#endif

#ifndef TEXTINVADER_H_
#define TEXTINVADER_H_

#include <vector>
#include <string>

const char* PLAYER_SPRITE[] = { " =A= ", "=====" };
const char* PLAYER_EXPLOSION_SPRITE[] = { ",~^,'", "=====", "'+-`.", "=====" };
const char PLAYER_MISSILE_SPRITE = '|';
const char* SHIELD_SPRITE[] = { "/IIIII\\", "IIIIIII", "I/   \\I"};
const char* ALIEN30_SPRITE[] = { "/oo\\", "<  >", "/oo\\", "/\"\"\\" };
const char* ALIEN20_SPRITE[] = { " >< ", "|\\/|", "|><|", "/  \\" };
const char* ALIEN10_SPRITE[] = { "/--\\", "/  \\", "/--\\", "<  >" };
const char* ALIEN_EXPLOSION[] = { "\\||/", "/||\\" };
const char* ALIEN_BOMB_SPRITE = "\\|/-";

enum {
	SHEILD_SPRITE_HEIGHT = 3,
	SHEILD_SPRITE_WIDTH = 7,
	NUM_ALIEN_ROWS = 5,
	NUM_ALIEN_COLUMNS = 11,
	MAX_NUMBER_OF_ALIEN_BOMBS = 5,
	MAX_NUMBER_OF_LIVES = 3,
	PLAYER_SPRITE_WIDTH = 5,
	PLAYER_SPRITE_HEIGHT = 2,
	NOT_IN_PLAY = -1,
	PLAYER_MOVEMENT_AMOUNT = 3,
	PLAYER_MISSILE_SPEED = 1,
	FPS = 24,
	NUM_SHIELDS = 4,
	ALIEN_SPRITE_WIDTH = 4,
	ALIEN_SPRITE_HEIGHT = 2,
	ALIEN_PADDING = 1,
	ALIEN_EXPLOSION_TIME = 4,
	ALIEN_BOMB_SPEED = 1,
	WAIT_TIME = 10,
	NUM_LEVELS = 10
};

enum AlienState {
	AS_ALIVE = 0,
	AS_DEAD,
	AS_EXPLODING
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

struct Player {
	Position position;
	Position missile;
	Size spriteSize;
	int animation;
	int live; //max 3
	int score;
};


struct Shield {
	Position position;
	char* sprite[SHEILD_SPRITE_HEIGHT];
};

struct AlienBomb {
	Position position;
	int animation;
};

struct AlienSwarm {
	Position position;
	AlienState aliens[NUM_ALIEN_ROWS][NUM_ALIEN_COLUMNS];
	AlienBomb bombs[MAX_NUMBER_OF_ALIEN_BOMBS];
	Size spriteSize;
	int animation;
	int direction; // 1 for right, -1 for left;
	int numberOfBombsInPlay;
	int movementTime; //this is going to capture how fast the aliens should be
	int explosionTimer; //this going to capture how long an alien explodes
	int numAliensLeft; //this is to capture when to go to the next level
	int line; //This is to capture when aleins win
};

struct AlienUFO {
	Position position;
	Size size;
	int points;
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
};

#endif
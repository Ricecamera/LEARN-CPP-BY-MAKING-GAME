#ifndef __BATTLESHIP_H__
#define __BATTLESHIP_H__

enum
{
    AIRCRAFT_CARRIER_SIZE = 5,
    BATTLESHIP_SIZE = 4,
    CRUISER_SIZE = 3,
    DESTROYER_SIZE = 3,
    SUBMARINE_SIZE = 2,

    BOARD_SIZE = 10,
    NUM_SHIPS = 5,
    PLAYER_NAME_SIZE = 8, //Player1, Player2
    MAX_SHIP_SIZE = AIRCRAFT_CARRIER_SIZE
};
enum ShipType
{
    ST_NONE = 0,
    ST_AIRCRAFT_CARRIER,
    ST_BATTLESHIP,
    ST_CRUISER,
    ST_DESTROYER,
    ST_SUBMARINE
};

enum ShipOrientationType
{
    SO_HORIZONTAL = 0,
    SO_VERTICAL,
};

struct ShipPositionType
{
    int row;
    int col;
};

struct Ship
{
    ShipType shipType;
    int shipSize;
    ShipOrientationType orientation;
    ShipPositionType position;
};

enum GuessType
{
    GT_NONE = 0,
    GT_MISSED,
    GT_HIT
};

struct ShipPartType
{
    ShipType shipType;
    bool isHit;
};

enum PlayerType
{
    PT_HUMAN = 0,
    PT_AI
};
struct Player
{
    PlayerType playerType;
    char playerName[PLAYER_NAME_SIZE];
    Ship ships[NUM_SHIPS];
    GuessType guessBoard[BOARD_SIZE][BOARD_SIZE];
    ShipPartType shipBoard[BOARD_SIZE][BOARD_SIZE];
};

void InitializePlayer(Player& player, const char* playerName);
void InitializeShip(Ship& ship, int shipSize, ShipType shipType);

void PlayGame(Player& player1, Player& player2);
bool WantToPlayAgain();

void SetupBoards(Player& player);
void ClearBoards(Player& player);
void DrawBoards(const Player& player);

void DrawSeparatorLine();
void DrawColumnsRow();
void DrawShipBoardRow(const Player& player, int row);
void DrawGuessBoardRow(const Player& player, int row);
char GetGuessRepresentationAt(const Player& player, int row, int col);
char GetShipRepresentationAt(const Player& player, int row, int col);

const char* GetShipNameForShipType(ShipType shipType);
ShipPositionType GetBoardPosition();
ShipOrientationType GetShipOrientation();

bool IsValidPlacement(const Player& player, const Ship& currentShip, const ShipPositionType& shipPosition, ShipOrientationType orientation);
void PlaceShipOnBoard(Player& player, Ship& currentShip, const ShipPositionType& shipPosition, ShipOrientationType orientation);
ShipType UpdateBoards(ShipPositionType guess, Player& currentPlayer, Player& otherPlayer);
bool IsGameOver(const Player& player1, const Player& player2);
bool AreAllShipsSunk(const Player& player);
bool IsSunk(const Player& player, const Ship& ship);

void SwitchPlayers(Player** currentPlayer, Player** otherPlayer);
void DisplayWinner(const Player& player1, const Player& player2);

PlayerType GetPlayer2Type();
ShipPositionType GetAIGuess(const Player& aiPlayer);
void SetupAIBoards(Player& player);

#endif

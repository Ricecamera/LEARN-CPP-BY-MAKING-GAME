#include <iostream>
#include <cstring>
#include <cctype>

#include "Utils.h"

using namespace std;

enum GameResult {
	Tie,
	PlayerOneWin,
	PlayerTwoWin
};

const int IGNORE_CHARS = 256;
const char* INPUT_ERROR_STRING = "Input error! Please try again.";

void PlayGame(int starter);
bool WantToPlayAgain(int& lastStarter);

int GetPlayerInput(int playerTurn, char* validPos, int dynamicSize);

void DrawBoard(char* Board);
void UpdateBoard(char* Board, int playerTurn, int placePosition, char* validPos, int &dynamicSize);

bool IsGameOver(char* Board, int& playerTurn, GameResult& result);

int main() {
	int starter = 1;
	do
	{
		PlayGame(starter);

	} while (WantToPlayAgain(starter));


	return 0;
}

void PlayGame(int stater) {
	const int BOARD_SIZE = 9;
	GameResult result;
	int playerTurn = stater;

	char* Board = new char[BOARD_SIZE]; // Create new empty board
	for (int i = 0; i < BOARD_SIZE; ++i) {
		Board[i] = ' ';
	}
	DrawBoard(Board); // Display empty board

	int dynamicSize;
	char* validPos = CreateDynamicArray(BOARD_SIZE, dynamicSize);
	for (int i = 0; i < dynamicSize; ++i) {
		validPos[i] = i + 1 + '0';
	}

	int position;
	do {
		position = GetPlayerInput(playerTurn, validPos, dynamicSize);
		UpdateBoard(Board, playerTurn, position, validPos, dynamicSize); 
		DrawBoard(Board);
	} while (!IsGameOver(Board, playerTurn, result));

	// Show result
	switch (result)
	{
	case Tie:
		cout << "Cat Game!" << endl;
		break;

	case PlayerOneWin:
		cout << "Player1 wins!" << endl;
		break;

	case PlayerTwoWin:
		cout << "Player2 wins!" << endl;
		break;
	}
	delete[] Board;
	DeleteDynamicArray(validPos, dynamicSize);
}

int GetPlayerInput(int playerTurn, char* validPos, int dynamicSize) {
	cout << "-------Player" << playerTurn << "'s turn-------\n";
	char input = GetCharacter("Pls, select your position (1-9): ", INPUT_ERROR_STRING, validPos, dynamicSize);
	int position = (input - '0') - 1;
	DeleteElementByValue(validPos, input, dynamicSize);
	return position;
}

void UpdateBoard(char* Board, int playerTurn, int placePosition, char* validPos, int& dynamicSize) {
	if (playerTurn == 1) {
		Board[placePosition] = 'O';
	}
	else if (playerTurn == 2) {
		Board[placePosition] = 'X';
	}
	DeleteElementByValue(validPos, placePosition, dynamicSize);
}

bool IsGameOver(char* Board, int& playerTurn, GameResult& result) {
	int lastPlayerturn = playerTurn;
	playerTurn = 1 + playerTurn % 2; // Change player turn

	char checking;
	if (lastPlayerturn == 1) {
		checking = 'O';
	}
	else {
		checking = 'X';
	}
	// Horizontal
	for (int i = 0; i < 9; i = i + 3) {
		bool found = true;
		for (int j = i; j < i + 3; ++j) {
			if (Board[j] != checking) {
				found = false;
				break;
			}
		}
		if (found) {
			result = (lastPlayerturn == 1) ? PlayerOneWin : PlayerTwoWin;
			return true;
		}
	}
	// Vertical
	for (int i = 0; i < 3; ++i) {
		bool found = true;
		for (int j = i; j < 9; j = j + 3) {
			if (Board[j] != checking) {
				found = false;
				break;
			}
		}
		if (found) {
			result = (lastPlayerturn == 1) ? PlayerOneWin : PlayerTwoWin;
			return true;
		}
	}

	// Right diagonal
	if (Board[0] == checking && Board[4] == checking && Board[8] == checking) {
		result = (lastPlayerturn == 1) ? PlayerOneWin : PlayerTwoWin;
		return true;
	}

	// Left diagonal
	if (Board[2] == checking && Board[4] == checking && Board[6] == checking) {
		result = (lastPlayerturn == 1) ? PlayerOneWin : PlayerTwoWin;
		return true;
	}

	// If There are some blank spaces, game is not finish yet.
	for (int i = 0; i < 9; ++i) {
		if (Board[i] == ' ') {
			return false;
		}
	}

	result = Tie;
	return true;
}

bool WantToPlayAgain(int& lastStarter) {
	const char validInputs[] = { 'y', 'n' };

	char response = GetCharacter("Would you like to play again? (y/n) ", INPUT_ERROR_STRING, validInputs, 2);
	lastStarter = 1 + (lastStarter % 2);
	return response == 'y';
}

void DrawBoard(char* Board) {
	ClearScreen();

	cout << "+-+-+-+" << endl;
	cout << "|"<< Board[0] << "|" << Board[1] << "|" << Board[2] << "|" << endl;
	cout << "+-+-+-+" << endl;
	cout << "|" << Board[3] << "|" << Board[4] << "|" << Board[5] << "|" << endl;
	cout << "+-+-+-+" << endl;
	cout << "|" << Board[6] << "|" << Board[7] << "|" << Board[8] << "|" << endl;
	cout << "+-+-+-+" << endl;
}

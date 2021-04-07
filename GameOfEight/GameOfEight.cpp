#include <iostream>

void PlayGame();
bool WantToPlayAgain();
int GetNumber(int lastPlayerChoose);

const int IGNORE_CHAR = 256;
int main()
{
    std::cout << "----------This is Game of Eight----------" << '\n';
    do {
        PlayGame();
    } while (WantToPlayAgain());
    
}

void PlayGame() {
    bool playerOneTurn{ true };
    bool isGameOver{ false };
    int playerWin, lastPlayerChoose{ 0 }, choose;
    int number{ 8 };

    do {
        std::cout << '\n';
        std::cout << "Player" << ((playerOneTurn) ? 1 : 2) << " Turn! \n";
        std::cout << "Current number is " << number << ". ";
        choose = GetNumber(lastPlayerChoose);
        number -= choose;
        if (number == 0) {
            playerWin = (playerOneTurn) ? 1 : 2;
            isGameOver = true;
        }
        else if (number < 0) {
            playerWin = (playerOneTurn) ? 2 : 1;
            isGameOver = true;
        }
        playerOneTurn = !playerOneTurn;
        lastPlayerChoose = choose;
    } while (!isGameOver);
    
    std::cout << "Game Over! Player" << playerWin << " Win." << std::endl;
}

bool IsValid(int input, int lastPlayerChoose) {
    return input > 0 && input <= 3 && input != lastPlayerChoose;
}

int GetNumber(int lastPlayerChoose) {
    int input;
    bool failure;

    std::cout << "Choose number from 1 to 3:";
    do {
        failure = false;
        std::cin >> input;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(IGNORE_CHAR, '\n');
            std::cout << "Input error! Please try again." << std::endl;
            failure = true;
        }
        else if (!IsValid(input, lastPlayerChoose)) {
            std::cin.clear();
            std::cin.ignore(IGNORE_CHAR, '\n');
            std::cout << "Nope! Choose another number:";
            failure = true;
        }

    } while (failure);
    return input;
}

bool WantToPlayAgain() {
    char input;
    bool failure;

    do {
        failure = false;

        std::cout << "Would you like to play again? (Y/N):";
        std::cin >> input;

        if (std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(IGNORE_CHAR, '\n');
            std::cout << "Input error! Please try again." << std::endl;
            failure = true;
        }
        else {
            std::cin.ignore(IGNORE_CHAR, '\n');
            input = tolower(input);
        }
    } while (failure);

    return input == 'y';
}
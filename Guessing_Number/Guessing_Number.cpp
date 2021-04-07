// Guessing_Number.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include <time.h>


void PlayGame();
bool WantToPlayAgain();
int GetGuess(int numberOfTries);

const int IGNORE_CHARS = 256;
int main()
{
    std::cout << "----------This is the number gusser game----------\n";
    srand(time(NULL));
    do {
        PlayGame();
    } while (WantToPlayAgain());
    
    return 0;
}

void PlayGame() {
    int iSecret, iGeuss, count;

    srand(time(NULL));

    iSecret = rand() % 100 + 1;
    count = 7;
    do {
        iGeuss = GetGuess(count);
        if (iSecret == iGeuss) {
            std::cout << "You are won!\n"; 
            std::cout << "The secret number is " << iSecret << std::endl;
            return;
        }
        else if (iSecret > iGeuss) {
            std::cout << "Nope, It is higher." << '\n';
        }
        else {
            std::cout << "Nope, It is lower." << '\n';
        }
        --count;
    } while (count > 0);
    std::cout << "You are lost!\n";
    std::cout << "The secret number is " << iSecret << std::endl;
}

int GetGuess(int numberOfTries)
{
    int guess;
    bool failure;

    do
    {
        failure = false;

        std::cout << "Please enter your guess (number of guesses left: " << numberOfTries << "): ";
        std::cin >> guess;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(IGNORE_CHARS, '\n');
            std::cout << "Input error! Please try again." << std::endl;
            failure = true;
        }

    } while (failure);

    return guess;
}

bool WantToPlayAgain()
{
    char input;
    bool failure;
    do {
        failure = false;

        std::cout << "Would you like to play again? (y/n):";
        std::cin >> input;

        if(std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(IGNORE_CHARS, '\n');
            std::cout << "Input Error! Please try again." << std::endl;
            failure = true;
        }
        else
        {
            std::cin.ignore(IGNORE_CHARS, '\n');
            input = tolower(input);
        }

    } while (failure);

    return input == 'y';
}
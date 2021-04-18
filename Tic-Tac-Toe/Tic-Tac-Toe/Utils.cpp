/*
 * Utils.cpp
 *
 *  Created on: Sep 22, 2016
 *      Author: serge
 */


#include "Utils.h"
#include <iostream>
#include <cctype>

using namespace std;

char GetCharacter(const char* prompt, const char* error)
{

	const int IGNORE_CHARS = 256;

	char input;
	bool failure;

	do
	{
		failure = false;

		cout << prompt;
		cin >> input;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(IGNORE_CHARS, '\n');
			cout << error << endl;
			failure = true;
		}
		else
		{
			cin.ignore(IGNORE_CHARS, '\n');

			if (isalpha(input))
			{
				input = tolower(input);
			}
			else
			{
				cout << error << endl;
				failure = true;
			}
		}

	} while (failure);

	return input;
}

char GetCharacter(const char* prompt, const char* error, const char validInput[], int validInputLength)
{

	const int IGNORE_CHARS = 256;

	char input;
	bool failure;

	do
	{
		failure = false;

		cout << prompt;
		cin >> input;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(IGNORE_CHARS, '\n');
			cout << error << endl;
			failure = true;
		}
		else
		{
			cin.ignore(IGNORE_CHARS, '\n');

			for (int i = 0; i < validInputLength; i++)
			{
				if (input == validInput[i])
				{
					return input;
				}
			}

			cout << error << endl;
			failure = true;

		}

	} while (failure);

	return input;
}

void ClearScreen()
{
	system("cls"); //for windows only!
	//system("clear");
}


void WaitForKeyPress()
{
	system("pause"); //Windows only!

	//system("read -n 1 -s -p \"Press any key to continue...\";echo");
}


char* CreateDynamicArray(int capacity, int& size) {
	char* dynamicArray = new char[capacity]();
	size = capacity;
	return dynamicArray;
}

void DeleteDynamicArray(char* dynamicArray, int& size) {
	delete[] dynamicArray;
	size = 0;
}

void InsertElement(char* dynamicArray, int element, int& size, int capacity) {
	if (size + 1 > capacity) {
		ResizeDynamicArray(dynamicArray, 2 * capacity);
	}
	dynamicArray[size++] = element;
}

void DeleteElementByIndex(char* dynamicArray, int elementIndex, int& size) {
	for (int i = elementIndex; i + 1 < size; ++i) {
		dynamicArray[i] = dynamicArray[i+1];
	}
	size--;
}

void DeleteElementByValue(char* dynamicArray, char elementValue, int& size) {
	int idx = 0;
	for (; idx < size; ++idx) {
		if (elementValue == dynamicArray[idx]) {
			break;
		}
	}

	if (idx == size) {
		cout << "index out of bound" << endl;
		return;
	}
	DeleteElementByIndex(dynamicArray, idx, size);
}

void ResizeDynamicArray(char* dynamicArray, int newCapacity) {
	char* newArray = new char[newCapacity]();
	for (int i = 0; i < newCapacity / 2; ++i) {
		newArray[i] = dynamicArray[i];
	}
	delete[] dynamicArray;
	dynamicArray = newArray;
}


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


char GetCharacter(const char* prompt, const char* error, CharacterCaseType charCase)
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
				if (charCase == CC_UPPER_CASE) {
					input = toupper(input);
				}
				else if (charCase == CC_LOWER_CASE) {
					input = tolower(input);
				}
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


DynamicArray* CreateDynamicArray(int capacity) {
	DynamicArray* darray = new DynamicArray; 
	darray->dynamicArray = new char[capacity]();
	darray->capacity = capacity;
	darray->size = 0;
	return darray;
}

void DeleteDynamicArray(DynamicArray* darray) {
	delete[] darray->dynamicArray;
	delete darray;
	darray = nullptr;
}

void InsertElement(DynamicArray* darray, char element) {
	int size = darray->size;
	if (size + 1 > darray->capacity) {
		ResizeDynamicArray(darray, 2 * darray->capacity);
	}
	darray->dynamicArray[size++] = element;
	darray->size = size;
}

void DeleteElementByIndex(DynamicArray* darray, int elementIndex) {
	if (elementIndex >= darray->size) {
		cout << "Index out of bound" << endl;
		return;
	}
	for (int i = elementIndex; i + 1 < darray->size; ++i) {
		darray->dynamicArray[i] = darray->dynamicArray[i+1];
	}
	darray->size--;
}

void DeleteElementByValue(DynamicArray* darray, char elementValue) {
	int idx = 0;
	for (; idx < darray->size; ++idx) {
		if (elementValue == darray->dynamicArray[idx]) {
			break;
		}
	}

	if (idx == darray->size) {
		cout << "Not found" << endl;
		return;
	}
	DeleteElementByIndex(darray, idx);
}

void ResizeDynamicArray(DynamicArray* darray, int newCapacity) {
	char* newArray = new char[newCapacity]();
	for (int i = 0; i < newCapacity / 2; ++i) {
		newArray[i] = darray->dynamicArray[i];
	}
	delete[] darray->dynamicArray;
	darray->dynamicArray = newArray;
	darray->capacity = newCapacity;
}


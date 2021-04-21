#ifndef __UTILS_H__
#define __UTILS_H__

enum CharacterCaseType {
	CC_UPPER_CASE = 0,
	CC_LOWER_CASE,
	CC_EITHER
};

struct DynamicArray {
	char* dynamicArray;
	int capacity;
	int size;
};

char GetCharacter(const char* prompt, const char* error, CharacterCaseType charCase);
char GetCharacter(const char* prompt, const char* error, const char validInput[], int validInputLength);

void ClearScreen();

void WaitForKeyPress();

DynamicArray* CreateDynamicArray(int capacity);
void DeleteDynamicArray(DynamicArray* darray);
void InsertElement(DynamicArray* darray, char element);
void DeleteElementByIndex(DynamicArray* darray, int elementIndex);
void DeleteElementByValue(DynamicArray* darray, char elementValue);
void ResizeDynamicArray(DynamicArray* darray, int newCapacity);


#endif

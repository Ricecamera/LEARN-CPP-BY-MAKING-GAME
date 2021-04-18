#ifndef __UTILS_H__
#define __UTILS_H__

char GetCharacter(const char* prompt, const char* error);
char GetCharacter(const char* prompt, const char* error, const char validInput[], int validInputLength);

void ClearScreen();

void WaitForKeyPress();

char* CreateDynamicArray(int capacity, int& size);
void DeleteDynamicArray(char* dynamicArray, int& size);
void InsertElement(char* dynamicArray, int element, int& size, int capacity);
void DeleteElementByIndex(char* dynamicArray, int elementIndex, int& size);
void DeleteElementByValue(char* dynamicArray, char elementValue, int& size);
void ResizeDynamicArray(char* dynamicArray, int newCapacity);


#endif

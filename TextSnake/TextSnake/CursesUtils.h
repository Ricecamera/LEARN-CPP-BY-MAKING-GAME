/*
 * CursesUtils.h
 *
 *  Created on: Oct 7, 2016
 *      Author: serge
 */

#ifndef CURSESUTILS_H_
#define CURSESUTILS_H_

#include "curses.h"
#include <string>

enum ArrowKeys
{
	UP = KEY_UP,
	DOWN = KEY_DOWN,
	LEFT = KEY_LEFT,
	RIGHT = KEY_RIGHT
};

void InitializeCurses(bool nodelay);
void ShutdownCurses();
void ClearScreen();
void RefreshScreen();
int ScreenWidth();
int ScreenHeight();
int GetChar();
void DrawCharacter(int xPos, int yPos, char aCharacter);
void MoveCursor(int xPos, int yPos);
void DrawSprite(int xPos, int yPos, const char* sprite[], int spriteHeight, int offset = 0);
void DrawString(int xPos, int yPos, const std::string& string);

#endif /* CURSESUTILS_H_ */

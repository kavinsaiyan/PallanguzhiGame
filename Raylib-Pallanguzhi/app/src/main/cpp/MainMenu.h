#include "raylib.h"

#ifndef MAINMENU_H
#define MAINMENU_H

//need to draw BG first
//need to draw two buttons, play and exit
//have a update method, that checks for the button click on them

void InitializeMainMenu(Sound* clickSound);

void DeInitializeMainMenu();

void DrawMainMenu(Texture2D* bgTexture);

bool IsPlayButtonClicked(Vector2 mousePosition);

bool IsExitButtonClicked(Vector2 mousePosition);

void InitializeMainMenuForDrawing();
#endif

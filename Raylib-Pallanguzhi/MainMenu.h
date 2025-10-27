#include "raylib.h"

#ifndef MAINMENU_H
#define MAINMENU_H

typedef struct
{
    Rectangle playButton;
    Rectangle exitButton;
} MainMenu;

//need to draw BG first
//need to draw two buttons, play and exit
//have a update method, that checks for the button click on them

//what do we need, the Textures for the BG and the buttons
//coordinates for where to draw the buttons
void InitializeMainMenu(MainMenu* mainMenu);

void DrawMainMenu(MainMenu* mainMenu);

bool IsPlayButtonClicked(MainMenu* mainMenu);

bool IsExitButtonClicked(MainMenu* mainMenu);

void DeInitializeMainMenu(MainMenu* mainMenu);
#endif

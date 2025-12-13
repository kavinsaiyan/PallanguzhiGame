#include "raylib.h"

#ifndef MAINMENU_H
#define MAINMENU_H

typedef struct
{
    Rectangle playButton;
    Color playButtonColor;
    Rectangle exitButton;
    Color exitButtonColor;
    Sound* clickSound;
} MainMenuData;

//need to draw BG first
//need to draw two buttons, play and exit
//have a update method, that checks for the button click on them

//what do we need, the Textures for the BG and the buttons
//coordinates for where to draw the buttons
void InitializeMainMenu(MainMenuData* mainMenu,Sound* clickSound);

void DrawMainMenu(MainMenuData* mainMenu, Texture2D* bgTexture);

bool IsPlayButtonClicked(MainMenuData* mainMenu,Vector2 mousePosition);

bool IsExitButtonClicked(MainMenuData* mainMenu,Vector2 mousePosition);

void InitializeMainMenuForDrawing();
#endif

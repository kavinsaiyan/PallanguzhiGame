#include "MainMenu.h"
#include "raylib.h"
#include "Button.h"
#include "Translation.h"

static Button playButton;
static Button exitButton;

void InitializeMainMenu(MainMenuData* mainMenu, Sound* clickSound)
{
    TextID play = Play;
    InitButton(&playButton,340,300,140,50,play,clickSound);

    TextID exit = Exit;
    InitButton(&exitButton,340,380,140,50,exit,clickSound);

    mainMenu->clickSound = clickSound;
}

void DrawMainMenu(MainMenuData* mainMenu, Texture2D* bgTexture)
{
    ClearBackground(RAYWHITE);
    DrawTexture(*bgTexture,0,100,WHITE);

    bool drawButtonText = true;
    DrawButton(&playButton,drawButtonText);
    DrawButton(&exitButton,drawButtonText);
}

bool IsPlayButtonClicked(MainMenuData* mainMenu,Vector2 mousePosition)
{
    if(IsButtonClicked(&playButton,mousePosition))
        return true;
    return false;
}

bool IsExitButtonClicked(MainMenuData* mainMenu,Vector2 mousePosition)
{
    if(IsButtonClicked(&exitButton,mousePosition))
        return true;
    return false;
}

void DeInitializeMainMenu(MainMenuData* mainMenu)
{
}

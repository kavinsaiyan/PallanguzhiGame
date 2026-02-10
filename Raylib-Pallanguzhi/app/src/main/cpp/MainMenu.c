#include "MainMenu.h"
#include "raylib.h"
#include "Button.h"
#include "Translation.h"

static Button playButton;
static Button onlinePlayButton;
static Button exitButton;

void InitializeMainMenu(Sound* clickSound)
{
    TextID play = Play;
    InitButton(&playButton,400,220,140,50,play,clickSound);

    TextID onlinePlay = OnlinePlay;
    InitButton(&onlinePlayButton,400,300,140,50,onlinePlay,clickSound);

    TextID exit = Exit;
    InitButton(&exitButton,400,380,140,50,exit,clickSound);
}

void InitializeMainMenuForDrawing()
{
   ResizeButtonForText(&playButton);
   ResizeButtonForText(&exitButton);
}

void DrawMainMenu(Texture2D* bgTexture)
{
    ClearBackground(RAYWHITE);
    DrawTexture(*bgTexture,0,100,WHITE);

    bool drawButtonText = true;
    DrawButton(&playButton,drawButtonText);
    DrawButton(&onlinePlayButton,drawButtonText);
    DrawButton(&exitButton,drawButtonText);
}

bool IsPlayButtonClicked(Vector2 mousePosition)
{
    if(IsButtonClicked(&playButton,mousePosition))
        return true;
    return false;
}

bool IsExitButtonClicked(Vector2 mousePosition)
{
    if(IsButtonClicked(&exitButton,mousePosition))
        return true;
    return false;
}

bool IsOnlinePlayButtonClicked(Vector2 mousePosition)
{
    if(IsButtonClicked(&onlinePlayButton,mousePosition))
        return true;
    return false;
}

void DeInitializeMainMenu()
{
}

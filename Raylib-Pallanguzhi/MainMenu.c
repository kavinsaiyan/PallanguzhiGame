#include "MainMenu.h"
#include "raylib.h"

void InitializeMainMenu(MainMenuData* mainMenu)
{
    mainMenu->playButton.x=340;
    mainMenu->playButton.y=300;
    mainMenu->playButton.width = 100;
    mainMenu->playButton.height = 50;
    mainMenu->playButtonColor = LIGHTGRAY;

    mainMenu->exitButton.x=340;
    mainMenu->exitButton.y=380;
    mainMenu->exitButton.width = 100;
    mainMenu->exitButton.height = 50;
    mainMenu->exitButtonColor = LIGHTGRAY;
}

void DrawMainMenu(MainMenuData* mainMenu)
{
    ClearBackground(RAYWHITE);
    DrawRectangleRec(mainMenu->playButton, mainMenu->playButtonColor);
    DrawText("Play",mainMenu->playButton.x+16,mainMenu->playButton.y+10,32,BLACK);

    DrawRectangleRec(mainMenu->exitButton, mainMenu->exitButtonColor);
    DrawText("Exit",mainMenu->exitButton.x+16,mainMenu->exitButton.y+10,32,BLACK);
}

bool IsPlayButtonClicked(MainMenuData* mainMenu)
{
    mainMenu->playButtonColor = LIGHTGRAY;
    if(CheckCollisionPointRec(GetMousePosition(), mainMenu->playButton))
    {
        mainMenu->playButtonColor = GRAY;
        if(IsMouseButtonPressed(0))
            return true;
    }
    return false;
}

bool IsExitButtonClicked(MainMenuData* mainMenu)
{
    mainMenu->exitButtonColor = LIGHTGRAY;
    if(CheckCollisionPointRec(GetMousePosition(), mainMenu->exitButton))
    {
        mainMenu->exitButtonColor = GRAY;
        if(IsMouseButtonPressed(0))
            return true;
    }
    return false;
}

void DeInitializeMainMenu(MainMenuData* mainMenu)
{
}

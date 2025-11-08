#include "MainMenu.h"
#include "raylib.h"

void InitializeMainMenu(MainMenuData* mainMenu, Sound* clickSound)
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

    mainMenu->clickSound = clickSound;
}

void DrawMainMenu(MainMenuData* mainMenu, Texture2D* bgTexture)
{
    ClearBackground(RAYWHITE);
    DrawTexture(*bgTexture,0,100,WHITE);
    DrawRectangleRec(mainMenu->playButton, mainMenu->playButtonColor);
    DrawText("Play",mainMenu->playButton.x+16,mainMenu->playButton.y+10,32,BLACK);

    DrawRectangleRec(mainMenu->exitButton, mainMenu->exitButtonColor);
    DrawText("Exit",mainMenu->exitButton.x+16,mainMenu->exitButton.y+10,32,BLACK);
}

bool IsPlayButtonClicked(MainMenuData* mainMenu,Vector2 mousePosition)
{
    mainMenu->playButtonColor = LIGHTGRAY;
    if(CheckCollisionPointRec(mousePosition, mainMenu->playButton))
    {
        mainMenu->playButtonColor = GRAY;
        if(IsMouseButtonPressed(0))
        {
            PlaySound(*(mainMenu->clickSound));
            return true;
        }
    }
    return false;
}

bool IsExitButtonClicked(MainMenuData* mainMenu,Vector2 mousePosition)
{
    mainMenu->exitButtonColor = LIGHTGRAY;
    if(CheckCollisionPointRec(mousePosition, mainMenu->exitButton))
    {
        mainMenu->exitButtonColor = GRAY;
        if(IsMouseButtonPressed(0))
        {
            PlaySound(*(mainMenu->clickSound));
            return true;
        }
    }
    return false;
}

void DeInitializeMainMenu(MainMenuData* mainMenu)
{
}

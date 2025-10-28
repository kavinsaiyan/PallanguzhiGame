#include "raylib.h"
#include "EndScreen.h"

void InitializeEndScreen(EndScreen* endScreen)
{
    endScreen->retryButton.x=320;
    endScreen->retryButton.y=300;
    endScreen->retryButton.width = 120;
    endScreen->retryButton.height = 50;
    endScreen->retryButtonColor = LIGHTGRAY;
}

void DrawEndScreen(EndScreen* endScreen, int player1Score, int player2Score, int playerWon)
{
    ClearBackground(RAYWHITE);
    DrawText(TextFormat("Player 1 Score : %d",player1Score),300,100,16,BLACK);
    DrawText(TextFormat("Player 2 Score : %d",player2Score),300,120,16,BLACK);
    if(playerWon == 0)
        DrawText("You Won!",300,160,16,BLACK);
    else 
        DrawText("You lost!",300,160,16,BLACK);

    DrawRectangleRec(endScreen->retryButton, endScreen->retryButtonColor);
    DrawText("Retry",endScreen->retryButton.x+16,endScreen->retryButton.y+10,32,BLACK);
}

bool IsRetryButtonClicked(EndScreen* endScreen)
{
    endScreen->retryButtonColor = LIGHTGRAY;
    if(CheckCollisionPointRec(GetMousePosition(), endScreen->retryButton))
    {
        endScreen->retryButtonColor = GRAY;
        if(IsMouseButtonPressed(0))
            return true;
    }

    return false;
}

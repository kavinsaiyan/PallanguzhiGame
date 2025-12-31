#include "raylib.h"
#include "Button.h"
#include "EndScreen.h"
#include "TextRender.h"

static Button retryButton;

void InitializeEndScreen(Sound* clickSound)
{
    TextID retryText = Retry;
    InitButton(&retryButton,360,320,120,50,retryText ,clickSound);
}

void PrepareEndScreenForDrawing()
{
    ResizeButtonForText(&retryButton);
} 

void DrawEndScreen(int player1Score, int player2Score, int playerWon)
{
    ClearBackground(RAYWHITE);
    RenderTextDirect(TextFormat(GetText(YourScore),player1Score),(Vector2){200,100},BLACK);
    RenderTextDirect(TextFormat(GetText(AIScore),player2Score),(Vector2){200,160},BLACK);
    if(playerWon == 0)
        RenderText(YouWon,(Vector2){200,220},BLACK);
    else
        RenderText(YouLost,(Vector2){200,220},BLACK);

    DrawButton(&retryButton,true);
}

bool IsRetryButtonClicked(Vector2 mousePosition)
{
    if(IsButtonClicked(&retryButton, mousePosition))
        return true; 
    return false;
}

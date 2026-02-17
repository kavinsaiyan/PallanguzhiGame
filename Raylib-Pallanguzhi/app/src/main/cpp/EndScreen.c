#include "raylib.h"
#include "Button.h"
#include "EndScreen.h"
#include "TextRender.h"

static Button retryButton;
static Button goToHomeButton;

void InitializeEndScreen(Sound* clickSound)
{
    TextID retryText = Retry;
    InitButton(&retryButton,360,320,120,50,retryText, clickSound);
    TextID goToHomePageText = GoToHomePage;
    InitButton(&goToHomeButton,360,320,120,50,goToHomePageText, clickSound);
}

void PrepareEndScreenForDrawing()
{
    ResizeButtonForText(&retryButton);
} 

void DrawEndScreen(int player1Score, int player2Score, int playerWon,bool drawGoToHomePageButton)
{
    ClearBackground(RAYWHITE);
    RenderTextDirect(TextFormat(GetText(YourScore),player1Score),(Vector2){200,100},BLACK);
    RenderTextDirect(TextFormat(GetText(AIScore),player2Score),(Vector2){200,160},BLACK);
    if(playerWon == 0)
        RenderText(YouWon,(Vector2){200,220},BLACK);
    else
        RenderText(YouLost,(Vector2){200,220},BLACK);

    if(drawGoToHomePageButton)
        DrawButton(&goToHomeButton,true);
    else 
        DrawButton(&retryButton,true);
}

bool IsRetryButtonClicked(Vector2 mousePosition)
{
    if(IsButtonClicked(&retryButton, mousePosition))
        return true; 
    return false;
}

bool IsGoToHomePageButtonClicked(Vector2 mousePosition)
{
    if(IsButtonClicked(&goToHomeButton, mousePosition))
        return true; 
    return false;
}

#include "raylib.h"

#ifndef ENDSCREEN_H
#define ENDSCREEN_H

typedef struct 
{
    Rectangle retryButton;
    Color retryButtonColor;
} EndScreen;

void InitializeEndScreen(EndScreen* endScreen);

void DrawEndScreen(EndScreen* endScreen, int player1Score, int player2Score, int playerWon);

bool IsRetryButtonClicked(EndScreen* endScreen, Vector2 mousePosition);
#endif

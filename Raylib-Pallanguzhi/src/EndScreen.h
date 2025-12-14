#include "raylib.h"

#ifndef ENDSCREEN_H
#define ENDSCREEN_H

void InitializeEndScreen(Sound* clickSound);

void PrepareEndScreenForDrawing();

void DrawEndScreen(int player1Score, int player2Score, int playerWon);

bool IsRetryButtonClicked(Vector2 mousePosition);
#endif

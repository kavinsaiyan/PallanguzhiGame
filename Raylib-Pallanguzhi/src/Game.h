#include "Board.h"
#include "Queue.h"
#include "SlotSelector.h"

#ifndef GAME_H
#define GAME_H

typedef enum
{
    PlayerMove,
    Animating,
    GameOver,
    MainMenu,
    PauseMenu
} GameState;

typedef enum 
{
    Player1Turn = 0,
    Player2Turn = 1,
} PlayerTurn;

typedef struct
{
    GameState state;
    PlayerTurn playerTurn;
    int playerWon; 
} GameStateData;

void InitializeGameStateData(GameStateData* gameStateData);
void StartMove(GameState* gameState,Board* board, Queue* animQ, int currentIndex);
void DrawBoardGame(Board* board,SlotSelector* slotSelector, Texture2D* boardTexture, Texture2D* ballTexture, Texture2D* slotSelectorTexture, PlayerTurn playerTurn);
#endif

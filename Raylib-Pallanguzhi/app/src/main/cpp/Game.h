#include "Board.h"
#include "Queue.h"
#include "SlotSelector.h"
#include "SaveData.h"

#ifndef GAME_H
#define GAME_H

typedef enum
{
    PlayerMove,
    Animating,
    GameOver,
    MainMenu,
    LanguageSelection,
    WaitingForOnlinePlayer
} GameState;

typedef enum 
{
    Player1Turn = 0,
    Player2Turn = 1,
} PlayerTurn;

typedef enum 
{
    SinglePlayer = 0,
    MultiPlayer = 1
} GameMode;

typedef struct
{
    GameState state;
    PlayerTurn playerTurn;
    int playerWon;
    GameMode gameMode;
} GameStateData;

void InitializeGameStateData(GameStateData* gameStateData, SaveData* saveData);
void StartMove(GameState* gameState,Board* board, Queue* animQ, int currentIndex);
void DrawBoardGame(Board* board,SlotSelector* slotSelector, Texture2D* boardTexture, Texture2D* ballTexture, Texture2D* slotSelectorTexture, PlayerTurn playerTurn);
void OpenMainMenu(GameStateData* gamestate);
#endif

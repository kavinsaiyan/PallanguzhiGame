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

void StartMove(Board* board, Queue* animQ, int currentIndex);
#endif

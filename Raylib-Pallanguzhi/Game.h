#ifndef GAME_H
#define GAME_H

typedef enum e_GameState
{
    Player1Turn,
    Player2Turn,
    Animating,
    GameOver,
    MainMenu,
    PauseMenu
} GameState;

#endif

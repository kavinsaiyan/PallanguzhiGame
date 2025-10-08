#ifndef BOARD_H
#define BOARD_H

typedef enum e_BeadState
{
    InsideSlot,
    Animating,
    CollectedByPlayer1,
    CollectedByPlayer2,
    None
} BeadState;

typedef struct s_Bead
{
    BeadState state;
    Vector2 position;
} Bead;

typedef struct s_Slot
{
    Vector2 position;
} Slot;

typedef struct s_Board
{
    int player1Score;
    int player2Score;
    Slot slots[14];
    Bead beads[5 * 14];
} Board;
#endif

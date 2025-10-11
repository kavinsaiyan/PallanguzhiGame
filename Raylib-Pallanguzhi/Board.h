#include "raylib.h"
#include "Render.h"

#ifndef BOARD_H
#define BOARD_H

extern const int BOARD_Y_OFFSET;
extern const int SLOT_X_OFFSET;

extern const int TOTAL_SLOTS;
extern const int TOTAL_BEADS;

typedef enum
{
    InsideSlot,
    CollectedByPlayer1,
    CollectedByPlayer2,
    None
} BeadState;

typedef struct
{
    BeadState state;
    RenderState renderState;
    Vector2 position;
    int slotIndex;
} Bead;

typedef struct
{
    Vector2 position;
} Slot;

typedef struct
{
    int player1Score;
    int player2Score;
    Slot slots[14];
    Bead beads[70];
} Board;

void InitializeBoard(Board* board);

#endif

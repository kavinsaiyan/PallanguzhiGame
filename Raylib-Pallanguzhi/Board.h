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
    InsideBoard,
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
    Slot slots[14];
    Bead beads[70];
} Board;

void InitializeBoard(Board* board);

void SetBeadRenderStateInSlot(Board *board,int slotIndex, RenderState renderState);

int GetPlayer1Score(Board* board);

int GetPlayer2Score(Board* board);

#endif

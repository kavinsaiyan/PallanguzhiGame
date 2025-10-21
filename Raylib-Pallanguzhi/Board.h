#include "raylib.h"
#include "Render.h"
#include "Array.h"

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
    int currentMoveIndex;
} Board;

void InitializeBoard(Board* board);

void SetBeadRenderStateInSlot(Board *board,int slotIndex, RenderState renderState);

void MoveBeadTo(Board* board, int beadIndex);

int GetPlayer1Score(Board* board);

int GetPlayer2Score(Board* board);

void DrawBoard(Board* board, Texture2D* boardTexture, Texture2D* ballTexture);

Array* GetAllBeadsFrom(Board* board, int slotIndex);

int GetBeadCountInSlot(Board* board, int slotIndex);
#endif

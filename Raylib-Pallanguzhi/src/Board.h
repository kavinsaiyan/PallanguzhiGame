#include "raylib.h"
#include "Render.h"
#include "Array.h"

#ifndef BOARD_H
#define BOARD_H

extern const int BOARD_Y_OFFSET;
extern const int SLOT_X_OFFSET;

extern const int TOTAL_SLOTS;
extern const int TOTAL_BEADS;

extern const int SLOT_RADIUS;

extern const int INITIAL_BEADS_PER_SLOT;

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
    Vector2 beadCountTextPosition;
    int beadCount;
} Slot;

typedef struct
{
    Slot slots[14];
    Bead beads[70];
    int currentMoveIndex;
    int player1Score;
    int player2Score;
} Board;

void InitializeBoard(Board* board);

void SetBeadRenderStateInSlot(Board *board,int slotIndex, RenderState renderState);

void MoveBeadTo(Board* board, int beadIndex);

void UpdatePlayerScore(Board *board);

void DrawBoard(Board* board, Texture2D* boardTexture, Texture2D* ballTexture);

Array* GetAllBeadsFrom(Board* board, int slotIndex);

int GetBeadCountInSlot(Board* board, int slotIndex);

void AddBeadsToPlayer(Board* board,int playerIndex, int slotIndex);

Array* GetSlotsThatHaveBeads(Board* board, int startSlot, int endSlot);

int HasAnyPlayerWon(Board* board);

int CheckMouseHoverOnSlot(Board* board, Vector2 mousePosition);
#endif

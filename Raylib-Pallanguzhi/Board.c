#include "Board.h"
#include "Render.h"
#include "raylib.h"

// board draw position offset is { 0, 200 }
// first slot center is 70, 180 - index 0
// second slot center is 180, 180 - index 1
// last slot center is 70, 80 - index 13
const int SLOT_X_OFFSET = 110;
const int BORAD_Y_OFFSET = 200/2;

const int TOTAL_SLOTS = 14;
const int TOTAL_BEADS = 5 * 7 * 2;

void InitializeBoard(Board *board)
{
    for(int i=0; i < 8; i++)
    {
        board->slots[i].position.x = 70 + i * SLOT_X_OFFSET;
        board->slots[i].position.y = 180 + BORAD_Y_OFFSET;
    }

    for(int i=TOTAL_SLOTS; i >= 7; i--)
    {
        board->slots[i].position.x = 70 + (13 - i) * SLOT_X_OFFSET;
        board->slots[i].position.y = 280 + BORAD_Y_OFFSET;
    }

    //Initalize the bead positions
    for(int i=0; i < TOTAL_SLOTS; i++)
    {
        for(int j=i*5; j < i*5+5 && j < TOTAL_BEADS;j++)
        {
           board->beads[j].slotIndex = i;
           board->beads[j].renderState = Render;
           board->beads[j].position.x = board->slots[i].position.x + GetRandomValue(-20,20);
           board->beads[j].position.y = board->slots[i].position.y + GetRandomValue(-20,20);
        }
    }
}

void SetBeadRenderStateInSlot(Board *board,int slotIndex, RenderState renderState)
{
    for(int i =0; i < TOTAL_BEADS; i++)
    {
        if(board->beads[i].slotIndex == slotIndex)
            board->beads[i].renderState = renderState;
    }
}

void MoveBeadTo(Board* board, int beadIndex, int slotIndex)
{
    if(slotIndex < 0 || slotIndex >= TOTAL_SLOTS)
    {
        return;
    }
    if(beadIndex < 0 || beadIndex >= TOTAL_BEADS)
    {
        return;
    }

    board->beads[beadIndex].slotIndex = slotIndex;
    board->beads[beadIndex].position.x = board->slots[slotIndex].position.x + GetRandomValue(-20,20);
    board->beads[beadIndex].position.y = board->slots[slotIndex].position.y + GetRandomValue(-20,20);
}

int GetPlayer1Score(Board *board)
{
    int score = 0;
    for(int i=0;i<TOTAL_BEADS;i++)
    {
        if(board->beads[i].state == CollectedByPlayer1)
           score++;
    }
    return score;
}

int GetPlayer2Score(Board *board)
{
    int score = 0;
    for(int i=0;i<TOTAL_BEADS;i++)
    {
        if(board->beads[i].state == CollectedByPlayer2)
           score++;
    }
    return score;
}

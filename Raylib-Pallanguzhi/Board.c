#include "Board.h"

void InitializeBoard(Board *board, int slotXOffset, int boardYOffset)
{
    for(int i=0; i < 8; i++)
    {
        board->slots[i].position.x = 70 + i * slotXOffset;
        board->slots[i].position.y = 180 + boardYOffset;
    }

    for(int i=13; i >= 7; i--)
    {
        board->slots[i].position.x = 70 + (13 - i) * slotXOffset;
        board->slots[i].position.y = 280 + boardYOffset;
    }
}

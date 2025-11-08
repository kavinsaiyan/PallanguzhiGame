#include "Board.h"
#include "Render.h"
#include "raylib.h"
#include "raymath.h"
#include "Array.h"
#include "stddef.h"

// board draw position offset is { 0, 200 }
// first slot center is 70, 180 - index 0
// second slot center is 180, 180 - index 1
// last slot center is 70, 80 - index 13
const int SLOT_X_OFFSET = 110;
const int BORAD_Y_OFFSET = 200/2;

const int TOTAL_SLOTS = 14;
const int TOTAL_BEADS = 5 * 7 * 2;

const int SLOT_RADIUS = 44;

const int INITIAL_BEADS_PER_SLOT = 5;

void InitializeBoard(Board *board)
{
    board->player1Score = 0;
    board->player2Score = 0;

    for(int i=0; i < 8; i++)
    {
        board->slots[i].position.x = 70 + i * SLOT_X_OFFSET;
        board->slots[i].position.y = 176 + BORAD_Y_OFFSET;
        board->slots[i].beadCount = INITIAL_BEADS_PER_SLOT;
        board->slots[i].beadCountTextPosition.x = board->slots[i].position.x;
        board->slots[i].beadCountTextPosition.y = board->slots[i].position.y - 84;
    }

    for(int i=TOTAL_SLOTS; i >= 7; i--)
    {
        board->slots[i].position.x = 70 + (13 - i) * SLOT_X_OFFSET;
        board->slots[i].position.y = 282 + BORAD_Y_OFFSET;
        board->slots[i].beadCount = INITIAL_BEADS_PER_SLOT;
        board->slots[i].beadCountTextPosition.x = board->slots[i].position.x;
        board->slots[i].beadCountTextPosition.y = board->slots[i].position.y + 70;
    }

    //Initalize the bead positions
    for(int i=0; i < TOTAL_SLOTS; i++)
    {
        for(int j=i* INITIAL_BEADS_PER_SLOT; j < i * INITIAL_BEADS_PER_SLOT + INITIAL_BEADS_PER_SLOT && j < TOTAL_BEADS;j++)
        {
           board->beads[j].slotIndex = i;
           board->beads[j].renderState = Render;
           board->beads[j].state = InsideBoard;
           board->beads[j].position.x = board->slots[i].position.x + GetRandomValue(-20,20);
           board->beads[j].position.y = board->slots[i].position.y + GetRandomValue(-20,20);
        }
    }
}

void SetBeadRenderStateInSlot(Board *board,int slotIndex, RenderState renderState)
{
    for(int i =0; i < TOTAL_BEADS; i++)
    {
        if(board->beads[i].slotIndex == slotIndex && board->beads[i].state == InsideBoard)
            board->beads[i].renderState = renderState;
    }
}

void MoveBeadTo(Board* board, int beadIndex)
{
    int slotIndex = board->currentMoveIndex;
    if(slotIndex < 0 || slotIndex >= TOTAL_SLOTS)
    {
        TraceLog(LOG_ERROR, "[Board.c/MoveBeadTo]: current slot Index is not in range : %d",slotIndex);
        return;
    }
    if(beadIndex < 0 || beadIndex >= TOTAL_BEADS)
    {
        TraceLog(LOG_ERROR, "[Board.c/MoveBeadTo]: current bead Index is not in range : %d",beadIndex);
        return;
    }

    // reduce count in previous slot
    if(board->beads[beadIndex].slotIndex < 0 || board->beads[beadIndex].slotIndex >= TOTAL_SLOTS)
    {
        TraceLog(LOG_ERROR, "[Board.c/MoveBeadTo]: current slot Index is not in range : %d",board->beads[beadIndex].slotIndex);
        return;
    }
    else
        board->slots[board->beads[beadIndex].slotIndex].beadCount--;

    board->beads[beadIndex].slotIndex = slotIndex;
    board->beads[beadIndex].position.x = board->slots[slotIndex].position.x + GetRandomValue(-20,20);
    board->beads[beadIndex].position.y = board->slots[slotIndex].position.y + GetRandomValue(-20,20);

    // Move the current Move Index to next one
    board->currentMoveIndex = (int)Wrap(board->currentMoveIndex - 1,0,TOTAL_SLOTS);

    // Set Render state to render
    board->beads[beadIndex].renderState = Render;

    // Add count in next slot
    board->slots[slotIndex].beadCount++;
}

void AddBeadsToPlayer(Board* board,int playerIndex, int slotIndex)
{
    int count=0;
    for(int i=0;i<TOTAL_BEADS;i++)
    {
        if(board->beads[i].slotIndex == slotIndex && board->beads[i].state == InsideBoard)
        {
            count++;
            board->beads[i].state = (playerIndex == 0) ? CollectedByPlayer1 : CollectedByPlayer2;
            board->beads[i].renderState = DontRender;
            board->slots[slotIndex].beadCount--;
        }
    }
    TraceLog(LOG_INFO,"beads added are %d",count);
}

void UpdatePlayerScore(Board *board)
{
    int score1 = 0;
    int score2 = 0;
    for(int i=0;i<TOTAL_BEADS;i++)
    {
        if(board->beads[i].state == CollectedByPlayer1)
           score1++;
        else if(board->beads[i].state == CollectedByPlayer2)
           score2++;
    }
    board->player1Score = score1;
    board->player2Score = score2;
}

void DrawBoard(Board* board, Texture2D* boardTexture, Texture2D* ballTexture)
{
    DrawTexture(*boardTexture,0,200, WHITE);
    for(int i=0; i< TOTAL_BEADS; i++)
    {
        if(board->beads[i].renderState == Render)
        {
            DrawTexture(*ballTexture, board->beads[i].position.x - 16,board->beads[i].position.y - 16, WHITE);
        }
    }
    for(int i=0; i < TOTAL_SLOTS; i++)
    {
        DrawText(TextFormat("%d",board->slots[i].beadCount),board->slots[i].beadCountTextPosition.x,board->slots[i].beadCountTextPosition.y,16,BLACK);
    }
}

Array* GetAllBeadsFrom(Board* board, int slotIndex)
{
    int len = 0;
    for(int i =0; i < TOTAL_BEADS; i++)
    {
        if(board->beads[i].slotIndex == slotIndex && board->beads[i].state == InsideBoard)
            len++;
    }

    if(len == 0)
    {
        TraceLog(LOG_WARNING, "[Board.c/GetAllBeadsFrom]: Length is zero");
    }

    Array* arr = CreateArray(len);
    len = 0;

    for(int i =0; i < TOTAL_BEADS; i++)
    {
        if(board->beads[i].slotIndex == slotIndex && board->beads[i].state == InsideBoard)
        {
            arr->arr[len] = i;
            len++;
        }
    }

    return arr;
}

int GetBeadCountInSlot(Board* board, int slotIndex)
{
    int count = 0;
    
    if(slotIndex < 0 || slotIndex >= TOTAL_SLOTS)
    {
        TraceLog(LOG_ERROR,"[Board.c/GetBeadCountInSlot]: slot index is out of range!");
        return -1; 
    }


    for(int i=0; i < TOTAL_BEADS; i++)
    {
        if(board->beads[i].slotIndex == slotIndex && board->beads[i].state == InsideBoard)
        {
            count++;
        }
    }
    return count;
}

Array* GetSlotsThatHaveBeads(Board* board, int startSlot, int endSlot)
{
    if(startSlot < 0 || startSlot >= TOTAL_SLOTS)
    {
        TraceLog(LOG_ERROR, "The start slot index is out range:%d",startSlot);
        return NULL;
    }
    if(endSlot < 0 || endSlot >= TOTAL_SLOTS)
    {
        TraceLog(LOG_ERROR, "The end slot index is out range:%d",endSlot);
        return NULL;
    }

    if(startSlot >= endSlot)
    {
        TraceLog(LOG_ERROR, "The start slot : %d needs to be greater than end slot : %d",startSlot,endSlot); 
        return NULL;
    }
    int len = 0;
    int hashLength = endSlot - startSlot;
    int hash[hashLength];
    for(int i = 0; i < hashLength; i++)
        hash[i] = 0;

    for(int i=0; i < TOTAL_BEADS; i++)
    {
        if(board->beads[i].slotIndex >= startSlot && board->beads[i].slotIndex < endSlot
            && board->beads[i].state == InsideBoard)
        {
            hash[board->beads[i].slotIndex - startSlot]++;
        }
    }

    for(int i=0; i < hashLength; i++)
    {
        if(hash[i] > 0)
            len++;
    }

    Array* arr = CreateArray(len);
    len = 0;

    TraceLog(LOG_INFO, "len of hash is %d",arr->len);
    for(int i=0; i < hashLength; i++)
    {
        if(hash[i] > 0)
        {
            arr->arr[len] = i+startSlot;
            TraceLog(LOG_INFO, "bead cound > 0 in slot index :  %d",arr->arr[len]); 
            len++;
        } 
    }

    return arr;
}

int HasAnyPlayerWon(Board* board)
{
    int res = -1;
    
    //Get the total no of beads inside the board
    int total = 0;
    for(int i=0;i<TOTAL_BEADS;i++)
    {
       if(board->beads[i].state == InsideBoard)
            total++; 
    }

    //If the total no of beads is less than the difference in score between the player then one of them has won
    int diffInScore = board->player1Score - board->player2Score;
    diffInScore = diffInScore < 0 ? -diffInScore: diffInScore;
    if(diffInScore > total)
    {
        res = board->player1Score > board->player2Score ? 0 : 1;// 0 is player1 and 1 is player 2
    }

    return res;
}

int CheckMouseHoverOnSlot(Board* board, Vector2 mousePosition)
{
    for(int i = 0; i < TOTAL_SLOTS; i++)
    {
        if(CheckCollisionPointCircle(mousePosition, board->slots[i].position,SLOT_RADIUS))
        {
            return i;
        }
    }

    return -1;
}

#include "raylib.h"
#include "raymath.h"
#include "Board.h"
#include "Game.h"
#include "Render.h"
#include "SlotSelector.h"
#include "Queue.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 680;

    InitWindow(screenWidth, screenHeight, "Pallanguzhi");
    
    Texture2D ballTexture = LoadTexture("Resources/Sample Bead.png");
    Texture2D boardTexture = LoadTexture("Resources/Board.png");
    Texture2D slotSelectorTexture = LoadTexture(SELECTOR_TEXTURE_PATH);

    //Initialize Game State
    GameState gameState = Player1Turn;
    Queue* animQ = CreateQueue();

    //Initialize the board
    Board board;
    InitializeBoard(&board);

    //Player Input
    SlotSelector slotSelector;
    slotSelector.renderState = Render;

    //timer 
    float timer = 0;
    int indexOfBeadToMove;
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        float dt = GetFrameTime();

        // Get user input from keys for moving slotSelector
        if(IsKeyPressed(KEY_RIGHT)) slotSelector.currentIndex += 1;
        if(IsKeyPressed(KEY_LEFT)) slotSelector.currentIndex -= 1;
        slotSelector.currentIndex = (int)Wrap(slotSelector.currentIndex,0,TOTAL_SLOTS/2);
        
        // Get user input for actually selecting that slot
        if(IsKeyPressed(KEY_ENTER) && gameState == Player1Turn) 
        {
            gameState = Animating;
            SetBeadRenderStateInSlot(&board,slotSelector.currentIndex,DontRender);
            slotSelector.renderState = DontRender;
            // Add to Queue
            Array* arr = GetAllBeadsFrom(&board,slotSelector.currentIndex);
            EnqueueArray(animQ, arr->arr, arr->len);
            board.currentMoveIndex = slotSelector.currentIndex + 1;
            DestoryArray(arr);
        }

        switch(gameState)
        {
            case Player1Turn: break;
            case Player2Turn: break;
            case Animating:
                timer += dt;
                if(timer > 0.4f)
                {
                    timer = 0;
                    TraceLog(LOG_INFO,"next move index is %d",board.currentMoveIndex);
                    if(animQ->count > 0)
                    {
                        // move one bead to next slot
                        indexOfBeadToMove = Dequeue(animQ);
                        MoveBeadTo(&board,indexOfBeadToMove);

                        TraceLog(LOG_INFO,"Animqueue count is not zero");
                    }
                    else
                    {
                        int beads = GetBeadCountInSlot(&board, board.currentMoveIndex);
                        TraceLog(LOG_INFO,"Animqueue count is zero and beads in curmovindex is %d",beads);
                        if(beads != 0)
                        {
                            SetBeadRenderStateInSlot(&board,board.currentMoveIndex,DontRender);
                            // Add to Queue
                            Array* arr = GetAllBeadsFrom(&board,board.currentMoveIndex);
                            EnqueueArray(animQ, arr->arr, arr->len);
                            board.currentMoveIndex = board.currentMoveIndex + 1;
                            DestoryArray(arr);

                            TraceLog(LOG_INFO, "move intermediate end");
                        }
                        else
                        {
                            //Add the next slot score current player
                            
                            TraceLog(LOG_INFO, "move end");
                        }
                    }
                }
                break;
            case GameOver: break;
            case MainMenu: break;
            case PauseMenu: break;
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawBoard(&board, &boardTexture, &ballTexture);
        DrawSlotSelector(&slotSelector, &slotSelectorTexture, board.slots[slotSelector.currentIndex].position);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    DestroyQueue(animQ);

    UnloadTexture(ballTexture);
    UnloadTexture(boardTexture);
    UnloadTexture(slotSelectorTexture);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

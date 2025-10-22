#include "raylib.h"
#include "raymath.h"
#include "Board.h"
#include "Render.h"
#include "SlotSelector.h"
#include "Queue.h"
#include "Game.h"

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
    GameState gameState = PlayerMove;
    PlayerTurn playerTurn = Player1Turn;
    Queue* animQ = CreateQueue();

    //Initialize the board
    Board board;
    InitializeBoard(&board);

    //Player Input
    SlotSelector slotSelector;
    slotSelector.currentIndex =(int)(TOTAL_SLOTS * 0.75f);
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
        if(IsKeyPressed(KEY_RIGHT)) slotSelector.currentIndex -= 1;
        if(IsKeyPressed(KEY_LEFT)) slotSelector.currentIndex += 1;
        slotSelector.currentIndex = (int)Wrap(slotSelector.currentIndex,TOTAL_SLOTS/2,TOTAL_SLOTS);
        
        // Get user input for actually selecting that slot
        if(IsKeyPressed(KEY_ENTER) && gameState == PlayerMove && playerTurn == Player1Turn) 
        {
            gameState = Animating;
            slotSelector.renderState = DontRender;
            StartMove(&board,animQ,slotSelector.currentIndex);
        }

        switch(gameState)
        {
            case PlayerMove: 
                if(playerTurn == Player2Turn)
                {
                    //AI move range (0,TOTAL_SLOTS/2)
                    //Need to know all indices that have beads in them
                    Array* arr = GetSlotsThatHaveBeads(&board,0,TOTAL_SLOTS/2);
                    //for now choose randomly
                    int rand = GetRandomValue(0,arr->len);
                    board.currentMoveIndex = arr->arr[rand];
                    TraceLog(LOG_INFO,"AI chooses %d slot index randomly", arr->arr[rand]);
                    DestroyArray(arr);
                    //Change Gamestate
                    gameState = PauseMenu;
                    //StartMove(&board,animQ,board.currentMoveIndex);
                }
                break;
            case Animating:
                timer += dt;
                if(timer > 0.1f)
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
                            Array* tempArr = GetAllBeadsFrom(&board,board.currentMoveIndex);
                            EnqueueArray(animQ, tempArr->arr, tempArr->len);
                            board.currentMoveIndex = (int)Wrap(board.currentMoveIndex - 1,0,TOTAL_SLOTS);
                            DestroyArray(tempArr);

                            TraceLog(LOG_INFO, "move intermediate end");
                        }
                        else
                        {
                            //Add the next slot score current player
                            AddBeadsToPlayer(&board,playerTurn,board.currentMoveIndex-1);
                            UpdatePlayerScore(&board);
                            playerTurn = playerTurn == Player1Turn ? Player2Turn : Player1Turn;
                            gameState = PlayerMove;
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
        DrawText(TextFormat("Player 1 Score : %d",board.player1Score),0,100,16,BLACK);
        DrawText(TextFormat("Player 2 Score : %d",board.player2Score),0,120,16,BLACK);

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

void StartMove(Board* board, Queue* animQ, int currentIndex)
{
    SetBeadRenderStateInSlot(board,currentIndex,DontRender);
    Array* arr = GetAllBeadsFrom(board,currentIndex);
    EnqueueArray(animQ, arr->arr, arr->len);
    board->currentMoveIndex = (int)Wrap(currentIndex - 1,0,TOTAL_SLOTS);
    DestroyArray(arr);
}

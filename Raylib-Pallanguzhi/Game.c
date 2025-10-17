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
    Queue animQ = CreateQueue();

    //Initialize the board
    Board board;
    InitializeBoard(&board);

    //Player Input
    SlotSelector slotSelector;
    slotSelector.renderState = Render;

    //timer 
    float timer = 0;
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    TraceLog(LOG_INFO, "logging using trace log");

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
            
        }

        switch(gameState)
        {
            case Player1Turn: break;
            case Player2Turn: break;
            case Animating:
                timer += dt;
                if(timer > 0.4f)
                {
                    // move one bead to next slot
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
    UnloadTexture(ballTexture);
    UnloadTexture(boardTexture);
    UnloadTexture(slotSelectorTexture);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

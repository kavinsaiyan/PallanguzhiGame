#include "raylib.h"
#include "raymath.h"
#include "Board.h"
#include "Game.h"
#include "Render.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 680;

    InitWindow(screenWidth, screenHeight, "Pallanguzhi");
    
    Texture2D ballTexture = LoadTexture("Resources/Sample Bead.png");
    Texture2D boardTexture = LoadTexture("Resources/Board.png");

    //Initialize the board
    Board board;
    InitializeBoard(&board);
    
    //TODO: Set a sample distribution of beads inside the slot, use GetRandomValue(int min, int max), it is available in raylib
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        // float dt = GetFrameTime();

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawTexture(boardTexture,0,200, WHITE);
            for(int i=0; i< TOTAL_SLOTS; i++)
                DrawCircleLinesV(board.slots[i].position, 16,BLACK);

            for(int i=0; i< TOTAL_BEADS; i++)
            {
                if(board.beads[i].renderState == Render)
                {
                    DrawTexture(ballTexture, board.beads[i].position.x - 16,board.beads[i].position.y - 16, WHITE);
                }
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(ballTexture);
    UnloadTexture(boardTexture);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

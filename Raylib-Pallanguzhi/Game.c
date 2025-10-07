#include "raylib.h"
#include "raymath.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 680;

    InitWindow(screenWidth, screenHeight, "Pallanguzhi");
    
    Texture2D ballTexture = LoadTexture("Resources/Sample Bead.png");
    Texture2D boardTexture = LoadTexture("Resources/Board.png");

    // board draw position offset is { 0, 200 }
    // first slot center is 70, 180 - index 0
    // second slot center is 180, 180 - index 1
    // last slot center is 70, 80 - index 13
    
    const int boardYOffset = 200/2;
    Vector2 ballPosition = { 70, 180 + boardYOffset };
    Vector2 ballStartPosition = { 70, 180 + boardYOffset  };
    Vector2 ballEndPosition = { 180, 180 + boardYOffset };
    
    //TODO: Define the board data
    //TODO: Set a sample distribution of beads inside the slot
    
    float t = 0;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        float dt = GetFrameTime();
        
        t += dt;
        if(t > 0 && t < 1)
        {
			ballPosition.x = Lerp(ballStartPosition.x, ballEndPosition.x, t);
			ballPosition.y = Lerp(ballStartPosition.y, ballEndPosition.y, t);
		}

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawTexture(boardTexture,0,200, WHITE);
            DrawTextureV(ballTexture, ballPosition, WHITE);
			DrawCircleLinesV(ballPosition, 16,BLACK);

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

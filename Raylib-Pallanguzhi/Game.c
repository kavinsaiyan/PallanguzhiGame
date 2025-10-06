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

    // first slot center is 70, 180
    // second slot center is 180, 180
    // last slot center is 70, 80 - index 13

    Vector2 ballPosition = { 100, 100 };
    Vector2 ballStartPosition = { 100, 100 };
    Vector2 ballEndPosition = { 150, 200 };
    
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
			DrawCircleLinesV(ballPosition, 16,WHITE);

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

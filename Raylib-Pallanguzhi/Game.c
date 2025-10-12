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
    Texture2D slotSelectorTexture = LoadTexture("Resources/SlotSelector.png");

    //Initialize Game State
    GameState gameState = Player1Turn;

    //Initialize the board
    Board board;
    InitializeBoard(&board);

    //Player Input
    int highlightedSlotIndex = 0;
    bool player1SelectedSlot = false;
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        // float dt = GetFrameTime();

        // Get user input from keys for moving slotSelector
        if(IsKeyPressed(KEY_RIGHT)) highlightedSlotIndex += 1;
        if(IsKeyPressed(KEY_LEFT)) highlightedSlotIndex -= 1;
        highlightedSlotIndex = (int)Wrap(highlightedSlotIndex,0,TOTAL_SLOTS/2);
        
        // Get user input for actually selecting that slot
        if(IsKeyPressed(KEY_ENTER) && gameState == Player1Turn) 
        {
            player1SelectedSlot = true;
            gameState = Animating;
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTexture(boardTexture,0,200, WHITE);
        for(int i=0; i< TOTAL_BEADS; i++)
        {
            if(board.beads[i].renderState == Render)
            {
                DrawTexture(ballTexture, board.beads[i].position.x - 16,board.beads[i].position.y - 16, WHITE);
            }
        }

        switch(gameState)
        {
            case Player1Turn:
                //Draw the selection Texture
                int yOffset = highlightedSlotIndex < TOTAL_SLOTS / 2 ? 4 : -2;
                if(highlightedSlotIndex >= 0 && highlightedSlotIndex < TOTAL_SLOTS)
                    DrawTexture(slotSelectorTexture,board.slots[highlightedSlotIndex].position.x - 50, board.slots[highlightedSlotIndex].position.y-50-yOffset,BLUE);
                break;
            case Player2Turn: break;
            case Animating: break;
            case GameOver: break;
            case MainMenu: break;
            case PauseMenu: break;
        }
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
